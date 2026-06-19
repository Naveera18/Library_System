#include "Member.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>

Member::Member() : id(0), name(""), contact(""), activeBorrowCount(0) {}

Member::Member(int id, const std::string& name, const std::string& contact,
               int activeBorrowCount)
    : id(id), name(name), contact(contact), activeBorrowCount(activeBorrowCount) {}

int Member::getId() const { return id; }
std::string Member::getName() const { return name; }
std::string Member::getContact() const { return contact; }
int Member::getActiveBorrowCount() const { return activeBorrowCount; }

void Member::setName(const std::string& n) { name = n; }
void Member::setContact(const std::string& c) { contact = c; }
void Member::incrementBorrowCount() { activeBorrowCount++; }
void Member::decrementBorrowCount() {
    if (activeBorrowCount > 0) activeBorrowCount--;
}

// Pipe-delimited format: id|name|contact|activeBorrowCount
std::string Member::toFileString() const {
    std::ostringstream oss;
    oss << id << "|" << name << "|" << contact << "|" << activeBorrowCount;
    return oss.str();
}

Member Member::fromFileString(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    while (std::getline(ss, field, '|')) {
        fields.push_back(field);
    }
    if (fields.size() < 4) {
        return Member(-1, "", "", 0);
    }
    int id = std::stoi(fields[0]);
    int count = std::stoi(fields[3]);
    return Member(id, fields[1], fields[2], count);
}

void Member::display() const {
    std::cout << std::left
               << std::setw(6) << id
               << std::setw(20) << name
               << std::setw(20) << contact
               << "Active Borrows: " << activeBorrowCount
               << std::endl;
}
