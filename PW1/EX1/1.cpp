#include "iostream"

using namespace std;

void set(int &n) {
	n |= 0b1111;
}

int main() {
	int n;
	cin >> n;
	set(n);
	cout << n;

	return 0;
}