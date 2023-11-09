#include <iostream>
#include <string> 
#include <windows.h>
#include <fstream>
#include <vector>
#include <cctype>
#include <cstring>
#include <iomanip>
using namespace std;

struct Transaction {
    string description;
    int amount;

    Transaction(const string& desc, int amt) : description(desc), amount(amt) {}
};

struct Account {
    int Account_No;
    char Account_Holder_Name[44];
    char type;
    int Deposit_Amount;
    vector<Transaction> transactions; 

    Account(int accNo, const char* name, char acctType, int depositAmount)
        : Account_No(accNo), type(acctType), Deposit_Amount(depositAmount) {
        strncpy(Account_Holder_Name, name, sizeof(Account_Holder_Name));
    }
};

class Bank {
    vector<Account> accounts;

public:
    void Open_account();
    void Display_account(int accountNumber) const;
    void Update(int accountNumber);
    void Deposit(int accountNumber);
    void Withdraw(int accountNumber);
    void Report() const;
    void Delete_account(int accountNumber);
    void Passbook(int accountNumber);
};

void Bank::Open_account() {
    int accNo, depositAmount;
    char name[44], acctType;

    cout << "\nPlease! Enter The account No. : ";
    cin >> accNo;

    for (const auto& acc : accounts) {
        if (acc.Account_No == accNo) {
            cout << "\nAccount with the same ID already exists. Cannot create a new account.";
            return;
        }
    }

    cout << "\nPlease! Enter The Name of The account Holder : ";
    cin.ignore();
    cin.getline(name, 44);
    cout << "\nPlease! Enter Type of The account (C/S) : ";
    cin >> acctType;
    acctType = toupper(acctType);

    cout << "\nPlease! Enter The Initial amount: ";
    cin >> depositAmount;

    if ((acctType == 'S' && depositAmount < 440) || (acctType == 'C' && depositAmount < 1000)) {
        cout << "\nInitial deposit amount is insufficient for the selected account type.";
    } else {
        Account newAccount(accNo, name, acctType, depositAmount);

        
        Transaction initialDeposit("Deposit", depositAmount);
        newAccount.transactions.push_back(initialDeposit);

        accounts.push_back(newAccount);
        cout << "\nAccount Created.";
    }
}

void Bank::Display_account(int accountNumber) const {
    for (const auto& acc : accounts) {
        if (acc.Account_No == accountNumber) {
            cout << "\nAccount No. : " << acc.Account_No;
            cout << "\nAccount Holder Name : " << acc.Account_Holder_Name;
            cout << "\nType of Account : " << acc.type;
            cout << "\nBalance amount : " << acc.Deposit_Amount;
            return;
        }
    }
    cout << "\nAccount number does not exist";
}

void Bank::Update(int accountNumber) {
    for (auto& acc : accounts) {
        if (acc.Account_No == accountNumber) {
            cout << "\nAccount No. : " << acc.Account_No;
            cout << "\nUpdate Account Holder Name : ";
            cin.ignore();
            cin.getline(acc.Account_Holder_Name, 44);
            cout << "\nUpdate Type of Account : ";
            cin >> acc.type;
            acc.type = toupper(acc.type);
            cout << "\nUpdate Balance amount : ";
            cin >> acc.Deposit_Amount;
            cout << "\nRecord Updated";
            return;
        }
    }
    cout << "\nRecord Not Found";
}

void Bank::Deposit(int accountNumber) {
    bool accountExists = false;
    for (auto& acc : accounts) {
        if (acc.Account_No == accountNumber) {
            int depositAmount;
            cout << "\nPlease enter the amount to be Deposited: ";
            cin >> depositAmount;
            acc.Deposit_Amount += depositAmount;
            Transaction depositTransaction("Deposit", depositAmount);
            acc.transactions.push_back(depositTransaction);

            cout << "\nAmount Deposited.";
            accountExists = true;
            break;
        }
    }
    if (!accountExists) {
        cout << "\nAccount number does not exist. Cannot deposit money.";
    }
}

void Bank::Withdraw(int accountNumber) {
    for (auto& acc : accounts) {
        if (acc.Account_No == accountNumber) {
            int withdrawAmount;
            cout << "\nPlease enter the amount to be Withdrawn: ";
            cin >> withdrawAmount;
            int balance = acc.Deposit_Amount - withdrawAmount;
            if ((balance < 440 && acc.type == 'S') || (balance < 1000 && acc.type == 'C')) {
                cout << "Insufficient balance";
            } else {
                acc.Deposit_Amount = balance;

                Transaction withdrawalTransaction("Withdrawal", withdrawAmount);
                acc.transactions.push_back(withdrawalTransaction);

                cout << "\nAmount Withdrawn.";
            }
            return;
        }
    }
    cout << "\nAccount number does not exist";
}

void Bank::Report() const {
    cout << "A/c no." << setw(15) << "NAME" << setw(20) << "Type" << setw(15) << "Balance\n";
    cout << "****************************************************\n";

    for (const auto& acc : accounts) {
        cout << setw(7) << acc.Account_No << setw(20) << acc.Account_Holder_Name << setw(10) << acc.type << setw(15) << acc.Deposit_Amount << endl;
    }
}


void Bank::Delete_account(int accountNumber) {
    auto it = accounts.begin();
    while (it != accounts.end()) {
        if (it->Account_No == accountNumber) {
            it = accounts.erase(it);
            cout << "\nRecord Deleted.";
            return;
        } else {
            ++it;
        }
    }
    cout << "\nRecord Not Found";
}

void Bank::Passbook(int accountNumber) {
    bool accountExists = false;
    for (const auto& acc : accounts) {
        if (acc.Account_No == accountNumber) {
            accountExists = true;
            cout << "Passbook for Account No. " << acc.Account_No << " - " << acc.Account_Holder_Name << "\n";
            cout << "--------------------------------------------\n";

            int currentBalance = 0;

            cout << setw(4) << "S.No" << setw(20) << "Description" << setw(10) << "Amount" << setw(15) << "Balance" << "\n";
            cout << "--------------------------------------------\n";

            int serialNumber = 1;
            for (const auto& transaction : acc.transactions) {
                if (transaction.description == "Deposit") {
                    currentBalance += transaction.amount;
                } else if (transaction.description == "Withdrawal") {
                    currentBalance -= transaction.amount;
                }
                cout << setw(4) << serialNumber << setw(20) << transaction.description << setw(10) << transaction.amount << setw(15) << currentBalance << "\n";
                serialNumber++;
            }

            cout << "--------------------------------------------\n";
            break;
        }
    }
    if (!accountExists) {
        cout << "\nAccount number does not exist. Cannot generate a passbook.";
    }
}


int main() {
    Bank bank;
    char ch;
    int num;
    string S = "****BANK MANAGEMENT SYSTEM****";
    int padding = (80 - S.length()) / 2; 

    for (int i = 0; i < padding; i++) {
        cout << " ";  
    }

    for (int i = 0; i < S.length(); i++) {
        cout << S[i];
        Sleep(100);   
    }
    cout << endl;

    do {
        system("cls");

        cout << "\n\n\t\tMAIN MENU";
        cout << "\n\t\t01. NEW ACCOUNT";
        cout << "\n\t\t02. Deposit AMOUNT";
        cout << "\n\t\t03. Withdraw AMOUNT";
        cout << "\n\t\t04. BALANCE ENQUIRY";
        cout << "\n\t\t05. ALL ACCOUNT HOLDER LIST";
        cout << "\n\t\t06. CLOSE AN ACCOUNT";
        cout << "\n\t\t07. Update AN ACCOUNT";
        cout << "\n\t\t08. Generate Passbook";
        cout << "\n\t\t09. EXIT";
        cout << "\n\t\tSelect Your Option (1-9) ";
        cin >> ch;
        system("cls");

        switch (ch) {
            case '1':
                bank.Open_account();
                break;
            case '2':
                cout << "\n\t\tPlease! Enter The account No. : ";
                cin >> num;
                bank.Deposit(num);
                break;
            case '3':
                cout << "\n\t\tPlease! Enter The account No. : ";
                cin >> num;
                bank.Withdraw(num);
                break;
            case '4':
                cout << "\n\t\tPlease! Enter The account No. : ";
                cin >> num;
                bank.Display_account(num);
                break;
            case '5':
                bank.Report();
                break;
            case '6':
                cout << "\n\t\tPlease! Enter The account No. : ";
                cin >> num;
                bank.Delete_account(num);
                break;
            case '7':
                cout << "\n\t\tPlease! Enter The account No. : ";
                cin >> num;
                bank.Update(num);
                break;
            case '8':
                cout << "\n\t\tPlease! Enter The account No. : ";
                cin >> num;
                bank.Passbook(num);
                break;
            case '9':
                for (char c : "Thanks for using the bank management system") {
                    cout << c;
                    cout.flush();
                    Sleep(100);
                }
                break;
            default:
                cout << "\a";
                break;
        }
        cin.ignore();
        cin.get();
    } while (ch != '9');
    return 0;
}

