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

//---------------------------(Haikal Func Prototype START)-----------------------------
void saveFile(string filename, int, vector<vector<string>>&); // Saves attendance data into a CSV file
//---------------------------(Haikal Func Prototype END)-----------------------------

//---------------------------(Naim Func Prototype START)-----------------------------
int sheetOption(); // Shows menu to user: load sheet or create new sheet
bool loadFile(string filename, int&, vector<vector<string>>&); // Loads attendance data from a CSV file into Table
string getTermFile(); // Prompts user to create or load a term file (acts as database)
void addSheetToTerm(string termFile, string sheetFile); //Stores the sheet filename into the term file (like adding record)
vector<string> loadSheetsFromTerm(string termFile); //Reads all sheet filenames stored in the term file
string chooseSheet(const vector<string>& sheets, string termName); // Lets user choose which sheet to open by number
//---------------------------(Naim Func Prototype END)-----------------------------

//---------------------------(Zakwan Func Prototype START)-----------------------------
void clearCin ();
void checkDataType (string&, vector<vector<string>>&, int, int);
void findStudentID (string&, vector<vector<string>>&, int&, bool&);
void askWhichToUpdate (int, int&, vector<vector<string>>&);
void askContinueUpdate (bool&);
//---------------------------(Zakwan Func Prototype END)-------------------------------

//----------------------------(Arif Func Prototype START)-------------------------------
void deleteRow(int columnAmount, int rowIndex, vector<vector<string>>& Table);
//----------------------------(Arif Func Prototype END)--------------------------------

int main ()
{
    vector<vector<string>> Table; //Initialize 2D vector
    int columnAmount, updateOrDelete, rowIndex = 0, whichToUpdate;
    string filename, columnName, dataType, newEntry, newRowYesNo, selectedStudentID, updateData;
    bool newRow = true, isNumber, found, keepUpdating;

    displayTitle(); //Display assignment title

//---------------------------------(Naim START)------------------------------------------
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
        option = sheetOption();
    }


    if (option == 1)
    {
        // load file
        bool loaded = false;

        do
        {
            if (!sheets.empty())
                filename = chooseSheet(sheets, termFile);
            else
                {
                    cout << "\nEnter filename to load: ";
                    getline(cin, filename);
                }

            loaded = loadFile(filename, columnAmount, Table);
        } while (!loaded);
//---------------------------------(Naim END)------------------------------------------

//------------------------------------------(Zakwan START)------------------------------------------
        while (true)
        {
            found = false;
            rowIndex = 0;
            displaySheet(columnAmount, Table);

            cout << "Num. of Rows/Final Attendance Count: " << Table[0].size() - 2 << endl; //Haikal's

            cout << "\nWhat do you want to do on the current sheet?\n" << "1. Update a row\n" << "2. Delete a row\n" << "3. Add a row\n" << "4. Save file\n" << "5. Other input: Exit\n";
            cout << "\nEnter (1/2): ";
            if (!(cin >> updateOrDelete && updateOrDelete == 1 || updateOrDelete == 2 || updateOrDelete == 3 || updateOrDelete == 4))
                break; //If user enter anything other than 1 or 2, exits program

            clearCin(); //Used to clear buffer after cin

            if (updateOrDelete == 1) //To display what the user has chosen (update or delete)
                cout << "\nEnter " << Table[0][1] << " to update: ";
            else if (updateOrDelete == 2)
                cout << "\nEnter " << Table[0][1] << " to delete: ";

            if (updateOrDelete == 1 || updateOrDelete == 2)

            do //#Arif guna ni utk cari studentID
            {
                checkDataType(selectedStudentID, Table, 0, 0); //Make sure the input datatype matches the column datatype

                findStudentID (selectedStudentID, Table, rowIndex, found); //Find the index of the row so it only focuses on that row
            } while (!found);

            if (updateOrDelete == 1)
            {
                do
                {
                    askWhichToUpdate(columnAmount, whichToUpdate, Table); //ASk user which to update on the row (id or name or status etc)

                    cout << "\nEnter the new data for " << Table[whichToUpdate - 1][1] << ": ";

                    checkDataType(updateData, Table, whichToUpdate - 1, 0); //Check datatype of input

                    Table[whichToUpdate - 1][rowIndex] = updateData; //Update the data for (id or name or status etc)

                    cout << "\nDisplaying updated sheet: \n";
                    displaySheet(columnAmount, Table);

                    askContinueUpdate (keepUpdating); //Ask the user if they wanna continue updating on the current row or not

                } while (keepUpdating == true);
            }
//------------------------------------------(Zakwan END)-------------------------------------------

//------------------------------------------(Arif START)-------------------------------------------
            else if (updateOrDelete == 2)
            {
                char confirm;
                cout << "Are you sure you want to delete this row? (Y = Yes, other input = Cancel):";
                cin >> confirm;
                clearCin();

                if (toupper(confirm) == 'Y')
                    deleteRow(columnAmount, rowIndex, Table);
                else
                    cout << "Deletion cancelled.\n\n";
            }
//------------------------------------------(Arif END)-------------------------------------------

//------------------------------------------(Haikal START)-------------------------------------------
            else if (updateOrDelete == 3)
            {
                string newData;
                cout << "Adding new row. Please enter the necessary data.\n";

                for (int i = 0; i < columnAmount; i++)
                {
                    cout << "Enter " << Table[i][1] << ": ";

                    // Make sure the data matches the type (INT or TEXT)
                    checkDataType(newData, Table, i, 0);

                    // Add the new data to the bottom of the current column
                    Table[i].push_back(newData);
                }

            cout << "New row added successfully!\n\n";
            }
            else if (updateOrDelete == 4)
            {
                char confirm;
                cout << "Are you sure you want to save this file? (Y/N): ";
                cin >> confirm;
                clearCin();

                if (toupper(confirm) == 'Y')
                {
                    saveFile(filename, columnAmount, Table);
                }
                else
                {
                    cout << "Save File cancelled.\n";
                }
            }
        }
//------------------------------------------(Haikal END)-------------------------------------------
        cout << "\nExiting program. See you again.";
        return 0; // exit after loading & displaying
    }


//-------------(NAIM START)changing "attendance sheet name" into actual file saving (modify zakwan code)----------------
    while (true)
    {
        // Ask user to enter a new sheet filename
        cout << "\nEnter new sheet filename: ";
        getline(cin, filename);
        filename = filename + "_" + termFile;

        // Check if the file already exists to prevent overwriting
        ifstream check(filename);
        if (check)
        {
            cout << "Error: file already exists.\n";
            continue; // Ask again
        }

        // If filename is valid and does not exist, accept it
        cout << "Sheet file " << filename << " created successfully.\n\n";
        break; // Exit loop when a valid filename is entered
    }

//---------------------------------(Naim END)------------------------------------------

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

    cout << "Displaying the Attendance Sheet: " << filename << endl;
    cout << endl;

    displaySheet(columnAmount, Table); //Displays the table

    saveFile(filename, columnAmount, Table); //Haikal's
    addSheetToTerm(termFile, filename); //Naim's

    return 0;
}




//----------------------------------------FUNCTIONS------------------------------------------------

//---------------------------------(Naim Functions START)------------------------------------------
// Menu option: load or create
int sheetOption()
{
    int choice;  // Stores the user's menu selection (1 = load, 2 = create)

    // Display menu options to the user
    cout << "1. Load attendance sheet from file\n";
    cout << "2. Create new attendance sheet\n";
    cout << "Enter (1/2): ";

    // Loop until user enters a valid integer AND it is either 1 or 2
    while (!(cin >> choice) || (choice != 1 && choice != 2))
    {
        // If input is not an integer or not 1/2, show error message
        cout << "\nInvalid choice. Enter (1/2): ";

        // Clear error state of cin (in case user entered letters)
        cin.clear();

        // Remove invalid input from the buffer to prevent infinite loop
        cin.ignore(10000, '\n');
    }

    // Clear leftover newline character after valid input
    cin.ignore(10000, '\n');

    // Return the valid user choice to the caller
    return choice;
}

//------------------------------------------(Haikal START)-------------------------------------------
// Save attendance to file
void saveFile(string filename, int columnAmount, vector<vector<string>>& Table)
{

    ofstream outFile(filename);
    if (!outFile)
    {
        cout << "Error: opening file\n";
        return;
    }

    int totalRows = Table[0].size();

    for (int r = 0; r < totalRows; r++)
    {
        outFile << Table[0][r];

        for (int c = 1; c < columnAmount; c++)
        {
            outFile << "," << Table [c][r];
        }
        outFile << endl;
    }
    outFile.close();
    cout << "\nFile saved successfully.\n" << endl;
}
//------------------------------------------(Haikal END)-------------------------------------------


// Load attendance from file
bool loadFile(string filename, int& columnAmount, vector<vector<string>>& Table)
{
    ifstream inFile(filename);

    // If file does not exist
    if (!inFile)
    {
        cout << "Error: file not found\n";
        return false;
    }

    Table.clear();

    string line, cell;
    vector<vector<string>> rows;

    // Read each line from CSV
    while (getline(inFile, line))
    {
        vector<string> row;
        stringstream split(line);

        // Split by comma
        while (getline(split, cell, ','))
        {
            row.push_back(cell);
        }

        rows.push_back(row);
    }

    // Reject empty file
    if (rows.empty())
    {
        cout << "file is empty.\n";
        return false;
    }

        int expectedColumns = rows[0].size();

    for (int i = 0; i < rows.size(); i++)
    {
        if (rows[i].size() != expectedColumns)
        {
            cout << "Error: CSV format is corrupted\n";
            return false;
        }
    }

    // Convert rows into column-based table
    columnAmount = rows[0].size();
    Table.resize(columnAmount);

    for (int r = 0; r < rows.size(); r++)
    {
        for (int c = 0; c < columnAmount; c++)
        {
            Table[c].push_back(rows[r][c]);
        }
    }

    cout << "Attendance loaded successfully.\n\n";
    return true;
}

string getTermFile()
{
    int choice;
    string term;

    // Ask user whether to create new term or load existing term
    cout << "What do you want to do?\n";
    cout << "1. Create new term\n";
    cout << "2. Load existing term\n";
    cout << "3. Other input: Exit program\n";
    cout << "Enter (1/2): ";

    // Input validation: only allow 1 or 2
    while (!(cin >> choice) || (choice != 1 && choice != 2))
    {
        cout << "\nExiting program. See you again.";
        exit(EXIT_SUCCESS);
    }
    cin.ignore(10000, '\n');

    if (choice == 1)
    {
        // CREATE NEW TERM
        while (true)
        {
            cout << "\nEnter new term name: ";
            getline(cin, term);
            term = term + ".csv";

            // Prevent overwriting existing term
            ifstream check(term);
            if (check)
            {
                cout << "Error: File already exists.\n";
                continue;
            }

            // Create empty term file
            ofstream file(term);
            if (!file)
            {
                cout << "Error creating file. Try again.\n"; //in case file failed to open for writing
                continue;
            }

            file.close();
            cout << "Term created successfully.\n";
            return term;
        }
    }
    else
    {
        // LOAD EXISTING TERM
        while (true)
        {
            cout << "\nEnter existing term name: ";
            getline(cin, term);
            term = term + ".csv";

            // Ensure file exists before loading
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
    // Prevent duplicate sheet names in term file
    vector<string> existing = loadSheetsFromTerm(termFile);
    for (string sheetname : existing)
        if (sheetname == sheetFile)
            return;

    // Append new sheet name to term file
    ofstream file(termFile, ios::app);
    file << sheetFile << endl;
    file.close();
}

vector<string> loadSheetsFromTerm(string termFile)
{
    vector<string> sheets;
    ifstream file(termFile);

    // If term file cannot be opened, return empty list
    if (!file)
    {
        cout << "Error: Could not open term file.\n";
        return sheets;
    }

    string line;

    while (getline(file, line))
    {
        // Remove Windows carriage return if present
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        // Skip empty lines
        if (line.empty())
            continue;

        // Only keep the sheet if the file actually exists
        ifstream test(line);
        if (test)
            sheets.push_back(line);
    }

    file.close();
    return sheets;
}

string chooseSheet(const vector<string>& sheets, termName)
{
    cout << "\nSheets in this term:\n";
    for (int i = 0; i < sheets.size(); i++)
        cout << i + 1 << ". " << sheets[i] - termName << endl;

    int choice;

    while (true)
    {
        cout << "Choose sheet (enter number): ";

        // Ensure input is a number
        if (!(cin >> choice))
        {
            cout << "\nInvalid input. Enter a number only.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        // Ensure number is within valid range
        if (choice < 1 || choice > sheets.size())
        {
            cout << "\nInvalid number. Choose between 1 and " << sheets.size() << ".\n";
            continue;
        }

        cin.ignore(10000, '\n');
        return sheets[choice - 1];
    }
}
//---------------------------------(Naim Functions End)------------------------------------------

void displayTitle()
{
    cout << "===========================================" << endl;
    cout << "STUDENT ATTENDANCE TRACKER - MILESTONE 2" << endl;
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
    // Loop through EVERY row in the vector, starting from 0 (the header)
    for (int j = 1; j < Table[0].size(); j++)
    {
        for (int i = 0; i < columnAmount; i++)
        {
            cout << Table[i][j] << (i == columnAmount - 1 ? "" : ", ");
        }
        cout << endl;
    }
}

//---------------------------------(Zakwan's Functions START)-------------------------------------
void clearCin ()
{
    cin.clear();
    cin.ignore(10000, '\n');
}

void checkDataType (string& input, vector<vector<string>>& Table, int columnIndex, int rowIndex)
{
    bool isNumber;

    if (Table[columnIndex][rowIndex] == "INT")
    {
        do
        {
            getline(cin, input);

            for(char val : input)
            {
                    if(!isdigit(val))
                    {
                        cout << "" << Table[columnIndex][1] << " only accepts integer value. Please try again: ";
                        isNumber = false;
                        break;
                    }
                    else
                        isNumber = true;
            }
        } while (isNumber == false);
    }
    else
        getline(cin, input);
}

void findStudentID (string& studentID, vector<vector<string>>& Table, int& rowIndex, bool& found)
{
    for(string val : Table[0])
        if (val == studentID)
        {
            found = true;
            break;
        }

    if (!found)
        cout << Table[0][1] << " not found. Please try again: ";
    else
    {
        rowIndex = 0;
        for(string val : Table[0])
        if (val != studentID)
            rowIndex++;
        else
            break;
    }
}

void askWhichToUpdate (int columnAmount, int& whichToUpdate, vector<vector<string>>& Table)
{
    for(int i = 0 ; i < columnAmount ; i++)
    {
        cout << i + 1 << ". " << Table[i][1] << endl;
    }

    cout << "\nWhat do you want to update from this row: ";

    while (true)
    {
        if (!(cin >> whichToUpdate) || whichToUpdate > columnAmount)
        {
            cout << "Invalid input. Please try again: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        else
        {
            cin.ignore(10000, '\n');
            break;
        }
    }
}

void askContinueUpdate (bool& keepUpdating)
{
    char keepUpdatingConfirmation;
    cout << "\nContinue updating current row? (Y = Yes, other input = Cancel): ";
    cin >> keepUpdatingConfirmation;
    cin.ignore(10000, '\n');

    if (toupper(keepUpdatingConfirmation) == 'Y')
        keepUpdating = true;
    else
        keepUpdating = false;
}
//---------------------------------(Zakwan's Functions END)-------------------------------------


//---------------------------------(Arif's code START)------------------------------------------
void deleteRow (int columnAmount, int rowIndex, vector<vector<string>>& Table)
{
    for (int i= 0; i < columnAmount; i++)
    {
        Table[i].erase(Table[i].begin() + rowIndex);
    }

    cout << "Row deleted successfully!\n";
    cout << "\nDisplaying updated sheet: \n\n";
}
//--------------------------------(Arif's code END)----------------------------------------------

//----------------------------------------FUNCTIONS END------------------------------------------------
