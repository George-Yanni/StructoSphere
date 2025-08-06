#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

class FMIndex {
private:
    std::string text;
    std::string bwt;
    std::vector<int> suffixArray;
    std::vector<std::vector<int>> occ; // Occurrence table
    std::vector<int> C;                // Count of characters < c

    // Construct BWT using naive suffix array
    std::string computeBWT(const std::string& s) {
        std::string t = s + '$';
        int n = t.length();
        suffixArray.resize(n);
        for (int i = 0; i < n; ++i) suffixArray[i] = i;

        std::sort(suffixArray.begin(), suffixArray.end(), [&](int a, int b) {
            return t.substr(a) < t.substr(b);
        });

        std::string result;
        for (int i : suffixArray) {
            result += (i == 0 ? t[n - 1] : t[i - 1]);
        }
        return result;
    }

    void buildOcc() {
        int n = bwt.length();
        occ.assign(n + 1, std::vector<int>(256, 0));
        for (int i = 0; i < n; ++i) {
            occ[i + 1] = occ[i];
            occ[i + 1][(unsigned char)bwt[i]]++;
        }

        C.assign(256, 0);
        for (int i = 1; i < 256; ++i) {
            C[i] = C[i - 1] + occ[n][i - 1];
        }
    }

public:
    FMIndex(const std::string& s) : text(s) {
        bwt = computeBWT(s);
        buildOcc();
    }

    std::vector<int> search(const std::string& pattern) {
        int left = 0, right = bwt.length() - 1;
        for (int i = pattern.length() - 1; i >= 0 && left <= right; --i) {
            unsigned char c = pattern[i];
            left = C[c] + (left > 0 ? occ[left][c] : 0);
            right = C[c] + occ[right + 1][c] - 1;
        }

        std::vector<int> result;
        if (left <= right) {
            for (int i = left; i <= right; ++i)
                result.push_back(suffixArray[i]);
            std::sort(result.begin(), result.end()); // optional
        }
        return result;
    }

    void print() {
        std::cout << "Suffix Array:\n";
        for (int i = 0; i < suffixArray.size(); ++i)
            std::cout << std::setw(2) << suffixArray[i] << ": " << text.substr(suffixArray[i]) << "\n";

        std::cout << "\nBWT: " << bwt << "\n";
    }
};

int main() {
    std::string text = "banana";
    FMIndex fm(text);
    fm.print();

    std::string pattern = "ana";
    std::vector<int> positions = fm.search(pattern);

    std::cout << "\nPattern '" << pattern << "' found at positions: ";
    if (positions.empty()) std::cout << "Not found";
    else for (int p : positions) std::cout << p << " ";
    std::cout << "\n";

    return 0;
}
