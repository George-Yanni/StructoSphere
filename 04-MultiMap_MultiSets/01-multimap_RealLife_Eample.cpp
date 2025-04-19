//Cybersecurity Event Logging System

#include <iostream>
#include <map>
#include <string>
#include <vector>

void printLogs(const std::multimap<std::string, std::string>& logs) {
    for (const auto& [timestamp, log] : logs) {
        std::cout << timestamp << ": " << log << '\n';
    }
}

int main() {
    // Create a multimap where the key is the date and the value is the attack log.
    std::multimap<std::string, std::string> attackLogs;

    // Inserting sample attack logs (date, attack type, source IP)
    attackLogs.insert({"2025-04-18", "DDoS Attack from 192.168.1.10"});
    attackLogs.insert({"2025-04-18", "SQL Injection Attempt from 203.0.113.45"});
    attackLogs.insert({"2025-04-19", "Malware Download from 198.51.100.72"});
    attackLogs.insert({"2025-04-19", "Phishing Attempt from 192.168.2.33"});
    attackLogs.insert({"2025-04-20", "Brute Force Login from 10.0.0.5"});
    attackLogs.insert({"2025-04-20", "DDoS Attack from 192.168.1.12"});
    attackLogs.insert({"2025-04-21", "Privilege Escalation Attempt from 203.0.113.60"});
    
    // Print all logs
    std::cout << "Complete Cybersecurity Attack Logs:\n";
    printLogs(attackLogs);

    // Retrieve all logs for a specific date
    std::cout << "\nLogs for 2025-04-19:\n";
    auto range = attackLogs.equal_range("2025-04-19");
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << it->second << '\n';
    }

    // Add a new attack log for a specific day
    attackLogs.insert({"2025-04-21", "Brute Force Login from 192.168.2.44"});
    std::cout << "\nUpdated Cybersecurity Attack Logs:\n";
    printLogs(attackLogs);

    // Remove all attack logs for a specific date (e.g., to archive them)
    attackLogs.erase("2025-04-18");
    std::cout << "\nAfter Removing 2025-04-18 Logs:\n";
    printLogs(attackLogs);

    return 0;
}
