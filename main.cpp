// Finance Flare: a console personal finance manager.
// C++ OOP final project by the Byte Masters team.
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <cstdio>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#endif
using namespace std;

// Animations run only in a real terminal; piped input and output stay instant.
bool animationsOn() {
#ifdef _WIN32
    static bool on = _isatty(_fileno(stdout)) != 0;
#else
    static bool on = isatty(fileno(stdout)) != 0;
#endif
    return on;
}

// ─── Colours ────────────────────────────────────────────────────────
const string RESET = "\033[0m", RED = "\033[31m", GREEN = "\033[32m", YELLOW = "\033[33m",
             CYAN = "\033[96m", BRIGHT_GREEN = "\033[92m", GREY = "\033[90m";

// ─── Investment options (static array) ──────────────────────────────
// Expected yearly return of each option. Profit is simple interest:
// amount × yearly rate × months / 12.
struct InvestmentOption {
    const char* name;
    double yearlyRate;
};

const int OPTION_COUNT = 20;
const InvestmentOption investmentOptions[OPTION_COUNT] = {
    {"High-risk Investment", 0.38},       {"Venture Capital", 0.36},
    {"Stock Market Investment", 0.28},    {"Hedge Funds", 0.26},
    {"Medium-risk Investment", 0.24},     {"Real Estate Investment", 0.20},
    {"Peer-to-Peer Lending", 0.18},       {"Precious Metals (Gold/Silver)", 0.14},
    {"Low-risk Investment", 0.12},        {"Cryptocurrency Investment", 0.11},
    {"Bonds", 0.098},                     {"Mutual Funds", 0.09},
    {"Art and Collectibles", 0.084},      {"Savings Account", 0.065},
    {"Carbon Credit Investment", 0.06},   {"Emergency Fund", 0.036},
    {"Diversified Portfolio", 0.032},     {"Retirement Account", 0.03},
    {"Investment in Health", 0.02},       {"Investment in Education", 0.012}};

// Suggested split of a balance under $10,000 across starter options.
struct StarterSplit {
    const char* name;
    double share;
};
const int STARTER_COUNT = 10;
const StarterSplit starterSplit[STARTER_COUNT] = {
    {"Fractional Shares", 0.20},      {"Micro-Investing Apps", 0.22},
    {"Cryptocurrency", 0.20},         {"Peer-to-Peer Lending", 0.10},
    {"Robo-Advisors", 0.08},          {"Real Estate Crowdfunding", 0.06},
    {"Dividend Stocks", 0.05},        {"NFTs (Non-Fungible Tokens)", 0.02},
    {"Collectibles", 0.02},           {"Precious Metals", 0.05}};

// ─── Small helpers ──────────────────────────────────────────────────
// $1,234.56 with thousands separators.
string money(double value) {
    ostringstream out;
    out << fixed << setprecision(2) << (value < 0 ? -value : value);
    string digits = out.str();
    for (int i = (int)digits.find('.') - 3; i > 0; i -= 3) digits.insert(i, ",");
    return (value < 0 ? "-$" : "$") + digits;
}

string percent(double rate) {
    ostringstream out;
    out << setprecision(3) << rate * 100 << "%";
    return out.str();
}

void pauseFor(int ms) {
    if (animationsOn()) this_thread::sleep_for(chrono::milliseconds(ms));
}

// Prints text one character at a time.
void typeOut(const string& text, int msPerChar = 18) {
    for (char ch : text) {
        cout << ch << flush;
        pauseFor(msPerChar);
    }
}

// Splits a saved line on commas.
vector<string> splitFields(const string& line) {
    vector<string> fields;
    string field;
    stringstream in(line);
    while (getline(in, field, ',')) fields.push_back(field);
    if (!line.empty() && line.back() == ',') fields.push_back("");
    return fields;
}

// ─── Screen ─────────────────────────────────────────────────────────
class screen {
public:
    static void clear() {
#ifdef _WIN32
        system("cls");
#else
        cout << "\033[2J\033[H";
#endif
    }
};

// ─── Input ──────────────────────────────────────────────────────────
// Every read is a whole line, so a letter where a number belongs is just
// an invalid answer, and closed input (Ctrl+D or the end of a piped file)
// exits cleanly instead of looping forever.
class Input {
public:
    static string readLine() {
        string line;
        if (!getline(cin, line)) {
            cout << "\nInput closed. Exiting.\n";
            exit(0);
        }
        if (!line.empty() && line.back() == '\r') line.pop_back();
        return line;
    }

    static int readInt(const string& prompt, int lo, int hi) {
        while (true) {
            cout << prompt;
            string line = readLine();
            try {
                size_t used = 0;
                int value = stoi(line, &used);
                if (line.find_first_not_of(" \t", used) == string::npos && value >= lo && value <= hi) return value;
            } catch (...) {}
            cout << RED << "Please enter a whole number from " << lo << " to " << hi << "." << RESET << endl;
        }
    }

    static double readAmount(const string& prompt, double maxValue) {
        while (true) {
            cout << prompt;
            string line = readLine();
            try {
                size_t used = 0;
                double value = stod(line, &used);
                if (line.find_first_not_of(" \t", used) != string::npos) throw 1;
                if (value <= 0) {
                    cout << RED << "The amount must be greater than zero." << RESET << endl;
                } else if (value > maxValue) {
                    cout << RED << "The amount can't be more than " << money(maxValue) << "." << RESET << endl;
                } else {
                    return value;
                }
                continue;
            } catch (...) {}
            cout << RED << "Please enter a number, for example 250 or 99.50." << RESET << endl;
        }
    }

    // Text that is saved to a file: commas would split the record, so they
    // become semicolons.
    static string readText(const string& prompt, bool required = true) {
        while (true) {
            cout << prompt;
            string text = readLine();
            replace(text.begin(), text.end(), ',', ';');
            size_t first = text.find_first_not_of(" \t");
            text = (first == string::npos) ? "" : text.substr(first, text.find_last_not_of(" \t") - first + 1);
            if (!text.empty() || !required) return text;
            cout << RED << "This can't be empty." << RESET << endl;
        }
    }

    static void waitForEnter() {
        cout << GREY << "\nPress Enter to continue..." << RESET;
        readLine();
    }
};

// ─── Record type ────────────────────────────────────────────────────
class RecordType {
public:
    static const int income = 0;
    static const int expense = 1;
    static string label(int type) { return type == income ? "Income" : "Expense"; }
};

// ─── Validation ─────────────────────────────────────────────────────
class Validator {
public:
    static string selectCategory(const string& title, const vector<string>& categories) {
        cout << "\n" << YELLOW << "=== Select " << title << " Category ===" << RESET << "\n";
        for (size_t i = 0; i < categories.size(); i++)
            cout << YELLOW << i + 1 << ". " << RESET << categories[i] << endl;
        int choice = Input::readInt("Enter your choice (1-" + to_string(categories.size()) + "): ", 1, (int)categories.size());
        return categories[choice - 1];
    }

    static string selectIncomeCategory() {
        return selectCategory("Income", {"Salary", "Business", "Investment", "Gift", "Other"});
    }

    static string selectExpenseCategory() {
        return selectCategory("Expense", {"Food", "Housing", "Transportation", "Entertainment", "Shopping",
                                          "Utilities", "Healthcare", "Education", "Personal", "Other"});
    }

    static bool isLeapYear(int year) { return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0); }

    static int daysInMonth(int month, int year) {
        if (month == 2) return isLeapYear(year) ? 29 : 28;
        if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
        return 31;
    }

    // Returns the date as DD/MM/YYYY.
    static string validateDate() {
        while (true) {
            int day = Input::readInt(YELLOW + "Enter day (1-31): " + RESET, 1, 31);
            int month = Input::readInt(YELLOW + "Enter month (1-12): " + RESET, 1, 12);
            int year = Input::readInt(YELLOW + "Enter year (2000-2100): " + RESET, 2000, 2100);
            if (day > daysInMonth(month, year)) {
                cout << RED << "That month only has " << daysInMonth(month, year) << " days. Please enter the date again." << RESET << endl;
                continue;
            }
            ostringstream out;
            out << setfill('0') << setw(2) << day << "/" << setw(2) << month << "/" << year;
            return out.str();
        }
    }

    // DD/MM/YYYY -> YYYYMMDD, for sorting by date.
    static string sortKey(const string& date) {
        if (date.size() != 10) return date;
        return date.substr(6, 4) + date.substr(3, 2) + date.substr(0, 2);
    }
};

// ─── Transactions (inheritance) ─────────────────────────────────────
class Transaction {
protected:
    string description;
    double amount;
    string date;
    string category;

public:
    Transaction(string d, double a, string dt, string c) : description(d), amount(a), date(dt), category(c) {}
    virtual ~Transaction() {}

    void setDescription(string d) { description = d; }
    void setAmount(double a) { amount = a; }
    void setDate(string dt) { date = dt; }
    void setCategory(string c) { category = c; }

    string getDescription() const { return description; }
    double getAmount() const { return amount; }
    string getDate() const { return date; }
    string getCategory() const { return category; }

    virtual int type() const = 0;

    string toLine() const { return description + "," + to_string(amount) + "," + date + "," + category; }

    void display() const {
        cout << (type() == RecordType::income ? BRIGHT_GREEN : RED) << left << setw(8) << RecordType::label(type())
             << RESET << " | " << setw(12) << date << " | " << setw(14) << category << " | " << right << setw(12)
             << money(amount) << " | " << description << endl;
    }
};

class Income : public Transaction {
public:
    Income(string d, double a, string dt, string c) : Transaction(d, a, dt, c) {}
    int type() const override { return RecordType::income; }
};

class Expense : public Transaction {
public:
    Expense(string d, double a, string dt, string c) : Transaction(d, a, dt, c) {}
    int type() const override { return RecordType::expense; }
};

// ─── Investments (array of objects) ─────────────────────────────────
struct Investment {
    int optionNumber;
    int durationMonths;
    double amount;
    string date;

    Investment() : optionNumber(0), durationMonths(0), amount(0.0), date("") {}
    Investment(int opt, int dur, double amt, string dt) : optionNumber(opt), durationMonths(dur), amount(amt), date(dt) {}
};

class InvestmentUtil {
public:
    static bool isValidOption(int optionNumber) { return optionNumber >= 1 && optionNumber <= OPTION_COUNT; }

    static string name(int optionNumber) {
        return isValidOption(optionNumber) ? investmentOptions[optionNumber - 1].name : "Unknown option";
    }

    static double yearlyRate(int optionNumber) {
        return isValidOption(optionNumber) ? investmentOptions[optionNumber - 1].yearlyRate : 0.0;
    }

    static double expectedProfit(const Investment& inv) {
        return inv.amount * yearlyRate(inv.optionNumber) * inv.durationMonths / 12.0;
    }
};

// ─── Files ──────────────────────────────────────────────────────────
class FileManager {
public:
    static const int MAX_INVESTMENTS = 100;

    template <typename T>
    static vector<T> readTransactions(const string& path) {
        vector<T> records;
        ifstream file(path);
        string line;
        while (getline(file, line)) {
            vector<string> f = splitFields(line);
            if (f.size() != 4) continue;
            try {
                records.emplace_back(f[0], stod(f[1]), f[2], f[3]);
            } catch (...) {
                // A damaged line is skipped instead of stopping the program.
            }
        }
        return records;
    }

    static vector<Income> readIncome() { return readTransactions<Income>("income.txt"); }
    static vector<Expense> readExpenses() { return readTransactions<Expense>("expense.txt"); }

    static bool append(const string& path, const string& line) {
        ofstream file(path, ios::app);
        if (!file) {
            cout << RED << "Could not open " << path << "." << RESET << endl;
            return false;
        }
        file << line << endl;
        return true;
    }

    static void saveTransaction(const Transaction& t) {
        append(t.type() == RecordType::income ? "income.txt" : "expense.txt", t.toLine());
        append("transactions.txt", t.toLine());
    }

    // Rewrites a file without the lines whose description matches; returns how many were removed.
    static int removeByDescription(const string& path, const string& description) {
        ifstream in(path);
        if (!in) return 0;
        vector<string> kept;
        int removed = 0;
        string line;
        while (getline(in, line)) {
            vector<string> f = splitFields(line);
            if (!f.empty() && f[0] == description) removed++;
            else kept.push_back(line);
        }
        in.close();
        ofstream out(path);
        for (const string& k : kept) out << k << endl;
        return removed;
    }

    static void saveInvestment(const Investment& inv) {
        if (append("investments.txt", to_string(inv.optionNumber) + "," + to_string(inv.durationMonths) + "," +
                                          to_string(inv.amount) + "," + inv.date))
            cout << GREEN << "Investment saved." << RESET << endl;
    }

    static int readInvestments(Investment investments[]) {
        ifstream file("investments.txt");
        int count = 0;
        string line;
        while (count < MAX_INVESTMENTS && getline(file, line)) {
            vector<string> f = splitFields(line);
            if (f.size() != 4) continue;
            try {
                investments[count] = Investment(stoi(f[0]), stoi(f[1]), stod(f[2]), f[3]);
                count++;
            } catch (...) {}
        }
        return count;
    }
};

// ─── Business logic ─────────────────────────────────────────────────
class FinanceManager {
public:
    static void addIncome(string description, double amount, string date, string category) {
        FileManager::saveTransaction(Income(description, amount, date, category));
        cout << GREEN << "Income of " << money(amount) << " saved." << RESET << endl;
    }

    // Overload: the note is kept with the description.
    static void addIncome(string description, double amount, string date, string category, string note) {
        addIncome(description + " (" + note + ")", amount, date, category);
    }

    static void addExpense(string description, double amount, string date, string category) {
        FileManager::saveTransaction(Expense(description, amount, date, category));
        cout << GREEN << "Expense of " << money(amount) << " saved." << RESET << endl;
    }

    static double totalIncome() {
        double total = 0;
        for (const auto& t : FileManager::readIncome()) total += t.getAmount();
        return total;
    }

    static double totalExpenses() {
        double total = 0;
        for (const auto& t : FileManager::readExpenses()) total += t.getAmount();
        return total;
    }

    static double totalInvested() {
        Investment investments[FileManager::MAX_INVESTMENTS];
        int count = FileManager::readInvestments(investments);
        double total = 0;
        for (int i = 0; i < count; i++) total += investments[i].amount;
        return total;
    }

    static double availableBalance() { return totalIncome() - totalExpenses() - totalInvested(); }

    static void printHeader(const string& title) {
        cout << CYAN << "==== " << title << " ====" << RESET << endl;
        cout << left << setw(8) << "Type" << " | " << setw(12) << "Date" << " | " << setw(14) << "Category" << " | "
             << right << setw(12) << "Amount" << " | Description" << endl;
        cout << string(72, '-') << endl;
    }

    template <typename T>
    static void list(const string& title, const vector<T>& records) {
        printHeader(title);
        if (records.empty()) cout << YELLOW << "Nothing recorded yet." << RESET << endl;
        double total = 0;
        for (const auto& r : records) {
            r.display();
            total += r.getAmount();
        }
        cout << string(72, '-') << endl << "Total: " << money(total) << " in " << records.size() << " record(s)" << endl;
    }

    static void viewIncome() { list("Income", FileManager::readIncome()); }
    static void viewExpense() { list("Expenses", FileManager::readExpenses()); }

    // Income and expenses together, oldest first.
    static void viewAllTransactions() {
        vector<Income> income = FileManager::readIncome();
        vector<Expense> expenses = FileManager::readExpenses();
        vector<const Transaction*> all;
        for (const auto& t : income) all.push_back(&t);
        for (const auto& t : expenses) all.push_back(&t);
        stable_sort(all.begin(), all.end(), [](const Transaction* a, const Transaction* b) {
            return Validator::sortKey(a->getDate()) < Validator::sortKey(b->getDate());
        });
        printHeader("All Transactions");
        if (all.empty()) cout << YELLOW << "Nothing recorded yet." << RESET << endl;
        for (const Transaction* t : all) t->display();
        cout << string(72, '-') << endl << "Balance: " << money(totalIncome() - totalExpenses()) << endl;
    }

    static void deleteTransaction(const string& description) {
        int removed = FileManager::removeByDescription("income.txt", description) +
                      FileManager::removeByDescription("expense.txt", description);
        FileManager::removeByDescription("transactions.txt", description);
        if (removed == 0) cout << YELLOW << "No transaction is called \"" << description << "\"." << RESET << endl;
        else cout << GREEN << "Deleted " << removed << " transaction(s) called \"" << description << "\"." << RESET << endl;
    }

    static void viewInvestments() {
        Investment investments[FileManager::MAX_INVESTMENTS];
        int count = FileManager::readInvestments(investments);
        cout << CYAN << "==== Your Investments ====" << RESET << endl;
        if (count == 0) {
            cout << YELLOW << "No investments yet." << RESET << endl;
            return;
        }
        for (int i = 0; i < count; i++) {
            const Investment& inv = investments[i];
            double profit = InvestmentUtil::expectedProfit(inv);
            cout << YELLOW << "Investment:      " << RESET << InvestmentUtil::name(inv.optionNumber) << " (option "
                 << inv.optionNumber << ", " << percent(InvestmentUtil::yearlyRate(inv.optionNumber)) << " a year)" << endl;
            cout << YELLOW << "Amount invested: " << RESET << money(inv.amount) << endl;
            cout << YELLOW << "Duration:        " << RESET << inv.durationMonths << " months from " << inv.date << endl;
            cout << GREEN << "Expected profit: " << RESET << money(profit) << endl;
            cout << GREEN << "Expected value:  " << RESET << money(inv.amount + profit) << endl;
            cout << "------------------------" << endl;
        }
    }

    // A spinner while the suggestions are prepared.
    static void showLoading(const string& message = "Analysing your balance") {
        if (!animationsOn()) return;
        const char frames[] = {'|', '/', '-', '\\'};
        for (int i = 0; i < 14; ++i) {
            cout << "\r" << YELLOW << frames[i % 4] << RESET << " " << GREY << message << RESET << flush;
            pauseFor(70);
        }
        cout << "\r" << GREEN << "✓" << RESET << " " << message << "   " << endl;
    }

    static void suggestInvestmentsAndOptions() {
        double income = totalIncome(), expenses = totalExpenses(), invested = totalInvested();
        double available = income - expenses - invested;

        const int w = 20;
        cout << CYAN << "========= Investment Suggestions =========" << RESET << endl;
        cout << GREEN << right << setw(w) << "Total Income" << setw(w) << "Total Expenses" << setw(w) << "Invested"
             << setw(w) << "Available" << endl;
        cout << setw(w) << money(income) << setw(w) << money(expenses) << setw(w) << money(invested) << setw(w)
             << money(available) << RESET << endl;

        if (available <= 0) {
            cout << RED << "\nYou don't have any money available to invest right now." << RESET << endl;
            return;
        }

        showLoading();
        if (available < 10000) {
            cout << YELLOW << "\nSuggested split for a balance under $10,000:" << RESET << endl;
            for (int i = 0; i < STARTER_COUNT; i++)
                cout << "  " << left << setw(30) << starterSplit[i].name << right << setw(5)
                     << percent(starterSplit[i].share) << "  " << money(available * starterSplit[i].share) << endl;
        }

        cout << YELLOW << "\nInvestment options (expected return per year):" << RESET << endl;
        for (int i = 0; i < OPTION_COUNT; i++) {
            double rate = investmentOptions[i].yearlyRate;
            string colour = rate >= 0.24 ? BRIGHT_GREEN : rate >= 0.12 ? YELLOW : "\033[91m";
            cout << right << setw(3) << i + 1 << ". " << left << setw(32) << investmentOptions[i].name << colour
                 << percent(rate) << RESET << endl;
        }
        cout << CYAN << right << setw(3) << OPTION_COUNT + 1 << ". Return to main menu" << RESET << endl;

        int option = Input::readInt(BRIGHT_GREEN + "Choose an option (1-21): " + RESET, 1, OPTION_COUNT + 1);
        if (option == OPTION_COUNT + 1) return;

        double amount = Input::readAmount(YELLOW + "Amount to invest: " + RESET, available);
        int months = Input::readInt(YELLOW + "Duration in months (1-600): " + RESET, 1, 600);
        string date = Validator::validateDate();

        Investment inv(option, months, amount, date);
        FileManager::saveInvestment(inv);

        double profit = InvestmentUtil::expectedProfit(inv);
        cout << YELLOW << "\nYou invested " << money(amount) << " in " << InvestmentUtil::name(option) << " for "
             << months << " months." << RESET << endl;
        pauseFor(600);
        cout << "Expected profit: " << GREEN << money(profit) << RESET << endl;
        cout << "Expected value:  " << GREEN << money(amount + profit) << RESET << endl;
    }

    static void bar(const string& label, const string& colour, double pct) {
        cout << colour << left << setw(11) << label << RESET << right << setw(6) << fixed << setprecision(1) << pct
             << "% ";
        int length = pct > 0 ? (int)(min(pct, 100.0) / 2) : 0;
        cout << colour;
        for (int i = 0; i < length; i++) {   // the bar grows
            cout << "█" << flush;
            pauseFor(12);
        }
        cout << RESET << endl;
    }

    static void displayFinancialSummary() {
        double income = totalIncome(), expenses = totalExpenses(), invested = totalInvested();
        double available = income - expenses - invested;

        Investment investments[FileManager::MAX_INVESTMENTS];
        int count = FileManager::readInvestments(investments);
        double expectedProfit = 0;
        for (int i = 0; i < count; i++) expectedProfit += InvestmentUtil::expectedProfit(investments[i]);

        cout << CYAN << "========== Financial Summary ==========" << RESET << "\n\n";
        cout << GREEN << "Total income:          " << RESET << money(income) << endl;
        cout << RED << "Total expenses:        " << RESET << money(expenses) << endl;
        cout << YELLOW << "Total invested:        " << RESET << money(invested) << endl;
        cout << YELLOW << "Available balance:     " << RESET << money(available) << endl;
        cout << YELLOW << "Expected profit:       " << RESET << money(expectedProfit) << endl;
        cout << GREEN << "Expected value of investments: " << RESET << money(invested + expectedProfit) << endl;

        cout << "\n" << CYAN << "========== Where the income went ==========" << RESET << "\n";
        if (income <= 0) {
            cout << YELLOW << "Add some income to see the split." << RESET << endl;
            return;
        }
        bar("Expenses", RED, expenses / income * 100);
        bar("Invested", YELLOW, invested / income * 100);
        bar("Available", GREEN, available / income * 100);
    }
};

// ─── Menus ──────────────────────────────────────────────────────────
class UserInterface {
public:
    // Animated title screen, shown once at start-up.
    static void splash() {
        const string logo[] = {
            "  ___ _                          ___ _               ",
            " | __(_)_ _  __ _ _ _  __ ___   | __| |__ _ _ _ ___  ",
            " | _|| | ' \\/ _` | ' \\/ _/ -_)  | _|| / _` | '_/ -_) ",
            " |_| |_|_||_\\__,_|_||_\\__\\___|  |_| |_\\__,_|_| \\___| "};
        const string shades[] = {"\033[93m", "\033[33m", "\033[38;5;208m", "\033[91m"};
        screen::clear();
        cout << endl;
        for (int i = 0; i < 4; i++) {
            cout << shades[i] << logo[i] << RESET << endl;
            pauseFor(110);
        }
        cout << endl << "  ";
        typeOut("Track income and expenses. Plan investments. By Byte Masters.");
        cout << endl << endl << "  " << GREY << "[" << RESET;
        for (int i = 0; i < 30; i++) {
            cout << BRIGHT_GREEN << "=" << RESET << flush;
            pauseFor(25);
        }
        cout << GREY << "]" << RESET << " ready" << endl;
        pauseFor(500);
    }

    // One line with the money picture, refreshed every time the menu is shown.
    static void dashboard() {
        double income = FinanceManager::totalIncome(), expenses = FinanceManager::totalExpenses();
        double available = income - expenses - FinanceManager::totalInvested();
        cout << GREEN << " In " << money(income) << RESET << GREY << "  |  " << RESET << RED << "Out " << money(expenses)
             << RESET << GREY << "  |  " << RESET << (available >= 0 ? BRIGHT_GREEN : RED) << "Available "
             << money(available) << RESET << endl;
    }

    static void displayMenu() {
        cout << "==================================" << endl;
        cout << BRIGHT_GREEN << "          Byte Masters" << RESET << endl;
        cout << GREEN << "  Welcome to the Finance Manager" << RESET << endl;
        cout << "==================================" << endl;
        dashboard();
        cout << "----------------------------------" << endl;
        cout << CYAN << "1. Add Income\n2. Add Expense\n3. Delete Transaction\n4. Investment Suggestions\n5. Stats & Reports" << RESET << endl;
        cout << RED << "6. Exit" << RESET << endl;
    }

    static void displayStatsMenu() {
        cout << "==================================" << endl;
        cout << BRIGHT_GREEN << "     Finance Statistics Menu" << RESET << endl;
        cout << "==================================" << endl;
        cout << CYAN << "1. View Income\n2. View Expenses\n3. View All Transactions\n4. View Investments\n5. Financial Summary\n6. Return to Main Menu" << RESET << endl;
    }

    static void handleStatsChoice() {
        while (true) {
            screen::clear();
            displayStatsMenu();
            int choice = Input::readInt(GREEN + "Enter your choice: " + RESET, 1, 6);
            if (choice == 6) return;
            screen::clear();
            switch (choice) {
            case 1: FinanceManager::viewIncome(); break;
            case 2: FinanceManager::viewExpense(); break;
            case 3: FinanceManager::viewAllTransactions(); break;
            case 4: FinanceManager::viewInvestments(); break;
            case 5: FinanceManager::displayFinancialSummary(); break;
            }
            Input::waitForEnter();
        }
    }

    static void handleUserChoice(int choice) {
        switch (choice) {
        case 1: {
            string category = Validator::selectIncomeCategory();
            string description = Input::readText(YELLOW + "Income description: " + RESET);
            string note = Input::readText(YELLOW + "Note (optional, press Enter to skip): " + RESET, false);
            double amount = Input::readAmount(GREEN + "Income amount: " + RESET, 1e12);
            string date = Validator::validateDate();
            if (note.empty()) FinanceManager::addIncome(description, amount, date, category);
            else FinanceManager::addIncome(description, amount, date, category, note);
            Input::waitForEnter();
            break;
        }
        case 2: {
            string category = Validator::selectExpenseCategory();
            string description = Input::readText(YELLOW + "Expense description: " + RESET);
            double amount = Input::readAmount(GREEN + "Expense amount: " + RESET, 1e12);
            string date = Validator::validateDate();
            FinanceManager::addExpense(description, amount, date, category);
            Input::waitForEnter();
            break;
        }
        case 3: {
            string description = Input::readText(CYAN + "Description of the transaction to delete: " + RESET);
            FinanceManager::deleteTransaction(description);
            Input::waitForEnter();
            break;
        }
        case 4:
            screen::clear();
            FinanceManager::suggestInvestmentsAndOptions();
            Input::waitForEnter();
            break;
        case 5:
            handleStatsChoice();
            break;
        case 6:
            screen::clear();
            cout << "\033[1;31mExiting" << RESET;
            for (int i = 0; i < 5; ++i) {
                cout << "." << flush;
                pauseFor(100);
            }
            cout << "\n" << CYAN << "==================================" << RESET << endl;
            cout << "\033[1;32m  Thank you for using the Finance Manager" << RESET << endl;
            cout << "\033[1;33m  Developed by the Byte Masters team" << RESET << endl;
            cout << CYAN << "==================================" << RESET << endl;
            break;
        }
    }
};

int main() {
#ifdef _WIN32
    // Let the Windows console show the colours.
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(out, &mode)) SetConsoleMode(out, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    SetConsoleOutputCP(CP_UTF8);
#endif
    UserInterface::splash();
    int choice;
    do {
        screen::clear();
        UserInterface::displayMenu();
        choice = Input::readInt(GREEN + "Enter your choice: " + RESET, 1, 6);
        UserInterface::handleUserChoice(choice);
    } while (choice != 6);
    return 0;
}
