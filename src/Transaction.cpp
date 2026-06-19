#include "Transaction.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>

Transaction::Transaction()
    : transactionId(0), bookId(0), memberId(0), issueDay(0), dueDay(0),
      returnDay(-1), returned(false) {}

Transaction::Transaction(int transactionId, int bookId, int memberId, int issueDay,
                          int dueDay, int returnDay, bool returned)
    : transactionId(transactionId), bookId(bookId), memberId(memberId),
      issueDay(issueDay), dueDay(dueDay), returnDay(returnDay), returned(returned) {}

int Transaction::getTransactionId() const { return transactionId; }
int Transaction::getBookId() const { return bookId; }
int Transaction::getMemberId() const { return memberId; }
int Transaction::getIssueDay() const { return issueDay; }
int Transaction::getDueDay() const { return dueDay; }
int Transaction::getReturnDay() const { return returnDay; }
bool Transaction::isReturned() const { return returned; }

void Transaction::markReturned(int day) {
    returnDay = day;
    returned = true;
}

// Pipe-delimited format: transactionId|bookId|memberId|issueDay|dueDay|returnDay|returned
std::string Transaction::toFileString() const {
    std::ostringstream oss;
    oss << transactionId << "|" << bookId << "|" << memberId << "|"
        << issueDay << "|" << dueDay << "|" << returnDay << "|"
        << (returned ? 1 : 0);
    return oss.str();
}

Transaction Transaction::fromFileString(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    while (std::getline(ss, field, '|')) {
        fields.push_back(field);
    }
    if (fields.size() < 7) {
        return Transaction(-1, -1, -1, 0, 0, -1, false);
    }
    return Transaction(
        std::stoi(fields[0]),
        std::stoi(fields[1]),
        std::stoi(fields[2]),
        std::stoi(fields[3]),
        std::stoi(fields[4]),
        std::stoi(fields[5]),
        fields[6] == "1"
    );
}

void Transaction::display() const {
    std::cout << std::left
               << "TxnID: " << std::setw(6) << transactionId
               << "BookID: " << std::setw(6) << bookId
               << "MemberID: " << std::setw(6) << memberId
               << "Issued(day): " << std::setw(6) << issueDay
               << "Due(day): " << std::setw(6) << dueDay
               << "Status: " << (returned ? "Returned" : "Issued")
               << std::endl;
}
