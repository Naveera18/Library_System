#include "Auth.h"
#include <fstream>
#include <sstream>
#include <iostream>

Auth::Auth(const std::string& dataDir)
    : currentRole(Role::NONE), currentMemberId(-1) {
    credentialsFile = dataDir + "/admin_credentials.txt";
    ensureDefaultAdmin();
}

// Creates a default admin account (admin / admin123) on first run if no
// credentials file exists yet. The user is expected to change this in a
// real deployment; this is sufficient for an intermediate coursework project.
void Auth::ensureDefaultAdmin() const {
    std::ifstream check(credentialsFile);
    bool exists = check.good() && check.peek() != std::ifstream::traits_type::eof();
    check.close();

    if (!exists) {
        std::ofstream out(credentialsFile, std::ios::trunc);
        out << "admin|admin123\n";
        out.close();
    }
}

Role Auth::login(const std::string& username, const std::string& password, int memberId) {
    // Admin login: checked against credentials file
    std::ifstream file(credentialsFile);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string user, pass;
        std::getline(ss, user, '|');
        std::getline(ss, pass, '|');
        if (user == username && pass == password) {
            currentRole = Role::ADMIN;
            currentMemberId = -1;
            return currentRole;
        }
    }

    // Member login: simplified — a member "logs in" with their Member ID
    // and a password that is just their ID repeated (e.g., memberId=5 -> "5555").
    // This keeps the bonus feature simple without requiring a separate
    // member-password file, while still demonstrating role separation.
    if (memberId != -1) {
        std::string expectedPassword = std::to_string(memberId) + std::to_string(memberId);
        if (password == expectedPassword) {
            currentRole = Role::MEMBER;
            currentMemberId = memberId;
            return currentRole;
        }
    }

    currentRole = Role::NONE;
    currentMemberId = -1;
    return currentRole;
}

void Auth::logout() {
    currentRole = Role::NONE;
    currentMemberId = -1;
}

Role Auth::getRole() const { return currentRole; }
int Auth::getMemberId() const { return currentMemberId; }
