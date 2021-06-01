
// TraversalWithoutRecursion

#include <stdio.h>
#include <iostream>
#include <stack>

using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    explicit TreeNode(int _val = 0)
        : val(_val),
          left(NULL),
          right(NULL) {}
    TreeNode(int _val, TreeNode* _left, TreeNode* _right)
        : val(_val),
          left(_left),
          right(_right) {}
    ~TreeNode() {}
};

struct Node {
    TreeNode* node;
    int times;
    Node(TreeNode* _node, int _times = 0)
      : node(_node),
        times(_times) {}
    ~Node() {}
};

void preOrder(TreeNode* root) {
    if (root == NULL) return;
    stack<TreeNode*> node_stack;
    node_stack.push(root);
    while (!node_stack.empty()) {
       TreeNode* tmp = node_stack.top();
       node_stack.pop();
       cout << tmp->val;
       if (tmp->right) node_stack.push(tmp->right);
       if (tmp->left) node_stack.push(tmp->left);
    }
    return;
}

void midOrder(TreeNode* root) {
    if (root == NULL) return;
    stack<Node*> node_stack;
    node_stack.push(new Node(root));
    while (!node_stack.empty()) {
        Node* tmp = node_stack.top();
        node_stack.pop();
        ++tmp->times;
        if (tmp->times == 2) {
          cout << tmp->node->val;
          if (tmp->node->right) {
            node_stack.push(new Node(tmp->node->right));
          }
          delete tmp;
        }
        else {
          node_stack.push(tmp);
          if (tmp->node->left) {
            node_stack.push(new Node(tmp->node->left));
          }
        }
    }
    return;
}

void postOrder(TreeNode* root) {
    if (root == NULL) return;
    stack<Node*> node_stack;
    node_stack.push(new Node(root));
    while (!node_stack.empty()) {
        Node* tmp = node_stack.top();
        node_stack.pop();
        ++tmp->times;
        if (tmp->times == 3) {
            cout << tmp->node->val;
            delete tmp;
        }
        else if (tmp->times == 1) {
            node_stack.push(tmp);
            if (tmp->node->left) {
               node_stack.push(tmp->node->left);
            }
        }
        else if (tmp->times == 2) {
            node_stack.push(tmp);
            if (tmp->node->right) {
                node_stack.push(tmp->node->right);
            }
        }
    }
    return;
}
