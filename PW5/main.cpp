#include "iostream"
#include "vector"
#include "string"

using namespace std;

template <class T>
bool contains(vector<T> arr, T elem) {
	for (int i = 0; i < arr.size(); i++) {
		if (elem == arr[i]) return true;
	}
	return false;
}

bool contains(string arr, char elem) {
	for (int i = 0; i < arr.size(); i++) {
		if (elem == arr[i]) return true;
	}
	return false;
}

template <class T>
void showMatrix(vector<vector<T>> matrix, string separator = ", ") {
	cout << endl;
	for (int i = 0; i < matrix.size(); i++) {
		cout << "{ ";
		for (int j = 0; j < matrix[i].size() - 1; j++)
			cout << matrix[i][j] << separator;
		cout << matrix[i][matrix[i].size() - 1] << " }\n";
	}
	cout << endl;
}

template <class T>
void showArr(vector<T> arr, string separator = " ") {
	if (arr.size() == 0) {
		cout << "Nothing here" << endl;
		return;
	}
	for (int i = 0; i < arr.size() - 1; i++) cout << arr[i] << separator;
	cout << arr[arr.size() - 1];
	cout << endl;
}

struct Graph {
	vector<vector<int>> graph;
	vector<int> ids;

	Graph() {}

	void add(vector<int> elem, string connectionType) {
		if (!contains(ids, elem[0])) {
			ids.push_back(elem[0]);
		}
		if (!contains(ids, elem[1])) {
			ids.push_back(elem[1]);
		}

		graph.push_back(elem);

		if (connectionType == "reversible") {
			vector<int> elem2;
			elem2.push_back(elem[1]);
			elem2.push_back(elem[0]);
			elem2.push_back(elem[2]);

			graph.push_back(elem2);
		}
	}

	void inputElems(bool oneElem) {
		int a, b, weight, type;
		vector<int> res;

		while (true) {
			cout << "Parent: "; cin >> a;
			if (oneElem || a == 0) {
				break;
			}

			cout << "Child: "; cin >> b;
			cout << "Weight: "; cin >> weight;
			cout << "Type (0 - edge, 1 - arc): "; cin >> type;

			if (type == 1) {
				add({a, b, weight}, "");
			}

			else {
				add({a, b, weight}, "reversible");
			}
			
			cout << "Element ";
			showElem({a, b, weight});
			cout << " (type - " << (type == 1 ? "arc" : "edge") << ") added\n\n";
		}
	}

	vector<string> getLength(int id, int elemToSearch = 0) {
		vector<vector<string>> children;

		vector<int> elem;
		for (int i = 0; i < graph.size(); i++) {
			elem = graph[i];

			if (id == elem[0]) {
				vector<string> childrenElem;
				childrenElem.push_back(to_string(elem[1]));
				childrenElem.push_back(to_string(elem[2]));
				childrenElem.push_back(to_string(elem[0]));

				children.push_back(childrenElem);
			}
		}

		if (!elemToSearch) {
			vector<vector<string>> minLengths;

			for (int i = 0; i < ids.size(); i++) {
				if (ids[i] != id) {
					minLengths.push_back(getLength(children, id, ids[i]));
				}
			}

			int maxLength = 0;
			vector<string> res;
			for (int i = 0; i < minLengths.size(); i++) {
				if (stoi(minLengths[i][0]) > maxLength) {
					maxLength = stoi(minLengths[i][0]);
					res = minLengths[i];
				}
			}

			return res;
		}

		return getLength(children, id, elemToSearch);
	}

private:
	vector<string> getLength(vector<vector<string>> elems, int id, int elemToSearch) {
		vector<vector<string>> newElems;
		bool trigger = false;
		bool localTrigger = true;

		vector<string> masElem;
		vector<int> graphElem;
		for (int i = 0; i < elems.size(); i++) {
			masElem = elems[i];

			for (int j = 0; j < graph.size(); j++) {
				graphElem = graph[j];

				if (graphElem[0] == elemToSearch) {
					continue;
				}

				if (
					masElem[0] == to_string(graphElem[0]) && 
					!contains(masElem[2], graphElem[1] + 48)
				) {
					vector<string> childrenElem;
					childrenElem.push_back(to_string(graphElem[1]));
					childrenElem.push_back(to_string(graphElem[2] + stoi(masElem[1])));
					childrenElem.push_back(masElem[2] + masElem[0]);

					newElems.push_back(childrenElem);
					trigger = true;
					localTrigger = false;
				}
			}

			if (localTrigger) {
				newElems.push_back(masElem);
			}

			localTrigger = true;
		}

		if (trigger) return getLength(newElems, id, elemToSearch);
		
		int resLength = 1000000;
		string lastElem = to_string(elemToSearch);
		string usedElems;

		for (int i = 0; i < newElems.size(); i++) {
			if (
				newElems[i][0] == to_string(elemToSearch) && 
				stoi(newElems[i][1]) < resLength
			) {
				resLength = stoi(newElems[i][1]);
				usedElems = newElems[i][2];
			}
		}

		string way = to_string(id);
		for (int i = 1; i < usedElems.length(); i++) {
			way += (" -> " + to_string(usedElems[i] - 48));
		}
		way += (" -> " + lastElem);

		vector<string> res;
		res.push_back(to_string(resLength));
		res.push_back(lastElem);
		res.push_back(way);
		
		return res;
	}

public:
	void showElem(vector<int> elem) {
		cout 
			<< "{ from: " << elem[0] 
			<< ", to: " << elem[1]
			<< ", weight: " << elem[2] << " }";
	}

	void show() {
		cout << endl;
		for (int i = 0; i < graph.size(); i++) {
			showElem(graph[i]);
			cout << endl;
		}
		cout << endl;
	}
};

void showCommands() {
	cout 
		<< "1 - Show graph\n"
		<< "2 - Enter graph (0 is the end)\n"
		<< "3 - Add element\n"
		<< "4 - Find minimum length between 2 elements\n"
		<< "5 - Find eccentricity of element\n"
		<< "0 - Show commands\n";
}

int main() {
	Graph graph;

	int a, b;
	vector<string> res;
	string command;

	graph.add({1, 2, 23}, "reversible");
	graph.add({1, 3, 12}, "reversible");
	graph.add({3, 2, 25}, "reversible");
	graph.add({4, 3, 18}, "reversible");
	graph.add({4, 6, 20}, "reversible");
	graph.add({6, 5, 23}, "reversible");
	graph.add({2, 5, 22}, "reversible");
	graph.add({2, 8, 35}, "reversible");
	graph.add({7, 8, 16}, "reversible");
	graph.add({7, 5, 14}, "reversible");
	graph.add({7, 6, 24}, "reversible");

	showCommands();
	while (true) {
		cin >> command;

		if (command == "1") {
			graph.show();
		}

		else if (command == "2") {
			graph.inputElems(false);
		}

		else if (command == "3") {
			graph.inputElems(true);
		}

		else if (command == "4") {
			cout << "First element: "; cin >> a;
			cout << "Second: "; cin >> b;

			res = graph.getLength(a, b);
			cout << "Minimum length: " << res[0] << endl;
			cout << "Way: " << res[2] << endl;
		}

		else if (command == "5") {
			cout << "Element: "; cin >> a;

			res = graph.getLength(a);
			cout << "Eccentricity: " << res[0] << endl;
			cout << "Way: " << res[2] << endl;
		}

		else {
			showCommands();
		}
	}

	return 0;
}

	// graph.add({1, 2, 3}, "");
	// graph.add({2, 3, 4}, "");
	// graph.add({1, 4, 5}, "");
	// graph.add({2, 5, 6}, "");
	// graph.add({4, 6, 7}, "");
	// graph.add({2, 1, 8}, "");
	// graph.add({6, 9, 8}, "");
	// graph.add({3, 5, 1}, "");

	// graph.getLength(2);