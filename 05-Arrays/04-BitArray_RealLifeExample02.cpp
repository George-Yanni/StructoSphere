#include <iostream>
#include <bitset>

using namespace std;

#define DATABASE_SIZE 10  // Number of records

void demonstrateBitmapIndexing() {
    // Bitmap to represent the presence of items in a database
    bitset<DATABASE_SIZE> bitmap;

    // Marking items present in the database (1 for present, 0 for absent)
    bitmap.set(2);  // Item 3 is present
    bitmap.set(5);  // Item 6 is present
    bitmap.set(8);  // Item 9 is present

    cout << "Bitmap Indexing: " << bitmap << endl;  // Output: 0010100101

    // Check if a specific item is present in the database
    int itemIndex = 5;
    if (bitmap.test(itemIndex)) {
        cout << "Item " << itemIndex + 1 << " is present in the database." << endl;
    } else {
        cout << "Item " << itemIndex + 1 << " is not present in the database." << endl;
    }

    // Mark an item as absent (reset bit)
    bitmap.reset(2);  // Mark Item 3 as absent
    cout << "Bitmap after removing Item 3: " << bitmap << endl;  // Output: 0000100101
}

int main() {
    cout << "Demonstrating Bitmap Indexing with Bit Array:" << endl << endl;
    demonstrateBitmapIndexing();
    return 0;
}

//Illustration of Bitmap Indexing:
// Bitmap Indexing: We use a bit array (bitset) to represent whether each item in a database is present or not. Each bit corresponds to an item in the database.

// This technique is useful when performing quick searches to check the presence of records, without storing additional data.