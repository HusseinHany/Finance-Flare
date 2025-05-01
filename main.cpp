#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include <thread>
#include <chrono>
#include <memory>
#include <algorithm>

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
    Expense(string desc, double amt) : description(move(desc)), amount(amt) {}

    void display() const override {
        cout << "\033[1;31mExpense:\033[0m " << description << " | Amount: $" << amount << endl;
    }

    string getDetails() const override {
        ostringstream oss;
        oss << "Expense," << description << "," << amount;
        return oss.str();
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
    Income(string src, double amt) : source(move(src)), amount(amt) {}

    void display() const override {
        cout << "\033[1;32mIncome:\033[0m " << source << " | Amount: $" << amount << endl;
    }

    string getDetails() const override {
        ostringstream oss;
        oss << "Income," << source << "," << amount;
        return oss.str();
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
    Investment(string t, double amt) : type(move(t)), recommendedAmount(amt) {}

    void display() const {
        cout << "\033[1;34mRecommended Investment:\033[0m " << type << " | Suggested Amount: $" << recommendedAmount << endl;
    }
};

class FinanceManager {
private:
    vector<unique_ptr<Record>> records;
    vector<Investment> investments;
    double totalIncome = 0.0;
    double totalExpenses = 0.0;

public:
    void addRecord(unique_ptr<Record> record, bool isIncome) {
        if (isIncome) totalIncome += record->getAmount();
        else totalExpenses += record->getAmount();
        records.push_back(move(record));
    }

    void displayRecords() const {
        cout << "\n\033[1;36m--- Financial Records ---\033[0m\n";
        for (const auto& record : records) {
            record->display();
        }
        cout << "\n\033[1;32mTotal Income: $" << totalIncome << "\033[0m\n";
        cout << "\033[1;31mTotal Expenses: $" << totalExpenses << "\033[0m\n";
        cout << "\033[1;33mBalance: $" << (totalIncome - totalExpenses) << "\033[0m\n";
    }

    void suggestInvestments(const string& riskLevel) {
        investments.clear();
        cout << "\n\033[1;35m--- Suggested Investments (" << riskLevel << ") ---\033[0m\n";

        if (riskLevel == "conservative") {
            investments.emplace_back("Bonds (U.S. Treasuries)", 3000);
            investments.emplace_back("REITs (Real Estate)", 2000);
            investments.emplace_back("Gold", 1000);
        } else if (riskLevel == "balanced") {
            investments.emplace_back("Tech Stocks (S&P 500)", 4000);
            investments.emplace_back("REITs (Real Estate)", 2000);
            investments.emplace_back("Bonds (Corporate)", 2000);
            investments.emplace_back("Cryptocurrencies", 1000);
        } else if (riskLevel == "aggressive") {
            investments.emplace_back("Growth Stocks (Tech)", 5000);
            investments.emplace_back("Cryptocurrencies", 3000);
            investments.emplace_back("REITs", 1000);
            investments.emplace_back("Commodities", 1000);
        } else {
            cout << "\033[1;31mInvalid risk level entered.\033[0m\n";
            return;
        }

        for (const auto& inv : investments) inv.display();
    }

    void saveData() const {
        ofstream out("financial_records.txt");
        if (!out) {
            cerr << "\033[1;31mError opening file for saving data.\033[0m\n";
            return;
        }
        for (const auto& record : records) {
            out << record->getDetails() << endl;
        }
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
        if (!file) {
            cerr << "\033[1;31mUnable to open file for loading data. Make sure 'financial_records.txt' exists.\033[0m\n";
            return;
        }

        records.clear();
        totalIncome = 0;
        totalExpenses = 0;

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string type, desc;
            double amt;

            getline(ss, type, ',');
            getline(ss, desc, ',');
            ss >> amt;

            if (type == "Expense") addRecord(make_unique<Expense>(desc, amt), false);
            else if (type == "Income") addRecord(make_unique<Income>(desc, amt), true);
        }

        cout << "\033[1;32mData loaded and analyzed successfully!\033[0m\n";
    }
};

class Screen {
private:
    FinanceManager& manager;

    static void ignoreInput() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    template<typename T>
    T readNumber(const string& prompt) {
        T num;
        while (true) {
            cout << prompt;
            if (cin >> num) {
                if (num >= 0)
                    break;
                else
                    cout << "\033[1;31mPlease enter a non-negative number.\033[0m\n";
            } else {
                cout << "\033[1;31mInvalid input. Please enter a number.\033[0m\n";
                cin.clear();
            }
            ignoreInput();
        }
        ignoreInput();
        return num;
    }

    string readLine(const string& prompt) {
        cout << prompt;
        string line;
        getline(cin, line);
        return line;
    }

    string readRiskLevel() {
        string risk;
        while (true) {
            cout << "Enter risk level (conservative / balanced / aggressive): ";
            getline(cin, risk);
            transform(risk.begin(), risk.end(), risk.begin(), ::tolower);
            if (risk == "conservative" || risk == "balanced" || risk == "aggressive") {
                break;
            } else {
                cout << "\033[1;31mInvalid risk level. Please try again.\033[0m\n";
            }
        }
        return risk;
    }

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
        string desc = readLine("Enter expense description: ");
        double amt = readNumber<double>("Enter expense amount: $");
        manager.addRecord(make_unique<Expense>(desc, amt), false);
        cout << "\033[1;32mExpense added successfully.\033[0m\n";
    }

    void addIncome() {
        string src = readLine("Enter income source: ");
        double amt = readNumber<double>("Enter income amount: $");
        manager.addRecord(make_unique<Income>(src, amt), true);
        cout << "\033[1;32mIncome added successfully.\033[0m\n";
    }

    void viewRecords() const {
        manager.displayRecords();
    }

    void suggestInvestments() {
        string level = readRiskLevel();
        manager.suggestInvestments(level);
    }

    void saveData() const {
        manager.saveData();
    }

    void loadData() {
        manager.loadData();
    }

    void run() {
        int choice;
        bool running = true;

        while (running) {
            displayMenu();
            if (!(cin >> choice)) {
                cout << "\033[1;31mInvalid input. Please enter a number between 1 and 7.\033[0m\n";
                cin.clear();
                ignoreInput();
                continue;
            }
            ignoreInput();
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
