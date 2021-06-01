
#include "data_structure/Calculator.h"

#include <math.h>

int Calculator::result() {
    int pos = 0;
    while (pos < expression_.size()) {
        if (expression_[pos] >= '0' && expression_[pos] <= '9') {
            num_stack_.push(getNumber(expression_, pos));
        }
        else if (expression_[pos] == '(') {
            op_stack_.push(expression_[pos]);
        }
        else if (expression_[pos] == ')') {
            while (!op_stack_.empty()) {
                char op = op_stack_.top();
                op_stack_.pop();
                if (op == '(') break;
                else num_stack_.push(binaryOp(op, num_stack_));
            }
        }
        else if (expression_[pos] == '+' || expression_[pos] == '-') {
            while (!op_stack_.empty()) {
                char op = op_stack_.top();
                if (op == '(') break;
                op_stack_.pop();
                num_stack_.push(binaryOp(op, num_stack_));
            }
            op_stack_.push(expression_[pos]);
        }
        else if (expression_[pos] == '*' || expression_[pos] == '/') {
            while (!op_stack_.empty()) {
                char op = op_stack_.top();
                if (op == '(' || op == '+' || op == '-') break;
                op_stack_.pop();
                num_stack_.push(binaryOp(op, num_stack_));
            }
            op_stack_.push(expression_[pos]);
        }
        else if (expression_[pos] == '^') op_stack_.push(expression_[pos]);
        ++pos;
    }
}

int getNumber(const string& expression, int& pos) {
    int num = 0;
    while (expression[pos] < expression.size() && expression[pos] >= '0' 
        && expression[pos] <= '9') {
            num *= 10;
            num += static_cast<int>(expression[pos] - '0');
            ++pos;
    }
    if (pos != expression.size()) --pos;
    return num;
}

int binaryOp(const char op, stack<int>& num_stack) {
    int n1 = num_stack.top();
    num_stack.pop();
    int n2 = num_stack.top();
    num_stack.pop();
    if (op == '+') return n1 + n2;
    if (op == '-') return n1 - n2;
    if (op == '/') return n1 / n2;
    if (op == '*') return n1 * n2;
    if (op == '^') return pow(n1, n2);
    return 0;
}
