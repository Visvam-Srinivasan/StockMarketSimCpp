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
#include <chrono>
#include <ctime>

#include "csvLibrary.h"

//      DATABASE FILE NAMES

std::string bankAccountDataBasefileName = "bankAccountDataBase.csv";
std::string sellOrderBookFile = "stockMarketDataBases\\sellOrderBook.csv";
std::string buyOrderBookFile = "stockMarketDataBases\\buyOrderBook.csv";
std::string companyInfoDBFile = "stockMarketDataBases\\companyShareData\\comapanyInfo.csv";
std::string companyMarkPriceHistoryFile = "stockMarketDataBases\\companyShareData\\companyMarketPriceHistory.csv";

int const colNums = 7;
int const numOfCompanies = 5;

char bufferChar;
 
//      CONSOLE OUTPUT FORMATTERS

std::ostream &newLineBody(std::ostream &output)
{
    output << "\n\t\t\t\t";
    return output;
}

std::ostream &newLineHeader(std::ostream &output)
{
    output << "\n\n\t\t\t\t\t\t";
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
    void openStockMarketMenu();
    void placeOrderStockMarketMenu();

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
    int balanceSM;


    public:

    void setStockMarketAccount(std::string);
    void showPortfolio();
    int enableStockMarket(int, std::string);
    void searchSymbolStockMarket();
    void placeOrderMenuStockMarket();
    void buySharesStockMarket(std::string, int);
    void setData(int balanceInput, std::string accountNumberInput)
    {
        balanceSM = balanceInput;
        accountNumber = accountNumberInput;
    }
    bool chkIfCanBuy(int);
    void addBuyOrder(int durationTypeInput, std::string symbolInput, int numOfSharesInput, std::string bidPrice, int limitInput, int tirggerInput);

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
    std::cout << newLineHeader << "STOCK MARKET SIM" << drawLine << newLineBody << "1 - Login" << newLineBody <<"2 - Create Bank and Market Account" << std::endl << newLineBody <<"Enter Choice: ";
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
        std::cout << newLineHeader<< "ACCOUNT MENU" << drawLine << newLineBody <<  "1 - STOCK MARKET MENU" << newLineBody << "2 - Withdraw" << newLineBody << "3 - Deposit" << newLineBody << "4 - Transfer" << newLineBody << "5 - Display Account Details" << newLineBody << "6 - Delete Account" << std::endl << newLineBody << "Enter choice: ";
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

void Menu::openStockMarketMenu()
{
    int choice;

    do
    {  
        std::cout << newLineHeader << "STOCK MARKET" << drawLine << newLineBody << "1 - Open Portfolio" << newLineBody << "2 - Place Order" << newLineBody << "3 - View Orders" << newLineBody << "4 - Search Symbols" << newLineBody << "5 - Display Account Details" << newLineBody << "6 - Return to Bank Account" << std::endl << newLineBody << "Enter choice: ";
        std::cin >> choice;

        switch(choice)
        {

            case 1:
            StockMarketAccObj.showPortfolio();
            break;

            case 4:
            StockMarketAccObj.searchSymbolStockMarket();
            break;


            


            case 6:
            MenuObj.bankAccountMenu();
            break;

            default:
            std::cout << newLineBody << "\nINVALID INPUT - Please enter a number upto 5\n\n";
        }

    }while(choice<5);


}

void Menu::placeOrderStockMarketMenu()
{
    int choice;

    do
    {  
        std::cout << newLineHeader << "PLACE ORDER" << drawLine << newLineBody << "1 - Search Symbol" << newLineBody << "2 - Place Order" << std::endl << newLineBody << "Enter choice: ";
        std::cin >> choice;

        switch(choice)
        {

            case 1:
            StockMarketAccObj.searchSymbolStockMarket();
            break;

            case 2:
            StockMarketAccObj.placeOrderMenuStockMarket();
            break;

            default:
            std::cout << newLineBody << "\nINVALID INPUT - Please enter a number upto 5\n\n";
        }

    }while(choice<5);



}

//      BANK ACCOUNT FUNCTIONS

//  CREATE ACCOUNT

void BankAccount::createAccount()
{
    getAccountDetails();
    generateAccountID();

    addRow(bankAccountDataBasefileName, rowItems, colNums);

    std::cout << newLineBody << "## ACCOUNT SUCCESSFULLY CREATED ##\n";
    displayBankAccountDetails();

    std::cout << newLineBody << "\n!! PLEASE RE-LOGIN TO CONTINUE USING YOUR NEW ACCOUNT !!\n";
    MenuObj.mainMenu();
}

void BankAccount::getAccountDetails()
{
    std::cout << newLineHeader << "CREATE AN ACCOUNT" << drawLine << newLineBody << "\nEnter name: ";
    std::cin.get(bufferChar);
    std::string name;
    std::getline(std::cin, name);
    rowItems[4] = name;
    std::cout << newLineBody << "Enter Mail ID: ";
    std::cin >> rowItems[5];
    std::cout << newLineBody << "Enter initial deposit: ";
    std::cin >> balance;
    rowItems[2] = std::to_string(balance);
    std::cout << newLineBody << "Enter password for account: ";
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

    std::cout<< newLineHeader << "LOGIN TO ACCOUNT" << drawLine  << newLineBody << "Enter Account Number: ";
    std::cin >> accNoInput;
    loginInput.push_back(accNoInput);
    std::cout << newLineBody << "Enter password: ";
    std::cin >> passwordInput;
    loginInput.push_back(passwordInput);

    loginSuccessStatus = findIfInRow(bankAccountDataBasefileName, loginInput, columnNumber);

    if(loginSuccessStatus)
    {
        std::cout<< newLineBody << "### LOGIN SUCCESSFULL ###\n";
        currentAccountRow = findRowNumber(bankAccountDataBasefileName, accNoInput, 0);
        setBankAccountData();
        MenuObj.bankAccountMenu();
    }
    else
    {
        std::cout<< newLineBody << "!!! INCORRECT ACCOUNT NUMBER OR PASSWORD !!!\n";
        MenuObj.mainMenu();
    }
}

void BankAccount::logoutBank()
{
    char choice;
    std::cout << newLineBody << "CONFIRM IF YOU WANT TO LOGOUT (Y/N): ";
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

    std::cout << newLineBody << "Enter amount to withdraw: ";
    std::cin >> withdrawAmount;

    if(balance >= withdrawAmount)
    {
        balance-=withdrawAmount;
        changeDataItem(bankAccountDataBasefileName, std::to_string(balance), currentAccountRow, 2);
        std::cout << newLineBody << "\nSUCCESSFULLY WITHDRAWN" << drawLine << newLineBody << "Current Balance: " << balance;
    }
    else
    {
        std::cout << newLineBody << "Withdraw amount greater than balance." << std::endl;

    }

}

void BankAccount::deposit()
{
    int depositAmount;

    std::cout << newLineBody << "Enter amount to deposit: ";
    std::cin >> depositAmount;
    
    balance+=depositAmount;
    changeDataItem(bankAccountDataBasefileName, std::to_string(balance), currentAccountRow, 2);
    std::cout << newLineBody << "SUCCESSFULLY DEPOSITED" << drawLine << newLineBody << " Current Balance: " << balance << std::endl;
}

void BankAccount::transfer()
{
    int transferAmount;
    std::string transferAccountNumber;
    bool transferSuccessStatus = 0;

    std::string transferAccountBalanceString;
    int transferAccountBalance;
    int transferAccountRowNumber = 0;

    std::cout << newLineBody << "Enter account number to transfer to: ";
    std::cin >> transferAccountNumber;
    std::cout << newLineBody << "Enter amount to transfer: ";
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

        std::cout << newLineBody << "SUCCESSFULLY TRANSFERRED" << newLineBody << "Current Balance: " << balance;

    }
    else
    {
        std::cout << newLineBody << "Withdraw amount greater than balance." << std::endl;

    }

}

void BankAccount::displayBankAccountDetails()
{
    char choice;
    std::cout << newLineHeader << "BANK ACCOUNT DETAILS" << drawLine << newLineBody << "Name: " << name << newLineBody << "Account Number: "<<accountNumber<< newLineBody << "Balance: "<<balance<< newLineBody << "E-Mail ID: " << emailID << std::endl;
    std::cout << newLineBody << "Do you want to view your account password (Y/N): ";
    std::cin >> choice;
    if(choice=='Y'||choice=='y')
    {
        std::cout << newLineBody << "Password: " << password;
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
    std::cout << newLineBody << "ARE YOU SURE YOU WANT TO DELETE THE ACCOUNT?(Y/N): ";
    if(choice=='Y'||choice=='y')
    {
        std::cout << newLineBody << "\nEnter account number again: ";
        std::cin >> accountNumberInput;
        deleteAccountInput.push_back(accountNumberInput);
        std::cout << newLineBody << "Enter password: ";
        std::cin >> passwordInput;
        deleteAccountInput.push_back(passwordInput);

        successStatus = findIfInRow(bankAccountDataBasefileName, deleteAccountInput, columnNumber);
        if(successStatus)
        {
            findRowNumber(bankAccountDataBasefileName, accountNumberInput, 0);
            deleteRow(bankAccountDataBasefileName, rowNumberInput);
            std::cout << newLineBody << drawLine << "\nAccount successfully deleted.";
        }
        else
        {
            std::cout << newLineBody << drawLine <<"\nIncorrect Account number or password, please relogin.";
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
        std::cout << newLineHeader << "STOCK MARKET MENU" << drawLine << newLineBody << "1 - Open Stock Market Account" << newLineBody << "2 - Enable Stock Market" << newLineBody << "3 - Return to Bank Account" << newLineBody << "4 - Logout" << std::endl << newLineBody << "Enter Choice: ";
        std::cin >> choice;
        switch(choice)
        {
            case 1:
                if(stockMarketAccountStatus)
                {
                    //StockMarketAccObj.setStockMarketAccount(accountNumber);
                    StockMarketAccObj.setData(balance, accountNumber);
                    MenuObj.openStockMarketMenu();
                }
                else
                {
                    std::cout << "\n\t\t!! You have not enabled the stock market for your bank account.\n\t\t!! Please Enable it to open the market!!\n";
                }
            break;

            case 2:
            stockMarketAccountStatus = StockMarketAccObj.enableStockMarket(currentAccountRow, accountNumber);
            StockMarketAccObj.setData(balance, accountNumber);
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
        std::cerr << newLineBody << "\nError opening file1." << std::endl;
        return 0;  
    }

    file << "Symbol, No. Of Shares, Purchase Price, Order Status";

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
        std::cout << newLineBody << "## NO SHARES OWNED ##\n";
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

void StockMarketAccount::placeOrderMenuStockMarket()
{
    std::string symbolInput;
    char actionInput;
    int numberOfShares;
    int rowNumberOfSymbol;

    std::cout << newLineBody << "Enter Symbol: ";
    std::cin >> symbolInput;
    findRowNumber(companyInfoDBFile, symbolInput, 0);
    do
    {
        std::cout << newLineBody << "Select Action (B - buy/S - Sell): ";
        std::cin >> actionInput;
        if(actionInput == 'B' || actionInput == 'b')
        {

        }
        else if(actionInput == 'S' || actionInput == 's')
        {

        }
        else
        {
            std::cout << newLineBody << "!! INVALID INPUT TRY AGAIN !!\n";
        }

    }while(actionInput != 'B' || actionInput != 'b' || actionInput != 'S' || actionInput != 's');
    

}

void StockMarketAccount::buySharesStockMarket(std::string symbolInput, int symbolRowNumberInput)
{
    int choice;
    int numberOfShares;
    int durationType;
    int limitPrice;
    int triggerPrice;

    std::string currentMarketPriceString = getDataItem(companyInfoDBFile, symbolRowNumberInput, 1);
    int currentMarketPrice = stoi(currentMarketPriceString);
    do
    {
        std::cout << newLineHeader << "BUYING " << symbolInput << drawLine << newLineBody << "Choose Order Type: " << newLineBody << "1 - Market Price Order" << newLineBody << "2 - Limit Order" << newLineBody << "3 - Stop Limit Order" << newLineBody << "\nEnter Choice: ";  
        std::cin >> choice;
        switch(choice)
        {
            case 1:
            std::cout << newLineBody << "Current Market Price: " << currentMarketPrice << newLineBody << "Enter number of Shares: "; 
            std::cin >> numberOfShares;
            std::cout << newLineBody << "Choose duration (0 - Day/1 - Good Unitl cancelled): " << durationType;
            if(chkIfCanBuy(currentMarketPrice*numberOfShares))
            {
                balanceSM-=currentMarketPrice*numberOfShares;

                addBuyOrder(durationType, symbolInput, numberOfShares, currentMarketPriceString, 0, -1);

                //Change only when order is a success
                changeDataItem(bankAccountDataBasefileName, std::to_string(balanceSM), currentAccountRow, 2);
                break;
            }
            else
            {
                break;
            }

            case 2:
            std::cout << newLineBody << "Current Market Price: " << currentMarketPrice << newLineBody << "Enter limit price: "; 
            std::cin >> limitPrice;
            std::cout << newLineBody << "Enter number of shares: ";
            std::cin >> numberOfShares;
            std::cout << newLineBody << "Choose duration (0 - Day/1 - Good Unitl cancelled): " << durationType;
            if(chkIfCanBuy(limitPrice*numberOfShares))
            {
                balanceSM-=currentMarketPrice*numberOfShares;

                addBuyOrder(durationType, symbolInput, numberOfShares, std::to_string(limitPrice), 1, -1);

                //Change only when order is a success
                changeDataItem(bankAccountDataBasefileName, std::to_string(balanceSM), currentAccountRow, 2);
                break;
            }
            else
            {
                break;
            }

            case 3:
            std::cout << newLineBody << "Current Market Price: " << currentMarketPrice << newLineBody << "Enter limit price: "; 
            std::cin >> limitPrice;
            std::cout << newLineBody << "Enter trigger price: ";
            std::cin >> triggerPrice;           
            std::cout << newLineBody << "Enter number of shares: ";
            std::cin >> numberOfShares;
            std::cout << newLineBody << "Choose duration (0 - Day/1 - Good Unitl cancelled): " << durationType;
            if(chkIfCanBuy(limitPrice*numberOfShares))
            {
                balanceSM-=currentMarketPrice*numberOfShares;

                addBuyOrder(durationType, symbolInput, numberOfShares, std::to_string(limitPrice), 1, triggerPrice);

                //Change only when order is a success
                changeDataItem(bankAccountDataBasefileName, std::to_string(balanceSM), currentAccountRow, 2);
                break;
            }
            else
            {
                break;
            }


        }

    }while(choice<5);

}

bool StockMarketAccount::chkIfCanBuy(int amount)
{   
    if(amount>balanceSM)
    {
        std::cout << newLineBody << "Total cost exceeds balance\n";
        return 0;
    }
    else
    {
        return 1;
    }

}

std::string getSystemTime();

void StockMarketAccount::addBuyOrder(int durationTypeInput, std::string symbolInput, int numOfSharesInput, std::string bidPrice, int limitInput, int tirggerInput)
{
    std::string orderData[9];
    std::string orderInfoToPortfolio[4];

    //Add data to order book
    std::string orderNumber = std::to_string(stoi(getDataItem(buyOrderBookFile, numberOfEntries(buyOrderBookFile), 0)) + 1);
    orderData[0] = orderNumber;
    orderData[1] = std::to_string(durationTypeInput);
    orderData[2] = accountNumber;
    orderData[3] = symbolInput;
    orderData[4] = std::to_string(numOfSharesInput);
    orderData[5] = bidPrice;
    orderData[6] = std::to_string(limitInput);
    orderData[7] = std::to_string(tirggerInput);
    orderData[8] = getSystemTime();
    addRow(buyOrderBookFile, orderData, 9);

    //Add order to portfolio
    orderInfoToPortfolio[0] = symbolInput;
    orderInfoToPortfolio[1] = std::to_string(numOfSharesInput);
    orderInfoToPortfolio[2] = bidPrice;
    orderInfoToPortfolio[3] = "0\0";
    addRow(accountPortfolioFile, orderInfoToPortfolio, 4);

}

void StockMarketAccount::searchSymbolStockMarket()
{
    std::vector<std::string> rowItem;
    int symbolRowNumber;
    std::string symbolInput;
    char choice;
    std::cout << newLineHeader << "SEARCH SYMBOL" << drawLine << newLineBody << "Enter symbol: ";
    std::cin >> symbolInput;
    symbolRowNumber = findRowNumber(companyInfoDBFile, symbolInput, 0);
    if(symbolRowNumber==0)
    {
        std::cout << newLineBody << "## Given Symbol does not exist. Try Again. ##";
        return;
    }
    else
    {
        rowItem = getRow(companyInfoDBFile, symbolRowNumber);
        std::cout << newLineHeader << rowItem[0] << drawLine << newLineBody << "Company Name: " << rowItem[3] << newLineBody << "Current Price: " << rowItem[1] << newLineBody << "Number Of Outstanding shares: " << rowItem[2] << newLineBody << "Do you want to view the trends (Y/N): ";
        std::cin >> choice;
        if(choice=='y'||choice=='Y')
        {


        }
        else
        {
            return;
        }


    }

}







std::string getSystemTime()
{
        // Get the current time from the system clock
    auto now = std::chrono::system_clock::now();

    // Convert current time to time_t to obtain a calendar time
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    // Convert time_t to tm structure for local time
    std::tm* localTime = std::localtime(&currentTime);

    // Format the date as a string in YYYY-MM-DD format
    std::ostringstream dateStream;
    dateStream << std::put_time(localTime, "%Y-%m-%d");

    // Return the formatted date string
    return dateStream.str();
}
