#include "Library.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>

Library::Library(const std::string& dataDir)
    : fileManager(dataDir), nextBookId(1), nextMemberId(1), nextTransactionId(1),
      currentDay(0), finePerDay(10.0) {
    loadAll();
}

void Library::loadAll() {
    books = fileManager.loadBooks();
    members = fileManager.loadMembers();
    transactions = fileManager.loadTransactions();

    // Recompute next-ID counters from the highest existing ID, so IDs stay
    // unique across program restarts.
    for (const auto& b : books) nextBookId = std::max(nextBookId, b.getId() + 1);
    for (const auto& m : members) nextMemberId = std::max(nextMemberId, m.getId() + 1);
    for (const auto& t : transactions) {
        nextTransactionId = std::max(nextTransactionId, t.getTransactionId() + 1);
        // Keep "currentDay" advanced at least as far as the latest known transaction day
        currentDay = std::max(currentDay, t.getDueDay());
    }
}

int Library::findBookIndex(int bookId) const {
    for (size_t i = 0; i < books.size(); ++i) {
        if (books[i].getId() == bookId) return static_cast<int>(i);
    }
    return -1;
}

int Library::findMemberIndex(int memberId) const {
    for (size_t i = 0; i < members.size(); ++i) {
        if (members[i].getId() == memberId) return static_cast<int>(i);
    }
    return -1;
}

void Library::persistBooks() { fileManager.saveBooks(books); }
void Library::persistMembers() { fileManager.saveMembers(members); }
void Library::persistTransactions() { fileManager.saveTransactions(transactions); }

bool Library::bookExists(int bookId) const { return findBookIndex(bookId) != -1; }
bool Library::memberExists(int memberId) const { return findMemberIndex(memberId) != -1; }

// ---------------- Book Management ----------------

void Library::addBook(const std::string& title, const std::string& author,
                       const std::string& category) {
    Book newBook(nextBookId++, title, author, category, true);
    books.push_back(newBook);
    persistBooks();
    std::cout << "Book added successfully with ID: " << newBook.getId() << std::endl;
}

bool Library::updateBook(int bookId, const std::string& title, const std::string& author,
                          const std::string& category) {
    int idx = findBookIndex(bookId);
    if (idx == -1) {
        std::cout << "Error: Book ID " << bookId << " not found." << std::endl;
        return false;
    }
    if (!title.empty())    books[idx].setTitle(title);
    if (!author.empty())   books[idx].setAuthor(author);
    if (!category.empty()) books[idx].setCategory(category);
    persistBooks();
    std::cout << "Book updated successfully." << std::endl;
    return true;
}

bool Library::deleteBook(int bookId) {
    int idx = findBookIndex(bookId);
    if (idx == -1) {
        std::cout << "Error: Book ID " << bookId << " not found." << std::endl;
        return false;
    }
    if (!books[idx].isAvailable()) {
        std::cout << "Error: Cannot delete a book that is currently issued." << std::endl;
        return false;
    }
    books.erase(books.begin() + idx);
    persistBooks();
    std::cout << "Book deleted successfully." << std::endl;
    return true;
}

void Library::viewAllBooks() const {
    if (books.empty()) {
        std::cout << "No books in the system yet." << std::endl;
        return;
    }
    std::cout << std::left
               << std::setw(6) << "ID"
               << std::setw(30) << "Title"
               << std::setw(20) << "Author"
               << std::setw(15) << "Category"
               << "Status" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    for (const auto& b : books) b.display();
}

// ---------------- Member Management ----------------

void Library::addMember(const std::string& name, const std::string& contact) {
    Member newMember(nextMemberId++, name, contact, 0);
    members.push_back(newMember);
    persistMembers();
    std::cout << "Member added successfully with ID: " << newMember.getId() << std::endl;
}

void Library::viewAllMembers() const {
    if (members.empty()) {
        std::cout << "No members registered yet." << std::endl;
        return;
    }
    std::cout << std::left
               << std::setw(6) << "ID"
               << std::setw(20) << "Name"
               << std::setw(20) << "Contact"
               << "Active Borrows" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    for (const auto& m : members) m.display();
}

void Library::viewMemberHistory(int memberId) const {
    if (!memberExists(memberId)) {
        std::cout << "Error: Member ID " << memberId << " not found." << std::endl;
        return;
    }
    bool found = false;
    std::cout << "Borrowing history for Member ID " << memberId << ":" << std::endl;
    for (const auto& t : transactions) {
        if (t.getMemberId() == memberId) {
            t.display();
            found = true;
        }
    }
    if (!found) std::cout << "No borrowing history found." << std::endl;
}

// ---------------- Borrow & Return ----------------

bool Library::issueBook(int bookId, int memberId, int loanDays) {
    int bIdx = findBookIndex(bookId);
    int mIdx = findMemberIndex(memberId);

    if (bIdx == -1) {
        std::cout << "Error: Book ID " << bookId << " not found." << std::endl;
        return false;
    }
    if (mIdx == -1) {
        std::cout << "Error: Member ID " << memberId << " not found." << std::endl;
        return false;
    }
    if (!books[bIdx].isAvailable()) {
        std::cout << "Error: Book \"" << books[bIdx].getTitle()
                   << "\" is currently unavailable (already issued)." << std::endl;
        return false;
    }

    books[bIdx].setAvailable(false);
    members[mIdx].incrementBorrowCount();

    int dueDay = currentDay + loanDays;
    Transaction t(nextTransactionId++, bookId, memberId, currentDay, dueDay, -1, false);
    transactions.push_back(t);

    persistBooks();
    persistMembers();
    persistTransactions();

    std::cout << "Book issued successfully. Due on day " << dueDay
               << " (in " << loanDays << " days)." << std::endl;
    return true;
}

bool Library::returnBook(int bookId, int memberId) {
    int bIdx = findBookIndex(bookId);
    int mIdx = findMemberIndex(memberId);

    if (bIdx == -1) {
        std::cout << "Error: Book ID " << bookId << " not found." << std::endl;
        return false;
    }
    if (mIdx == -1) {
        std::cout << "Error: Member ID " << memberId << " not found." << std::endl;
        return false;
    }

    // Find the open (unreturned) transaction matching this book + member
    Transaction* openTxn = nullptr;
    for (auto& t : transactions) {
        if (t.getBookId() == bookId && t.getMemberId() == memberId && !t.isReturned()) {
            openTxn = &t;
            break;
        }
    }

    if (openTxn == nullptr) {
        std::cout << "Error: No active borrow record found for this book/member pair."
                   << std::endl;
        return false;
    }

    double fine = 0.0;
    if (currentDay > openTxn->getDueDay()) {
        int overdueDays = currentDay - openTxn->getDueDay();
        fine = overdueDays * finePerDay;
    }

    openTxn->markReturned(currentDay);
    books[bIdx].setAvailable(true);
    members[mIdx].decrementBorrowCount();

    persistBooks();
    persistMembers();
    persistTransactions();

    std::cout << "Book returned successfully." << std::endl;
    if (fine > 0.0) {
        std::cout << "NOTE: This return was overdue. Fine due: $"
                   << std::fixed << std::setprecision(2) << fine << std::endl;
    }
    return true;
}

double Library::calculateFine(int bookId, int memberId) const {
    for (const auto& t : transactions) {
        if (t.getBookId() == bookId && t.getMemberId() == memberId && !t.isReturned()) {
            if (currentDay > t.getDueDay()) {
                return (currentDay - t.getDueDay()) * finePerDay;
            }
            return 0.0;
        }
    }
    return 0.0;
}

// ---------------- Search ----------------

void Library::searchById(int bookId) const {
    int idx = findBookIndex(bookId);
    if (idx == -1) {
        std::cout << "No book found with ID " << bookId << std::endl;
        return;
    }
    books[idx].display();
}

void Library::searchByTitle(const std::string& title) const {
    std::string lowerQuery = title;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    bool found = false;
    for (const auto& b : books) {
        std::string lowerTitle = b.getTitle();
        std::transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);
        if (lowerTitle.find(lowerQuery) != std::string::npos) {
            b.display();
            found = true;
        }
    }
    if (!found) std::cout << "No books found matching title \"" << title << "\"." << std::endl;
}

void Library::searchByAuthor(const std::string& author) const {
    std::string lowerQuery = author;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    bool found = false;
    for (const auto& b : books) {
        std::string lowerAuthor = b.getAuthor();
        std::transform(lowerAuthor.begin(), lowerAuthor.end(), lowerAuthor.begin(), ::tolower);
        if (lowerAuthor.find(lowerQuery) != std::string::npos) {
            b.display();
            found = true;
        }
    }
    if (!found) std::cout << "No books found by author \"" << author << "\"." << std::endl;
}

// ---------------- Reporting ----------------

void Library::reportMostBorrowed(int topN) const {
    if (transactions.empty()) {
        std::cout << "No transactions recorded yet." << std::endl;
        return;
    }
    std::map<int, int> borrowCounts; // bookId -> times borrowed
    for (const auto& t : transactions) {
        borrowCounts[t.getBookId()]++;
    }

    std::vector<std::pair<int, int>> sorted(borrowCounts.begin(), borrowCounts.end());
    std::sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    std::cout << "Most Borrowed Books:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    int count = 0;
    for (const auto& [bookId, times] : sorted) {
        if (count >= topN) break;
        int idx = findBookIndex(bookId);
        std::string title = (idx != -1) ? books[idx].getTitle() : "(deleted book)";
        std::cout << std::left << std::setw(6) << bookId
                   << std::setw(30) << title
                   << "Borrowed " << times << " time(s)" << std::endl;
        count++;
    }
}

void Library::reportCurrentlyIssued() const {
    bool found = false;
    std::cout << "Currently Issued Books:" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    for (const auto& t : transactions) {
        if (!t.isReturned()) {
            t.display();
            found = true;
        }
    }
    if (!found) std::cout << "No books are currently issued." << std::endl;
}

void Library::reportOverdue() const {
    bool found = false;
    std::cout << "Overdue Books (as of day " << currentDay << "):" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    for (const auto& t : transactions) {
        if (!t.isReturned() && currentDay > t.getDueDay()) {
            int overdueDays = currentDay - t.getDueDay();
            double fine = overdueDays * finePerDay;
            t.display();
            std::cout << "   -> Overdue by " << overdueDays << " day(s). Fine: $"
                       << std::fixed << std::setprecision(2) << fine << std::endl;
            found = true;
        }
    }
    if (!found) std::cout << "No overdue books." << std::endl;
}

void Library::exportReportToCSV(const std::string& filename) const {
    std::ofstream file(filename, std::ios::trunc);
    if (!file.is_open()) {
        std::cout << "Error: Could not create CSV file." << std::endl;
        return;
    }
    file << "TransactionID,BookID,BookTitle,MemberID,MemberName,IssueDay,DueDay,ReturnDay,Status,Fine\n";
    for (const auto& t : transactions) {
        int bIdx = findBookIndex(t.getBookId());
        int mIdx = findMemberIndex(t.getMemberId());
        std::string title = (bIdx != -1) ? books[bIdx].getTitle() : "(deleted)";
        std::string memberName = (mIdx != -1) ? members[mIdx].getName() : "(deleted)";

        double fine = 0.0;
        int referenceDay = t.isReturned() ? t.getReturnDay() : currentDay;
        if (referenceDay > t.getDueDay()) {
            fine = (referenceDay - t.getDueDay()) * finePerDay;
        }

        file << t.getTransactionId() << ","
             << t.getBookId() << "," << title << ","
             << t.getMemberId() << "," << memberName << ","
             << t.getIssueDay() << "," << t.getDueDay() << ","
             << t.getReturnDay() << ","
             << (t.isReturned() ? "Returned" : "Issued") << ","
             << std::fixed << std::setprecision(2) << fine << "\n";
    }
    file.close();
    std::cout << "Report exported to " << filename << std::endl;
}

// ---------------- Misc ----------------

void Library::advanceDay(int days) {
    currentDay += days;
    std::cout << "Simulated time advanced by " << days << " day(s). Current day is now "
               << currentDay << "." << std::endl;
}

int Library::getCurrentDay() const { return currentDay; }
