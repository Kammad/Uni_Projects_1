// Word puzzle.
//
// Dmitry Smirnov 16.03.2023
#include <iostream>
#include <cstring>
#include <iomanip>
using namespace std;

// The structure Word used to store words as well as their orientation in the matrix and the coordinates of the starting point.
struct Word
{
    char letters[20];
    int row;
    int col;
    char orientation[20];
};

void DisplayPuzzle(char letterlist[][20], int rowcol);
void FillPuzzle(char letterlist[][20], int rowcol);
void FillWords(Word wlist[], int wnum);
void PuzzleScan(Word word[], char letterlist[][20], int rowcol, int wnum);
int LineSearch(char[], Word);
void ClearLine(char line[], int rowcol);
void DisplayResults(Word words[], int wnum, int rowcol);
int CstringLength(char[]);

// Main function to test the program.
int main()
{
    char puzzle[20][20], new_word[20];
    int choice = 0, rowcol = 0, wnum = 0;
    const int MAX_ROWCOL = 20;
    Word wordlist[40];

    do
    {
        cout << "\nMenu:" << endl;
        cout << "-------------------------------------------" << endl;
        cout << "1. Fill in the puzzle" << endl;
        cout << "2. Display the puzzle" << endl;
        cout << "3. Enter the words to look for" << endl;
        cout << "4. Start the search" << endl;
        cout << "0. Exit" << endl;
        cout << "\nEnter your choice (0-4): ";

        cin >> choice;

        switch (choice)
        {
        case 1:
            do {
            cout << "\nEnter the number of rows and columns: ";
            cin >> rowcol;
            if (rowcol > MAX_ROWCOL) {
                cout << "\nError. Please, enter a number under " << MAX_ROWCOL << endl;
            }
            if (rowcol <= 0) {
                cout << "\nError. Please, enter a positive value." << endl;
            }
            } while (rowcol <= 0 || rowcol > MAX_ROWCOL);

            cout << "\nEnter the characters (" << rowcol << " per row): ";
            FillPuzzle(puzzle, rowcol);

            break;

        case 2:
            if (rowcol == 0)
            {
                cout << "\nYou should enter the puzzle first in order to display it." << endl;
            }
            else
            {
                DisplayPuzzle(puzzle, rowcol);
            }
            break;

        case 3:
            do
            {
                cout << "\nEnter the number of words to look for: ";
                cin >> wnum;

                if (wnum > 40 || wnum <= 0)
                {
                    cout << "\nError. The number of words should be between 0 and 40." << endl;
                }

            } while (wnum > 40 || wnum <= 0);

            cout << "\nEnter the " << wnum << " words:" << endl;
            FillWords(wordlist, wnum);

            for (int z = 0; z < wnum; z++) {
                wordlist[z].orientation[0] = '\0';
            }
            break;

        case 4:
            if (rowcol == 0)
            {
                cout << "\nError. Please, fill in the puzzle first." << endl;
            }
            if (wnum == 0)
            {
                cout << "\nError. Please, state the words to look for." << endl;
            }
            if (wnum > 0 && rowcol > 0)
            {
                PuzzleScan(wordlist, puzzle, rowcol, wnum);
            }
            break;

        case 0:
            cout << "\nExiting program." << endl;
            break;

        default:
            cout << "\nError. Please, choose the number between 0 and 4." << endl;
        }
        cin.ignore();
    } while (choice != 0);

    return 0;
}

// This function returns the number of characters in the cstring before the null character.
// It was created because the in-built function strlen did not work properly.
int CstringLength(char str[])
{
    for (int i = 0; i <= 20 || str[i] != '\0'; i++)
    {
        if (str[i] == '\0')
        {
            return i;
        }
    }
    return -1;
}

// When this function is called, the user is asked to enter the puzzle letters row by row.
void FillPuzzle(char letterlist[][20], int rowcol)
{
    for (int row = 0; row < rowcol; row++)
    {
        for (int col = 0; col < rowcol; col++)
        {
            cin >> letterlist[row][col];
        }
    }
}

// This functions displays the matrix of the puzzle in the console.
void DisplayPuzzle(char letterlist[][20], int rowcol)
{
    for (int row = 0; row < rowcol; row++)
    {
        for (int col = 0; col < rowcol; col++)
        {
            cout << letterlist[row][col] << "  ";
        }
        cout << endl;
    }
}

// When this function is called, the user is asked to enter the words to be looked for in the puzzle matrix.
void FillWords(Word wlist[], int wnum)
{
    for (int i = 0; i < wnum; i++)
    {
        cin >> wlist[i].letters;
    }
}

// This function searches the cstring and searches for a word as its substring. It does this in both directions.
// The output of this function returns the index of the letter in the string and the direction of reading.
// E.g. -3 = Third letter, read backwards. 1 = Letter under index 1, read forwards.
int LineSearch(char p1[], Word word)
{
    bool found;

    if (CstringLength(word.letters) <= CstringLength(p1))
    {
        for (int letterline = 0; letterline < CstringLength(p1); letterline++)
        {
            if (p1[letterline] == word.letters[0])
            {
                found = true;

                for (int letterw = 0; letterw < CstringLength(word.letters); letterw++)
                {
                    if (!(p1[letterline + letterw] == word.letters[letterw]))
                    {
                        found = false;
                    }
                    if ((letterline + letterw) >= CstringLength(p1) || letterw >= (CstringLength(p1) - letterline))
                    {
                        found = false;
                    }
                }

                if (found == true)
                {
                    return letterline;
                }
            }
        }

        for (int letterline = 0; letterline < CstringLength(p1); letterline++)
        {
            if (p1[letterline] == word.letters[0])
            {
                found = true;

                for (int letterw = 0; letterw < CstringLength(word.letters); letterw++)
                {
                    if (!(p1[letterline - letterw] == word.letters[letterw]))
                    {
                        found = false;
                    }
                    if ((letterline - letterw) < 0 || letterw > letterline)
                    {
                        found = false;
                    }
                }

                if (found == true)
                {
                    return -letterline;
                }
            }
        }
    }

    return -21;
}

// This function replaces all characters in the cstring with null character.
void ClearLine(char line[], int rowcol)
{
    for (int k = 0; k < rowcol + 1; k++)
    {
        if (k <= 20)
        {
            line[k] = '\0';
        }
    }
}

// This function collects all the possible the strings from the matrix and forces them into the function LineSearch. Then it accepts the result
// and based on this result fills in the data in the structure Word of each word in the array words.
// At the end it calls the function to display the results.
void PuzzleScan(Word words[], char letterlist[][20], int rowcol, int wnum)
{
    char line[20];
    int reply, i = 0, j = 0, initial_i = 0, initial_j = 0;

    ClearLine(line, rowcol);

    // Rows
    for (int row = 0; row < rowcol; row++)
    {
        for (int col = 0; col < rowcol; col++)
        {
            line[col] = letterlist[row][col];
        }
        for (int z = 0; z < wnum; z++)
        {

            reply = LineSearch(line, words[z]);
            if (reply != -21)
            {

                words[z].row = row;
                if (reply >= 0)
                {
                    words[z].col = reply;
                    strcpy(words[z].orientation, "Horizontally Right");
                }
                else
                {
                    words[z].col = -1 * reply;
                    strcpy(words[z].orientation, "Horizontally Left");
                }
                cout << "\nRows " << line << " " << reply << " " << words[z].letters << endl;
            }
        }
        ClearLine(line, rowcol);
    }

    // Columns
    for (int col = 0; col < rowcol; col++)
    {
        for (int row = 0; row < rowcol; row++)
        {
            line[row] = letterlist[row][col];
        }
        for (int z = 0; z < wnum; z++)
        {

            reply = LineSearch(line, words[z]);
            if (reply != -21)
            {
                words[z].col = col;
                if (reply >= 0)
                {
                    words[z].row = reply;
                    strcpy(words[z].orientation, "Vertically Down");
                }
                else
                {
                    words[z].row = -1 * reply;
                    strcpy(words[z].orientation, "Vertically Up");
                }
            }
        }
        ClearLine(line, rowcol);
    }

    // Left bottom to right top
    for (int counter = 0; counter < rowcol; counter++)
    {
        initial_i = 0;
        initial_j = counter;
        i = 0;
        j = counter;

        while ((i < rowcol && i >= 0) && (j < rowcol && j >= 0))
        {
            line[i] = letterlist[i][j];
            i++;
            j++;
        }

        for (int z = 0; z < wnum; z++)
        {
            reply = LineSearch(line, words[z]);

            if (reply != -21)
            {
                if (reply >= 0) {
                    words[z].row = initial_i + reply;
                    words[z].col = initial_j + reply; 
                    strcpy(words[z].orientation, "Left Top to Right Bottom");
                } else {
                    words[z].row = initial_i - reply;
                    words[z].col = initial_i - reply;
                    strcpy(words[z].orientation, "Right Bottom to Left Top");
                }
            }
        }
        ClearLine(line, rowcol);
    }

    for (int counter = rowcol - 1; counter >= 0; counter--)
    {
        initial_i = counter;
        initial_j = 0;
        i = counter;
        j = 0;

        while ((i < rowcol && i >= 0) && (j < rowcol && j >= 0))
        {
            line[j] = letterlist[i][j];
            i++;
            j++;
        }

        for (int z = 0; z < wnum; z++)
        {

            reply = LineSearch(line, words[z]);
            if (reply != -21)
            {
                if (reply >= 0) {
                    words[z].row = initial_i + reply;
                    words[z].col = initial_j + reply;
                    strcpy(words[z].orientation, "Left Top to Right Bottom");
                } else {
                    words[z].row = initial_i - reply;
                    words[z].col = initial_j - reply;
                    strcpy(words[z].orientation, "Right Bottom to Left Top");
                }
            }
        }
        ClearLine(line, rowcol);
    }

    // Left top to right bottom
    for (int counter = 0; counter < rowcol; counter++)
    {
        initial_i = 0;
        initial_j = counter;
        i = 0;
        j = counter;

        while ((i < rowcol && i >= 0) && (j < rowcol && j >= 0))
        {
            line[i] = letterlist[i][j];
            i++;
            j--;
        }

        for (int z = 0; z < wnum; z++)
        {
            reply = LineSearch(line, words[z]);

            if (reply != -21)
            {
                if (reply >= 0) {
                    words[z].row = initial_i + reply;
                    words[z].col = initial_j - reply; 
                    strcpy(words[z].orientation, "Right Top to Left Bottom");
                } else {
                    words[z].row = initial_i - reply;
                    words[z].col = initial_i + reply;
                    strcpy(words[z].orientation, "Left Bottom to Right Top");
                }
            }
        }
        ClearLine(line, rowcol);
    }

    for (int counter = rowcol - 1; counter >= 0; counter--)
    {
        initial_i = counter;
        initial_j = rowcol - 1;
        i = counter;
        j = rowcol - 1;

        while ((i < rowcol && i >= 0) && (j < rowcol && j >= 0))
        {
            line[rowcol - 1 - i] = letterlist[i][j];
            i++;
            j--;
        }

        for (int z = 0; z < wnum; z++)
        {
            reply = LineSearch(line, words[z]);
            if (reply != -21)
            {
                if (reply >= 0) {
                    words[z].row = initial_i - reply;
                    words[z].col = initial_j + reply; 
                    strcpy(words[z].orientation, "Left Bottom to Right Top");
                } else {
                    words[z].row = initial_i + reply;
                    words[z].col = initial_i - reply;
                    strcpy(words[z].orientation, "Right Top to Left Bottom");
                }
            }
        }
        ClearLine(line, rowcol);
    }
    
    DisplayResults(words, wnum, rowcol);
}

// This function gathers the data from the words array and displays it in a table.
void DisplayResults(Word words[], int wnum, int rowcol) {
    cout << "\nWORDS" << endl;
    cout << "*****" << endl;
    cout << "\n";

    for (int z = 0; z < wnum; z++) {
        if (words[z].orientation[0] != '\0') {
            cout << setw(rowcol) << words[z].letters << "                " << "FOUND (Row: " << words[z].row << ", Column: " << words[z].col << ", " << words[z].orientation << endl;
        } else {
            cout << setw(rowcol) << words[z].letters << "                " << "NOT FOUND" << endl;
        }
    }
}
