/*
	Bank System
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>

using namespace std;

string ClientFileName = "Clients.txt";

void MainMenuScreen();
void ShowTransactionsMenuScreen();

enum enMainMenuOption
{
	enListClients = 1, enAddClients = 2, enDeleteClients = 3,
	enUpdateClients = 4, enFindClients = 5, enShowTransactionsMenu = 6, enExit = 7
};

enum enTransactionsMenuOption
{
	enDeposit = 1, enWithdraw = 2, enShowTotalBalance = 3, enShowMainMenu = 4
};

struct stClient
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0;
	bool MarkForDelete = false;
};

short ReadMainMenuOption()
{
	short NumberOp = 0;

	cout << "Choose what do you want to do? [1 to 7]? ";
	cin >> NumberOp;

	return NumberOp;
}

short ReadTransactionsMenuOption()
{
	short NumberOp = 0;

	cout << "Choose what do you want to do? [1 to 4]? ";
	cin >> NumberOp;

	return NumberOp;
}

string ReadClientAccountNumber()
{
	string AccountNumber;

	cout << "\nPlease enter AccountNumber? ";
	cin >> AccountNumber;

	return AccountNumber;
}

vector <string> SplittingString(string stLine, string Delim)
{
	vector <string> vString;

	string sWord = "";
	short Pos = 0;

	while ((Pos = stLine.find(Delim)) != stLine.npos)
	{
		sWord = stLine.substr(0, Pos);

		if (sWord != "")
		{
			vString.push_back(sWord);
		}

		stLine.erase(0, Pos + Delim.length());
	}

	if (stLine != "")
		vString.push_back(stLine);

	return vString;
}

stClient ConvertLineToRecord(string stLine, string Separator = "#//#")
{
	stClient Client;
	vector <string> vString = SplittingString(stLine, Separator);

	Client.AccountNumber = vString[0];
	Client.PinCode = vString[1];
	Client.Name = vString[2];
	Client.Phone = vString[3];
	Client.AccountBalance = stod(vString[4]);

	return Client;
}

vector <stClient> LoadClientsDataFromFile(string FileName)
{
	vector <stClient> vClients;
	fstream MyFile;

	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string stLine = "";
		stClient Client;

		while (getline(MyFile, stLine))
		{
			Client = ConvertLineToRecord(stLine);
			vClients.push_back(Client);
		}
	}

	return vClients;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string stLine;

		while (getline(MyFile, stLine))
		{
			stClient Client = ConvertLineToRecord(stLine);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
		}
		MyFile.close();
	}
	return false;
}

void ShowClientListOutputFormatted()
{
	cout << "| " << setw(15) << left << "Account Number";
	cout << "| " << setw(12) << left << "Pin Code";
	cout << "| " << setw(40) << left << "Client Name";
	cout << "| " << setw(15) << left << "Phone";
	cout << "| " << setw(15) << left << "Balance";
}

void PrintClientListScreen()
{
	vector <stClient> vClients = LoadClientsDataFromFile(ClientFileName);

	if (vClients.size() == 0)
	{
		cout << "\t\t\t\tNo Clients Available In the System! \n";
	}
	else
	{
		for (stClient& C : vClients)
		{
			cout << "| " << setw(15) << left << C.AccountNumber;
			cout << "| " << setw(12) << left << C.PinCode;
			cout << "| " << setw(40) << left << C.Name;
			cout << "| " << setw(15) << left << C.Phone;
			cout << "| " << setw(15) << left << C.AccountBalance;
			cout << endl;
		}
	}
}

stClient ReadNewClient()
{
	stClient Client;

	cout << "\nEnter Account Number? ";
	getline(cin >> ws, Client.AccountNumber);

	while (ClientExistsByAccountNumber(Client.AccountNumber, ClientFileName))
	{
		cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number ? ";
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

stClient AddNewClient()
{
	stClient Client = ReadNewClient();
	return Client;
}

string ConvertRecordToLine(stClient Client, string Separator = "#//#")
{
	string stLine = "";

	stLine += Client.AccountNumber + Separator;
	stLine += Client.PinCode + Separator;
	stLine += Client.Name + Separator;
	stLine += Client.Phone + Separator;
	stLine += to_string(Client.AccountBalance);

	return stLine;
}

void AddDataLineToFile(stClient Client, string FileName)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		string stLine = ConvertRecordToLine(Client);
		MyFile << stLine << endl;

		MyFile.close();
	}
}

void AddNewClients()
{
	char Answer = 'n';

	do
	{
		cout << "Adding New Client:\n";
		stClient Client = AddNewClient();
		AddDataLineToFile(Client, ClientFileName);
		cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
		cin >> Answer;
	} while (toupper(Answer) == 'Y');
}

bool FindClientByAccountNumber(vector <stClient> vClients, stClient& Client, string AccountNumber)
{
	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

void PrintClientCard(stClient Client)
{
	cout << "\nThe Following are the client details:\n";
	cout << "_____________________________________";
	cout << "\nAccount Number: " << Client.AccountNumber;
	cout << "\nPin Code: " << Client.PinCode;
	cout << "\nName: " << Client.Name;
	cout << "\nPhone: " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n_____________________________________\n";
}

bool MarkClientForDeleteByAccountNumber(vector <stClient>& vClients, string AccountNumber)
{
	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

void SaveClientDataToFile(string FileName, vector <stClient> vClients)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		for (stClient& C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				string stLine = ConvertRecordToLine(C);
				MyFile << stLine << endl;
			}
		}
		MyFile.close();
	}
}

bool DeleteClientByAccountNumber()
{
	vector <stClient> vClients = LoadClientsDataFromFile(ClientFileName);
	string AccountNumber = ReadClientAccountNumber();
	stClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(vClients, Client, AccountNumber))
	{
		PrintClientCard(Client);

		cout << "\n\nAre you sure you want delete this Client? Y/N? ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			MarkClientForDeleteByAccountNumber(vClients, AccountNumber);
			SaveClientDataToFile(ClientFileName, vClients);

			vClients = LoadClientsDataFromFile(ClientFileName);
			cout << "\n\nClient Deleted Successfully.\n";
			return true;
		}

	}
	else
	{
		cout << "Client with Account Number (" << AccountNumber << ") is Not Found!\n";
		return false;
	}
}

stClient ChangeClientRecord(string AccountNumber)
{
	stClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "\nEnter PinCode? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;
}

bool UpdateClientByAccountNumber()
{
	vector <stClient> vClients = LoadClientsDataFromFile(ClientFileName);
	string AccountNumber = ReadClientAccountNumber();
	stClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(vClients, Client, AccountNumber))
	{
		PrintClientCard(Client);

		cout << "\n\nAre you sure you want update this client? Y/N? ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{
			stClient Client = ChangeClientRecord(AccountNumber);
			for (stClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = Client;
					break;
				}
			}

			SaveClientDataToFile(ClientFileName, vClients);
			cout << "\n\nClient Update Successfully\n";

			return true;
		}
	}
	else
	{
		cout << "Client with Account Number (" << AccountNumber << ") is Not Found!\n";
		return false;
	}
}

bool FindClient()
{
	vector <stClient> vClients = LoadClientsDataFromFile(ClientFileName);
	string AccountNumber = ReadClientAccountNumber();
	stClient Client;

	if (FindClientByAccountNumber(vClients, Client, AccountNumber))
	{
		PrintClientCard(Client);
		return true;
	}
	else
	{
		cout << "Client with Account Number [" << AccountNumber << "] is Not Found!\n";
		return false;
	}
}

bool DepositBalanceCalculation(string AccountNumber, double Amount, vector <stClient> vClients)
{
	double NewBalance = 0;
	char Answer = 'n';

	cout << "\n\nAre you sure you want perfrom this Transactions? y/n? ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
	{
		for (stClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				NewBalance = C.AccountBalance;
				break;
			}
		}

		SaveClientDataToFile(ClientFileName, vClients);
		cout << "\n\nDone Successfully. New balance is: " << NewBalance << endl;

		return true;
	}


	return false;
}

void DepositBalanceToClientByAccountNumber()
{
	string AccountNumber = ReadClientAccountNumber();
	vector <stClient> vClients = LoadClientsDataFromFile(ClientFileName);
	stClient Client;

	while (!FindClientByAccountNumber(vClients, Client, AccountNumber))
	{
		cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(Client);


	double DepositAmount = 0;

	cout << "\nPlease enter deposit amount? ";
	cin >> DepositAmount;

	DepositBalanceCalculation(AccountNumber, DepositAmount, vClients);
}

void ShowBalanceClientListFormatted()
{
	cout << "| " << setw(15) << left << "Account Number";
	cout << "| " << setw(40) << left << "Client Name";
	cout << "| " << setw(30) << left << "Balance";
}

void ShowBalanceClientListOutput(vector <stClient> vClients, double& TotalBalance)
{
	if (vClients.size() == 0)
	{
		cout << "\t\t\t\t\tNo Clients Available In the system!";
	}

	else
	{
		for (stClient& C : vClients)
		{
			cout << "| " << setw(15) << left << C.AccountNumber;
			cout << "| " << setw(40) << left << C.Name;
			cout << "| " << setw(30) << left << C.AccountBalance;
			TotalBalance += C.AccountBalance;
			cout << endl;
		}
	}
}

void WithdrawBalanceByAccountNumber()
{
	string AccountNumber = ReadClientAccountNumber();
	vector <stClient> vClients = LoadClientsDataFromFile(ClientFileName);
	stClient Client;

	while (!FindClientByAccountNumber(vClients, Client, AccountNumber))
	{
		cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease enter withdraw amount? ";
	cin >> Amount;

	while (Amount > Client.AccountBalance)
	{
		cout << "Amount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
		cout << "Please enter another amount? ";
		cin >> Amount;
	}

	DepositBalanceCalculation(AccountNumber, Amount * -1, vClients);
}

void GoBackToMainMenu()
{
	cout << "\n\nPress any key to go back to Main Menu...";
	system("pause > 0");
	MainMenuScreen();
}

void GoBackToTransactionsMenu()
{
	cout << "\n\nPress any key to go back to Transactions Menu...";
	system("pause > 0");
	ShowTransactionsMenuScreen();
}

void ShowTotalBalanceScreen()
{
	vector <stClient> vClients = LoadClientsDataFromFile(ClientFileName);
	double TotalBalance = 0;
	cout << "\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).\n";
	cout << "________________________________________________________________________________________________________________________\n\n";
	ShowBalanceClientListFormatted();
	cout << "\n________________________________________________________________________________________________________________________\n\n";
	ShowBalanceClientListOutput(vClients, TotalBalance);
	cout << "\n________________________________________________________________________________________________________________________\n\n";
	cout << "\t\t\t\t\t\tTotal Balance = " << TotalBalance << endl;
}

void ShowWithdrawScreen()
{
	cout << "\n_____________________________________\n";
	cout << "\tWithdraw Screen";
	cout << "\n_____________________________________\n";
	WithdrawBalanceByAccountNumber();
}

void ShowDepositScreen()
{
	cout << "\n_____________________________________\n";
	cout << "\tDeposit Screen";
	cout << "\n_____________________________________\n";
	DepositBalanceToClientByAccountNumber();
}

void PerformTransactionsMenuOption(enTransactionsMenuOption TransactionsMenuOption)
{
	switch (TransactionsMenuOption)
	{
	case enTransactionsMenuOption::enDeposit:

		system("cls");
		ShowDepositScreen();
		GoBackToTransactionsMenu();
		break;

	case enTransactionsMenuOption::enWithdraw:

		system("cls");
		ShowWithdrawScreen();
		GoBackToTransactionsMenu();
		break;

	case enTransactionsMenuOption::enShowTotalBalance:

		system("cls");
		ShowTotalBalanceScreen();
		GoBackToTransactionsMenu();
		break;

	case enTransactionsMenuOption::enShowMainMenu:

		MainMenuScreen();
		break;

	default:

		system("cls");
		cout << "Error! Not Correct Number Enter Valid Number...\n";
		GoBackToTransactionsMenu();
		break;

	}
}

void ShowTransactionsMenuScreen()
{
	system("cls");
	cout << "============================================\n";
	cout << "\t\tTransactions Menu Screen\n";
	cout << "============================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menu.\n";
	cout << "============================================\n";
	PerformTransactionsMenuOption((enTransactionsMenuOption)ReadTransactionsMenuOption());
}

void ShowClientListScreen()
{
	vector <stClient> vClients = LoadClientsDataFromFile(ClientFileName);
	cout << "\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).\n";
	cout << "________________________________________________________________________________________________________________________\n\n";
	ShowClientListOutputFormatted();
	cout << "\n\n________________________________________________________________________________________________________________________\n\n";
	PrintClientListScreen();
	cout << "\n________________________________________________________________________________________________________________________\n";
}

void ShowAddClientsScreen()
{
	cout << "\n_____________________________________\n";
	cout << "\tAdd New Clients Screen";
	cout << "\n_____________________________________\n";
	AddNewClients();
}

void ShowDeleteClientsScreen()
{
	cout << "\n_____________________________________\n";
	cout << "\tDelete Clients Screen";
	cout << "\n_____________________________________\n";
	DeleteClientByAccountNumber();
}

void ShowUpdateClientsScreen()
{
	cout << "\n_____________________________________\n";
	cout << "\tUpdate Client Info Screen";
	cout << "\n_____________________________________\n";
	UpdateClientByAccountNumber();
}

void ShowFindClientsScreen()
{
	cout << "\n_____________________________________\n";
	cout << "\tFind Client Screen";
	cout << "\n_____________________________________\n";
	FindClient();
}

void ShowExitScreen()
{
	cout << "\n_____________________________________\n";
	cout << "\tProgram Ends :-)";
	cout << "\n_____________________________________\n";
}

void PerformMainMenuOption(enMainMenuOption MainMenuOption)
{
	switch (MainMenuOption)
	{
	case enMainMenuOption::enListClients:

		system("cls");
		ShowClientListScreen();
		GoBackToMainMenu();
		break;

	case enMainMenuOption::enAddClients:

		system("cls");
		ShowAddClientsScreen();
		GoBackToMainMenu();
		break;

	case enMainMenuOption::enDeleteClients:

		system("cls");
		ShowDeleteClientsScreen();
		GoBackToMainMenu();
		break;

	case enMainMenuOption::enUpdateClients:
		system("cls");
		ShowUpdateClientsScreen();
		GoBackToMainMenu();
		break;

	case enMainMenuOption::enFindClients:

		system("cls");
		ShowFindClientsScreen();
		GoBackToMainMenu();
		break;

	case enMainMenuOption::enShowTransactionsMenu:

		system("cls");
		ShowTransactionsMenuScreen();

	case enMainMenuOption::enExit:

		system("cls");
		ShowExitScreen();
		break;

	default:

		system("cls");
		cout << "Error! Not Correct Number Enter Valid Number...\n";
		GoBackToMainMenu();
		break;

	}
}

void MainMenuScreen()
{
	system("cls");
	cout << "============================================\n";
	cout << "\t\tMain Menu Screen\n";
	cout << "============================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Exit.\n";
	cout << "============================================\n";
	PerformMainMenuOption((enMainMenuOption)ReadMainMenuOption());
}

int main()
{
	MainMenuScreen();
	system("puase > 0");
	return 0;
}