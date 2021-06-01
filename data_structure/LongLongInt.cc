
#include "data_structure/LongLongInt.h"

LongLongInt::LongLongInt(const char* n) {
    int len = strlen(n);
    if (len == 0) {
        sign = '+';
        num = new char[2];
        num[0] = '0';
        num[1] = '\0';
        return;
    }
    int i = 0;
    if (n[i] == '+' || n[i] == '-') {
        sign = n[i];
        ++i;
    }
    else sign = '+';
    num = new char[len - i + 1];
    for (int j = 0; j < len - i + 1; ++j) {
        num[j] = n[i + j];
    }
    num[len - i] = '\0';
    return;
}

LongLongInt::LongLongInt(const LongLongInt& n) {
    sign = n.sign;
    int len = strlen(n.num);
    num = new char[len + 1];
    strcpy(num, n.num);
    num[len] = '\0';
}

LongLongInt& LongLongInt::operator=(const LongLongInt& other) {
    if (this == &other) return *this;
    sign = other.sign;
    delete num;
    int len = strlen(other.num);
    num = new char[len + 1];
    strcpy(num, other.num);
    num[len] = '\0';
    return *this;
}

LongLongInt operator+(const LongLongInt& n1, const LongLongInt& n2) {
    LongLongInt n;
    n.sign = n1.sign;
    if (n1.sign == n2.sign) {
        n.num = add(n1.num, n2.num);
    }
    else if (strcmp(n.num, n2.num) > 0) {
        n.num = sub(n1.num, n2.num);
    }
    else {
        n.sign = n2.sign;
        n.num = sub(n2.num, n.num);
    }
    return n;
}

LongLongInt operator-(const LongLongInt& n1, const LongLongInt& n2) {
    LongLongInt tmp(n2);
    if (tmp.sign == '+') tmp.sign = '-';
    else tmp.sign = '+';
    return n1 + tmp;
}

ostream& operator<<(ostream& os, const LongLongInt& n) {
    int len = strlen(n.num);
    os << n.sign;
    for (int i = 0; i < len; ++i) {
        os << n.num[i];
    }
}

char* add(const char* n1, const char* n2) {
    int len1 = strlen(n1), len2 = strlen(n2), len = max(len1, len2);
    char* tmp = new char[max(len1, len2) + 1];
    tmp[len] = '\0';
    int i = len - 1, j = len1 - 1, k = len2 - 1, carry = 0;
    while (j >= 0 && k >=0) {
        int sum = static_cast<int>(n1[j] + n2[k] - '0') + carry;
        carry = sum / 10;
        sum %= 10;
        tmp[i] = sum + '0';
        --i;
        --j;
        --k;
    }
    while (j >= 0) {
        int sum = static_cast<int>(n1[j] - '0') + carry;
        carry = sum / 10;
        sum %= 10;
        tmp[i] = sum + '0';
        --i;
        --j;
    }
    while (k >= 0) {
        int sum = static_cast<int>(n2[k] - '0') + carry;
        carry = sum / 10;
        sum %= 10;
        tmp[i] = sum + '0';
        --i;
        --k;
    }
    if (carry) tmp[--i] = '0' + carry;
    return tmp + i;
}

char* sub(const char* n1, const char* n2) {
    int len1 = strlen(n1), len2 = strlen(n2);
    char* tmp = new char[len1 + 1];
    tmp[len1] = '\0';
    int i = len1 - 1, j = len1 - 1, k = len2 - 1, minus = 0;
    while (j >= 0 && k >= 0) {
        int num1 = static_cast<int>(n1[j] - '0'), num2 = static_cast<int>(n2[k] - '0');
        if (num1 - minus >= num2) {
            tmp[i] = num1 - minus - num2 + '0';
            minus = 0;
        }
        else {
            tmp[i] = num1 + 10 - num2 + '0';
            minus = 1;
        }
        --i;
        --j;
        --k;
    }
    while (j >= 0) {
        int num1 = static_cast<int>(n1[j] - '0');
        if (num1 - minus >= 0) {
            tmp[i] = num1 - minus + '0';
            minus = 0;
        }
        else {
            tmp[i] = num1 + 10 - '0';
            minus = 1;
        }
        --i;
        --j;
    }
    while (tmp[i] == '0') ++i;
    return tmp + i;
}

int main() {
    LongLongInt n1("+10000"), n2(n1), n3 = n1;
    cout << n1 << n2 << n3 << endl;
    LongLongInt n4("20000");
    cout << n1 + n4 << n1 - n4 << endl;
    LongLongInt n5("-10000"), n6("399");
    cout << n5 - n6 << n5 + n6 << endl;
    return 0;
}
