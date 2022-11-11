#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

int compares = 0;

struct Node {
    Node* left;
    Node* right;
    string key;
    int lineIndex;

    Node(string key) {
        this->key = key;
        this->lineIndex = -1;
        left = nullptr;
        right = nullptr;
    }
};

int turns = 0;

struct SplayTree {
    Node* root = nullptr;

public:
    SplayTree() {}
    SplayTree(Node* node) {
        root = node;
    }

    void add(Node* node) {
        if (!root) {
            root = node;
            return;
        }

        add(root, node);
        splay(root, node->key);
    }

private:
    Node* add(Node* currentNode, Node* newNode) {
        if (!currentNode) {
            return newNode;
        }

        if (currentNode->key > newNode->key) {
            currentNode->left = add(currentNode->left, newNode);
        } else {
            currentNode->right = add(currentNode->right, newNode);
        }

        return currentNode;
    }

public:
    void printTree() {
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
        printBinTree(root, 0, 4);
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
    }

private:
    void printBinTree(Node* node, int level, int L) {
        if (node) {
            printBinTree(node->right, level + 1, L);

            cout << endl;
            for (int i = 1; i <= level * L; i++) {
                cout << "  ";
            }
            cout << node->key;

            printBinTree(node->left, level + 1, L);
        }
    }

public:
    void deleteNode(string key) {
        Node* nodeToDelete = root;
        Node* parentNode = root;
        while (true) {
            if (!nodeToDelete) {
                return;
            }

            if (key == nodeToDelete->key) {
                break;
            }

            if (key > nodeToDelete->key) {
                parentNode = nodeToDelete;
                nodeToDelete = nodeToDelete->right;
            } else {
                parentNode = nodeToDelete;
                nodeToDelete = nodeToDelete->left;
            }
        }

        if (!nodeToDelete) {
            return;
        }

        if (!nodeToDelete->left && !nodeToDelete->right) {
            if (parentNode->key > nodeToDelete->key) {
                parentNode->left = nullptr;
            }
            else {
                parentNode->right = nullptr;
            }
        }

        else if (!nodeToDelete->left) {
            if (parentNode->key > nodeToDelete->key) {
                parentNode->left = nodeToDelete->right;
            }
            else {
                parentNode->right = nodeToDelete->right;
            }
        }

        else if (!nodeToDelete->right) {
            if (parentNode->key > nodeToDelete->key) {
                parentNode->left = nodeToDelete->left;
            }
            else {
                parentNode->right = nodeToDelete->left;
            }
        }

        else {
            Node* parentChangeNode = nodeToDelete->left;
            Node* changeNode = parentChangeNode;
            while (changeNode->right) {
                parentChangeNode = changeNode;
                changeNode = changeNode->right;
            }

            if (changeNode != nodeToDelete->left) {
                parentChangeNode->right = changeNode->left;
            }

            if (nodeToDelete == root) {
                changeNode->right = root->right;
                if (changeNode != nodeToDelete->left) {
                    changeNode->left = nodeToDelete->left;
                }

                root = changeNode;
            }

            else {
                if (parentNode->key > changeNode->key) {
                    parentNode->left = changeNode;
                } else {
                    parentNode->right = changeNode;
                }

                if (changeNode != nodeToDelete->left) {
                    changeNode->left = nodeToDelete->left;
                }

                changeNode->right = nodeToDelete->right;
                return;
            }
        }
    }

    Node* findNode(string key) {
        return splay(root, key);
    }

private:
    Node* rightRotate(Node* x) {
        turns++;
        Node* y = x->left;
        x->left = y->right;
        y->right = x;
        return y; 
    }

    Node* leftRotate(Node* x) {
        turns++;
        Node* y = x->right; 
        x->right = y->left; 
        y->left = x;
        return y; 
    }

public:
    Node* splay(Node* &root, string key) {
        compares++;
        if (root == NULL || root->key == key) 
            return root; 

        compares++;
        if (root->key > key) { 
            compares++;
            if (root->left == NULL) return root; 
    
            compares++;
            if (root->left->key > key) { 
                root->left->left = splay(root->left->left, key); 
                root = rightRotate(root); 
            } 

            else {
                compares++;
                if (root->left->key < key) { 
                    root->left->right = splay(root->left->right, key); 
        
                    compares++;
                    if (root->left->right != NULL) 
                        root->left = leftRotate(root->left); 
                } 
            }
    
            compares++;
            root = (root->left == NULL) ? root : rightRotate(root);
            return root; 
        } 
        else { 
            compares++;
            if (root->right == NULL) return root; 
    
            compares++;
            if (root->right->key > key) { 
                root->right->left = splay(root->right->left, key); 

                compares++;
                if (root->right->left != NULL) 
                    root->right = rightRotate(root->right); 
            } 

            else {
                compares++;
                if (root->right->key < key) { 
                    root->right->right = splay(root->right->right, key); 
                    root = leftRotate(root); 
                } 
            }

            compares++;
            root = (root->right == NULL) ? root : leftRotate(root);
            return root; 
        } 
    }
};

struct File {
    SplayTree splayTree;
    string binaryFilePath;
    string textFilePath;
    int maxIndex = 0;

public:
    void createTree(string textFilePath, string binaryFilePath) {
        this->binaryFilePath = binaryFilePath;
        this->textFilePath = textFilePath;
        ifstream textFile(textFilePath);
        ofstream binaryFile(binaryFilePath, ios::binary);

        string line, key;
        Node* node = nullptr;
        auto size = line.size();
        while (!textFile.eof()) {
            getline(textFile, line);
            size = line.size();

            key = line.substr(0, line.find(","));
            node = new Node(key);
            node->lineIndex = maxIndex;
            splayTree.add(node);

            binaryFile.write((char*)&size, sizeof(size));
            binaryFile.write((char*)&line[0], size);
            maxIndex++;
        }

        textFile.close();
        binaryFile.close();
    }

    void find(string key) {
        Node* foundNode = splayTree.findNode(key);
        if (!foundNode) {
            cout << "No such element\n";
            return;
        }

        ifstream binaryFile(binaryFilePath, ios::binary);
        int lineIndex = 0;
        string result;
        size_t size;
        while (!binaryFile.eof()) {
            binaryFile.read((char*)&size, sizeof(size));
            result.resize(size);
            binaryFile.read((char*)&result[0], size);

            if (lineIndex == foundNode->lineIndex) {
                cout << result << endl;
                break;
            }

            lineIndex++;
        }

        binaryFile.close();
    }

    void print() {
        splayTree.printTree();
    }

    void deleteByKey(string key) {
        splayTree.deleteNode(key);
    }
};

int main() {
    File file;
    file.createTree("../data.txt", "../dataBinary.bin");
    float average = (float)turns / 10;
    cout << "Average number of turns: " << average;

    file.print();
    file.find("people");
    file.print();
    file.find("number");
    file.print();
    file.deleteByKey("number");
    file.deleteByKey("water");
    file.print();

    // for (int i = 0; i < 10000; i++) {
	// 	file.splayTree.add(new Node(to_string(i)));
	// }
    // compares = 0;

	// chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    // file.splayTree.findNode("5000");
	// chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    
	// cout << chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count() << endl;
	// cout << compares << endl;

    return 0;

    // cout << "����� �������� � ������ 68839" << endl;
    // high_resolution_clock::time_point t1 = high_resolution_clock::now();
    // high_resolution_clock::time_point t2 = high_resolution_clock::now();

    // cout << "�������� �������� � ������ 85721" << endl;
    // fileManag.deleteNode(85721);

    // auto duration = duration_cast<microseconds>(t2 - t1);  cout << duration.count() << endl;

    /*int key;
    cin >> key;
    Node* root = new Node(key);
    BinTree tree = BinTree(root);
    for (int i = 0; i < 6; i++) {
        cin >> key;
        Node* node = new Node(key);
        tree.add(node);
    }
    tree.printTree();
    cin >> key;
    tree.deleteNode(key);
    tree.printTree();*/


    /*  Node* newNode = new Node(13);
      binTree.add(newNode);
      Node* newNode2 = new Node(32);
      binTree.add(newNode2);
      Node* newNode3 = new Node(54);


      /*cout << endl << binTree.countNodes() << endl;
      binTree.deleteLeftLeave();

      binTree.printTree();

      cout << endl << binTree.countNodes() << endl;*/
}
