#include <iostream>
using namespace std;

void findSecondLargest(int arr[], int n) {
	if (n < 2) {
		cout << "None" << endl;
		return;
	}
	
	// Giá trị sentinel nhỏ hơn giới hạn dưới của đề (-10^6)
	int first = -2000001LL;
	int second = -2000001LL;
	
	for (int i = 0; i < n; i++) {
		int num = arr[i];

		if (num > first) {
			second = first;
			first = num;
		}
		else if (num > second && num != first) {  // thêm != first để xử lý tốt hơn khi có trùng lặp
			second = num;
		}
	}
	
	// Nếu second vẫn là giá trị khởi tạo → không có phần tử lớn thứ hai thực sự
	if (second == -2000001LL) {
		cout << "None" << endl;
	} else {
		cout << second << endl;
	}
}

int main() {
	const int MAX_N = 100000;
	int arr[MAX_N];
	int n = 0;
	
	while (cin >> arr[n]) {
		n++;
	}
	
	findSecondLargest(arr, n);
	return 0;
}