#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
#include <limits>
using namespace std;

class Book {                                  // OOP: class + encapsulation
    int id; string title; bool issued = false;
public:
    Book(int i, string t) : id(i), title(t) {}
    virtual ~Book() {}
    int getId() const { return id; }
    bool isIssued() const { return issued; }
    void setIssued(bool s) { issued = s; }
    virtual void show() const {               // polymorphism (virtual)
        cout << id << " | " << title << " | " << (issued ? "Issued" : "Available") << "\n";
    }
};

class EBook : public Book {                   // OOP: inheritance
    double size;
public:
    EBook(int i, string t, double s) : Book(i, t), size(s) {}
    void show() const override { Book::show(); cout << "   (E-Book, " << size << " MB)\n"; }
};

class Library {
    vector<shared_ptr<Book>> books;           // STL: vector
    map<int, string> members;                 // STL: map
    shared_ptr<Book> find(int id) {
        for (auto& b : books) if (b->getId() == id) return b;
        throw runtime_error("Book not found!");   // exception handling
    }
public:
    void addBook(shared_ptr<Book> b) { books.push_back(b); }
    void addMember(int id, string name) { members[id] = name; }
    void showAll() const { for (auto& b : books) b->show(); }
    void issue(int bookId, int memId) {
        if (!members.count(memId)) throw runtime_error("Member not found!");
        auto b = find(bookId);
        if (b->isIssued()) throw runtime_error("Book already issued!");
        b->setIssued(true);
        cout << "Issued to " << members[memId] << "\n";
    }
    void giveBack(int bookId) {
        auto b = find(bookId);
        if (!b->isIssued()) throw runtime_error("Book was not issued!");
        b->setIssued(false); cout << "Returned.\n";
    }
};

int main() {
    Library lib;
    lib.addBook(make_shared<Book>(1, "C++ Primer"));
    lib.addBook(make_shared<EBook>(2, "Effective Modern C++", 3.5));
    lib.addMember(101, "Alice");
    int ch = -1, b, m;
    do {
        cout << "\n1.Show Books 2.Issue 3.Return 0.Exit : ";
        if (!(cin >> ch)) {                   // handle non-numeric / EOF input
            if (cin.eof()) break;
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); ch = -1;
        }
        try {
            if (ch == 1) lib.showAll();
            else if (ch == 2) { cout << "Book ID, Member ID: "; cin >> b >> m; lib.issue(b, m); }
            else if (ch == 3) { cout << "Book ID: "; cin >> b; lib.giveBack(b); }
        } catch (exception& e) { cout << "Error: " << e.what() << "\n"; }
    } while (ch != 0);
    cout << "Goodbye!\n";
}