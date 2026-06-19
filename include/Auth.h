#ifndef AUTH_H
#define AUTH_H

#include <string>

// Simple role-based login system (bonus feature).
// Credentials are stored in a plain text file for this intermediate-level
// project; in a production system these would be hashed.
enum class Role { NONE, ADMIN, MEMBER };

class Auth {
private:
    std::string credentialsFile;
    Role currentRole;
    int currentMemberId; // valid only when currentRole == MEMBER

public:
    Auth(const std::string& dataDir = "data");

    void ensureDefaultAdmin() const;

    // Returns the role achieved after a successful login, or Role::NONE on failure.
    Role login(const std::string& username, const std::string& password, int memberId = -1);
    void logout();

    Role getRole() const;
    int getMemberId() const;
};

#endif // AUTH_H
