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
    void accountMenu();

}MenuObj;

class Account
{
    private:

        string name; 
        string emailID;
        string password;
        string accountID;
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
    cout << "\t\tSTOCK MARKET SIM\n************************************************\n1 - Login\n2 - Create Account\n\nEnter Choice: ";
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
void Menu::accountMenu()
{
    int choice;

    do
    {  
        cout << "\t\tACCOUNT MENU\n\t****************************\n\n\t1 - Display Account Details\n\t2 - Withdraw\n\t3 - Deposit\n\t4 - Logout\n\n5 - Delete Account\n\n\tEnter choice: ";
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

//Get details to create bank account
void Account::getAccountDetails()
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
    //Removes any blank spaces in the name
    for(int i = 0; i < name.length(); i++)
    {
        if(name[i]==' ')
            {
                name[i] = '#';
            }
    }
}

//Creates account and stores data
void Account::createAccount()
{
    getAccountDetails();
    generateAccountID();

    fstream file;
    file.open("accountDataBase.csv", ios::app | ios::in | ios::out);
    file<<accountID<<','<<password<<','<<name<<','<<balance<<','<<emailID<<endl;
    file.close();

    MenuObj.mainMenu();
}

//Generates unique AccountID for new account
void Account::generateAccountID()
{
    fstream file;
    string sequenceComponent;
    int randomComponent;

    file.open("accountDataBase.cvv", ios::in | ios::out | ios::app);

    //Initially sets the sequential component of the accountID and saves it onto the accounts data base
    if(file.tellg()==0)
    {
        file << 1000 << endl;
    }

    //Getting unique first sequence for account ID
    file.seekg(0, ios::beg);
    getline(file, sequenceComponent);

    //Generating random sequence for account ID
    int randomComponent = rand();

    accountID = sequenceComponent + to_string(randomComponent);

    file.close();

    updateSequenceAccIDComponent();
}

//Updates the initial sequence to the Account ID creation in the database
void Account::updateSequenceAccIDComponent()
{
    fstream file;
    string line;
    int sequenceComponent;

    file.open("accountsDataBase.cvv", ios::in|ios::out);
    file.seekg(0, ios::beg);
    file.seekp(0, ios::beg);

    //Incrementing unique initial sequence
    getline(file, line);
    sequenceComponent = stoi(line);
    sequenceComponent++;

    //Storing incremented sequence
    file << to_string(sequenceComponent) << endl;

    file.close();
}

//Login to bank
void Account::loginBank()
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
        cout<<"\n\n\t\t### LOGIN SUCCESSFULL ###";
        MenuObj.accountMenu();
    }
    else
    {
        cout<<"\n\n\t\t!!! INCORRECT ACCOUNT NUMBER OR PASSWORD !!!";
        MenuObj.mainMenu();
    }
}

//Stores indices of columns in the logged in account
void Account::setColumnIndex()
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

//Validates the credentials for login provided by user
bool Account::validateLogin(string accountNumInput, string passwordInput)
{
    string line;
    string columnValues;
    fstream file;

    int columnNumber = 0;
    int lineNumber = 0;
    bool successLoginFlag = 0;

    file.open("accountDataBase.csv", ios::app | ios::in);

    while(getline(file, line))
    {   
        //Splits string line into distinct data items onto the stream 's'
        stringstream s(line);

        //Iterates through distinct data items from stream 's'
        while(s >> columnValues)
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

//Logout
void Account::logoutBank()
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
        MenuObj.accountMenu();
    }
}

int main()
{
    //Sets seed of random value generator to the current time
    srand(time(0));
    MenuObj.mainMenu();
    return 0;
}