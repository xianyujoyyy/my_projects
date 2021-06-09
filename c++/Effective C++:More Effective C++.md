# 尽量以const,enum,inline替换#define

对于单纯常量，最好以const对象或enums替代#defines。

对于形似函数的宏（macros），最好改用inline函数替换#defines。

