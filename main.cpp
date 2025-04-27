#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

class Record {
public:
    virtual void display() const = 0;  // Pure virtual function for displaying records
    virtual string getDetails() const = 0;
};

class Expense : public Record {
private:
    string description;
    double amount;
public:
    Expense(string desc, double amt) : description(desc), amount(amt) {}

    void display() const override {
        cout << "Expense: " << description << " | Amount: $" << amount << endl;
    }

    string getDetails() const override {
        return "Expense: " + description + " | Amount: " + to_string(amount);
    }

    string getDescription() const { return description; }
    double getAmount() const { return amount; }
};

class Income : public Record {
private:
    string source;
    double amount;
public:
    Income(string src, double amt) : source(src), amount(amt) {}

    void display() const override {
        cout << "Income: " << source << " | Amount: $" << amount << endl;
    }

    string getDetails() const override {
        return "Income: " + source + " | Amount: " + to_string(amount);
    }

    string getSource() const { return source; }
    double getAmount() const { return amount; }
};

class Investment {
private:
    string type;
    double recommendedAmount;
public:
    Investment(string t, double amt) : type(t), recommendedAmount(amt) {}

    void display() const {
        cout << "Recommended Investment: " << type << " | Suggested Amount: $" << recommendedAmount << endl;
    }
};

class FinanceManager {
private:
    vector<Record*> records;  // Array of pointers to Record objects
    vector<Investment> investments;
    double totalIncome = 0.0;
    double totalExpenses = 0.0;

public:
    ~FinanceManager() {
        for (auto record : records) {
            delete record;
        }
    }

    void addRecord(Expense* expense) {
        records.push_back(expense);
        totalExpenses += expense->getAmount();
    }

    void addRecord(Income* income) {
        records.push_back(income);
        totalIncome += income->getAmount();
    }

    void displayRecords() const {
        for (const auto& record : records) {
            record->display();
        }
        cout << "Total Income: $" << totalIncome << endl;
        cout << "Total Expenses: $" << totalExpenses << endl;
        cout << "Balance: $" << totalIncome - totalExpenses << endl;
    }

    void suggestInvestments(const string& riskLevel) {
        investments.clear();  // Clear previous suggestions

        if (riskLevel == "conservative") {
            investments.push_back(Investment("Bonds (U.S. Treasuries)", 3000));
            investments.push_back(Investment("REITs (Real Estate)", 2000));
            investments.push_back(Investment("Gold", 1000));
        } else if (riskLevel == "balanced") {
            investments.push_back(Investment("Tech Stocks (S&P 500)", 4000));
            investments.push_back(Investment("REITs (Real Estate)", 2000));
            investments.push_back(Investment("Bonds (Corporate)", 2000));
            investments.push_back(Investment("Cryptocurrencies (Bitcoin, Ethereum)", 1000));
        } else if (riskLevel == "aggressive") {
            investments.push_back(Investment("Growth Stocks (Tech, Biotech)", 5000));
            investments.push_back(Investment("Cryptocurrencies (Bitcoin, Ethereum)", 3000));
            investments.push_back(Investment("REITs (Real Estate)", 1000));
            investments.push_back(Investment("Commodities (Oil, Precious Metals)", 1000));
        }

        for (const auto& investment : investments) {
            investment.display();
        }
    }

    void saveData() const {
        ofstream outFile("financial_records.txt");

        for (const auto& record : records) {
            outFile << record->getDetails() << endl;
        }
        outFile.close();
    }

    void loadData() {
        ifstream inFile("financial_records.txt");
        string line;
        while (getline(inFile, line)) {
            cout << line << endl;
        }
        inFile.close();
    }
};

class Screen {
private:
    FinanceManager& manager;

public:
    Screen(FinanceManager& mgr) : manager(mgr) {}

    void displayMenu() const {
        cout << "\n--- Finance-Flare ---" << endl;
        cout << "1. Add Expense" << endl;
        cout << "2. Add Income" << endl;
        cout << "3. View All Records" << endl;
        cout << "4. Suggest Investments" << endl;
        cout << "5. Save Data" << endl;
        cout << "6. Load Data" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice: ";
    }

    void addExpense() {
        string description;
        double amount;
        cout << "Enter expense description: ";
        cin.ignore();  // To clear the input buffer
        getline(cin, description);
        cout << "Enter expense amount: $";
        cin >> amount;

        Expense* expense = new Expense(description, amount);
        manager.addRecord(expense);
    }

    void addIncome() {
        string source;
        double amount;
        cout << "Enter income source: ";
        cin.ignore();  // To clear the input buffer
        getline(cin, source);
        cout << "Enter income amount: $";
        cin >> amount;

        Income* income = new Income(source, amount);
        manager.addRecord(income);
    }

    void viewRecords() {
        manager.displayRecords();
    }

    void suggestInvestments() {
        string riskLevel;
        cout << "Enter your risk level (conservative, balanced, aggressive): ";
        cin >> riskLevel;
        manager.suggestInvestments(riskLevel);
    }

    void saveData() {
        manager.saveData();
        cout << "Data saved successfully!" << endl;
    }

    void loadData() {
        cout << "\nLoaded Data from File:\n";
        manager.loadData();
    }

    void run() {
        int choice;
        bool running = true;

        while (running) {
            displayMenu();
            cin >> choice;

            switch (choice) {
                case 1:
                    addExpense();
                    break;
                case 2:
                    addIncome();
                    break;
                case 3:
                    viewRecords();
                    break;
                case 4:
                    suggestInvestments();
                    break;
                case 5:
                    saveData();
                    break;
                case 6:
                    loadData();
                    break;
                case 7:
                    running = false;
                    cout << "Exiting application." << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                    break;
            }
        }
    }
};

int main() {
    FinanceManager manager;
    Screen screen(manager);

    screen.run();

    return 0;
}

