#include <iostream>
using namespace std;

int main() {
		const int MAXN = 100010;
    long long arr[MAXN];
    int n = 0;
    
    while (cin >> arr[n]) {
        n++;
        if (n >= MAXN) break;
    }

		long long max_ending_here = -2000000000000LL;
    long long max_so_far     = -2000000000000LL;

    for (int i = 0; i < n; i++) {
				max_ending_here = (max_ending_here + arr[i] > arr[i]) ? max_ending_here + arr[i] : arr[i];

        if (max_ending_here > max_so_far) {
            max_so_far = max_ending_here;
        }
    }

    cout << max_so_far << '\n';

    return 0;
}