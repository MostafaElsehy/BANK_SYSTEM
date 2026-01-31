#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cctype>
#include <limits> // Added for numeric_limits

using namespace std;

const string ClientsFileName = "Clients.txt";

enum enMainMenueOptions {
    eListClients = 1,
    eAddNewClient = 2,
    eDeleteClient = 3,
    eUpdateClient = 4,
    eFindClient = 5,
    eTransactions = 6,
    eExit = 7
};

enum enTransactionsOptions {
    eDeposit = 1,
    eWithdraw = 2,
    eTotalBalances = 3,
    eMainMenue = 4
};

struct stClientData
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance = 0;
    bool MarkForDelete = false;
};

// =============================================================
//                      Input Validation Utils
// =============================================================

// Helper to read numbers safely without crashing if text is entered
double ReadDouble(string Message)
{
    double Number = 0;
    cout << Message;
    while (!(cin >> Number)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid Input. Please enter a numerical value: ";
    }
    // Clear buffer after reading a number to prepare for next getline
    return Number;
}

// =============================================================
//                      String Helper Functions
// =============================================================

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "")
    {
        vString.push_back(S1);
    }
    return vString;
}

string ConvertRecordToLine(stClientData Client, string Seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

stClientData ConvertLineToRecord(string Line, string Seperator = "#//#")
{
    stClientData Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);

    if (vClientData.size() == 5) // Basic validation to ensure line is not corrupted
    {
        Client.AccountNumber = vClientData[0];
        Client.PinCode = vClientData[1];
        Client.Name = vClientData[2];
        Client.Phone = vClientData[3];
        Client.AccountBalance = stod(vClientData[4]);
    }
    return Client;
}

// =============================================================
//                      File I/O Functions
// =============================================================

vector<stClientData> LoadClientsDataFromFile(string FileName)
{
    vector<stClientData> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in); // Read Mode

    if (MyFile.is_open())
    {
        string Line;
        stClientData Client;

        while (getline(MyFile, Line))
        {
            if (Line != "") // Avoid empty lines
            {
                Client = ConvertLineToRecord(Line);
                vClients.push_back(Client);
            }
        }
        MyFile.close();
    }
    return vClients;
}

void SaveClientsDataToFile(string FileName, vector<stClientData> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out); // Overwrite Mode

    string DataLine;

    if (MyFile.is_open())
    {
        for (stClientData C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
}

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

// =============================================================
//                      Search & Display Logic
// =============================================================

bool FindClientByAccountNumber(string AccountNumber, vector<stClientData> vClients, stClientData& Client)
{
    for (stClientData C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

void PrintClientCard(stClientData Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccount Number : " << Client.AccountNumber;
    cout << "\nPin Code       : " << Client.PinCode;
    cout << "\nName           : " << Client.Name;
    cout << "\nPhone          : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}

void PrintAllClientsData(vector<stClientData> vClients)
{
    cout << "\n\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    for (stClientData Client : vClients)
    {
        cout << "| " << left << setw(15) << Client.AccountNumber;
        cout << "| " << left << setw(10) << Client.PinCode;
        cout << "| " << left << setw(40) << Client.Name;
        cout << "| " << left << setw(12) << Client.Phone;
        cout << "| " << left << setw(12) << Client.AccountBalance;
        cout << endl;
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

// =============================================================
//                      CRUD Operations
// =============================================================

stClientData ReadNewClient(vector<stClientData>& clients)
{
    stClientData Client;
    stClientData tempClient;

    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AccountNumber); // remove buffer spaces

    while (FindClientByAccountNumber(Client.AccountNumber, clients, tempClient))
    {
        cout << "Client with Account Number [ " << Client.AccountNumber << " ] already exists\n\n";
        cout << "Enter Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    Client.AccountBalance = ReadDouble("Enter AccountBalance? ");

    return Client;
}

void AddNewClients(vector<stClientData>& clients)
{
    stClientData Client;
    Client = ReadNewClient(clients);
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClientScreen(vector<stClientData>& clients)
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Client Screen";
    cout << "\n-----------------------------------\n";
    AddNewClients(clients);
    cout << "\nClient Added Successfully, saved to file.\n";
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<stClientData>& vClients)
{
    stClientData Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (stClientData& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C.MarkForDelete = true;
                    break;
                }
            }
            SaveClientsDataToFile(ClientsFileName, vClients);
            vClients = LoadClientsDataFromFile(ClientsFileName); // Refresh
            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
    return false;
}

stClientData ChangeClientRecord(string AccountNumber)
{
    stClientData Client;
    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter New Client Data: \n";
    cout << "----------------------\n";

    cout << "Enter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    Client.AccountBalance = ReadDouble("Enter AccountBalance? ");

    return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<stClientData>& vClients)
{
    stClientData Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (stClientData& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveClientsDataToFile(ClientsFileName, vClients);
            cout << "\n\nClient Updated Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
    return false;
}

// =============================================================
//                      Transaction Logic
// =============================================================

void Deposit(stClientData& Client, vector<stClientData>& vClients)
{
    double amount = 0;
    amount = ReadDouble("\nPlease enter amount to deposit: ");

    char option = 'n';
    cout << "\nAre you sure you want to perform this transaction? y/n ? ";
    cin >> option;

    if (toupper(option) == 'Y')
    {
        Client.AccountBalance += amount;
        for (stClientData& C : vClients)
        {
            if (C.AccountNumber == Client.AccountNumber)
            {
                C.AccountBalance = Client.AccountBalance;
                break;
            }
        }
        SaveClientsDataToFile(ClientsFileName, vClients);
        cout << "\nAmount deposited successfully.\n";
        cout << "New Balance is: " << Client.AccountBalance << endl;
    }
}

void Withdraw(stClientData& Client, vector<stClientData>& vClients)
{
    double amount = 0;
    amount = ReadDouble("\nPlease enter amount to withdraw: ");

    // FIX: Use 'while' instead of 'if'
    // This ensures we keep asking until a valid amount is entered
    while (amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount: ";
        cin >> amount;
    }

    char option = 'n';
    cout << "\nAre you sure you want to perform this transaction? y/n ? ";
    cin >> option;

    if (toupper(option) == 'Y')
    {
        Client.AccountBalance -= amount;
        for (stClientData& C : vClients)
        {
            if (C.AccountNumber == Client.AccountNumber)
            {
                C.AccountBalance = Client.AccountBalance;
                break;
            }
        }
        SaveClientsDataToFile(ClientsFileName, vClients);
        cout << "\nAmount Withdrawn Successfully.\n";
        cout << "New Balance is: " << Client.AccountBalance << endl;
    }
}

// =============================================================
//                      UI & Main Menu
// =============================================================

string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nPlease enter Account Number: ";
    cin >> AccountNumber;
    return AccountNumber;
}

void ShowDeleteClientScreen(vector<stClientData>& vClients)
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientScreen(vector<stClientData>& vClients)
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowFindClientScreen(vector<stClientData>& vClients)
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";
    string AccountNumber = ReadClientAccountNumber();
    stClientData Client;
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
    }
    else
    {
        cout << "\nClient with Account Number [" << AccountNumber << "] is not found!";
    }
}

void ShowDepositScreen(vector<stClientData>& vClients)
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";

    string AccountNumber = ReadClientAccountNumber();
    stClientData Client;
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        Deposit(Client, vClients);
    }
    else
    {
        cout << "\nClient with Account Number [" << AccountNumber << "] is not found!";
    }
}

void ShowWithdrawScreen(vector<stClientData>& vClients)
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    string AccountNumber = ReadClientAccountNumber();
    stClientData Client;
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        Withdraw(Client, vClients);
    }
    else
    {
        cout << "\nClient with Account Number [" << AccountNumber << "] is not found!";
    }
}

void ShowTotalBalancesScreen(vector<stClientData>& vClients)
{
    double TotalBalances = 0;

    cout << "\n\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\tNo Clients Available In the System!";
    else
    {
        for (stClientData Client : vClients)
        {
            cout << "| " << left << setw(15) << Client.AccountNumber;
            cout << "| " << left << setw(40) << Client.Name;
            cout << "| " << left << setw(12) << Client.AccountBalance;
            TotalBalances += Client.AccountBalance;
            cout << endl;
        }
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << TotalBalances << endl;
}

int ReadOption(int start, int end)
{
    int Choice = 0;
    cout << "Choose what do you want to do? [" << start << " to " << end << "] ? ";
    // Use the robust validation here so menus don't crash
    while (!(cin >> Choice) || (Choice < start || Choice > end))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid Option. Choose [" << start << " to " << end << "] ? ";
    }
    return Choice;
}

void GoBackToTransactions()
{
    cout << "\n\nPress Enter to go back to Transaction Menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void ShowTransactionsScreen(vector<stClientData>& vClients)
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t  Transaction Menu Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menu.\n";
    cout << "===========================================\n";

    enTransactionsOptions Choice = (enTransactionsOptions)ReadOption(1, 4);

    switch (Choice)
    {
    case eDeposit:
        system("cls");
        ShowDepositScreen(vClients);
        GoBackToTransactions();
        ShowTransactionsScreen(vClients);
        break;

    case eWithdraw:
        system("cls");
        ShowWithdrawScreen(vClients);
        GoBackToTransactions();
        ShowTransactionsScreen(vClients);
        break;

    case eTotalBalances:
        system("cls");
        ShowTotalBalancesScreen(vClients);
        GoBackToTransactions();
        ShowTransactionsScreen(vClients);
        break;

    case eMainMenue:
        // Do nothing, just returns, which goes back to Main Menu
        break;
    }
}

void GoBackToMainMenue()
{
    cout << "\n\nPress Enter to go back to Main Menu...";

    // 1. Clear the "Enter" key left in the buffer from the previous choice
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // 2. Wait for the user to press Enter again
    cin.get();
}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menu Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Exit.\n";
    cout << "===========================================\n";
}

void StartBankApplication()
{
    bool Running = true;
    while (Running)
    {
        ShowMainMenue();
        enMainMenueOptions Choice = (enMainMenueOptions)ReadOption(1, 7);

        vector<stClientData> vClients = LoadClientsDataFromFile(ClientsFileName);

        switch (Choice)
        {
        case eListClients:
            system("cls");
            PrintAllClientsData(vClients);
            GoBackToMainMenue();
            break;
        case eAddNewClient:
            system("cls");
            AddNewClientScreen(vClients);
            GoBackToMainMenue();
            break;
        case eDeleteClient:
            system("cls");
            ShowDeleteClientScreen(vClients);
            GoBackToMainMenue();
            break;
        case eUpdateClient:
            system("cls");
            ShowUpdateClientScreen(vClients);
            GoBackToMainMenue();
            break;
        case eFindClient:
            system("cls");
            ShowFindClientScreen(vClients);
            GoBackToMainMenue();
            break;
        case eTransactions:
            system("cls");
            ShowTransactionsScreen(vClients);
            break;
        case eExit:
            system("cls");
            cout << "\n\n-------------------------------------------\n";
            cout << "\t\tProgram Ends :-)";
            cout << "\n-------------------------------------------\n";
            Running = false;
            break;
        }
    }
}

int main()
{
    StartBankApplication();
    return 0;
}