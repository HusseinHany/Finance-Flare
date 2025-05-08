    #include <iostream>
    #include <string>
    #include <fstream>
    #include <vector>
    #include <iomanip>
    #include <thread>
    #include <chrono>
    #include <cstdlib>
    using namespace std;

    // Forward declaration of classes`
    class FinanceManager;
	// class to represent the type of record
    class RecordType {
    public:
        static const int income = 0;
        static const int expense = 1;
    };
	// class to represent the investment options
    class InvestmentUtil {
    public:
        static double getMonthlyROI(int optionNumber) {
            switch (optionNumber) {
            case 1: return 0.38;     // High-risk Investment
            case 2: return 0.36;     // Venture Capital
            case 3: return 0.28;     // Stock Market Investment
            case 4: return 0.26;     // Hedge Funds
            case 5: return 0.24;     // Medium-risk Investment
            case 6: return 0.20;     // Real Estate Investment
            case 7: return 0.18;     // Peer-to-Peer Lending
            case 8: return 0.14;     // Precious Metals (Gold/Silver)
            case 9: return 0.12;     // Low-risk Investment
            case 10: return 0.11;    // Cryptocurrency Investment
            case 11: return 0.098;   // Bonds
            case 12: return 0.09;    // Mutual Funds
            case 13: return 0.084;   // Art and Collectibles
            case 14: return 0.065;   // Savings Account
            case 15: return 0.06;    // Carbon Credit Investment
            case 16: return 0.036;   // Emergency Fund
            case 17: return 0.032;   // Diversified Portfolio
            case 18: return 0.03;    // Retirement Account
            case 19: return 0.02;    // Investment in Health
            case 20: return 0.012;   // Investment in Education
            default: return 0.01;    // Default 1%
            }
        }
    };
	// class to validate the input
    class Validator {
    public:
        static string selectIncomeCategory() {
            vector<string> categories = { "Salary", "Business", "Investment", "Gift", "Other" };

            cout << "\n\033[33m=== Select Income Category ===\033[0m\n";
            for (size_t i = 0; i < categories.size(); i++) {
                cout << "\033[33m" << i + 1 << ". " << "\033[0m" << categories[i] << endl;
            }

            int choice;
            cout << "Enter your choice (1-" << categories.size() << "): ";
            cin >> choice;

            while (choice < 1 || choice > static_cast<int>(categories.size()) || cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\033[31mInvalid choice. Please try again: \033[0m";
                cin >> choice;
            }

            return categories[choice - 1];
        }

        static string selectExpenseCategory() {
            vector<string> categories = { "Food", "Housing", "Transportation", "Entertainment",
                                       "Shopping", "Utilities", "Healthcare", "Education",
                                       "Personal", "Other" };

            cout << "\n\033[33m=== Select Expense Category ===\033[0m\n";
            for (size_t i = 0; i < categories.size(); i++) {
                cout << "\033[33m" << i + 1 << ". " << "\033[0m" << categories[i] << endl;
            }

            int choice;
            cout << "Enter your choise (1-" << categories.size() << "): ";
            cin >> choice;

            while (choice < 1 || choice > static_cast<int>(categories.size()) || cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\033[31mInvalid choice. Please try again : \033[0m";
                cin >> choice;
            }

            return categories[choice - 1];
        }

        static string validateDate() {
            int day, month, year;
            bool validDate = false;

            while (!validDate) {
                cout << "\033[33mEnter day (1-31): \033[0m";
                cin >> day;

                while (day < 1 || day > 31 || cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\033[31mInvalid day. Please enter a valuue between 1 and 31: \033[0m";
                    cin >> day;
                }

                cout << "\033[33mEnter month (1-12): \033[0m";
                cin >> month;

                while (month < 1 || month > 12 || cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\033[31mInvalid month. Please enter a value between 1 and 12: \033[0m";
                    cin >> month;
                }

                cout << "\033[33menter year (e.g., 2025): \033[0m";
                cin >> year;

                while (year < 2000 || year > 2030 || cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\033[31minvalid year. Please enter a value between 2000 and 2030 : \033[0m";
                    cin >> year;
                }

                bool validDaysInMonth = true;

                if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
                    validDaysInMonth = false;
                }
                else if (month == 2) {
                    bool isLeapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
                    if ((isLeapYear && day > 29) || (!isLeapYear && day > 28)) {
                        validDaysInMonth = false;
                    }
                }

                if (!validDaysInMonth) {
                    cout << "\033[31mInvalid date: The month you entered doesn't have that many days.\033[0m\n";
                    continue;
                }

                validDate = true;
            }

            string formattedDay = (day < 10) ? "0" + to_string(day) : to_string(day);
            string formattedMonth = (month < 10) ? "0" + to_string(month) : to_string(month);

            return formattedDay + "/" + formattedMonth + "/" + to_string(year);
        }
    };
	// class to represent the transaction 
    class Transaction {
    protected:
        string description;
        double amount;
        string date;
        string category;

    public:
        Transaction(string d, double a, string dt, string c) {
            description = d;
            amount = a;
            date = dt;
            category = c;
        }

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
	// class to represent the investment
    struct Investment {
        int optionNumber;
        int durationMonths;
        double amount;
        string date;

        Investment() : optionNumber(0), durationMonths(0), amount(0.0), date("") {}

        Investment(int opt, int dur, double amt, string dt) : optionNumber(opt), durationMonths(dur), amount(amt), date(dt) {}
    };
	// class to manage file operations
    class Filemanager {
    public:
        static const int MAX_INVESTMENTS = 100;
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

                    while ((pos = line.find(",")) != string::npos) {
                        token = line.substr(0, pos);
                        tokens.push_back(token);
                        line.erase(0, pos + 1);
                    }
                    tokens.push_back(line);

                    if (tokens.size() == 4) {
                        transactions.emplace_back(tokens[0], stod(tokens[1]), tokens[2], tokens[3]);
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
               cout << "\033[96m    ==== All Transactions ==== \033[0m" << endl;
               for (const auto& transaction : transactions) {
                   cout << "\033[92m" << "Transactions" << " | " << transaction.getDescription() << " | "
                        << transaction.getAmount() << " | " << transaction.getDate() << " | "
                        << transaction.getCategory() << "\033[0m" << endl;
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

        static int readInvestments(Investment investments[]) {
        ifstream file("investments.txt");
        int count = 0;
        if (file.is_open()) {
            string line;
            while (getline(file, line) && count < MAX_INVESTMENTS) {
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
                    investments[count] = Investment(
                        stoi(tokens[0]),    // optionNumber
                        stoi(tokens[1]),    // durationMonths
                        stod(tokens[2]),    // amount
                        tokens[3]           // date
                    );
                    count++;
                }
            }
            file.close();
        } else {
            cout << "\033[31mInvestments file not found or could not be opened!\033[0m" << endl;
        }
        return count;
    }

        static void viewInvestments() {
        Investment investments[MAX_INVESTMENTS];
        int count = readInvestments(investments);

        if (count == 0) {
            cout << "\033[33mNo investments found!\033[0m" << endl;
            return;
        }

        cout << "\033[96m\n==== Your Investments ====\n\033[0m";
        for (int i = 0; i < count; i++) {
            cout << "\033[33mInvestment Option: \033[0m" << investments[i].optionNumber << endl;
            cout << "\033[33mAmount Invested: \033[0m" << fixed << setprecision(2) << investments[i].amount << "$" << endl;
            cout << "\033[33mDuration: \033[0m" << investments[i].durationMonths << " months" << endl;
            cout << "\033[33mDate Invested: \033[0m" << investments[i].date << endl;

            double monthlyROI = InvestmentUtil::getMonthlyROI(investments[i].optionNumber);
            double expectedProfit = investments[i].amount * monthlyROI * investments[i].durationMonths;

            cout << "\033[32mExpected Profit: \033[0m" << fixed << setprecision(2) << expectedProfit << "$" << endl;
            cout << "\033[32mTotal Expected Value: \033[0m" << fixed << setprecision(2)
                 << (investments[i].amount + expectedProfit) << "" << endl;
            cout << "------------------------" << endl;
        }
    }

        static double calculateTotalInvestments() {
        Investment investments[MAX_INVESTMENTS];
        int count = readInvestments(investments);
        double totalInvested = 0.0;

        for (int i = 0; i < count; i++) {
            totalInvested += investments[i].amount;
        }
        return totalInvested;
    }
    };
	// class to represent the income transactions
    class Income : public Transaction {
    public:
        Income(string d, double a, string dt, string c) : Transaction(d, a, dt, c) {
            description = d;
            amount = a;
            date = dt;
            category = c;


        }

        void display() const {

            cout << "\033[92m" << "Income " << " | " << description << " | " << amount << "$" << " | " << date << " | " << category << "\033[0m" << endl;
        }

        void saveToFile() {
            ofstream file("income.txt", ios::app);
            if (file.is_open()) {
                file << description << "," << amount << "," << date << "," << category << endl;
                file.close();
            }
            else {
                cout << "\033[31mError opening file!\033[0m" << endl;
            }
        }

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
	// class to represent the expense transactions
    class Expense : public Transaction {
    public:
        Expense(string d, double a, string dt, string c) : Transaction(d, a, dt, c) {
            description = d;
            amount = a;
            date = dt;
            category = c;
        }

        void display() const {
            cout << "\033[92m" << "Expense " << " | " << description << " | " << amount << "$" << " | " << date << " | " << category << "\033[0m" << endl;
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
	// class to manage the finance transactions
    class FinanceManager {
    public:
        static void addIncome(string description, double amount, string date, string category) {
            Income income(description, amount, date, category);
            income.saveToFile();
            Filemanager::saveTransaction(income);
        }

        static void addExpense(string description, double amount, string date, string category) {
            Expense expense(description, amount, date, category);
            expense.saveToFile();
            Filemanager::saveTransaction(expense);
        }

        static void viewIncome() {
            vector<Income> incomes = Income::readFromFile();
            cout << "\033[96m    ==== Income Transaction ==== \033[0m" << endl;
            for (const auto& income : incomes) {
                income.display();
                cout << "----------------------------------" << endl;
            }
        }

        static void viewExpense() {
            vector<Expense> expenses = Expense::readFromFile();
            cout << "\033[96m    ==== Expense Transaction ==== \033[0m" << endl;
            for (const auto& expense : expenses) {
                expense.display();
                cout << "----------------------------------" << endl;
            }
        }

        static void displayInvestments() {
            Filemanager::viewInvestments();
        }

        static void viewAllTransactions() {
            Filemanager::displayTransactions();
        }

        static void deleteTransaction(string description) {
            Filemanager::deleteTransaction(description);

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
            }
            else {
                cout << "\033[31mError opening income file!\033[0m" << endl;
            }

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
            }
            else {
                cout << "\033[31mError opening expense file!\033[0m" << endl;
            }
        }

        static void suggestInvestmentsAndOptions() {
            double totalIncome = Income::calculateTotal();
            double totalExpenses = Expense::calculateTotal();
            double totalInvested = Filemanager::calculateTotalInvestments();
            double balance = totalIncome - totalExpenses;
            double availableBalance = balance - totalInvested;

            if (balance <= 0) {
                cout << "\033[31m\nYou currently don't have a positive balance for investment in your accounts.\n\033[0m";
                return;
            }

            const int colWidth = 22;
            cout << "\033[96m                          ========= Investment Suggestions =========\n\033[0m";

            cout << "\033[32m"
                << setw(colWidth) << "Total Income"
                << setw(colWidth) << "Total Expenses"
                << setw(colWidth) << "Invested Money"
                << setw(colWidth) << "Available Balance" << endl;

            cout << fixed << setprecision(2) << "\033[32m"
                << setw(colWidth) << ("$" + to_string(totalIncome).substr(0, to_string(totalIncome).find(".") + 3))
                << setw(colWidth) << ("$" + to_string(totalExpenses).substr(0, to_string(totalExpenses).find(".") + 3))
                << setw(colWidth) << ("$" + to_string(totalInvested).substr(0, to_string(totalInvested).find(".") + 3))
                << setw(colWidth) << ("$" + to_string(balance).substr(0, to_string(balance).find(".") + 3))
                << "\033[0m" << endl;
            cout << "                          ------------------------------------------\n";

            if (availableBalance < 10000) {
                  cout << "\033[90mLoading\033[0m";
        for (int i = 0; i < 5; ++i) {
            cout << ".";
            cout.flush();
            this_thread::sleep_for(chrono::milliseconds(500));
                 }
                cout << endl;
            cout << "1. Fractional Shares: $ -> " << "\033[92m(25%)\033[0m" << fixed << setprecision(2) << (availableBalance * 0.25) << endl;
            cout << "2. Micro-Investing Apps: $ -> " << "\033[92m(22%)\033[0m" << fixed << setprecision(2) << (availableBalance * 0.22) << endl;
            cout << "3. Cryptocurrency: $ -> " << "\033[93m(20%)\033[0m" << fixed << setprecision(2) << (availableBalance * 0.20) << endl;
            cout << "4. Peer-to-Peer Lending: $ -> " << "\033[93m(10%)\033[0m" << fixed << setprecision(2) << (availableBalance * 0.10) << endl;
            cout << "5. Robo-Advisors: $ -> " << "\033[93m(8%)\033[0m" << fixed << setprecision(2) << (availableBalance * 0.08) << endl;
            cout << "6. Real Estate Crowdfunding: $ -> " << "\033[33m(6%)\033[0m" << fixed << setprecision(2) << (availableBalance * 0.06) << endl;
            cout << "7. Dividend Stocks: $ -> " << "\033[33m(5%)\033[0m" << fixed << setprecision(2) << (availableBalance * 0.05) << endl;
            cout << "8. NFTs (Non-Fungible Tokens): $ -> " << "\033[33m(2%)\033[0m" << fixed << setprecision(2) << (availableBalance * 0.02) << endl;
            cout << "9. Collectibles: $ -> " << "\033[33m(2%)\033[0m" << fixed << setprecision(2) << (availableBalance * 0.02) << endl;
            cout << "10. Precious Metals: $ -> " << "\033[33m(5%)\033[0m" << fixed << setprecision(2) << (availableBalance * 0.05) << endl;
            cout << "\033[96m11. Return To Main Menu \033[0m" << fixed << setprecision(2) << endl;
	    }
            else {
                  cout << "\033[90mLoading\033[0m";
        for (int i = 0; i < 5; ++i) {
            cout << ".";
            cout.flush();
            this_thread::sleep_for(chrono::milliseconds(500));
                 }
                cout << endl;
                cout << "1. High-risk Investment: $ -> " << "\033[92m(38%)\033[0m" << fixed << setprecision(2) << endl;
                cout << "2. Venture Capital: $ -> " << "\033[92m(36%)\033[0m" << fixed << setprecision(2) << endl;
                cout << "3. Stock Market Investment: $ -> " << "\033[93m(28%)\033[0m" << fixed << setprecision(2) << endl;
                cout << "4. Hedge Funds: $ -> " << "\033[93m(26%)\033[0m" << fixed << setprecision(2) << endl;
                cout << "5. Medium-risk Investment: $ -> " << "\033[93m(24%)\033[0m" << fixed << setprecision(2) << endl;
                 this_thread::sleep_for(chrono::seconds(1));
                cout << "6. Real Estate Investment: $ -> " << "\033[33m(20%)\033[0m" << fixed << setprecision(2) << endl;
                cout << "7. Peer-to-Peer Lending: $ -> " << "\033[33m(18%)\033[0m" << fixed << setprecision(2) << endl;
                cout << "8. Emergency Fund: $ -> " << "\033[33m(14%%)\033[0m" << fixed << setprecision(2) << endl;
                cout << "9. Low-risk Investment: $ -> " << "\033[33m(12%)\033[0m" << fixed << setprecision(2) << endl;
                cout << "10. Cryptocurrency Investment: $ -> " << "\033[91m(11%)\033[0m" << fixed << setprecision(2) << endl;
                 this_thread::sleep_for(chrono::seconds(1));
                cout << "11. Bonds: $ -> " << "\033[91m(9.8%)\033[0m" << fixed << setprecision(2) << endl;
                cout << "12. Mutual Funds: $ -> " << "\033[91m(9%)\033[0m" << fixed << setprecision(2) << endl;
                cout << "13. Art and Collectibles: $ -> " << "\033[91m(8.4%)\033[0m" << fixed << setprecision(2) << endl;
                cout << "14. Savings Account: $ -> " << "\033[91m(6.5%)\033[0m" << fixed << setprecision(2) << endl;
                cout << "15. Carbon Credit Investment: $ -> " << "\033[91m(6%)\033[0m" << fixed << setprecision(2) << endl;
                 this_thread::sleep_for(chrono::seconds(1));
                cout << "16. Precious Metals (Gold/Silver): $ -> " << "\033[91m(3.6%)\033[0m" << fixed << setprecision(2) << endl;
                cout << "17. Diversified Portfolio: $ -> " << "\033[91m(3.2%)\033[0m" << fixed << setprecision(2) << endl;
                cout << "18. Retirement Account: $ -> " << "\033[91m(3%)\033[0m" << fixed << setprecision(2) << endl;
                cout << "19. Investment in Health: $ -> " << "\033[91m(2%)\033[0m" << fixed << setprecision(2) << endl;
                cout << "20. Investment in Education: $ -> " << "\033[91m(1.2%)\033[0m" << fixed << setprecision(2) << endl;
                cout << "\033[96m21. Return To Main Menu \033[0m" << fixed << setprecision(2) << endl;

	    }

            int x, m;
            double bal;
            string date;

            cout << "\033[92mChoose What Is the Most Suitable case For You: \n\033[0m";
            cin >> x;
             while (x < 1 || x > 21 || cin.fail()) {
	        cin.clear();
	        cin.ignore(numeric_limits<streamsize>::max(), '\n');
	        cout << "\033[31mInvalid choice. Please try again: \033[0m";
             }
        if (x == 21 || x == 11) {
            cout << "\033[33mReturning to main menu...\033[0m" << endl;
            this_thread::sleep_for(chrono::seconds(1));
            return;
        }
            cout << "Please Enter The Amount You Want To Invest\n";
            cin >> bal;

            while (bal > availableBalance || bal <= 0) {
                if (bal <= 0) {
                    cout << "\033[31mInvestment amount must be greater than zero! Please enter a valid amount: \033[0m";
                }
                else {
                    cout << "\033[31mYou cannot invest more than your available balance of $"
                        << fixed << setprecision(2) << availableBalance
                        << " Please enter a valid amount: \033[0m";
                }
                cin >> bal;
            }

            cout << "Please Enter The Duration In Months\n";
            cin >> m;
            date = Validator::validateDate();

            Filemanager::saveInvestment(x, m, bal, date);

            cout << "You Have Chosen Investment Option " << x << " with an amount of $" << fixed << setprecision(2) << bal << " for a duration of " << m << " months." << endl;
            this_thread::sleep_for(chrono::seconds(1)); // 1 seconds delay
            cout << "Your investment will be monitored and adjusted based on market conditions." << endl;
             this_thread::sleep_for(chrono::seconds(2));//  2 seconds delay
            cout << "Please wait while we process your investment...\n";
            this_thread::sleep_for(chrono::seconds(1)); // 2 seconds delay
            cout << "Investment processed successfully!" << endl;

            switch (x) {
            case 1: {
                double roi = InvestmentUtil::getMonthlyROI(1);
                double profit = bal * roi * m;
                cout << "High-risk Investment: $" << fixed << setprecision(2) << bal << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 2: {
                double roi = InvestmentUtil::getMonthlyROI(2);
                double profit = bal * roi * m;
                cout << "Low-risk Investment: $" << fixed << setprecision(2) << bal << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 3: {
                double roi = InvestmentUtil::getMonthlyROI(3);
                double profit = bal * roi * m;
                cout << "Medium-risk Investment: $" << fixed << setprecision(2) << bal << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 4: {
                double roi = InvestmentUtil::getMonthlyROI(4);
                double profit = bal * roi * m;
                cout << "Cryptocurrency Investment: $" << fixed << setprecision(2) << bal << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 5: {
                double roi = InvestmentUtil::getMonthlyROI(5);
                double profit = bal * roi * m;
                cout << "Diversified Portfolio: $" << fixed << setprecision(2) << bal << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 6: {
                double roi = InvestmentUtil::getMonthlyROI(6);
                double profit = bal * roi * m;
                cout << "Savings Account: $" << fixed << setprecision(2) << bal << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 7: {
                double roi = InvestmentUtil::getMonthlyROI(7);
                double profit = bal * roi * m;
                cout << "Retirement Account: $" << fixed << setprecision(2) << bal << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 8: {
                double roi = InvestmentUtil::getMonthlyROI(8);
                double profit = bal * roi * m;
                cout << "Emergency Fund: $" << fixed << setprecision(2) << bal << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 9: {
                double roi = InvestmentUtil::getMonthlyROI(9);
                double profit = bal * roi * m;
                cout << "Investment in Education: $" << fixed << setprecision(2) << bal << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 10: {
                double roi = InvestmentUtil::getMonthlyROI(10);
                double profit = bal * roi * m;
                cout << "Investment in Health: $" << fixed << setprecision(2) << bal << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 11: {
                double roi = InvestmentUtil::getMonthlyROI(11);
                double profit = bal * roi * m;
                cout << "Real Estate Investment: $" << fixed << setprecision(2) << bal << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 12: {
                double roi = InvestmentUtil::getMonthlyROI(12);
                double profit = bal * roi * m;
                cout << "Bonds: $" << fixed << setprecision(2) << bal << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 13: {
                double roi = InvestmentUtil::getMonthlyROI(13);
                double profit = bal * roi * m;
                cout << "Mutual Funds: $" << fixed << setprecision(2) << bal << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 14: {
                double roi = InvestmentUtil::getMonthlyROI(14);
                double profit = bal * roi * m;
                cout << "Stock Market Investment: $" << fixed << setprecision(2) << bal << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 15: {
                double roi = InvestmentUtil::getMonthlyROI(15);
                double profit = bal * roi * m;
                cout << "Precious Metals (Gold/Silver): $" << fixed << setprecision(2) << bal << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 16: {
                double roi = InvestmentUtil::getMonthlyROI(16);
                double profit = bal * roi * m;
                cout << "Peer-to-Peer Lending: $" << fixed << setprecision(2) << bal << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 17: {
                double roi = InvestmentUtil::getMonthlyROI(17);
                double profit = bal * roi * m;
                cout << "Hedge Funds: $" << fixed << setprecision(2) << bal << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 18: {
                double roi = InvestmentUtil::getMonthlyROI(18);
                double profit = bal * roi * m;
                cout << "Venture Capital: $" << fixed << setprecision(2) << bal << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 19: {
                double roi = InvestmentUtil::getMonthlyROI(19);
                double profit = bal * roi * m;
                cout << "Art and Collectibles: $" << fixed << setprecision(2) << bal << endl;
                this_thread::sleep_for(chrono::seconds(1));
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 20: {
                double roi = InvestmentUtil::getMonthlyROI(20);
                double profit = bal * roi * m;
                cout << "Carbon Credit Investment: $" << fixed << setprecision(2) << bal << endl;
                cout << "Expected Profit (for " << m << " months): $" << fixed << setprecision(2) << profit << endl;
                break;
            }
            case 21: {
	            cout << "\033[33mReturning to Main Menu...\033[0m" << endl;
	            this_thread::sleep_for(chrono::seconds(1)); // 1 seconds delay
	             break;
            }
            default:
                cout << "Invalid choice! Please try again." << endl;
                break;
            }
        }

        static void displayFinancialSummary() {
            double totalIncome = Income::calculateTotal();
            double totalExpenses = Expense::calculateTotal();
            double totalInvested = Filemanager::calculateTotalInvestments();
            double balance = totalIncome - totalExpenses;
            double availableBalance = balance - totalInvested;

            Investment investments[Filemanager::MAX_INVESTMENTS];
            int count = Filemanager::readInvestments(investments);
            double totalExpectedProfit = 0.0;
            double totalExpectedValue = 0.0;

            for (const auto& investment : investments) {
                double monthlyROI = InvestmentUtil::getMonthlyROI(investment.optionNumber);
                double profit = investment.amount * monthlyROI * investment.durationMonths;
                totalExpectedProfit += profit;
                totalExpectedValue += (investment.amount + profit);
            }

            cout << "       \n\033[96m==========  Financial Summary ========== \033[0m\n\n";
            cout << "\033[32mTotal Income: \033[0m" << fixed << setprecision(2) << totalIncome << "$" << endl;
            cout << "\033[31mTotal Expenses: \033[0m" << fixed << setprecision(2) << totalExpenses << "$" << endl;
            cout << "\033[33mTotal Money Invested: \033[0m" << fixed << setprecision(2) << totalInvested << "$" << endl;
            cout << "\033[33mAvailable Balance: \033[0m" << fixed << setprecision(2) << availableBalance << "$" << endl;
            cout << "\033[33mTotal Expected Profit: \033[0m" << fixed << setprecision(2) << totalExpectedProfit << "$" << endl;
            cout << "\033[32mTotal Expected Value: \033[0m" << fixed << setprecision(2) << totalExpectedValue << "$" << endl;

            cout << "       \n\033[96m========== Financial Distribution ========== \033[0m\n";
            cout << "\n";
            double total = totalIncome;
            if (total > 0) {
                double expensePercentage = (totalExpenses / total) * 100;
                double investedPercentage = (totalInvested / total) * 100;
                double availablePercentage = (availableBalance / total) * 100;

                cout << "\033[31mExpenses: \033[0m" << fixed << setprecision(1) << expensePercentage << " % ";
                int barLength = static_cast<int>(expensePercentage / 2);
                for (int i = 0; i < barLength; i++) cout << "-";
                cout << ">";
                cout << endl;

                cout << "\033[33mInvested: \033[0m" << fixed << setprecision(1) << investedPercentage << " % ";
                barLength = static_cast<int>(investedPercentage / 2);
                for (int i = 0; i < barLength; i++) cout << "-";
                cout << ">";
                cout << endl;

                cout << "\033[32mAvailable: \033[0m" << fixed << setprecision(1) << availablePercentage << " % ";
                barLength = static_cast<int>(availablePercentage / 2);
                for (int i = 0; i < barLength; i++) cout << "-";
                cout << ">";
                cout << endl;
            }

            cout << "\n\033[1;36m=====================================\033[0m\n";
        }
    };
	// class to clear the screen
    class screen {
    public:
        static void clear() {
            system("cls");
        }
    };
	// class for the user interface
    class UserInterface {
    public:
        static void displayMenu() {
            cout << "==================================" << endl;
            cout << "\033[92m	Byte Masters\033[0m" << endl;
            cout << "\033[32mWelcome to the Finance Manager\033[0m" << endl;
            cout << "==================================" << endl;
            cout << "\033[96m1. Add Income\033[0m" << endl;
            cout << "\033[96m2. Add Expense\033[0m" << endl;
            cout << "\033[96m3. Delete Transaction\033[0m" << endl;
            cout << "\033[96m4. Investment Suggestions\033[0m" << endl;
            cout << "\033[96m5. Stats & Reports\033[0m" << endl;
            cout << "\033[31m6. Exit\033[0m" << endl;
        }

        static void displayLoading() {
            cout << "\033[33mLoading...\033[0m" << endl;
            for (int i = 0; i < 3; i++) {
                cout << "\033[33m.\033[0m";
                this_thread::sleep_for(chrono::milliseconds(500));
            }
            cout << endl;
        }

        static void displayStatsMenu() {
            cout << "==================================" << endl;
            cout << "\033[92m     Finance Statistics Menu\033[0m" << endl;
            cout << "==================================" << endl;
            cout << "\033[96m1. View Income\033[0m" << endl;
            cout << "\033[96m2. View Expense\033[0m" << endl;
            cout << "\033[96m3. View All Transactions\033[0m" << endl;
            cout << "\033[96m4. View Investments\033[0m" << endl;
            cout << "\033[96m5. Financial Summary\033[0m" << endl;
            cout << "\033[96m6. Return to Main Menu\033[0m" << endl;
            cout << "\033[32mEnter your choice: \033[0m";
        }

        static void handleStatsChoice() {
            int statsChoice;
            do {
                screen::clear();
                displayStatsMenu();
                cin >> statsChoice;

                switch (statsChoice) {
                case 1:
                    screen::clear();
                    FinanceManager::viewIncome();
                    system("pause");
                    break;
                case 2:
                    screen::clear();
                    FinanceManager::viewExpense();
                    system("pause");
                    break;
                case 3:
                    screen::clear();
                    FinanceManager::viewAllTransactions();
                    system("pause");
                    break;
                case 4:
                    screen::clear();
                    FinanceManager::displayInvestments();
                    system("pause");
                    break;
                case 5:
                    screen::clear();
                    FinanceManager::displayFinancialSummary();
                    system("pause");
                    break;
                case 6:
                    cout << "\033[33mReturning to the main menu...\033[0m" << endl;
                    this_thread::sleep_for(chrono::milliseconds(500));
                    break;
                default:
                    cout << "\033[31mInvalid choice! Please try again.\033[0m" << endl;
                    system("pause");
                    break;
                }
            } while (statsChoice != 6);
        }

        static void handleUserChoice(int choice) {
            switch (choice) {
            case 1: {
                string description, date, category;
                double amount;

                category = Validator::selectIncomeCategory();

                cout << "\033[33mEnter income description: \033[0m";
                cin.ignore();
                getline(cin, description);

                cout << "\033[32mEnter income amount: \033[0m";
                cin >> amount;

                date = Validator::validateDate();

                FinanceManager::addIncome(description, amount, date, category);
                break;
            }
            case 2: {
                string description, date, category;
                double amount;

                category = Validator::selectExpenseCategory();

                cout << "\033[33mEnter expense description: \033[0m";
                cin.ignore();
                getline(cin, description);

                cout << "\033[32mEnter expense amount: \033[0m";
                cin >> amount;

                date = Validator::validateDate();

                FinanceManager::addExpense(description, amount, date, category);
                break;
            }
            case 3: {
                string description;
                cout << "\033[36mEnter the description of the transaction to delete: \033[0m";
                cin.ignore();
                getline(cin, description);
                FinanceManager::deleteTransaction(description);
                break;
            }
            case 4:
                screen::clear();
                FinanceManager::suggestInvestmentsAndOptions();
                system("pause");
                break;
            case 5:
                handleStatsChoice();
                break;
            case 6:
                screen::clear();
                
                   cout << "\033[1;31mExiting \033[0m";
                   for (int i = 0; i < 5; ++i) {
                      cout << ".";
                            cout.flush();
                           this_thread::sleep_for(chrono::milliseconds(100));
                   }
                    cout << endl;
                    cout << "\033[1;32mThank you for using the \033[1;36mFinance Manager\033[0m" << endl;
                    this_thread::sleep_for(chrono::milliseconds(900));
                    cout << "\033[1;32mThis code is developed by \033[1;33mByte Masters Team\033[0m" << endl;
                 this_thread::sleep_for(chrono::milliseconds(900));

                break;


            default:
                cout << "\033[31mInvalid choice! Please try again.\033[0m" << endl;
                break;
            }
        }
    };
	// main function
    int main() {
        int choice;
        do {
            screen::clear();
            UserInterface::displayMenu();
            cout << "\033[32mEnter your choice: \033[0m";
            cin >> choice;
            UserInterface::handleUserChoice(choice);
        } while (choice != 6);
        return 0;
    }
