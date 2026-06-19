#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

// Represents a single borrow/return transaction.
// dueDate / returnDate are stored as "days since program epoch" (simple integer day-counter),
// which keeps overdue logic simple without needing a full date library.
class Transaction {
private:
    int transactionId;
    int bookId;
    int memberId;
    int issueDay;     // day number when book was issued
    int dueDay;       // day number when book is due
    int returnDay;    // -1 if not yet returned
    bool returned;

public:
    Transaction();
    Transaction(int transactionId, int bookId, int memberId, int issueDay,
                int dueDay, int returnDay = -1, bool returned = false);

    int getTransactionId() const;
    int getBookId() const;
    int getMemberId() const;
    int getIssueDay() const;
    int getDueDay() const;
    int getReturnDay() const;
    bool isReturned() const;

    void markReturned(int returnDay);

    std::string toFileString() const;
    static Transaction fromFileString(const std::string& line);

    void display() const;
};

#endif // TRANSACTION_H
