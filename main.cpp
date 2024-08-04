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

        //Functions for creating bank account
        void createAccount();
        void getAccountDetails();
        void generateAccountID();

        //Functions for login and logout
        void loginBank();
        bool validateLogin(string , string);
        void setColumnIndex();
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

    fstream file;
    file.open("accountDataBase.csv", ios::app | ios::in | ios::out);
    if (!file.is_open()) 
    {
        cerr << "Error opening file in generation ID." << endl;
        return;  
    }

    file<<endl<<accountNumber<<','<<password<<','<<balance<<','<<name<<','<<emailID;

    file.close();

    cout << "\n\t## ACCOUNT SUCCESSFULLY CREATED ##\n";
    displayBankAccountDetails();

    cout << "\n\t!! PLEASE RE-LOGIN TO CONTINUE USING YOUR NEW ACCOUNT !!\n";
    MenuObj.mainMenu();
}

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
    }
    else
    {
        //Generates account number for the first account created
        accountNumber = to_string(randomComponent) + to_string(1000);
    }

    file.close();

}


//LOGIN ACCOUNT

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

bool BankAccount::validateLogin(string accountNumInput, string passwordInput)
{
    string row; 
    string columnValues;

    int columnNumber = 0;
    int rowNumber = 0;
    bool loginSuccessStatus = 0;
    bool skipFirstLine = 0;

    fstream file;
    file.open("accountDataBase.csv", ios::app | ios::in);
    if (!file.is_open()) 
    {
        cerr << "Error opening file." << endl;
        return 0;  
    }

    while(getline(file, row))
    {   
        columnNumber = 0;

        //Skips the header of the csv file
        if(skipFirstLine == 0)
        {
            skipFirstLine = 1;
            rowNumber++;
            continue;
        }

        //Splits string line into distinct data items onto the stream 's'
        stringstream s(row);

        //Iterates through distinct data items separated by ',' items from stream 's' 
        while(getline(s, columnValues, ','))
        {
            //Checking if input values match
            if(columnNumber == 0 && columnValues != accountNumInput)
                break;
            else 
            {
                if(columnNumber == 1 && columnValues == passwordInput) 
                {
                    loginSuccessStatus = 1;
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

        if(loginSuccessStatus)
            break;
        else
            rowNumber++; 
    }

    if(loginSuccessStatus)
    {
        currentAccountRow = rowNumber;
        setColumnIndex();
    }

    file.close();

    return loginSuccessStatus;
}

//Stores indices of columns in the logged in account
void BankAccount::setColumnIndex()
{
    string line;
    int currentIndex = 0;

    fstream file;
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

    //Gets balance substring from the row
    balanceAmountString = row.substr(columnIndex[1], columnIndex[2] - columnIndex[1] - 1);

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
            return successStatus;
        }
        else
        {
            balance-=changeByAmount;
            successStatus = 1;
        }

    }

    //Moves put pointer to start of current account's row
    file.seekp(-(row.length() + 2), ios::cur);

    //Erases data in row
    for(int i = 0; i < row.length(); i++)
    {
        file << '#';
    }

    //Moves put pointer back to start of row
    file.seekp(-(row.length()), ios::cur);

    file<<accountNumber<<','<<password<<','<<balance<<','<<name<<','<<emailID;

    file.close();

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
        balanceAmountString = row.substr(columnIndex[1], columnIndex[2] - columnIndex[1] - 1);

        //Deposit
        if(choiceOfOp == 0)
        {
            balanceOfInputAccount = stoi(balanceAmountString) + changeByAmount;
        }
    
        //Erares row of input bank account
        file.seekp(-(row.length() + 2), ios::cur);
        for(int i = 0; i < row.length(); i++)
        {
            file << '#';
        }
        file.seekp(-(row.length()), ios::cur);

        //Sets bank account class data members to the input account data
        setBankAccountData(rowNumber);

        //Assigns new balance
        balance = balanceOfInputAccount;

        file<<accountNumber<<','<<password<<','<<balance<<','<<name<<','<<emailID;

    }

    file.close();

    //Resets bank account class data members to current account data
    setBankAccountData(currentAccountRow);

    return 1;
}

//Assigns bank account class' members with input row number's data
void BankAccount::setBankAccountData(int inputRowNumber)
{
    string row;
    string rowItems;

    fstream file;
    file.open("accountDataBase.csv", ios::in);
    if (!file.is_open()) 
    {
        cerr << "Error opening file." << endl;
        return;  
    }

    for(int i = 0; i <=inputRowNumber; i++)
    {
        getline(file, row);
    }

    stringstream s(row);

    //Assigns row members to class members
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

    file.close();
}

void BankAccount::displayBankAccountDetails()
{
    char choice;
    cout << "\n\t\tBANK ACCOUNT DETAILS\n************************************************\n\t\tName: " << name <<"\n\t\tAccount Number: "<<accountNumber<<"\n\t\tBalance: "<<balance<<"\n\t\tE-Mail ID: " << emailID << endl;
    cout << "\tDo you want to view your account password (Y/N): ";
    cin >> choice;
    if(choice=='Y'||choice=='y')
    {
        cout << "\n\t\tPassword: " << password;
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