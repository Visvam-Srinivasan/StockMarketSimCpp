#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
#include<stdlib.h>
#include<cstdlib>
#include<time.h>
#include<iomanip>
#include<math.h>

#include "csvLibrary.h"

std::string fileName = "accountDataBase.csv";
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

        std::string name; 
        std::string emailID;
        std::string password;
        std::string accountNumber;
        int balance;

        std::string rowItems[colNums];

        int currentAccountRow = 0;
        int columnIndex[4] = {0};

    public:

        //Functions for creating bank account
        void createAccount();
        void getAccountDetails();
        void generateAccountID();

        //Functions for login and logout
        void loginBank();
        bool validateLogin(std::string , std::string);
        void setColumnIndex(int);
        void logoutBank();
        void setBankAccountData();

        //Functions to deposit, withdraw and transfer
        void deposit();
        void withdraw();
        void transfer();
        bool changeBalance(int, bool);
        bool changeBalance(int, bool, std::string);

        //Other bank account operations
        void displayBankAccountDetails();
        void deleteAccount();


}AccObj;


int main()
{
    //Sets seed of random value generator to the current time
    srand(time(0));
    
    MenuObj.mainMenu();
    return 0;
}


//      MENUS

void Menu::mainMenu()
{
    int choice;
    std::cout << "\n\t\tSTOCK MARKET SIM\n************************************************\n\t\t1 - Login\n\t\t2 - Create Account\n\nEnter Choice: ";
    std::cin >> choice;
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
        std::cout << "\n\t\tACCOUNT MENU\n************************************************\n\n\t1 - Display Account Details\n\t2 - Withdraw\n\t3 - Deposit\n\t4 - Transfer\n\t5 - Delete Account\n\n\tEnter choice: ";
        std::cin >> choice;

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
            std::cout << "\n\nINVALID INPUT - Please enter a number upto 5\n\n";
        }

    }while(choice>5);

}


//      BANK ACCOUNT FUNCTIONS

//  CREATE ACCOUNT

void BankAccount::createAccount()
{
    getAccountDetails();
    generateAccountID();

    addRow(fileName, rowItems, colNums);

    std::cout << "\n\t## ACCOUNT SUCCESSFULLY CREATED ##\n";
    displayBankAccountDetails();

    std::cout << "\n\n\t!! PLEASE RE-LOGIN TO CONTINUE USING YOUR NEW ACCOUNT !!\n";
    MenuObj.mainMenu();
}

void BankAccount::getAccountDetails()
{
    std::cout << "\n\n\t\tCREATE AN ACCOUNT\n************************************************\n\nEnter name: ";
    std::cin.get(bufferChar);
    std::string name;
    std::getline(std::cin, name);
    rowItems[3] = name;
    std::cout << "\nEnter Mail ID: ";
    std::cin >> rowItems[4];
    std::cout << "\nEnter initial deposit: ";
    std::cin >> balance;
    rowItems[2] = std::to_string(balance);
    std::cout << "\nEnter password for account: ";
    std::cin >> rowItems[1];
    //Make function to check valid mail and password
}

void BankAccount::generateAccountID()
{
    int randomComponent = rand();
    int lastEntryRowNumber;
    std::string latestAccountNumber;

    //Gets the latest account number in the data base
    lastEntryRowNumber = numberOfEntries("accountDataBase.csv");
    latestAccountNumber = getDataItem("accountDataBase.csv", lastEntryRowNumber, 0);
 
    if(lastEntryRowNumber>0)
    {
        //Erases random component of the account number to get the sequence component
        latestAccountNumber.erase(latestAccountNumber.begin(), latestAccountNumber.end()-4);

        //Generates account number for new account
        accountNumber = std::to_string(randomComponent) + std::to_string(stoi(latestAccountNumber) + 1);
        rowItems[0] = accountNumber;
    }
    else
    {
        //Generates account number for the first account created
        accountNumber = std::to_string(randomComponent) + std::to_string(1000);
        rowItems[0] = accountNumber;

    }

}

//  LOGIN LOGUT OPERATIONS

void BankAccount::loginBank()
{
    std::vector<std::string> loginInput;
    std::vector<int> columnNumber ({0, 1});
    std::string accNoInput;
    std::string passwordInput;
    bool loginSuccessStatus = 0;

    std::cout<<"\n\n\t\tLOGIN TO ACCOUNT\n************************************************\n\nEnter Account Number: ";
    std::cin >> accNoInput;
    loginInput.push_back(accNoInput);
    std::cout << "Enter password: ";
    std::cin >> passwordInput;
    loginInput.push_back(passwordInput);

    loginSuccessStatus = findIfInRow(fileName, loginInput, columnNumber);

    if(loginSuccessStatus)
    {
        std::cout<<"\n\n\t\t### LOGIN SUCCESSFULL ###\n\n";
        currentAccountRow = findRowNumber(fileName, accNoInput, 0);
        setBankAccountData();
        MenuObj.bankAccountMenu();
    }
    else
    {
        std::cout<<"\n\n\t\t!!! INCORRECT ACCOUNT NUMBER OR PASSWORD !!!\n";
        MenuObj.mainMenu();
    }
}

void BankAccount::logoutBank()
{
    char choice;
    std::cout << "\n\tCONFIRM IF YOU WANT TO LOGOUT (Y/N): ";
    std::cin >> choice;

    if(choice=='Y'||choice=='y')
    {
        MenuObj.mainMenu();
    }
    else
    {
        MenuObj.bankAccountMenu();
    }
}

//Assigns bank account class' members with input row number's data
void BankAccount::setBankAccountData()
{
    std::vector<std::string> rowItem = getRow(fileName, currentAccountRow);

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

//  BANK ACCOUNT OPERATIONS

void BankAccount::withdraw()
{
    int withdrawAmount;
    bool withdrawSuccessStatus = 0;

    std::cout << "\nEnter amount to withdraw: ";
    std::cin >> withdrawAmount;

    if(balance >= withdrawAmount)
    {
        balance-=withdrawAmount;
        changeDataItem(fileName, std::to_string(balance), currentAccountRow, 2);
        std::cout << "\nSUCCESSFULLY WITHDRAWN\nCurrent Balance: " << balance;
    }
    else
    {
        std::cout << "\nWithdraw amount greater than balance." << std::endl;

    }

}

void BankAccount::deposit()
{
    int depositAmount;

    std::cout << "\nEnter amount to deposit: ";
    std::cin >> depositAmount;
    
    balance+=depositAmount;
    changeDataItem(fileName, std::to_string(balance), currentAccountRow, 2);
    std::cout << "\nSUCCESSFULLY DEPOSITED\nCurrent Balance: " << balance << std::endl;
}

void BankAccount::transfer()
{
    int transferAmount;
    std::string transferAccountNumber;
    bool transferSuccessStatus = 0;

    std::string transferAccountBalanceString;
    int transferAccountBalance;
    int transferAccountRowNumber = 0;

    std::cout << "\nEnter account number to transfer to: ";
    std::cin >> transferAccountNumber;
    std::cout << "Enter amount to transfer: ";
    std::cin >> transferAmount;

    if(balance >= transferAmount)
    {
        balance-=transferAmount;

        //Changes balance in current account
        changeDataItem(fileName, std::to_string(balance), currentAccountRow, 2);

        //Chnages balance in the reciever's account
        transferAccountRowNumber = findRowNumber(fileName, transferAccountNumber, 0);
        transferAccountBalanceString = getDataItem(fileName, transferAccountRowNumber, 2);
        transferAccountBalance = stoi(transferAccountBalanceString) + transferAmount;
        changeDataItem(fileName, std::to_string(transferAccountBalance), transferAccountRowNumber, 2);

        std::cout << "\nSUCCESSFULLY TRANSFERRED\nCurrent Balance: " << balance;

    }
    else
    {
        std::cout << "\nWithdraw amount greater than balance." << std::endl;

    }

}

void BankAccount::displayBankAccountDetails()
{
    char choice;
    std::cout << "\n\t\tBANK ACCOUNT DETAILS\n************************************************\n\t\tName: " << name <<"\n\t\tAccount Number: "<<accountNumber<<"\n\t\tBalance: "<<balance<<"\n\t\tE-Mail ID: " << emailID << std::endl;
    std::cout << "\n\tDo you want to view your account password (Y/N): ";
    std::cin >> choice;
    if(choice=='Y'||choice=='y')
    {
        std::cout << "\t\tPassword: " << password;
    }
    else
    {
        MenuObj.bankAccountMenu();
    }

}

void BankAccount::deleteAccount()
{
    std::vector<std::string> deleteAccountInput;
    std::vector<int> columnNumber ({0, 1});
    std::string accountNumberInput, passwordInput;
    int rowNumberInput;
    bool successStatus = 0;

    char choice;
    std::cout << "\nARE YOU SURE YOU WANT TO DELETE THE ACCOUNT?(Y/N): ";
    if(choice=='Y'||choice=='y')
    {
        std::cout << "\nEnter account number again: ";
        std::cin >> accountNumberInput;
        deleteAccountInput.push_back(accountNumberInput);
        std::cout << "Enter password: ";
        std::cin >> passwordInput;
        deleteAccountInput.push_back(passwordInput);

        successStatus = findIfInRow(fileName, deleteAccountInput, columnNumber);
        if(successStatus)
        {
            findRowNumber(fileName, accountNumberInput, 0);
            deleteRow(fileName, rowNumberInput);
            std::cout << "\nAccount successfully deleted.";
        }
        else
        {
            std::cout << "\nIncorrect Account number or password, please relogin.";
        }

        MenuObj.mainMenu();

    }
    else
    {
        MenuObj.bankAccountMenu();
    }

}
