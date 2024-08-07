#ifndef csvLibrary
#define csvLibrary

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>

void addRow(std::string fileName, std::string row[], int noOfColumns);

bool findIfInRow(std::string fileName, std::vector<std::string> columnInput, std::vector<int> columnNumberInput);
void changeDataItem(std::string fileName, std::string newStringInput, int rowNumberInput, int columnNumberInput);

int findRowNumber(std::string fileName, std::string stringInput, int columnNumberInput);


#endif