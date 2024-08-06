#ifndef csvLibrary
#define csvLibrary

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>

void addRow(std::string fileName, std::string row[], int noOfColumns);

bool findInRow(std::string fileName, std::vector<std::string> columnInput, std::vector<int> columnNumberInput);

#endif