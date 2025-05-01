#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include <thread>
#include <chrono>
using namespace std;

class Record {
public:
    virtual void display() const = 0;
    virtual string getDetails() const = 0;
    virtual string getType() const = 0;
    virtual string getDesc() const = 0;
    virtual double getAmount() const = 0;
    virtual ~Record() {}
};

class Expense : public Record {
private:
    string description;
    double amount;
public:
    Expense(string desc, double amt) : description(desc), amount(amt) {}

    void display() const override {
        cout << "\033[1;31mExpense:\033[0m " << description << " | Amount: $" << amount << endl;
    }

    string getDetails() const override {
        return "Expense," + description + "," + to_string(amount);
    }

    string getType() const override { return "Expense"; }
    string getDesc() const override { return description; }
    double getAmount() const override { return amount; }
};

class Income : public Record {
private:
    string source;
    double amount;
public:
    Income(string src, double amt) : source(src), amount(amt) {}

    void display() const override {
        cout << "\033[1;32mIncome:\033[0m " << source << " | Amount: $" << amount << endl;
    }

    string getDetails() const override {
        return "Income," + source + "," + to_string(amount);
    }

    string getType() const override { return "Income"; }
    string getDesc() const override { return source; }
    double getAmount() const override { return amount; }
};

class Investment {
private:
    string type;
    double recommendedAmount;
public:
    Investment(string t, double amt) : type(t), recommendedAmount(amt) {}

    void display() const {
        cout << "\033[1;34mRecommended Investment:\033[0m " << type << " | Suggested Amount: $" << recommendedAmount << endl;
    }
};

class FinanceManager {
private:
    vector<Record*> records;
    vector<Investment> investments;
    double totalIncome = 0.0;
    double totalExpenses = 0.0;

public:
    ~FinanceManager() {
        for (auto record : records) delete record;
    }

    void addRecord(Record* record, bool isIncome) {
        records.push_back(record);
        if (isIncome) totalIncome += record->getAmount();
        else totalExpenses += record->getAmount();
    }

    void displayRecords() const {
        cout << "\n\033[1;36m--- Financial Records ---\033[0m\n";
        for (const auto& record : records) record->display();
        cout << "\n\033[1;32mTotal Income: $" << totalIncome << "\033[0m\n";
        cout << "\033[1;31mTotal Expenses: $" << totalExpenses << "\033[0m\n";
        cout << "\033[1;33mBalance: $" << (totalIncome - totalExpenses) << "\033[0m\n";
    }

    void suggestInvestments(const string& riskLevel) {
        investments.clear();
        cout << "\n\033[1;35m--- Suggested Investments (" << riskLevel << ") ---\033[0m\n";

        if (riskLevel == "conservative") {
            investments.push_back(Investment("Bonds (U.S. Treasuries)", 3000));
            investments.push_back(Investment("REITs (Real Estate)", 2000));
            investments.push_back(Investment("Gold", 1000));
        } else if (riskLevel == "balanced") {
            investments.push_back(Investment("Tech Stocks (S&P 500)", 4000));
            investments.push_back(Investment("REITs (Real Estate)", 2000));
            investments.push_back(Investment("Bonds (Corporate)", 2000));
            investments.push_back(Investment("Cryptocurrencies", 1000));
        } else if (riskLevel == "aggressive") {
            investments.push_back(Investment("Growth Stocks (Tech)", 5000));
            investments.push_back(Investment("Cryptocurrencies", 3000));
            investments.push_back(Investment("REITs", 1000));
            investments.push_back(Investment("Commodities", 1000));
        }

        for (const auto& inv : investments) inv.display();
    }

    void saveData() const {
        ofstream out("financial_records.txt");
        for (const auto& record : records) {
            out << record->getDetails() << endl;
        }
        out.close();
        cout << "\n\033[1;32mData saved successfully!\033[0m\n";
    }

    void loadData() {
        cout << "\n\033[1;33mLoading financial data...\033[0m\n";
        for (int i = 0; i < 3; ++i) {
            cout << ".";
            cout.flush();
            this_thread::sleep_for(chrono::milliseconds(500));
        }

        cout << "\n\033[1;34mAnalyzing records...\033[0m\n";
        this_thread::sleep_for(chrono::milliseconds(1000));

        ifstream file("financial_records.txt");
        string line;

        for (auto r : records) delete r;
        records.clear();
        totalIncome = totalExpenses = 0;

        while (getline(file, line)) {
            stringstream ss(line);
            string type, desc;
            double amt;

            getline(ss, type, ',');
            getline(ss, desc, ',');
            ss >> amt;

            if (type == "Expense") addRecord(new Expense(desc, amt), false);
            else if (type == "Income") addRecord(new Income(desc, amt), true);
        }

        file.close();
        cout << "\033[1;32mData loaded and analyzed successfully!\033[0m\n";
    }
};

class Screen {
private:
    FinanceManager& manager;

public:
    Screen(FinanceManager& mgr) : manager(mgr) {}

    void displayMenu() const {
        cout << "\n\033[1;36m======= Finance-Flare Menu =======\033[0m\n";
        cout << "1. Add Expense\n";
        cout << "2. Add Income\n";
        cout << "3. View All Records\n";
        cout << "4. Suggest Investments\n";
        cout << "5. Save Data to File\n";
        cout << "6. Load Data from File\n";
        cout << "7. Exit\n";
        cout << "\033[1;36m==================================\033[0m\n";
        cout << "Enter your choice: ";
    }

    void addExpense() {
        string desc;
        double amt;
        cout << "Enter expense description: ";
        cin.ignore();
        getline(cin, desc);
        cout << "Enter expense amount: $";
        cin >> amt;
        manager.addRecord(new Expense(desc, amt), false);
    }

    void addIncome() {
        string src;
        double amt;
        cout << "Enter income source: ";
        cin.ignore();
        getline(cin, src);
        cout << "Enter income amount: $";
        cin >> amt;
        manager.addRecord(new Income(src, amt), true);
    }

    void viewRecords() { manager.displayRecords(); }

    void suggestInvestments() {
        string level;
        cout << "Enter risk level (conservative / balanced / aggressive): ";
        cin >> level;
        manager.suggestInvestments(level);
    }

    void saveData() { manager.saveData(); }

    void loadData() { manager.loadData(); }

    void run() {
        int choice;
        bool running = true;

        while (running) {
            displayMenu();
            cin >> choice;
            switch (choice) {
                case 1: addExpense(); break;
                case 2: addIncome(); break;
                case 3: viewRecords(); break;
                case 4: suggestInvestments(); break;
                case 5: saveData(); break;
                case 6: loadData(); break;
                case 7:
                    running = false;
                    cout << "\n\033[1;35mThank you for using Finance-Flare!\033[0m\n";
                    break;
                default:
                    cout << "\033[1;31mInvalid choice. Try again.\033[0m\n";
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
