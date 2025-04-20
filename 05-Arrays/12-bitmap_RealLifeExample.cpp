#include <iostream>
#include <unordered_map>
#include <vector>
#include <thread>
#include <chrono>
using namespace std;

// Simple bitmap class
class Bitmap {
private:
    vector<uint8_t> bits;
    size_t size;

public:
    Bitmap(size_t bitSize) : size(bitSize) {
        bits.resize((bitSize + 7) / 8, 0);
    }

    void set(size_t index) {
        if (index >= size) throw out_of_range("Index out of range");
        bits[index / 8] |= (1 << (index % 8));
    }

    void clear(size_t index) {
        if (index >= size) throw out_of_range("Index out of range");
        bits[index / 8] &= ~(1 << (index % 8));
    }

    bool test(size_t index) const {
        if (index >= size) throw out_of_range("Index out of range");
        return bits[index / 8] & (1 << (index % 8));
    }

    void print() const {
        cout << "Bitmap: ";
        for (int i = static_cast<int>(size) - 1; i >= 0; --i) {
            cout << test(i);
        }
        cout << "\n";
    }
    
};

// Simulated slow data source
int slowDataSource(int key) {
    cout << "Fetching from slow source for key " << key << "...\n";
    this_thread::sleep_for(chrono::milliseconds(500)); // simulate delay
    return key * 10; // dummy data
}

// Cache system using bitmap
class Cache {
private:
    unordered_map<int, int> cacheStorage;  // actual data storage
    Bitmap bitmap;                         // bitmap to track cached items
    size_t maxKey;

public:
    Cache(size_t maxKeyValue) : bitmap(maxKeyValue + 1), maxKey(maxKeyValue) {}

    int get(int key) {
        if (key > maxKey) throw out_of_range("Key out of cache bounds");

        if (bitmap.test(key)) {
            cout << "Cache HIT for key " << key << ": ";
            return cacheStorage[key];
        } else {
            cout << "Cache MISS for key " << key << "\n";
            int data = slowDataSource(key);
            cacheStorage[key] = data;
            bitmap.set(key);
            return data;
        }
    }

    void clear(int key) {
        bitmap.clear(key);
        cacheStorage.erase(key);
    }

    void showBitmap() {
        bitmap.print();
    }
};

int main() {
    Cache myCache(32);  // supports keys from 0 to 32

    // Try accessing some data
    cout << "Data for key 5: " << myCache.get(5) << "\n";
    cout << "Data for key 12: " << myCache.get(12) << "\n";
    cout << "Data for key 5: " << myCache.get(5) << "\n"; // Cached

    // Visualize bitmap
    myCache.showBitmap();

    // Clear a cache entry
    myCache.clear(5);
    cout << "Data for key 5 after clearing: " << myCache.get(5) << "\n"; // Refetch

    return 0;
}
