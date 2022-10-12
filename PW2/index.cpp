#include <iostream>
#include <queue>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <windows.h>
#include <cmath>
#include <stdio.h>
#include <ios>
#include <time.h>
#include <chrono>

using namespace std;

int randNum(int start = 0, int end = 9) {
	if (start == end) return start;
	return (rand() % abs(end - start + 1) + start);
}

char* toCharArray(string str) {
	char* charArray = new char[str.length() + 1];
	strcpy(charArray, str.c_str());
	return charArray;
}

vector<string> split(string str, char separator = ' ') {
	vector<string> arr;
	string tempStr = "";
	for (int i = 0; i < str.length() - 1; i++) {
		if (str[i] == separator) {
			if (tempStr != "" && str[i + 1] != separator) {
				arr.push_back(tempStr);
				tempStr = "";
			}
		} else tempStr.push_back(str[i]);
	}
	if (str[str.size() - 1] != separator)
		tempStr.push_back(str[str.size() - 1]);
	if (tempStr != "")
		arr.push_back(tempStr);
	return arr;
}

int getNextPrime(int n) {
	while (1) {
		int c = 1;
		n++;
		for (int i = 2; i <= n; i++)
			if (n % i == 0)
				c++;
		if (c == 2)
			return n;
	}
}

struct Node {
	string englishWord = "";
	int index = -1;
	bool isFree = true;
	bool isDeleted = false;

public:
	Node() {}

	Node(string englishWord, int index) {
		this->englishWord = englishWord;
		this->index = index;
	}

	void output() {
		if (isFree) {
			cout << "(free)";
			return;
		}
		if (isDeleted) {
			cout << "(deleted) ";
		}

		cout << "{englishWord: " + englishWord + ", " + "index: " + to_string(index) + "}";
	}
};

class HashTable {
	int step = 1;
	int size = 11;
	int amountItemsExisting = 0;
	int amountItemsDeleted = 0;

public:
	vector<Node> table;

	HashTable() {
		table.resize(this->size);
	}

	boolean push(Node node) {
		if (node.englishWord == "") return 0;

		int key = getHash(node);

		// обработка коллизии
		while (!table[key].isFree) {
			key += step;
			if (key >= size) {
				key %= size;
			}
		}

		node.isFree = false;
		node.isDeleted = false;
		table[key] = node;
		amountItemsExisting++;

		if (isOverflow()) {
			rehash();
			return 1;
		}

		return 0;
	}

	void remove(string englishWord) {
		find(englishWord)->isDeleted = true;

		amountItemsExisting--;
		amountItemsDeleted++;
	}
	
	Node* find(string englishWord) {
		int key = getHash(englishWord);
		Node* selNode;
		for (int i = key; i < table.size(); i++) {
			selNode = &table[i];
			if (selNode->isDeleted) continue;
			if (selNode->englishWord == englishWord) return selNode;
		}

		return new Node("", -1);
	}

	void rehash() {
		if (isOverflow()) size = getNextPrime(size);
		amountItemsDeleted = 0;
		amountItemsExisting = 0;

		vector<Node> temp(table);
		table.clear();
		table.resize(size);

		Node node;
		while (!(temp.empty())) {
			node = temp.back();
			temp.pop_back();

			if (node.isFree) continue;
			if (node.isDeleted) continue;
			push(node);
		}
	}

	void output() {
		for (auto i = 0; i < this->table.size(); i++) {
			cout << i << '\t';
			table[i].output();
			cout << endl;
		}
	}

private:
	bool isOverflow() {
		return ((double)amountItemsExisting + amountItemsDeleted) / size > 0.75 ? true : false;
	}
	
	int getHash(Node node) {
		return getHash(node.englishWord);
	}
	
	int getHash(string englishWord) {
		int res = 0;

		for (int i = 0; i < englishWord.length(); i++) {
			res += (int) englishWord[i];
		}

		return res % size;
	}

public:
	int getAmountItems() {
		return amountItemsExisting;
	}
};

// sizeof = 64
struct FileNodeChar {
	char englishWord[32];
	char russianWord[32];

	FileNodeChar() {
		fill();
	}

	FileNodeChar(string englishWord, string russianWord) {
		fill();
		for (int i = 0; i < englishWord.size(); i++) {
			this->englishWord[i] = englishWord[i];
		}
		for (int i = 0; i < russianWord.size(); i++) {
			this->russianWord[i] = russianWord[i];
		}
	}

	void fill() {
		for (int i = 0; i < 32; i++) {
			this->englishWord[i] = '\0';
			this->russianWord[i] = '\0';
		}
	}

	void output() {
		cout << "{englishWord: ";
		for (int i = 0; i < 32; i++) {
			cout << englishWord[i];
		}
		cout << ", russianWord: ";
		for (int i = 0; i < 32; i++) {
			cout << russianWord[i];
		}
		cout << "}";
	}
};

// sizeof = 64
struct FileNode {
	string englishWord = "";
	string russianWord = "";

public:
	FileNode() {}
	FileNode(string englishWord, string russianWord) {
		this->englishWord = englishWord;
		this->russianWord = russianWord;
	}

	void output() {
		cout << "{englishWord: " + englishWord + ", " + "russianWord: " + russianWord + "}";
	};
};

class File {
	string path = "";
	FILE* file;
	HashTable* table = new HashTable();
	int position = 0;

public:
	File(string path) {
		file = fopen(toCharArray(path), "wb+");
		this->path = path;
	}

	void push(FileNode fileNode) {
		fseek(file, 0, 2);
		FileNodeChar temp = FileNodeChar(fileNode.englishWord, fileNode.russianWord);
		fwrite((char*)&temp, 64, 1, file);
		table->push(Node(fileNode.englishWord, position++));
	}

	FileNode find(string englishWord) {
		int index = table->find(englishWord)->index;
		fseek(file, index * 64, SEEK_SET);

		FileNodeChar temp;
		fread((char*)&temp, 64, 1, file);
		FileNode res = FileNode(temp.englishWord, temp.russianWord);

		return res;
	}

	void remove(string englishWord) {
		int index = table->find(englishWord)->index;
		table->remove(englishWord);
		position--;

		FILE* tempFile = fopen(toCharArray("temp.bin"), "wb+");

		FileNodeChar temp;
		fseek(file, 0, 0);
		for (int i = 0; i < index; i++) {
			fread((char*)&temp, 64, 1, file);
			fwrite((char*)&temp, 64, 1, tempFile);
		}
		fseek(file, (index + 1) * 64, 0);
		while(fread((char*)&temp, 64, 1, file)) {
			fwrite((char*)&temp, 64, 1, tempFile);
			table->find(temp.englishWord)->index--;
		}

		fclose(file);
		file = fopen(toCharArray(path), "wb+");
		fseek(tempFile, 0, 0);
		while (fread((char*)&temp, 64, 1, tempFile)) {
			fwrite((char*)&temp, 64, 1, file);
		}
		fclose(tempFile);
	}

	void outputTable() {
		cout << "\nTable:\n";
		table->output();
	}

	void output() {
		cout << "\nFile:\n";
		FileNodeChar temp;
		fseek(file, 0, 0);
		while (fread((char*)&temp, 64, 1, file)) {
			temp.output();
			cout << endl;
		}
	}
};

void searchTest(File file, string englishWord) {
	auto start = chrono::duration_cast<std::chrono::nanoseconds>(
		chrono::system_clock::now()
			.time_since_epoch()
	).count();
	FileNode res;
	res = file.find(englishWord);
	auto end = chrono::duration_cast<std::chrono::nanoseconds>(
		chrono::system_clock::now()
			.time_since_epoch()
	).count();
	cout << end - start << " ";
	res.output();
	cout << endl;
}

int main() {
	srand(time(0));

	File file("test.bin");
	file.push(FileNode("ourCoolTest1", "test"));

	int amount;
	char letter;
	for (int i = 0; i < 10000; i++) {
		string englishWord = "";
		amount = randNum(3, 10);
		for (int i = 0; i < amount; i++) {
			englishWord += (char) randNum(97, 122);
		}
		file.push(FileNode(englishWord, "test"));
		if (i == 5000) {
			file.push(FileNode("ourCoolTest2", "test"));
		}
	}
	file.push(FileNode("ourCoolTest3", "test"));

	searchTest(file, "ourCoolTest1");
	searchTest(file, "ourCoolTest2");
	searchTest(file, "ourCoolTest3");

	// cout << ('k' + 'e' + 'y' + '2') % 11;

	file.push(FileNode("ab1", "znachenie1"));
	file.push(FileNode("ba1", "znachenie2"));
	file.push(FileNode("key2", "cluch2"));

	file.remove("ba1");
	file.find("key2").output();

	file.output();
	file.outputTable();

	// file.push(FileNode("key3", "cluch3"));
	// file.push(FileNode("key4", "cluch4"));
	// file.push(FileNode("key5", "cluch5"));
	// file.push(FileNode("key6", "cluch6"));
	// file.push(FileNode("key7", "cluch7"));
	// file.push(FileNode("key8", "cluch8"));
	// file.push(FileNode("key9", "cluch9"));
	// file.push(FileNode("key10", "cluch10"));
	// file.push(FileNode("key11", "cluch11"));

	// file.output();
	// file.outputTable();

	return 0;
}