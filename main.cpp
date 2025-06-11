#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>
#include <sstream>

using namespace std;

class Transaction {
public:
    string date;
    string type;
    string category;
    double amount;
    string note;

    Transaction(string d, string t, string c, double a, string n)
        : date(d), type(t), category(c), amount(a), note(n) {}

    string toString() const {
        return date + "," + type + "," + category + "," + to_string(amount) + "," + note;
    }

    static Transaction fromString(const string& line) {
        stringstream ss(line);
        string d, t, c, a, n;
        getline(ss, d, ',');
        getline(ss, t, ',');
        getline(ss, c, ',');
        getline(ss, a, ',');
        getline(ss, n);
        return Transaction(d, t, c, stod(a), n);
    }
};

// Holds monthly budget limits for each category
map<string, double> categoryBudget = {
    {"Food", 2000},
    {"Transport", 1500},
    {"Rent", 8000},
    {"Shopping", 3000},
    {"Other", 1000}
};

void showMenu() {
    cout << "\n===== Personal Finance Tracker =====\n";
    cout << "1. Add Transaction\n";
    cout << "2. View All Transactions\n";
    cout << "3. Show Summary\n";
    cout << "4. Filter by Month/Year\n";
    cout << "5. Exit\n";
    cout << "Choose an option: ";
}

void loadFromFile(vector<Transaction>& transactions) {
    ifstream file("transactions.txt");
    string line;
    while (getline(file, line)) {
        transactions.push_back(Transaction::fromString(line));
    }
    file.close();
}

void saveToFile(const vector<Transaction>& transactions) {
    ofstream file("transactions.txt");
    for (const auto& t : transactions) {
        file << t.toString() << endl;
    }
    file.close();
}

void showSummary(const vector<Transaction>& transactions) {
    double totalIncome = 0, totalExpense = 0;
    map<string, double> categoryExpense;

    for (const auto& t : transactions) {
        if (t.type == "Income") {
            totalIncome += t.amount;
        } else {
            totalExpense += t.amount;
            categoryExpense[t.category] += t.amount;
        }
    }

    cout << "\n--- Summary ---\n";
    cout << fixed << setprecision(2);
    cout << "Total Income  : ₹" << totalIncome << endl;
    cout << "Total Expense : ₹" << totalExpense << endl;
    cout << "Savings       : ₹" << (totalIncome - totalExpense) << endl;

    cout << "\nSpending by Category:\n";
    for (const auto& pair : categoryExpense) {
        cout << "  " << pair.first << ": ₹" << pair.second;

        // Budget alert
        if (categoryBudget.find(pair.first) != categoryBudget.end()) {
            double limit = categoryBudget[pair.first];
            if (pair.second > limit) {
                cout << " ⚠️  Exceeded budget limit of ₹" << limit;
            }
        }
        cout << endl;
    }
}

void filterByMonth(const vector<Transaction>& transactions) {
    string monthInput;
    cout << "Enter month and year (YYYY-MM): ";
    cin >> monthInput;

    cout << "\nTransactions for " << monthInput << ":\n";
    bool found = false;
    for (const auto& t : transactions) {
        if (t.date.substr(0, 7) == monthInput) {
            cout << t.date << " | " << t.type << " | " << t.category << " | ₹"
                 << fixed << setprecision(2) << t.amount << " | " << t.note << endl;
            found = true;
        }
    }
    if (!found) cout << "No transactions found for this month.\n";
}

int main() {
    vector<Transaction> transactions;
    loadFromFile(transactions);

    int choice;
    do {
        showMenu();
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string date, type, category, note;
            double amount;

            cout << "Enter date (YYYY-MM-DD): ";
            getline(cin, date);
            cout << "Type (Income/Expense): ";
            getline(cin, type);
            cout << "Category (e.g., Food, Rent): ";
            getline(cin, category);
            cout << "Amount (₹): ";
            cin >> amount;
            cin.ignore();
            cout << "Note: ";
            getline(cin, note);

            Transaction t(date, type, category, amount, note);
            transactions.push_back(t);
            saveToFile(transactions);
            cout << "Transaction added.\n";

        } else if (choice == 2) {
            cout << "\n--- All Transactions ---\n";
            for (const auto& t : transactions) {
                cout << t.date << " | " << t.type << " | " << t.category << " | ₹"
                     << fixed << setprecision(2) << t.amount << " | " << t.note << endl;
            }

        } else if (choice == 3) {
            showSummary(transactions);

        } else if (choice == 4) {
            filterByMonth(transactions);

        } else if (choice == 5) {
            cout << "Exiting. Stay financially smart!\n";

        } else {
            cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 5);

    return 0;
}
