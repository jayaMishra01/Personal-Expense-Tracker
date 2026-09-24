
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <limits>

using namespace std;

// =========================
// Expense / Transaction Class
// =========================
class Transaction {
public:
    int id;
    string type;          // Income or Expense
    string category;
    double amount;
    string date;          // DD-MM-YYYY
    string description;

    Transaction() {}

    Transaction(int id, string type, string category,
                double amount, string date, string description) {
        this->id = id;
        this->type = type;
        this->category = category;
        this->amount = amount;
        this->date = date;
        this->description = description;
    }
};

// =========================
// Global Variables
// =========================
vector<Transaction> transactions;
const string FILE_NAME = "expenses.txt";

// =========================
// Generate New ID
// =========================
int generateID() {
    int maxID = 0;

    for (const auto &t : transactions) {
        if (t.id > maxID) {
            maxID = t.id;
        }
    }

    return maxID + 1;
}

// =========================
// Save Data to File
// =========================
void saveToFile() {

    ofstream file(FILE_NAME);

    if (!file) {
        cout << "\nError: Unable to save data!\n";
        return;
    }

    for (const auto &t : transactions) {
        file << t.id << "|"
             << t.type << "|"
             << t.category << "|"
             << t.amount << "|"
             << t.date << "|"
             << t.description << "\n";
    }

    file.close();
}

// =========================
// Load Data from File
// =========================
void loadFromFile() {

    ifstream file(FILE_NAME);

    if (!file) {
        return;
    }

    transactions.clear();

    string line;

    while (getline(file, line)) {

        stringstream ss(line);

        string idStr;
        string type;
        string category;
        string amountStr;
        string date;
        string description;

        getline(ss, idStr, '|');
        getline(ss, type, '|');
        getline(ss, category, '|');
        getline(ss, amountStr, '|');
        getline(ss, date, '|');
        getline(ss, description);

        if (!idStr.empty()) {

            Transaction t;

            t.id = stoi(idStr);
            t.type = type;
            t.category = category;
            t.amount = stod(amountStr);
            t.date = date;
            t.description = description;

            transactions.push_back(t);
        }
    }

    file.close();
}

// =========================
// Add Transaction
// =========================
void addTransaction(string type) {

    string category;
    string date;
    string description;
    double amount;

    cout << "\n===== Add " << type << " =====\n";

    cout << "Enter amount: Rs. ";
    cin >> amount;

    while (amount <= 0) {
        cout << "Amount must be greater than 0.\n";
        cout << "Enter amount again: Rs. ";
        cin >> amount;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter category: ";
    getline(cin, category);

    cout << "Enter date (DD-MM-YYYY): ";
    getline(cin, date);

    cout << "Enter description: ";
    getline(cin, description);

    int id = generateID();

    Transaction t(
        id,
        type,
        category,
        amount,
        date,
        description
    );

    transactions.push_back(t);

    saveToFile();

    cout << "\nTransaction added successfully!\n";
    cout << "Transaction ID: " << id << "\n";
}

// =========================
// View Transactions
// =========================
void viewTransactions() {

    if (transactions.empty()) {
        cout << "\nNo transactions found.\n";
        return;
    }

    cout << "\n================ ALL TRANSACTIONS ================\n";

    cout << left
         << setw(5) << "ID"
         << setw(12) << "Type"
         << setw(15) << "Category"
         << setw(12) << "Amount"
         << setw(15) << "Date"
         << "Description\n";

    cout << "---------------------------------------------------"
            "-------------------------\n";

    cout << fixed << setprecision(2);

    for (const auto &t : transactions) {

        cout << left
             << setw(5) << t.id
             << setw(12) << t.type
             << setw(15) << t.category
             << setw(12) << t.amount
             << setw(15) << t.date
             << t.description << "\n";
    }
}

// =========================
// Search Transaction
// =========================
void searchTransaction() {

    if (transactions.empty()) {
        cout << "\nNo transactions available.\n";
        return;
    }

    string keyword;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nEnter category or description to search: ";
    getline(cin, keyword);

    bool found = false;

    cout << "\n===== Search Results =====\n";

    for (const auto &t : transactions) {

        if (t.category.find(keyword) != string::npos ||
            t.description.find(keyword) != string::npos ||
            t.type.find(keyword) != string::npos) {

            cout << "\nID: " << t.id;
            cout << "\nType: " << t.type;
            cout << "\nCategory: " << t.category;
            cout << "\nAmount: Rs. " << fixed << setprecision(2) << t.amount;
            cout << "\nDate: " << t.date;
            cout << "\nDescription: " << t.description;
            cout << "\n-------------------------";

            found = true;
        }
    }

    if (!found) {
        cout << "\nNo matching transaction found.\n";
    }
}

// =========================
// Update Transaction
// =========================
void updateTransaction() {

    if (transactions.empty()) {
        cout << "\nNo transactions available.\n";
        return;
    }

    int id;

    cout << "\nEnter Transaction ID to update: ";
    cin >> id;

    for (auto &t : transactions) {

        if (t.id == id) {

            cout << "\nTransaction found.\n";

            cout << "Current Category: " << t.category << "\n";
            cout << "Enter new category: ";

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, t.category);

            cout << "Enter new amount: Rs. ";
            cin >> t.amount;

            while (t.amount <= 0) {
                cout << "Amount must be greater than 0.\n";
                cout << "Enter amount again: Rs. ";
                cin >> t.amount;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter new date (DD-MM-YYYY): ";
            getline(cin, t.date);

            cout << "Enter new description: ";
            getline(cin, t.description);

            saveToFile();

            cout << "\nTransaction updated successfully!\n";
            return;
        }
    }

    cout << "\nTransaction ID not found.\n";
}

// =========================
// Delete Transaction
// =========================
void deleteTransaction() {

    if (transactions.empty()) {
        cout << "\nNo transactions available.\n";
        return;
    }

    int id;

    cout << "\nEnter Transaction ID to delete: ";
    cin >> id;

    for (auto it = transactions.begin();
         it != transactions.end();
         ++it) {

        if (it->id == id) {

            transactions.erase(it);

            saveToFile();

            cout << "\nTransaction deleted successfully!\n";
            return;
        }
    }

    cout << "\nTransaction ID not found.\n";
}

// =========================
// Calculate Balance
// =========================
void checkBalance() {

    double totalIncome = 0;
    double totalExpense = 0;

    for (const auto &t : transactions) {

        if (t.type == "Income") {
            totalIncome += t.amount;
        }
        else if (t.type == "Expense") {
            totalExpense += t.amount;
        }
    }

    double balance = totalIncome - totalExpense;

    cout << "\n========== BALANCE ==========\n";

    cout << fixed << setprecision(2);

    cout << "Total Income  : Rs. " << totalIncome << "\n";
    cout << "Total Expense : Rs. " << totalExpense << "\n";
    cout << "-----------------------------\n";
    cout << "Remaining     : Rs. " << balance << "\n";
}

// =========================
// Monthly Summary
// =========================
void monthlySummary() {

    if (transactions.empty()) {
        cout << "\nNo transactions available.\n";
        return;
    }

    string month;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nEnter month (MM-YYYY): ";
    getline(cin, month);

    double income = 0;
    double expense = 0;

    bool found = false;

    for (const auto &t : transactions) {

        if (t.date.length() >= 10) {

            string transactionMonth =
                t.date.substr(3, 7);

            if (transactionMonth == month) {

                found = true;

                if (t.type == "Income") {
                    income += t.amount;
                }
                else if (t.type == "Expense") {
                    expense += t.amount;
                }
            }
        }
    }

    if (!found) {
        cout << "\nNo transactions found for " << month << ".\n";
        return;
    }

    cout << "\n========== MONTHLY SUMMARY ==========\n";

    cout << fixed << setprecision(2);

    cout << "Month          : " << month << "\n";
    cout << "Total Income   : Rs. " << income << "\n";
    cout << "Total Expense  : Rs. " << expense << "\n";
    cout << "Monthly Balance : Rs. "
         << income - expense << "\n";
}

// =========================
// Main Menu
// =========================
void showMenu() {

    cout << "\n\n========================================\n";
    cout << "       PERSONAL EXPENSE TRACKER\n";
    cout << "========================================\n";

    cout << "1. Add Income\n";
    cout << "2. Add Expense\n";
    cout << "3. View Transactions\n";
    cout << "4. Search Transaction\n";
    cout << "5. Update Transaction\n";
    cout << "6. Delete Transaction\n";
    cout << "7. Monthly Summary\n";
    cout << "8. Check Balance\n";
    cout << "9. Exit\n";

    cout << "========================================\n";
    cout << "Enter your choice: ";
}

// =========================
// Main Function
// =========================
int main() {

    loadFromFile();

    int choice;

    cout << "\nWelcome to Personal Expense Tracker!\n";

    do {

        showMenu();

        cin >> choice;

        switch (choice) {

            case 1:
                addTransaction("Income");
                break;

            case 2:
                addTransaction("Expense");
                break;

            case 3:
                viewTransactions();
                break;

            case 4:
                searchTransaction();
                break;

            case 5:
                updateTransaction();
                break;

            case 6:
                deleteTransaction();
                break;

            case 7:
                monthlySummary();
                break;

            case 8:
                checkBalance();
                break;

            case 9:
                cout << "\nThank you for using Personal Expense Tracker!\n";
                break;

            default:
                cout << "\nInvalid choice! Please try again.\n";
        }

    } while (choice != 9);

    return 0;
}
