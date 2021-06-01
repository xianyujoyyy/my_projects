
#include <string>
#include <stack>

using namespace std;

// Suffix calculator

class Calculator {
public:
    explicit Calculator(string _expression)
        : expression_(_expression) {}
    
    int result();

private:
    int getNumber(const string& expression, int& pos);
    int binaryOp(const char op, stack<int>& num_stack);

    string expression_;
    stack<char> op_stack_;
    stack<int> num_stack_;

};
