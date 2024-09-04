#ifndef csvLibrary
#define csvLibrary

void addRow(std::string fileName, std::string row[], int noOfColumns);
void addRow(std::string fileName, std::vector<std::string> rowItem, int noOfColumns);
void deleteRow(std::string fileName, int rowNumberInput);
void changeDataItem(std::string fileName, std::string newStringInput, int rowNumberInput, int columnNumberInput);

std::string getDataItem(std::string fileName,int rowNumberInput, int columnNumberInput);
std::vector<std::string> getRow(std::string fileName, int rowNumberInput);

int findRowNumber(std::string fileName, std::string stringInput, int columnNumberInput);
int numberOfEntries(std::string fileName);
bool findIfInRow(std::string fileName, std::vector<std::string> columnInput, std::vector<int> columnNumberInput);
int findRowNumber(std::string fileName, std::vector<std::string> columnInput, std::vector<int> columnNumberInput);

#endif