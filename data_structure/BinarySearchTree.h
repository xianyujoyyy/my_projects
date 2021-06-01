
#include <stdio.h>

template<class KEY, class VALUE>
class BinarySearchTree {
public:
    BinarySearchTree() {
        root = NULL;
    }
    ~BinarySearchTree() {
        makeEmpty(root);
    }

    pair<KEY, VALUE>* find(const KEY& key) const;
    void insert(const pair<KEY, VALUE>& data);
    void remove(const KEY& key);

private:
    struct Node {
        pair<KEY, VALUE> data;
        Node* left,* right;
        Node(pair<KEY, VALUE> _data, Node* _left = NULL, Node* _right = NULL) 
            : data(_data), left(_left), right(_right) {}
    };
    Node* root;

    void makeEmpty(Node* node);
    Node* find(const KEY& key, Node* node) const;
    void insert(const pair<KEY, VALUE>& data, Node*& node);
    void remove(const KEY& key, Node*& node);

};
