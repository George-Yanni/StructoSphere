#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>
#include <sstream>
#include <cctype>
#include <iomanip>

using namespace std;

// Structure for storing sparse matrix in CSR format
struct SparseMatrix {
    vector<float> values;       // Non-zero values
    vector<int> colIndices;    // Column indices of non-zero values
    vector<int> rowPointers;   // Row pointers
};

// Function to preprocess the text and build a vocabulary
void preprocessDocuments(const vector<string>& documents, vector<unordered_map<string, int>>& termFreqs, unordered_map<string, int>& vocab) {
    for (int docId = 0; docId < documents.size(); ++docId) {
        stringstream ss(documents[docId]);
        string word;
        while (ss >> word) {
            // Remove punctuation and convert to lowercase
            string cleanedWord;
            for (char ch : word) {
                if (isalnum(ch)) {
                    cleanedWord += tolower(ch);
                }
            }
            if (cleanedWord.empty()) continue;

            // Count term frequency
            termFreqs[docId][cleanedWord]++;
            
            // Add to vocabulary
            if (vocab.find(cleanedWord) == vocab.end()) {
                vocab[cleanedWord] = vocab.size();
            }
        }
    }
}

// Function to compute the term frequency (TF)
vector<vector<float>> computeTF(const vector<unordered_map<string, int>>& termFreqs, const unordered_map<string, int>& vocab) {
    int numDocs = termFreqs.size();
    int numTerms = vocab.size();
    vector<vector<float>> tf(numDocs, vector<float>(numTerms, 0.0));

    for (int docId = 0; docId < numDocs; ++docId) {
        int totalTerms = 0;
        for (const auto& term : termFreqs[docId]) {
            totalTerms += term.second;
        }
        if (totalTerms == 0) continue;
        for (const auto& term : termFreqs[docId]) {
            int termIndex = vocab.at(term.first);
            tf[docId][termIndex] = static_cast<float>(term.second) / totalTerms;
        }
    }
    return tf;
}

// Function to compute the inverse document frequency (IDF)
vector<float> computeIDF(const vector<unordered_map<string, int>>& termFreqs, const unordered_map<string, int>& vocab) {
    int numDocs = termFreqs.size();
    vector<float> idf(vocab.size(), 0.0);

    for (const auto& termEntry : vocab) {
        const string& term = termEntry.first;
        int docCount = 0;
        for (int docId = 0; docId < numDocs; ++docId) {
            if (termFreqs[docId].find(term) != termFreqs[docId].end()) {
                docCount++;
            }
        }
        idf[termEntry.second] = log(static_cast<float>(numDocs) / (1 + docCount));
    }
    return idf;
}

// Function to compute TF-IDF
vector<vector<float>> computeTFIDF(const vector<vector<float>>& tf, const vector<float>& idf) {
    int numDocs = tf.size();
    int numTerms = idf.size();
    vector<vector<float>> tfidf(numDocs, vector<float>(numTerms, 0.0));

    for (int docId = 0; docId < numDocs; ++docId) {
        for (int termIndex = 0; termIndex < numTerms; ++termIndex) {
            tfidf[docId][termIndex] = tf[docId][termIndex] * idf[termIndex];
        }
    }
    return tfidf;
}

// Function to convert the TF-IDF matrix to a sparse CSR format
SparseMatrix convertToCSR(const vector<vector<float>>& tfidf) {
    if (tfidf.empty() || tfidf[0].empty()) {
        return SparseMatrix();
    }

    int numDocs = tfidf.size();
    int numTerms = tfidf[0].size();

    SparseMatrix csr;
    csr.rowPointers.push_back(0);

    for (int i = 0; i < numDocs; ++i) {
        for (int j = 0; j < numTerms; ++j) {
            if (tfidf[i][j] != 0.0f) {
                csr.values.push_back(tfidf[i][j]);
                csr.colIndices.push_back(j);
            }
        }
        csr.rowPointers.push_back(csr.values.size());
    }

    return csr;
}

// Function to get term by index from vocabulary
string getTermByIndex(const unordered_map<string, int>& vocab, int index) {
    for (const auto& pair : vocab) {
        if (pair.second == index) {
            return pair.first;
        }
    }
    return "UNKNOWN";
}

// Function to display the vocabulary
void displayVocabulary(const unordered_map<string, int>& vocab) {
    cout << "\nVocabulary (Term -> Index):\n";
    for (const auto& pair : vocab) {
        cout << pair.first << " -> " << pair.second << endl;
    }
}

// Function to display the TF-IDF matrix
void displayTFIDF(const vector<vector<float>>& tfidf, const unordered_map<string, int>& vocab) {
    cout << "\nTF-IDF Matrix (Rows: Documents, Columns: Terms):\n";
    cout << setw(10) << "Doc\\Term";
    for (int j = 0; j < vocab.size(); ++j) {
        cout << setw(12) << getTermByIndex(vocab, j);
    }
    cout << endl;

    for (int i = 0; i < tfidf.size(); ++i) {
        cout << setw(10) << "Doc " + to_string(i);
        for (int j = 0; j < tfidf[i].size(); ++j) {
            cout << setw(12) << fixed << setprecision(6) << tfidf[i][j];
        }
        cout << endl;
    }
}

// Function to display the CSR representation
void displayCSR(const SparseMatrix& csr, int numDocs, const unordered_map<string, int>& vocab) {
    cout << "\nCSR Representation:\n";
    cout << "Values: ";
    for (const auto& value : csr.values) {
        cout << fixed << setprecision(6) << value << " ";
    }
    cout << endl;

    cout << "Column Indices: ";
    for (const auto& index : csr.colIndices) {
        cout << index << " ";
    }
    cout << endl;

    cout << "Row Pointers: ";
    for (const auto& pointer : csr.rowPointers) {
        cout << pointer << " ";
    }
    cout << endl;

    // Detailed CSR breakdown
    cout << "\nDetailed CSR Breakdown by Document:\n";
    for (int docId = 0; docId < numDocs; ++docId) {
        cout << "Document " << docId << ":\n";
        int start = csr.rowPointers[docId];
        int end = csr.rowPointers[docId + 1];
        if (start == end) {
            cout << "  No non-zero terms.\n";
            continue;
        }
        for (int i = start; i < end; ++i) {
            int colIndex = csr.colIndices[i];
            float value = csr.values[i];
            string term = getTermByIndex(vocab, colIndex);
            cout << "  Term: " << term << " (Index: " << colIndex << "), TF-IDF: " << fixed << setprecision(6) << value << endl;
        }
    }
}

int main() {
    // Sample documents
    vector<string> documents = {
        "I love machine learning and AI",
        "Machine learning is amazing",
        "I love programming in Python",
        "Python is a powerful language for AI"
    };

    if (documents.empty()) {
        cout << "Error: No documents provided." << endl;
        return 1;
    }

    unordered_map<string, int> vocab;
    vector<unordered_map<string, int>> termFreqs(documents.size());

    // Preprocess documents and build vocabulary
    preprocessDocuments(documents, termFreqs, vocab);

    if (vocab.empty()) {
        cout << "Error: Vocabulary is empty." << endl;
        return 1;
    }

    // Display vocabulary
    displayVocabulary(vocab);

    // Compute term frequency (TF)
    vector<vector<float>> tf = computeTF(termFreqs, vocab);

    // Compute inverse document frequency (IDF)
    vector<float> idf = computeIDF(termFreqs, vocab);

    // Compute TF-IDF
    vector<vector<float>> tfidf = computeTFIDF(tf, idf);

    // Display TF-IDF matrix
    displayTFIDF(tfidf, vocab);

    // Convert TF-IDF to CSR format
    SparseMatrix csr = convertToCSR(tfidf);

    // Display the CSR representation
    displayCSR(csr, documents.size(), vocab);

    return 0;
}