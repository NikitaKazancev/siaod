#include "iostream"
#include "vector"
#include "cmath"

using namespace std;

int doOperation(int a, int b, char op) {
	switch (op){
		case '+':
			return a + b;
		case '-':
			return a - b;
		case '*':
			return a * b;
		case '/':
			return a / b;
		default:
			return 0;
	}
}

vector<vector<char>> matrixWithoutSpace(vector<vector<char>> matrix) {
	vector<vector<char>> res(matrix.size());
	int leftIndex = 0, rightIndex = 0;
	bool space = true;

	for (int i = 0; i < matrix[0].size(); i++) {
		for (int j = 0; j < matrix.size(); j++) {
			// cout << matrix[j][i];
			if (matrix[j][i] != ' ') {
				space = false;
				break;
			}
		}
		if (!space) {
			leftIndex = i;
			break;
		}
	}
	for (int i = matrix[0].size() - 1; i >= 0; i--) {
		for (int j = 0; j < matrix.size(); j++) {
			if (matrix[j][i] != ' ') {
				space = false;
				break;
			}
		}
		if (!space) {
			rightIndex = i;
			break;
		}
	}

	for (int i = 0; i < matrix.size(); i++) {
		for (int j = leftIndex; j <= rightIndex; j++) {
			res[i].push_back(matrix[i][j]);
		}
	}

	return res;
}

template <class T>
void showMatrix(vector<vector<T>> matrix) {
	matrix = matrixWithoutSpace(matrix);
	cout << "\n------------\n";
	for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix[i].size(); j++)
			cout << matrix[i][j];
		cout << endl;
	}
	cout << "\n------------\n";
}

void showMatrixReverse(vector<vector<char>> matrix, char separator = ' ') {
	matrix = matrixWithoutSpace(matrix);
	cout << "\n------------\n";
	for (int i = matrix[0].size() - 1; i >= 0; i--) {
		for (int j = 0; j < matrix.size(); j++) {
			if (matrix[j][i] == separator) {
				cout << "\t";
			} else {
				cout << matrix[j][i];
			}
		}
		cout << endl;
	}
	cout << "\n------------\n";
}

bool contains(string str, char c) {
	for (int i = 0; i < str.length(); i++) {
		if (c == str[i]) return true;
	}
	return false;
}

struct Expression {
	Expression* parent = nullptr;
	Expression* firstChild = nullptr;
	Expression* secondChild = nullptr;
	char sign = ' ';
	int number = 0;

	Expression() {}
	Expression(Expression* parent) {
		this->parent = parent;
	}
	Expression(int number, Expression* parent) {
		this->number = number;
		this->parent = parent;
	}
	Expression(Expression* firstChild, Expression* secondChild, char sign, Expression* parent) {
		this->secondChild = secondChild;
		this->firstChild = firstChild;
		this->sign = sign;
		this->parent = parent;
	}

	bool checkExpressionStr(string expressionStr) {
		string numbers = "0123456789";
		string signs = "+-*/";

		char symbol = expressionStr[0], prevSymbol;
		int 
			leftBracketCounter = 0,
			rightBracketCounter = 0,
			signsCounter = 0,
			numsCounter = 0;
		for (int i = 0; i < expressionStr.length(); i++) {
			prevSymbol = symbol;
			symbol = expressionStr[i];

			if (contains(numbers, symbol)) {
				numsCounter++;

				if (prevSymbol == ')' || contains(numbers, prevSymbol)) return false;
			} else 

			if (contains(signs, symbol)) {
				signsCounter++;

				if (prevSymbol == '(' || contains(signs, prevSymbol)) return false;
			} else 

			if (symbol == '(') {
				leftBracketCounter++;

				if (prevSymbol == ')' || contains(numbers, prevSymbol)) return false;
			} else 

			if (symbol == ')') {
				rightBracketCounter++;

				if (prevSymbol == '(' || contains(signs, prevSymbol)) return false;
			}

			else return false;
		}

		if (
			leftBracketCounter != rightBracketCounter ||
			leftBracketCounter != signsCounter ||
			numsCounter - 1 != signsCounter
		) return false;

		return true;
	}

	string addBrackets(string expressionStr) {
		string res = expressionStr;

		string allSigns = "+-*/";
		string biggerSigns = "*/";
		string smallerSigns = "+-";

		char symbol; // текущий символ
		char left, right; // символы слева и справа от него при проходе в цикле
		int leftIndex, rightIndex; // индексы этих символов
		int rightBrAm = 0, leftBrAm = 0; // временные переменные для подсчета левых и правых скобок
		bool br = true; // переменная для указания, что пора break
		bool exist = true; // переменная для указания, что при проходах выяснилось, что выражение неверное
		int leftLBrAm = 0, leftRBrAm = 0, rightRBrAm = 0, rightLBrAm = 0; // временные переменные
		int tempLeftIndex = 0, tempRightIndex = 0; // временные переменные

		// после каждой установки скобок для упрощения строка заново создается и все циклы останавливаются, поиск необходимых других скобок начинается заново
		for (int z = 0; z < 50; z++) {
			if (!br || !exist) break;
			br = false;

			// цикл для проверки наличия знаков слева и справа, удалённых на одно расстояние
			for (int i = 0; i < res.size(); i++) {
				if (res[i] == ')' || res[i] == '(') continue;
				symbol = res[i];
				for (int k = 1; k <= i && k <= res.size() - i - 1; k++) {

					leftIndex = i - k;
					rightIndex = i + k;
					left = res[leftIndex];
					right = res[rightIndex];

					if (left == '(') {
						leftLBrAm++;
					} else if (left == ')') {
						leftRBrAm++;
					}

					if (right == '(') {
						rightLBrAm++;
					} else if (right == ')') {
						rightRBrAm++;
					}

					// проверка того, что случай, когда знаки на одном расстоянии, нам подходит 
					// (проверки всего, что я смог найти, возможно, проверка не полная)
					if ((
							(leftLBrAm - leftRBrAm > 0) && 
							(leftLBrAm - leftRBrAm == rightRBrAm - rightLBrAm) || 
							(leftRBrAm == 0 && rightLBrAm == 0)
						) 
						&& contains(allSigns, left) 
						&& contains(allSigns, right)
					) {
						if (leftIndex - 1 < 0 || rightIndex >= res.size()) {
							exist = false;
							break;
						}

						// выполнение тех случаев, когда скобки нужно поставить для первого знака
						if (
							contains(biggerSigns, left) || 
							(
								contains(smallerSigns, left) && 
								contains(smallerSigns, right)
							) || 
							(
								contains(biggerSigns, left) && 
								contains(biggerSigns, right)
							)
						) {
							// проверяем, рядом со знаком выражения или нет, если да, то узнаём через цикл, куда нужно поставить скобку (1)
							if (res[leftIndex - 1] == ')') {
								for (int x = leftIndex - 1; x >= 0 ; x--) {
									if (res[x] == ')') rightBrAm++;
									if (res[x] == '(') leftBrAm++;
									if (rightBrAm == leftBrAm) {
										tempLeftIndex = x;
										break;
									}
									if (x == 0) {
										exist = false;
									}
								}
								rightBrAm = 0;
								leftBrAm = 0;
							} else {
								tempLeftIndex = leftIndex - 1;
							}

							// (1)
							if (res[leftIndex + 1] == '(') {
								for (int x = leftIndex + 1; x < rightIndex ; x++) {
									if (res[x] == ')') rightBrAm++;
									if (res[x] == '(') leftBrAm++;
									if (rightBrAm == leftBrAm) {
										tempRightIndex = x;
										break;
									}
									if (x == 0) {
										exist = false;
									}
								}
								rightBrAm = 0;
								leftBrAm = 0;
							} else {
								tempRightIndex = leftIndex + 1;
							}

							// создание новой строки
							res = 
								res.substr(0, tempLeftIndex) + '(' + 
								res.substr(tempLeftIndex, tempRightIndex - tempLeftIndex + 1) + ')' + 
								res.substr(tempRightIndex + 1, res.size() - tempRightIndex + 1);
						} else

						// выполнение тех случаев, когда скобки нужно поставить для второго знака
						if (contains(smallerSigns, left) && contains(biggerSigns, right)) {

							// (1)
							if (res[rightIndex - 1] == ')') {
								for (int x = rightIndex - 1; x > leftIndex ; x--) {
									if (res[x] == ')') rightBrAm++;
									if (res[x] == '(') leftBrAm++;
									if (rightBrAm == leftBrAm) {
										tempLeftIndex = x;
										break;
									}
									if (x == 0) {
										exist = false;
									}
								}
								rightBrAm = 0;
								leftBrAm = 0;
							} else {
								tempLeftIndex = rightIndex - 1;
							}

							// (1)
							if (res[rightIndex + 1] == '(') {
								for (int x = rightIndex + 1; x < res.size() ; x++) {
									if (res[x] == ')') rightBrAm++;
									if (res[x] == '(') leftBrAm++;
									if (rightBrAm == leftBrAm) {
										tempRightIndex = x;
										break;
									}
									if (x == 0) {
										exist = false;
									}
								}
								rightBrAm = 0;
								leftBrAm = 0;
							} else {
								tempRightIndex = rightIndex + 1;
							}

							res = 
								res.substr(0, tempLeftIndex) + '(' + 
								res.substr(tempLeftIndex, tempRightIndex - tempLeftIndex + 1) + ')' + 
								res.substr(tempRightIndex + 1, res.size() - tempRightIndex + 1);
						}

						tempLeftIndex = 0;
						tempRightIndex = 0;

						br = true;
						break;
					}

					if (br || !exist) break;
				}

				leftLBrAm = 0;
				leftRBrAm = 0;
				rightLBrAm = 0;
				rightRBrAm = 0;

				if (br || !exist) break;
			}
		}

		if (!exist) {
			return "0";
		}

		// установка первой и последней скобки, если их нет
		int lBracketsCounter = 0, signsCounter = 0;
		for (int i = 0; i < res.size(); i++) {
			if (contains(allSigns, res[i])) {
				signsCounter++;
			} else if (res[i] == '(') {
				lBracketsCounter++;
			}
		}
		if (lBracketsCounter != signsCounter) {
			res = '(' + res + ')';
		}

		return res;
	}

	Expression* createExpr(string expressionStr, bool &good) {
		expressionStr = addBrackets(expressionStr);
		
		if (!checkExpressionStr(expressionStr)) {
			good = false;
			return new Expression();
		} else {
			good = true;
		}

		string numbers = "0123456789";
		string signs = "+-*/";

		Expression* currentNode = new Expression();
		Expression* res = currentNode;
		char symbol = expressionStr[0], prevSymbol;
		for (int i = 1; i < expressionStr.length() - 1; i++) {
			prevSymbol = symbol;
			symbol = expressionStr[i];

			if (symbol == '(') {
				if (prevSymbol == '(') {
					// создание первого ребенка и перемещение к нему
					currentNode->firstChild = new Expression(currentNode);
					currentNode = currentNode->firstChild;
				} else if (contains(signs, prevSymbol)) {
					// создание второго ребенка и перемещение к нему
					currentNode->secondChild = new Expression(currentNode);
					currentNode = currentNode->secondChild;
				}
			} else if (contains(numbers, symbol)) {
				if (prevSymbol == '(') {
					// создание первого ребенка в качестве числа
					currentNode->firstChild = new Expression((int) symbol, currentNode);
				} else if (contains(signs, prevSymbol)) {
					// создание второго ребенка в качестве числа
					currentNode->secondChild = new Expression((int) symbol, currentNode);
				}
			} else if (contains(signs, symbol)) {
				currentNode->sign = symbol;
			} else if (symbol == ')') {
				if (currentNode->parent) {
					currentNode = currentNode->parent;
				}
			}
		}

		return res;
	}

	int getMaxLevel(Expression* expr, int index = 1) {
		int first = 0, second = 0;
		if (expr->firstChild != nullptr) first = getMaxLevel(expr->firstChild, index + 1);
		if (expr->secondChild != nullptr) second = getMaxLevel(expr->secondChild, index + 1);
		if (first != 0 && second != 0) return first > second ? first : second;
		if (first != 0) return first;
		if (second != 0) return second;
		return index;
	}

private:
	void getMatrix(
		vector<vector<char>> &matrix, 
		Expression* expr, 
		int leftIndex, 
		int rightIndex, 
		int bottomIndex = 0
	) {
		int middle = (rightIndex + leftIndex) / 2;
		if (expr->number) {
			matrix[bottomIndex][middle] = expr->number;
		} else {
			matrix[bottomIndex][middle] = expr->sign;
			if (expr->firstChild) {
				getMatrix(matrix, expr->firstChild, leftIndex, middle, bottomIndex + 1);
			}
			if (expr->secondChild) {
				getMatrix(matrix, expr->secondChild, middle, rightIndex, bottomIndex + 1);
			}
		}
	}

public:
	vector<vector<char>> getMatrix(Expression* expr) {
		int maxLevel = expr->getMaxLevel(expr);
		vector<vector<char>> matrix(maxLevel);
		for (int i = 0; i < matrix.size(); i++) {
			for (int j = 0; j < pow(2, maxLevel) - 1; j++) {
				matrix[i].push_back(' ');
			}
		}
		getMatrix(matrix, expr, 0, pow(2, maxLevel) - 1);
		return matrix;
	}

	int count(Expression* expr) {
		int a, b;
		if (expr->firstChild) a = count(expr->firstChild);
		if (expr->secondChild) b = count(expr->secondChild);
		if (expr->firstChild || expr->secondChild) {
			return doOperation(a, b, expr->sign);
		}
		return expr->number - 48;
	}
};

struct Tree {
	string expression = "";
	bool good = false;
	Expression* mainNode = new Expression();
	vector<vector<char>> matrix;
	Tree() {}
	Tree(string expression) {
		changeData(expression);
	}
	void changeData(string expression) {
		this->expression = expression;
		mainNode = mainNode->createExpr(expression, good);
		if (good) {
			matrix = mainNode->getMatrix(mainNode);
		}
	}
	void show(bool reversed = false) {
		if (expression == "") {
			cout << "\nNothing to show :(\n";
			return;
		}
		if (!checkExpression()) {
			cout << "\nExpression is wrong\n";
			return;
		}
		if (reversed) {
			showMatrixReverse(matrix);
		} else {
			showMatrix(matrix);
		}
	}
	bool checkExpression() {
		return good;
	}
	int count() {
		return mainNode->count(mainNode);
	}
	int countLeft(bool &good) {
		if (mainNode->firstChild) {
			return mainNode->count(mainNode->firstChild);
			good = true;
		} else {
			good = false;
			return 0;
		}
	}
	int countRight(bool &good) {
		if (mainNode->secondChild) {
			good = true;
			return mainNode->count(mainNode->secondChild);
		} else {
			good = false;
			return 0;
		}
	}
};

void test(string expressionStr) {
	Expression* test;
	bool goodExpr = true;
	test = test->createExpr(expressionStr, goodExpr);
	if (goodExpr) {
		showMatrix(test->getMatrix(test));
		cout << "Result: " << test->count(test) << endl;
		if (contains(expressionStr, '*')) cout << "Expression contains operation \"multiply\" (*)";
		else cout << "Expression does not contains operation \"multiply\" (*)";
	} else {
		cout << "Expression is wrong";
	}
}

void showCommands() {
	cout 
	<< "\n1 - display tree\n"
	<< "2 - display reversed tree\n"
	<< "3 - display tree's result\n"
	<< "4 - show expression\n"
	<< "5 - change expression\n"
	<< "6 - count left tree\n"
	<< "7 - count right tree\n"
	<< "0 - show commands\n";
}

int main() {
	string command;
	showCommands();
	Tree* tree = new Tree();
	string expression = "";
	bool good = true;
	int counter;

	while (true) {
		cin >> command;
		if (command == "1") {
			tree->show();
		} else 
		
		if (command == "2") {
			tree->show(true);
		} else 
		
		if (command == "3") {
			if (tree->checkExpression()) {
				cout << endl << "Result: " << tree->count() << endl;
			} else {
				cout << "\nExpression is wrong\n";
			}
		} else 
		
		if (command == "4") {
			if (expression == "") {
				cout << "\nNothing to show :(\n";
			} else {
				cout << endl << expression << endl;
			}
		} else 
		
		if (command == "5") {
			cout << "Input new expression: ";
			cin >> expression;
			tree->changeData(expression);
		} else 

		if (command == "6") {
			counter = tree->countLeft(good);
			if (good) {
				cout << "Result: " << counter << endl << endl;
			} else {
				cout << "There's no left tree\n\n";
			}
		} else 
		
		if (command == "7") {
			counter = tree->countRight(good);
			if (good) {
				cout << "Result: " << counter << endl << endl;
			} else {
				cout << "There's no right tree\n\n";
			}
		} else {
			showCommands();
		}
	}

	return 0;
}