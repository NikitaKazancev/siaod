#include "iostream"

using namespace std;

void set(int &n) {
	n <<= 4;
}

int main() {
	int n;
	cin >> n;
	set(n);
	cout << n;

	return 0;
}