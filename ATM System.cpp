//Project #4, ATM System
//Due: In 2026/6/16

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>

using namespace std;

const string ClientsFileName = "Clients.txt";

enum enMainMenueOptions { enQuickWithdraw = 1, enNormalWithdraw = 2, enDeposit = 3, enCheckBalance = 4, enLogout = 5 };

struct stClient
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0.0;
	bool MarkForDelete = false;
};

stClient CurrentClient;

void Login();

void ShowMainMenue();

void ShowQuickWithdrawScreen();

void ShowNormalWithdrawScreen();

string PrintPunctuations(char Punct, short NumberOfPunct)
{
	string sPunct = "";

	for (short i = 0; i < NumberOfPunct; i++)
	{
		sPunct += Punct;
	}

	return sPunct;
}

vector <string> SplitString(string S1, string Delim = " ")
{
	vector <string> vString;

	short pos = 0;
	string sWord;

	while ((pos = S1.find(Delim)) != string::npos)
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

stClient ConvertLineToRecord(string Line, string Seperator = "#//#")
{
	stClient Client;

	vector <string> vClientData;
	vClientData = SplitString(Line, Seperator);

	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]); //cast string to double

	return Client;
}

string ConvertRecordToLine(stClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";

	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);

	return stClientRecord;
}

vector <stClient> LoadClientsDataFromFile(string FileName)
{
	vector <stClient> vClients;

	fstream MyFile;
	MyFile.open(FileName, ios::in); //read Mode

	if (MyFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);

			vClients.push_back(Client);
		}

		MyFile.close();
	}

	return vClients;
}

vector <stClient> SaveClientsDataToFile(string FileName, vector <stClient>& vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out); //overWrite

	string DataLine;

	if (MyFile.is_open())
	{
		for (stClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				// we only write records that are not marked for delete.
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}

		MyFile.close();
	}
	return vClients;
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient& Client)
{
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	for (stClient& U : vClients)
	{
		if (U.AccountNumber == AccountNumber && U.PinCode == PinCode)
		{
			Client = U;
			return true;
		}
	}
	return false;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <stClient>& vClients)
{
	char Answer = 'n';

	cout << "\n\nAre you sure you want perform this transaction? y/n? ";
	cin >> Answer;

	if (tolower(Answer) == 'y')
	{
		for (stClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveClientsDataToFile(ClientsFileName, vClients);
				cout << "\n\nDone Successfully New Balance is: " << C.AccountBalance;

				return true;
			}
		}
		return false;
	}
	return false;
}

short ReadQuickWithdrawOption()
{
	short Choose = 0;
	do
	{
		cout << "Choose what to do from[1] to [9] ? ";
		cin >> Choose;

	} while (!(Choose >= 1 && Choose <= 9));

	return Choose;
}

short getQuickWithDrawAmount(short QuickWithdrawOption)
{
	switch (QuickWithdrawOption)
	{
		case 1:
			return 20;
		case 2:
			return 50;
		case 3:
			return 100;
		case 4:
			return 200;
		case 5:
			return 400;
		case 6:
			return 600;
		case 7:
			return 800;
		case 8:
			return 1000;
		default:
			return 0;
	}
}

void PerformQuickWithdrawOption(short QuickWithdrawOption)
{
	if (QuickWithdrawOption == 9) // exit
		return;

	short WithDrawBalance = getQuickWithDrawAmount(QuickWithdrawOption);

	if (WithDrawBalance > CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeds your balance, make another choice.\n";
		cout << "Press AnyKey to continue...";
		system("Pause>0");
		ShowQuickWithdrawScreen();
		return;
	}

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	if (DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients))
		CurrentClient.AccountBalance -= WithDrawBalance;
}

void ShowQuickWithdrawScreen()
{
	system("cls");
	cout << PrintPunctuations('=', 45) << "\n";
	cout << PrintPunctuations('\t', 2) << " Quick Withdraw\n";
	cout << PrintPunctuations('=', 45) << "\n";

	cout << right << setw(15) << "[1] 20 " << setw(17) << "[2] 50  " << "\n";
	cout << right << setw(15) << "[3] 100" << setw(17) << "[4] 200 " << "\n";
	cout << right << setw(15) << "[5] 400" << setw(17) << "[6] 600 " << "\n";
	cout << right << setw(15) << "[7] 800" << setw(17) << "[8] 1000" << "\n";
	cout << right << setw(16) << "[9] Exit" << "\n";
	cout << PrintPunctuations('=', 45) << "\n";

	cout << "Your Balance is " << CurrentClient.AccountBalance << "\n";

	PerformQuickWithdrawOption(ReadQuickWithdrawOption());
}

int ReadWithDrawAmount()
{
	int Amount = 0;
	do
	{
		cout << "\nEnter an amount multiple of 5's ? ";
		cin >> Amount;

	} while (Amount % 5 != 0);

	return Amount;
}

void PerformNormalWithdrawOption()
{
	int WithDrawBalance = ReadWithDrawAmount();

	if (WithDrawBalance > CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeds your balance, make another choice.";
		cout << "\nPress Anykey to continue...";
		system("pause>0");
		ShowNormalWithdrawScreen();
		return;
	}

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	if (DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients))
		CurrentClient.AccountBalance -= WithDrawBalance;
}

void ShowNormalWithdrawScreen()
{
	system("cls");
	cout << PrintPunctuations('=', 45) << "\n";
	cout << PrintPunctuations('\t', 2) << "Normal Withdraw Screen\n";
	cout << PrintPunctuations('=', 45) << "\n";
	PerformNormalWithdrawOption();
}

double ReadDepositAmount()
{
	double Amount = 0;
	do
	{
		cout << "\nEnter a positive Deposit Amount? ";
		cin >> Amount;
	} while (Amount <= 0);

	return Amount;
}

void PerformDepositOption()
{
	double DepositAmount = ReadDepositAmount();

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients);
	CurrentClient.AccountBalance += DepositAmount;
}

void ShowDepositScreen()
{
	cout << PrintPunctuations('=', 45) << "\n";
	cout << PrintPunctuations('\t', 2) << "Deposit Screen\n";
	cout << PrintPunctuations('=', 45) << "\n";
	PerformDepositOption();
}

void ShowClientBalanceScreen()
{
	cout << PrintPunctuations('=', 45) << "\n";
	cout << PrintPunctuations('\t', 2) << "Check Balance Screen\n";
	cout << PrintPunctuations('=', 45) << "\n";

	cout << "Your Balance is " << CurrentClient.AccountBalance << "\n";
}

void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
	ShowMainMenue();
}

short ReadMainMenueOption()
{
	short Choose = 0;
	do
	{
		cout << "Choose what do you want to do? [1 to 5]? ";
		cin >> Choose;

	} while (!(Choose >= 1 && Choose <= 5));

	return Choose;
}

void PerformMainMenueOption(enMainMenueOptions MainMenueOption)
{
	switch (MainMenueOption)
	{
		case enMainMenueOptions::enQuickWithdraw:
		{
			system("cls");
			ShowQuickWithdrawScreen();
			GoBackToMainMenue();
			break;
		}

		case enMainMenueOptions::enNormalWithdraw:
		{
			system("cls");
			ShowNormalWithdrawScreen();
			GoBackToMainMenue();
			break;
		}

		case enMainMenueOptions::enDeposit:
		{
			system("cls");
			ShowDepositScreen();
			GoBackToMainMenue();
			break;
		}

		case enMainMenueOptions::enCheckBalance:
		{
			system("cls");
			ShowClientBalanceScreen();
			GoBackToMainMenue();
			break;
		}

		case enMainMenueOptions::enLogout:
		{
			system("cls");
			Login();
			break;
		}
	}
}

void ShowMainMenue()
{
	system("cls");
	cout << PrintPunctuations('=', 45) << "\n";
	cout << PrintPunctuations('\t', 2) << "ATM Main Menue Screen\n";
	cout << PrintPunctuations('=', 45) << "\n";
	cout << PrintPunctuations('\t', 1) << "[1] Quick Withdraw." << "\n";
	cout << PrintPunctuations('\t', 1) << "[2] Normal Withdraw." << "\n";
	cout << PrintPunctuations('\t', 1) << "[3] Deposit." << "\n";
	cout << PrintPunctuations('\t', 1) << "[4] Check Balance." << "\n";
	cout << PrintPunctuations('\t', 1) << "[5] Logout." << "\n";
	cout << PrintPunctuations('=', 45) << "\n";

	PerformMainMenueOption(enMainMenueOptions(ReadMainMenueOption()));
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
	if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
		return true;
	else
		return false;
}

void Login()
{
	bool LoginFaild = false;
	string AccountNumber, PinCode;

	do
	{
		system("cls");

		cout << PrintPunctuations('-', 32) << "\n";
		cout << PrintPunctuations('\t', 1) << "Login Screen" << "\n";
		cout << PrintPunctuations('-', 32) << "\n";

		if (LoginFaild)
		{
			cout << "Invlaid Account Number/PinCode!\n";
		}

		cout << "Enter Account Number? ";
		cin >> AccountNumber;

		cout << "Enter Pin? ";
		cin >> PinCode;

		LoginFaild = !LoadClientInfo(AccountNumber, PinCode);

	} while (LoginFaild);

	ShowMainMenue();
}


int main()
{
	Login();



	system("pause>0");
	return 0;
}