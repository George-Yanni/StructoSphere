#include <iostream>
#include <string>

// Define a structure for a song
struct Song {
    std::string title;
    std::string artist;
    int duration; // Duration in seconds
    Song* next;   // Pointer to the next song in the list

    Song(std::string t, std::string a, int d) : title(t), artist(a), duration(d), next(nullptr) {}
};

// Define a class for the playlist
class Playlist {
private:
    Song* head; // Pointer to the first song in the playlist

public:
    Playlist() : head(nullptr) {}

    // Destructor to free memory
    ~Playlist() {
        Song* current = head;
        while (current != nullptr) {
            Song* next = current->next;
            delete current;
            current = next;
        }
    }

    // Add a song to the end of the playlist
    void addSong(std::string title, std::string artist, int duration) {
        Song* newSong = new Song(title, artist, duration);
        if (head == nullptr) {
            head = newSong;
        } else {
            Song* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newSong;
        }
    }

    // Remove a song by title
    void removeSong(std::string title) {
        if (head == nullptr) {
            std::cout << "Playlist is empty." << std::endl;
            return;
        }

        if (head->title == title) {
            Song* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        Song* current = head;
        while (current->next != nullptr && current->next->title != title) {
            current = current->next;
        }

        if (current->next == nullptr) {
            std::cout << "Song not found." << std::endl;
            return;
        }

        Song* temp = current->next;
        current->next = current->next->next;
        delete temp;
    }

    // Display the playlist
    void displayPlaylist() const {
        if (head == nullptr) {
            std::cout << "Playlist is empty." << std::endl;
            return;
        }

        Song* current = head;
        while (current != nullptr) {
            std::cout << "Title: " << current->title << ", Artist: " << current->artist
                      << ", Duration: " << current->duration << " seconds" << std::endl;
            current = current->next;
        }
    }
};

int main() {
    Playlist myPlaylist;

    myPlaylist.addSong("Shape of You", "Ed Sheeran", 274);
    myPlaylist.addSong("Blinding Lights", "The Weeknd", 200);
    myPlaylist.addSong("Someone You Loved", "Lewis Capaldi", 182);

    std::cout << "Playlist:" << std::endl;
    myPlaylist.displayPlaylist();

    std::cout << "\nRemoving a song..." << std::endl;
    myPlaylist.removeSong("Blinding Lights");

    std::cout << "\nUpdated Playlist:" << std::endl;
    myPlaylist.displayPlaylist();

    return 0;
}
