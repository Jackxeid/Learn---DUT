#include <iostream>
#include <math.h>
using namespace std;

void test(int *p) {
	*p = 20;
};

int main(){
		int A[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
		int *ptrA = A;
		for(int i = 0; i < 20; i++){
			*(ptrA + i) = pow(i, 2);
		}
		for (int i = 0; i < 20; i++){
			cout << "A[" << i << "] = " << ptrA[i] << endl;
			cout << "A[" << i << "] = " << A[i] << endl;
		}
	
	cout << "==============================" << endl;
	int a = 10;
	int N = 99;
	int *ptr = &a; // Ban đầu ptr trỏ vào a
	cout << "Địa chỉ của a: " << &a << ", giá trị của a: " << a << endl;
	cout << "Địa chỉ của N: " << &N << ", giá trị của N: " << N << endl;
	cout << "Địa chỉ mà ptr đang trỏ tới ptr: " << ptr << endl;
	cout << "Giá trị tại địa chỉ đó *ptr: " << *ptr << endl;
	cout << "==============================" << endl;

	// TRƯỜNG HỢP A: ptr = &N;
	cout << "TRƯỜNG HỢP A: ptr = &N:" << endl;
	ptr = &N;
	cout << "Giá trị của a sau khi ptr = &N: " << a << endl;
	cout << "Địa chỉ mà ptr đang trỏ tới ptr: " << ptr << endl;
	cout << "Giá trị tại địa chỉ đó *ptr: " << *ptr << endl;
	cout << "==============================" << endl;
	// Kết quả: ptr không còn trỏ vào a nữa, nó trỏ vào N. 
	// Giá trị của a vẫn là 10.

	// TRƯỜNG HỢP B: *ptr = N;
	cout << "TRƯỜNG HỢP B: *ptr = N:" << endl;
	*ptr = N; 
	cout << "Giá trị của a sau khi *ptr = N: " << a << endl;
	cout << "Địa chỉ mà ptr đang trỏ tới ptr: " << ptr << endl;
	cout << "Giá trị tại địa chỉ đó *ptr: " << *ptr << endl;
	cout << "==============================" << endl;
	// Kết quả: ptr vẫn trỏ vào a, nhưng giá trị của a đã bị đổi thành 99.
	// Biến N vẫn là 99 (không đổi).

	int *p1 = new int(10);
	int *p2 = new int[10];
	cout << *p1 << endl;
	cout << *p2 << endl;

	test(p1);
	cout << *p1 << endl;

	int a1 = 10;
	void *p = &a1;
	printf("%d", *(int*)p);
}
