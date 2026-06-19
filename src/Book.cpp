#include "Book.h"
#include <sstream>
#include <iomanip>
#include <vector>

Book::Book() : id(0), title(""), author(""), category(""), available(true) {}

Book::Book(int id, const std::string& title, const std::string& author,
           const std::string& category, bool available)
    : id(id), title(title), author(author), category(category), available(available) {}

int Book::getId() const { return id; }
std::string Book::getTitle() const { return title; }
std::string Book::getAuthor() const { return author; }
std::string Book::getCategory() const { return category; }
bool Book::isAvailable() const { return available; }

void Book::setTitle(const std::string& t) { title = t; }
void Book::setAuthor(const std::string& a) { author = a; }
void Book::setCategory(const std::string& c) { category = c; }
void Book::setAvailable(bool status) { available = status; }

// Pipe-delimited format: id|title|author|category|available
std::string Book::toFileString() const {
    std::ostringstream oss;
    oss << id << "|" << title << "|" << author << "|" << category << "|"
        << (available ? 1 : 0);
    return oss.str();
}

Book Book::fromFileString(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    while (std::getline(ss, field, '|')) {
        fields.push_back(field);
    }
    // Defensive: if the line is malformed, return a default/invalid book (id = -1)
    if (fields.size() < 5) {
        return Book(-1, "", "", "", false);
    }
    int id = std::stoi(fields[0]);
    bool available = (fields[4] == "1");
    return Book(id, fields[1], fields[2], fields[3], available);
}

void Book::display() const {
    std::cout << std::left
               << std::setw(6) << id
               << std::setw(30) << title
               << std::setw(20) << author
               << std::setw(18) << category
               << (available ? "Available" : "Issued")
               << std::endl;
}
