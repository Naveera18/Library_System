#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <string>
#include "Book.h"
#include "Member.h"
#include "Transaction.h"
#include "FileManager.h"

// Core orchestrating class. Holds in-memory copies of all books, members,
// and transactions, and keeps the data files in sync after every operation.
class Library {
private:
    std::vector<Book> books;
    std::vector<Member> members;
    std::vector<Transaction> transactions;
    FileManager fileManager;

    int nextBookId;
    int nextMemberId;
    int nextTransactionId;
    int currentDay; // simple simulated "today" as a day counter
    double finePerDay;

    int findBookIndex(int bookId) const;
    int findMemberIndex(int memberId) const;

    void persistBooks();
    void persistMembers();
    void persistTransactions();

public:
    Library(const std::string& dataDir = "data");

    void loadAll();

    // ---- Book Management ----
    void addBook(const std::string& title, const std::string& author,
                 const std::string& category);
    bool updateBook(int bookId, const std::string& title, const std::string& author,
                     const std::string& category);
    bool deleteBook(int bookId);
    void viewAllBooks() const;

    // ---- Member Management ----
    void addMember(const std::string& name, const std::string& contact);
    void viewAllMembers() const;
    void viewMemberHistory(int memberId) const;

    // ---- Borrow & Return ----
    bool issueBook(int bookId, int memberId, int loanDays = 14);
    bool returnBook(int bookId, int memberId);

    // ---- Search ----
    void searchById(int bookId) const;
    void searchByTitle(const std::string& title) const;
    void searchByAuthor(const std::string& author) const;

    // ---- Reporting ----
    void reportMostBorrowed(int topN = 5) const;
    void reportCurrentlyIssued() const;
    void reportOverdue() const;
    void exportReportToCSV(const std::string& filename) const; // bonus

    // ---- Misc ----
    void advanceDay(int days = 1); // simulate passage of time for overdue/fine testing
    int getCurrentDay() const;
    double calculateFine(int bookId, int memberId) const;
    bool bookExists(int bookId) const;
    bool memberExists(int memberId) const;
};

#endif // LIBRARY_H
