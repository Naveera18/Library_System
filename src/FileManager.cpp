#include "FileManager.h"
#include <fstream>
#include <sys/stat.h>
#include <iostream>
#include <direct.h>

FileManager::FileManager(const std::string& dataDir) {
    booksFile = dataDir + "/books.txt";
    membersFile = dataDir + "/members.txt";
    transactionsFile = dataDir + "/transactions.txt";
    fineRatePerDayFile = dataDir + "/config.txt";

    // Create the data directory if it doesn't exist (portable enough for
    // the Linux/macOS environments this project targets).
    _mkdir(dataDir.c_str());
    ensureDataFilesExist();
}

void FileManager::ensureDataFilesExist() const {
    // ofstream with app mode creates the file if missing, without truncating
    // existing content.
    std::ofstream(booksFile, std::ios::app).close();
    std::ofstream(membersFile, std::ios::app).close();
    std::ofstream(transactionsFile, std::ios::app).close();
}

std::vector<Book> FileManager::loadBooks() const {
    std::vector<Book> result;
    std::ifstream file(booksFile);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        Book b = Book::fromFileString(line);
        if (b.getId() != -1) result.push_back(b);
    }
    return result;
}

void FileManager::saveBooks(const std::vector<Book>& books) const {
    std::ofstream file(booksFile, std::ios::trunc);
    for (const auto& b : books) {
        file << b.toFileString() << "\n";
    }
}

std::vector<Member> FileManager::loadMembers() const {
    std::vector<Member> result;
    std::ifstream file(membersFile);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        Member m = Member::fromFileString(line);
        if (m.getId() != -1) result.push_back(m);
    }
    return result;
}

void FileManager::saveMembers(const std::vector<Member>& members) const {
    std::ofstream file(membersFile, std::ios::trunc);
    for (const auto& m : members) {
        file << m.toFileString() << "\n";
    }
}

std::vector<Transaction> FileManager::loadTransactions() const {
    std::vector<Transaction> result;
    std::ifstream file(transactionsFile);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        Transaction t = Transaction::fromFileString(line);
        if (t.getTransactionId() != -1) result.push_back(t);
    }
    return result;
}

void FileManager::saveTransactions(const std::vector<Transaction>& transactions) const {
    std::ofstream file(transactionsFile, std::ios::trunc);
    for (const auto& t : transactions) {
        file << t.toFileString() << "\n";
    }
}
