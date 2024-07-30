#include<iostream>
#include<fstream>
#include<vector>
#include<string.h>
#include<stdlib.h>
#include<iomanip>
#include<math.h>

using namespace std;


//Classes

class Portfolio;

class Account
{
    private:
        string name; 
        int accountNum;
        int balance;

    public:
        void getAccountInfo();
        void displayAccountDetails();
        void deposit();
        void withdraw();

}

