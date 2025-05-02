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
    static double calculateTotal() {
        double total = 0.0;
        vector<Income> incomes = readFromFile();
        for (const auto& income : incomes) {
            total += income.getAmount();
        }
        return total;
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
    static double calculateTotal() {
        double total = 0.0;
        vector<Expense> expenses = readFromFile();
        for (const auto& expense : expenses) {
            total += expense.getAmount();
        }
        return total;
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

    static void suggestInvestmentsAndOptions() {
    double totalIncome = Income::calculateTotal();
    double totalExpenses = Expense::calculateTotal();
    double balance = totalIncome - totalExpenses;

    if (balance <= 0) {
        cout << "\nYou currently don't have a positive balance for investment in your accounts.\n";
        return;
    }

    cout << "\n=== Investment Suggestions ===\n";
    cout << "Your Total Income: $" << fixed << setprecision(2) << totalIncome << endl;
    cout << "Your Total Expenses: $" << fixed << setprecision(2) << totalExpenses << endl;
    cout << "Your Available Balance: $" << fixed << setprecision(2) << balance << endl << endl;

	 if (balance < 10000) {
        cout << "Emergency Fund: $" << fixed << setprecision(2) << (balance * 0.8) << endl;
        cout << "Low-risk Investment: $" << fixed << setprecision(2) << (balance * 0.2) << endl;
	 } 
	 
	 else {
		cout << "1. High-risk Investment: $" << fixed << setprecision(2) << (balance * 0.1) << endl;
		cout << "2. Low-risk Investment: $" << fixed << setprecision(2) << (balance * 0.3) << endl;	
		cout << "3. Medium-risk Investment: $" << fixed << setprecision(2) << (balance * 0.2) << endl;
		cout << "4. Cryptocurrency Investment: $" << fixed << setprecision(2) << (balance * 0.1) << endl;
		cout << "5. Diversified Portfolio: $" << fixed << setprecision(2) << (balance * 0.4) << endl;
		cout << "6. Savings Account: $" << fixed << setprecision(2) << (balance * 0.5) << endl;
		cout << "7. Retirement Account: $" << fixed << setprecision(2) << (balance * 0.3) << endl;
		cout << "8. Emergency Fund: $" << fixed << setprecision(2) << (balance * 0.2) << endl;
		cout << "9. Investment in Education: $" << fixed << setprecision(2) << (balance * 0.1) << endl;	
		cout << "10. Investment in Health: $" << fixed << setprecision(2) << (balance * 0.1) << endl;
		cout << "11. Real Estate Investment: $" << fixed << setprecision(2) << (balance * 0.25) << endl;	
		cout << "12. Bonds: $" << fixed << setprecision(2) << (balance * 0.15) << endl;
		cout << "13. Mutual Funds: $" << fixed << setprecision(2) << (balance * 0.2) << endl;
		cout << "14. Stock Market Investment: $" << fixed << setprecision(2) << (balance * 0.35) << endl;
		cout << "15. Precious Metals (Gold/Silver): $" << fixed << setprecision(2) << (balance * 0.1) << endl;
		cout << "16. Peer-to-Peer Lending: $" << fixed << setprecision(2) << (balance * 0.05) << endl;
		cout << "17. Hedge Funds: $" << fixed << setprecision(2) << (balance * 0.2) << endl;
		cout << "18. Venture Capital: $" << fixed << setprecision(2) << (balance * 0.15) << endl;
		cout << "19. Art and Collectibles: $" << fixed << setprecision(2) << (balance * 0.05) << endl;
		cout << "20. Carbon Credit Investment: $" << fixed << setprecision(2) << (balance * 0.05) << endl;
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
                cout << "7. Investment Suggestions" << endl;
		cout << "8. Exit" << endl;
	}
};

class screen {
public:
	static void clear() {
		system("cls");
	}
};

int main() {
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
                    screen::clear();
                    FinananceManager::suggestInvestmentsAndOptions();
                    system("pause");
                    break;
		case 8:
			cout << "Exiting..." << endl;
			break;
		default:
			cout << "Invalid choice! Please try again." << endl;
			break;
		}
	} while (choice != 8);
	return 0;
}
