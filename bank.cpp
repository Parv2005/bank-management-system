#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class Account {
private:
    int accNo;
    char name[50];
    char type;
    double balance;

public:
    void createAccount() {
        cout << "\nEnter The Account No.: ";
        cin >> accNo;
        cout << "\n\nEnter The Name of The Account Holder: ";
        cin.ignore();
        cin.getline(name, 50);
        cout << "\nEnter Type of The Account (C/S): ";
        cin >> type;
        type = toupper(type);
        cout << "\nEnter The Initial amount (>=500 for Saving and >=1000 for Current ): ";
        cin >> balance;
        cout << "\n\nAccount Created..";
    }

    void showAccount() const {
        cout << "\nAccount No.: " << accNo;
        cout << "\nAccount Holder Name: " << name;
        cout << "\nType of Account: " << type;
        cout << "\nBalance amount: " << balance;
    }

    void modify() {
        cout << "\nAccount No.: " << accNo;
        cout << "\nModify Account Holder Name: ";
        cin.ignore();
        cin.getline(name, 50);
        cout << "\nModify Type of Account: ";
        cin >> type;
        type = toupper(type);
        cout << "\nModify Balance amount: ";
        cin >> balance;
    }

    void deposit(double amt) {
        balance += amt;
    }

    void withdraw(double amt) {
        balance -= amt;
    }

    int getAccNo() const {
        return accNo;
    }

    double getBalance() const {
        return balance;
    }

    char getType() const {
        return type;
    }
};

// Function declarations
void writeAccount();
void displayAccount(int);
void modifyAccount(int);
void deleteAccount(int);
void displayAll();
void depositWithdraw(int, int);

int main() {
    int choice;
    int num;

    do {
        cout << "\n\n\t*** BANK MANAGEMENT SYSTEM ***";
        cout << "\n\n\t1. New Account";
        cout << "\n\t2. Deposit Amount";
        cout << "\n\t3. Withdraw Amount";
        cout << "\n\t4. Balance Enquiry";
        cout << "\n\t5. All Account Holder List";
        cout << "\n\t6. Close An Account";
        cout << "\n\t7. Modify An Account";
        cout << "\n\t8. Exit";
        cout << "\n\n\tSelect Your Option (1-8): ";
        cin >> choice;

        switch(choice) {
            case 1: writeAccount(); break;
            case 2: 
                cout << "\n\nEnter The account No. : "; cin >> num;
                depositWithdraw(num, 1); 
                break;
            case 3: 
                cout << "\n\nEnter The account No. : "; cin >> num;
                depositWithdraw(num, 2); 
                break;
            case 4:
                cout << "\n\nEnter The account No. : "; cin >> num;
                displayAccount(num); 
                break;
            case 5: displayAll(); break;
            case 6:
                cout << "\n\nEnter The account No. : "; cin >> num;
                deleteAccount(num);
                break;
            case 7:
                cout << "\n\nEnter The account No. : "; cin >> num;
                modifyAccount(num);
                break;
            case 8: cout << "\n\nThanks for using bank management system!\n"; break;
            default: cout << "\a";
        }
    } while(choice != 8);

    return 0;
}

// Function definitions

void writeAccount() {
    Account acc;
    ofstream outFile;
    acc.createAccount();
    outFile.open("account.dat", ios::binary | ios::app);
    outFile.write(reinterpret_cast<char *> (&acc), sizeof(Account));
    outFile.close();
}

void displayAccount(int n) {
    Account acc;
    bool found = false;
    ifstream inFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be open! Press any Key...";
        return;
    }
    while (inFile.read(reinterpret_cast<char *> (&acc), sizeof(Account))) {
        if (acc.getAccNo() == n) {
            acc.showAccount();
            found = true;
        }
    }
    inFile.close();
    if (!found)
        cout << "\n\nAccount number does not exist";
}

void modifyAccount(int n) {
    bool found = false;
    Account acc;
    fstream File;
    File.open("account.dat", ios::binary | ios::in | ios::out);
    if (!File) {
        cout << "File could not be open!";
        return;
    }

    while (!File.eof() && !found) {
        int pos = File.tellg();
        File.read(reinterpret_cast<char *> (&acc), sizeof(Account));
        if (acc.getAccNo() == n) {
            acc.showAccount();
            cout << "\n\nEnter New Details of account" << endl;
            acc.modify();
            File.seekp(pos);
            File.write(reinterpret_cast<char *> (&acc), sizeof(Account));
            cout << "\n\n\t Record Updated";
            found = true;
        }
    }
    File.close();
    if (!found)
        cout << "\n\n Record Not Found ";
}

void deleteAccount(int n) {
    Account acc;
    ifstream inFile;
    ofstream outFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be open!";
        return;
    }
    outFile.open("Temp.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char *> (&acc), sizeof(Account))) {
        if (acc.getAccNo() != n) {
            outFile.write(reinterpret_cast<char *> (&acc), sizeof(Account));
        }
    }
    inFile.close();
    outFile.close();
    remove("account.dat");
    rename("Temp.dat", "account.dat");
    cout << "\n\n\tRecord Deleted...";
}

void displayAll() {
    Account acc;
    ifstream inFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be open!";
        return;
    }
    cout << "\n\n\t\tACCOUNT HOLDER LIST\n\n";
    cout << "====================================================\n";
    cout << "A/c no.      NAME           Type  Balance\n";
    cout << "====================================================\n";
    while (inFile.read(reinterpret_cast<char *> (&acc), sizeof(Account))) {
        cout << acc.getAccNo() << setw(15) << acc.getType() << setw(10)
             << acc.getBalance() << endl;
    }
    inFile.close();
}

void depositWithdraw(int n, int option) {
    double amt;
    bool found = false;
    Account acc;
    fstream File;
    File.open("account.dat", ios::binary | ios::in | ios::out);
    if (!File) {
        cout << "File could not be open!";
        return;
    }

    while (!File.eof() && !found) {
        int pos = File.tellg();
        File.read(reinterpret_cast<char *> (&acc), sizeof(Account));
        if (acc.getAccNo() == n) {
            acc.showAccount();
            if (option == 1) {
                cout << "\n\nTo Deposit amount: ";
                cin >> amt;
                acc.deposit(amt);
            }
            if (option == 2) {
                cout << "\n\nTo Withdraw amount: ";
                cin >> amt;
                double bal = acc.getBalance() - amt;
                if ((bal < 500 && acc.getType() == 'S') || (bal < 1000 && acc.getType() == 'C'))
                    cout << "Insufficient balance";
                else
                    acc.withdraw(amt);
            }
            File.seekp(pos);
            File.write(reinterpret_cast<char *> (&acc), sizeof(Account));
            cout << "\n\n\t Record Updated";
            found = true;
        }
    }
    File.close();
    if (!found)
        cout << "\n\n Record Not Found ";
}
