#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

const string ClientsFileName = "Clients.txt";

enum enMainMenueOptions {
    eListClients = 1,
    eAddNewClient = 2,
    eDeleteClient = 3,
    eUpdateClient = 4,
    eFindClient = 5,
    eExit = 6
};

struct stClientData
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

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

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);
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
            Client = ConvertLineToRecord(Line);
            vClients.push_back(Client);
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
            // Only save clients that are NOT marked for delete
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
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
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
    // Usage of getline to handle potential spaces, though acc num is usually no space
    getline(cin >> ws, Client.AccountNumber);
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

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

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
            // Mark for delete in the vector
            for (stClientData& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C.MarkForDelete = true;
                    break;
                }
            }
            // Save the vector (excluding marked items) to file
            SaveClientsDataToFile(ClientsFileName, vClients);

            // Reload vector to match file state
            vClients = LoadClientsDataFromFile(ClientsFileName);
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

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
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
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number [" << AccountNumber << "] is not found!";
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
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
    cout << "\t[6] Exit.\n";
    cout << "===========================================\n";
}

void StartBankApplication()
{
    bool Running = true;
    while (Running)
    {
        ShowMainMenue();
        enMainMenueOptions Choice = (enMainMenueOptions)ReadMainMenueOption();

        // Load data fresh from file every time we return to menu
        // This ensures if we added something, it is reflected in the list immediately
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