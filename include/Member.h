#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <vector>

// Represents a library member.
class Member {
private:
    int id;
    std::string name;
    std::string contact;
    int activeBorrowCount;

public:
    Member();
    Member(int id, const std::string& name, const std::string& contact,
           int activeBorrowCount = 0);

    int getId() const;
    std::string getName() const;
    std::string getContact() const;
    int getActiveBorrowCount() const;

    void setName(const std::string& name);
    void setContact(const std::string& contact);
    void incrementBorrowCount();
    void decrementBorrowCount();

    std::string toFileString() const;
    static Member fromFileString(const std::string& line);

    void display() const;
};

#endif // MEMBER_H
