#include "iostream"

using namespace std;

void set(int &n, int bit) {
	int mask = 1 << bit;
	n |= mask;
}

int main() {
	int n, bit;
	cin >> n >> bit;
	set(n, bit);
	cout << n;

	return 0;
}