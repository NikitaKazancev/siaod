#include "iostream"
#include "string"
#include <bitset>

using namespace std;

void set(int &n, int mask) {
	n &= mask;
}

string getOutputMask(int n, int mask) {
	string res = "";

	while (mask) {
		res = to_string(mask % 2) + res;
		n /= 2;
		mask /= 2;
	}
	while (n) {
		res = "0" + res;
		n /= 2;
	}

	return res;
}

string toBinary(int n) {
	string res = "";

	while (n) {
		res = to_string(n % 2) + res;
		n /= 2;
	}

	return res;
}

int getMask(int n) {
	int bits = 0;
	while (n) {
		n /= 2;
		bits++;
	}
	bits--;

	int mask = 1;
	for (int i = 0; i < bits; i++) {
		if (bits - i == 3 || bits - i == 5 || bits - i == 11) {
			mask--;
		}

		mask = (mask << 1) + 1;
	}

	return mask;
}

int main() {
	int n;
	cin >> n;
	int mask = getMask(n);
	cout << "Number in binary system: " + toBinary(n) << endl;
	cout << "Mask: " + getOutputMask(n, mask) << endl;
	set(n, mask);
	cout << "Result: " + to_string(n);

	return 0;
}