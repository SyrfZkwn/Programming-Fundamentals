#include <iostream>
#include <vector>
#include <string>
#include <fstream>     //added for file handling (naim)
#include <sstream>     //for csv files (naim)
using namespace std;

void displayTitle ();
void createTable(int&, vector<vector<string>>&);
void createColumn (int i, string&, string&, vector<vector<string>>&);
void insertRow (int i, string, bool, vector<vector<string>>&);
void newRowConfirmation (string, bool&);
void displaySheet (int, vector<vector<string>>&);

//file handling function declarations (naim)
int startOption();                                      // menu: load or create
string getFile(string purpose);                         // get filename from user
bool isCsvFile(string filename);                        // validate .csv
void saveFile(string filename, int, vector<vector<string>>&); // save attendance
bool loadFile(string filename, int&, vector<vector<string>>&); // load attendance

string getTermFile();
void addSheetToTerm(string termFile, string sheetFile);
vector<string> loadSheetsFromTerm(string termFile);
string chooseSheet(const vector<string>& sheets);
//(naim end)

int main ()
{
    vector<vector<string>> Table; //Initialize 2D vector
    int columnAmount;
    string sheetName, columnName, dataType, newEntry, newRowYesNo;
    bool newRow = true, isNumber;

    displayTitle(); //Display assignment title

    //for term database (naim start)
    string termFile = getTermFile();
    vector<string> sheets = loadSheetsFromTerm(termFile);

    int option;

    if (sheets.empty())
    {
        cout << "No sheets found in this term yet.\n";

        string answer;
        while (true)
        {
            cout << "Do you want to create a new sheet? (Y/N): ";
            getline(cin, answer);

            for (char &c : answer) c = toupper(c);

            if (answer == "Y")
            {
                option = 2;   // Create new sheet
                break;
            }
            else if (answer == "N")
            {
                cout << "Exiting program.\n";
                return 0;
            }
            else
            {
                cout << "Invalid input. Please enter Y or N.\n";
            }
        }
    }
    else
    {
        // user chooses to load file OR create new sheet
        option = startOption();
    }


    if (option == 1)
    {
        // load file
        string filename;
        bool loaded = false;

        do
        {
            if (!sheets.empty())
                filename = chooseSheet(sheets);
            else
                filename = getFile("load");

            loaded = loadFile(filename, columnAmount, Table);
        } while (!loaded);

        displaySheet(columnAmount, Table);
        return 0; // exit after loading & displaying
    }
//(naim end)

    cout << "\nEnter attendance sheet name: ";
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

    //save file at the end (naim start)
    string filename = getFile("save");
    saveFile(filename, columnAmount, Table);
    addSheetToTerm(termFile, filename);

    return 0;
    //(naim end)
}

//file handling functions (naim start)
// Menu option: load or create
int startOption()
{
    int choice;
    cout << "1. Load attendance sheet from file\n";
    cout << "2. Create new attendance sheet\n";
    cout << "Enter (1/2):\n ";

    while (!(cin >> choice) || (choice != 1 && choice != 2))
    {
        cout << "\nInvalid choice. Enter (1/2):\n ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    cin.ignore(10000, '\n');
    return choice;
}

// Get filename from user
string getFile(string purpose)
{
    string filename;

    while (true)
    {
        cout << "\nEnter filename to " << purpose << " (.csv only): ";
        getline(cin, filename);

        if (!isCsvFile(filename))
        {
            cout << "Error: csv files only\n";
        }
        else
        {
            return filename;
        }
    }
}

// Ensure file ends with .txt
bool isCsvFile(string filename)
{
    if (filename.length() < 4) return false;
    return filename.substr(filename.length() - 4) == ".csv";
}

// Save attendance to file
void saveFile(string filename, int columnAmount, vector<vector<string>>& Table)
{
    ofstream outFile(filename);

    if (!outFile)
    {
        cout << "Error: creating file\n";
        return;
    }

    int rows = Table[0].size();

    for (int r = 1; r < rows; r++)
    {
        for (int c = 0; c < columnAmount; c++)
        {
            outFile << Table[c][r];
            if (c != columnAmount - 1)
                outFile << ",";
        }
        outFile << endl;
    }

    outFile.close();
    cout << "Attendance saved successfully (CSV format).\n";
}


// Load attendance from file
bool loadFile(string filename, int& columnAmount, vector<vector<string>>& Table)
{
    ifstream inFile(filename);

    if (!inFile)
    {
        cout << "Error: file not found\n";
        return false;
    }

    Table.clear();

    string line, cell;
    vector<vector<string>> rows;

    while (getline(inFile, line))
    {
        vector<string> row;
        stringstream ss(line);

        while (getline(ss, cell, ','))
        {
            row.push_back(cell);
        }

        rows.push_back(row);
    }

    if (rows.empty())
    {
        cout << "CSV is empty.\n";
        return false;
    }

    columnAmount = rows[0].size();
    Table.resize(columnAmount);

    for (int r = 0; r < rows.size(); r++)
    {
        for (int c = 0; c < columnAmount; c++)
        {
            Table[c].push_back(rows[r][c]);
        }
    }

    cout << "Attendance loaded successfully (CSV).\n\n";
    return true;
}

string getTermFile()
{
    int choice;
    string term;

    cout << "1. Create new term\n";
    cout << "2. Load existing term\n";
    cout << "Enter (1/2):\n";

    while (!(cin >> choice) || (choice != 1 && choice != 2))
    {
        cout << "\nInvalid choice. Enter (1/2):\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    cin.ignore(10000, '\n');

    if (choice == 1)
    {
        while (true)
        {
            cout << "\nEnter new term name (.csv): ";
            getline(cin, term);

            if (!isCsvFile(term))
            {
                cout << "Error: must be .csv file\n";
                continue;
            }

            ofstream file(term);
            if (!file)
            {
                cout << "Error creating file. Try again.\n";
                continue;
            }

            file.close();
            cout << "Term created successfully.\n";
            return term;
        }
    }
    else
    {
        while (true)
        {
            cout << "\nEnter existing term name (.csv): ";
            getline(cin, term);

            if (!isCsvFile(term))
            {
                cout << "Error: must be .csv file\n";
                continue;
            }

            ifstream file(term);
            if (!file)
            {
                cout << "Error: term file not found.\n";
                continue;
            }

            file.close();
            cout << "\nTerm loaded successfully.\n";
            return term;
        }
    }
}


void addSheetToTerm(string termFile, string sheetFile)
{
    // Prevent duplicates
    vector<string> existing = loadSheetsFromTerm(termFile);
    for (string s : existing)
        if (s == sheetFile)
            return;

    ofstream file(termFile, ios::app);
    file << sheetFile << endl;
    file.close();
}

vector<string> loadSheetsFromTerm(string termFile)
{
    vector<string> sheets;
    ifstream file(termFile);

    if (!file)
    {
        cout << "Error: Could not open term file.\n";
        return sheets;
    }

    string line;

    while (getline(file, line))
    {
        // Fix Windows line ending issue
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        // Ignore empty lines
        if (line.empty())
            continue;

        // Only keep sheet if file actually exists
        ifstream test(line);
        if (test)
            sheets.push_back(line);
    }

    file.close();
    return sheets;
}

string chooseSheet(const vector<string>& sheets)
{
    cout << "\nSheets in this term:\n";
    for (int i = 0; i < sheets.size(); i++)
        cout << i + 1 << ". " << sheets[i] << endl;

    int choice;

    while (true)
    {
        cout << "Choose sheet (enter number):";

        if (!(cin >> choice))
        {
            cout << "\nInvalid input. Enter a number only.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (choice < 1 || choice > sheets.size())
        {
            cout << "\nInvalid number. Choose between 1 and " << sheets.size() << ".\n";
            continue;
        }

        cin.ignore(10000, '\n');
        return sheets[choice - 1];
    }
}

//(naim end)


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
