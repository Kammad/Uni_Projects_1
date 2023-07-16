// Project 2
// Interactive Library. 
// Dmitry Smirnov, 20/04/2023
#include <iostream>
#include <cstring>
using namespace std;

const int MAX_CUSTOMERS = 100;
const int MAX_BOOKS = 100;

struct Book {
    char Title[100];
    char Author[50];
    int Year;
    char ISBN[30];
    bool On_loan;
    bool On_reserve;
    char Borrower_ID[8];
};

struct Customer {
    char Name[50];
    char Address[100];
    char ID[8];
    Book Books[5];
    int Books_num;
};

struct Date {
    int Day;
    int Month;
    int Year;
};

// This struct is used to connect the customer to the book he or she borrowed. 
// It is also used for logging purposes.
struct Receipt {
    Customer Borrower;
    Book Borrowed_book;
    Date Due_date;
    double Fine;
    bool Returned;
    bool Reserved;
};

void sortCustomers(Customer[], int);
void printDetails(Customer[], char[], int);
int findCustomer(Customer[], Customer, int);
bool inCustomers(Customer[], Customer, int);
void printBooks(Book[], int);
bool dateValidate(char[]);
bool idValidate(char[]);
bool isbnValidate(char[]);
void displayUserReceipts(Receipt[], Customer, int);
void returnBook(Customer, Receipt, Book);
void calculateFine(Receipt, Date, int[]);

int main() {
    int menu_choice, book_choice, total_receipts = 0, customers_num = 0, books_num = 0, selected_customer = -1, chardif = '0';
    int days_in_month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool customer_exists = false, valid_date, receipt_found = false;
    char user_input_id[30], parsedate[11], reserve_input, isbn_input[30];
    double total_fine;
    Customer Customers[MAX_CUSTOMERS], new_customer;
    Book Books[MAX_BOOKS];
    Receipt All_receipts[MAX_BOOKS];
    Date Input_date;

    // Loop for the menu
    do {
        cout << "\nMenu: " << endl;
        cout << "---------------------------------------------" << endl;
        cout << "1. Enter a new customer" << endl;
        cout << "2. Enter a new book" << endl;
        cout << "3. Return the number of customers" << endl;
        cout << "4. Find customer" << endl;
        cout << "5. Print Details of customer" << endl;
        cout << "6. Borrow book(s)" << endl;
        cout << "7. Reserve book(s)" << endl;
        cout << "8. Return book" << endl;
        cout << "9. Reports" << endl;
        cout << "10. Calculate and print fines" << endl;
        cout << "0. Exit" << endl;

        cout << "\nEnter your choice: ";
        cin >> menu_choice;


        switch (menu_choice) {
            case 1:
                cout << "\nNew customer" << endl;
                cout << "---------------------------------------------" << endl;
                cout << "\nEnter the name: ";

                cin.ignore();
                cin.getline(new_customer.Name, 50);
                cout << "\nEnter the address (up to 100 characters): ";
                cin.getline(new_customer.Address, 100);
                
                // ID validation
                do {
                    cout << "\nEnter the ID (U******): ";
                    cin.getline(new_customer.ID, 30);
                    
                    if (idValidate(new_customer.ID) == false) {
                        cout << "\nError. Please, use the form U****** for id, where * is a digit." << endl;
                    }

                } while (idValidate(new_customer.ID) == false);

                // Also ID validation, it checks for whether this ID exists
                if (findCustomer(Customers, new_customer, customers_num) != -1) {
                    customer_exists = true;
                }

                if (customer_exists == false) {
                    Customers[customers_num] = new_customer;
                    Customers[customers_num].Books_num = 0;

                    cout << "---------------------------------------------" << endl;
                    cout << "New customer has been added: " << endl;
                    cout << Customers[customers_num].Name << endl;
                    cout << Customers[customers_num].Address << endl;
                    cout << Customers[customers_num].ID << endl;
                    cout << "\n" << endl;

                    customers_num++;

                    if (customers_num > 1) {
                        sortCustomers(Customers, customers_num);
                    }
                } else {
                    cout << "\nError. This customer is already added. Please, retry." << endl;
                    customer_exists = false;
                }              


                break;

            case 2:
                cout << "\nNew book" << endl;
                cout << "---------------------------------------------" << endl;
                cout << "\nEnter the title (up to 100 characters): ";
                cin.ignore();
                cin.getline(Books[books_num].Title, 100);
                cout << "\nEnter the name of the author: ";
                cin.getline(Books[books_num].Author, 50);
                cout << "\nEnter the year of publication: ";
                cin >> Books[books_num].Year;
                cin.ignore();

                // ISBN validation
                do {
                    cout << "\nEnter the ISBN of the book: ";
                    cin.getline(Books[books_num].ISBN, 30);

                    if (isbnValidate(Books[books_num].ISBN) == false) {
                        cout << "\nError. ISBN can only contain digits and hyphens. " << endl;
                    }

                } while(isbnValidate(Books[books_num].ISBN) == false);

                Books[books_num].On_loan = false;
                Books[books_num].On_reserve = false;
                Books[books_num].Borrower_ID[0] = '\0';
                books_num++;
                
                break;

            case 3:
                cout << "\nNumber of customers" << endl;
                cout << "---------------------------------------------" << endl;
                cout << "\nNumber of current customers: " << customers_num << endl;
                break;

            // Does not print the details of customer. It only tells whether the customer is in the system.
            case 4:
                cout << "\nFind customer" << endl;
                cout << "---------------------------------------------" << endl;
                cout << "\nEnter the ID of the customer: ";
                cin.ignore();
                cin.getline(new_customer.ID, 8);

                if (inCustomers(Customers, new_customer, customers_num)) {
                    cout << "\nThis customers exists. " << endl;
                } else {
                    cout << "\nThere is no such customer." << endl;
                }

                break;

            //Prints the details of the customers, but also uses the function from menu choice 4
            case 5:
                cout << "\nPrint details of customer" << endl;
                cout << "---------------------------------------------" << endl;
                cout << "\nEnter the ID of the customer: ";
                cin.ignore();
                cin.getline(new_customer.ID, 8);

                if (inCustomers(Customers, new_customer, customers_num)) {
                    cout << "\n";
                    printDetails(Customers, new_customer.ID, customers_num);
                } else {
                    cout << "\nError. There is no such customer with ID: " << new_customer.ID << endl;
                }
                break;

            case 6:
                cout << "\nBorrow books" << endl;
                cout << "---------------------------------------------" << endl;
                cout << "\nEnter the ID of the customer: ";

                cin.ignore();
                cin.getline(new_customer.ID, 8);

                if (inCustomers(Customers, new_customer, customers_num)) {
                    selected_customer = findCustomer(Customers, new_customer, customers_num);

                    // Prints all books in the database by their index number and asks for a book to borrow.
                    do {
                        cout << "\nList of books: " << endl;
                        cout << "---------------------------------------------\n" << endl;

                        printBooks(Books, books_num);

                        cout << "\nSelect the book to borrow (-1 to finish): ";

                        do {
                        cin >> book_choice;
                        cin.ignore();

                        if (book_choice >= books_num) {
                            cout << "\nError. Please, enter an existing number for the book: ";
                        }
                        
                        } while (book_choice != -1 && book_choice >= books_num);

                        if (book_choice != -1 && book_choice >= 0 && book_choice <= books_num) {
                            if (Books[book_choice].On_loan == false) {
                                do {
                                    valid_date = true;

                                    cout << "\nEnter a date due to return the book (in the form  DD/MM/YYYY): ";
                                    parsedate[0] = '\0';
                                    
                                    cin.getline(parsedate, 11);
                                    cout << parsedate << endl;

                                    if (dateValidate(parsedate) == true) {
                                        Input_date.Day = (parsedate[0]-chardif)*10 + parsedate[1]-chardif;
                                        Input_date.Month = (parsedate[3]-chardif)*10 + parsedate[4]-chardif;
                                        Input_date.Year = (parsedate[6]-chardif)*1000 + (parsedate[7]-chardif)*100 + (parsedate[8]-chardif)*10 + parsedate[9]-chardif;
                                        
                                        // Date validation check. Ensures that the date is not out of bounds
                                        if (Input_date.Day > 31) {
                                            cout << "\nError. There cannot be more than 31 days in a month. Please, enter a new date." << endl;
                                            valid_date = false;

                                        } else if (Input_date.Day == 0 || Input_date.Month == 0 || Input_date.Year == 0) {
                                            cout << "\nError. None of the date numbers can be equal to 0." << endl;
                                            valid_date = false;

                                        } else if ((Input_date.Month > 12)) {
                                            cout << "\nError. There can only be 12 months in a year. Please, enter another date." << endl;
                                            valid_date = false;

                                        } else if (Input_date.Day > days_in_month[Input_date.Month]) {
                                            cout << "\nError. There cannot be more than " << days_in_month[Input_date.Month] << " days in this month. " << endl;
                                            valid_date = false;

                                        } else if ((Input_date.Month == 2 && Input_date.Year % 4 != 0) && Input_date.Day > 28) {
                                            cout << "\nError. Year " << Input_date.Year << " is not a leap year. The maximum number of days in February is 28." << endl;
                                            valid_date = false;

                                        } 

                                    } else {
                                        cout << "\nError. The date must be in the form DD/MM/YYYY. Please, enter again." << endl;
                                        valid_date = false;
                                    }
                                    
                                } while(valid_date == false);

                                
                                // 5 books limit check
                                if (Customers[selected_customer].Books_num == 5) {
                                    cout << "\nSorry. One user may only borrow no more than 5 books. Please, return one book to take another one." << endl;
                                } else {
                                    Customers[selected_customer].Books_num++;
                                    Customers[selected_customer].Books[Customers[selected_customer].Books_num] = Books[book_choice];
                                    Books[book_choice].On_loan = true;
                                    strcpy(Books[book_choice].Borrower_ID, Customers[selected_customer].ID);

                                    All_receipts[total_receipts].Borrowed_book = Books[book_choice];
                                    All_receipts[total_receipts].Borrower = Customers[selected_customer];
                                    All_receipts[total_receipts].Due_date = Input_date;
                                    All_receipts[total_receipts].Fine = 0;
                                    All_receipts[total_receipts].Returned = false;
                                    All_receipts[total_receipts].Reserved = false;

                                    total_receipts++;
                                }

                            } else {
                                // Offers to reserve the book
                                if (Books[book_choice].On_reserve == false) {
                                    cout << "\nSorry. This books is already taken. Do you wish to reserve it? (y/n): ";

                                    do {
                                        cin >> reserve_input;

                                        if (reserve_input == 'y') {
                                            Books[book_choice].On_reserve == true;

                                            All_receipts[total_receipts].Borrowed_book = Books[book_choice];
                                            All_receipts[total_receipts].Borrower = Customers[selected_customer];
                                            All_receipts[total_receipts].Fine = 0;
                                            All_receipts[total_receipts].Returned = false;
                                            All_receipts[total_receipts].Reserved = true;

                                            total_receipts++;

                                            cout << "\nThe book \"" << Books[book_choice].Title << "\" is successfully reserved. " << endl;
                                        } else if (reserve_input == 'n') {
                                            cout << "\nYou can select another book. " << endl;
                                        } else {
                                            cout << "\nError. Only y and n replies are available. Please, enter your choice again: " << endl;
                                        }

                                    } while (reserve_input != 'y' && reserve_input != 'n');
                                } else {
                                    cout << "\nSorry. This book is already taken and reserved. You may ask again later." << endl;
                                }
                            }
                        } else if (book_choice == -1) {
                            cout << "\nExiting. Your receipts are: " << endl;
                            cout << "---------------------------------------------" << endl;

                            displayUserReceipts(All_receipts, Customers[selected_customer], total_receipts);
                            cout << "\nTotal borrowed books: " << Customers[selected_customer].Books_num << endl;
                        } else {
                            cout << "\nError. Please, enter a correct choice for a book or -1 to exit." << endl;
                        }
                    } while (book_choice != -1);

                } else {
                    cout << "\nError. There is no such customer with ID: " << new_customer.ID << endl;
                }

                break;

            case 7:
                cout << "\nReserve books" << endl;
                cout << "---------------------------------------------\n" << endl;

                cout << "\nEnter the ID of the customer: ";

                cin.ignore();
                cin.getline(new_customer.ID, 8);

                // Validation by ID
                if (inCustomers(Customers, new_customer, customers_num)) {
                    selected_customer = findCustomer(Customers, new_customer, customers_num);

                    do {
                        cout << "\nList of books: " << endl;
                        cout << "---------------------------------------------\n" << endl;

                        printBooks(Books, books_num);

                        cout << "\nSelect the book to reserve (-1 to finish): ";
                        cin >> book_choice;
                        cin.ignore();

                        if (book_choice != -1 && book_choice >= 0 && book_choice <= books_num) {
                            if (Books[book_choice].On_reserve == false) {
                                Books[book_choice].On_reserve == true;

                                All_receipts[total_receipts].Borrowed_book = Books[book_choice];
                                All_receipts[total_receipts].Borrower = Customers[selected_customer];
                                All_receipts[total_receipts].Fine = 0;
                                All_receipts[total_receipts].Returned = false;
                                All_receipts[total_receipts].Reserved = true;

                                total_receipts++;
                                cout << "\nThe book \"" << Books[book_choice].Title << "\" is successfully reserved." << endl;
                            }
                        } else {
                            cout << "\nError. Please, enter a correct choice for a book or -1 to exit." << endl;
                        }

                    } while (book_choice != -1);

                    cout << "\nExiting. Your receipts are: " << endl;
                    cout << "---------------------------------------------" << endl;

                    displayUserReceipts(All_receipts, Customers[selected_customer], total_receipts);

                } else {
                    cout << "\nError. There is no such customer with ID: " << new_customer.ID << endl;
                }

                break;

            case 8:
                cout << "\nReturn book" << endl;
                cout << "---------------------------------------------\n" << endl;

                cout << "\nEnter the ID of the customer: ";

                cin.ignore();
                cin.getline(new_customer.ID, 8);

                if (inCustomers(Customers, new_customer, customers_num)) {
                    selected_customer = findCustomer(Customers, new_customer, customers_num);

                    displayUserReceipts(All_receipts, Customers[selected_customer], total_receipts);

                    do {
                        cout << "\nEnter the ISBN of the book to return it (-1 to exit): ";
                        cin.getline(isbn_input, 30);

                        if (strcmp(isbn_input, "-1") == 0) {
                            cout << "Exiting. Your receipts are: " << endl;
                            cout << "---------------------------------------------" << endl;

                            displayUserReceipts(All_receipts, Customers[selected_customer], total_receipts);

                        } else {
                            receipt_found = false;

                            for (int i = 0; i < total_receipts; i++) {
                                if (strcmp(isbn_input, All_receipts[i].Borrowed_book.ISBN) == 0) {
                                    for (int j = 0; j < books_num; j++) {
                                        if (strcmp(All_receipts[i].Borrowed_book.ISBN, Books[j].ISBN) == 0) {
                                            receipt_found = true;
                                            returnBook(Customers[selected_customer], All_receipts[i], Books[j]);
                                            Books[j].On_loan = false;
                                            Customers[selected_customer].Books_num--;
                                            cout << "\nThe book \"" << All_receipts[i].Borrowed_book.Title << "\" successfully returned." << endl;
                                        }
                                    }
                                }
                            }

                            if (receipt_found == false) {
                                cout << "\nError. There is no book with ISBN \"" << isbn_input << "\" in the system. Please, enter again or -1 to finish." << endl;
                            }
                        }
                    } while (strcmp(isbn_input, "-1") != 0);

                } else {
                    cout << "\nError. There is no such customer with ID: " << new_customer.ID << endl;
                }



                break;

            case 9:
                cout << "\nReports" << endl;
                cout << "---------------------------------------------\n" << endl;

                cout << "Maximum books: " << endl;
                cout << "---------------------------------------------\n" << endl;
                for (int i = 0; i < customers_num; i++) {
                    if (Customers[i].Books_num >= 5) {
                        printDetails(Customers, Customers[i].ID, customers_num);
                    }
                }
                cout << "---------------------------------------------\n" << endl;

                cout << "Exceeded the limit of 5 books: " << endl;
                cout << "---------------------------------------------\n" << endl;
                for (int i = 0; i < customers_num; i++) {
                    if (Customers[i].Books_num > 5) {
                        printDetails(Customers, Customers[i].ID, customers_num);
                        cout << "--Books on loan--" << endl;
                        for (int j = 0; j < Customers[i].Books_num; j++) {
                            cout << "Title: \"" << Customers[i].Books[j].Title << "\"" << endl;
                            cout << "Author: " << Customers[i].Books[j].Author << "\n" << endl;
                        }
                    }
                }
                cout << "---------------------------------------------\n" << endl;

                cout << "Books on reserve: " << endl;
                cout << "---------------------------------------------\n" << endl;
                for (int i = 0; i < total_receipts; i++) {
                    if (All_receipts[i].Reserved == true) {
                        cout << "Title: \"" << All_receipts[i].Borrowed_book.Title << "\" " << endl;
                        cout << "Author: " << All_receipts[i].Borrowed_book.Author << "\n" << endl;
                    }
                }
                cout << "---------------------------------------------\n" << endl;

                break;

            case 10:
                cout << "\nCalculate and print fines" << endl;
                cout << "---------------------------------------------\n" << endl;

                do {
                    valid_date = true;
                    cout << "Enter the current date in the form DD/MM/YYYY: ";
                    cin.ignore();
                    cin.getline(parsedate, 11);

                    if (dateValidate(parsedate)) {
                        Input_date.Day = (parsedate[0]-chardif)*10 + parsedate[1]-chardif;
                        Input_date.Month = (parsedate[3]-chardif)*10 + parsedate[4]-chardif;
                        Input_date.Year = (parsedate[6]-chardif)*1000 + (parsedate[7]-chardif)*100 + (parsedate[8]-chardif)*10 + parsedate[9]-chardif;
                                            
                        if (Input_date.Day > 31) {
                            cout << "\nError. There cannot be more than 31 days in a month. Please, enter a new date." << endl;
                            valid_date = false;

                        } else if (Input_date.Day == 0 || Input_date.Month == 0 || Input_date.Year == 0) {
                            cout << "\nError. None of the date numbers can be equal to 0." << endl;
                            valid_date = false;

                        } else if ((Input_date.Month > 12)) {
                            cout << "\nError. There can only be 12 months in a year. Please, enter another date." << endl;
                            valid_date = false;

                        } else if (Input_date.Day > days_in_month[Input_date.Month]) {
                            cout << "\nError. There cannot be more than " << days_in_month[Input_date.Month] << " days in this month. " << endl;
                            valid_date = false;

                        } else if ((Input_date.Month == 2 && Input_date.Year % 4 != 0) && Input_date.Day > 28) {
                            cout << "\nError. Year " << Input_date.Year << " is not a leap year. The maximum number of days in February is 28." << endl;
                            valid_date = false;
                        } 
                        
                    } else {
                        cout << "\nError. The date must be in the form DD/MM/YYYY. Please, enter again." << endl;
                    }
                } while (valid_date == false);

                for (int i = 0; i < total_receipts; i++) {
                    if (All_receipts[i].Due_date.Year <= Input_date.Year) {
                        if (All_receipts[i].Due_date.Year == Input_date.Year && All_receipts[i].Due_date.Month <= Input_date.Month) {
                            if (All_receipts[i].Due_date.Month == Input_date.Month && All_receipts[i].Due_date.Day < Input_date.Day) {
                                calculateFine(All_receipts[i], Input_date, days_in_month);
                            } else if (All_receipts[i].Due_date.Month < Input_date.Month) {
                                calculateFine(All_receipts[i], Input_date, days_in_month);
                            }
                        } else if (All_receipts[i].Due_date.Year < Input_date.Year) {
                            calculateFine(All_receipts[i], Input_date, days_in_month);
                        }
                    } 
                }

                for (int i = 0; i < customers_num; i++) {
                    total_fine = 0;
                    for (int j = 0; j < total_receipts; j++) {
                        if (strcmp(All_receipts[j].Borrower.ID, Customers[i].ID) == 0 && All_receipts[j].Fine > 0) {
                            
                            total_fine += All_receipts[j].Fine;
                        }
                    }

                    if (total_fine > 0) {
                        printDetails(Customers, Customers[i].ID, customers_num);
                        cout << "Total fine: " << total_fine << " EUR" << endl;
                    }
                }

                break;

            case 0:
                cout << "\nExiting program." << endl;
                cout << "---------------------------------------------\n" << endl;
                break;

            default:
                cout << "Error. Please, enter a suitable choice (0 - 10) " << endl;

        }
    } while (menu_choice != 0);
    
    return 0;
}

// This function uses bubble sorting algorithm to put all the customers in an alphabetical order of the name.
void sortCustomers(Customer Customers[], int customers_num) {
    Customer temp;
    
    for (int i = 0; i < customers_num; i++) {
        for (int j = i; j < customers_num; j++) {
            if (strcmp(Customers[i].Name, Customers[j].Name) == 1) {
                temp = Customers[i];
                Customers[i] = Customers[j];
                Customers[j] = temp;
            }
        }
    }
}

// This function looks for a customer in the database, using ID, and returns an index position of this customer in an array or -1 in case nothing is found.
int findCustomer(Customer Customers[], Customer to_find, int customers_num) {
    for (int i = 0; i < customers_num; i++) {
        if (strcmp(Customers[i].ID, to_find.ID) == 0) {
            return i;
        }
    }

    return -1;
}

// Same as above, but returns True if found and False if not.
bool inCustomers(Customer Customers[], Customer to_find, int customers_num) {
    for (int i = 0; i < customers_num; i++) {
        if (strcmp(Customers[i].ID, to_find.ID) == 0) {
            return true;
        }
    }

    return false;
}

// Displays the details of the customer with the right ID.
void printDetails(Customer Customers[], char id_number[8], int customers_num) {
    Customer sample;
    strcpy(sample.ID, id_number);
    int selected_customer = findCustomer(Customers, sample, customers_num);

    cout << "Name: " << Customers[selected_customer].Name << endl;
    cout << "Address: " << Customers[selected_customer].Address << endl;
    cout << "ID: " << id_number << endl;
}

// This function displays all the books in the library by their index in array and shows their loan status.
void printBooks(Book All_books[], int books_num) {
    for (int i = 0; i < books_num; i++) {
        if (All_books[i].On_loan != true) {
            cout << i << ". \"" << All_books[i].Title << "\", " << All_books[i].Author << ", AVAILABLE" << endl;
        } else {
            cout << i << ". \"" << All_books[i].Title << "\", " << All_books[i].Author << ", BORROWED" << endl;
        }
    }
}

// Returns true if the cstring is in the form ii/ii/iiii and false if otherwise.
bool dateValidate(char parseline[]) {
    bool valid = true;
    if (parseline[2] != '/' || parseline[5] != '/') {
        valid = false;
    }
    if (parseline[0] < '0' || parseline[0] > '9') {
        valid = false;
    }
    if (parseline[1] < '0' || parseline[1] > '9') {
        valid = false;
    }
    if (parseline[3] < '0' || parseline[3] > '9') {
        valid = false;
    }
    if (parseline[4] < '0' || parseline[4] > '9') {
        valid = false;
    }
    for (int i = 6; i < 10; i++) {
        if (parseline[i] < '0' || parseline[i] > '9') {
            valid = false;
        }
    }
    if (parseline[10] != '\0') {
        valid = false;
    }

    return valid;
}

// Function is used to ensure that the ID is in the form U******. Returns true or false, depending on the outcome.
bool idValidate(char id[]) {
    bool valid = true;
    if (id[0] != 'U') {
        valid = false;
    }
    if (strlen(id) != 7) {
        valid = false;
    }
    for (int i = 1; i < 7; i++) {
        if (id[i] < '0' || id[i] > '9') {
            valid = false;
        }
    }
    return valid;
}

// Function checks ISBN, so that it consists only from numbers and -.
bool isbnValidate(char isbn[]) {
    bool valid = true;
    
    for (int i = 0; i < strlen(isbn); i++) {
        if (isbn[i] != '-' && (isbn[i] < '0' || isbn[i] > '9')) {
            valid = false;
        } 
    }

    return valid;
}

// This function displays all the receipts connected to a certain customer.
void displayUserReceipts(Receipt All_receipts[], Customer user, int total_receipts) {
    for (int i = 0; i < total_receipts; i++) {
        if (strcmp(All_receipts[i].Borrower.ID, user.ID) == 0) {

            cout << "\nBorrower: " << All_receipts[i].Borrower.Name << ", " << All_receipts[i].Borrower.ID << endl;
            cout << "Borrowed book: \"" << All_receipts[i].Borrowed_book.Title << "\", " << All_receipts[i].Borrowed_book.Author << ", " << All_receipts[i].Borrowed_book.ISBN << endl;
            cout << "ISBN: " << All_receipts[i].Borrowed_book.ISBN << endl;
            cout << "Due date: " << All_receipts[i].Due_date.Day << "/" << All_receipts[i].Due_date.Month << "/" << All_receipts[i].Due_date.Year << endl;

            if (All_receipts[i].Reserved == false) {
                cout << "Reservation status: Available." << endl;
            } else {
                cout << "Reservation status: Reserved." << endl;
            }

            if (All_receipts[i].Returned == false) {
                cout << "Borrowing status: Borrowed" << endl;
            } else {
                cout << "Borrowing status: Returned." << endl;
            }
        }
    }
}

// This function is called to update the status of returned book, receipt and customer.
void returnBook(Customer user, Receipt cheque, Book to_return) {
    for (int i = 0; i < user.Books_num; i++) {
        if (strcmp(user.Books[i].ISBN, to_return.ISBN) == 0) {
            for (int j = i; j < (user.Books_num - 1); j++) {
                user.Books[j] = user.Books[j+1];
            }
        }
    }
    user.Books_num--;

    cheque.Returned = true;
    cheque.Reserved = false;
    cheque.Borrowed_book.On_loan = false;
    cheque.Borrowed_book.Borrower_ID[0] = '\0';

    to_return.Borrower_ID[0] = '\0';
    to_return.On_loan = false;
    cout << to_return.ISBN << " " << to_return.On_loan << endl;
}

// This function calculates the fine on the receipt until the entered date, using the number of days in each month.
void calculateFine(Receipt cheque, Date current_date, int days_in_month[]) {
    if (cheque.Due_date.Year == current_date.Year) {
        for (int month = cheque.Due_date.Month; month <= current_date.Month; month++) {
            if (current_date.Month == month) {
                for (int day = cheque.Due_date.Day; day < current_date.Day; day++) {
                    cheque.Fine += 0.50;
                }
            } else {
                if (month != 2) {
                    cheque.Fine += 0.50 * days_in_month[month];
                        
                } else {
                    if (cheque.Due_date.Year % 4 == 0) {
                        cheque.Fine += 0.50 * 29;
                    } else {
                        cheque.Fine += 0.50 * 28;
                    }
                }
            }
        }
    }

    if (cheque.Due_date.Year < current_date.Year) {
        for (int month = cheque.Due_date.Month; month <= 12; month++) {
            if (month != 2) {
                cheque.Fine += 0.50 * days_in_month[month];
            } else {
                if (cheque.Due_date.Year % 4 == 0) {
                    cheque.Fine += 0.50 * 29;
                } else {
                    cheque.Fine += 0.50 * 28;
                }
            }
        }

        for (int year = cheque.Due_date.Year + 1; year < current_date.Year; year++) {
            for (int month = 1; month <= 12; month++) {
                if (month != 2) {
                    cheque.Fine += 0.50 * days_in_month[month];
                } else {
                    if (year % 4 == 0) {
                        cheque.Fine += 0.50 * 29;
                    } else {
                        cheque.Fine += 0.50 * 28;
                    }
                }
            }
        }

        for (int month = 1; month <= current_date.Month; month++) {
                if (current_date.Month == month) {
                    for (int day = cheque.Due_date.Day; day < current_date.Day; day++) {
                        cheque.Fine += 0.50;
                    }
                } else {
                if (month != 2) {
                    cheque.Fine += 0.50 * days_in_month[month];
                        
                } else {
                    if (current_date.Year % 4 == 0) {
                        cheque.Fine += 0.50 * 29;
                    } else {
                        cheque.Fine += 0.50 * 28;
                    }
                }
            }
        }
    }
    cout << cheque.Borrower.Name << ": " << cheque.Fine << " EUR " << endl;
}
