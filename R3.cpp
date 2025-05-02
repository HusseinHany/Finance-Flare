#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;																				

class RecordType {
public:
	static const int income = 0;
	static const int expense = 1;

};

class Transaction{

protected:
	string description;
	double amount;
	string date;
	string category;

public:

	// Constructor
	Transaction(string d,double a , string dt, string c){
		description = d;
		amount = a;
		date = dt;
		category = c;
	}

	// Setters 
	void setDescription(string d) {
		description = d;
	}
	void setAmount(double a) {
		amount = a;
	}
	void setDate(string dt) {
		date = dt;
	}
	void setCategory(string c) {
		category = c;
	}
	// Getters
	// Getters
string getDescription() const {
    return description;
}
double getAmount() const {
    return amount;
}
string getDate() const {
    return date;
}
string getCategory() const {
    return category;
}




};

class Income : public Transaction {

public:
	// Constructor

	Income(string d, double a, string dt, string c) : Transaction(d, a, dt, c) {
		description = d;
		amount = a;
		date = dt;
		category = c;
	}

	void display() const {
		cout << "Income Transaction: " << endl;
		cout << "Description: " << description << endl;
		cout << "Amount: $" << amount << endl;
		cout << "Date: " << date << endl;
		cout << "Category: " << category << endl;
	}
	
	void saveToFile() {
		ofstream file("income.txt", ios::app);
		if (file.is_open()) {
			file << description << "," << amount << "," << date << "," << category << endl;
			file.close();
		}
		else {
			cout << "Error opening file!" << endl;
		}
	}
	// Function to read income transactions from file
	static vector<Income> readFromFile() {
		vector<Income> transactions;
		ifstream file("income.txt");
		if (file.is_open()) {
			string line;
			while (getline(file, line)) {
				size_t pos = 0;
				string token;
				vector<string> tokens;
				while ((pos = line.find(",")) != string::npos) {
					token = line.substr(0, pos);
					tokens.push_back(token);
					line.erase(0, pos + 1);
				}
				tokens.push_back(line); // Last token
				if (tokens.size() == 4) {
					transactions.push_back(Income(tokens[0], stod(tokens[1]), tokens[2], tokens[3]));
				}
			}
			file.close();
		}
		else {
			cout << "Error opening file!" << endl;
		}
		return transactions;
	}
};

class Expense : public Transaction {
public:
	// Constructor
	Expense(string d, double a, string dt, string c) : Transaction(d, a, dt, c) {
		description = d;
		amount = a;
		date = dt;
		category = c;
	}
	void display() const {
		cout << "Expense Transaction: " << endl;
		cout << "Description: " << description << endl;
		cout << "Amount: $" << amount << endl;
		cout << "Date: " << date << endl;
		cout << "Category: " << category << endl;
	}

	void saveToFile() {
		ofstream file("expense.txt", ios::app);
		if (file.is_open()) {
			file << description << "," << amount << "," << date << "," << category << endl;
			file.close();
		}
		else {
			cout << "Error opening file!" << endl;
		}
	}
	// Function to read expense transactions from file
	static vector<Expense> readFromFile() {
		vector<Expense> transactions;
		ifstream file("expense.txt");
		if (file.is_open()) {
			string line;
			while (getline(file, line)) {
				size_t pos = 0;
				string token;
				vector<string> tokens;
				while ((pos = line.find(",")) != string::npos) {
					token = line.substr(0, pos);
					tokens.push_back(token);
					line.erase(0, pos + 1);
				}
				tokens.push_back(line); // Last token
				if (tokens.size() == 4) {
					transactions.push_back(Expense(tokens[0], stod(tokens[1]), tokens[2], tokens[3]));
				}
			}
			file.close();
		}
		else {
			cout << "Error opening file!" << endl;
		}
		return transactions;
	}
};

class Filemanager : public Transaction{

public:
	static void saveTransaction(const Transaction& transaction) {
		ofstream file("transactions.txt", ios::app);
		if (file.is_open()) {
			file << transaction.getDescription() << "," << transaction.getAmount() << "," << transaction.getDate() << "," << transaction.getCategory() << endl;
			file.close();
		}
		else {
			cout << "Error opening file!" << endl;
		}
	}

	static vector<Transaction> readTransactions() {
		vector<Transaction> transactions;
		ifstream file("transactions.txt");
		if (file.is_open()) {
			string line;
			while (getline(file, line)) {
				size_t pos = 0;
				string token;
				vector<string> tokens;
				while ((pos = line.find(",")) != string::npos) {
					token = line.substr(0, pos);
					tokens.push_back(token);
					line.erase(0, pos + 1);
				}
				tokens.push_back(line); // Last token
				if (tokens.size() == 4) {
					transactions.push_back(Transaction(tokens[0], stod(tokens[1]), tokens[2], tokens[3]));
				}
			}
			file.close();
		}
		else {
			cout << "Error opening file!" << endl;
		}
		return transactions;
	}

	static void displayTransactions() {
		vector<Transaction> transactions = readTransactions();
		for (const auto& transaction : transactions) {
			cout << "Description: " << transaction.getDescription() << endl;
			cout << "Amount: $" << transaction.getAmount() << endl;
			cout << "Date: " << transaction.getDate() << endl;
			cout << "Category: " << transaction.getCategory() << endl;
			cout << "------------------------" << endl;
		}
	}


};

class FinananceManager{

public:
	static void addIncome(string description, double amount, string date, string category) {
		Income income(description, amount, date, category);
		income.saveToFile();
	}
	static void addExpense(string description, double amount, string date, string category) {
		Expense expense(description, amount, date, category);
		expense.saveToFile();
	}
	static void viewIncome() {
		vector<Income> incomes = Income::readFromFile();
		for (const auto& income : incomes) {
			income.display();
			cout << "------------------------" << endl;
		}
	}
	static void viewExpense() {
		vector<Expense> expenses = Expense::readFromFile();
		for (const auto& expense : expenses) {
			expense.display();
			cout << "------------------------" << endl;
		}
	}
	static void viewAllTransactions() {
		Filemanager::displayTransactions();
	}
	static void deleteTransaction(string description) {
		vector<Transaction> transactions = Filemanager::readTransactions();
		ofstream file("transactions.txt");
		if (file.is_open()) {
			for (const auto& transaction : transactions) {
				if (transaction.getDescription() != description) {
					file << transaction.getDescription() << "," << transaction.getAmount() << "," << transaction.getDate() << "," << transaction.getCategory() << endl;
				}
			}
			file.close();
		}
		else {
			cout << "Error opening file!" << endl;
		}
	}

};

class UserInterface {
public:
	static void displayMenu() {
		cout << "Welcome to the Finance Manager!" << endl;
		cout << "1. Add Income" << endl;
		cout << "2. Add Expense" << endl;
		cout << "3. View Income" << endl;
		cout << "4. View Expense" << endl;
		cout << "5. View All Transactions" << endl;
		cout << "6. Delete Transaction" << endl;
		cout << "7. Exit" << endl;
	}
};

class screen {
public:
	static void clear() {
		system("cls");
	}
};

class User {
public:
	string username;
	string password;
	User(string u, string p) {
		username = u;
		password = p;
	}
};

class UserManager {
private:
	vector<User> users;
public:
	void addUser(string username, string password) {
		users.push_back(User(username, password));
	}
	bool authenticate(string username, string password) {
		for (const auto& user : users) {
			if (user.username == username && user.password == password) {
				return true;
			}
		}
		return false;
	}
};

class Authentication {
private:
	UserManager userManager;
public:
	Authentication() {
		userManager.addUser("admin", "password");
	}
	bool login(string username, string password) {
		return userManager.authenticate(username, password);
	}
};

int main() {
	Authentication auth;
	string username, password;
	cout << "Enter username: ";
	cin >> username;
	cout << "Enter password: ";
	cin >> password;
	if (!auth.login(username, password)) {
		cout << "Invalid username or password!" << endl;
		return 0;
	}
	int choice;
	do {
		screen::clear();
		UserInterface::displayMenu();
		cout << "Enter your choice: ";
		cin >> choice;
		switch (choice) {
		case 1: {
			string description, date, category;
			double amount;
			cout << "Enter income description: ";
			cin.ignore();
			getline(cin, description);
			cout << "Enter income amount: ";
			cin >> amount;
			cout << "Enter income date (YYYY-MM-DD): ";
			cin >> date;
			cout << "Enter income category: ";
			cin.ignore();
			getline(cin, category);
			FinananceManager::addIncome(description, amount, date, category);
			break;
		}
		case 2: {
			string description, date, category;
			double amount;
			cout << "Enter expense description: ";
			cin.ignore();
			getline(cin, description);
			cout << "Enter expense amount: ";
			cin >> amount;
			cout << "Enter expense date (YYYY-MM-DD): ";
			cin >> date;
			cout << "Enter expense category: ";
			cin.ignore();
			getline(cin, category);
			FinananceManager::addExpense(description, amount, date, category);
			break;
		}
		case 3:
			screen::clear();
			FinananceManager::viewIncome();
			system("pause");
			break;
		case 4:
			screen::clear();
			FinananceManager::viewExpense();
			system("pause");

			break;
		case 5:
			screen::clear();
			FinananceManager::viewAllTransactions();
			system("pause");
			break;
		case 6: {
			string description;
			cout << "Enter the description of the transaction to delete: ";
			cin.ignore();
			getline(cin, description);
			FinananceManager::deleteTransaction(description);
			break;
		}
		case 7:
			cout << "Exiting..." << endl;
			break;
		default:
			cout << "Invalid choice! Please try again." << endl;
			break;
		}
	} while (choice != 7);
	return 0;
}
