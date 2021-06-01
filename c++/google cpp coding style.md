* cpplint

  Pip3 install cpplint

  Cpplint src.cc

  vscode + cpplint

# google cpp coding style

## 变量命名规范

1. 变量名用小写字母，变量之间用下划线隔开， 如time_in_sec
2. 函数名每个单词首字母大写，不用下划线隔开，如GetTime()
3. Class/struct名每个单词首字母大写，不用下划线隔开，如TableInfo
4. class内的变量以下划线结尾，如time_in_sec_;输入类的变量以下划线开头Construction(int _k): k\_(\_k){}
5. **结构体变量不要加下划线**
6. 常数变量以k开头，如 const int kDaysInAWeek = 7；

## 作用域

1. 利用命名空间来避免命名冲突
2. **对于只能在当前文件访问的函数和变量，利用匿名命名空间保护**
3. 局部变量的定语与初始化，一行定义一个，定义的时候初始化
4. 不要使用using namespace xx；可使用using std::xxx;

## 类

1. 避免隐形的数据类型转换，Static_cast<>()，单变量构造函数explicit

2. 必要时禁用operator = 和拷贝构造函数，  

   MyClass(const MyClass&) = delete;  

   MyClass& operator = (const MyClass&) = delete;eg：

   默认拷贝构造函数拷贝数组指针，如果数组析构函数进行了定义，数组指针可能会被析构两次

3. 声明顺序，public在前，private在后

## 函数

### 参数传递

1. 输入参数一般用const&，如void func(const vector\<int>& vec)
2. c++基本类型作为输入参数不需要引用符号，如void func(int x)
3. **输出参数用指针，不要用引用，如void func(int y, int\* x)**
4. 输入参数要放在输出参数前面，如void func(int y, int\* x)

### 函数重载不要仅用变量名来区分

​	void Analyze(const string &text);

​	void Analyze(const char\* text, size_t textlen);

### 其他

1. auto的使用，尽量少用，不清晰
2. 使用c++的类型转换，如static_cast<>().不要使用int y = (int)x 或 int y = int(x)等转换方式
3. 不允许使用缺省函数参数，少数极端情况除外。尽可能改用函数重载
4. 不要把算法逻辑放到构造函数里

