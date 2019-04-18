//
// Created by pasha on 13.04.2019.
//

#include <iostream>
#include <cstddef>
#include <queue>
#include <stack>

template <typename Key>
struct Node {
    Key key;
    size_t priority;
    Node *left, *right;

    Node<Key>( Key key, size_t priority ):
        key(key), priority(priority),
        left(nullptr), right(nullptr) {}
};

template <typename Key>
class BTree {
public:
    ~BTree(){
        deleteTree(root);
    }

    int MaxWidth() const {
        if ( root == nullptr ) {
            return 0;
        }
        Node<Key> *node = nullptr;
        std::queue<Node<Key>*> queue;
        queue.push(root);

        int currentWidth, nextWidth, maxWidth;

        currentWidth = maxWidth = 1;
        nextWidth = 0;

        while ( !queue.empty() ) {
            if ( currentWidth == 0 ) {
                currentWidth = nextWidth;
                nextWidth = 0;

                if ( currentWidth > maxWidth ) {
                    maxWidth = currentWidth;
                }
            }

            node = queue.front();
            if ( node->left ) {
                queue.push(node->left);
                nextWidth++;
            }
            if ( node->right ) {
                queue.push(node->right);
                nextWidth++;
            }
            queue.pop();
            currentWidth--;
        }
        return maxWidth;
    }

    void InOrder() const {
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

    Node<Key>* BinarySearch( Key key ) const {
        return binarySearch(root, key);
    }

protected:
    Node<Key> *root = nullptr;

    void deleteTree( Node<Key> *currentNode ) {
        if ( currentNode == nullptr )
            return;

        deleteTree(currentNode->left);
        deleteTree(currentNode->right);
        delete currentNode;
    }

    Node<Key>*& binarySearch( Node<Key> *&currentNode, Key key) const {
        if ( currentNode == nullptr ) {
            return nullptr;
        }
        if ( currentNode->key == key ) {
            return currentNode;
        } else {
            if ( currentNode->key > key ) {
                return binarySearch(currentNode->left, key);
            } else {
                return binarySearch(currentNode->right, key);
            }
        }
    }
};

template <typename Key>
class NaiveBTree: public BTree<Key> {
public:
    void Insert( Key key ) {
        insert(BTree<Key>::root, key);
    }

private:
    void insert(Node<Key> *&currentNode, Key key) {
        if ( currentNode == nullptr ) {
            currentNode = new Node<Key>(key, 0);
        } else {
            if ( currentNode->key > key ) {
                insert(currentNode->left, key);
            } else {
                insert(currentNode->right, key);
            }
        }
    }
};

template <typename Key>
class DecartTree: public BTree<Key> {
public:

    void Insert( Key key, size_t priority );
    void Split( Node<Key> *currentNode, Key key, Node<Key> *&left, Node<Key> *&right );
    Node<Key>* Merge( Node<Key> *left, Node<Key> *right );

private:
    Node<Key>*& binaryPrioritySearch( Node<Key> *&currentNode, Key key, size_t priority ) const {
        if ( currentNode == nullptr || currentNode->priority < priority ) {
            return currentNode;
        } else {
            if (currentNode->key > key) {
                return binaryPrioritySearch(currentNode->left, key, priority);
            } else {
                return binaryPrioritySearch(currentNode->right, key, priority);
            }
        }
    }
};

template <typename Key>
void DecartTree<Key>::Split( Node<Key> *currentNode, Key key, Node<Key> *&left, Node<Key> *&right ) {
    if ( currentNode == nullptr ) {
        left  = nullptr;
        right = nullptr;
        return;
    }

    if ( currentNode->key <= key) {
        Split(currentNode->right, key, currentNode->right, right);
        left = currentNode;
    } else {
        Split(currentNode->left, key, left, currentNode->left);
        right = currentNode;
    }
}

template <typename Key>
Node<Key>* DecartTree<Key>::Merge(Node<Key> *left, Node<Key> *right) {

    if (left == nullptr || right == nullptr) {
        return left == nullptr ? right : left;
    }

    if (left->priority > right->priority) {
        left->right = Merge(left->right, right);
        return left;
    } else {
        right->left = Merge(left, right->left);
        return right;
    }
}

template <typename Key>
void DecartTree<Key>::Insert( Key key, size_t priority ) {

    if ( BTree<Key>::root == nullptr ) {
        BTree<Key>::root = new Node<Key>(key, priority);
        return;
    }

    Node<Key> *&node = binaryPrioritySearch(BTree<Key>::root, key, priority);

    Node<Key> *left;
    Node<Key> *right;

    Split( node, key, left, right );

    Node<Key> *newNode = new Node<Key>(key, priority);
    newNode->left  = left;
    newNode->right = right;

    node = newNode;
}

//template <typename Key>
//bool DecartTree<Key>::Remove( Key key ) {
//    if ( root == nullptr ) {
//        return false;
//    }
//
//    Node<Key> *&node = binarySearch( key );
//
//    if ( node == nullptr ) {
//        return false;
//    } else {
//        node = Merge(node->left, node->right);
//        return true;
//    }
//}

int main() {
    DecartTree<size_t> dtree;
    NaiveBTree<size_t> ntree;

    size_t n;
    size_t key;
    size_t priority;

    std::cin >> n;

    for (size_t i = 0; i < n; ++i) {
        std::cin >> key >> priority;
        dtree.Insert(key, priority);
        ntree.Insert(key);
    }

    dtree.InOrder();
    std::cout << std::endl;
    ntree.InOrder();
    std::cout << std::endl;

    std::cout << ntree.MaxWidth() - dtree.MaxWidth();

    return 0;
}

