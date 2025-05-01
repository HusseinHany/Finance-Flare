#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <limits>
#include <algorithm>
#include <sstream>
#include <map>
#include <cctype>  // For password validation

using namespace std;

// ANSI Color codes for enhanced UI
namespace Color {
    const string RESET      = "\033[0m";
    const string BLACK      = "\033[30m";
    const string RED        = "\033[31m";
    const string GREEN      = "\033[32m";
    const string YELLOW     = "\033[33m";
    const string BLUE       = "\033[34m";
    const string MAGENTA    = "\033[35m";
    const string CYAN       = "\033[36m";
    const string WHITE      = "\033[37m";
    const string BOLD       = "\033[1m";
    const string UNDERLINE  = "\033[4m";
    const string BG_BLACK   = "\033[40m";
    const string BG_RED     = "\033[41m";
    const string BG_GREEN   = "\033[42m";
    const string BG_YELLOW  = "\033[43m";
    const string BG_BLUE    = "\033[44m";
    const string BG_MAGENTA = "\033[45m";
    const string BG_CYAN    = "\033[46m";
    const string BG_WHITE   = "\033[47m";
}

// Utility functions
string getCurrentDate() {
    time_t now = time(0);
    tm* localTime = localtime(&now);

    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", localTime);
    return string(buffer);
}

string getCurrentDateTime() {
    time_t now = time(0);
    tm* localTime = localtime(&now);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
    return string(buffer);
}

void splitString(const string& str, vector<string>& tokens, char delimiter) {
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }

    tokens.push_back(str.substr(start));
}

void displayHeader(const string& title) {
    cout << "\n" << Color::BG_BLUE << Color::WHITE << Color::BOLD;
    cout << " " << string(title.length() + 8, ' ') << " ";
    cout << Color::RESET << endl;

    cout << Color::BG_BLUE << Color::WHITE << Color::BOLD;
    cout << "    " << title << "    ";
    cout << Color::RESET << endl;

    cout << Color::BG_BLUE << Color::WHITE << Color::BOLD;
    cout << " " << string(title.length() + 8, ' ') << " ";
    cout << Color::RESET << endl;
}

void displayDivider() {
    cout << Color::CYAN << string(70, '-') << Color::RESET << endl;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// User class for authentication
class User {
private:
    string username;
    string password;
    string fullName;
    string email;
    string registrationDate;
    string lastLogin;

public:
    User(const string& username = "", const string& password = "",
         const string& fullName = "", const string& email = "")
        : username(username), password(password), fullName(fullName), email(email) {
        registrationDate = getCurrentDateTime();
        lastLogin = registrationDate;
    }

    // Getters
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getFullName() const { return fullName; }
    string getEmail() const { return email; }
    string getRegistrationDate() const { return registrationDate; }
    string getLastLogin() const { return lastLogin; }

    // Setters
    void setPassword(const string& newPassword) { password = newPassword; }
    void setFullName(const string& newFullName) { fullName = newFullName; }
    void setEmail(const string& newEmail) { email = newEmail; }
    void updateLastLogin() { lastLogin = getCurrentDateTime(); }

    // Convert user data to string for file storage
    string toString() const {
        return username + "," + password + "," + fullName + "," + email + "," +
               registrationDate + "," + lastLogin;
    }

    // Parse user data from string
    static User fromString(const string& str) {
        vector<string> tokens;
        splitString(str, tokens, ',');

        if (tokens.size() < 6) {
            return User(); // Return empty user if format is invalid
        }

        User user(tokens[0], tokens[1], tokens[2], tokens[3]);
        user.registrationDate = tokens[4];
        user.lastLogin = tokens[5];

        return user;
    }
};

// User Manager class for handling authentication and user management
class UserManager {
private:
    map<string, User> users;
    string currentUsername;
    string usersFilename;

    bool isValidUsername(const string& username) const {
        // Username must be at least 4 characters, alphanumeric with underscores
        if (username.length() < 4) {
            return false;
        }

        for (char c : username) {
            if (!isalnum(c) && c != '_') {
                return false;
            }
        }

        return true;
    }

    bool isStrongPassword(const string& password) const {
        // Password must be at least 6 characters with at least one number
        if (password.length() < 6) {
            return false;
        }

        bool hasDigit = false;
        for (char c : password) {
            if (isdigit(c)) {
                hasDigit = true;
                break;
            }
        }

        return hasDigit;
    }

public:
    UserManager(const string& usersFilename = "users.csv")
        : usersFilename(usersFilename), currentUsername("") {
        loadUsers();
    }

    // Load users from file
    void loadUsers() {
        ifstream file(usersFilename);
        if (!file) {
            // Create default admin user if file doesn't exist
            User admin("admin", "admin123", "Administrator", "admin@financeflare.com");
            users["admin"] = admin;
            saveUsers();
            return;
        }

        string line;
        while (getline(file, line)) {
            User user = User::fromString(line);
            if (!user.getUsername().empty()) {
                users[user.getUsername()] = user;
            }
        }

        file.close();
    }

    // Save users to file
    void saveUsers() const {
        ofstream file(usersFilename);
        if (!file) {
            cout << Color::RED << "✗ " << Color::BOLD << "Error: Unable to save user data." << Color::RESET << endl;
            return;
        }

        for (const auto& pair : users) {
            file << pair.second.toString() << endl;
        }

        file.close();
    }

    // Check if a username exists
    bool usernameExists(const string& username) const {
        return users.find(username) != users.end();
    }

    // Register a new user
    bool registerUser() {
        clearScreen();
        displayHeader("Create New Account");

        string username, password, confirmPassword, fullName, email;

        // Get username
        bool validUsername = false;
        while (!validUsername) {
            cout << "\n" << Color::YELLOW << "► " << Color::RESET << "Enter username (min 4 characters, alphanumeric): ";
            getline(cin, username);

            if (!isValidUsername(username)) {
                cout << Color::RED << "✗ Invalid username. Use at least 4 alphanumeric characters or underscore." << Color::RESET << endl;
                continue;
            }

            if (usernameExists(username)) {
                cout << Color::RED << "✗ Username already exists. Please choose another one." << Color::RESET << endl;
                continue;
            }

            validUsername = true;
        }

        // Get password
        bool validPassword = false;
        while (!validPassword) {
            cout << Color::YELLOW << "► " << Color::RESET << "Enter password (min 6 characters, include at least 1 number): ";
            getline(cin, password);

            if (!isStrongPassword(password)) {
                cout << Color::RED << "✗ Password too weak. Use at least 6 characters including at least 1 number." << Color::RESET << endl;
                continue;
            }

            cout << Color::YELLOW << "► " << Color::RESET << "Confirm password: ";
            getline(cin, confirmPassword);

            if (password != confirmPassword) {
                cout << Color::RED << "✗ Passwords don't match. Please try again." << Color::RESET << endl;
                continue;
            }

            validPassword = true;
        }

        // Get full name
        cout << Color::YELLOW << "► " << Color::RESET << "Enter your full name: ";
        getline(cin, fullName);

        // Get email
        cout << Color::YELLOW << "► " << Color::RESET << "Enter your email: ";
        getline(cin, email);

        // Create and store the new user
        User newUser(username, password, fullName, email);
        users[username] = newUser;
        saveUsers();

        cout << "\n" << Color::GREEN << "✓ " << Color::BOLD << "Account created successfully!" << Color::RESET << endl;
        cout << Color::YELLOW << "► " << Color::RESET << "You can now log in with your credentials." << endl;

        cout << "\n" << Color::MAGENTA << "Press Enter to continue..." << Color::RESET;
        cin.get();

        return true;
    }

    // Authenticate a user
    bool login() {
        clearScreen();
        displayHeader("User Login");

        string username, password;
        int attempts = 0;
        const int maxAttempts = 3;

        while (attempts < maxAttempts) {
            if (attempts > 0) {
                cout << Color::YELLOW << "⚠ " << Color::RESET << "Attempt " << attempts + 1 << " of " << maxAttempts << endl;
            }

            cout << "\n" << Color::YELLOW << "► " << Color::RESET << "Username: ";
            getline(cin, username);

            cout << Color::YELLOW << "► " << Color::RESET << "Password: ";
            getline(cin, password);

            if (usernameExists(username) && users[username].getPassword() == password) {
                users[username].updateLastLogin();
                saveUsers();

                currentUsername = username;

                cout << "\n" << Color::GREEN << "✓ " << Color::BOLD << "Login successful!" << Color::RESET << endl;
                cout << Color::CYAN << "Welcome back, " << users[username].getFullName() << "!" << Color::RESET << endl;

                cout << "\n" << Color::MAGENTA << "Press Enter to continue..." << Color::RESET;
                cin.get();

                return true;
            } else {
                cout << "\n" << Color::RED << "✗ " << Color::BOLD << "Invalid username or password." << Color::RESET << endl;
                attempts++;

                if (attempts < maxAttempts) {
                    cout << "Please try again." << endl;
                }
            }
        }

        cout << "\n" << Color::RED << "✗ " << Color::BOLD << "Too many failed attempts. Please try again later." << Color::RESET << endl;

        cout << "\n" << Color::MAGENTA << "Press Enter to continue..." << Color::RESET;
        cin.get();

        return false;
    }

    // Logout current user
    void logout() {
        currentUsername = "";
    }

    // Get the current user
    User getCurrentUser() const {
        if (currentUsername.empty() || !usernameExists(currentUsername)) {
            return User();
        }
        return users.at(currentUsername);
    }

    // Check if a user is logged in
    bool isLoggedIn() const {
        return !currentUsername.empty();
    }

    // Display the authentication menu
    int displayAuthMenu() const {
        clearScreen();

        cout << Color::CYAN << "\n";
        cout << " ╔═══════════════════════════════════════════════════╗\n";
        cout << " ║                                                   ║\n";
        cout << " ║  " << Color::BOLD << Color::YELLOW << "███████╗██╗███╗   ██╗ █████╗ ███╗   ██╗ ██████╗███████╗" << Color::CYAN << "  ║\n";
        cout << " ║  " << Color::BOLD << Color::YELLOW << "██╔════╝██║████╗  ██║██╔══██╗████╗  ██║██╔════╝██╔════╝" << Color::CYAN << "  ║\n";
        cout << " ║  " << Color::BOLD << Color::YELLOW << "█████╗  ██║██╔██╗ ██║███████║██╔██╗ ██║██║     █████╗  " << Color::CYAN << "  ║\n";
        cout << " ║  " << Color::BOLD << Color::YELLOW << "██╔══╝  ██║██║╚██╗██║██╔══██║██║╚██╗██║██║     ██╔══╝  " << Color::CYAN << "  ║\n";
        cout << " ║  " << Color::BOLD << Color::YELLOW << "██║     ██║██║ ╚████║██║  ██║██║ ╚████║╚██████╗███████╗" << Color::CYAN << "  ║\n";
        cout << " ║  " << Color::BOLD << Color::YELLOW << "╚═╝     ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝╚══════╝" << Color::CYAN << "  ║\n";
        cout << " ║                     " << Color::BOLD << Color::GREEN << "FLARE" << Color::CYAN << "                      ║\n";
        cout << " ║                                                   ║\n";
        cout << " ╚═══════════════════════════════════════════════════╝\n";
        cout << Color::RESET << "\n";

        cout << Color::GREEN << " Personal Finance Management System" << Color::RESET << "\n";
        cout << Color::YELLOW << " Current Date: " << Color::CYAN << getCurrentDate() << Color::RESET << "\n\n";

        cout << Color::BG_CYAN << Color::BLACK << Color::BOLD << " LOGIN MENU " << Color::RESET << "\n\n";

        cout << Color::YELLOW << " [1] " << Color::RESET << Color::CYAN << "Login" << Color::RESET << "\n";
        cout << Color::YELLOW << " [2] " << Color::RESET << Color::GREEN << "Create New Account" << Color::RESET << "\n";
        cout << Color::YELLOW << " [3] " << Color::RESET << Color::RED << "Exit" << Color::RESET << "\n";

        cout << "\n" << Color::YELLOW << "► " << Color::RESET << "Enter your choice (1-3): ";

        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        return choice;
    }
};

// Abstract base class for all financial records (demonstrates abstraction)
class Record {
protected:
    string date;
    double amount;
    string description;

public:
    // Constructor
    Record(const string& date = "", double amount = 0.0, const string& description = "")
        : date(date), amount(amount), description(description) {}

    // Virtual destructor for proper cleanup in inheritance hierarchy
    virtual ~Record() {}

    // Getters and setters (encapsulation)
    string getDate() const { return date; }
    double getAmount() const { return amount; }
    string getDescription() const { return description; }

    void setDate(const string& date) { this->date = date; }
    void setAmount(double amount) { this->amount = amount; }
    void setDescription(const string& description) { this->description = description; }

    // Pure virtual functions (polymorphism)
    virtual void display() const = 0;
    virtual string getType() const = 0;
    virtual string toFileString() const = 0;
    virtual string getColoredAmount() const = 0;

    // Function overloading example
    void update(const string& date) { this->date = date; }
    void update(double amount) { this->amount = amount; }
    void update(const string& date, double amount, const string& description) {
        this->date = date;
        this->amount = amount;
        this->description = description;
    }
};

// Expense class derived from Record
class Expense : public Record {
private:
    string category;

public:
    // Constructor
    Expense(const string& date = "", double amount = 0.0,
            const string& description = "", const string& category = "")
        : Record(date, amount, description), category(category) {}

    // Getters and setters
    string getCategory() const { return category; }
    void setCategory(const string& category) { this->category = category; }

    // Implementation of pure virtual functions
    void display() const override {
        cout << Color::CYAN << left << setw(12) << date << Color::RESET
             << left << setw(20) << description
             << Color::YELLOW << setw(15) << category << Color::RESET
             << right << setw(10) << getColoredAmount() << endl;
    }

    string getType() const override { return "EXPENSE"; }

    string toFileString() const override {
        return getType() + "," + date + "," + to_string(amount) + "," + description + "," + category;
    }

    string getColoredAmount() const override {
        stringstream ss;
        ss << Color::RED << "-$" << fixed << setprecision(2) << amount << Color::RESET;
        return ss.str();
    }

    // Function overloading
    void update(const string& category) { this->category = category; }
    void update(const string& date, double amount, const string& description, const string& category) {
        Record::update(date, amount, description);
        this->category = category;
    }
};

// Income class derived from Record
class Income : public Record {
private:
    string source;

public:
    // Constructor
    Income(const string& date = "", double amount = 0.0,
           const string& description = "", const string& source = "")
        : Record(date, amount, description), source(source) {}

    // Getters and setters
    string getSource() const { return source; }
    void setSource(const string& source) { this->source = source; }

    // Implementation of pure virtual functions
    void display() const override {
        cout << Color::CYAN << left << setw(12) << date << Color::RESET
             << left << setw(20) << description
             << Color::MAGENTA << setw(15) << source << Color::RESET
             << right << setw(10) << getColoredAmount() << endl;
    }

    string getType() const override { return "INCOME"; }

    string toFileString() const override {
        return getType() + "," + date + "," + to_string(amount) + "," + description + "," + source;
    }

    string getColoredAmount() const override {
        stringstream ss;
        ss << Color::GREEN << "+$" << fixed << setprecision(2) << amount << Color::RESET;
        return ss.str();
    }

    // Function overloading
    void update(const string& source) { this->source = source; }
    void update(const string& date, double amount, const string& description, const string& source) {
        Record::update(date, amount, description);
        this->source = source;
    }
};

// Investment class
class Investment {
private:
    string name;
    string type;
    double expectedReturn;
    double riskLevel; // 1-10 scale
    double minInvestment;

public:
    // Constructor
    Investment(const string& name = "", const string& type = "",
               double expectedReturn = 0.0, double riskLevel = 1.0, double minInvestment = 0.0)
        : name(name), type(type), expectedReturn(expectedReturn),
          riskLevel(riskLevel), minInvestment(minInvestment) {}

    // Getters
    string getName() const { return name; }
    string getType() const { return type; }
    double getExpectedReturn() const { return expectedReturn; }
    double getRiskLevel() const { return riskLevel; }
    double getMinInvestment() const { return minInvestment; }

    // Display method
    void display() const {
        cout << Color::BOLD << Color::CYAN << left << setw(20) << name << Color::RESET
             << Color::BLUE << setw(15) << type << Color::RESET;

        // Color-code return rates
        cout << right << setw(8);
        double returnPercentage = expectedReturn * 100;
        if (returnPercentage > 15) {
            cout << Color::GREEN << Color::BOLD;
        } else if (returnPercentage > 7) {
            cout << Color::YELLOW;
        } else {
            cout << Color::WHITE;
        }
        cout << fixed << setprecision(2) << returnPercentage << "%" << Color::RESET;

        // Color-code risk levels
        cout << right << setw(7);
        if (riskLevel >= 8) {
            cout << Color::RED << Color::BOLD;
        } else if (riskLevel >= 5) {
            cout << Color::YELLOW;
        } else {
            cout << Color::GREEN;
        }
        cout << fixed << setprecision(1) << riskLevel << "/10" << Color::RESET;

        cout << setw(15) << fixed << setprecision(2) << "$" << minInvestment << endl;
    }

    // Calculate potential returns
    double calculatePotentialReturn(double investment) const {
        return investment * expectedReturn;
    }

    // Function overloading
    bool isSuitableFor(double availableAmount) const {
        return availableAmount >= minInvestment;
    }

    bool isSuitableFor(double availableAmount, double maxRisk) const {
        return availableAmount >= minInvestment && riskLevel <= maxRisk;
    }
};

// Financial Manager class - acts as a controller
class FinanceManager {
private:
    vector<Record*> records;
    vector<Investment> investments;
    string dataFilename;
    string username;

    // Helper methods
    double getTotalIncome() const {
        double total = 0.0;
        for (const auto& record : records) {
            if (record->getType() == "INCOME") {
                total += record->getAmount();
            }
        }
        return total;
    }

    double getTotalExpenses() const {
        double total = 0.0;
        for (const auto& record : records) {
            if (record->getType() == "EXPENSE") {
                total += record->getAmount();
            }
        }
        return total;
    }

public:
    // Constructor - now accepts username to create user-specific files
    FinanceManager(const string& username)
        : username(username), dataFilename("finance_data_" + username + ".csv") {
        // Initialize with some investment options
        investments.push_back(Investment("Tech Growth Fund", "Stock", 0.12, 7.5, 1000.0));
        investments.push_back(Investment("Real Estate Trust", "Real Estate", 0.08, 5.0, 5000.0));
        investments.push_back(Investment("Government Bonds", "Bonds", 0.04, 2.0, 500.0));
        investments.push_back(Investment("Bitcoin", "Cryptocurrency", 0.25, 9.5, 250.0));
        investments.push_back(Investment("High-Yield Savings", "Savings", 0.03, 1.0, 100.0));
        investments.push_back(Investment("Green Energy ETF", "Stock", 0.15, 6.5, 750.0));
        investments.push_back(Investment("Gold", "Commodity", 0.06, 4.0, 300.0));

        loadFromFile();
    }

    // Destructor to clean up dynamically allocated records
    ~FinanceManager() {
        for (auto record : records) {
            delete record;
        }
    }

    // Display welcome screen
    void displayWelcome(const User& user) const {
        clearScreen();

        cout << Color::CYAN << "\n";
        cout << " ╔═══════════════════════════════════════════════════╗\n";
        cout << " ║                                                   ║\n";
        cout << " ║  " << Color::BOLD << Color::YELLOW << "███████╗██╗███╗   ██╗ █████╗ ███╗   ██╗ ██████╗███████╗" << Color::CYAN << "  ║\n";
        cout << " ║  " << Color::BOLD << Color::YELLOW << "██╔════╝██║████╗  ██║██╔══██╗████╗  ██║██╔════╝██╔════╝" << Color::CYAN << "  ║\n";
        cout << " ║  " << Color::BOLD << Color::YELLOW << "█████╗  ██║██╔██╗ ██║███████║██╔██╗ ██║██║     █████╗  " << Color::CYAN << "  ║\n";
        cout << " ║  " << Color::BOLD << Color::YELLOW << "██╔══╝  ██║██║╚██╗██║██╔══██║██║╚██╗██║██║     ██╔══╝  " << Color::CYAN << "  ║\n";
        cout << " ║  " << Color::BOLD << Color::YELLOW << "██║     ██║██║ ╚████║██║  ██║██║ ╚████║╚██████╗███████╗" << Color::CYAN << "  ║\n";
        cout << " ║  " << Color::BOLD << Color::YELLOW << "╚═╝     ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝╚══════╝" << Color::CYAN << "  ║\n";
        cout << " ║                     " << Color::BOLD << Color::GREEN << "FLARE" << Color::CYAN << "                      ║\n";
        cout << " ║                                                   ║\n";
        cout << " ╚═══════════════════════════════════════════════════╝\n";
        cout << Color::RESET << "\n";

        cout << Color::YELLOW << " ★ " << Color::RESET << "Welcome back, " << Color::BOLD << Color::GREEN << user.getFullName() << Color::RESET << "!\n";
        cout << Color::YELLOW << " ★ " << Color::RESET << "Username: " << Color::CYAN << user.getUsername() << Color::RESET << "\n";
        cout << Color::YELLOW << " ★ " << Color::RESET << "Current Date: " << Color::CYAN << getCurrentDate() << Color::RESET << "\n";
        cout << Color::YELLOW << " ★ " << Color::RESET << "Last Login: " << Color::MAGENTA << user.getLastLogin() << Color::RESET << "\n\n";

        // Show a quick summary if there are records
        if (!records.empty()) {
            double balance = getTotalIncome() - getTotalExpenses();
            cout << Color::WHITE << " Current Balance: ";
            if (balance > 0) {
                cout << Color::GREEN;
            } else if (balance < 0) {
                cout << Color::RED;
            }
            cout << "$" << fixed << setprecision(2) << balance << Color::RESET << "\n\n";
        }

        cout << Color::MAGENTA << " Press Enter to continue..." << Color::RESET;
        cin.get();
    }

    // Add a new expense
    void addExpense() {
        string date, description, category;
        double amount;

        displayHeader("Add New Expense");

        cout << "\n" << Color::YELLOW << "► " << Color::RESET << "Date (YYYY-MM-DD) " << Color::CYAN << "[Today: " << getCurrentDate() << "]: " << Color::RESET;
        getline(cin, date);
        if (date.empty()) {
            date = getCurrentDate();
        }

        cout << Color::YELLOW << "► " << Color::RESET << "Amount: " << Color::GREEN << "$" << Color::RESET;
        cin >> amount;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << Color::YELLOW << "► " << Color::RESET << "Description: " << Color::RESET;
        getline(cin, description);

        cout << Color::YELLOW << "► " << Color::RESET << "Category " << Color::CYAN << "(e.g., Food, Transport, Utilities): " << Color::RESET;
        getline(cin, category);

        records.push_back(new Expense(date, amount, description, category));

        cout << "\n" << Color::GREEN << "✓ " << Color::BOLD << "Expense added successfully!" << Color::RESET << "\n";

        saveToFile();
    }

    // Add a new income
    void addIncome() {
        string date, description, source;
        double amount;

        displayHeader("Add New Income");

        cout << "\n" << Color::YELLOW << "► " << Color::RESET << "Date (YYYY-MM-DD) " << Color::CYAN << "[Today: " << getCurrentDate() << "]: " << Color::RESET;
        getline(cin, date);
        if (date.empty()) {
            date = getCurrentDate();
        }

        cout << Color::YELLOW << "► " << Color::RESET << "Amount: " << Color::GREEN << "$" << Color::RESET;
        cin >> amount;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << Color::YELLOW << "► " << Color::RESET << "Description: " << Color::RESET;
        getline(cin, description);

        cout << Color::YELLOW << "► " << Color::RESET << "Source " << Color::CYAN << "(e.g., Salary, Freelance, Investment): " << Color::RESET;
        getline(cin, source);

        records.push_back(new Income(date, amount, description, source));

        cout << "\n" << Color::GREEN << "✓ " << Color::BOLD << "Income added successfully!" << Color::RESET << "\n";

        saveToFile();
    }

    // View all records
    void viewRecords() const {
        displayHeader("Financial Records");

        if (records.empty()) {
            cout << "\n" << Color::YELLOW << "ⓘ " << Color::RESET << "No records found. Try adding some income or expenses!\n";
            return;
        }

        cout << "\n" << Color::BOLD << Color::WHITE << left << setw(12) << "Date"
             << setw(20) << "Description"
             << setw(15) << "Category/Source"
             << right << setw(12) << "Amount" << Color::RESET << endl;

        displayDivider();

        for (const auto& record : records) {
            record->display();
        }

        // Display summary
        double totalIncome = getTotalIncome();
        double totalExpenses = getTotalExpenses();
        double balance = totalIncome - totalExpenses;

        displayDivider();

        cout << Color::BOLD << left << setw(47) << "Total Income:"
             << right << setw(10) << Color::GREEN << "$" << fixed << setprecision(2) << totalIncome << Color::RESET << endl;

        cout << Color::BOLD << left << setw(47) << "Total Expenses:"
             << right << setw(10) << Color::RED << "$" << fixed << setprecision(2) << totalExpenses << Color::RESET << endl;

        displayDivider();

        cout << Color::BOLD << left << setw(47) << "Balance:"
             << right << setw(10);

        if (balance >= 0) {
            cout << Color::GREEN;
        } else {
            cout << Color::RED;
        }

        cout << "$" << fixed << setprecision(2) << balance << Color::RESET << endl;

        cout << "\n" << Color::CYAN << "ⓘ " << Color::RESET;
        if (balance > 0) {
            cout << "You have a positive balance. Consider investing some of your savings!\n";
        } else if (balance < 0) {
            cout << "Your expenses exceed your income. Consider reviewing your budget.\n";
        } else {
            cout << "Your budget is exactly balanced.\n";
        }
    }

    // Edit a record
    void editRecord() {
        if (records.empty()) {
            cout << "\n" << Color::YELLOW << "ⓘ " << Color::RESET << "No records to edit. Try adding some first!\n";
            return;
        }

        displayHeader("Edit Record");

        // Display records with indices
        cout << "\n" << Color::BOLD << left << setw(7) << "Index"
             << setw(12) << "Date"
             << setw(20) << "Description"
             << setw(15) << "Category/Source"
             << right << setw(12) << "Amount" << Color::RESET << endl;

        displayDivider();

        for (size_t i = 0; i < records.size(); i++) {
            cout << Color::BOLD << Color::WHITE << left << setw(7) << i << Color::RESET;
            records[i]->display();
        }

        int index;
        cout << "\n" << Color::YELLOW << "► " << Color::RESET << "Enter the index of the record to edit "
             << Color::CYAN << "(0-" << records.size() - 1 << "): " << Color::RESET;
        cin >> index;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (index < 0 || index >= static_cast<int>(records.size())) {
            cout << "\n" << Color::RED << "✗ " << Color::BOLD << "Invalid index. Operation cancelled." << Color::RESET << "\n";
            return;
        }

        Record* record = records[index];
        string date, description;
        double amount;

        displayDivider();
        cout << Color::YELLOW << "Editing Record:\n" << Color::RESET;
        record->display();
        displayDivider();

        cout << "\n" << Color::YELLOW << "► " << Color::RESET << "New Date (YYYY-MM-DD) "
             << Color::CYAN << "[" << record->getDate() << "]: " << Color::RESET;
        getline(cin, date);
        if (!date.empty()) {
            record->setDate(date);
        }

        cout << Color::YELLOW << "► " << Color::RESET << "New Amount "
             << Color::CYAN << "[$" << record->getAmount() << "]: $" << Color::RESET;
        string amountStr;
        getline(cin, amountStr);
        if (!amountStr.empty()) {
            amount = stod(amountStr);
            record->setAmount(amount);
        }

        cout << Color::YELLOW << "► " << Color::RESET << "New Description "
             << Color::CYAN << "[" << record->getDescription() << "]: " << Color::RESET;
        getline(cin, description);
        if (!description.empty()) {
            record->setDescription(description);
        }

        if (record->getType() == "EXPENSE") {
            Expense* expense = dynamic_cast<Expense*>(record);
            string category;
            cout << Color::YELLOW << "► " << Color::RESET << "New Category "
                 << Color::CYAN << "[" << expense->getCategory() << "]: " << Color::RESET;
            getline(cin, category);
            if (!category.empty()) {
                expense->setCategory(category);
            }
        } else if (record->getType() == "INCOME") {
            Income* income = dynamic_cast<Income*>(record);
            string source;
            cout << Color::YELLOW << "► " << Color::RESET << "New Source "
                 << Color::CYAN << "[" << income->getSource() << "]: " << Color::RESET;
            getline(cin, source);
            if (!source.empty()) {
                income->setSource(source);
            }
        }

        cout << "\n" << Color::GREEN << "✓ " << Color::BOLD << "Record updated successfully!" << Color::RESET << "\n";
        saveToFile();
    }

    // Delete a record
    void deleteRecord() {
        if (records.empty()) {
            cout << "\n" << Color::YELLOW << "ⓘ " << Color::RESET << "No records to delete. Try adding some first!\n";
            return;
        }

        displayHeader("Delete Record");

        // Display records with indices
        cout << "\n" << Color::BOLD << left << setw(7) << "Index"
             << setw(12) << "Date"
             << setw(20) << "Description"
             << setw(15) << "Category/Source"
             << right << setw(12) << "Amount" << Color::RESET << endl;

        displayDivider();

        for (size_t i = 0; i < records.size(); i++) {
            cout << Color::BOLD << Color::WHITE << left << setw(7) << i << Color::RESET;
            records[i]->display();
        }

        int index;
        cout << "\n" << Color::YELLOW << "► " << Color::RESET << "Enter the index of the record to delete "
             << Color::CYAN << "(0-" << records.size() - 1 << "): " << Color::RESET;
        cin >> index;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (index < 0 || index >= static_cast<int>(records.size())) {
            cout << "\n" << Color::RED << "✗ " << Color::BOLD << "Invalid index. Operation cancelled." << Color::RESET << "\n";
            return;
        }

        // Show the record to be deleted
        displayDivider();
        cout << Color::RED << "Record to delete:\n" << Color::RESET;
        records[index]->display();
        displayDivider();

        // Confirm deletion
        char confirm;
        cout << "\n" << Color::YELLOW << "⚠ " << Color::RESET
             << "Are you sure you want to delete this record? "
             << Color::RED << "(y/n): " << Color::RESET;
        cin >> confirm;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (tolower(confirm) != 'y') {
            cout << "\n" << Color::YELLOW << "ⓘ " << Color::RESET << "Deletion cancelled.\n";
            return;
        }

        // Delete the record
        delete records[index];
        records.erase(records.begin() + index);

        cout << "\n" << Color::GREEN << "✓ " << Color::BOLD << "Record deleted successfully!" << Color::RESET << "\n";
        saveToFile();
    }

    // Get investment suggestions
    void getInvestmentSuggestions() const {
        displayHeader("Investment Suggestions");

        double totalIncome = getTotalIncome();
        double totalExpenses = getTotalExpenses();
        double balance = totalIncome - totalExpenses;

        cout << "\n" << Color::CYAN << "ⓘ " << Color::RESET << "Your current balance: ";
        if (balance >= 0) {
            cout << Color::GREEN;
        } else {
            cout << Color::RED;
        }
        cout << "$" << fixed << setprecision(2) << balance << Color::RESET << "\n\n";

        if (balance <= 0) {
            cout << Color::YELLOW << "⚠ " << Color::RESET << "You currently don't have a positive balance for investments.\n";
            cout << "   Focus on increasing income or reducing expenses before investing.\n";
            return;
        }

        cout << Color::BOLD << "Here are some investment options based on your financial situation:" << Color::RESET << "\n\n";

        cout << Color::BOLD << left << setw(20) << "Investment"
             << setw(15) << "Type"
             << right << setw(10) << "Return"
             << setw(10) << "Risk"
             << setw(15) << "Min. Amount" << Color::RESET << endl;

        displayDivider();

        // Filter investments based on available balance
        bool foundSuitableInvestments = false;

        for (const auto& investment : investments) {
            if (investment.isSuitableFor(balance)) {
                investment.display();

                // Show potential returns
                double annualReturn = investment.calculatePotentialReturn(balance);
                cout << "   " << Color::CYAN << "➤ " << Color::RESET << "Potential annual return: "
                     << Color::GREEN << "$" << fixed << setprecision(2) << annualReturn << Color::RESET << "\n";

                foundSuitableInvestments = true;
            }
        }

        if (!foundSuitableInvestments) {
            cout << Color::YELLOW << "ⓘ " << Color::RESET << "No suitable investments found for your current balance.\n";
            cout << "   Consider saving more or exploring micro-investment options.\n";
        }

        cout << "\n" << Color::YELLOW << "⚠ " << Color::BOLD << "Investment Disclaimer: " << Color::RESET
             << "These are simplified suggestions for educational purposes.\n";
        cout << "   Always consult a financial advisor before making investment decisions.\n";
    }

    // Load data from file
    void loadFromFile() {
        ifstream file(dataFilename);
        if (!file) {
            cout << Color::YELLOW << "ⓘ " << Color::RESET << "No existing data file found for user " << username << ". A new file will be created when records are added.\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            vector<string> tokens;
            splitString(line, tokens, ',');

            if (tokens.size() < 5) continue;

            string type = tokens[0];
            string date = tokens[1];
            double amount = stod(tokens[2]);
            string description = tokens[3];
            string categoryOrSource = tokens[4];

            if (type == "EXPENSE") {
                records.push_back(new Expense(date, amount, description, categoryOrSource));
            } else if (type == "INCOME") {
                records.push_back(new Income(date, amount, description, categoryOrSource));
            }
        }

        file.close();
    }

    // Save data to file
    void saveToFile() const {
        ofstream file(dataFilename);
        if (!file) {
            cout << "\n" << Color::RED << "✗ " << Color::BOLD << "Error: Unable to open file for writing: "
                 << dataFilename << Color::RESET << endl;
            return;
        }

        for (const auto& record : records) {
            file << record->toFileString() << endl;
        }

        file.close();
    }

    // Display menu
    void displayMenu() const {
        clearScreen();

        cout << "\n" << Color::BG_CYAN << Color::BLACK << Color::BOLD << " FINANCE-FLARE MENU " << Color::RESET << "\n\n";

        cout << Color::YELLOW << " [1] " << Color::RESET << Color::RED << "Add Expense" << Color::RESET << "\n";
        cout << Color::YELLOW << " [2] " << Color::RESET << Color::GREEN << "Add Income" << Color::RESET << "\n";
        cout << Color::YELLOW << " [3] " << Color::RESET << Color::CYAN << "View Financial Records" << Color::RESET << "\n";
        cout << Color::YELLOW << " [4] " << Color::RESET << Color::MAGENTA << "Edit Record" << Color::RESET << "\n";
        cout << Color::YELLOW << " [5] " << Color::RESET << Color::RED << "Delete Record" << Color::RESET << "\n";
        cout << Color::YELLOW << " [6] " << Color::RESET << Color::BLUE << "Get Investment Suggestions" << Color::RESET << "\n";
        cout << Color::YELLOW << " [7] " << Color::RESET << Color::GREEN << "Save and Logout" << Color::RESET << "\n";
        cout << Color::YELLOW << " [8] " << Color::RESET << "Exit Application" << Color::RESET << "\n";

        cout << "\n" << Color::YELLOW << "► " << Color::RESET << "Enter your choice (1-8): ";
    }
};

int main() {
    UserManager userManager;

    bool exitApplication = false;

    while (!exitApplication) {
        // Show authentication menu if not logged in
        if (!userManager.isLoggedIn()) {
            int authChoice = userManager.displayAuthMenu();

            switch (authChoice) {
                case 1: // Login
                    if (!userManager.login()) {
                        continue;
                    }
                    break;

                case 2: // Create new account
                    userManager.registerUser();
                    continue;

                case 3: // Exit application
                    cout << Color::YELLOW << "ⓘ " << Color::BOLD << "Thank you for using Finance-Flare!" << Color::RESET << "\n";
                    exitApplication = true;
                    continue;

                default:
                    cout << Color::RED << "✗ " << Color::BOLD << "Invalid choice. Please try again." << Color::RESET << "\n";
                    continue;
            }
        }

        // Get current user and initialize finance manager
        User currentUser = userManager.getCurrentUser();
        FinanceManager manager(currentUser.getUsername());

        // Display welcome screen
        manager.displayWelcome(currentUser);

        // Main application loop
        int choice = 0;
        bool loggedOut = false;

        while (!loggedOut && choice != 8) {
            manager.displayMenu();

            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            clearScreen();

            switch (choice) {
                case 1:
                    manager.addExpense();
                    break;
                case 2:
                    manager.addIncome();
                    break;
                case 3:
                    manager.viewRecords();
                    break;
                case 4:
                    manager.editRecord();
                    break;
                case 5:
                    manager.deleteRecord();
                    break;
                case 6:
                    manager.getInvestmentSuggestions();
                    break;
                case 7:
                    cout << Color::GREEN << "✓ " << Color::BOLD << "Saving data and logging out..." << Color::RESET << "\n";
                    userManager.logout();
                    loggedOut = true;
                    break;
                case 8:
                    cout << Color::YELLOW << "ⓘ " << Color::BOLD << "Exiting application..." << Color::RESET << "\n";
                    exitApplication = true;
                    break;
                default:
                    cout << Color::RED << "✗ " << Color::BOLD << "Invalid choice. Please try again." << Color::RESET << "\n";
            }

            if (!loggedOut && choice != 8) {
                cout << "\n" << Color::MAGENTA << "Press Enter to continue..." << Color::RESET;
                cin.get();
            }
        }
    }

    return 0;
}
