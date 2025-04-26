#ifndef TREAP_RLE_IDS_CPP
#define TREAP_RLE_IDS_CPP

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <regex>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <tuple> // Include for std::tuple
#include <chrono> // For seeding random number generator

// Represents a node in the Treap
class TreapNode {
public:
    std::string ip;                     // IP address (key for BST)
    double priority;                    // Random priority (key for Heap)
    int score;                          // Suspicion score
    std::vector<std::string> timestamps; // Timestamps of events for this IP
    TreapNode* left;
    TreapNode* right;

    TreapNode(const std::string& ip_addr, double prio, int initial_score, const std::string& timestamp)
        : ip(ip_addr), priority(prio), score(initial_score), left(nullptr), right(nullptr) {
        timestamps.push_back(timestamp);
    }

    // Basic destructor (vector manages its own memory)
    ~TreapNode() = default;
};

// Intrusion Detection System implemented using a Treap
class TreapIDS {
private:
    TreapNode* root;
    mutable std::ofstream log_file; // Marked as mutable to allow writing in const methods
    std::mt19937 rng; // Mersenne Twister random number generator

    // --- Validation ---

    // Validates IPv4 address format using regex
    bool validate_ip(const std::string& ip) const {
        // Regex for standard IPv4 format
        const std::regex ip_pattern(
            "^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
            "(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"
        );
        return std::regex_match(ip, ip_pattern);
    }

    // Validates timestamp format (YYYY-MM-DDTHH:MM:SS)
    bool validate_timestamp(const std::string& timestamp) const {
         std::tm tm = {};
         std::istringstream ss(timestamp);
         // Attempt to parse the timestamp according to the expected format
         ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
         // Check if parsing failed OR if the entire string wasn't consumed
         if (ss.fail() || !ss.eof()) {
             return false;
         }
         return true; // Parsing succeeded and entire string was consumed
    }

    // --- Treap Operations ---

    // Performs a right rotation at the given node to maintain heap property
    TreapNode* rotate_right(TreapNode* node) {
        if (!node || !node->left) return node; // Cannot rotate right without a left child
        TreapNode* new_root = node->left;
        node->left = new_root->right;
        new_root->right = node;
        // Note: Size/count updates would go here if maintained
        return new_root;
    }

    // Performs a left rotation at the given node to maintain heap property
    TreapNode* rotate_left(TreapNode* node) {
        if (!node || !node->right) return node; // Cannot rotate left without a right child
        TreapNode* new_root = node->right;
        node->right = new_root->left;
        new_root->left = node;
        // Note: Size/count updates would go here if maintained
        return new_root;
    }

    // Recursive helper function to insert a node into the Treap
    TreapNode* insert(TreapNode* node, const std::string& ip, double priority, int score, const std::string& timestamp) {
        // Base case: Found the spot to insert
        if (!node) {
            return new TreapNode(ip, priority, score, timestamp);
        }

        // If IP already exists, update score and add timestamp
        if (ip == node->ip) {
            node->score += score;
            node->timestamps.push_back(timestamp);
            log_file << current_time() << " - INFO - Updated IP: " << ip
                     << ", score incremented by " << score << ", new score: " << node->score
                     << ", timestamp: " << timestamp << "\n";
            return node; // No structural change needed
        }

        // Navigate based on IP (BST property)
        if (ip < node->ip) {
            node->left = insert(node->left, ip, priority, score, timestamp);
            // Maintain heap property: if left child has higher priority, rotate right
            if (node->left && node->left->priority > node->priority) {
                node = rotate_right(node);
            }
        } else { // ip > node->ip
            node->right = insert(node->right, ip, priority, score, timestamp);
            // Maintain heap property: if right child has higher priority, rotate left
            if (node->right && node->right->priority > node->priority) {
                node = rotate_left(node);
            }
        }
        return node;
    }

    // Recursive helper function to delete a node from the Treap
    TreapNode* delete_node(TreapNode* node, const std::string& ip) {
        if (!node) {
            return nullptr; // IP not found
        }

        if (ip < node->ip) {
            node->left = delete_node(node->left, ip);
        } else if (ip > node->ip) {
            node->right = delete_node(node->right, ip);
        } else { // Found the node to delete (node->ip == ip)
            // Case 1: Node has no left child
            if (!node->left) {
                TreapNode* temp = node->right;
                delete node; // Free memory
                return temp; // Replace node with its right child
            }
            // Case 2: Node has no right child
            if (!node->right) {
                TreapNode* temp = node->left;
                delete node; // Free memory
                return temp; // Replace node with its left child
            }
            // Case 3: Node has both children
            // Rotate the node down based on child priorities until it becomes a leaf or has one child
            if (node->left->priority > node->right->priority) {
                node = rotate_right(node);
                // Recursively delete from the new right subtree
                node->right = delete_node(node->right, ip);
            } else {
                node = rotate_left(node);
                // Recursively delete from the new left subtree
                node->left = delete_node(node->left, ip);
            }
        }
        return node; // Return the (potentially new) root of this subtree
    }

    // Recursive helper function to search for an IP
    // Returns const pointer as it doesn't modify the node
    const TreapNode* search(const TreapNode* node, const std::string& ip) const {
        if (!node || node->ip == ip) {
            return node; // Found or not present in this subtree
        }
        if (ip < node->ip) {
            return search(node->left, ip);
        } else { // ip > node->ip
            return search(node->right, ip);
        }
    }

    // Overload for non-const search if needed internally (e.g., before delete)
    // Not strictly necessary for current implementation but good practice
    TreapNode* search(TreapNode* node, const std::string& ip) {
         if (!node || node->ip == ip) {
            return node; // Found or not present in this subtree
        }
        if (ip < node->ip) {
            return search(node->left, ip);
        } else { // ip > node->ip
            return search(node->right, ip);
        }
    }


    // Recursive helper for range query (in-order traversal within the range)
    void range_query(const TreapNode* node, const std::string& start_ip, const std::string& end_ip,
                     std::vector<std::tuple<std::string, int, std::vector<std::string>>>& result) const {
        if (!node) {
            return;
        }

        // If current node's IP is less than the start of the range,
        // only need to check the right subtree.
        if (node->ip < start_ip) {
            range_query(node->right, start_ip, end_ip, result);
        }
        // If current node's IP is greater than the end of the range,
        // only need to check the left subtree.
        else if (node->ip > end_ip) {
            range_query(node->left, start_ip, end_ip, result);
        }
        // If current node's IP is within the range [start_ip, end_ip]
        else {
            // Recursively check left subtree (for IPs >= start_ip)
            range_query(node->left, start_ip, end_ip, result);
            // Add current node's data to the result
            result.emplace_back(node->ip, node->score, node->timestamps);
            // Recursively check right subtree (for IPs <= end_ip)
            range_query(node->right, start_ip, end_ip, result);
        }
    }

    // Recursive helper for in-order traversal (collects all IPs and scores)
    void inorder(const TreapNode* node, std::vector<std::pair<std::string, int>>& result) const {
        if (node) {
            inorder(node->left, result);
            result.emplace_back(node->ip, node->score);
            inorder(node->right, result);
        }
    }

    // Recursively deletes all nodes in the Treap (post-order traversal)
    void cleanup(TreapNode* node) {
        if (node) {
            cleanup(node->left);
            cleanup(node->right);
            delete node;
        }
    }

    // --- Utility ---

    // Gets the current time as a formatted string (YYYY-MM-DDTHH:MM:SS)
    std::string current_time() const {
        auto now_p = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now_p);
        // Using thread-safe version if available (C++11 onwards, but check compiler support)
        // std::tm now_tm;
        // localtime_s(&now_tm, &now_c); // Windows/MSVC specific
        // localtime_r(&now_c, &now_tm); // POSIX specific
        // For portability, stick with std::localtime but be aware of thread-safety issues
        std::tm now_tm = *std::localtime(&now_c);
        std::stringstream ss;
        ss << std::put_time(&now_tm, "%Y-%m-%dT%H:%M:%S"); // Consistent format
        return ss.str();
    }

public:
    // Constructor: Initializes empty Treap, seeds RNG, opens log file
    TreapIDS(const std::string& log_filename = "ids_log.txt") : root(nullptr) {
        // Seed the random number generator more robustly
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        rng.seed(seed);

        log_file.open(log_filename, std::ios::app); // Append mode
        if (!log_file.is_open()) {
            // Use cerr for critical errors like failing to open log
            std::cerr << "CRITICAL ERROR: Failed to open log file: " << log_filename << std::endl;
            throw std::runtime_error("Failed to open log file: " + log_filename);
        }
        log_file << current_time() << " - INFO - IDS Initialized. Logging to " << log_filename << "\n";
    }

    // Destructor: Cleans up all nodes and closes the log file
    ~TreapIDS() {
        // Ensure log file is open before writing shutdown message
        if (log_file.is_open()) {
             log_file << current_time() << " - INFO - IDS Shutting down.\n";
        }
        cleanup(root);
        root = nullptr; // Good practice to nullify root after cleanup
        if (log_file.is_open()) {
            log_file.close();
        }
    }

    // Public interface to insert an IP event
    void insert(const std::string& ip, int score, const std::string& timestamp) {
        if (!validate_ip(ip)) {
            log_file << current_time() << " - ERROR - Invalid IP address format for insertion: " << ip << "\n";
            throw std::invalid_argument("Invalid IP address format: " + ip);
        }
        if (!validate_timestamp(timestamp)) {
            log_file << current_time() << " - ERROR - Invalid timestamp format for insertion: " << timestamp << "\n";
            throw std::invalid_argument("Invalid timestamp format: " + timestamp);
        }

        // Generate a random priority for the new node (if it's a new IP)
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        double priority = dist(rng);

        // Check if IP exists before calling insert to log accurately
        bool existed_before = (search(root, ip) != nullptr);

        root = insert(root, ip, priority, score, timestamp); // Call recursive helper

        // Log insertion only if it was a new IP (update is logged in helper)
        if (!existed_before) {
             log_file << current_time() << " - INFO - Inserted new IP: " << ip << " with score: " << score << " at " << timestamp << "\n";
        }
        // Log processing regardless (optional, could be verbose)
        // log_file << current_time() << " - DEBUG - Processed insertion request for IP: " << ip << "\n";
    }

    // Public interface to remove an IP and all its associated data
    void remove(const std::string& ip) {
        if (!validate_ip(ip)) {
            log_file << current_time() << " - ERROR - Invalid IP address format for deletion: " << ip << "\n";
            throw std::invalid_argument("Invalid IP address format: " + ip);
        }
        // Use the non-const search helper to find the node
        if (!search(root, ip)) { // Check if node exists before attempting deletion
             log_file << current_time() << " - WARNING - Attempted to delete non-existent IP: " << ip << "\n";
             return; // Or throw an exception if preferred
        }

        root = delete_node(root, ip); // Call recursive helper
        log_file << current_time() << " - INFO - Deleted IP: " << ip << "\n";
    }

    // Public interface to search for an IP (const version)
    // Returns const pointer to the node if found, nullptr otherwise
    const TreapNode* search(const std::string& ip) const {
        if (!validate_ip(ip)) {
            log_file << current_time() << " - ERROR - Invalid IP address format for search: " << ip << "\n";
            throw std::invalid_argument("Invalid IP address format: " + ip);
        }
        const TreapNode* node = search(root, ip); // Call recursive const helper
        if (node) {
            log_file << current_time() << " - INFO - Found IP: " << ip << ", score: " << node->score
                     << ", timestamps count: " << node->timestamps.size() << "\n";
            // Optionally log all timestamps (can be verbose)
            // log_file << "    Timestamps: [";
            // for (size_t i = 0; i < node->timestamps.size(); ++i) {
            //     log_file << node->timestamps[i] << (i < node->timestamps.size() - 1 ? ", " : "");
            // }
            // log_file << "]\n";
        } else {
             log_file << current_time() << " - INFO - IP not found: " << ip << "\n";
        }
        return node;
    }

    // Public interface for range query
    std::vector<std::tuple<std::string, int, std::vector<std::string>>> range_query(const std::string& start_ip, const std::string& end_ip) const {
        if (!validate_ip(start_ip)) {
             log_file << current_time() << " - ERROR - Invalid start IP address for range query: " << start_ip << "\n";
            throw std::invalid_argument("Invalid start IP address format: " + start_ip);
        }
         if (!validate_ip(end_ip)) {
             log_file << current_time() << " - ERROR - Invalid end IP address for range query: " << end_ip << "\n";
            throw std::invalid_argument("Invalid end IP address format: " + end_ip);
        }
        // Simple string comparison works correctly for lexicographical IP ordering
        if (start_ip > end_ip) {
             log_file << current_time() << " - ERROR - Invalid IP range: start IP (" << start_ip
                      << ") > end IP (" << end_ip << ")\n";
            throw std::invalid_argument("Invalid IP range: start_ip cannot be greater than end_ip");
        }

        std::vector<std::tuple<std::string, int, std::vector<std::string>>> result;
        range_query(root, start_ip, end_ip, result); // Call recursive helper
        log_file << current_time() << " - INFO - Range query [" << start_ip << " to " << end_ip
                 << "]: " << result.size() << " IPs found.\n";
        return result;
    }

    // Gets the top k IPs with the highest scores
    std::vector<std::pair<std::string, int>> get_top_suspicious(size_t k) const {
        if (k == 0) {
             log_file << current_time() << " - INFO - Requested top 0 suspicious IPs.\n";
             return {}; // Return empty vector
        }

        std::vector<std::pair<std::string, int>> all_ips;
        inorder(root, all_ips); // Get all IPs and scores using the const helper

        // Use std::partial_sort for efficiency if k is much smaller than total IPs
        if (k < all_ips.size()) {
            std::partial_sort(all_ips.begin(), all_ips.begin() + k, all_ips.end(),
                              [](const auto& a, const auto& b) {
                                  return a.second > b.second; // Higher score first
                              });
            // Resize the vector to keep only the top k elements
            all_ips.resize(k);
        } else {
            // If k >= size, just sort the whole vector
            std::sort(all_ips.begin(), all_ips.end(),
                      [](const auto& a, const auto& b) {
                          return a.second > b.second; // Higher score first
                      });
        }

        // Log the result
        log_file << current_time() << " - INFO - Top " << all_ips.size() << " (requested " << k << ") suspicious IPs: [";
        for (size_t i = 0; i < all_ips.size(); ++i) {
            log_file << "(" << all_ips[i].first << ", " << all_ips[i].second << ")";
            if (i < all_ips.size() - 1) log_file << ", ";
        }
        log_file << "]\n";

        return all_ips; // Return the (partially) sorted vector
    }
};

// --- Simulation ---

// Generates a timestamp string for the current time
std::string get_current_timestamp() {
    auto now_p = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now_p);
    // std::tm now_tm;
    // localtime_s(&now_tm, &now_c); // Windows/MSVC specific
    // localtime_r(&now_c, &now_tm); // POSIX specific
    std::tm now_tm = *std::localtime(&now_c); // Portable but potentially not thread-safe
    std::stringstream ss;
    ss << std::put_time(&now_tm, "%Y-%m-%dT%H:%M:%S");
    return ss.str();
}

// Simulates network traffic and demonstrates IDS functionality
void simulate_network_traffic() {
    std::cout << "Starting Network Traffic Simulation..." << std::endl;
    TreapIDS ids; // Uses default log file "ids_log.txt"

    // Simulate IP access attempts with current timestamps
    std::vector<std::tuple<std::string, int, std::string>> traffic = {
        {"192.168.1.10", 1, get_current_timestamp()},
        {"10.0.0.5", 5, get_current_timestamp()},
        {"192.168.1.20", 3, get_current_timestamp()},
        {"192.168.1.10", 2, get_current_timestamp()}, // Repeated access, score increases
        {"172.16.0.100", 10, get_current_timestamp()},
        {"192.168.1.30", 5, get_current_timestamp()},
        {"10.0.0.5", 7, get_current_timestamp()},     // Repeated access
        {"192.168.1.40", 1, get_current_timestamp()},
        {"192.168.1.10", 4, get_current_timestamp()}  // Repeated access
    };

    std::cout << "\n--- Processing Traffic ---" << std::endl;
    for (const auto& [ip, score, timestamp] : traffic) {
        try {
            std::cout << "Inserting/Updating: IP=" << ip << ", Score=" << score << ", Time=" << timestamp << std::endl;
            ids.insert(ip, score, timestamp);
        } catch (const std::exception& e) {
            std::cerr << "Error during insertion: " << e.what() << std::endl;
        }
    }

    // Search for specific IPs
    std::cout << "\n--- Searching for IPs ---" << std::endl;
    std::vector<std::string> ips_to_search = {"192.168.1.10", "10.0.0.5", "1.2.3.4"}; // Last one shouldn't exist
    for(const auto& ip : ips_to_search) {
        try {
            std::cout << "Searching for: " << ip << "..." << std::endl;
            const TreapNode* node = ids.search(ip); // Use the public const search
            if (node) {
                std::cout << "  Found - IP: " << node->ip << ", Score: " << node->score << ", Timestamps: [";
                for (size_t i = 0; i < node->timestamps.size(); ++i) {
                    std::cout << node->timestamps[i] << (i < node->timestamps.size() - 1 ? ", " : "");
                }
                std::cout << "]" << std::endl;
            } else {
                std::cout << "  Not Found." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error during search: " << e.what() << std::endl;
        }
    }


    // Range query for a subnet
    std::cout << "\n--- Range Query [192.168.1.10 to 192.168.1.35] ---" << std::endl;
    try {
        auto subnet_ips = ids.range_query("192.168.1.10", "192.168.1.35");
        std::cout << "IPs found in subnet:" << std::endl;
        if (subnet_ips.empty()) {
            std::cout << "  No IPs found in this range." << std::endl;
        } else {
            for (const auto& [ip, score, timestamps] : subnet_ips) {
                std::cout << "  IP: " << ip << ", Score: " << score << ", Timestamps Count: " << timestamps.size() << std::endl;
                // Optionally print all timestamps
                // std::cout << "    Timestamps: [";
                // for (size_t i = 0; i < timestamps.size(); ++i) {
                //     std::cout << timestamps[i] << (i < timestamps.size() - 1 ? ", " : "");
                // }
                // std::cout << "]" << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error during range query: " << e.what() << std::endl;
    }

    // Get top suspicious IPs
    std::cout << "\n--- Top 3 Suspicious IPs ---" << std::endl;
    try {
        auto top_ips = ids.get_top_suspicious(3);
        std::cout << "Top IPs by score:" << std::endl;
         if (top_ips.empty()) {
            std::cout << "  No IPs recorded yet." << std::endl;
        } else {
            for (const auto& [ip, score] : top_ips) {
                std::cout << "  IP: " << ip << ", Score: " << score << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error getting top suspicious IPs: " << e.what() << std::endl;
    }

    // Delete an IP
    std::cout << "\n--- Deleting IP ---" << std::endl;
    std::string ip_to_delete = "10.0.0.5";
    try {
        std::cout << "Attempting to delete IP: " << ip_to_delete << std::endl;
        ids.remove(ip_to_delete);
        std::cout << "  Deletion request processed." << std::endl; // Changed message slightly
        // Verify deletion
        std::cout << "Verifying deletion by searching for " << ip_to_delete << "..." << std::endl;
        const TreapNode* node = ids.search(ip_to_delete); // Use public const search
         if (!node) {
             std::cout << "  Verification successful: IP " << ip_to_delete << " not found." << std::endl;
         } else {
             // This case should ideally not happen if remove worked correctly
             std::cout << "  Verification FAILED: IP " << ip_to_delete << " still found! Score: " << node->score << std::endl;
         }
    } catch (const std::exception& e) {
        std::cerr << "Error during deletion: " << e.what() << std::endl;
    }

     // Attempt to delete a non-existent IP
    std::cout << "\n--- Deleting Non-Existent IP ---" << std::endl;
    ip_to_delete = "1.1.1.1";
    try {
        std::cout << "Attempting to delete IP: " << ip_to_delete << std::endl;
        ids.remove(ip_to_delete); // Should log a warning but not throw
        std::cout << "  Deletion attempt completed (check log for warnings)." << std::endl;
    } catch (const std::exception& e) {
        // Catch potential invalid_argument from validate_ip if it were invalid format
        std::cerr << "Error during deletion attempt: " << e.what() << std::endl;
    }


    std::cout << "\nSimulation Finished. Check 'ids_log.txt' for detailed logs." << std::endl;
}

int main() {
    try {
        simulate_network_traffic();
    } catch (const std::exception& e) {
        // Catch exceptions that might propagate from TreapIDS constructor or simulate_network_traffic
        std::cerr << "FATAL ERROR in main: " << e.what() << std::endl;
        return 1; // Indicate error
    } catch (...) {
        std::cerr << "Unknown FATAL ERROR in main." << std::endl;
        return 1; // Indicate error
    }
    return 0; // Indicate success
}

#endif // TREAP_RLE_IDS_CPP
