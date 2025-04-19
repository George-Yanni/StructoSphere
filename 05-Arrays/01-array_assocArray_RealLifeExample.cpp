#include <iostream>
#include <map>
#include <string>

using namespace std;

struct Member {
    string name;
    int borrowedBooks;
};

// Demonstrating array and map in a Library system
void librarySystem() {
    // Array to store fixed list of books
    string books[] = {"The Great Gatsby", "1984", "To Kill a Mockingbird", "Pride and Prejudice"};
    int numBooks = sizeof(books) / sizeof(books[0]);

    cout << "Library Books (Array): " << endl;
    for (int i = 0; i < numBooks; ++i) {
        cout << i+1 << ". " << books[i] << endl;
    }
    cout << endl;

    // Map to store members (key: Member ID, value: Member info)
    map<int, Member> members;

    // Adding members to the map
    members[101] = {"Alice", 2};
    members[102] = {"Bob", 1};
    members[103] = {"Charlie", 0};

    // Displaying member info
    cout << "Library Members (Associative Array - map):" << endl;
    for (const auto& member : members) {
        cout << "Member ID: " << member.first << " -> Name: " << member.second.name
             << ", Borrowed Books: " << member.second.borrowedBooks << endl;
    }
    cout << endl;

    // Example of member borrowing a book
    int memberId = 102;
    cout << "Updating member (ID: " << memberId << ") who borrowed a book..." << endl;
    members[memberId].borrowedBooks++;  // Bob borrows one more book

    // Display updated member info
    cout << "Updated Member Info:" << endl;
    cout << "Member ID: " << memberId << " -> Name: " 
         << members[memberId].name 
         << ", Borrowed Books: " << members[memberId].borrowedBooks << endl;
}

int main() {
    librarySystem();
    return 0;
}
