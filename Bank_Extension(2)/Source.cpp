#include<iostream>
#include<fstream>
#include<iomanip>
#include<vector>
#include<cctype>
#include<string>

using namespace std;

const string FileName = "file.txt";
const string UserFile = "User.txt";

struct sClient {
	string AccountNumber = "";
	string PINcode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0;
};

struct sUser {
	string UserName = "";
	string password = "";
	int Permission = 0;
};
sUser CurrentUser;

enum enOptionScreen {
	ClientList = 1,
	AddNewClientS = 2,
	DeleteClientS = 3,
	UpdateClientS = 4,
	FindClientS = 5,
	Transactions = 6,
	ManageUsers = 7,
	Logout = 8

};

enum enOptionManageUserScreen {
	UserList = 1,
	AddNewUser = 2,
	DeleteUser = 3,
	UpdateUser = 4,
	FindUser = 5,
	MainMenuS = 6


};

enum enOptionTransactionsScreen {
	Deposit = 1,
	Withdraw = 2,
	TotalBalances = 3,
	MainMenu = 4
};

void MainMenuScreen();
void TransactionsScreen();
void ManageUsersScreen();
void LoginScreen();
void BackToTransactionsScreen();

void printClient(sClient Client) {
	cout << "\nthe following are the client details :\n\n";
	cout << "----------------------------------\n";
	cout << "Account Number : " << Client.AccountNumber << endl;
	cout << "Pin code : " << Client.PINcode << endl;
	cout << "Name : " << Client.Name << endl;
	cout << "Phone : " << Client.Phone << endl;
	cout << "Account Balance : " << Client.AccountBalance << endl;
	cout << "----------------------------------\n";
}

vector<string> split(string Line, string separator = "#//#") {
	int pos = 0;
	string word = "";
	vector<string> vClient;
	while ((pos = Line.find(separator)) != std::string::npos) {
		word = Line.substr(0, pos);
		if (word != "")
		{
			vClient.push_back(word);
		}
		Line.erase(0, pos + separator.length());

	}
	if (Line != "")
	{
		vClient.push_back(Line);
	}
	return vClient;
}

string ReadAccountNumber() {
	string number;
	cout << "\nplease enter Account Number?";
	getline(cin >> ws, number);
	return number;
}

sClient LineToRecord(string Line) {
	sClient Client;
	vector<string> vClient = split(Line);
	Client.AccountNumber = vClient[0];
	Client.PINcode = vClient[1];
	Client.Name = vClient[2];
	Client.Phone = vClient[3];
	Client.AccountBalance = stod(vClient[4]);

	return Client;
}

void AddNewClientToFile(string Line) {
	fstream file;
	file.open(FileName, ios::out | ios::app);
	while (file.is_open()) {

		file << Line << endl;

		file.close();
	}
}

string RecordToLine(sClient Client, string separator = "#//#") {
	string Line = "";
	Line += Client.AccountNumber + separator;
	Line += Client.PINcode + separator;
	Line += Client.Name + separator;
	Line += Client.Phone + separator;
	Line += to_string(Client.AccountBalance);
	return Line;
}

vector<sClient> LoadClientFromFile() {
	vector<sClient> vClients;
	sClient Client;
	fstream file;
	string Line;
	file.open(FileName, ios::in);
	while (file.is_open()) {
		while (getline(file, Line))
		{

			Client = LineToRecord(Line);
			vClients.push_back(Client);

		}
		file.close();
	}
	return vClients;
}

bool SearchByAccountNumber(string AccountNumber, sClient& C) {

	vector<sClient> vClients = LoadClientFromFile();

	for (sClient& Client : vClients)
	{
		if (Client.AccountNumber == AccountNumber) {
			C = Client;

			return false;
		}

	}
	return true;

}

double ReadDeposit() {

	double num;
	cout << "Please Enter deposit amount ?";
	cin >> num;
	while (num < 0)
	{
		cout << "Amount is Negative = " << num << endl;
		cout << "Please Enter another amount (positive) ?";
		cin >> num;
	}
	return num;

}

double ReadWithdraw(sClient& c) {

	double num;
	cout << "Please Enter Withdraw amount ?";
	cin >> num;
	while (num < 0)
	{
		cout << "Amount is Negative = " << num << endl;
		cout << "Please Enter another amount (positive) ?";
		cin >> num;
	}
	while (num > c.AccountBalance)
	{
		cout << "Amount exceeds the Balance, you can withdraw up to :" << c.AccountBalance << endl;
		cout << "Please Enter another amount ?";

		cin >> num;
	}


	return num;

}

double DepositToAccount(double DepositAmount, string AccountNumber) {
	vector<sClient> vClients = LoadClientFromFile();
	fstream file;
	double AfterDeposit = 0;

	file.open(FileName, ios::out);
	while (file.is_open()) {
		for (sClient& client : vClients)
		{
			if (client.AccountNumber == AccountNumber)
			{
				client.AccountBalance += DepositAmount;
				file << RecordToLine(client) << endl;
				AfterDeposit = client.AccountBalance;
			}
			else
			{
				file << RecordToLine(client) << endl;
			}
		}

		file.close();
	}
	return 	AfterDeposit;
}

double WithdrawToAccount(double DepositAmount, string AccountNumber) {
	vector<sClient> vClients = LoadClientFromFile();
	fstream file;
	double AfterWithdraw = 0;
	file.open(FileName, ios::out);
	while (file.is_open()) {
		for (sClient& client : vClients)
		{
			if (client.AccountNumber == AccountNumber)
			{
				client.AccountBalance -= DepositAmount;
				file << RecordToLine(client) << endl;
				AfterWithdraw = client.AccountBalance;
			}
			else
			{
				file << RecordToLine(client) << endl;
			}
		}

		file.close();
	}
	return AfterWithdraw;
}

sClient ReadClientFromuser(string AccountNumber) {
	sClient Client;

	Client.AccountNumber = AccountNumber;
	cout << "Enter Pin code ?";
	getline(cin >> ws, Client.PINcode);
	cout << "Enter Name ?";
	getline(cin, Client.Name);
	cout << "Enter Phone ?";
	getline(cin, Client.Phone);
	cout << "Enter AccountBalance ?";
	cin >> Client.AccountBalance;
	cin.ignore();

	return Client;

}

void DeleteClientFromfile(string AccountNumber) {
	vector<sClient> vClients = LoadClientFromFile();
	fstream file;
	file.open(FileName, ios::out);
	while (file.is_open()) {
		for (sClient& Client : vClients)
		{
			if (Client.AccountNumber != AccountNumber)
			{
				file << RecordToLine(Client) << endl;
			}
		}
		file.close();
	}
}

void UpdateClientFromfile(string AccountNumber, sClient C)
{
	vector<sClient> vClients = LoadClientFromFile();
	fstream file;
	file.open(FileName, ios::out);
	while (file.is_open()) {
		for (sClient& Client : vClients)
		{
			if (Client.AccountNumber == AccountNumber)
			{
				file << RecordToLine(C) << endl;
			}
			else
			{
				file << RecordToLine(Client) << endl;
			}
		}
		file.close();
	}
}

void DepositScreen() {
	sClient Client;
	system("cls");
	cout << "----------------------------------\n";
	cout << "\t Deposit Screen\n";
	cout << "----------------------------------\n";
	string AccountNumber = ReadAccountNumber();
	while (SearchByAccountNumber(AccountNumber, Client))
	{
		cout << "\nClient with[" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadAccountNumber();
	}
	printClient(Client);
	double DepositAmount = ReadDeposit();
	cout << "\nAre you sure you want  perform this transactions? Y/N?";
	char ans;
	cin >> ans;
	if (toupper(ans) == 'Y')
	{
		double NewBalance = DepositToAccount(DepositAmount, AccountNumber);
		cout << "\nDone Successfully. New balance is : " << NewBalance << endl;;
	}
}

void WithdrawScreen() {
	sClient Client;
	system("cls");
	cout << "----------------------------------\n";
	cout << "\t Withdraw Screen\n";
	cout << "----------------------------------\n";
	string AccountNumber = ReadAccountNumber();
	while (SearchByAccountNumber(AccountNumber, Client))
	{
		cout << "\nClient with[" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadAccountNumber();
	}
	printClient(Client);


	double WithdrawAmount = ReadWithdraw(Client);

	cout << "\nAre you sure you want  perform this transactions? Y/N?";
	char ans;
	cin >> ans;
	if (toupper(ans) == 'Y')
	{
		double NewBalance = WithdrawToAccount(WithdrawAmount, AccountNumber);
		cout << "\nDone Successfully. New balance is : " << NewBalance << endl;
	}
}

void TotalBalanceScreen() {
	system("cls");
	vector<sClient> vClients = LoadClientFromFile();
	double SumBalanceOfAllAccount = 0;
	cout << "\t\t\t\tClient List (" << vClients.size() << ") Client(s).\n";
	cout << "__________________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(18) << "Account Number";
	cout << "| " << left << setw(35) << " Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n__________________________________________________________________________________________________________\n\n";
	if (vClients.size() == 0)
	{
		cout << "\n \t\t\t NO Client Available In The System\n";
	}
	else
	{
		for (sClient Client : vClients)
		{

			cout << "| " << left << setw(18) << Client.AccountNumber;
			cout << "| " << left << setw(35) << Client.Name;
			cout << "| " << left << setw(12) << Client.AccountBalance << endl;
			SumBalanceOfAllAccount += Client.AccountBalance;
		}
		cout << "\n__________________________________________________________________________________________________________\n";
		cout << "\n \t\t\t\t Total Balance = " << SumBalanceOfAllAccount << endl << endl;
	}
}

void BackToTransactionsScreen() {

	cout << "press any key to go back to Transactions Screen...\n";
	system("pause>0");
	TransactionsScreen();
}

void BackToMainMenuScreen() {


	cout << "press any key to go back to Main Menu...\n";
	system("pause>0");
	MainMenuScreen();

}

void BackToManageMenuScreen() {

	cout << "press any key to go back to Manage User Menu...\n";
	system("pause>0");
	ManageUsersScreen();

}

void ClientListScreen() {
	system("cls");

	vector<sClient> vClients = LoadClientFromFile();

	cout << "\t\t\t\tClient List (" << vClients.size() << ") Client(s).\n";
	cout << "__________________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(18) << "Account Number";
	cout << "| " << left << setw(11) << "Pin code";
	cout << "| " << left << setw(35) << " Client Name";
	cout << "| " << left << setw(15) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n__________________________________________________________________________________________________________\n\n";

	for (sClient Client : vClients)
	{
		cout << "| " << left << setw(18) << Client.AccountNumber;
		cout << "| " << left << setw(11) << Client.PINcode;
		cout << "| " << left << setw(35) << Client.Name;
		cout << "| " << left << setw(15) << Client.Phone;
		cout << "| " << left << setw(12) << Client.AccountBalance << endl;
	}
	cout << "\n__________________________________________________________________________________________________________\n";

}

void AddNewClientScreen() {
	char AddMoreClient;
	string Line;
	string AccountNumber;
	sClient C;
	do {
		system("cls");
		cout << "----------------------------------\n";
		cout << "\t Add New Client Screen\n";
		cout << "----------------------------------\n";
		cout << "Adding New Client :\n\n";
		cout << "Enter Account Number ?";
		getline(cin >> ws, AccountNumber);
		while (!SearchByAccountNumber(AccountNumber, C))
		{
			cout << "Client  with[" << AccountNumber << "] Already exist, Enter another Account Number ?";
			getline(cin, AccountNumber);
		}
		sClient c = ReadClientFromuser(AccountNumber);
		Line = RecordToLine(c);
		AddNewClientToFile(Line);

		cout << "Client Add successfully, do you want to add more clients? Y/N?";
		cin >> AddMoreClient;
	} while (toupper(AddMoreClient) == 'Y');

}

void deleteClientScreen() {

	sClient Client;
	system("cls");
	cout << "----------------------------------\n";
	cout << "\t Delete Client Screen\n";
	cout << "----------------------------------\n";
	string AccountNumber = ReadAccountNumber();
	if (!SearchByAccountNumber(AccountNumber, Client))
	{
		printClient(Client);
		cout << "\n\nAre you sure you want delete this Client? Y/N?";
		char Ans;
		cin >> Ans;
		if (toupper(Ans) == 'Y') {

			DeleteClientFromfile(AccountNumber);

			cout << "\n Client delete successfully.\n";
		}
	}
	else
	{
		cout << "Client with Account Number [" << AccountNumber << "] is Not Found! " << endl;
	}

}

void UpdateClientScreen() {
	sClient Client;
	system("cls");
	cout << "----------------------------------\n";
	cout << "\t Update Client  Info Screen\n";
	cout << "----------------------------------\n";
	string AccountNumber = ReadAccountNumber();
	if (!SearchByAccountNumber(AccountNumber, Client))
	{
		printClient(Client);
		cout << "\n\nAre you sure you want Update this Client? Y/N?";
		char Ans;
		cin >> Ans;
		if (toupper(Ans) == 'Y') {

			Client = ReadClientFromuser(AccountNumber);
			UpdateClientFromfile(AccountNumber, Client);

			cout << "\n Client Update successfully.\n";
		}
	}
	else
	{
		cout << "Client with Account Number [" << AccountNumber << "] is Not Found! " << endl;
	}

}

void FindClientScreen() {
	sClient Client;
	system("cls");
	cout << "----------------------------------\n";
	cout << "\t Find Client Screen\n";
	cout << "----------------------------------\n";
	string AccountNumber = ReadAccountNumber();
	if (!SearchByAccountNumber(AccountNumber, Client))
	{
		printClient(Client);
	}
	else
	{
		cout << "Client with Account Number [" << AccountNumber << "] is Not Found! " << endl;
	}

}

void TransactionsScreen() {
	system("cls");
	cout << "================================================\n";
	cout << "\t\t Transactions Screen\n";
	cout << "================================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] withdraw.\n";
	cout << "\t[3] Total Balance\n";
	cout << "\t[4] Main Menu.\n";
	cout << "================================================\n";
	short NumberYouWant;
	do {
		cout << "Choose what do you want to do? [1 to 4] ? ";
		cin >> NumberYouWant;
	} while (NumberYouWant < 1 || NumberYouWant > 4);
	switch (enOptionTransactionsScreen(NumberYouWant)) {
	case   enOptionTransactionsScreen::Deposit:
	{
		DepositScreen();
		BackToTransactionsScreen();
		break;
	}
	case   enOptionTransactionsScreen::Withdraw:
	{
		WithdrawScreen();
		BackToTransactionsScreen();
		break;
	}
	case   enOptionTransactionsScreen::TotalBalances:
	{
		TotalBalanceScreen();
		BackToTransactionsScreen();
		break;
	}
	case   enOptionTransactionsScreen::MainMenu:
	{
		MainMenuScreen();
		break;
	}

	}
}

void NotHavePermission() {
	system("cls");
	cout << "----------------------------------\n";
	cout << "Access Denied,\n You dont Have permission To Do this, \n Please Contact Your Admin";
	cout << "\n----------------------------------\n";
}

sUser ReadUser() {
	sUser User;
	cout << "\nEnter Username ? ";
	getline(cin >> ws, User.UserName);
	cout << "\nEnter Password ? ";
	getline(cin, User.password);
	return User;
}

sUser ReadUserName() {
	sUser User;
	cout << "\nEnter Username ? ";
	getline(cin >> ws, User.UserName);

	return User;
}

sUser LineToRecordUser(string Line)
{
	sUser User;
	vector<string> v = split(Line);
	User.UserName = v[0];
	User.password = v[1];
	User.Permission = stoi(v[2]);
	return User;
}

vector<sUser> LoadUserFromFile() {
	vector<sUser> users;
	string Line;
	fstream Users;
	Users.open(UserFile, ios::in);
	while (Users.is_open()) {
		while (getline(Users, Line)) {
			users.push_back(LineToRecordUser(Line));
		}

		Users.close();
	}
	return users;
}

string RecordToLineUser(sUser user, string separator = "#//#") {
	string line = "";
	line += user.UserName + separator;
	line += user.password + separator;
	line += to_string(user.Permission);
	return line;
}

void AddNewUserToFile(string line) {
	fstream user;
	user.open(UserFile, ios::out | ios::app);
	while (user.is_open()) {

		user << line << endl;
		user.close();
	}

}

void UpdateUserToFile(sUser& User) {
	vector<sUser> vUsers = LoadUserFromFile();
	fstream user;
	user.open(UserFile, ios::out);
	while (user.is_open()) {

		for (sUser& u : vUsers) {
			if (u.UserName == User.UserName) {
				u = User;
				user << RecordToLineUser(u) << endl;
			}
			else {
				user << RecordToLineUser(u) << endl;
			}
		}
		user.close();
	}

}

bool SearchUserByUserNameAndPassword(sUser& User) {

	vector<sUser> Users = LoadUserFromFile();
	for (sUser& user : Users) {
		if (user.UserName == User.UserName)
		{
			if (user.password == User.password)
			{
				User = user;
				return true;
			}
		}
	}
	return false;
}

bool SearchUserByUserName(sUser& User) {

	vector<sUser> Users = LoadUserFromFile();
	for (sUser& user : Users) {
		if (user.UserName == User.UserName) {
			User = user;
			return false;

		}

	}
	return true;
}

void Login() {

	system("cls");
	cout << "----------------------------------\n";
	cout << "\t Login Screen \n";
	cout << "----------------------------------\n";


}

void UserListScreen() {
	system("cls");
	vector<sUser> vUser = LoadUserFromFile();
	cout << "\t\t\t\tUser List (" << vUser.size() << ") User(s).\n";
	cout << "__________________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(18) << "User Name";
	cout << "| " << left << setw(11) << "password";
	cout << "| " << left << setw(35) << " permissions";
	cout << "\n__________________________________________________________________________________________________________\n\n";
	for (sUser& user : vUser)
	{
		cout << "| " << left << setw(18) << user.UserName;
		cout << "| " << left << setw(11) << user.password;
		cout << "| " << left << setw(35) << user.Permission << endl;

	}
	cout << "\n__________________________________________________________________________________________________________\n";

}

bool FullAccessToUser() {
	cout << "Do you want to give full access? Y/N ? ";
	char ans = ' ';
	cin >> ans;
	if (toupper(ans) == 'Y')
		return true;
	else if (toupper(ans) == 'N')
		return false;
}

int AccessToUser() {
	int Permission = 0;
	char ans = ' ';
	cout << "Do you Want to give access To:\n";
	cout << "Show Clients List? Y/N ?";
	cin >> ans;
	if (toupper(ans) == 'Y') {
		Permission++;
	}
	cout << "Add New Client? Y/N ?";
	cin >> ans;
	if (toupper(ans) == 'Y') {
		Permission += 2;
	}
	cout << "Delete Client? Y/N ?";
	cin >> ans;
	if (toupper(ans) == 'Y') {
		Permission += 4;
	}
	cout << "Update Client? Y/N ?";
	cin >> ans;
	if (toupper(ans) == 'Y') {
		Permission += 8;
	}
	cout << "Find Client? Y/N ?";
	cin >> ans;
	if (toupper(ans) == 'Y') {
		Permission += 16;
	}
	cout << "Transactions? Y/N ?";
	cin >> ans;
	if (toupper(ans) == 'Y') {
		Permission += 32;
	}
	cout << "Manage User? Y/N ?";
	cin >> ans;
	if (toupper(ans) == 'Y') {
		Permission += 64;
	}
	if (Permission == 127)
		return -1;

	return Permission;
}

void AddNewUserScreen() {
	system("cls");
	cout << "----------------------------------\n";
	cout << "\t Add New User Screen \n";
	cout << "----------------------------------\n";
	cout << "Adding New User:\n";
	sUser user;
	char Ans = ' ';
	do {
		user = ReadUserName();
		while (!SearchUserByUserName(user))
		{
			cout << "User With [" << user.UserName << "] already Exists ,Enter Another UserName ? ";
			getline(cin, user.UserName);

		}
		cout << "\nEnter Password ? ";
		getline(cin, user.password);
		bool ansAccess = FullAccessToUser();
		if (ansAccess)
		{
			user.Permission = -1;
		}
		else
		{
			user.Permission = AccessToUser();
		}
		AddNewUserToFile(RecordToLineUser(user));
		cout << "User Added successfully,do You want add more Users? Y/N?";
		cin >> Ans;
	} while (toupper(Ans) == 'Y');

}

void deleteUserFromFile(sUser& User) {
	vector<sUser> vUsers = LoadUserFromFile();
	fstream userfile;
	userfile.open(UserFile, ios::out);
	while (userfile.is_open()) {

		for (sUser& user : vUsers)
		{
			if (user.UserName != User.UserName)
			{
				userfile << RecordToLineUser(user) << endl;
			}
		}
		userfile.close();
	}

}

void PrintUserdetails(sUser user)
{
	cout << "\nthe following are the User details :\n\n";
	cout << "----------------------------------\n";
	cout << "User Name : " << user.UserName << endl;
	cout << "password : " << user.password << endl;
	cout << "permissions: " << user.Permission << endl;
	cout << "----------------------------------\n";
}

void DeleteUserScreen() {
	system("cls");
	cout << "----------------------------------\n";
	cout << "\t Delete User Screen \n";
	cout << "----------------------------------\n";
	sUser user = ReadUserName();
	char ans = ' ';
	if (user.UserName == "Admin")
	{
		cout << "you cannot delete This User." << endl;
	}
	else if (!SearchUserByUserName(user))
	{
		PrintUserdetails(user);
		cout << "Are You Sure want delete  this user? Y/n ?";
		cin >> ans;
		if (toupper(ans) == 'Y')
		{
			deleteUserFromFile(user);
			cout << "\nUser Delete Successfully\n";
		}


	}
	else
	{
		cout << "User With Username (" << user.UserName << ") is not Found!\n";
	}
}

void UpdateUserScreen() {
	system("cls");
	cout << "----------------------------------\n";
	cout << "\tUpdate User Screen \n";
	cout << "----------------------------------\n";
	sUser user = ReadUserName();
	char ans = ' ';
	if (!SearchUserByUserName(user))
	{

		PrintUserdetails(user);
		cout << "Are You Sure want Update this user? Y/n ?";
		cin >> ans;
		if (toupper(ans) == 'Y')
		{
			cout << "\nEnter Password ? ";
			getline(cin >> ws, user.password);
			bool ansAccess = FullAccessToUser();
			if (ansAccess)
			{
				user.Permission = -1;
			}
			else
			{
				user.Permission = AccessToUser();
			}
			UpdateUserToFile(user);

		}


	}
	else
	{
		cout << "User With Username (" << user.UserName << ") is not Found!\n";
	}
}

void FindUserScreen() {
	system("cls");
	cout << "----------------------------------\n";
	cout << "\tUpdate User Screen \n";
	cout << "----------------------------------\n";
	sUser user = ReadUserName();
	if (!SearchUserByUserName(user))
	{
		PrintUserdetails(user);
	}
	else {
		cout << "User With Username (" << user.UserName << ") is not Found!\n";
	}
}
void ManageUsersScreen() {
	system("cls");
	cout << "---------------------------------------\n";
	cout << "\t manage Users Menu Screen \n";
	cout << "---------------------------------------\n";
	cout << "================================================\n";
	cout << "\t[1] List Users.\n";
	cout << "\t[2] Add New User.\n";
	cout << "\t[3] Delete User\n";
	cout << "\t[4] Update User.\n";
	cout << "\t[5] Find User.\n";
	cout << "\t[6] Main Menu.\n";
	cout << "================================================\n";
	short NumberYouWant;
	do {
		cout << "Choose what do you want to do? [1 to 6] ? ";
		cin >> NumberYouWant;
	} while (NumberYouWant < 1 || NumberYouWant > 6);

	switch (enOptionManageUserScreen(NumberYouWant)) {
	case   enOptionManageUserScreen::UserList:
	{
		UserListScreen();
		BackToManageMenuScreen();
		break;
	}
	case enOptionManageUserScreen::AddNewUser:
	{
		AddNewUserScreen();
		BackToManageMenuScreen();
		break;
	}
	case  enOptionManageUserScreen::DeleteUser:
	{
		DeleteUserScreen();
		BackToManageMenuScreen();
		break;
	}
	case  enOptionManageUserScreen::UpdateUser:
	{
		UpdateUserScreen();
		BackToManageMenuScreen();
		break;
	}
	case  enOptionManageUserScreen::FindUser:
	{
		FindUserScreen();
		BackToManageMenuScreen();
		break;
	}
	case  enOptionManageUserScreen::MainMenuS:
	{
		MainMenuScreen();
		break;
	}

	}
}

void MainMenuScreen() {
	system("cls");
	cout << "================================================\n";
	cout << "\t\t Main Menu Screen\n";
	cout << "================================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client\n";
	cout << "\t[4] Update Client.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Manage Users.\n";
	cout << "\t[8] logout.\n";
	cout << "================================================\n";
	short NumberYouWant;

	do {
		cout << "Choose what do you want to do? [1 to 8] ? ";
		cin >> NumberYouWant;
	} while (NumberYouWant < 1 || NumberYouWant > 8);

	switch (enOptionScreen(NumberYouWant)) {
	case   enOptionScreen::ClientList:
	{
		if ((CurrentUser.Permission & 1) == 1 || CurrentUser.Permission == -1) {
			ClientListScreen();
			BackToMainMenuScreen();
			break;
		}
		else
		{
			NotHavePermission();
			BackToMainMenuScreen();
			break;
		}

	}
	case  enOptionScreen::AddNewClientS:
	{
		if ((CurrentUser.Permission & 2) == 2 || CurrentUser.Permission == -1) {
			AddNewClientScreen();
			BackToMainMenuScreen();
			break;
		}
		else
		{
			NotHavePermission();
			BackToMainMenuScreen();
			break;
		}
	}
	case  enOptionScreen::DeleteClientS:
	{
		if ((CurrentUser.Permission & 4) == 4 || CurrentUser.Permission == -1) {
			deleteClientScreen();
			BackToMainMenuScreen();
			break;
		}
		else {
			NotHavePermission();
			BackToMainMenuScreen();
			break;
		}

	}
	case  enOptionScreen::UpdateClientS:
	{
		if ((CurrentUser.Permission & 8) == 8 || CurrentUser.Permission == -1) {
			UpdateClientScreen();
			BackToMainMenuScreen();
			break;
		}
		else
		{
			NotHavePermission();
			BackToMainMenuScreen();
			break;
		}
	}
	case  enOptionScreen::FindClientS:
	{
		if ((CurrentUser.Permission & 16) == 16 || CurrentUser.Permission == -1)
		{
			FindClientScreen();
			BackToMainMenuScreen();
			break;
		}
		else {
			NotHavePermission();
			BackToMainMenuScreen();
			break;
		}
	}
	case  enOptionScreen::Transactions:
	{
		if ((CurrentUser.Permission & 32) == 32 || CurrentUser.Permission == -1)
		{
			TransactionsScreen();
			break;
		}
		else
		{
			NotHavePermission();
			BackToMainMenuScreen();
			break;
		}
	}
	case  enOptionScreen::ManageUsers:
	{
		if ((CurrentUser.Permission & 64) == 64 || CurrentUser.Permission == -1)
		{
			ManageUsersScreen();
			break;
		}
		else {
			NotHavePermission();
			BackToMainMenuScreen();
			break;
		}
	}
	case enOptionScreen::Logout:
	{
		LoginScreen();
		break;
	}
	}
}

void LoginScreen()
{
	Login();
	sUser user;

	while (true) {
		user = ReadUser();

		if (SearchUserByUserNameAndPassword(user)) {
			CurrentUser = user;
			break;
		}
		else
		{
			Login();
			cout << "\nInvalid UserName/Password!\n";
		}

	}

	MainMenuScreen();
}

int main() {

	LoginScreen();

	system("pause>0");
	return 0;

}