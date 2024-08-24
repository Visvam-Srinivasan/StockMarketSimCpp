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

std::string bankAccountDataBasefileName = "bankAccountDataBase.csv";
int const colNums = 7;

char bufferChar;
 
//      CONSOLE OUTPUT FORMATTERS

std::ostream &newLine(std::ostream &output)
{
    output << "\n\t\t\t\t\t\t";
    return output;
}

std::ostream &drawLine(std::ostream &output)
{
    output << "\n\t\t\t\t************************************************\n";
    return output;
}








class Menu
{
    public:
    void mainMenu();
    void bankAccountMenu();
    void openStockMarket();
    void placeOrderStockMarket();

}MenuObj;

class BankAccount
{
    private:

        std::string name; 
        std::string emailID;
        std::string password;
        std::string accountNumber;
        int balance;
        bool stockMarketAccountStatus;
        int panNumber = 0;

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

        //Opening and enabling stock market for account
        void openStockMarketMainMenu();



}BankAccObj;

class StockMarketAccount
{
    std::string accountNumber;
    std::string accountPortfolioFile;
    int currentAccountRow;


    public:

    void setStockMarketAccount(std::string);
    void showPortfolio();
    int enableStockMarket(int, std::string);


}StockMarketAccObj;


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
    std::cout << newLine << "STOCK MARKET SIM" << drawLine << newLine << "1 - Login" << newLine <<"2 - Create Bank and Market Account" << newLine <<"\nEnter Choice: ";
    std::cin >> choice;
    switch(choice)
    {
        case 1:
        BankAccObj.loginBank();
        break;

        case 2:
        BankAccObj.createAccount();
        break;
    }
}

void Menu::bankAccountMenu()
{
    int choice;

    do
    {  
        std::cout << newLine << "ACCOUNT MENU" << drawLine << newLine <<  "1 - STOCK MARKET MENU" << newLine << "2 - Withdraw" << newLine << "3 - Deposit" << newLine << "4 - Transfer" << newLine << "5 - Display Account Details" << newLine << "6 - Delete Account" << newLine << "\nEnter choice: ";
        std::cin >> choice;

        switch(choice)
        {

            case 1:
            BankAccObj.openStockMarketMainMenu();
            break;

            case 2:
            BankAccObj.withdraw();
            break;

            case 3:
            BankAccObj.deposit();
            break;

            case 4:
            BankAccObj.transfer();
            break;

            case 5:
            BankAccObj.displayBankAccountDetails();
            break;

            case 6:
            BankAccObj.deleteAccount();
            break;

            default:
            std::cout << "\n\nINVALID INPUT - Please enter a number upto 5\n\n";
        }

    }while(choice>5);

}

void Menu::openStockMarket()
{
    int choice;

    do
    {  
        std::cout << newLine << "STOCK MARKET" << drawLine << newLine << "1 - Open Portfolio" << newLine << "2 - Place Order" << newLine << "3 - View Orders" << newLine << "4 - Search Symbols" << newLine << "5 - Display Account Details" << newLine << "6 - Return to Bank Account" << newLine << "\nEnter choice: ";
        std::cin >> choice;

        switch(choice)
        {

            case 1:
            StockMarketAccObj.showPortfolio();
            break;


            


            case 6:
            MenuObj.bankAccountMenu();
            break;

            default:
            std::cout << newLine << "\nINVALID INPUT - Please enter a number upto 5\n\n";
        }

    }while(choice>5);


}

void Menu::placeOrderStockMarket()
{


}

//      BANK ACCOUNT FUNCTIONS

//  CREATE ACCOUNT

void BankAccount::createAccount()
{
    getAccountDetails();
    generateAccountID();

    addRow(bankAccountDataBasefileName, rowItems, colNums);

    std::cout << newLine << "## ACCOUNT SUCCESSFULLY CREATED ##\n";
    displayBankAccountDetails();

    std::cout << newLine << "\n!! PLEASE RE-LOGIN TO CONTINUE USING YOUR NEW ACCOUNT !!\n";
    MenuObj.mainMenu();
}

void BankAccount::getAccountDetails()
{
    std::cout << newLine << "CREATE AN ACCOUNT" << drawLine << newLine << "\nEnter name: ";
    std::cin.get(bufferChar);
    std::string name;
    std::getline(std::cin, name);
    rowItems[4] = name;
    std::cout << newLine << "Enter Mail ID: ";
    std::cin >> rowItems[5];
    std::cout << newLine << "Enter initial deposit: ";
    std::cin >> balance;
    rowItems[2] = std::to_string(balance);
    std::cout << newLine << "Enter password for account: ";
    std::cin >> rowItems[1];

    //Sets stock market data to 0 as SM in account is not yet enabled
    rowItems[3] = '0';
    rowItems[6] = '0';
    //Make function to check valid mail and password
}

void BankAccount::generateAccountID()
{
    int randomComponent = rand();
    int lastEntryRowNumber;
    std::string latestAccountNumber;

    //Gets the latest account number in the data base
    lastEntryRowNumber = numberOfEntries(bankAccountDataBasefileName);
    latestAccountNumber = getDataItem(bankAccountDataBasefileName, lastEntryRowNumber, 0);
 
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

    std::cout<< newLine << "LOGIN TO ACCOUNT" << drawLine << newLine << "\nEnter Account Number: ";
    std::cin >> accNoInput;
    loginInput.push_back(accNoInput);
    std::cout << newLine << "Enter password: ";
    std::cin >> passwordInput;
    loginInput.push_back(passwordInput);

    loginSuccessStatus = findIfInRow(bankAccountDataBasefileName, loginInput, columnNumber);

    if(loginSuccessStatus)
    {
        std::cout<< newLine << "### LOGIN SUCCESSFULL ###\n";
        currentAccountRow = findRowNumber(bankAccountDataBasefileName, accNoInput, 0);
        setBankAccountData();
        MenuObj.bankAccountMenu();
    }
    else
    {
        std::cout<< newLine << "!!! INCORRECT ACCOUNT NUMBER OR PASSWORD !!!\n";
        MenuObj.mainMenu();
    }
}

void BankAccount::logoutBank()
{
    char choice;
    std::cout << newLine << "CONFIRM IF YOU WANT TO LOGOUT (Y/N): ";
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
    std::vector<std::string> rowItem = getRow(bankAccountDataBasefileName, currentAccountRow);

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
            stockMarketAccountStatus = stoi(rowItem[i]);
            break;

            case 4:
            name = rowItem[i];
            break;

            case 5:
            emailID = rowItem[i];
            break;

            case 6:
            panNumber = stoi(rowItem[i]);
            break;
        }
    }
}

//  BANK ACCOUNT OPERATIONS

void BankAccount::withdraw()
{
    int withdrawAmount;
    bool withdrawSuccessStatus = 0;

    std::cout << newLine << "Enter amount to withdraw: ";
    std::cin >> withdrawAmount;

    if(balance >= withdrawAmount)
    {
        balance-=withdrawAmount;
        changeDataItem(bankAccountDataBasefileName, std::to_string(balance), currentAccountRow, 2);
        std::cout << newLine << "\nSUCCESSFULLY WITHDRAWN" << drawLine << newLine << "Current Balance: " << balance;
    }
    else
    {
        std::cout << newLine << "Withdraw amount greater than balance." << std::endl;

    }

}

void BankAccount::deposit()
{
    int depositAmount;

    std::cout << newLine << "Enter amount to deposit: ";
    std::cin >> depositAmount;
    
    balance+=depositAmount;
    changeDataItem(bankAccountDataBasefileName, std::to_string(balance), currentAccountRow, 2);
    std::cout << newLine << "SUCCESSFULLY DEPOSITED" << drawLine << newLine << " Current Balance: " << balance << std::endl;
}

void BankAccount::transfer()
{
    int transferAmount;
    std::string transferAccountNumber;
    bool transferSuccessStatus = 0;

    std::string transferAccountBalanceString;
    int transferAccountBalance;
    int transferAccountRowNumber = 0;

    std::cout << newLine << "Enter account number to transfer to: ";
    std::cin >> transferAccountNumber;
    std::cout << newLine << "Enter amount to transfer: ";
    std::cin >> transferAmount;

    if(balance >= transferAmount)
    {
        balance-=transferAmount;

        //Changes balance in current account
        changeDataItem(bankAccountDataBasefileName, std::to_string(balance), currentAccountRow, 2);

        //Chnages balance in the reciever's account
        transferAccountRowNumber = findRowNumber(bankAccountDataBasefileName, transferAccountNumber, 0);
        transferAccountBalanceString = getDataItem(bankAccountDataBasefileName, transferAccountRowNumber, 2);
        transferAccountBalance = stoi(transferAccountBalanceString) + transferAmount;
        changeDataItem(bankAccountDataBasefileName, std::to_string(transferAccountBalance), transferAccountRowNumber, 2);

        std::cout << newLine << "SUCCESSFULLY TRANSFERRED" << newLine << "Current Balance: " << balance;

    }
    else
    {
        std::cout << newLine << "Withdraw amount greater than balance." << std::endl;

    }

}

void BankAccount::displayBankAccountDetails()
{
    char choice;
    std::cout << newLine << "BANK ACCOUNT DETAILS" << drawLine << newLine << "Name: " << name << newLine << "Account Number: "<<accountNumber<< newLine << "Balance: "<<balance<< newLine << "E-Mail ID: " << emailID << std::endl;
    std::cout << newLine << "Do you want to view your account password (Y/N): ";
    std::cin >> choice;
    if(choice=='Y'||choice=='y')
    {
        std::cout << newLine << "Password: " << password;
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
    std::cout << newLine << "ARE YOU SURE YOU WANT TO DELETE THE ACCOUNT?(Y/N): ";
    if(choice=='Y'||choice=='y')
    {
        std::cout << newLine << "\nEnter account number again: ";
        std::cin >> accountNumberInput;
        deleteAccountInput.push_back(accountNumberInput);
        std::cout << newLine << "Enter password: ";
        std::cin >> passwordInput;
        deleteAccountInput.push_back(passwordInput);

        successStatus = findIfInRow(bankAccountDataBasefileName, deleteAccountInput, columnNumber);
        if(successStatus)
        {
            findRowNumber(bankAccountDataBasefileName, accountNumberInput, 0);
            deleteRow(bankAccountDataBasefileName, rowNumberInput);
            std::cout << newLine << drawLine << "\nAccount successfully deleted.";
        }
        else
        {
            std::cout << newLine << drawLine <<"\nIncorrect Account number or password, please relogin.";
        }

        MenuObj.mainMenu();

    }
    else
    {
        MenuObj.bankAccountMenu();
    }

}

//  STOCK MARKET OPENING FOR BANK ACCOUNT

void BankAccount::openStockMarketMainMenu()
{
    int choice;
    do
    {
        std::cout << newLine << "STOCK MARKET MENU" << drawLine << newLine << "1 - Open Stock Market Account\n\t\t2 - Enable Stock Market\n\t\t3 - Return to Bank Account\n\t\t4 - Logout\n\n\t\tEnter Choice: ";
        std::cin >> choice;
        switch(choice)
        {
            case 1:
                if(stockMarketAccountStatus)
                {
                    //StockMarketAccObj.setStockMarketAccount(accountNumber);
                    MenuObj.openStockMarket();
                }
                else
                {
                    std::cout << "\n\t\t!! You have not enabled the stock market for your bank account.\n\t\t!! Please Enable it to open the market!!\n";
                }
            break;

            case 2:
            stockMarketAccountStatus = StockMarketAccObj.enableStockMarket(currentAccountRow, accountNumber);
            std::cout << "\n\t\t## STOCK MARKET ENABLED ##\n";
            break;

            case 3:
            MenuObj.bankAccountMenu();
            break;
        }
    }while(choice<4);

}


 //     STOCK MARKET FUNCTIONS

int StockMarketAccount::enableStockMarket(int accountRowNumberInut, std::string accountNumberInput)
{
    std::fstream file;

    changeDataItem(bankAccountDataBasefileName, "1\0", currentAccountRow, 3);

    accountNumber = accountNumberInput;
    accountPortfolioFile = "stockMarketDataBases\\portfolios\\" + accountNumber + "Portfolio.csv";
    
    file.open(accountPortfolioFile, std::ios::out);
    if (!file.is_open()) 
    {
        std::cerr << newLine << "\nError opening file1." << std::endl;
        return 0;  
    }

    file << "Symbol, Current Price, Today's Change, No. Of Shares, Total Value, Total Gain/Loss, Purchase Price" << std::endl;

    file.close();

    return 1;

}

//  STOCK MARKET ACCOUNT OPERATIONS

void StockMarketAccount::showPortfolio()
{
    int numOfEntries = numberOfEntries(accountPortfolioFile);
    std::vector<std::string> row;

    if(numOfEntries == 0)
    {
        std::cout << newLine << "## NO SHARES OWNED ##\n";
    }
    else
    {
        for(int i = 1; i < numOfEntries; i++)
        {
            row = getRow(accountPortfolioFile, i);
            std::cout<<"SYMBOL: " << row[0] << "\nDESCP: " << row[7] << "\nCURRENT PRICE: " << row[1] <<"\nTODAY'S CHANGE: " << row[2] << "\nNUMBER OF SHARES: " << row[3] << "\nTOTAL VALUE: " << row[4] << "\nPURCHASE";


        }

    }

}

