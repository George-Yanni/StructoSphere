#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class SuffixArray {
private:
    std::string text;
    std::vector<int> sa;   // Suffix array
    std::vector<int> lcp;  // Longest Common Prefix array

public:
    SuffixArray(const std::string& s) : text(s) {
        buildSuffixArray();
        buildLCP();
    }

    void buildSuffixArray() {
        int n = text.size();
        sa.resize(n);
        for (int i = 0; i < n; ++i) sa[i] = i;

        // Optimized comparator: avoid substr
        std::sort(sa.begin(), sa.end(), [&](int a, int b) {
            while (a < n && b < n) {
                if (text[a] != text[b]) return text[a] < text[b];
                ++a; ++b;
            }
            return a == n;
        });
    }

    void buildLCP() {
        int n = text.size();
        lcp.resize(n, 0);
        std::vector<int> rank(n, 0);

        for (int i = 0; i < n; ++i)
            rank[sa[i]] = i;

        int h = 0;
        for (int i = 0; i < n; ++i) {
            if (rank[i] > 0) {
                int j = sa[rank[i] - 1];
                while (i + h < n && j + h < n && text[i + h] == text[j + h])
                    ++h;
                lcp[rank[i]] = h;
                if (h > 0) --h;
            }
        }
    }

    std::vector<int> searchAll(const std::string& pattern) {
        std::vector<int> result;
        int n = text.size();
        int m = pattern.size();

        int left = 0, right = n - 1;
        int start = -1, end = -1;

        // Find lower bound
        while (left <= right) {
            int mid = (left + right) / 2;
            int cmp = text.compare(sa[mid], m, pattern);
            if (cmp >= 0) {
                right = mid - 1;
                if (cmp == 0) start = mid;
            } else {
                left = mid + 1;
            }
        }

        left = 0, right = n - 1;

        // Find upper bound
        while (left <= right) {
            int mid = (left + right) / 2;
            int cmp = text.compare(sa[mid], m, pattern);
            if (cmp <= 0) {
                left = mid + 1;
                if (cmp == 0) end = mid;
            } else {
                right = mid - 1;
            }
        }

        if (start != -1 && end != -1) {
            for (int i = start; i <= end; ++i)
                result.push_back(sa[i]);
        }

        std::sort(result.begin(), result.end()); // Optional: to return in original order
        return result;
    }

    void printSuffixArray() {
        std::cout << "\nSuffix Array:\n";
        for (int i : sa)
            std::cout << i << ": " << text.substr(i) << "\n";
    }

    void printLCP() {
        std::cout << "\nLCP Array:\n";
        for (int i = 1; i < lcp.size(); ++i)
            std::cout << "LCP between '" << text.substr(sa[i - 1]) << "' and '" << text.substr(sa[i])
                      << "' is " << lcp[i] << "\n";
    }
};

int main() {
    std::string text = "banana";
    SuffixArray sa(text);

    sa.printSuffixArray();
    sa.printLCP();

    std::string pattern = "ana";
    auto positions = sa.searchAll(pattern);

    std::cout << "\nPattern '" << pattern << "' found at positions: ";
    if (positions.empty()) {
        std::cout << "Not found";
    } else {
        for (int pos : positions)
            std::cout << pos << " ";
    }
    std::cout << "\n";

    return 0;
}
