#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <thread>
#include <chrono>
#include <cstdlib>
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
    // Parameterized Constructor
    Transaction(string d, double a, string dt, string c) {
        description = d;
        amount = a;
        date = dt;
        category = c;
    }

    // Setters and Getters 
    void setDescription(string d) { description = d; }
    void setAmount(double a) { amount = a; }
    void setDate(string dt) { date = dt; }
    void setCategory(string c) { category = c; }

    string getDescription() const { return description; }
    double getAmount() const { return amount; }
    string getDate() const { return date; }
    string getCategory() const { return category; }
	virtual ~Transaction() {}
};

struct Investment {
    int optionNumber;
    int durationMonths;
    double amount;
    string date;
    
    Investment(int opt, int dur, double amt, string dt) :  optionNumber(opt), durationMonths(dur), amount(amt), date(dt) {}
};

class Filemanager {
public:
	static void saveTransaction(const Transaction& transaction) {
		ofstream file("transactions.txt", ios::app);
		if (file.is_open()) {
			file << transaction.getDescription() << "," << transaction.getAmount() << "," << transaction.getDate() << "," << transaction.getCategory() << endl;
			file.close();
		}
		else {
			cout << "\033[31mError opening file!\033[0m" << endl;
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

            // Split the line by commas
            while ((pos = line.find(",")) != string::npos) {
                token = line.substr(0, pos);
                tokens.push_back(token);
                line.erase(0, pos + 1);
            }
            tokens.push_back(line); // Last token

            // Create a Transaction object if all fields are present
            if (tokens.size() == 4) {
                transactions.emplace_back(tokens[0], stod(tokens[1]), tokens[2], tokens[3]);
            }
        }
        file.close();
    } else {
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

	static void deleteTransaction(const string& description) {
		vector<Transaction> transactions = readTransactions();
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
			cout << "\033[31mError opening file!\033[0m" << endl;
		}
	}

	static void saveInvestment(const int& optionNumber, const int& durationMonths, const double& amount, const string& date) {
		ofstream file("investments.txt", ios::app);
		if (file.is_open()) {
			file << optionNumber << "," << durationMonths << "," << amount << "," << date << endl;
			file.close();
            cout << "\033[32mInvestment saved successfully!\033[0m" << endl;
		}
		else {
			cout << "\033[31mError opening investments file!\033[0m" << endl;
		}
	}
	
	static vector<Investment> readInvestments() {
		vector<Investment> investments;
		ifstream file("investments.txt");
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
					investments.emplace_back(
                        stoi(tokens[0]),    // optionNumber
                        stoi(tokens[1]),    // durationMonths
                        stod(tokens[2]),    // amount
                        tokens[3]           // date
                    );
				}
			}
			file.close();
		}
		else {
			cout << "\033[31mInvestments file not found or could not be opened!\033[0m" << endl;
		}
		return investments;
	}

	static void viewInvestments() {
		vector<Investment> investments = readInvestments();
		
		if (investments.empty()) {
			cout << "\033[33mNo investments found!\033[0m" << endl;
			return;
		}
		
		cout << "\n==== Your Investments ====\n";
		for (const auto& investment : investments) {
			cout << "\033[36mInvestment Option: \033[0m" << investment.optionNumber << endl;
			cout << "\033[36mAmount Invested: \033[0m$" << fixed << setprecision(2) << investment.amount << endl;
			cout << "\033[36mDuration: \033[0m" << investment.durationMonths << " months" << endl;
            cout << "\033[36mDate Invested: \033[0m" << investment.date << endl;
			
			double monthlyROI = 0.01; // Default 1% monthly return
			
			switch (investment.optionNumber) {
				case 1: monthlyROI = 0.03; break;    // High Risk
				case 2: monthlyROI = 0.01; break;    // Low Risk
				case 3: monthlyROI = 0.02; break;    // Medium Risk
				case 4: monthlyROI = 0.0167; break;  // Cryptocurrency
				case 5: monthlyROI = 0.0075; break;  // Diversified Portfolio
				case 6: monthlyROI = 0.0025; break;  // Savings Account
				case 7: monthlyROI = 0.005; break;   // Retirement Account
				case 8: monthlyROI = 0.003; break;   // Emergency Fund
				case 9: monthlyROI = 0.001; break;   // Education
				case 10: monthlyROI = 0.001; break;  // Health
				case 11: monthlyROI = 0.015; break;  // Real Estate
				case 12: monthlyROI = 0.005; break;  // Bonds
				case 13: monthlyROI = 0.0075; break; // Mutual Funds
				case 14: monthlyROI = 0.02; break;   // Stock Market
				case 15: monthlyROI = 0.01; break;   // Precious Metals
				case 16: monthlyROI = 0.015; break;  // Peer-to-Peer Lending
				case 17: monthlyROI = 0.02; break;   // Hedge Funds
				case 18: monthlyROI = 0.03; break;   // Venture Capital
				case 19: monthlyROI = 0.005; break;  // Art and Collectibles
				case 20: monthlyROI = 0.007; break;  // Carbon Credit
			}
			
			double expectedProfit = investment.amount * monthlyROI * investment.durationMonths;
			cout << "\033[32mExpected Profit: \033[0m$" << fixed << setprecision(2) << expectedProfit << endl;
			cout << "\033[32mTotal Expected Value: \033[0m$" << fixed << setprecision(2) 
                 << (investment.amount + expectedProfit) << endl;
			cout << "------------------------" << endl;
		}
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
		cout << "\033[33mIncome Transaction: \033[0m" << endl;
		cout << "\033[36mDescription: \033[0m" << description << endl;
		cout << "\033[36mAmount: \033[0m" << amount << "\033[36m $\033[0m" << endl;
		cout << "\033[36mDate: \033[0m" << date << endl;
		cout << "\033[36mCategory: \033[0m" << category << endl;
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
			cout << "\033[31mError opening file!\033[0m" << endl;
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
		cout << "\033[33mExpense Transaction:\033[0m" << endl;
		cout << "\033[36mDescription: \033[0m" << description << endl;
		cout << "\033[36mAmount: \033[0m" << amount << "\033[36m$\033[0m" << endl;
		cout << "\033[36mDate: \033[0m" << date << endl;
		cout << "\033[36mCategory: \033[0m" << category << endl;
	}

	void saveToFile() {
		ofstream file("expense.txt", ios::app);
		if (file.is_open()) {
			file << description << "," << amount << "," << date << "," << category << endl;
			file.close();
		}
		else {
			cout << "\033[31mError opening file!\033[0m" << endl;
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
			cout << "\033[31mError opening file!\033[0m" << endl;
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

class FinananceManager {

public:
	static void addIncome(string description, double amount, string date, string category) {
		Income income(description, amount, date, category);
		income.saveToFile();
        // Added: Save to transactions.txt as well
        Filemanager::saveTransaction(income);
	}
	static void addExpense(string description, double amount, string date, string category) {
		Expense expense(description, amount, date, category);
		expense.saveToFile();
        // Added: Save to transactions.txt as well
        Filemanager::saveTransaction(expense);
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

	static void displayInvestments() {
		Filemanager::viewInvestments();
	}

	static void viewAllTransactions() {
		Filemanager::displayTransactions();
	}
	static void deleteTransaction(string description) {
		// Delete from transactions.txt
        Filemanager::deleteTransaction(description);
        
        // Also need to delete from income.txt and expense.txt
        // Delete from income.txt
        vector<Income> incomes = Income::readFromFile();
        ofstream incomeFile("income.txt");
        if (incomeFile.is_open()) {
            for (const auto& income : incomes) {
                if (income.getDescription() != description) {
                    incomeFile << income.getDescription() << "," << income.getAmount() << ","
                            << income.getDate() << "," << income.getCategory() << endl;
                }
            }
            incomeFile.close();
        } else {
            cout << "\033[31mError opening income file!\033[0m" << endl;
        }
        
        // Delete from expense.txt
        vector<Expense> expenses = Expense::readFromFile();
        ofstream expenseFile("expense.txt");
        if (expenseFile.is_open()) {
            for (const auto& expense : expenses) {
                if (expense.getDescription() != description) {
                    expenseFile << expense.getDescription() << "," << expense.getAmount() << ","
                             << expense.getDate() << "," << expense.getCategory() << endl;
                }
            }
            expenseFile.close();
        } else {
            cout << "\033[31mError opening expense file!\033[0m" << endl;
        }
	}
	static void suggestInvestmentsAndOptions() {
		double totalIncome = Income::calculateTotal();
		double totalExpenses = Expense::calculateTotal();
		double balance = totalIncome - totalExpenses;

		if (balance <= 0) {
			cout << "\033[31m\nYou currently don't have a positive balance for investment in your accounts.\n\033[0m";
			return;
		}

		cout << "\n=== Investment Suggestions ===\n";
		cout << "\033[32mYour Total Income: \033[0m" << fixed << setprecision(2) << totalIncome << "\033[32m$\033[0m" << endl;
		cout << "\033[31mYour Total Expenses: \033[0m" << fixed << setprecision(2) << totalExpenses << "\033[31m$\033[0m" << endl;
		cout << "\033[36mYour Available Balance: \033[0m" << fixed << setprecision(2) << balance << "\033[36m$\033[0m" << endl;

		if (balance < 10000) {
			cout << "Emergency Fund: $" << fixed << setprecision(2) << (balance * 0.8) << endl;
			cout << "Low-risk Investment: $" << fixed << setprecision(2) << (balance * 0.2) << endl;
		}

		else {
			cout << "Analyzing your balance. Please wait...\n";
			this_thread::sleep_for(chrono::seconds(4)); // 4 seconds delay
			cout << "1. High-risk Investment: $ -> (36%) " << fixed << setprecision(2) << endl;
			this_thread::sleep_for(chrono::milliseconds(400));
			cout << "2. Low-risk Investment: $ -> (12%) " << fixed << setprecision(2) << endl;
			cout << "3. Medium-risk Investment: $ -> (24%) " << fixed << setprecision(2) << endl;
			cout << "4. Cryptocurrency Investment: $ -> (9%) " << fixed << setprecision(2) << endl;
			cout << "5. Diversified Portfolio: $ -> (3%) " << fixed << setprecision(2) << endl;
			cout << "6. Savings Account: $ -> (6%) " << fixed << setprecision(2) << endl;
			cout << "7. Retirement Account: $ -> (3%) " << fixed << setprecision(2) << endl;
			cout << "8. Emergency Fund: $ -> (3.6%) " << fixed << setprecision(2) << endl;
			this_thread::sleep_for(chrono::milliseconds(500));
			cout << "9. Investment in Education: $ -> (1.2%) " << fixed << setprecision(2) << endl;
			cout << "10. Investment in Health: $ -> (2%) " << fixed << setprecision(2) << endl;
			cout << "11. Real Estate Investment: $ -> (18%) " << fixed << setprecision(2) << endl;
			cout << "12. Bonds: $ -> (9%) " << fixed << setprecision(2) << endl;
			cout << "13. Mutual Funds: $ -> (9%) " << fixed << setprecision(2) << endl;
			cout << "14. Stock Market Investment: $ -> (24%) " << fixed << setprecision(2) << endl;
			cout << "15. Precious Metals (Gold/Silver): $ -> (12%) " << fixed << setprecision(2) << endl;
			this_thread::sleep_for(chrono::milliseconds(500));
			cout << "16. Peer-to-Peer Lending: $ -> (18%) " << fixed << setprecision(2) << endl;
			cout << "17. Hedge Funds: $ -> (24%) " << fixed << setprecision(2) << endl;
			cout << "18. Venture Capital: $ -> (36%) " << fixed << setprecision(2) << endl;
			cout << "19. Art and Collectibles: $ -> (8.4%) " << fixed << setprecision(2) << endl;
			cout << "20. Carbon Credit Investment: $ -> (6%) " << fixed << setprecision(2) << endl;
		}

		int x, m;
		double bal;
		string date;  // Variable to hold the investment date

		cout << "Choose What Is the Most Suitable case For You\n";
		cin >> x;
		cout << "Please Enter The Amount You Want To Invest\n";
		cin >> bal;
		cout << "Please Enter The Duration In Months\n";
		cin >> m;
		cout << "Please Enter the Investment Date (YYYY-MM-DD): ";
		cin	>> date;  // Collect the investment date

		// Save the investment to the file
		Filemanager::saveInvestment(x, m, bal, date);


		cout << "You Have Chosen Investment Option " << x << " with an amount of $" << fixed << setprecision(2) << bal << " for a duration of " << m << " months." << endl;
		this_thread::sleep_for(chrono::seconds(1)); // 1 seconds delay
		cout << "Your investment will be monitored and adjusted based on market conditions." << endl;
		this_thread::sleep_for(chrono::milliseconds(800)); // 1 seconds delay
		cout << "Please wait while we process your investment...\n";
		this_thread::sleep_for(chrono::seconds(2)); // 2 seconds delay
		cout << "Investment processed successfully!" << endl;

		

		switch (x)
		{
		case 1: {
			double highRiskROI = 0.03;  // 3% monthly ROI
			double highRiskProfit = bal * highRiskROI * m;
			cout << "High-risk Investment: $" << fixed << setprecision(2) << bal << endl;
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

class screen {
public:
	static void clear() {
		system("cls");
	}
};

class UserInterface {
public:
	static void displayMenu() {
		cout << "=================================" << endl;
		cout << "\033[92m	Byte Masters!\033[0m" << endl;
		cout << "\033[32mWelcome to the Finance Manager!\033[0m" << endl;
		cout << "=================================" << endl;
		cout << "\033[36m1. Add Income\033[0m" << endl;
		cout << "\033[36m2. Add Expense\033[0m" << endl;
		cout << "\033[36m3. View Income\033[0m" << endl;
		cout << "\033[36m4. View Expense\033[0m" << endl;
		cout << "\033[36m5. View All Transactions\033[0m" << endl;
		cout << "\033[36m6. Delete Transaction\033[0m" << endl;
		cout << "\033[36m7. Investment Suggestions\033[0m" << endl;
		cout << "\033[36m8. View Investments\033[0m" << endl;
		cout << "\033[31m9. Exit\033[0m" << endl;
	}

	static void displayLoading() {
		cout << "\033[33mLoading...\033[0m" << endl;
		for (int i = 0; i < 3; i++) {
			cout << "\033[33m.\033[0m";
			this_thread::sleep_for(chrono::milliseconds(500));
		}
		cout << endl;
	}

	static void handleUserChoice(int choice) {
		switch (choice) {
		case 1: {
			string description, date, category;
			double amount;
			cout << "\033[33mEnter income description: \033[0m";
			cin.ignore();
			getline(cin, description);
			cout << "\033[32mEnter income amount: \033[0m";
			cin >> amount;
			cout << "\033[33mEnter income date (YYYY-MM-DD): \033[0m";
			cin >> date;
			cout << "\033[33mEnter income category: \033[0m";
			cin.ignore();
			getline(cin, category);
			FinananceManager::addIncome(description, amount, date, category);
			break;
		}
		case 2: {
			string description, date, category;
			double amount;
			cout << "\033[33mEnter expense description: \033[0m";
			cin.ignore();
			getline(cin, description);
			cout << "\033[32mEnter expense amount: \033[0m";
			cin >> amount;
			cout << "\033[33mEnter expense date (YYYY-MM-DD): \033[0m";
			cin >> date;
			cout << "\033[33mEnter expense category: \033[0m";
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
			cout << "\033[36mEnter the description of the transaction to delete: \033[0m";
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
			screen::clear();
			FinananceManager::displayInvestments();
			system("pause");
			break;
			
		case 9:
			screen::clear();
			cout << "\033[31mExit \033[0m" << endl;
			break;
		default:
			cout << "\033[31mInvalid choice! Please try again.\033[0m" << endl;
			break;
		}
	}
};

int main() {
	int choice;
	do {
		screen::clear();
		UserInterface::displayMenu();
		cout << "\033[32mEnter your choice: \033[0m";
		cin >> choice;
		UserInterface::handleUserChoice(choice);
	} while (choice != 8);
	return 0;
}
