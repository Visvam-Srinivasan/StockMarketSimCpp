#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
#include<stdlib.h>
#include<cstdlib>
#include<time.h>
#include<cstdlib>
#include<iomanip>
#include<math.h>

using namespace std;

char bufferChar;

class Menu
{
    public:
    void mainMenu();
    void bankAccountMenu();

}MenuObj;

class BankAccount
{
    private:

        string name; 
        string emailID;
        string password;
        string accountNumber;
        int balance;

        int currentAccountRow;
        int columnIndex[4] = {0};

    public:

        void loginBank();
        void createAccount();
        bool validateLogin(string , string);
        void setColumnIndex();

        void getAccountDetails();
        void displayAccountDetails();
        void deposit();
        void withdraw();

        void logoutBank();
        void deleteAccount();

        void generateAccountID();
        void updateSequenceAccIDComponent();
}AccObj;

//Main menu
void Menu::mainMenu()
{
    int choice;
    cout << "\n\t\tSTOCK MARKET SIM\n************************************************\n1 - Login\n2 - Create Account\n\nEnter Choice: ";
    cin >> choice;
    switch(choice)
    {
        case 1:
        AccObj.loginBank();
        break;


        case 2:
        AccObj.createAccount();
        break;
    }
}

//Menu after login
void Menu::bankAccountMenu()
{
    int choice;

    do
    {  
        cout << "\t\tACCOUNT MENU\n\t****************************\n\n\t1 - Display Account Details\n\t2 - Withdraw\n\t3 - Deposit\n\t4 - Logout\n\t5 - Delete Account\n\n\tEnter choice: ";
        cin >> choice;

        switch(choice)
        {
            case 1:
            AccObj.displayAccountDetails();
            break;

            case 2:
            AccObj.withdraw();
            break;

            case 3:
            AccObj.deposit();
            break;

            case 4:
            AccObj.logoutBank();
            break;

            case 5:
            AccObj.deleteAccount();
            break;

            default:
            cout << "\n\nINVALID INPUT - Please enter a number upto 5\n\n";
        }

    } while (choice>5);

}

//Creates account and stores data
void BankAccount::createAccount()
{
    getAccountDetails();
    generateAccountID();

    fstream file;
    file.open("accountDataBase.csv", ios::app | ios::in | ios::out);
    if (!file.is_open()) 
    {
        cerr << "Error opening file in generation ID." << endl;
        return;  // Exit the function if the file couldn't be open
    }

    file<<endl<<accountNumber<<','<<password<<','<<balance<<','<<name<<','<<emailID;
    file.close();

    MenuObj.mainMenu();
}

//Get details to create bank account
void BankAccount::getAccountDetails()
{
    cout << "\t\tCREATE AN ACCOUNT\n************************************************\n\nEnter name: ";

    cin.get(bufferChar);

    getline(cin, name);
    cout << "\nEnter Mail ID: ";
    cin >> emailID;
    cout << "\nEnter initial deposit: ";
    cin >> balance;
    cout << "\nEnter password for account: ";
    cin >> password;

    //Make function to check valid mail and password
}

//Generates unique AccountID for new account
void BankAccount::generateAccountID()
{
    int randomComponent = rand();
    int entryNumber = -1;
    string prevAccNumber;


    string line;
    fstream file;
    file.open("accountDataBase.csv", ios::ate|ios::in);
    if (!file.is_open()) 
    {
        cerr << "Error opening file in generation ID." << endl;
        return;  // Exit the function if the file couldn't be open
    }

    file.seekg(0);
    while(getline(file, line))
    {
        entryNumber++;
    }
 
    cout << endl << "entry number: " << entryNumber << endl;

    if(entryNumber>0)
    {
        stringstream s(line);
        getline(s, prevAccNumber, ',');

                        cout << endl << prevAccNumber << endl;

        prevAccNumber.erase(prevAccNumber.begin(), prevAccNumber.end()-4);

                        cout << endl << prevAccNumber << endl;

        accountNumber = to_string(randomComponent) + to_string(stoi(prevAccNumber) + 1);
       
    }
    else
    {
        accountNumber = to_string(randomComponent) + to_string(1000);
    }

    cout << endl << accountNumber << endl;

    file.close();

}

//Login to bank
void BankAccount::loginBank()
{
    string accNoInput;
    string passwordInput;
    bool loginSuccessStatus = 0;
    cout<<"\t\tLOGIN TO ACCOUNT\n\n************************************************\n\nEnter Account Number: ";
    cin >> accNoInput;
    cout << "Enter password: ";
    cin >> passwordInput;

    loginSuccessStatus = validateLogin(accNoInput, passwordInput);

    if(loginSuccessStatus)
    {
        cout<<"\n\n\t\t### LOGIN SUCCESSFULL ###\n\n";
        MenuObj.bankAccountMenu();
    }
    else
    {
        cout<<"\n\n\t\t!!! INCORRECT ACCOUNT NUMBER OR PASSWORD !!!\n";
        MenuObj.mainMenu();
    }
}

//Validates the credentials for login provided by user
bool BankAccount::validateLogin(string accountNumInput, string passwordInput)
{
    string line; 
    string columnValues;
    fstream file;

    int columnNumber = 0;
    int lineNumber = 0;
    bool successLoginFlag = 0;

    file.open("accountDataBase.csv", ios::app | ios::in);
    if (!file.is_open()) 
    {
        cerr << "Error opening file." << endl;
        return 0;  // Exit the function if the file couldn't be opened
    }

    while(getline(file, line))
    {   
        //Splits string line into distinct data items onto the stream 's'
        stringstream s(line);

        //Iterates through distinct data separated by ',' items from stream 's' 
        while(getline(s, columnValues, ','))
        {
            //Checking if input values match
            if(columnNumber == 0 && columnValues != accountNumInput)
                break;
            else 
            {
                if(columnNumber == 1 && columnValues == passwordInput) 
                {
                    successLoginFlag = 1;
                    break;
                }
                else
                {
                    if(columnNumber>=1) 
                        break;
                    else
                    {
                        columnNumber++;
                        continue;
                    }
                }
            }
        }

        if(successLoginFlag)
            break;
        else
            lineNumber++;
    }

    if(successLoginFlag)
    {
        currentAccountRow = lineNumber;
        setColumnIndex();
    }

    file.close();

    return successLoginFlag;
}

//Stores indices of columns in the logged in account
void BankAccount::setColumnIndex()
{
    string line;
    fstream file;
    int currentIndex = 1;

    file.open("accountDataBase.csv", ios::in);

    for(int i = 0; i <= currentAccountRow; i++)
    {
        getline(file, line);
    }

    for(int i = 0; i < line.length(); i++)
    {
        if(line[i] == ',')
        {
            columnIndex[currentIndex] = i+1;
            currentIndex++;
        }
    }

    file.close();
}

//Logout
void BankAccount::logoutBank()
{
    char choice;
    cout << "\n\tCONFIRM IF YOU WANT TO LOGOUT (Y/N): ";
    cin >> choice;

    if(choice=='Y'||choice=='y')
    {
        MenuObj.mainMenu();
    }
    else
    {
        MenuObj.bankAccountMenu();
    }
}

void BankAccount::withdraw()
{}

void BankAccount::deposit()
{}

void BankAccount::displayAccountDetails()
{}

void BankAccount::deleteAccount()
{}

int main()
{
    //Sets seed of random value generator to the current time
    srand(time(0));
    MenuObj.mainMenu();
    return 0;
}