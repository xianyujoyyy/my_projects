
#include <iostream>
#include <string.h>

using namespace std;

// +,-,<<,strlen,strcmp,strcpy,

class LongLongInt {
public:
    friend LongLongInt operator+(const LongLongInt&, const LongLongInt&);
    friend LongLongInt operator-(const LongLongInt&, const LongLongInt&);
    friend ostream& operator<<(ostream&, const LongLongInt&);

    explicit LongLongInt(const char* = "");
    explicit LongLongInt(const LongLongInt&);
    LongLongInt& operator=(const LongLongInt&);
    ~LongLongInt() { delete num; }

private:
    char* add(const char* n1, const char* n2);
    char* sub(const char* n1, const char* n2);

    char sign;
    char* num;

};
