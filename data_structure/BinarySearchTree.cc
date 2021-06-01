
#include "data_structure/BinarySearchTree.h"

template<class KEY, class VALUE>
pair<KEY, VALUE>* BinarySearchTree<KEY, VALUE>::find(const KEY& key) const {
    return &(find(key, root)->data);
}

template<class KEY, class VALUE>
void* BinarySearchTree<KEY, VALUE>::insert(const pair<KEY, VALUE>& data) {
    insert(data, root);
}

template<class KEY, class VALUE>
void* BinarySearchTree<KEY, VALUE>::remove(const KEY& key) {
    remove(key, root);
}

template<class KEY, class VALUE>
void* BinarySearchTree<KEY, VALUE>::makeEmpty(Node* node) {
    if (node == NULL) return;
    makeEmpty(node->left);
    makeEmpty(node->right);
    delete node;
}

template<class KEY, class VALUE>
Node* BinarySearchTree<KEY, VALUE>::find(const KEY& key, Node* node) const {
    if (node == NULL || node->data.first == key) return node;
    if (key < node->data.first) find(key, node->right);
    if (key > node->data.first) find(key, node->left);
}

template<class KEY, class VALUE>
void BinarySearchTree<KEY, VALUE>::insert(const pair<KEY, VALUE>& data, Node*& node) {
    if (node == NULL) {
        node = new Node(data);
        return;
    }
    if (data.first == node->data.first) {
        node->data.second = data.second;
        return;
    }
    if (data.first < node->data.first) {
        insert(data, node->right);
        return;
    }
    if (data.first > node->data.first) {
        find(data, node->left);
        return;
    }
}

template<class KEY, class VALUE>
void BinarySearchTree<KEY, VALUE>::remove(const KEY& key, Node*& node) {
    if (node == NULL) return;
    if (key < node->data.first) {
        remove(key, node->left);
        return;
    }
    if (key > node->data.first) {
        remove(key, node->right);
        return;
    }
    if (key == node->data.first) {
        if (node->right == NULL || node->left == NULL) {
            Node* tmp = node;
            if (node->right) {
                node = node->right;
            }
            else if (node->left) {
                node = node->left;
            }
            delete tmp;
        }
        else {
            Node* tmp = node->right;
            while (tmp->left) {
                tmp = tmp->left;
            }
            node->data = tmp->data;
            remove(tmp->data.first, tmp);
        }
    }
    return;
}
