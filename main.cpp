#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <regex>
#include <vector>
#include <algorithm>
#include "md5.h"

using namespace std;

vector<pair<string, string>> readUsers() {
    ifstream usersFile("users.txt");
    if (!usersFile) {
        return {};
    }

    vector<pair<string, string>> users;
    for (string line; getline(usersFile, line);) {
        size_t pos = line.find(":");
        if (pos != string::npos) {
            string username = line.substr(0, pos);
            string password = line.substr(pos + 1);
            users.push_back({username, password});
        }
    }

    usersFile.close();
    return users;
}

void writeUsers(const vector<pair<string, string>>& users) {
    ofstream usersFile("users.txt");
    if (!usersFile) {
        cerr << "Could not open the users file for writing." << endl;
        exit(EXIT_FAILURE);
    }

    for (const auto& user : users) {
        usersFile << user.first << ":" << user.second << endl;
    }

    usersFile.close();
}

bool validateUsername(const string& username) {
    return regex_match(username, regex("[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}"));
}

bool userExists(const std::string& username) {
    std::ifstream infile("users.txt");
    if (!infile) {
        std::cerr << "Error opening users.txt for reading.\n";
        return false;
    }

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string user;
        std::getline(iss, user, ':');
        if (user == username) {
            infile.close();
            return true;
        }
    }

    infile.close();
    return false;
}


bool validatePassword(const string& password) {
    bool hasUpperCase = any_of(password.begin(), password.end(), [](char c) { return isupper(c); });
    bool hasLowerCase = any_of(password.begin(), password.end(), [](char c) { return islower(c); });
    bool hasDigit = any_of(password.begin(), password.end(), [](char c) { return isdigit(c); });
    bool hasSpecialChar = any_of(password.begin(), password.end(), [](char c) { return ispunct(c); });

    return (password.length() >= 8) && hasUpperCase && hasLowerCase && hasDigit && hasSpecialChar;
}

void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void createUser() {
    string username, password;

    while (true) {
        cout << "Ange e-postadress (användarnamn): ";
        cin >> ws; 
        getline(cin, username);

       
        if (userExists(username)) {
            cout << "Användarnamnet är upptaget. Försök igen med en annan e-postadress." << endl;
        } else if (validateUsername(username)) {
            cout << "E-postadressen är giltig." << endl;
            break;
        } else {
            cout << "Ogiltig e-postadress. Försök igen." << endl;
        }
    }

  
    while (true) {
        cout << "Ange lösenord: ";
        cin >> ws;
        getline(cin, password);

 
        if (validatePassword(password)) {
           
            string hashedPassword = md5(password);

            ofstream usersFile("users.txt", ios::app);
            if (!usersFile) {
                cerr << "Could not open the users file for writing." << endl;
                return;
            }
            usersFile << username << ":" << hashedPassword << endl;
            usersFile.close();

            cout << "Användaren har skapats." << endl;

            break;
        } else {
            
            cout << "Ogiltigt lösenord. Försök igen med ett lösenord som uppfyller följande krav:\n";
            cout << "- Minst 8 tecken\n";
            cout << "- Minst en stor bokstav\n";
            cout << "- Minst en liten bokstav\n";
            cout << "- Minst en siffra\n";
            cout << "- Minst ett specialtecken\n";
        }
    }
}


void testLogin() {
    string username, password;

    cout << "Ange e-postadress (användarnamn): ";
    cin >> username;

    cout << "Ange lösenord: ";
    cin >> password;

    vector<pair<string, string>> users = readUsers();

   
    auto user = find_if(users.begin(), users.end(), [username](const pair<string, string>& u) {
        return u.first == username;
    });

    if (user != users.end() && user->second == md5(password)) {
        cout << "Inloggning lyckades. Välkommen, " << username << "!" << endl;
    } else {
        cout << "Felaktigt användarnamn eller lösenord. Försök igen." << endl;
    }
}


int main() {
    std::string input;
    int choice;

    do {
        std::cout << "\nHuvudmeny\n";
        std::cout << "1. Skapa användare\n";
        std::cout << "2. Test login\n";
        std::cout << "0. Avsluta\n";
        std::cout << "Välj ett alternativ: ";
        std::cin >> input;

        if (!std::all_of(input.begin(), input.end(), ::isdigit)) {
            std::cout << "Ogiltig inmatning. Försök igen.\n";
            continue;
        }

    
        if (!(std::istringstream(input) >> choice) || (choice < 0 || choice > 2)) {
            std::cout << "Ogiltig inmatning. Försök igen.\n";
            continue;
        }

        switch (choice) {
            case 1:
                createUser();
                break;
            case 2:
                testLogin();
                break;
            case 0:
                std::cout << "Programmet avslutas.\n";
                break;
            default:
                break;
        }

    } while (choice != 0);

    return 0;
}
