#include <bits/stdc++.h>
using namespace std;

class B{
private:
	int b;
public:
	B(){
		b = 10;
	}

	void getB(){
		cout<<b;
	}
	
	friend class A;
};

class A{
public:
	void print(B arg){
		cout<<arg.b;
	}
};
int main(){
	B b;
	A a;
	b.getB();
	b.b = 20;
return 0;
}