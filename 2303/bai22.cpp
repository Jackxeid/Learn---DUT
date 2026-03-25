#include <iostream>
#include <unordered_set>
using namespace std;

int main() {
    const int MAXN = 100010;
    long long arr[MAXN];
    int n = 0;
    
    while (cin >> arr[n]) {
        n++;
        if (n >= MAXN) break;
    }

    long long result[MAXN];
    int count = 0;

    unordered_set<long long> seen;

    for (int i = 0; i < n; i++) {
        long long val = arr[i];
        if (seen.insert(val).second) {
            result[count++] = val;
        }
    }
		cout << count << "\n";

    for (int i = 0; i < count; i++) {
        if (i > 0) cout << " ";
        cout << result[i];
    }
    cout << "\n";

    return 0;
}