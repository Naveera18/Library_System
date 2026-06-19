# Library Management System (CLI + File Handling)

A console-based library management system written in modern C++ (C++17), built with
object-oriented design and persistent file storage. Supports book/member management,
borrowing & returns with overdue fines, search, reporting, CSV export, and a role-based
login system (admin/member).

## Project Structure

```
library_system/
├── include/              # Class headers
│   ├── Book.h
│   ├── Member.h
│   ├── Transaction.h
│   ├── FileManager.h
│   ├── Library.h
│   └── Auth.h
├── src/                  # Class implementations + entry point
│   ├── Book.cpp
│   ├── Member.cpp
│   ├── Transaction.cpp
│   ├── FileManager.cpp
│   ├── Library.cpp
│   ├── Auth.cpp
│   └── main.cpp
├── data/                 # Auto-created at runtime; persisted records live here
│   ├── books.txt
│   ├── members.txt
│   ├── transactions.txt
│   └── admin_credentials.txt
├── Makefile
└── README.md
```

## Design Overview

- **`Book`, `Member`, `Transaction`** — plain data classes with private fields,
  getters/setters, and `toFileString()` / `fromFileString()` methods for
  pipe-delimited (`|`) file serialization.
- **`FileManager`** — owns all file I/O. Nothing outside this class touches
  `std::ifstream`/`std::ofstream` for the core data files, keeping persistence
  logic in one place.
- **`Library`** — the core orchestrator. Holds in-memory `std::vector`s of
  books/members/transactions (loaded from disk at startup) and writes back to
  disk after every mutating operation, so the system never loses data even if
  it's closed unexpectedly between menu actions. Implements book/member CRUD,
  issue/return logic, search, and reporting.
- **`Auth`** — simple role-based login (bonus feature). Distinguishes Admin vs
  Member sessions and restricts the menu accordingly.
- **`main.cpp`** — the menu-driven CLI loop, with input validation helpers
  (`readInt`, `readLine`) that reject malformed input and re-prompt instead of
  crashing.

### Overdue / Fine logic
The system uses a simple simulated day-counter (`currentDay`) instead of wall-clock
dates, exactly as the task spec allows ("simple logic based on days"). Each issued
book gets a `dueDay = currentDay + loanDays`. Use menu option **17 (Admin)** to
advance simulated time for testing overdue scenarios and fines. Fine = `overdueDays
× finePerDay` (default $10/day).

## Building

Requires a C++17-capable compiler (g++ 7+ / clang++ 5+).

```bash
make          # builds ./library_system
make clean    # removes build artifacts
```

## How To Run
g++ -std=c++17 -Wall -Iinclude src/Book.cpp src/Member.cpp src/Transaction.cpp src/FileManager.cpp src/Library.cpp src/Auth.cpp src/main.cpp -o library_system.exe

.\library_system.exe


## Running

```bash
./library_system
```

On first run, a `data/` folder is created automatically with empty data files and
a default admin account.

**Default admin login:** username `admin`, password `admin123`
**Member login:** enter the Member ID, then a password equal to the ID typed twice
(e.g., Member ID `5` → password `55`). This is a simplified scheme for
demonstrating role separation per the bonus requirement; a production system
would use per-member passwords and hashing.

## Features Implemented

| Requirement | Status |
|---|---|
| Book add/update/delete/view | ✅ |
| Member add/view/borrow history | ✅ |
| Issue / return with availability checks | ✅ |
| File persistence (load on start, save after every op) | ✅ |
| Search by ID / title / author | ✅ |
| Most borrowed books report | ✅ |
| Currently issued books report | ✅ |
| Overdue tracking (day-based) | ✅ |
| **Bonus:** Fine system for late returns | ✅ |
| **Bonus:** Admin/member login system | ✅ |
| **Bonus:** Export reports to CSV | ✅ |
| **Bonus:** Menu-driven UI | ✅ |

## Sample Workflow

1. Run `./library_system`, log in as admin.
2. Add a few books and members.
3. Issue a book to a member (choose a loan period, e.g. 14 days).
4. Use option 17 to "advance" simulated days past the due date.
5. Check the **Overdue Books** report (option 15) — it'll show the fine owed.
6. Return the book (option 9) — the fine is shown at return time.
7. Export everything to CSV (option 16) for a spreadsheet-friendly report.

## Notes on Data Files

All data is stored as plain pipe-delimited text under `data/` for transparency
and easy debugging:

```
books.txt:        id|title|author|category|available(1/0)
members.txt:      id|name|contact|activeBorrowCount
transactions.txt: txnId|bookId|memberId|issueDay|dueDay|returnDay|returned(1/0)
```

Delete the `data/` folder to reset the system to a clean state.
