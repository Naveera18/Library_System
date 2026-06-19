#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include "Book.h"
#include "Member.h"
#include "Transaction.h"

// Centralizes all file read/write operations so persistence logic
// lives in one place. Each entity has its own data file.
class FileManager {
private:
    std::string booksFile;
    std::string membersFile;
    std::string transactionsFile;
    std::string fineRatePerDayFile; // stores configurable fine rate (bonus)

public:
    FileManager(const std::string& dataDir = "data");

    // Books
    std::vector<Book> loadBooks() const;
    void saveBooks(const std::vector<Book>& books) const;

    // Members
    std::vector<Member> loadMembers() const;
    void saveMembers(const std::vector<Member>& members) const;

    // Transactions
    std::vector<Transaction> loadTransactions() const;
    void saveTransactions(const std::vector<Transaction>& transactions) const;

    // Ensures data directory and files exist before first read
    void ensureDataFilesExist() const;
};

#endif // FILE_MANAGER_H
