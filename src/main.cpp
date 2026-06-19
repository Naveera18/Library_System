#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
#include "Library.h"
#include "Auth.h"

// ---------- Input helper utilities (basic error handling) ----------

void clearInputError() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.eof()) {
            // Input stream closed (e.g., piped input exhausted). Exit cleanly
            // instead of spinning forever.
            std::cout << "\nNo more input available. Exiting.\n";
            std::exit(0);
        }
        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a whole number.\n";
            clearInputError();
            continue;
        }
        clearInputError();
        return value;
    }
}

std::string readLine(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    if (!std::getline(std::cin, value)) {
        std::cout << "\nNo more input available. Exiting.\n";
        std::exit(0);
    }
    return value;
}

// ---------- Menu screens ----------

void printAdminMenu() {
    std::cout << "\n========== NAVEERA'S LIBRARY SYSTEM (Admin) ==========\n";
    std::cout << " 1. Add Book\n";
    std::cout << " 2. Update Book\n";
    std::cout << " 3. Delete Book\n";
    std::cout << " 4. View All Books\n";
    std::cout << " 5. Add Member\n";
    std::cout << " 6. View All Members\n";
    std::cout << " 7. View Member Borrow History\n";
    std::cout << " 8. Issue Book\n";
    std::cout << " 9. Return Book\n";
    std::cout << "10. Search Book by ID\n";
    std::cout << "11. Search Book by Title\n";
    std::cout << "12. Search Book by Author\n";
    std::cout << "13. Report: Most Borrowed Books\n";
    std::cout << "14. Report: Currently Issued Books\n";
    std::cout << "15. Report: Overdue Books\n";
    std::cout << "16. Export Report to CSV\n";
    std::cout << "17. Advance Simulated Day (testing overdue/fines)\n";
    std::cout << "18. Logout\n";
    std::cout << " 0. Exit\n";
    std::cout << "==========================================================\n";
}

void printMemberMenu() {
    std::cout << "\n========== NAVEERA'S LIBRARY SYSTEM (Member) ==========\n";
    std::cout << " 1. View All Books\n";
    std::cout << " 2. Search Book by ID\n";
    std::cout << " 3. Search Book by Title\n";
    std::cout << " 4. Search Book by Author\n";
    std::cout << " 5. View My Borrow History\n";
    std::cout << " 6. Logout\n";
    std::cout << " 0. Exit\n";
    std::cout << "==========================================================\n";
}

void printLoginMenu() {
    std::cout << "\n=============== NAVEERA'S LIBRARY SYSTEM ===============\n";
    std::cout << " 1. Login as Admin\n";
    std::cout << " 2. Login as Member\n";
    std::cout << " 0. Exit\n";
    std::cout << "==========================================================\n";
    std::cout << "(Default admin credentials: username 'admin', password 'admin123')\n";
    std::cout << "(Member login: enter your Member ID; password is your ID typed twice,\n";
    std::cout << " e.g. Member ID 5 -> password '55')\n";
}

// ---------- Admin actions ----------

void handleAddBook(Library& lib) {
    std::string title = readLine("Title: ");
    std::string author = readLine("Author: ");
    std::string category = readLine("Category: ");
    if (title.empty() || author.empty()) {
        std::cout << "Error: Title and Author cannot be empty.\n";
        return;
    }
    lib.addBook(title, author, category);
}

void handleUpdateBook(Library& lib) {
    int id = readInt("Book ID to update: ");
    std::cout << "Leave a field blank to keep its current value.\n";
    std::string title = readLine("New Title: ");
    std::string author = readLine("New Author: ");
    std::string category = readLine("New Category: ");
    lib.updateBook(id, title, author, category);
}

void handleDeleteBook(Library& lib) {
    int id = readInt("Book ID to delete: ");
    lib.deleteBook(id);
}

void handleAddMember(Library& lib) {
    std::string name = readLine("Name: ");
    std::string contact = readLine("Contact: ");
    if (name.empty()) {
        std::cout << "Error: Name cannot be empty.\n";
        return;
    }
    lib.addMember(name, contact);
}

void handleIssueBook(Library& lib) {
    int bookId = readInt("Book ID: ");
    int memberId = readInt("Member ID: ");
    int days = readInt("Loan period in days (e.g. 14): ");
    if (days <= 0) {
        std::cout << "Error: Loan period must be positive.\n";
        return;
    }
    lib.issueBook(bookId, memberId, days);
}

void handleReturnBook(Library& lib) {
    int bookId = readInt("Book ID: ");
    int memberId = readInt("Member ID: ");
    lib.returnBook(bookId, memberId);
}

void handleSearchById(const Library& lib) {
    int id = readInt("Book ID: ");
    lib.searchById(id);
}

void handleSearchByTitle(const Library& lib) {
    std::string title = readLine("Title keyword: ");
    lib.searchByTitle(title);
}

void handleSearchByAuthor(const Library& lib) {
    std::string author = readLine("Author keyword: ");
    lib.searchByAuthor(author);
}

// ---------- Main loop ----------

int main() {
    Library library("data");
    Auth auth("data");

    std::cout << "\nWelcome to the NAVEERA'S LIBRARY SYSTEM!\n";

    bool running = true;
    while (running) {
        if (auth.getRole() == Role::NONE) {
            printLoginMenu();
            int choice = readInt("Choose an option: ");
            if (choice == 0) {
                running = false;
            } else if (choice == 1) {
                std::string user = readLine("Username: ");
                std::string pass = readLine("Password: ");
                if (auth.login(user, pass) == Role::ADMIN) {
                    std::cout << "Login successful. Welcome, Admin.\n";
                } else {
                    std::cout << "Invalid admin credentials.\n";
                }
            } else if (choice == 2) {
                int memberId = readInt("Member ID: ");
                std::string pass = readLine("Password: ");
                if (!library.memberExists(memberId)) {
                    std::cout << "Error: No such Member ID.\n";
                } else if (auth.login("", pass, memberId) == Role::MEMBER) {
                    std::cout << "Login successful. Welcome, Member #" << memberId << ".\n";
                } else {
                    std::cout << "Invalid member credentials.\n";
                }
            } else {
                std::cout << "Invalid option.\n";
            }
            continue;
        }

        if (auth.getRole() == Role::ADMIN) {
            printAdminMenu();
            int choice = readInt("Choose an option: ");
            switch (choice) {
                case 1:  handleAddBook(library); break;
                case 2:  handleUpdateBook(library); break;
                case 3:  handleDeleteBook(library); break;
                case 4:  library.viewAllBooks(); break;
                case 5:  handleAddMember(library); break;
                case 6:  library.viewAllMembers(); break;
                case 7: {
                    int id = readInt("Member ID: ");
                    library.viewMemberHistory(id);
                    break;
                }
                case 8:  handleIssueBook(library); break;
                case 9:  handleReturnBook(library); break;
                case 10: handleSearchById(library); break;
                case 11: handleSearchByTitle(library); break;
                case 12: handleSearchByAuthor(library); break;
                case 13: library.reportMostBorrowed(5); break;
                case 14: library.reportCurrentlyIssued(); break;
                case 15: library.reportOverdue(); break;
                case 16: {
                    std::string filename = readLine("Output CSV filename (e.g. report.csv): ");
                    if (filename.empty()) filename = "report.csv";
                    library.exportReportToCSV(filename);
                    break;
                }
                case 17: {
                    int days = readInt("Advance by how many days?: ");
                    library.advanceDay(days);
                    break;
                }
                case 18:
                    auth.logout();
                    std::cout << "Logged out.\n";
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    std::cout << "Invalid option. Please try again.\n";
            }
        } else if (auth.getRole() == Role::MEMBER) {
            printMemberMenu();
            int choice = readInt("Choose an option: ");
            switch (choice) {
                case 1: library.viewAllBooks(); break;
                case 2: handleSearchById(library); break;
                case 3: handleSearchByTitle(library); break;
                case 4: handleSearchByAuthor(library); break;
                case 5: library.viewMemberHistory(auth.getMemberId()); break;
                case 6:
                    auth.logout();
                    std::cout << "Logged out.\n";
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    std::cout << "Invalid option. Please try again.\n";
            }
        }
    }

    std::cout << "\nThank you for using NAVEERA'S LIBRARY SYSTEM. Goodbye!\n";
    return 0;
}
