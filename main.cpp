#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <ctime>

using namespace std;

// Date class for transaction dates
class Date {
private:
    int day;
    int month;
    int year;

public:
    // Default constructor uses current date
    Date() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        day = ltm->tm_mday;
        month = 1 + ltm->tm_mon;
        year = 1900 + ltm->tm_year;
    }

    // Overloaded constructor - function overloading example
    Date(int d, int m, int y) : day(d), month(m), year(y) {}

    // Overloaded constructor from string - function overloading example
    Date(const string& dateStr) {
        try {
            size_t pos1 = dateStr.find('/');
            size_t pos2 = dateStr.find('/', pos1 + 1);
            
            day = stoi(dateStr.substr(0, pos1));
            month = stoi(dateStr.substr(pos1 + 1, pos2 - pos1 - 1));
            year = stoi(dateStr.substr(pos2 + 1));
        } catch (...) {
            // Default to current date on error
            *this = Date();
        }
    }

    string toString() const {
        return to_string(day) + "/" + to_string(month) + "/" + to_string(year);
    }
};

// Record types
enum class RecordType {
    INCOME,
    EXPENSE
};

// Base class for transactions (inheritance)
class Transaction {
protected:
    string description;
    double amount;
    string category;
    Date transactionDate;

public:
    // Constructor with default date - function overloading example
    Transaction(string desc, double amt, string cat)
        : description(desc), amount(amt), category(cat), transactionDate() {}

    // Overloaded constructor with custom date - function overloading example
    Transaction(string desc, double amt, string cat, const Date& date)
        : description(desc), amount(amt), category(cat), transactionDate(date) {}

    virtual void display() const = 0; // Pure virtual function for polymorphism

    // Overloaded display with formatting option - function overloading example
    virtual void display(bool detailed) const = 0;

    virtual string getFileFormat() const = 0; // Pure virtual for file saving

    // Getters
    string getDescription() const { return description; }
    double getAmount() const { return amount; }
    string getCategory() const { return category; }
    Date getDate() const { return transactionDate; }

    virtual ~Transaction() = default; // Virtual destructor for proper cleanup
};

// Derived class for records
class Record : public Transaction {
private:
    RecordType type;
    int id;
    static int nextId; // Static member to track IDs

public:
    // Constructor with default date - function overloading example
    Record(RecordType t, string desc, double amt, string cat)
        : Transaction(desc, amt, cat), type(t) {
        id = nextId++;
    }

    // Overloaded constructor with custom date - function overloading example
    Record(RecordType t, string desc, double amt, string cat, const Date& date)
        : Transaction(desc, amt, cat, date), type(t) {
        id = nextId++;
    }

    // Display method implementation
    void display() const override {
        display(false); // Default to non-detailed display
    }

    // Overloaded display with formatting option - function overloading example
    void display(bool detailed) const override {
        string typeStr = (type == RecordType::INCOME) ? "INCOME" : "EXPENSE";
        
        cout << "[" << id << "] " << typeStr << ": " << description 
             << " | $" << fixed << setprecision(2) << amount 
             << " | Category: " << category;
        
        if (detailed) {
            cout << " | Date: " << transactionDate.toString();
        }
        cout << endl;
    }

    string getFileFormat() const override {
        string typeStr = (type == RecordType::INCOME) ? "Income" : "Expense";
        return to_string(id) + "," + typeStr + "," + description + "," + 
               to_string(amount) + "," + category + "," + transactionDate.toString();
    }

    static Record fromFile(const string& line) {
        vector<string> parts;
        size_t start = 0, end = 0;
        
        // Split the line by commas
        while ((end = line.find(',', start)) != string::npos) {
            parts.push_back(line.substr(start, end - start));
            start = end + 1;
        }
        parts.push_back(line.substr(start)); // Last part
        
        if (parts.size() < 6) return Record(RecordType::EXPENSE, "", 0, "");
        
        int recordId = stoi(parts[0]);
        string typeStr = parts[1];
        string desc = parts[2];
        double amt = stod(parts[3]);
        string cat = parts[4];
        Date date(parts[5]);
        
        RecordType type = (typeStr == "Income") ? RecordType::INCOME : RecordType::EXPENSE;
        
        // Set the next ID if this one is higher
        if (recordId >= nextId) {
            nextId = recordId + 1;
        }
        
        return Record(type, desc, amt, cat, date);
    }

    bool isIncome() const { return type == RecordType::INCOME; }
    int getId() const { return id; }
};

// Initialize static member
int Record::nextId = 1;

// Budget class for budget management
class Budget {
private:
    string category;
    double limit;
    double spent;

public:
    // Constructor - function overloading example
    Budget(string cat, double lim) : category(cat), limit(lim), spent(0.0) {}
    
    // Overloaded constructor with initial spent amount - function overloading example
    Budget(string cat, double lim, double initialSpent) 
        : category(cat), limit(lim), spent(initialSpent) {}

    void addExpense(double amount) {
        spent += amount;
    }

    // Display budget info
    void display() const {
        double remaining = limit - spent;
        double percentUsed = (spent / limit) * 100;
        
        cout << "Budget: " << category 
             << " | Limit: $" << fixed << setprecision(2) << limit
             << " | Spent: $" << spent
             << " | Remaining: $" << remaining
             << " | " << setprecision(1) << percentUsed << "% used" << endl;
    }

    string getCategory() const { return category; }
    double getLimit() const { return limit; }
    double getSpent() const { return spent; }
    double getRemaining() const { return limit - spent; }

    string getFileFormat() const {
        return category + "," + to_string(limit) + "," + to_string(spent);
    }

    static Budget fromFile(const string& line) {
        vector<string> parts;
        size_t start = 0, end = 0;
        
        // Split the line by commas
        while ((end = line.find(',', start)) != string::npos) {
            parts.push_back(line.substr(start, end - start));
            start = end + 1;
        }
        parts.push_back(line.substr(start)); // Last part
        
        if (parts.size() < 3) return Budget("Other", 0.0);
        
        string cat = parts[0];
        double lim = stod(parts[1]);
        double spent = stod(parts[2]);
        
        return Budget(cat, lim, spent);
    }
};

// Finance manager class to handle all operations
class FinanceManager {
private:
    vector<Record> records;
    vector<Budget> budgets;
    double totalIncome = 0.0;
    double totalExpenses = 0.0;

    // Common income categories
    vector<string> incomeCategories = {
        "Salary", "Business", "Investment", "Gift", "Other"
    };

    // Common expense categories
    vector<string> expenseCategories = {
        "Food", "Housing", "Transportation", "Entertainment", "Shopping", 
        "Utilities", "Healthcare", "Education", "Personal", "Other"
    };

public:
    FinanceManager() {
        // Initialize default budgets
        for (const auto& cat : expenseCategories) {
            budgets.push_back(Budget(cat, 1000.0)); // Default limit
        }
    }

    // Add record - function overloading example
    void addRecord(const Record& record) {
        records.push_back(record);
        
        if (record.isIncome()) {
            totalIncome += record.getAmount();
        } else {
            totalExpenses += record.getAmount();
            // Update corresponding budget
            updateBudget(record.getCategory(), record.getAmount());
        }
    }

    // Overloaded search method - function overloading example
    void searchRecords(const string& keyword) const {
        cout << "\n=== Search Results for \"" << keyword << "\" ===\n";
        
        bool found = false;
        for (const auto& record : records) {
            if (record.getDescription().find(keyword) != string::npos || 
                record.getCategory().find(keyword) != string::npos) {
                record.display(true);
                found = true;
            }
        }
        
        if (!found) {
            cout << "No matching records found.\n";
        }
    }

    // Overloaded search method by date - function overloading example
    void searchRecords(const Date& date) const {
        cout << "\n=== Search Results for date " << date.toString() << " ===\n";
        
        bool found = false;
        for (const auto& record : records) {
            if (record.getDate().toString() == date.toString()) {
                record.display(true);
                found = true;
            }
        }
        
        if (!found) {
            cout << "No matching records found.\n";
        }
    }

    // Overloaded search method by ID - function overloading example
    void searchRecords(int id) const {
        cout << "\n=== Search Results for ID " << id << " ===\n";
        
        bool found = false;
        for (const auto& record : records) {
            if (record.getId() == id) {
                record.display(true);
                found = true;
                break;
            }
        }
        
        if (!found) {
            cout << "No record with ID " << id << " found.\n";
        }
    }

    // Update budget when expense is added
    void updateBudget(const string& category, double amount) {
        for (auto& budget : budgets) {
            if (budget.getCategory() == category) {
                budget.addExpense(amount);
                return;
            }
        }
    }

    // Display all records
    void displayRecords() const {
        cout << "\n=== Financial Records ===\n";
        
        if (records.empty()) {
            cout << "No records found.\n";
            return;
        }

        for (const auto& record : records) {
            record.display(true); // Display with details
        }
        
        cout << "\nTotal Income: $" << fixed << setprecision(2) << totalIncome << endl;
        cout << "Total Expenses: $" << fixed << setprecision(2) << totalExpenses << endl;
        cout << "Balance: $" << fixed << setprecision(2) << (totalIncome - totalExpenses) << endl;
    }

    // Display budgets
    void displayBudgets() const {
        cout << "\n=== Budget Status ===\n";
        
        if (budgets.empty()) {
            cout << "No budgets set up.\n";
            return;
        }

        for (const auto& budget : budgets) {
            budget.display();
        }
    }

    // Set budget for category
    void setBudget(const string& category, double limit) {
        for (auto& budget : budgets) {
            if (budget.getCategory() == category) {
                double spent = budget.getSpent();
                budgets.erase(remove_if(budgets.begin(), budgets.end(),
                    [&category](const Budget& b) { return b.getCategory() == category; }), 
                    budgets.end());
                budgets.push_back(Budget(category, limit, spent));
                return;
            }
        }
        
        // If we get here, category wasn't found
        budgets.push_back(Budget(category, limit));
    }

    void suggestInvestments() const {
        double balance = totalIncome - totalExpenses;
        
        if (balance <= 0) {
            cout << "\nYou currently don't have a positive balance for investment.\n";
            return;
        }
        
        cout << "\n=== Investment Suggestions ===\n";
        
        // Basic investment suggestions
        if (balance < 1000) {
            cout << "Emergency Fund: $" << fixed << setprecision(2) << (balance * 0.8) << endl;
            cout << "Low-risk Investment: $" << fixed << setprecision(2) << (balance * 0.2) << endl;
        } else {
            cout << "Emergency Fund: $" << fixed << setprecision(2) << (balance * 0.5) << endl;
            cout << "Low-risk Investment: $" << fixed << setprecision(2) << (balance * 0.3) << endl;
            cout << "Medium-risk Investment: $" << fixed << setprecision(2) << (balance * 0.2) << endl;
        }
    }

    // Save all data to files
    void saveData() const {
        // Save records
        ofstream recordFile("finance_records.txt");
        if (!recordFile) {
            cout << "Error: Could not open file for saving records.\n";
            return;
        }
        
        for (const auto& record : records) {
            recordFile << record.getFileFormat() << endl;
        }
        recordFile.close();
        
        // Save budgets
        ofstream budgetFile("finance_budgets.txt");
        if (!budgetFile) {
            cout << "Error: Could not open file for saving budgets.\n";
            return;
        }
        
        for (const auto& budget : budgets) {
            budgetFile << budget.getFileFormat() << endl;
        }
        budgetFile.close();
        
        cout << "Data saved successfully!\n";
    }

    // Load all data from files
    void loadData() {
        // Load records
        ifstream recordFile("finance_records.txt");
        if (recordFile) {
            records.clear();
            totalIncome = totalExpenses = 0.0;
            
            string line;
            while (getline(recordFile, line)) {
                Record record = Record::fromFile(line);
                addRecord(record);
            }
            recordFile.close();
        } else {
            cout << "No existing records file found.\n";
        }
        
        // Load budgets
        ifstream budgetFile("finance_budgets.txt");
        if (budgetFile) {
            budgets.clear();
            
            string line;
            while (getline(budgetFile, line)) {
                Budget budget = Budget::fromFile(line);
                budgets.push_back(budget);
            }
            budgetFile.close();
        } else {
            cout << "No existing budgets file found.\n";
        }
        
        cout << "Data loaded successfully!\n";
    }

    void displaySummary() const {
        cout << "\n=== Financial Summary ===\n";
        cout << "Total Income: $" << fixed << setprecision(2) << totalIncome << endl;
        cout << "Total Expenses: $" << fixed << setprecision(2) << totalExpenses << endl;
        cout << "Current Balance: $" << fixed << setprecision(2) << (totalIncome - totalExpenses) << endl;
        
        if (totalIncome > 0) {
            double savingsRate = ((totalIncome - totalExpenses) / totalIncome) * 100;
            cout << "Savings Rate: " << fixed << setprecision(2) << savingsRate << "%" << endl;
        }
    }

    void displayCategoryBreakdown() const {
        if (records.empty()) {
            cout << "No records to analyze.\n";
            return;
        }
        
        // Count expenses by category
        cout << "\n=== Expense Categories ===\n";
        for (const auto& cat : expenseCategories) {
            double total = 0.0;
            for (const auto& record : records) {
                if (!record.isIncome() && record.getCategory() == cat) {
                    total += record.getAmount();
                }
            }
            
            if (total > 0) {
                double percentage = (total / totalExpenses) * 100;
                cout << cat << ": $" << fixed << setprecision(2) << total 
                     << " (" << fixed << setprecision(1) << percentage << "%)" << endl;
            }
        }
    }

    const vector<string>& getIncomeCategories() const {
        return incomeCategories;
    }

    const vector<string>& getExpenseCategories() const {
        return expenseCategories;
    }
};

// User interface class
class FinanceUI {
private:
    FinanceManager manager;

    void clearScreen() {
        cout << string(50, '\n');
    }

    // Get string input - function overloading example
    string getInput() {
        string input;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, input);
        return input;
    }

    // Get double input with validation - function overloading example
    double getInput(const string& prompt) {
        double value;
        bool valid = false;
        
        while (!valid) {
            cout << prompt;
            if (cin >> value) {
                valid = true;
            } else {
                cout << "Invalid input. Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        
        return value;
    }

    // Get integer input with validation - function overloading example
    int getInput(int min, int max) {
        int value;
        bool valid = false;
        
        while (!valid) {
            if (cin >> value) {
                if (value >= min && value <= max) {
                    valid = true;
                } else {
                    cout << "Please enter a number between " << min << " and " << max << ": ";
                }
            } else {
                cout << "Invalid input. Please enter a whole number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        
        return value;
    }

    void waitForEnter() {
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (cin.peek() == '\n') cin.ignore();
    }

    Date getDateInput() {
        int day, month, year;
        
        cout << "Day (1-31): ";
        day = getInput(1, 31);
        
        cout << "Month (1-12): ";
        month = getInput(1, 12);
        
        cout << "Year (2000-2100): ";
        year = getInput(2000, 2100);
        
        return Date(day, month, year);
    }

public:
    void displayMenu() const {
        cout << "\n=== Finance-Flare Menu ===\n";
        cout << "1. Add Expense\n";
        cout << "2. Add Income\n";
        cout << "3. View Records\n";
        cout << "4. Search Records\n";
        cout << "5. Budget Management\n";
        cout << "6. Financial Summary\n";
        cout << "7. Investment Suggestions\n";
        cout << "8. Save Data\n";
        cout << "9. Load Data\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
    }

    void addExpense() {
        string description;
        double amount;
        string category;
        bool useCustomDate = false;
        Date transactionDate;
        
        cout << "\n=== Add Expense ===\n";
        
        cout << "Description: ";
        description = getInput();
        
        cout << "Amount: $";
        amount = getInput("Enter amount: $");
        
        // Show category options
        cout << "\nCategories:\n";
        const auto& categories = manager.getExpenseCategories();
        for (size_t i = 0; i < categories.size(); i++) {
            cout << i+1 << ". " << categories[i] << endl;
        }
        
        cout << "Select category (1-" << categories.size() << "): ";
        int choice = getInput(1, static_cast<int>(categories.size()));
        category = categories[choice-1];
        
        cout << "Use custom date? (1 = Yes, 0 = No): ";
        useCustomDate = getInput(0, 1);
        
        if (useCustomDate) {
            cout << "Enter date:\n";
            transactionDate = getDateInput();
            Record expense(RecordType::EXPENSE, description, amount, category, transactionDate);
            manager.addRecord(expense);
        } else {
            Record expense(RecordType::EXPENSE, description, amount, category);
            manager.addRecord(expense);
        }
        
        cout << "Expense added successfully!\n";
    }

    void addIncome() {
        string description;
        double amount;
        string category;
        bool useCustomDate = false;
        Date transactionDate;
        
        cout << "\n=== Add Income ===\n";
        
        cout << "Source: ";
        description = getInput();
        
        cout << "Amount: $";
        amount = getInput("Enter amount: $");
        
        // Show category options
        cout << "\nCategories:\n";
        const auto& categories = manager.getIncomeCategories();
        for (size_t i = 0; i < categories.size(); i++) {
            cout << i+1 << ". " << categories[i] << endl;
        }
        
        cout << "Select category (1-" << categories.size() << "): ";
        int choice = getInput(1, static_cast<int>(categories.size()));
        category = categories[choice-1];
        
        cout << "Use custom date? (1 = Yes, 0 = No): ";
        useCustomDate = getInput(0, 1);
        
        if (useCustomDate) {
            cout << "Enter date:\n";
            transactionDate = getDateInput();
            Record income(RecordType::INCOME, description, amount, category, transactionDate);
            manager.addRecord(income);
        } else {
            Record income(RecordType::INCOME, description, amount, category);
            manager.addRecord(income);
        }
        
        cout << "Income added successfully!\n";
    }

    void viewRecords() {
        manager.displayRecords();
    }

    void searchRecords() {
        cout << "\n=== Search Records ===\n";
        cout << "1. Search by keyword\n";
        cout << "2. Search by date\n";
        cout << "3. Search by ID\n";
        cout << "Enter choice: ";
        
        int choice = getInput(1, 3);
        
        switch (choice) {
            case 1: {
                cout << "Enter keyword: ";
                string keyword = getInput();
                manager.searchRecords(keyword);
                break;
            }
            case 2: {
                cout << "Enter date:\n";
                Date date = getDateInput();
                manager.searchRecords(date);
                break;
            }
            case 3: {
                cout << "Enter record ID: ";
                int id = getInput(1, 10000);
                manager.searchRecords(id);
                break;
            }
        }
    }

    void budgetManagement() {
        cout << "\n=== Budget Management ===\n";
        cout << "1. View Budgets\n";
        cout << "2. Set Budget\n";
        cout << "Enter choice: ";
        
        int choice = getInput(1, 2);
        
        switch (choice) {
            case 1:
                manager.displayBudgets();
                break;
            case 2: {
                const auto& categories = manager.getExpenseCategories();
                
                cout << "\nCategories:\n";
                for (size_t i = 0; i < categories.size(); i++) {
                    cout << i+1 << ". " << categories[i] << endl;
                }
                
                cout << "Select category (1-" << categories.size() << "): ";
                int catChoice = getInput(1, static_cast<int>(categories.size()));
                string category = categories[catChoice-1];
                
                cout << "Enter monthly budget limit for " << category << ": $";
                double limit = getInput("Enter budget limit: $");
                
                manager.setBudget(category, limit);
                cout << "Budget set successfully!\n";
                break;
            }
        }
    }

    void financialSummary() {
        manager.displaySummary();
        manager.displayCategoryBreakdown();
    }

    void suggestInvestments() {
        manager.suggestInvestments();
    }

    void saveData() {
        manager.saveData();
    }

    void loadData() {
        manager.loadData();
    }

    void run() {
        int choice;
        bool running = true;

        // Try to load data at startup
        manager.loadData();

        while (running) {
            displayMenu();
            choice = getInput(0, 9);
            
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
                    searchRecords();
                    break;
                case 5:
                    budgetManagement();
                    break;
                case 6:
                    financialSummary();
                    break;
                case 7:
                    suggestInvestments();
                    break;
                case 8:
                    saveData();
                    break;
                case 9:
                    loadData();
                    break;
                case 0:
                    running = false;
                    cout << "\nThank you for using Finance-Flare!\n";
                    break;
            }
            
            if (running) {
                waitForEnter();
            }
        }
    }
};

int main() {
    cout << "Welcome to Finance-Flare - Personal Finance Manager\n";
    
    FinanceUI ui;
    ui.run();
    
    return 0;
}
