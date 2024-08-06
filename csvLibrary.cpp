#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>

#include "csvLibrary.h"


void addRow(std::string fileName, std::string row[], int noOfColumns)
{

    std::fstream file;
    file.open(fileName, std::ios::app | std::ios::in | std::ios::out);
    if (!file.is_open()) 
    {
        std::cerr << "Error opening file in generation ID." << std::endl;
        return;  
    }

    file << std::endl;
    for(int i = 0; i < noOfColumns; i++)
    {
        std::cout << row[i] << std::endl;
        file << row[i];
        if(i == noOfColumns - 1)
            break;
        file << ",";
    }

    file.close();
}

bool findInRow(std::string fileName, std::vector<std::string> columnInput, std::vector<int> columnNumberInput)
{
    std::string row;
    int skipFirstLine = 0;
    int currentrowNumber = 0;
    int currentColumnNumber = 0;
    bool findSuccessStatus = 0;
    std::string column;
    std::fstream file;
    file.open(fileName, std::ios::in);
    if (!file.is_open()) 
    {
        std::cerr << "Error opening file." << std::endl;
        return 0;  
    }

    int i = 0;
     while(std::getline(file, row))
    { 

        //Skips the header of the csv file
        if(skipFirstLine == 0)
        {
            skipFirstLine = 1;
            currentrowNumber++;
            continue;
        }

        //Splits string line into distinct data items onto the stream 's'
        std::stringstream s(row);

        //Iterates through distinct data items separated by ',' items from stream 's' 
        while(std::getline(s, column, ','))
        {
            //Checking if input values match
            if(currentColumnNumber != columnNumberInput[i])
            {
                currentColumnNumber++;
                continue;
            }
            else
            {
                if(column == columnInput[i])
                {
                    findSuccessStatus = 1;
                    currentColumnNumber++;
                    i++;
                    continue;
                }
                else
                {
                    findSuccessStatus = 0;
                    break;
                }
            }
            
        }

        if(findSuccessStatus)
        {
            return findSuccessStatus;
        }
        else
        {
            continue;
        }

    }   

    return findSuccessStatus;

}

