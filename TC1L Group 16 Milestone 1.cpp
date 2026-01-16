#include <iostream>
#include <vector>
#include <string>
using namespace std;

void displayTitle ();
void createTable(int&, vector<vector<string>>&);
void createColumn (int i, string&, string&, vector<vector<string>>&);
void insertRow (int i, string, bool, vector<vector<string>>&);
void newRowConfirmation (string, bool&);
void displaySheet (int, vector<vector<string>>&);

int main ()
{
    vector<vector<string>> Table; //Initialize 2D vector
    int columnAmount;
    string sheetName, columnName, dataType, newEntry, newRowYesNo;
    bool newRow = true, isNumber;

    displayTitle(); //Display assignment title

    cout << "Enter attendance sheet name: ";
    getline(cin, sheetName);
    cout << "Attendance sheet " << sheetName << " created successfully." << endl << endl;

    createTable(columnAmount, Table); //Create table based on initialized 2D vector

    for (int i = 0 ; i < columnAmount; i++)
    {
        createColumn(i, columnName, dataType, Table); //Create columns for table
    }

    cout << endl << "Sheet structure created successfully." << endl << endl;

    cout << "Insert New Attendance Row" << endl;

    while (newRow) //While loop for adding rows
    {
        for (int i = 0 ; i < columnAmount ; i++)
        {
            insertRow(i, newEntry, isNumber, Table); //Insert new row
        }

        cout << "Row inserted successfully" << endl << endl;

        newRowConfirmation(newRowYesNo, newRow); //Checks if the user wants to add more rows or not
    }

    cout << "Displaying the Attendance Sheet" << sheetName << "." << endl;
    cout << endl;

    displaySheet(columnAmount, Table); //Displays the table
}

// Functions -----------------------------------------------------------------------

void displayTitle()
{
    cout << "===========================================" << endl;
    cout << "STUDENT ATTENDANCE TRACKER - MILESTONE 1" << endl;
    cout << "===========================================" << endl << endl;
}

void createTable(int& columnAmount, vector<vector<string>>& Table)
{
    cout << "Define number of columns (max 10): ";

    while (!(cin >> columnAmount) || columnAmount > 10 || columnAmount <= 0) //Asks for column number and checks if its and INT and between 1 - 10
    {
        cout << "Invalid Entry. Enter number of columns (max 10): ";
        cin.clear();                     //Clear the cin failed value
        cin.ignore(10000, '\n');         //Clear buffer
    }

    cin.ignore(10000, '\n'); //Clear buffer after getting valid column number input

    Table.resize(columnAmount); //Resize the 2D vector to create columns

    cout << endl;
}

void createColumn(int i, string& columnName, string& dataType, vector<vector<string>>& Table)
{
    //Below is to get column name
    cout << "Enter column " << i + 1 << " name: ";
    getline(cin, columnName);

    //Below is to get column data type INT/TEXT
    cout << "Enter column data type. Enter (INT/TEXT): ";
    getline(cin, dataType);

    for (char &val : dataType) //Convert dataType to all caps
        val = toupper(val);

    //Error handling if the input for dataType is not INT/TEXT
    while (dataType != "INT" && dataType != "TEXT")
    {
        cout << "Invalid data type. Enter (INT/TEXT): ";
        getline(cin, dataType);

        for (char &val : dataType) //Convert dataType to all caps
            val = toupper(val);
    }

    //Data type will be pushed into [0] and Column name will pushed into [1]
    Table[i].push_back(dataType);
    Table[i].push_back(columnName);
}

void insertRow(int i, string newEntry, bool isNumber, vector<vector<string>>& Table)
{
    cout << "Enter " << Table[i][1] << ": "; //Display the column name
    getline(cin, newEntry);

    if (Table[i][0] == "INT") //Checks if the column is INT data type
    {
        do
        {
            if (newEntry == "")
            {
                isNumber = false;
                cout << "Invalid INT value. Please enter a number: ";
                getline(cin, newEntry);
            }
            else
                for (char val : newEntry) //Range based loop for every char in the string
                {
                    if (!isdigit(val)) //Check if char is a digit
                    {
                        isNumber = false;
                        cout << "Invalid INT value. Please enter a number: ";
                        getline(cin, newEntry);
                        break; //Stops the range based loop loop if a non-digit is found
                    }
                    else
                        isNumber = true; //If all char is digit, the do-while loop stops
                }
        } while (isNumber == false);
    }

    Table[i].push_back(newEntry); //pushes the new information into the column
}

void newRowConfirmation (string newRowYesNo, bool& newRow)
{
    cout << "Insert New Attendance Row. Enter (Y/N): ";
    getline(cin, newRowYesNo);
    cout << endl;

    for (char &val : newRowYesNo)
        val = toupper(val);

    while (newRowYesNo != "Y" && newRowYesNo != "N") //Error handling if input is not Y or N
    {
        cout << "Invalid entry. Enter (Y/N): ";
        getline(cin, newRowYesNo);

        for (char &val : newRowYesNo)
            val = toupper(val);
    }

    if (newRowYesNo == "N")
    {
        newRow = false;
    }
}

void displaySheet (int columnAmount, vector<vector<string>>& Table)
{
    int numOfStudents = Table[0].size(); //Determine the number of rows

    for (int j = 1 ; j < numOfStudents ; j++)
    {
        for (int i = 0 ; i < columnAmount ; i++)
        {
            cout << Table[i][j];

            if (i != columnAmount - 1)
                cout << ", ";
        }

        cout << endl;
    }
}
