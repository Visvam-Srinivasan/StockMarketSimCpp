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

#include "csvLibrary.h"

using namespace std;

string fileName = "accountDataBase.csv";

int const colNums = 5;

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

        string rowItems[colNums];

        int currentAccountRow = 0;
        int columnIndex[4] = {0};

    public:

        //Functions for creating bank account
        void createAccount();
        void getAccountDetails();
        void generateAccountID();

        //Functions for login and logout
        void loginBank();
        bool validateLogin(string , string);
        void setColumnIndex(int);
        void logoutBank();
        void setBankAccountData();

        //Functions to deposit, withdraw and transfer
        void deposit();
        void withdraw();
        void transfer();
        bool changeBalance(int, bool);
        bool changeBalance(int, bool, string);

        //Other bank account operations
        void displayBankAccountDetails();
        void deleteAccount();


}AccObj;


//MENUS

void Menu::mainMenu()
{
    int choice;
    cout << "\n\t\tSTOCK MARKET SIM\n************************************************\n\t\t1 - Login\n\t\t2 - Create Account\n\nEnter Choice: ";
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

void Menu::bankAccountMenu()
{
    int choice;

    do
    {  
        cout << "\n\t\tACCOUNT MENU\n************************************************\n\n\t1 - Display Account Details\n\t2 - Withdraw\n\t3 - Deposit\n\t4 - Transfer\n\t5 - Delete Account\n\n\tEnter choice: ";
        cin >> choice;

        switch(choice)
        {
            case 1:
            AccObj.displayBankAccountDetails();
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


//CREATE ACCOUNT

void BankAccount::createAccount()
{
    getAccountDetails();
    generateAccountID();

    addRow("accountDataBase.csv", rowItems, colNums);

    cout << "\n\t## ACCOUNT SUCCESSFULLY CREATED ##\n";
    displayBankAccountDetails();

    cout << "\n\n\t!! PLEASE RE-LOGIN TO CONTINUE USING YOUR NEW ACCOUNT !!\n";
    MenuObj.mainMenu();
}

void BankAccount::getAccountDetails()
{
    cout << "\n\n\t\tCREATE AN ACCOUNT\n************************************************\n\nEnter name: ";
    cin.get(bufferChar);
    string name;
    getline(cin, name);
    rowItems[3] = name;
    cout << "\nEnter Mail ID: ";
    cin >> rowItems[4];
    cout << "\nEnter initial deposit: ";
    cin >> balance;
    rowItems[2] = to_string(balance);
    cout << "\nEnter password for account: ";
    cin >> rowItems[1];
    //Make function to check valid mail and password
}

void BankAccount::generateAccountID()
{
    int randomComponent = rand();
    int entryNumber = -1;
    string prevAccNumber;

    int lastEntryRow = numberOfEntries("accountDataBase.csv");

   // cout << "number: " << lastEntryRow << endl;
    prevAccNumber = getDataItem("accountDataBase.csv", lastEntryRow, 0);
    //cout << " account: " << prevAccNumber << endl;
 
    if(lastEntryRow>0)
    {
        //Erases random component of the account number to get the sequence component
        prevAccNumber.erase(prevAccNumber.begin(), prevAccNumber.end()-4);

        //Generates account number for new account
        accountNumber = to_string(randomComponent) + to_string(stoi(prevAccNumber) + 1);
        rowItems[0] = accountNumber;
    }
    else
    {
        //Generates account number for the first account created
        accountNumber = to_string(randomComponent) + to_string(1000);
        rowItems[0] = accountNumber;

    }

}


//LOGIN ACCOUNT

void BankAccount::loginBank()
{
    vector<string> loginInput;
    vector<int> columnNumber ({0, 1});
    string accNoInput;
    string passwordInput;
    bool loginSuccessStatus = 0;

    cout<<"\n\n\t\tLOGIN TO ACCOUNT\n************************************************\n\nEnter Account Number: ";
    cin >> accNoInput;
    loginInput.push_back(accNoInput);
    cout << "Enter password: ";
    cin >> passwordInput;
    loginInput.push_back(passwordInput);

    loginSuccessStatus = findIfInRow(fileName, loginInput, columnNumber);

    if(loginSuccessStatus)
    {
        cout<<"\n\n\t\t### LOGIN SUCCESSFULL ###\n\n";
        currentAccountRow = findRowNumber("accountDataBase.csv", accNoInput, 0);
        cout << currentAccountRow;
        setBankAccountData();
        MenuObj.bankAccountMenu();
    }
    else
    {
        cout<<"\n\n\t\t!!! INCORRECT ACCOUNT NUMBER OR PASSWORD !!!\n";
        MenuObj.mainMenu();
    }
}

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


//BANK ACCOUNT OPERATIONS

void BankAccount::withdraw()
{
    int withdrawAmount;
    bool withdrawSuccessStatus = 0;

    cout << "\nEnter amount to withdraw: ";
    cin >> withdrawAmount;

    cout << "\nBalance: " << balance;
    if(balance >= withdrawAmount)
    {
        balance-=withdrawAmount;
        changeDataItem(fileName, to_string(balance), currentAccountRow, 2);
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

    cout << "\nEnter amount to deposit: ";
    cin >> depositAmount;
    
    balance+=depositAmount;
    changeDataItem(fileName, to_string(balance), currentAccountRow, 2);
    cout << "\nSUCCESSFULLY DEPOSITED\nCurrent Balance: " << balance << endl;
}

void BankAccount::transfer()
{
    int transferAmount;
    string transferAccountNumber;
    bool transferSuccessStatus = 0;
    string stringBalanceAmount;

    string transferAccountBalanceString;
    int transferAccountBalance;
    int transferAccountRow = 0;

    cout << "\nEnter account number to transfer to: ";
    cin >> transferAccountNumber;
    cout << "Enter amount to transfer: ";
    cin >> transferAmount;

    if(balance >= transferAmount)
    {
        balance-=transferAmount;
        changeDataItem(fileName, to_string(balance), currentAccountRow, 2);
        cout << "\nSUCCESSFULLY TRANSFERRED\nCurrent Balance: " << balance;

        transferAccountRow = findRowNumber(fileName, transferAccountNumber, 0);
        transferAccountBalanceString = getDataItem(fileName, transferAccountRow, 2);
        transferAccountBalance = stoi(transferAccountBalanceString) + transferAmount;
        changeDataItem(fileName, to_string(transferAccountBalance), transferAccountRow, 2);
    }
    else
    {
        cout << "\nWithdraw amount greater than balance." << endl;

    }

}

//Assigns bank account class' members with input row number's data
void BankAccount::setBankAccountData()
{
    vector<string> rowItem = getRow(fileName, currentAccountRow);

    //Assigns row members to class members
    for(int i = 0; i < colNums; i++)
    {
        switch(i)
        {
            case 0:
            accountNumber = rowItem[i];
            break;
            case 1:
            password = rowItem[i];
            break;
            case 2:
            balance = stoi(rowItem[i]);
            break;
            case 3:
            name = rowItem[i];
            break;
            case 4:
            emailID = rowItem[i];
            break;
        }
    }
}

void BankAccount::displayBankAccountDetails()
{
    char choice;
    cout << "\n\t\tBANK ACCOUNT DETAILS\n************************************************\n\t\tName: " << name <<"\n\t\tAccount Number: "<<accountNumber<<"\n\t\tBalance: "<<balance<<"\n\t\tE-Mail ID: " << emailID << endl;
    cout << "\n\tDo you want to view your account password (Y/N): ";
    cin >> choice;
    if(choice=='Y'||choice=='y')
    {
        cout << "\t\tPassword: " << password;
    }
    else
    {
        MenuObj.bankAccountMenu();
    }

}

void BankAccount::deleteAccount()
{
    vector<string> deleteAccountInput;
    vector<int> columnNumber ({0, 1});
    string accountNumberInput, passwordInput;
    bool successStatus = 0;

    char choice;
    cout << "\nARE YOU SURE YOU WANT TO DELETE THE ACCOUNT?(Y/N): ";
    if(choice=='Y'||choice=='y')
    {
        cout << "\nEnter account number again: ";
        cin >> accountNumberInput;
        deleteAccountInput.push_back(accountNumberInput);
        cout << "Enter password: ";
        cin >> passwordInput;
        deleteAccountInput.push_back(passwordInput);
        successStatus = findIfInRow(fileName, deleteAccountInput, columnNumber);
        if(successStatus)
        {
            cout << "\nAccount successfully deleted.";
        }
        else
        {
            cout << "\nIncorrect Account number or password, please relogin.";
        }

        MenuObj.mainMenu();

    }
    else
    {
        MenuObj.bankAccountMenu();
    }

}

int main()
{
    //Sets seed of random value generator to the current time
    srand(time(0));
    
    MenuObj.mainMenu();
    return 0;
}