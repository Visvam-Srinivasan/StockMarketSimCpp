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

        int currentAccountRow = 0;
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
        void transfer();

        void logoutBank();
        void deleteAccount();

        void generateAccountID();
        void updateSequenceAccIDComponent();

        void updateAccountBalance(int);
        bool changeBalance(int, bool);
        bool changeBalance(int, bool, string);

        void setBankAccountData(int);
}AccObj;

//Main menu
void Menu::mainMenu()
{
    int choice;
    cout << "\n\t\t\tSTOCK MARKET SIM\n************************************************\n\t\t1 - Login\n\t\t2 - Create Account\n\nEnter Choice: ";
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
        cout << "\t\tACCOUNT MENU\n\t****************************\n\n\t1 - Display Account Details\n\t2 - Withdraw\n\t3 - Deposit\n\t4 - Transfer\n\t5 - Delete Account\n\n\tEnter choice: ";
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
            AccObj.transfer();
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

    cout << "\n\t## ACCOUNT SUCCESSFULLY CREATED ##\n";
    displayAccountDetails();
    cout << "\n\t!! PLEASE RE-LOGIN TO CONTINUE USING YOUR NEW ACCOUNT !!\n";

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
        setBankAccountData(currentAccountRow);
        MenuObj.bankAccountMenu();
    }
    else
    {
        cout<<"\n\n\t\t!!! INCORRECT ACCOUNT NUMBER OR PASSWORD !!!\n";
        MenuObj.mainMenu();
    }
}

void BankAccount::setBankAccountData(int inputRowNumber)
{
    fstream file;
    string row;
    string rowItems;
    file.open("accountDataBase.csv", ios::in);
    if (!file.is_open()) 
    {
        cerr << "Error opening file." << endl;
        return;  // Exit the function if the file couldn't be opened
    }

    for(int i = 0; i <=inputRowNumber; i++)
    {
        getline(file, row);
    }

    stringstream s(row);

    for(int i = 0; i < 5; i++)
    {
        getline(s, rowItems, ',');
        switch(i)
        {
            case 0:
            accountNumber = rowItems;
            break;
            case 1:
            password = rowItems;
            break;
            case 2:
            balance = stoi(rowItems);
            break;
            case 3:
            name = rowItems;
            break;
            case 4:
            emailID = rowItems;
            break;
        }
    }

            //cout << accountNumber << endl << password << endl << balance << endl << name << endl << emailID << " : stuff" << endl;

    file.close();
}

//Validates the credentials for login provided by user
bool BankAccount::validateLogin(string accountNumInput, string passwordInput)
{
    string line; 
    string columnValues;
    fstream file;

    int columnNumber = 0;
    int lineNumber = 0;
    bool loginSuccessFlag = 0;
    bool skipFirstLine = 0;

    file.open("accountDataBase.csv", ios::app | ios::in);
    if (!file.is_open()) 
    {
        cerr << "Error opening file." << endl;
        return 0;  // Exit the function if the file couldn't be opened
    }

    while(getline(file, line))
    {   

        columnNumber = 0;

        //Skips the header of the csv file
        if(skipFirstLine == 0)
        {
            skipFirstLine = 1;
            lineNumber++;
            continue;
        }

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
                    loginSuccessFlag = 1;
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

        if(loginSuccessFlag)
            break;
        else
            lineNumber++;
    }

    if(loginSuccessFlag)
    {
        currentAccountRow = lineNumber;
        setColumnIndex();
    }

    cout << endl << "Line number: " << lineNumber << endl;

    file.close();

    return loginSuccessFlag;
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
{
    int withdrawAmount;
    string row;
    string stringBalanceAmount;
    bool withdrawSuccess = 0;

    cout << "\nEnter amount to withdraw: ";
    cin >> withdrawAmount;
    

    withdrawSuccess = changeBalance(withdrawAmount, 1);

    if(withdrawSuccess)
    {
        cout << "\nSUCCESSFULLY WITHDRAWN\nCurrent Balance: " << balance;
    }
    else
    {
        cout << "\nWithdraw amount greater than balance." << endl;

    }

}

void BankAccount::deposit()
{
    int depositAmount;
    string row;
    string stringBalanceAmount;

    cout << "\nEnter amount to deposit: ";
    cin >> depositAmount;

    changeBalance(depositAmount, 0);
    cout << "\nSUCCESSFULLY DEPOSITED\nCurrent Balance: " << balance << endl;
}

void BankAccount::transfer()
{
    int transferAmount;
    string transferAccountNumber;
    string row;
    bool withdrawSuccess = 0;

    string stringBalanceAmount;

    cout << "\nEnter account number to transfer to: ";
    cin >> transferAccountNumber;

    cout << "Enter amount to transfer: ";
    cin >> transferAmount;

    withdrawSuccess = changeBalance(transferAmount, 1);

    if(!withdrawSuccess)
    {
        cout << "\nTransfer amount greater than balance." << endl;
        return;
    }
    else
    {
        changeBalance(transferAmount, 0, transferAccountNumber);
        cout << "\nSUCCESSFULLY TRANSFERED\nCurrent Balance: " << balance << endl;
    }

}

//Change balance of the current account
bool BankAccount::changeBalance(int changeByAmount, bool choiceOfOp)
{
    fstream file;
    string row;
    string stringBalanceAmount;
    bool success = 0;
    file.open("accountDataBase.csv", ios::ate|ios::in|ios::out);
    if (!file.is_open()) 
    {
        cerr << "Error opening file." << endl;
        return 0;  // Exit the function if the file couldn't be opened
    }

    file.seekg(0);
    file.seekp(0);

            //cout << endl << currentAccountRow << endl; 

    for(int i = 0; i <= currentAccountRow; i++)
    {
        getline(file, row);
    } 

    cout << endl << row << endl;
    //Gets balance substring from the row
    stringBalanceAmount = row.substr(columnIndex[2], columnIndex[3] - columnIndex[2] - 1);

                //cout << endl << stringBalanceAmount << endl;

    //Deposit
    if(choiceOfOp == 0)
    {
        balance+=changeByAmount;
        success = 1;
    }
    //Withdraw
    else
    {
        if(changeByAmount>stoi(stringBalanceAmount))
        {
            return success;
        }
        else
        {
            balance = balance - changeByAmount;
            success = 1;
        }

    }

    file.seekp(-(row.length() + 2), ios::cur);
    for(int i = 0; i < row.length(); i++)
    {
        file << '#';
    }
    file.seekp(-(row.length()), ios::cur);

    file<<accountNumber<<','<<password<<','<<balance<<','<<name<<','<<emailID;

    file.close();

    return success;
}

//Change balance of input account
bool BankAccount::changeBalance(int changeByAmount, bool choiceOfOp, string accNumberInput)
{
    fstream file;
    string row;
    string stringBalanceAmount;
    int balanceOfInputAccount;
    string accNumber;
    bool foundAccFlag = 0;
    int rowNumber = 0;

    file.open("accountDataBase.csv", ios::ate|ios::in|ios::out);
    if (!file.is_open()) 
    {
        cerr << "Error opening file." << endl;
        return 0;  // Exit the function if the file couldn't be opened
    }

    file.seekg(0);
    file.seekp(0);

    //Finding input account number
    while(getline(file, row))
    {
        stringstream s(row);
        getline(s, accNumber, ',');
        if(accNumber == accNumberInput)
        {
            foundAccFlag = 1;
            break;
        } 
        rowNumber++;
        
    }

    if(!foundAccFlag)
    {
        return foundAccFlag;
    }
    else
    {
        //Gets balance substring from the row
        stringBalanceAmount = row.substr(columnIndex[3], columnIndex[3] - columnIndex[2] - 1);

        //Deposit
        if(choiceOfOp == 0)
        {
            balanceOfInputAccount = stoi(stringBalanceAmount) + changeByAmount;
        }
    
        file.seekp(-(row.length() + 2), ios::cur);
        for(int i = 0; i < row.length(); i++)
        {
            file << '#';
        }
        file.seekp(-(row.length()), ios::cur);

        setBankAccountData(rowNumber);
        balance = balanceOfInputAccount;

        file<<accountNumber<<','<<password<<','<<balance<<','<<name<<','<<emailID;

    }

    file.close();

    setBankAccountData(currentAccountRow);

    return 1;

}

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