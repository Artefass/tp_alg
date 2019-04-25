#include <iostream>
#include <algorithm>
#include <cassert>
#include <stack>

template <typename Type>
struct equal {
    bool operator() ( const Type &left, const Type &right ) {
        return left == right;
    }
};

template <
        typename Key,
        class Less    = std::less<Key>,
        class Greater = std::greater<Key>,
        class Equal   = equal<Key>
        >
class AVLTree {
    struct Node {
        Key key;
        size_t height;
        size_t nodesCount;
        Node *left, *right;

        Node(Key key) : key(key), height(1), nodesCount(1), left(nullptr), right(nullptr) {}
    };

    Less    lt;
    Greater gt;
    Equal   eq;

public:
    ~AVLTree() {
        deleteTree( root );
    }

    void Insert( Key &key ) {
        root = insert( root, key );
    }

    void Remove( const Key &key ) {
        root = remove( root, key );
    }

    Key GetKey( size_t pos ) {
        return getKey(root, pos);
    }
private:
    Node *root = nullptr;

    void deleteTree( Node *node ) {
        if ( node ) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    void fixHeight( Node *node ) {
        assert( node != nullptr );

        size_t leftHeight  = getHeight(node->left);
        size_t rightHeight = getHeight(node->right);
        node->height = std::max(leftHeight, rightHeight) + 1;
    }

    void fixNodesCount( Node *node ) {
        assert( node != nullptr );

        size_t leftCount =  getNodesCount(node->left);
        size_t rightCount = getNodesCount(node->right);
        node->nodesCount = leftCount + rightCount + 1;
    }

    int getBalance( Node *node ) const {
        return getHeight(node->right) - getHeight(node->left);
    }

    size_t getHeight( Node *node ) const {
        if ( node != nullptr )
            return node->height;
        else
            return 0;
    }

    size_t getNodesCount( Node *node ) const {
        if ( node != nullptr )
            return node->nodesCount;
        else
            return 0;
    }

    Node* insert( Node *node, Key &key ) {
        if ( node == nullptr ) {
            return new Node(key);
        } else {
            if ( gt(node->key, key) ) {
                node->left  = insert(node->left, key);
            } else {
                node->right = insert(node->right, key);
            }
            return doBalanced(node);
        }
    }

    Node* remove( Node *node, const Key &key ) {
        if (!node)
            return nullptr;
        if ( lt(node->key, key) )
            node->right = remove(node->right, key);
        else if ( gt(node->key, key) )
            node->left  = remove(node->left, key);
        else {
            Node *left  = node->left;
            Node *right = node->right;

            delete node;

            if ( !right )
                return left;

            Node *min = nullptr;
            right = findAndRemoveMin(right, min);
            min->left  = left;
            min->right = right;
            return doBalanced(min);
        }

        return doBalanced(node);
    }

    Node* findAndRemoveMin( Node *node, Node *&min ) {
        if ( node->left == nullptr ) {
            min = node;
            return node->right;
        }

        node->left = findAndRemoveMin( node->left, min );
        return doBalanced(node);
    }

    Node* doBalanced( Node *node ) {
        fixHeight(node);
        fixNodesCount(node);

        switch (getBalance(node))
        {
            case 2:
            {
                if (getBalance(node->right) < 0)
                    node->right = rightRotate(node->right);
                return leftRotate(node);
            }
            case -2:
            {
                if (getBalance(node->left) > 0)
                    node->left = leftRotate(node->left);
                return rightRotate(node);
            }
            default:
                return node;
        }
    }

    Node* leftRotate( Node *node ) {
        Node *tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        fixHeight(node);
        fixHeight(tmp);
        fixNodesCount(node);
        fixNodesCount(tmp);
        return tmp;
    }

    Node* rightRotate( Node *node ) {
        Node *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        fixHeight(node);
        fixHeight(tmp);
        fixNodesCount(node);
        fixNodesCount(tmp);
        return tmp;
    }

    Key getKey( Node *node, size_t pos ) const {
        assert( node != nullptr);

        size_t leftNodesCount  = getNodesCount(node->left);

        if ( pos == leftNodesCount ) {
            return node->key;
        } else if ( pos > leftNodesCount ) {
            return getKey( node->right, pos - leftNodesCount - 1 );
        } else {
            return getKey( node->left, pos );
        }
    }
};

int main() {
    AVLTree<int> tree;
    size_t n;
    size_t pos;
    int    value;

    std::cin >> n;

    for ( size_t i = 0; i < n; i++ ) {
        std::cin >> value >> pos;
        if ( value > 0 )
            tree.Insert(value);
        else
            tree.Remove(-value);
        std::cout << tree.GetKey(pos) << std::endl;
    }

    return 0;
}