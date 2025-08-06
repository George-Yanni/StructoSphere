#include <iostream>
#include <vector>
using namespace std;

class FenwickTree {
    vector<int> bit;
    int n;

public:
    FenwickTree(int size) : n(size) {
        bit.assign(n + 1, 0);
    }

    void update(int index, int val) {
        for (++index; index <= n; index += index & -index)
            bit[index] += val;
    }

    int query(int index) {
        int sum = 0;
        for (++index; index > 0; index -= index & -index)
            sum += bit[index];
        return sum;
    }

    int rangeQuery(int l, int r) {
        return query(r) - query(l - 1);
    }
};

int main() {
    FenwickTree ft(10);
    ft.update(2, 5);
    ft.update(4, 3);
    ft.update(6, 7);

    cout << "Sum[0..6]: " << ft.query(6) << endl;
    cout << "Sum[2..6]: " << ft.rangeQuery(2, 6) << endl;
}
