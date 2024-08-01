#include<iostream>
#include<fstream>

#include<vector>
#include<string>
#include<sstream>
#include<stdlib.h>
#include<iomanip>
#include<math.h>

using namespace std;


//Classes

class Portfolio;


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
        int accountNum;
        int balance;

        int currentAccountIndex;
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




}accObj;


//Main menu function
void Menu::mainMenu()
{
    int choice;
    cout << "\t\tSTOCK MARKET SIM\n************************************************\n1 - Login\n2 - Create Account\n\nEnter Choice: ";
    cin >> choice;
    switch(choice)
    {
        case 1:
        accObj.loginBank();
        break;


        case 2:
        accObj.createAccount();
        break;
    }
}


void Account::getAccountDetails()
{
    cout << "\t\tCREATE AN ACCOUNT\n\t****************************\n\nEnter name: ";
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


//Create bank account
void Account::createAccount()
{
    getAccountDetails();

    fstream file;
    file.open("accountDataBase.dat", ios::app | ios::in | ios::out);
    file<<name<<" "<<password<<" "<<balance<<" "<<emailID<<endl;
    file.close();
}


void Account::loginBank()
{
    int accNoInput;
    string passwordInput;
    bool loginSuccessStatus = 0;
    cout<<"\t\tLOGIN TO ACCOUNT\n\t************************\n\nEnter Account Number: ";
    cin >> accNoInput;
    cout << "Enter password: ";
    cin >> passwordInput;

    loginSuccessStatus = validateLogin(to_string(accNoInput), passwordInput);

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

    file.open("accountDataBase.dat", ios::in);

    for(int i = 0; i <= currentAccountIndex; i++)
    {
        file>>"\n";
    }

    int currentIndex = 1;
    getline(file, line);

    for(int i = 0; i < line.length(); i++)
    {
        if(line[i] == ' ')
        {
            columnIndex[currentIndex] = i+1;
            currentIndex++;
        }
    }

}

//Validates the credentials for login provided by user
bool Account::validateLogin(string accountNumInput, string passwordInput)
{
    string line;
    string column;
    fstream file;
    int columnNumber = 0;
    int lineNumber = 0;
    bool successLoginFlag = 0;

    file.open("accountDataBase.dat", ios::app | ios::in);

    while(getline(file, line))
    {   

        stringstream s(line);

        while(s >> column)
        {
            if(columnNumber == 0 && column != accountNumInput)
                break;
            else 
            {
                if(columnNumber == 1 && column == passwordInput) 
                {
                    successLoginFlag = 1;
                    break;
                }
                else
                {
                    if(columnNumber>=1) 
                        break;
                    else
                        continue;
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
        currentAccountIndex = lineNumber;
        setColumnIndex();
    }


    file.close();
    return successLoginFlag;

}

