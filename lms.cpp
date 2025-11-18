#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <exception>
using namespace std;

// Abstract Base Class (Pure Virtual)
class Person {
protected:
    string name;
    int id;
public:
    Person(string n = "", int i = 0) : name(n), id(i) {}
    virtual void showInfo() const = 0; // Pure virtual function
    virtual ~Person() {}
};

class Member : public Person {
    static int memberCount;
    int borrowed;
public:
    Member(string n, int i) : Person(n, i), borrowed(0) { memberCount++; }
    void showInfo() const override {
        cout << "Member Name: " << name << ", ID: " << id << ", Books Borrowed: " << borrowed << endl;
    }
    void borrowBook() { borrowed++; }
    void returnBook() { if (borrowed > 0) borrowed--; }
    static int getMemberCount() { return memberCount; }
    int getId() const { return id; }
    ~Member() { memberCount--; }
};
int Member::memberCount = 0;

class Librarian : public Person {
public:
    Librarian(string n, int i) : Person(n, i) {}
    void showInfo() const override {
        cout << "Librarian Name: " << name << ", ID: " << id << endl;
    }
};

class Book {
    string title;
    string author;
    int id;
    bool issued;
public:
    Book(string t = "", string a = "", int i = 0)
        : title(t), author(a), id(i), issued(false) {}
    // Inline Function
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    int getId() const { return id; }
    bool isIssued() const { return issued; }
    void issue() { issued = true; }
    void returnBook() { issued = false; }

    // Operator Overloading
    bool operator==(const Book &b) const {
        return title == b.title && author == b.author && id == b.id;
    }

    // Friend Function Example
    friend ostream& operator<<(ostream&, const Book&);

    void showInfo() const {
        cout << "ID: " << id << ", Title: " << title 
             << ", Author: " << author 
             << ", Issued: " << (issued ? "Yes" : "No") << endl;
    }
};

ostream& operator<<(ostream& os, const Book& b) {
    os << b.id << " | " << b.title << " | " << b.author 
       << " | " << (b.issued ? "Issued" : "Available");
    return os;
}

// Template for search function
template <class T>
int searchItem(const vector<T>& arr, T item) {
    for (size_t i = 0; i < arr.size(); i++) // Use size_t for indexes
        if (arr[i] == item) return static_cast<int>(i);
    return -1;
}

class Library {
    vector<Book> books;
    vector<Member> members;
public:
    // Function Overloading
    void addBook(const Book& b) {
        books.push_back(b);
    }
    void addBook(string t, string a, int i) {
        books.push_back(Book(t, a, i));
    }
    void addMember(const Member& m) {
        members.push_back(m);
    }
    void addMember(string n, int i) {
        members.push_back(Member(n, i));
    }
    // Public Getter for books
    const vector<Book>& getBooks() const {
        return books;
    }
    // Public Getter for members (optional)
    const vector<Member>& getMembers() const {
        return members;
    }

    void showAllBooks() const {
        for (const auto& b : books) b.showInfo();
    }
    void showAllMembers() const {
        for (const auto& m : members) m.showInfo();
    }

    void issueBook(int bookId, int memberId) {
        try {
            int bi = -1, mi = -1;
            for (size_t i = 0; i < books.size(); i++) {
                if (books[i].getId() == bookId && !books[i].isIssued()) {
                    bi = static_cast<int>(i); break;
                }
            }
            for (size_t j = 0; j < members.size(); j++) {
                if (members[j].getId() == memberId) {
                    mi = static_cast<int>(j); break;
                }
            }
            if (bi == -1 || mi == -1)
                throw runtime_error("Book or Member not found.");
            books[bi].issue();
            members[mi].borrowBook();
            cout << "Book issued to member.\n";
        }
        catch (exception &e) {
            cout << "Exception: " << e.what() << endl;
        }
    }

    void saveBooksToFile(string filename) {
        ofstream fout(filename);
        for (const auto &b : books) {
            fout << b << endl;
        }
        fout.close();
    }

    void loadBooksFromFile(string filename) {
        ifstream fin(filename);
        books.clear();
        int i;
        string t, a, status;
        while (fin >> i >> t >> a >> status) {
            Book b(t, a, i);
            if (status == "Issued") b.issue();
            books.push_back(b);
        }
        fin.close();
    }
};

int main() {
    Library lib;
    int choice;

    while (true) {
        cout << "\n===== LIBRARY MENU =====\n";
        cout << "1. Add Book\n";
        cout << "2. Add Member\n";
        cout << "3. Show All Books\n";
        cout << "4. Show All Members\n";
        cout << "5. Issue Book\n";
        cout << "6. Search Book\n";
        cout << "7. Save Books to File\n";
        cout << "8. Load Books from File\n";
        cout << "9. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            string title, author;
            int id;
            cout << "Enter Book Title (no spaces): ";
            cin >> title;
            cout << "Enter Author Name (no spaces): ";
            cin >> author;
            cout << "Enter Book ID: ";
            cin >> id;
            lib.addBook(title, author, id);
            cout << "Book added!\n";
        }
        else if (choice == 2) {
            string name;
            int id;
            cout << "Enter Member Name (no spaces): ";
            cin >> name;
            cout << "Enter Member ID: ";
            cin >> id;
            lib.addMember(name, id);
            cout << "Member added!\n";
        }
        else if (choice == 3) {
            cout << "\n=== All Books ===\n";
            lib.showAllBooks();
        }
        else if (choice == 4) {
            cout << "\n=== All Members ===\n";
            lib.showAllMembers();
        }
        else if (choice == 5) {
            int bookId, memberId;
            cout << "Enter Book ID to issue: ";
            cin >> bookId;
            cout << "Enter Member ID: ";
            cin >> memberId;
            lib.issueBook(bookId, memberId);
        }
        else if (choice == 6) {
            string title, author;
            int id;
            cout << "Enter Book Title (no spaces): ";
            cin >> title;
            cout << "Enter Author: ";
            cin >> author;
            cout << "Enter ID: ";
            cin >> id;

            Book findBook(title, author, id);
            int pos = searchItem(lib.getBooks(), findBook);

            if (pos == -1)
                cout << "Book NOT found!\n";
            else
                cout << "Book found at position: " << pos << endl;
        }
        else if (choice == 7) {
            lib.saveBooksToFile("books.txt");
            cout << "Books saved to books.txt\n";
        }
        else if (choice == 8) {
            lib.loadBooksFromFile("books.txt");
            cout << "Books loaded from books.txt\n";
        }
        else if (choice == 9) {
            cout << "Exiting...\n";
            break;
        }
        else {
            cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}
