#include <iostream>
#include <stack>

template <typename Key>
struct Node {
    Key  key;
    Node *left;
    Node *right;
    Node *parent;

    Node(const Key &key): key(key), left(nullptr),
            right(nullptr), parent(nullptr) {}
};

template <typename Key>
class BinaryTree {
public:
    ~BinaryTree() {
        deleteAll(root);
    }

    void Insert(Key &key) {
        insert(root, key);
    }

    void InOrder() {
        inorder();
    }

private:
    Node<Key> *root = nullptr;

    void insert(Node<Key> *&node, Key &key) {
        if ( !node ) {
            node = new Node<Key>(key);
            return;
        }

        if (key < node->key) {
            insert(node->left, key);
        } else {
            insert(node->right, key);
        }
    }

    void deleteAll(Node<Key> *node) {
        if ( !node ) {
            return;
        }

        deleteAll(node->left);
        deleteAll(node->right);
        delete node;
    }

    void inorder() {
        std::stack<Node<Key>*> stackNodes;

        Node<Key> *curr = root;

        while ( !stackNodes.empty() || curr != nullptr ) {

            if ( curr != nullptr ) {
                stackNodes.push(curr);
                curr = curr->left;
            } else {
                curr = stackNodes.top();
                stackNodes.pop();
                std::cout << curr->key << " ";
                curr = curr->right;
            }
        }
    }
};

int main() {

    size_t n;
    size_t key;
    BinaryTree<size_t> bt;

    std::cin >> n;

    for (size_t i = 0; i < n; i++) {
        std::cin >> key;
        bt.Insert(key);
    }

    bt.InOrder();

    return 0;
}