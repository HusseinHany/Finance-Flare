#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <thread> 
#include <chrono>
using namespace std;

class RecordType {
public:
	static const int income = 0;
	static const int expense = 1;

};

class Transaction {

protected:
	string description;
	double amount;
	string date;
	string category;

public:

	// Constructor
	Transaction(string d, double a, string dt, string c) {
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

class Filemanager : public Transaction {

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

class FinananceManager {

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
			cout << "Analyzing your balance. Please wait...\n";
			this_thread::sleep_for(chrono::seconds(7)); // 7 seconds delay
			cout << "1. High-risk Investment: $ -> (36%) " << fixed << setprecision(2) << endl;
			cout << "2. Low-risk Investment: $ -> (12%) " << fixed << setprecision(2) << endl;
			cout << "3. Medium-risk Investment: $ -> (24%) " << fixed << setprecision(2) << endl;
			cout << "4. Cryptocurrency Investment: $ -> (9%) " << fixed << setprecision(2) << endl;
			cout << "5. Diversified Portfolio: $ -> (3%) " << fixed << setprecision(2) << endl;
			cout << "6. Savings Account: $ -> (6%) " << fixed << setprecision(2) << endl;
			cout << "7. Retirement Account: $ -> (3%) " << fixed << setprecision(2) << endl;
			cout << "8. Emergency Fund: $ -> (3.6%) " << fixed << setprecision(2) << endl;
			cout << "9. Investment in Education: $ -> (1.2%) " << fixed << setprecision(2) << endl;
			cout << "10. Investment in Health: $ -> (2%) " << fixed << setprecision(2) << endl;
			cout << "11. Real Estate Investment: $ -> (18%) " << fixed << setprecision(2) << endl;
			cout << "12. Bonds: $ -> (9%) " << fixed << setprecision(2) << endl;
			cout << "13. Mutual Funds: $ -> (9%) " << fixed << setprecision(2) << endl;
			cout << "14. Stock Market Investment: $ -> (24%) " << fixed << setprecision(2) << endl;
			cout << "15. Precious Metals (Gold/Silver): $ -> (12%) " << fixed << setprecision(2) << endl;
			cout << "16. Peer-to-Peer Lending: $ -> (18%) " << fixed << setprecision(2) << endl;
			cout << "17. Hedge Funds: $ -> (24%) " << fixed << setprecision(2) << endl;
			cout << "18. Venture Capital: $ -> (36%) " << fixed << setprecision(2) << endl;
			cout << "19. Art and Collectibles: $ -> (8.4%) " << fixed << setprecision(2) << endl;
			cout << "20. Carbon Credit Investment: $ -> (6%) " << fixed << setprecision(2) << endl;
		}

		int x, m;
		double bal;
		cout << "Choose What Is the Most Sutable case For You\n";
		cin >> x;
		cout << "Please Enter The Amount You Want To Invest\n";
		cin >> bal;
		cout << "Please Enter The Duration In Months\n";
		cin >> m;

		cout << "You Have Chosen Investment Option " << x << " with an amount of $" << fixed << setprecision(2) << bal << " for a duration of " << m << " months." << endl;
		cout << "Your investment will be monitored and adjusted based on market conditions." << endl;
		cout << "Please wait while we process your investment...\n";
		this_thread::sleep_for(chrono::seconds(3)); // 7 seconds delay
		cout << "Investment processed successfully!" << endl;

		switch (x)
		{
		case 1: {
			double highRiskROI = 0.03;  // 3% monthly ROI
			double highRiskProfit = bal * highRiskROI * m;
			cout << "High-risk Investment: $" << fixed << setprecision(2) << balance << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << highRiskProfit << endl;
			break;
		}
		case 2: {
			double lowRiskROI = 0.01;  // 1% monthly ROI
			double lowRiskProfit = bal * lowRiskROI * m;
			cout << "Low-risk Investment: $" << fixed << setprecision(2) << bal << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << lowRiskProfit << endl;
			break;
		}
		case 3: {
			double mediumRiskROI = 0.02;  // 2% monthly ROI
			double mediumRiskProfit = bal * mediumRiskROI * m;
			cout << "Medium-risk Investment: $" << fixed << setprecision(2) << bal << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << mediumRiskProfit << endl;
			break;
		}
		case 4: {
			double cryptoROI = 0.0167;  // 1.67% monthly ROI
			double cryptoProfit = bal * cryptoROI * m;
			cout << "Cryptocurrency Investment: $" << fixed << setprecision(2) << bal << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << cryptoProfit << endl;
			break;
		}
		case 5: {
			double diversifiedROI = 0.0075;  // 0.75% monthly ROI
			double diversifiedProfit = bal * diversifiedROI * m;
			cout << "Diversified Portfolio: $" << fixed << setprecision(2) << bal << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << diversifiedProfit << endl;
			break;
		}
		case 6: {
			double savingsAccountROI = 0.0025;  // 0.25% monthly ROI
			double savingsProfit = bal * savingsAccountROI * m;
			cout << "Savings Account: $" << fixed << setprecision(2) << bal << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << savingsProfit << endl;
			break;
		}
		case 7: {
			double retirementAccountROI = 0.005;  // 0.5% monthly ROI
			double retirementProfit = bal * retirementAccountROI * m;
			cout << "Retirement Account: $" << fixed << setprecision(2) << bal << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << retirementProfit << endl;
			break;
		}
		case 8: {
			double emergencyFundROI = 0.003;  // 0.3% monthly ROI
			double emergencyFundProfit = bal * emergencyFundROI * m;
			cout << "Emergency Fund: $" << fixed << setprecision(2) << bal << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << emergencyFundProfit << endl;
			break;
		}
		case 9: {
			double educationROI = 0.001;  // 0.1% monthly ROI
			double educationProfit = bal * educationROI * m;
			cout << "Investment in Education: $" << fixed << setprecision(2) << bal << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << educationProfit << endl;
			break;
		}
		case 10: {
			double healthROI = 0.001;  // 0.1% monthly ROI
			double healthProfit = bal * healthROI * m;
			cout << "Investment in Health: $" << fixed << setprecision(2) << bal << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << healthProfit << endl;
			break;
		}
		case 11: {
			double realEstateROI = 0.015;  // 1.5% monthly ROI
			double realEstateProfit = bal * realEstateROI * m;
			cout << "Real Estate Investment: $" << fixed << setprecision(2) << bal << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << realEstateProfit << endl;
			break;
		}
		case 12: {
			double bondsROI = 0.005;  // 0.5% monthly ROI
			double bondsProfit = bal * bondsROI * m;
			cout << "Bonds: $" << fixed << setprecision(2) << bal << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << bondsProfit << endl;
			break;
		}
		case 13: {
			double mutualFundsROI = 0.0075;  // 0.75% monthly ROI
			double mutualFundsProfit = bal * mutualFundsROI * m;
			cout << "Mutual Funds: $" << fixed << setprecision(2) << bal << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << mutualFundsProfit << endl;
			break;
		}
		case 14: {
			double stockMarketROI = 0.02;  // 2% monthly ROI
			double stockMarketProfit = bal * stockMarketROI * m;
			cout << "Stock Market Investment: $" << fixed << setprecision(2) << bal << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << stockMarketProfit << endl;
			break;
		}
		case 15: {
			double preciousMetalsROI = 0.01;  // 1% monthly ROI
			double preciousMetalsProfit = bal * preciousMetalsROI * m;
			cout << "Precious Metals (Gold/Silver): $" << fixed << setprecision(2) << bal << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << preciousMetalsProfit << endl;
			break;
		}
		case 16: {
			double peerToPeerROI = 0.015;  // 1.5% monthly ROI
			double peerToPeerProfit = bal * peerToPeerROI * m;
			cout << "Peer-to-Peer Lending: $" << fixed << setprecision(2) << bal << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << peerToPeerProfit << endl;
			break;
		}
		case 17: {
			double hedgeFundsROI = 0.02;  // 2% monthly ROI
			double hedgeFundsProfit = bal * hedgeFundsROI * m;
			cout << "Hedge Funds: $" << fixed << setprecision(2) << bal << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << hedgeFundsProfit << endl;
			break;
		}
		case 18: {
			double ventureCapitalROI = 0.03;  // 3% monthly ROI
			double ventureCapitalProfit = bal * ventureCapitalROI * m;
			cout << "Venture Capital: $" << fixed << setprecision(2) << bal << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << ventureCapitalProfit << endl;
			break;
		}
		case 19: {
			double artAndCollectiblesROI = 0.005;  // 0.5% monthly ROI
			double artAndCollectiblesProfit = bal * artAndCollectiblesROI * m;
			cout << "Art and Collectibles: $" << fixed << setprecision(2) << bal << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << artAndCollectiblesProfit << endl;
			break;
		}
		case 20: {
			double carbonCreditROI = 0.007;  // 0.7% monthly ROI
			double carbonCreditProfit = bal * carbonCreditROI * m;
			cout << "Carbon Credit Investment: $" << fixed << setprecision(2) << bal << endl;
			cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << carbonCreditProfit << endl;
			break;
		}
		default:
			cout << "Invalid choice! Please try again." << endl;
			break;
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

	static void handleUserChoice(int choice) {
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
		UserInterface::handleUserChoice(choice);
	} while (choice != 8);
	return 0;
}
