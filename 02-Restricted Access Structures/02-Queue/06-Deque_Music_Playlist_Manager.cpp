#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

// Song class to represent a track in the playlist
class Song {
public:
    string title;
    string artist;

    Song(string t, string a) : title(t), artist(a) {}
};

// Node class for the doubly linked list used in Deque
class Node {
public:
    Song data;
    Node* prev;
    Node* next;

    Node(const Song& s) : data(s), prev(nullptr), next(nullptr) {}
};

// Deque class to manage the playlist
class Deque {
private:
    Node* front;  // Pointer to the front of the deque
    Node* back;   // Pointer to the back of the deque
    int size;     // Current number of elements in the deque

public:
    // Constructor
    Deque() : front(nullptr), back(nullptr), size(0) {}

    // Destructor to free memory
    ~Deque() {
        while (!isEmpty()) {
            removeFront();
        }
    }

    // Check if the deque is empty
    bool isEmpty() const {
        return size == 0;
    }

    // Get the current size of the deque
    int getSize() const {
        return size;
    }

    // Add a song to the front of the deque
    void addFront(const Song& song) {
        Node* newNode = new Node(song);
        if (isEmpty()) {
            front = back = newNode;
        } else {
            newNode->next = front;
            front->prev = newNode;
            front = newNode;
        }
        size++;
    }

    // Add a song to the back of the deque
    void addBack(const Song& song) {
        Node* newNode = new Node(song);
        if (isEmpty()) {
            front = back = newNode;
        } else {
            newNode->prev = back;
            back->next = newNode;
            back = newNode;
        }
        size++;
    }

    // Remove and return the song from the front
    Song removeFront() {
        if (isEmpty()) {
            throw runtime_error("Deque is empty, cannot remove from front.");
        }
        Node* temp = front;
        Song song = temp->data;
        front = front->next;
        if (front) {
            front->prev = nullptr;
        } else {
            back = nullptr; // Deque is now empty
        }
        delete temp;
        size--;
        return song;
    }

    // Remove and return the song from the back
    Song removeBack() {
        if (isEmpty()) {
            throw runtime_error("Deque is empty, cannot remove from back.");
        }
        Node* temp = back;
        Song song = temp->data;
        back = back->prev;
        if (back) {
            back->next = nullptr;
        } else {
            front = nullptr; // Deque is now empty
        }
        delete temp;
        size--;
        return song;
    }

    // Display the entire playlist
    void display() const {
        if (isEmpty()) {
            cout << "Playlist is empty.\n";
            return;
        }
        Node* current = front;
        cout << "Playlist (" << size << " songs):\n";
        int index = 1;
        while (current != nullptr) {
            cout << index++ << ". " << current->data.title 
                 << " by " << current->data.artist << "\n";
            current = current->next;
        }
    }
};

// PlaylistManager class to simulate the application
class PlaylistManager {
private:
    Deque playlist;

public:
    // Add a song to play next (front)
    void addPlayNext(const string& title, const string& artist) {
        Song song(title, artist);
        playlist.addFront(song);
        cout << "Added '" << title << "' to play next.\n";
    }

    // Add a song to the end of the queue (back)
    void addToQueue(const string& title, const string& artist) {
        Song song(title, artist);
        playlist.addBack(song);
        cout << "Added '" << title << "' to the end of the queue.\n";
    }

    // Play (remove) the current song from the front
    void playCurrent() {
        try {
            Song current = playlist.removeFront();
            cout << "Playing: '" << current.title << "' by " 
                 << current.artist << "\n";
        } catch (const runtime_error& e) {
            cout << e.what() << "\n";
        }
    }

    // Skip the last song in the queue (remove from back)
    void skipLast() {
        try {
            Song last = playlist.removeBack();
            cout << "Skipped: '" << last.title << "' by " 
                 << last.artist << "\n";
        } catch (const runtime_error& e) {
            cout << e.what() << "\n";
        }
    }

    // Show the current playlist
    void showPlaylist() const {
        playlist.display();
    }
};

// Main function to test the PlaylistManager
int main() {
    PlaylistManager pm;

    // Test the playlist functionality
    pm.addToQueue("Bohemian Rhapsody", "Queen");
    pm.addToQueue("Shape of You", "Ed Sheeran");
    pm.addPlayNext("Imagine", "John Lennon");
    pm.showPlaylist();

    cout << "\n";
    pm.playCurrent();  // Play the first song
    pm.showPlaylist();

    cout << "\n";
    pm.addToQueue("Hotel California", "Eagles");
    pm.showPlaylist();

    cout << "\n";
    pm.skipLast();  // Remove the last song
    pm.showPlaylist();

    return 0;
}