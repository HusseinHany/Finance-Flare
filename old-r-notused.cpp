#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

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
    string date; // Simple date as string

public:
    // Constructor - function overloading example #1
    Transaction(string desc, double amt, string cat)
        : description(desc), amount(amt), category(cat) {
        // Default to current date (simplified)
        date = "Current";
    }

    // Overloaded constructor with custom date - function overloading example #2
    Transaction(string desc, double amt, string cat, string d)
        : description(desc), amount(amt), category(cat), date(d) {}

    virtual void display() const = 0; // Pure virtual function for polymorphism

    virtual string getFileFormat() const = 0; // Pure virtual for file saving

    // Getters
    string getDescription() const { return description; }
    double getAmount() const { return amount; }
    string getCategory() const { return category; }
    string getDate() const { return date; }

    virtual ~Transaction() = default; // Virtual destructor for proper cleanup
};

// Derived class for records
class Record : public Transaction {
private:
    RecordType type;
    int id;
    static int nextId; // Static member to track IDs

public:
    // Constructor - function overloading example (inherited from base class)
    Record(RecordType t, string desc, double amt, string cat)
        : Transaction(desc, amt, cat), type(t) {
        id = nextId++;
    }

    // Overloaded constructor with date - function overloading example
    Record(RecordType t, string desc, double amt, string cat, string date)
        : Transaction(desc, amt, cat, date), type(t) {
        id = nextId++;
    }

    void display() const override {
        string typeStr = (type == RecordType::INCOME) ? "INCOME" : "EXPENSE";
        
        cout << "[" << id << "] " << typeStr << ": " << description 
             << " | $" << fixed << setprecision(2) << amount 
             << " | Category: " << category 
             << " | Date: " << date << endl;
    }

    string getFileFormat() const override {
        string typeStr = (type == RecordType::INCOME) ? "Income" : "Expense";
        return to_string(id) + "," + typeStr + "," + description + "," + 
               to_string(amount) + "," + category + "," + date;
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
        string date = parts[5];
        
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

// Finance manager class to handle all operations
class FinanceManager {
private:
    vector<Record> records;
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
    // Add record - basic version
    void addRecord(const Record& record) {
        records.push_back(record);
        
        if (record.isIncome()) {
            totalIncome += record.getAmount();
        } else {
            totalExpenses += record.getAmount();
        }
    }

    // Search records - function overloading example #1
    void searchRecords(const string& keyword) const {
        cout << "\n=== Search Results for \"" << keyword << "\" ===\n";
        
        bool found = false;
        for (const auto& record : records) {
            if (record.getDescription().find(keyword) != string::npos || 
                record.getCategory().find(keyword) != string::npos) {
                record.display();
                found = true;
            }
        }
        
        if (!found) {
            cout << "No matching records found.\n";
        }
    }

    // Search records by ID - function overloading example #2
    void searchRecords(int id) const {
        cout << "\n=== Search Results for ID " << id << " ===\n";
        
        bool found = false;
        for (const auto& record : records) {
            if (record.getId() == id) {
                record.display();
                found = true;
                break;
            }
        }
        
        if (!found) {
            cout << "No record with ID " << id << " found.\n";
        }
    }

    void displayRecords() const {
        cout << "\n=== Financial Records ===\n";
        
        if (records.empty()) {
            cout << "No records found.\n";
            return;
        }

        for (const auto& record : records) {
            record.display();
        }
        
        cout << "\nTotal Income: $" << fixed << setprecision(2) << totalIncome << endl;
        cout << "Total Expenses: $" << fixed << setprecision(2) << totalExpenses << endl;
        cout << "Balance: $" << fixed << setprecision(2) << (totalIncome - totalExpenses) << endl;
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

    void saveData() const {
        ofstream file("finance_data.txt");
        
        if (!file) {
            cout << "Error: Could not open file for saving.\n";
            return;
        }
        
        for (const auto& record : records) {
            file << record.getFileFormat() << endl;
        }
        
        file.close();
        cout << "Data saved successfully!\n";
    }

    void loadData() {
        ifstream file("finance_data.txt");
        
        if (!file) {
            cout << "No existing data file found.\n";
            return;
        }
        
        // Clear existing records
        records.clear();
        totalIncome = totalExpenses = 0.0;
        
        string line;
        while (getline(file, line)) {
            Record record = Record::fromFile(line);
            addRecord(record);
        }
        
        file.close();
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

    // Get string input - function overloading example #1
    string getInput() {
        string input;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, input);
        return input;
    }

    // Get integer input - function overloading example #2
    int getInput(int min, int max) {
        int choice;
        while (true) {
            cin >> choice;
            if (cin.fail() || choice < min || choice > max) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice. Enter a number between " << min << " and " << max << ": ";
            } else {
                break;
            }
        }
        return choice;
    }

    void waitForEnter() {
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (cin.peek() == '\n') cin.ignore();
    }

public:
    void displayMenu() const {
        cout << "\n=== Finance-Flare Menu ===\n";
        cout << "1. Add Expense\n";
        cout << "2. Add Income\n";
        cout << "3. View Records\n";
        cout << "4. Search Records\n";
        cout << "5. Financial Summary\n";
        cout << "6. Investment Suggestions\n";
        cout << "7. Save Data\n";
        cout << "8. Load Data\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
    }

    void addExpense() {
        string description;
        double amount;
        string category;
        string date;
        
        cout << "\n=== Add Expense ===\n";
        
        cout << "Description: ";
        description = getInput();
        
        cout << "Amount: $";
        cin >> amount;
        
        // Show category options
        cout << "\nCategories:\n";
        const auto& categories = manager.getExpenseCategories();
        for (size_t i = 0; i < categories.size(); i++) {
            cout << i+1 << ". " << categories[i] << endl;
        }
        
        cout << "Select category (1-" << categories.size() << "): ";
        int choice = getInput(1, categories.size());
        category = categories[choice-1];
        
        cout << "Enter date (or leave blank for current): ";
        cin.ignore();
        getline(cin, date);
        if (date.empty()) date = "Current";
        
        Record expense(RecordType::EXPENSE, description, amount, category, date);
        manager.addRecord(expense);
        
        cout << "Expense added successfully!\n";
    }

    void addIncome() {
        string description;
        double amount;
        string category;
        string date;
        
        cout << "\n=== Add Income ===\n";
        
        cout << "Source: ";
        description = getInput();
        
        cout << "Amount: $";
        cin >> amount;
        
        // Show category options
        cout << "\nCategories:\n";
        const auto& categories = manager.getIncomeCategories();
        for (size_t i = 0; i < categories.size(); i++) {
            cout << i+1 << ". " << categories[i] << endl;
        }
        
        cout << "Select category (1-" << categories.size() << "): ";
        int choice = getInput(1, categories.size());
        category = categories[choice-1];
        
        cout << "Enter date (or leave blank for current): ";
        cin.ignore();
        getline(cin, date);
        if (date.empty()) date = "Current";
        
        Record income(RecordType::INCOME, description, amount, category, date);
        manager.addRecord(income);
        
        cout << "Income added successfully!\n";
    }

    void viewRecords() {
        manager.displayRecords();
    }

    void searchRecords() {
        cout << "\n=== Search Records ===\n";
        cout << "1. Search by keyword\n";
        cout << "2. Search by ID\n";
        cout << "Enter choice: ";
        
        int choice = getInput(1, 2);
        
        switch (choice) {
            case 1: {
                cout << "Enter keyword: ";
                string keyword = getInput();
                manager.searchRecords(keyword);
                break;
            }
            case 2: {
                cout << "Enter record ID: ";
                int id = getInput(1, 10000);
                manager.searchRecords(id);
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
            choice = getInput(1, 9);
            
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
                    financialSummary();
                    break;
                case 6:
                    suggestInvestments();
                    break;
                case 7:
                    saveData();
                    break;
                case 8:
                    loadData();
                    break;
                case 9:
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
