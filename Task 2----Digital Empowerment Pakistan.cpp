#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <windows.h> 

using namespace std;

class Contact {
public:
    string name;
    string phoneNumber;
    Contact(string n, string p) : name(n), phoneNumber(p) {}
};

class ContactManager {
private:
    map<string, string> credentials;
    vector<Contact> contacts;
    string email;
    string password;

    void pressEnterToContinue() {
        cout << "\nPress Enter to continue...";
        cin.get();
    }

    void loadCredentials() {
        ifstream infile("credentials.txt");
        string storedEmail, storedPassword;
        while (getline(infile, storedEmail)) {
            getline(infile, storedPassword);
            credentials[storedEmail] = storedPassword;
        }
        infile.close();
    }

    bool authenticate() {
        loadCredentials();
        if (credentials.find(email) != credentials.end() && credentials[email] == password) {
            return true;
        } else {
            printCentered("Invalid email or password.");
            return false;
        }
    }

    void setCredentials() {
        cout << "Set your email: ";
        getline(cin, email);
        cout << "Set your password: ";
        getline(cin, password);
        credentials[email] = password;
        ofstream outfile("credentials.txt", ios::app);
        outfile << email << endl;
        outfile << password << endl;
        outfile.close();
    }

    void saveContactsToFile() {
        ofstream outfile((email + "_contacts.txt").c_str());
        for (size_t i = 0; i < contacts.size(); ++i) {
            outfile << contacts[i].name << endl;
            outfile << contacts[i].phoneNumber << endl;
        }
        outfile.close();
    }

    void loadContactsFromFile() {
        ifstream infile((email + "_contacts.txt").c_str());
        string name, phoneNumber;
        while (getline(infile, name)) {
            getline(infile, phoneNumber);
            contacts.push_back(Contact(name, phoneNumber));
        }
        infile.close();
    }

    void printCentered(const string& text) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        int consoleWidth = csbi.dwSize.X;
        int padding = (consoleWidth - text.length()) / 2;
        cout << string(padding, ' ') << text << endl;
    }

public:
    void promptLogin() {
        printCentered("--- Login ---");
        cout << "Enter your email: ";
        getline(cin, email);
        cout << "Enter your password: ";
        getline(cin, password);
        if (!authenticate()) {
            setCredentials();
            printCentered("Credentials set. Please restart the program to log in.");
            exit(0);
        } else {
            loadContactsFromFile();
        }
    }

    void addContact() {
        string name, phoneNumber;
        printCentered("--- Add New Contact ---");
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Phone Number: ";
        getline(cin, phoneNumber);
        contacts.push_back(Contact(name, phoneNumber));
        printCentered("Contact added successfully!");
        saveContactsToFile();
    }

    void viewContacts() {
        printCentered("--- Contact List ---");
        if (contacts.empty()) {
            printCentered("No contacts available.");
        } else {
            for (size_t i = 0; i < contacts.size(); ++i) {
                cout << i + 1 << ". Name: " << contacts[i].name << "\n   Phone: " << contacts[i].phoneNumber << endl;
                if (i < contacts.size() - 1) {
                    printCentered("------------------------");
                }
            }
        }
    }

    void deleteContact() {
        printCentered("--- Delete Contact ---");
        if (contacts.empty()) {
            printCentered("No contacts available to delete.");
        } else {
            viewContacts();
            int index;
            cout << "\nEnter the number of the contact to delete: ";
            cin >> index;
            cin.ignore();
            if (index > 0 && index <= contacts.size()) {
                contacts.erase(contacts.begin() + index - 1);
                printCentered("Contact deleted successfully!");
                saveContactsToFile();
            } else {
                printCentered("Invalid contact number!");
            }
        }
    }

    void editContact() {
        printCentered("--- Edit Contact ---");
        if (contacts.empty()) {
            printCentered("No contacts available to edit.");
        } else {
            viewContacts();
            int index;
            cout << "\nEnter the number of the contact to edit: ";
            cin >> index;
            cin.ignore();
            if (index > 0 && index <= contacts.size()) {
                string newName, newPhoneNumber;
                cout << "\nEnter new name (leave empty to keep current): ";
                getline(cin, newName);
                cout << "Enter new phone number (leave empty to keep current): ";
                getline(cin, newPhoneNumber);
                if (!newName.empty()) {
                    contacts[index - 1].name = newName;
                }
                if (!newPhoneNumber.empty()) {
                    contacts[index - 1].phoneNumber = newPhoneNumber;
                }
                printCentered("Contact edited successfully!");
                saveContactsToFile();
            } else {
                printCentered("Invalid contact number!");
            }
        }
    }

    void showMenu() {
        while (true) {
            printCentered("--- Contact Management System ---");
            cout << "1. Add Contact" << endl;
            cout << "2. View Contacts" << endl;
            cout << "3. Edit Contact" << endl;
            cout << "4. Delete Contact" << endl;
            cout << "5. Exit" << endl;
            cout << "-------------------------------" << endl;
            cout << "Enter your choice: ";
            int choice;
            cin >> choice;
            cin.ignore();
            switch (choice) {
                case 1:
                    addContact();
                    break;
                case 2:
                    viewContacts();
                    break;
                case 3:
                    editContact();
                    break;
                case 4:
                    deleteContact();
                    break;
                case 5:
                    printCentered("Exiting program...");
                    return;
                default:
                    printCentered("Invalid choice! Please try again.");
            }
            pressEnterToContinue();
        }
    }
};

int main() {
    ContactManager manager;
    manager.promptLogin();
    manager.showMenu();
    return 0;
}

