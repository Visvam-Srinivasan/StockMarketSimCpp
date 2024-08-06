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
        void setBankAccountData(int);

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

    addRow("accountDataBase.csv", rowItems, 5);

    cout << "\n\t## ACCOUNT SUCCESSFULLY CREATED ##\n";
    displayBankAccountDetails();

    cout << "\n\n\t!! PLEASE RE-LOGIN TO CONTINUE USING YOUR NEW ACCOUNT !!\n";
    MenuObj.mainMenu();
}

void BankAccount::getAccountDetails()
{
    cout << "\n\n\t\tCREATE AN ACCOUNT\n************************************************\n\nEnter name: ";
    cin.get(bufferChar);
    getline(cin, rowItems[3]);
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
    string row;

    fstream file;
    file.open("accountDataBase.csv", ios::ate|ios::in);
    if (!file.is_open()) 
    {
        cerr << "Error opening file in generation ID." << endl;
        return;  
    }

    file.seekg(0);
    while(getline(file, row))
    {
        entryNumber++;
    }
 
    if(entryNumber>0)
    {
        stringstream s(row);

        //Gets account number of the last entry
        getline(s, prevAccNumber, ',');

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

    file.close();

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
        setBankAccountData(currentAccountRow);
        MenuObj.bankAccountMenu();
    }
    else
    {
        cout<<"\n\n\t\t!!! INCORRECT ACCOUNT NUMBER OR PASSWORD !!!\n";
        MenuObj.mainMenu();
    }
}

//Stores indices of columns in the logged in account
void BankAccount::setColumnIndex(int rowInput)
{
    string line;
    int currentIndex = 0;

    fstream file;
    file.open("accountDataBase.csv", ios::in);
    for(int i = 0; i <= rowInput; i++)
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
    
    withdrawSuccessStatus = changeBalance(withdrawAmount, 1);

    if(withdrawSuccessStatus)
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

    cout << "\nEnter amount to deposit: ";
    cin >> depositAmount;

    changeBalance(depositAmount, 0);

    cout << "\nSUCCESSFULLY DEPOSITED\nCurrent Balance: " << balance << endl;
}

void BankAccount::transfer()
{
    int transferAmount;
    string transferAccountNumber;
    bool transferSuccessStatus = 0;
    string stringBalanceAmount;

    cout << "\nEnter account number to transfer to: ";
    cin >> transferAccountNumber;
    cout << "Enter amount to transfer: ";
    cin >> transferAmount;

    //Changes balance in sender's account
    transferSuccessStatus = changeBalance(transferAmount, 1);

    if(!transferSuccessStatus)
    {
        cout << "\nTransfer amount greater than balance." << endl;
        return;
    }
    else
    {
        //Changes balance in receiver's account
        changeBalance(transferAmount, 0, transferAccountNumber);
        cout << "\nSUCCESSFULLY TRANSFERED\nCurrent Balance: " << balance << endl;
    }

}

//Change balance of the current bank account
bool BankAccount::changeBalance(int changeByAmount, bool choiceOfOp)
{
    string row;
    string balanceAmountString;
    bool successStatus = 0;

    //vector<string> row;
    fstream file;
    file.open("accountDataBase.csv", ios::ate|ios::in|ios::out);
    if (!file.is_open()) 
    {
        cerr << "Error opening file." << endl;
        return 0;  
    }

    file.seekg(0);
    file.seekp(0);

    for(int i = 0; i <= currentAccountRow; i++)
    {
        getline(file, row);
    } 

            //cout << endl << row << endl;

    //Gets balance substring from the row
    balanceAmountString = row.substr(columnIndex[1], columnIndex[2] - columnIndex[1] - 1);

    cout << balanceAmountString << endl;

    //Deposit
    if(choiceOfOp == 0)
    {
        balance+=changeByAmount;
        successStatus = 1;
    }
    //Withdraw
    else
    {
        if(changeByAmount>stoi(balanceAmountString))
        {
            cout << "in" << endl;
            return successStatus;
        }
        else
        {
            balance-=changeByAmount;
            successStatus = 1;
        }

    }
            cout << endl << "SA i p: " << file.tellg();
    //Moves put pointer to start of current account's row
    file.seekp(-(row.length() + 2), ios::cur);

            cout << endl << "SA m p: " << file.tellg();


    //Erases data in row
    for(int i = 0; i < row.length() + 1; i++)
    {
        file << '#';
    }

    //Moves put pointer back to start of row
    file.seekp(-(row.length() + 1), ios::cur);

                cout << endl << "SA e p: " << file.tellg();


    file<<accountNumber<<','<<password<<','<<balance<<','<<name<<','<<emailID<<endl;

    cout << endl << "SA e end: " << file.tellg();

    file.close();
    file.clear();

    return successStatus;
}

//Change balance of input bank account
bool BankAccount::changeBalance(int changeByAmount, bool choiceOfOp, string accNumberInput)
{
    string row;
    string balanceAmountString;

    string accNumber;
    int balanceOfInputAccount;
    int rowNumber = 0;

    bool foundAccFlag = 0;

    fstream file;
    file.open("accountDataBase.csv", ios::ate|ios::in|ios::out);
    if (!file.is_open()) 
    {
        cerr << "Error opening file." << endl;
        return 0;  
    }

    file.seekg(0);
    file.seekp(0);

    //Finding input account number
    while(getline(file, row))
    {
        stringstream s(row);
        getline(s, accNumber, ',');
                //cout << endl << accNumber << endl;
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
        setColumnIndex(rowNumber);
        //Gets balance substring from the row
        balanceAmountString = row.substr(columnIndex[1], columnIndex[2] - columnIndex[1] - 1);

        cout << endl << "Balance amm: " << balanceAmountString << endl;

        //Deposit
        if(choiceOfOp == 0)
        {
            balanceOfInputAccount = stoi(balanceAmountString) + changeByAmount;
        }

        cout << "row " << row << endl << "pos: " << file.tellp();

        setBankAccountData(rowNumber);

        //Erares row of input bank account
        file.seekp(-(row.length() + 3), ios::cur);
        cout << "\nnew pos: " << file.tellp();
        for(int i = 0; i < row.length(); i++)
        {
            file << '$';
        }
        file.seekp(-(row.length()), ios::cur);

        //Sets bank account class data members to the input account data

        //Assigns new balance
        balance = balanceOfInputAccount;

        file<<accountNumber<<','<<password<<','<<balance<<','<<name<<','<<emailID<<endl;

    }

    file.close();

    //Resets bank account class data members to current account data
    setBankAccountData(currentAccountRow);
    setColumnIndex(currentAccountRow);

    return 1;
}

//Assigns bank account class' members with input row number's data
void BankAccount::setBankAccountData(int inputRowNumber)
{
    string row;
    string rowDatas;

    fstream file;
    file.open("accountDataBase.csv", ios::in);
    if (!file.is_open()) 
    {
        cerr << "Error opening file." << endl;
        return;  
    }

    file.seekg(0);

    for(int i = 0; i <=inputRowNumber; i++)
    {
        getline(file, row);
    }

    stringstream s(row);

    //Assigns row members to class members
    for(int i = 0; i < 5; i++)
    {
        getline(s, rowDatas, ',');
        switch(i)
        {
            case 0:
            accountNumber = rowDatas;
            break;
            case 1:
            password = rowDatas;
            break;
            case 2:
            balance = stoi(rowDatas);
            break;
            case 3:
            name = rowDatas;
            break;
            case 4:
            emailID = rowDatas;
            break;
        }
    }

    file.close();
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
{}


int main()
{
    //Sets seed of random value generator to the current time
    srand(time(0));
    MenuObj.mainMenu();
    return 0;
}