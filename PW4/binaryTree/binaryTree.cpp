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

class BinTree {
    Node* root = nullptr;

public:
    BinTree() {}
    BinTree(Node* node) {
        root = node;
    }

    // int countNodes() {
    //     Node* node = root;
    //     return countNodes(node);
    // }

    // int countNodes(Node* node) {
    //     if (!node) return 0;
    //     else {
    //         return 1 + countNodes(node->left) + countNodes(node->right);
    //     }
    // }

    void add(Node* node) {
        if (!root) {
            root = node;
            return;
        }

        add(root, node);
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
        Node* foundNode = root;
        while (true) {
            compares++;
            if (!foundNode) {
                return nullptr;
            }
            compares++;
            if (key == foundNode->key) {
                return foundNode;
            }
            compares++;
            if (key > foundNode->key) {
                foundNode = foundNode->right;
            }
            else {
                foundNode = foundNode->left;
            }
        }
    }

    // int determineLevel(string key) {
    //     Node* node = root;
    //     int level = 0;
    //     while ((node->left != nullptr || node->right != nullptr)) {
    //         if (node->key == key) return level;
    //         if (node->key > key && node->left != nullptr) {
    //             node = getLeftNode(node);
    //         }
    //         else if (node->right != nullptr && node->key < key) {
    //             node = getRightNode(node);
    //         }
    //         else {
    //             return -1;
    //         }
    //         level++;
    //     }
    //     if (node->key == key) return level;
    //     return -1;
    // }


    // double averageBFS()
    // {

    //     queue<Node*> queue;
    //     queue.push(root);

    //     int sum = 0;
    //     int count = 0;

    //     while (!queue.empty()) {
    //         Node* node = queue.front();
    //         queue.pop();

    //         sum += node->key;
    //         count++;
    //         if (node->left != nullptr) {
    //             queue.push(node->left);
    //         }
    //         if (node->right != nullptr) {
    //             queue.push(node->right);
    //         }


    //     }
    //     return static_cast<double>(sum) / count;
    // }



};

struct File {
    BinTree binTree;
    string binaryFilePath;

public:
    void createTree(string textFilePath, string binaryFilePath) {
        ifstream textFile(textFilePath);
        this->binaryFilePath = binaryFilePath;
        ofstream binaryFile(binaryFilePath, ios::binary);

        string line, key;
        Node* node = nullptr;
        int lineIndex = 0;
        auto size = line.size();
        while (!textFile.eof()) {
            getline(textFile, line);
            size = line.size();

            key = line.substr(0, line.find(","));
            node = new Node(key);
            node->lineIndex = lineIndex;
            binTree.add(node);

            binaryFile.write((char*)&size, sizeof(size));
            binaryFile.write((char*)&line[0], size);
            lineIndex++;
        }

        textFile.close();
        binaryFile.close();
    }

    void find(string key) {
        Node* foundNode = binTree.findNode(key);
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
        binTree.printTree();
    }

    void deleteByKey(string key) {
        binTree.deleteNode(key);
    }
};

int main() {
    File file;
    file.createTree("../data.txt", "../dataBinary.bin");
    file.print();
    file.deleteByKey("people");
    file.find("water");
    file.print();

    // for (int i = 0; i < 10000000; i++) {
	// 	file.binTree.add(new Node(to_string(i)));
	// }

	// chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    // file.binTree.findNode("5000000");
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
