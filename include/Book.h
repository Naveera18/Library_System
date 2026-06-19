#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <iostream>

// Represents a single book record in the library.
class Book {
private:
    int id;
    std::string title;
    std::string author;
    std::string category;
    bool available; // true = available, false = currently issued

public:
    Book();
    Book(int id, const std::string& title, const std::string& author,
         const std::string& category, bool available = true);

    // Getters (encapsulation: no direct field access from outside)
    int getId() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getCategory() const;
    bool isAvailable() const;

    // Setters
    void setTitle(const std::string& title);
    void setAuthor(const std::string& author);
    void setCategory(const std::string& category);
    void setAvailable(bool status);

    // Serialization helpers for file persistence (pipe-delimited)
    std::string toFileString() const;
    static Book fromFileString(const std::string& line);

    void display() const;
};

#endif // BOOK_H
