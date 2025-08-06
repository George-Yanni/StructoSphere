#include <iostream>
#include <vector>
using namespace std;

class DisjointSet {
    vector<int> parent, rank;
public:
    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int u) {
        if (u != parent[u])
            parent[u] = find(parent[u]); // Path compression
        return parent[u];
    }

    void unite(int u, int v) {
        int pu = find(u), pv = find(v);
        if (pu == pv) return;
        if (rank[pu] < rank[pv]) parent[pu] = pv;
        else if (rank[pu] > rank[pv]) parent[pv] = pu;
        else {
            parent[pv] = pu;
            rank[pu]++;
        }
    }

    void printSets(int n) {
        for (int i = 0; i < n; ++i)
            cout << "Element " << i << " -> Set " << find(i) << '\n';
    }
};

int main() {
    DisjointSet ds(10);
    ds.unite(1, 2);
    ds.unite(3, 4);
    ds.unite(2, 4);
    ds.unite(5, 6);
    ds.unite(7, 8);
    ds.unite(5, 7);
    ds.unite(1, 5);
    ds.printSets(10);
}
