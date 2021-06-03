## 指定字面值的类型

L'a'   u8"hi"   42ULL   1E-3F   3.14159L

前缀

u Unicode16 char16_t     U Unicode32 char32_t

L wchar_t       u8 UTF-8 char

后缀

整型u/U unsigned  l/L long ll/LL long long 浮点型f/F float l/L long double

## 默认初始化

内置类型的变量未被显式初始化，它的值由定义的位置决定。定义于函数体之外的变量被初始化为0，定义于函数体内部的内置类型变量将不被初始化。一个未被初始化的内置类型变量的值是未定义的，如果试图拷贝或以其他形式访问此类值将引发错误。

建议初始化每一个内置类型的常量。

## 引用

引用即别名，不是一个对象，由类型相同的对象初始化，绑定在对象上。

## 指针

间接访问，是一个对象。

*解引用符，&取地址符。

最好使用nullprt，而非NULL。

Void*是一种特殊的指针类型，可用于存放任意对象的地址。

**面对一条比较复杂的指针或引用的声明语句时，从右向左阅读有助于弄清它的真实含义。int *&r = p；r是int指针的引用。**

## const

默认状态下，const对象仅在文件内有效。编译时会将const对象替换为对应值。

引用：常量引用const int&，不允许通过这个引用改变被绑定对象的值，不允许非常量引用用常量引用初始化，允许常量引用初始化时直接绑定能转化为此类型的值或者表达式或者字面值，允许被引用对象改变值。

指针：指向常量的指针const double* p,p自以为指向了一个const对象。不允许通过p改变指针指向的对象，不允许普通指针用p初始化，允许用非常量对象初始化p，允许被指向对象改变值，允许p指向其他对象。

常量指针int *const p,不变的是指针本身的值，而非指向的值。不允许改变指针本身，但可以通过指针改变被指向对象。

顶层const表示指针本身是个常量，底层const表示指针所指的对象是一个常量。

更一般的，顶层const表示任意的对象是常量，底层const则与复合类型的基本类型部分有关。一般来说，非常量可以转换为常量，反之则不行。

## constexpr

将变量声明为constexpr类型以便由编译器来验证变量的值是否是一个常量表达式。

一般来说，如果认定变量是一个常量表达式，那就把它声明成constexpr类型。

constexpr声明定义一个指针，constexpr int *q,相当于顶层const。

## auto

auto推断类型，定义的变量必须有初始值。

一般会忽略掉顶层const，同时底层const则会保留下来。如果希望推断出的auto类型是一个顶层const，需要明确指出。const auto &j =42；

int ia[10],auto ia2(ia);指针而非数组。

## decltype

从表达式的类型推断出要定义的变量的类型，但是不用该表达式的值初始化变量。包括顶层const和引用在内。

Decltype((i)), decltype(*p),int&.    int ia[10],decltype(ia),返回的类型是由10个整数构成的数组。

## string

Isalnum(c) 字母或数字 isalpha(c) 字母 iscntrl(c) 控制字符 isdigit(c) 数字 isgraph(c) 不是空格但可打印 islower(c) 小写字母

isprint(c) 可打印字符 ispunct(c) 标点符号 isspace(c) 空格 isupper(c) 大写字母 isxdigit(c) 十六进制数字 tolower(c) 转小写

toupper(c) 转大写

## 数组

列表初始化 int a2[] = {0, 1. 2}; int a3[5] = {0, 1, 2};      char a1[]初始化时要末尾添加空字符'\0'

int (*Parray) [10] = &arr; int (&arrRef) [10] = arr; 指向一个含有10个整数的数组，引用一个含有10个整数的数组。

int *(&array)[10] = ptrs; array是数组的引用，该数组含有10个指针。

## 指针和数组

在很多用到数组名字的地方，编译器都会自动地将其替换为一个指向数组首元素的指针。string *p2 = nums;

Begin(arr),end(arr)得到数组首元素和尾后指针。尾后指针不能解引用和递增。

### C风格字符串

将字符串存放在字符数组中并以空字符结束。'\0'

<string.h> strlen,strcmp,strcat,strcpy.

string专门提供了一个名为c_str的成员函数：const char* str = s.c_str(); 如果执行完c_str()函数后程序想一直都能使用其返回的数组，最好将该数组重新拷贝一份。

vector<int> ivec(begin(int_arr), end(int_arr));

要使用范围for语句处理多维数组，除了最内层的循环外，其他所有循环的控制变量都应该是引用类型（避免被转换为指针）。

## 左值 右值

当一个对象被用作右值的时候，用的是对象的值（内容）；当对象被用作左值的时候，用的是对象的身份（在内存中的位置）。

赋值运算符需要一个左值作为其左侧运算对象，得到的结果也仍然是一个左值。

取地址符作用于一个左值运算对象，返回一个指向该运算对象的指针，这个指针是一个右值。decltype(&p) int**

内置解引用运算符、下标运算符的求值结果都是左值。decltype(*p) int&

内置类型和迭代器的递增递减运算符作用于左值运算对象，其前置版本所得的结果也是左值。

## 求值顺序

f() + g() * h() + j()

如果f\g\h\j是无关函数，它们既不会改变同一对象的状态也不执行IO任务，那么函数的调用顺序不受限制。反之，如果其中某几个函数影响同一对象，则它是一条错误的表达式，将产生未定义的行为。

%结果的符号遵循第一个数字。

Cout << ((grade < 60) ? "fail" : "pass");

cout<< 42 + 10;

cout << (10 < 42);

sizeof得到数组大小：sizeof(ia)/sizeof(*ia)

## 隐式转换

整型提升，小于无符号数的带符号数转换为无符号数，int->double

数组转换成指针（decltype,&,sizeof,typeid时不会发生），指针的转换，转换成布尔类型（非0即为true），转换成常量（普通指针和引用可以转换为const指针和引用，反之不行），类类型定义的转换（只能执行一次）。

## 显式转换

Cast_name<type>(expression);

### static_cast

任何具有明确定义的类型转换，只要不包含底层const，都可以使用static_cast。

将较大的算术类型赋值给较小的类型，找回存在于void*指针中的值。

### const_cast

改变底层const。

### reinterpret_cast

通常为运算对象的位模式提供较低层次上的重新解释。

## static

C:1.用于函数内部修饰变量，即函数内的静态变量。这种变量的生存期长于该函数，使得函数具有一定的“状态”。使用静态变量的函数一般是不可重入的，也不是线程安全的。

2.用在文件级别（函数体之外），修饰变量或函数，表示该变量或函数只在本文件可见，其他文件看不到、也访问不到该变量和函数（internal linkage)。

c++:3.用于修饰class的数据成员，即所谓“静态成员”。这种数据成员的生存期大于class的对象。静态数据成员是每个class有一份，普通数据成员是每个instance有一份，因此也分别叫做class variable和instance variable。使用作用域运算符或者类的对象都可以访问。静态数据成员可以是类本身，可以使用静态成员作为默认实参。

4.用于修饰class的成员函数，即所谓“静态成员函数”。这种成员函数只能访问class variable和其他静态程序函数，不能访问instance variable或instance method。

## 尽量使用常量引用

把函数不会改变的形参定义成普通引用是一种比较常见的错误，这么做带给函数的调用者一种误导，即函数可以修改它的实参的值。此外，使用引用而非常量引用也会极大的限制函数所能接受的实参类型。不能把const对象、字面值或者需要类型转换的对象传递给普通的引用传递。

## main处理命令行选项

int main(int argc, char **argv) { ... }

argc表示数组中字符串的数量，argv的第一个元素指向程序的名字或者一个空字符串。

## 含有可变形参的函数

Initializer_list<T> lst;

## 返回数组指针

因为数组不能被拷贝，所以函数不能返回数组。不过，函数可以返回数组的指针或引用。

Typedef int arrT[10]; using arrT = int[10];

arrT* func(int i);   /   int (*func(int i)) [10];

**尾置返回类型** auto func(int i) -> int (*) [10];

**decltype** int odd[] = {...};      decltype(odd) *arrPtr(int i);

## 重载和const形参

顶层const不影响传入函数的对象。一个拥有顶层const的形参无法和另一个没有顶层const的形参区分开。

等价声明：

int func(int); int func(const int);

Int func(int const*); int func(int const\*);

重载：

int func(int&); int func(const int&);

Int func(int*); int func(const int\*);

## 默认实参初始值

局部变量不能作为默认实参。除此之外，只要表达式的类型能转换成形参所需的类型，该表达式就能作为默认实参。

```c++
sz wd = 80;
char def = ' ';
sz ht();
string screen(sz = ht(), sz = wd, char = def);
// 用作默认实参的名字在函数声明所在的作用域内解析，而这些名字的求值过程发生在函数调用时
void f2() {
  def = '*';					// 改变默认实参的值
  sz wd = 100;				// 隐藏了外层定义的wd，但是没有改变默认值
  window = screen();	// 调用screen(ht(), 80, '*')
}
```

## 函数指针

函数指针指向某种特定类型，由它的返回类型和形参类型共同决定，与函数名无关。

bool lengthCompare(const string &, const string &);

bool (*pf) (const string &, const string &);

赋值pf = lengthCompare;  pf = &lengthCompare;

调用bool b1 = pf(...); bool b2 = (*pf)(...); bool b3 = lengthCompare(...);

函数指针形参：

和数组类型，虽然不能定义函数类型的形参，但是形参可以是指向函数的指针。此时，形参看起来是函数类型，实际上却是当成指针使用：

void func(bool pf(...)); void func(bool (*pf)(...));

将函数作为实参使用，会自动转换成指针。

类型别名：

typedef bool func(...); // 函数

typedef decltype(function_name) func; // 函数

typedef bool (*pf) (...); // 函数指针

typedef decltype(function_name) *pf; // 函数指针

using F = int(int*, int); // 函数

using PF = int(\*) (int*, int); // 函数指针 

直接声明：

int (*f1(int)) (int \*, int);

尾置 auto f1(int) -> int (*) (int\*, int);

## this指针

this是一个常量指针（顶层const）。

**引入const成员函数**：默认情况下，this的类型是指向类类非常量版本的常量指针，意味着我们无法在一个常量对象上调用普通的成员函数。const的作用是修改隐式this指针的类型。把this设置为指向常量的指针有助于提高函数的灵活性。

Class Foo;

Const Foo foo; foo->mem_func(); \\\ mem_func必须为常量const成员函数。

### mutable

即使在const成员函数内，希望修改类的某个数据成员。在变量的声明加入mutable关键字。

Mutable int ...;

## 友元

友元函数定义在类的内部，是隐式内联的。

友元不存在传递性。

成员函数也可以作为友元，friend void Foo::clear(); 需要仔细组织程序的结构以满足声明和定义的彼此依赖关系。声明另一类，声明Foo，不定义，定义另一类，定义Foo。

函数重载声明为友元，需要将每一个都声明为友元。

友元不是声明，只影响访问权限。

## 委托构造函数

```c++
class Sales_data {
public:
  	Sales_data(std::string s, unsigned cnt, double price) :
  			bookNo(s), units_sold(cnt), revenue(cnt * price) {}
  	Sales_data():Sales_data("", 0, 0) {}
  	Sales_data(std::string s):Sales_data(s, 0, 0) {}
};
```

## 隐式的类类型转换

转换构造函数：能通过一个实参调用的构造函数定义了一条从构造函数的参数类型向类类型隐式转换的规则。只允许一步。

explicit阻止隐式转换。显式转换 item.combine(Sales_data(...));  item.combine(static_cast<Sales_data>(...));直接使用构造函数完成转换。

### 不完全类型

已经声明但是尚未定义的类型。不完全类型不能用于定义变量或者类的成员，但是用不完全类型定义指针或者引用是合法的。

# IO库

iostream

istream, wistream 从流读取数据

ostream,wostream 向流写入数据

iostream，wiostream 读写流

fstream

ifstream,wifstream 从文件读写数据

ofstream,wofstream 向文件写入数据

fstream,wfstream 读写文件

sstream

istringstream,wistringstream 从string读取数据

ostringstream,wostringstream 向string写入数据

stringstream,wstringstream 读写string

IO对象无拷贝或赋值，引用传递，不能是const。

while(cin >> word) // 读取成功

刷新输出缓冲区：endl 换行刷新 flush 刷新 ends 空字符刷新，cout << unitbuf; // 所有输出操作后都会立即刷新缓冲区 

cout << nounitbuf; // 回到正常的缓冲方式

# 顺序容器

接受两个迭代器参数的构造函数用这两个迭代器表示想要拷贝的元素范围，首元素和尾后元素。新容器中的每个元素都用范围中对应元素的值进行初始化。

标准库array具有固定大小，定义时，需指定元素类型和容器大小。 array<int, 42>,可以进行拷贝和对象赋值操作。

assign操作不适用于关联容器和array。替换元素。

Seq.assign(b,e) 双迭代器，seq.assign(i1)列表，seq.assign(n,t)n个值为t。

swap两个array会真正交换它们的元素，其他不会。

除了无序关联容器之外的所有容器都支持关系运算符，关系运算符左右两边的运算对象必须是相同类型的容器。容器的关系运算符使用元素的关系运算符完成比较。

添加元素：c.insert(p,t) c.emplace(p,args) 在迭代器p指向的元素之前创建一个值为t或由args创建的元素。返回指向新添加的元素的迭代器。 c.insert(p,n,t) c.insert(p,b,e) c.insert(p,i1)。向一个vector,string或deque插入元素会使所有指向容器的迭代器，引用和指针失效。

c.emplace_back(args) == c.push_back(Foo(args))

删除元素：c.erase(p), c.erase(b,e) 删除p所指，be范围内的元素，返回删除元素的后一个位置的迭代器。 c.clear(),返回void。

resize和reserve：resize来增大或缩小容器，如果当前大小大于所要求的大小，容器后部的元素会被删除，如果当前大小小于新大小，会将新元素添加到容器后部。如果容器保存的是类类型元素，且resize向容器添加新元素，则必须提供初始值，或者元素类型必须提供一个默认构造函数。

reserve：只有当需要的内存空间超过当前capacity时，reserve调用才会改变vector的容量，如果需求大小大于当前容量，reserve至少分配与需求一样大的内存空间（可能更大）。如果需求大小小于或等于当前容量，reserve什么都不做。特别是，当需求大小小于当前容量时，容器不会退回内存空间。因此，在调用reserve之后，capacity将会大于或等于传递给reserve的参数。

reserve永远也不会减少容器占有的内存空间，resize成员函数只改变容器中元素的数目，而不是容器的容量。

可以调用shrink_to_fit来要求deque\vector\string退回不需要的内存空间，具体实现可以选择忽略此请求。

### string

还支持三种构造函数，string s(cp,n)字符数组前n个字符，string s(s2,pos)string s2从下标pos2开始的字符的拷贝，string s(s2,pos2,len2)string s2从下标pos2开始len2个字符的拷贝。不管len2长度多少，最多拷贝到s2结尾。

append:在string末尾进行插入操作的一种简写形式。

replace：调用erase和insert的一种简写形式。s.replace(pos, len, s2); 从pos开始删除len长度的字符，并插入s2。

find：找到就返回第一个匹配位置的下标（string::size_type），否则返回string::npos。

s.find(args) 查找s中args第一次出现的位置 s.rfind(args) 查找s中args最后一次出现的位置

s.find_first_of(args) 在s中查找args中任何一个字符第一次出现的位置

s.find_last_of(args)  s.find_first_not_of(args)  s.find_last_not_of(args)

Args:c,pos 从s中位置pos开始查找字符c，pos默认为0. S2,pos 从s中位置pos开始查找字符串s2.pos默认为0.

cp，pos 从s中位置pos开始查找字符数组，pos默认值为0。 cp,pos,n 从s中位置pos开始查找字符数组的前n个字符。pos和n无默认值。

Compare: s.compare 返回值类似strcmp 参数形式 s2  pos1,n1,s2将s中从pos1开始的n1个字符与s2进行比较。

Pos1,n1,s2,pos2,n2.   cp C字符数组。 Pos1，n1，cp。   Pos1,n1,cp,n2

与数值转换：to_string(val) val可以是任何算数类型，小整型会被提升到int。

stoi，stol,stoul,stoll,stoull(s,p,b) b表示基数，默认为10，p为起始转换位置，默认为0.

Stof,stod,stold(s,p) p为起始转换位置，默认为0.

adapter:stack<string, vector<string>>

# 泛型算法

写容器元素的算法：必须注意确保序列原大小至少不小于算法要写入的元素数目，算法不会执行容器操作，因此它们自身不可能改变容器的大小。

back_inserter插入迭代器，接受一个指向容器的引用，返回一个与该容器绑定的插入迭代器。通过此迭代器赋值时，赋值运算符会调用push_back将一个具有给定值的元素添加到容器中。 Fill_n(back_inserter(vec), 10, 0);

**lambda**:谓词，可调用对象（函数，函数指针），未命名的内联函数。

[capture list\](parameter list)->return type{function body}可以忽略参数列表和返回类型，但必须永远包含捕获列表和函数体。

[\](const string& a, const string& b) { return a.size() < b.size(); } 空捕获列表表明此lambda不使用它所在函数中的任何局部变量。

lambda可以出现在一个函数中，使用其局部变量，但它只能使用那些明确指明的变量。一个lambda通过将局部变量包含在其捕获列表中来指出将会使用这些变量。捕获列表指引lambda在其内部包含访问局部变量所需的信息。

[sz\](const string&a) { return a.size() >= sz; }

捕获列表只用于局部非static变量。lambda可以直接使用局部static变量和在它所在函数之外声明的名字。

值捕获，引用捕获：[sz\] [&sz\]   当以引用方式捕获一个变量时，必须保证在lambda执行时变量是存在的。

隐式捕获：编译器根据lambda代码推断要捕获的变量，&或者=，&引用捕获，=值捕获。可以混合使用隐式捕获和显式捕获，[&,c]，第一个符号必须为&或=。

可变lambda：默认情况下，对于一个值被拷贝的变量，lambda不会改变其值。如果希望能改变一个被捕获变量的值，就必须在参数列表首加上关键字mutable。也可以省略参数列表。[v1\] () mutable { return ++v1; }

一个引用捕获的变量是否可以修改依赖于此引用指向的是一个const类型还是一个非const类型。

当需要为一个lambda定义返回类型时，必须使用尾置返回类型。[\] (int i) -> int { function body; }

**bind**: 通用函数适配器。

auto wc = find_if(words.begin(), words.end(), [sz\](const string& a)) == auto wc = find_if(words.begin(), words.end(), bind(check_sz, _1, sz));

Using std::placeholders::_1;   using namespace std::placeholders;

绑定引用参数 ref() cref() 也定义在functional中，bind(print, ref(os), _1, ' ');

## 迭代器

插入迭代器：迭代器适配器，接受一个容器，生成一个迭代器，能实现向给定容器添加元素。对插入迭代器赋值时，该迭代器调用容器操作来向给定容器的指定位置插入一个元素。back_inserter, front_inserter, inserter(n, insert_pos_iter)。

5类迭代器：泛型算法接受最小类别的迭代器。

输入迭代器（顺序读取，单遍扫描），输出迭代器（顺序写，单遍扫描），前向迭代器（读写，沿一个方向移动，可以多遍扫描），双向迭代器（正向/反向读写），随机访问迭代器（常数时间内访问序列中任意元素的能力）。

## 特定容器算法

lst.merge(lst2) lst.merge(lst2,cmp) 合并，两个lst都必须有序 lst.remove(val) lst.remove_if(pred) 删除一个值，或者使谓词为真的值

lst.reverse() lst.sort() lst.sort(comp) lst.unique() lst.unique(pred) erase掉同一个值的连续拷贝，第一个版本用==，第二个版本用二元谓词。

lst.splice(p, lst2) 将lst2所有元素移动到p之前的位置（lst),或者p之后的位置（flst）  （p，lst2，p2）拼接p2指向的元素

(P,lst2,b,e) 拼接b,e范围内的元素。

## 关联容器

Map,set,multimap,multiset,unordered_map,unordered_set,unordered_multimap,unordered_multiset

pair可以使用== 和 !=

Insert:

c.insert(v); c.emplace(args); c.insert(b, e); c.insert(il); c.insert(p,v),c.emplace(p,args);迭代器作为一个提示，指出从哪里开始搜索新元素应该存储的位置。返回一个迭代器，指向具有给定关键字的元素。 添加单一元素的insert和emplace返回一个pair，告诉我们插入是否成功，first是迭代器，second是bool，如果关键字已在容器中，则insert什么都不做，且返回值中的bool部分为false，如果关键字不存在，元素被插入容器中，且bool值为true。

c.erase(k);返回一个size_type的值，指出删除的元素的数量  c.erase(p);返回一个指向p之后元素的迭代器  c.erase(b,e);返回e

multimap和unordered_multimap不支持下标操作。

c.find(k) 返回一个迭代器，指向第一个关键字为k的元素，若k不在容器中，则返回尾后迭代器。

c.count(k) 返回关键字等于k的元素的数量。对于不允许重复关键字的容器，返回值永远是0或1.

c.lower_bound(k) 返回一个迭代器，指向第一个关键字不小于k的元素

c.upper_bound(k) 返回一个迭代器，指向第一个关键字大于k的元素

c.equal_range(k) 返回一个迭代器pair，表示关键字等于k的元素的范围。若k不存在，pair的两个成员均等于c.end()。

```c++
if (word_count.find("foobar") == word_count.end())
  cout << "not in" << endl;
```

Lower_bound返回的迭代器可能指向一个具有给定关键字的元素，但也可能不指向。如果关键字不在容器中，则lower_bound会返回关键字的第一个安全插入点（不影响容器中元素顺序的插入位置）。

## 无序容器

不能直接定义关键字类型为自定义类类型的无序容器。必须提供hash模版版本。

```c++
// hash function
size_t hasher(const Sales_data &sd) {
  return hash<string>()(sd.isbn());
}
// ==
bool eqOp(const Sales_data &lhs, const Sales_data &rhs) {
  return lhs.isbn() == rhs.isbn();
}

using SD_multiset = unordered_multiset<Sales_data, decltype(hasher)*, decltype(eqOp)*>;
// bucket size, hash_function, ==
SD_multiset bookstore(42, hasher, eqOp);
// == available
unordered_set<Foo, decltype(FooHash)*> fooSet(10, FooHash);
```

# 动态内存

<memory>

## shared_prt

操作：

Shared_prt<string> p1;空智能指针，可以指向类型为T的对象。

p, *p, p->mem, p.get() 返回p中保存的指针。要小心使用，若智能指针释放了其对象，返回的指针所指向的对象也要消失了。

swap(p, q) 交换p和q中的指针。p.swap(q).

Make_shared<T>(args) 返回一个shared_prt,指向一个动态分配的类型为T的对象。使用args初始化此对象。 

Shared_prt<T>p(q) p是shared_prt q的拷贝；此操作会递增q中的计数器。q中的指针必须能转换为T*。

p = q p和q都是shared_prt,所保存的指针必须能相互转换。此操作会递减p的引用计数，递增q的引用计数；若p的引用计数变为0，则将其管理的原内存释放。

p.unique() 若p.use_count()为1，返回true；否则返回false。 p.use_count() 返回与p共享对象的智能指针数量；可能很慢，主要用于调试。

**接受指针参数的智能指针构造函数是explicit的，不能将一个内置指针隐式转换为一个智能指针，必须使用直接初始化形式。**

Shared_prt<int> p2(new int(1024));

定义和改变shared_prt的其他方法：

Shared_prt<T> p(q) p管理内置指针q所指向的对象；q必须指向new分配的内存，且能够转换为T*类型

Shared_prt<T> p(u) p从unique_prt u接管对象所有权；u置空

Shared_prt<T> p(q, d) p接管了内置指针q所指向的对象的所有权。q必须能转换为T*类型。p将使用可调用对象d来替代delete

Shared_prt<T> p(p2, d) p是shared_prt p2的拷贝，唯一的区别是p将用可调用对象d来替代delete

p.reset() p.reset(q) p.reset(q, d) 若p是唯一指向其对象的shared_prt, reset会释放此对象。若传递了可选的参数内置指针q，会令p指向q，否则将p置为空。若还传递了参数d，将会调用d而不是delete来释放q

作为返回类型：

Shared_prt<Foo> function() {

​	return make_shared<Foo>(arg);

}

shared_prt<Foo> function() {

​	shared_prt<Foo> p = Foo(arg);

​	return p;

}

Shared_prt<Foo> function() {

​	return shared_prt<int>(new int(p));

}

智能指针注意事项：

1. 不使用相同的内置指针值初始化（或reset）多个智能指针。
2. 不delete get()返回的指针。
3. 不使用get()初始化或reset另一个智能指针。
4. 如果你使用get（）返回的指针，记住当最后一个对应的智能指针销毁后，你的指针就变为无效了。
5. 如果你使用智能指针管理的资源不是new分配的内存，记住传递给它一个删除器。

Deleter:

Shared_prt<objT, delT> p(new objT, fcn);   delT: decltype(fcn) *

## unique_prt

Unique_prt拥有它所指向的对象，某个时刻只能有一个unique_prt指向一个给定对象。

操作：

unique_prt<string> p1;空智能指针，可以指向类型为T的对象。

p, *p, p->mem, p.get() 返回p中保存的指针。要小心使用，若智能指针释放了其对象，返回的指针所指向的对象也要消失了。

swap(p, q) 交换p和q中的指针。p.swap(q).

与shared_prt不同，没有类似make_shared的标准库函数返回一个unique_prt。定义一个unique_prt时，需要将其绑定到一个new返回的指针上。

Unique_prt<int> p2(new int(42));

由于一个unique_prt拥有它指向的对象，因此unique_prt不支持普通的拷贝或赋值操作。

Unique_prt<T, D> u2; unique_prt<T, D> u(d); u = nullprt; u.release(); u放弃对指针的控制权，返回指针，并将u置为空

u.reset()； 释放u指向的对象 u.reset(q); u.reset(nullprt); 如果提供了内置指针q；令u指向这个对象；否则将u置为空

eg：unique_prt<string> p2(p1.release()); p2.reset(p3.release());

作为返回类型（可拷贝和赋值）：

Unique_prt<int> clone(int p) {

​	return unique_prt<int> (new int(p));

}

Unique_prt<int> clone(int p) {

​	unique_prt<int> ret(new int(p));

​	// ...

​	return ret;

}

## weak_prt

Weak_prt是一种不控制所指向对象生存期的智能指针，它指向由一个shared_prt管理的对象。

操作：

Weak_prt<T> w; weak_prt<T> w(sp); 与shared_prt sp指向相同对象的weak_prt。T必须能转换为sp指向的类型。

w = p; p可以是一个shared_prt或一个weak_prt。赋值后w与p共享对象。

w.reset(); 将w置空。 w.use_count();与w共享对象的shared_prt的数量。

w.expired()；若w.use_count()为0，返回true，否则返回false。

w.lock(); 如果expired为true，返回一个空shared_prt;否则返回一个指向w的对象的shared_prt。

## new/delete

int *pi1 = new int; 默认初始化；\*pi1的值为定义

int *pi2 = new int(); 值初始化为0；\*pi2为0

内置类型的值初始化有良好的初始值，而默认初始化的对象的值则是未定义的。

自定义类都使用默认构造函数。

动态分配const对象： const int *pci = new const int(1024);

**delete之后将指针值重置为nullprt，仅提供有限保护，使用智能指针。**

new T[]分配的内存为“动态数组”，得到的是一个数组元素类型的指针，并不是内置数组。不能使用begin(), end(), 范围for。 

动态分配一个空数组是合法的，但不能解引用。

delete [] pa;

智能指针和动态数组：

Unique_prt<int []> up(new int[10]); up.release(); // 自动调用delete []销毁其指针

指向数组的unique_prt不支持成员访问运算符，其他不变。

Unique_prt<T[]> u; unique_prt<T[]> u(p); u[i];

Shared_prt不直接支持管理动态数组。如果希望使用shared_prt管理一个动态数组，必须提供自己定义的删除器：

Eg: shared_prt<int> sp(new int[10], [\](int \*p) { delete [] p; });

访问数组元素，只能get（）取出内置指针后进行。

## allocator

allocator分配内存，会根据给定的对象类型来确定恰当的内存大小和对齐位置。

操作：

allocator<T> a 定义了一个名为a的allocator对象，它可以为类型为T的对象分配内存。

a.allocate(n) 分配一段原始的、未构造的内存，保存n个类型为T的对象。

a.deallocate(p, n) 释放从T*指针p中地址开始的内存，这块内存保存了n个类型为T的对象；p必须是一个先前由allocate返回的指针，且n必须是p创建时所要求的大小。在调用deallocate之前，用户必须对每个在这块内存中创建的对象调用destroy。

a.construct(p, args) p必须是一个类型为T*的指针，指向一块原始内存；arg被传递给类型为T的构造函数，用来在p指向的内存中构造一个对象。auto q = p; alloc.construct(q++, arg);

a.destroy(p) p为T*类型的指针，此操作对p指向的对象执行析构函数。 While (q != p) alloc.destroy(--q);

拷贝和填充未初始化内存的操作：

Uninitialized_copy(b, e, b2) 迭代器b，e范围内的元素拷贝到b2，b2指向的内存必须够大。

Uninitialized_copy_n(b, n, b2) 从迭代器b开始拷贝n个元素到b2。

Uninitialized_fill(b, e, t) 在迭代器b和e指定的原始内存范围中创建对象，对象的值均为t的拷贝。

Uninitialized_fill_n(b, n, t)

返回值均为最后一个构造元素之后的位置。

# 拷贝控制

拷贝控制操作（copy control）:

拷贝构造函数，拷贝赋值运算符，移动构造函数，移动赋值运算符，析构函数。

如果一个类有数据成员不能默认构造、拷贝、复制或销毁，则对应的成员函数将被定义为删除的。

## copy constructor

形式： Foo(const Foo&);

拷贝构造函数在几种情况下都会被隐式的使用，所以通常不应是explicit。

合成拷贝构造函数(synthesized copy constructor)：

定义了其他构造函数，编译器也会合成一个拷贝构造函数。

类类型，使用其拷贝构造函数来拷贝；内置类型直接拷贝；逐元素拷贝数组类型的成员。

拷贝构造函数不仅在我们用等号=定义变量时调用，在下列情况下也会调用：

(1) 将一个对象作为实参传递给一个非引用类型的形参；（拷贝构造函数形参必须是引用类型，不然调用无法成功）

(2) 从一个返回类型为非引用类型的函数返回一个对象；

(3) 用花括号列表初始化一个数组中的元素或一个聚合类中的成员；

(4) 标准库容器初始化，或者调用insert或push成员时，容器会对其元素进行拷贝初始化，emplace直接初始化。

编译器可以绕过拷贝构造函数进行直接初始化，但拷贝/移动构造函数必须是存在且可访问的。

在类内使用=default，是隐式内联的。若不想内联，在类外=default。

## copy-assignment operator

Foo& operator=(const Foo&);

**重载运算符本质上是函数，如果一个运算符是一个成员函数，其左侧运算对象就绑定到隐式的this参数。**

标准库通常要求保存在容器中的类型要具有赋值运算符，且其返回值是左侧运算对象的引用。

赋值运算符有两点需要注意：

如果将一个对象赋予它自身，赋值运算符必须能正确工作。

大多数赋值运算符组合了析构函数和拷贝构造函数的工作，这种情况下，公共的工作应该放在private的工具函数中完成。

method：

（1）先将右侧运算对象拷贝到一个局部临时对象中。销毁左侧。将临时对象拷贝到左侧运算对象的成员中。

（2）先递增右侧的引用计数，再递减左侧的引用计数。

（3）copy and swap。异常安全且能处理自赋值。

```c++
Foo& Foo::operator=(Foo rhs) { // rhs是按值传递的
	swap(*this, rhs);
  return *this;
}
```

（4）this != &rhs

## destructor

在一个构造函数中，成员的初始化是在函数体执行之前完成的，且按照它们在类中出现的顺序进行初始化。在一个析构函数中，首先执行函数体，然后销毁成员。成员按初始化顺序的逆序销毁。

容器（标准库容器，数组）被销毁时，其元素被销毁。

当指向一个对象的引用或指针离开作用域时，析构函数不会执行。

## 三/五法则

如果一个类需要自定义析构函数，几乎可以肯定它也需要自定义拷贝赋值运算符和拷贝构造函数。

如果一个类需要一个拷贝构造函数，几乎可以肯定它需要一个拷贝赋值运算符，反之亦然。（eg：类的每个对象有一个独特的序号，在拷贝时递增。）

如果一个类定义了任何一个拷贝操作，它通常拥有一个资源，一般来说，拷贝一个资源会导致一些额外开销，在这种拷贝非必要的情况下，定义移动构造函数和移动赋值运算符的类可以避免此开销。

## 拷贝控制和资源管理

可以定义拷贝操作，使类的行为看起来像一个值（string）或者像一个指针（shared_prt）。

IO类型和unique_prt不允许拷贝或赋值，因此它们的行为既不像值也不像指针。

## swap

标准库swap：一次拷贝，两次赋值。

自定义swap：

```c++
class Foo {
  friend void swap(Foo&, Foo&);
};
inline void swap(...) {...}
```

由于swap的存在就是为了优化代码，将其声明为inline函数。

与copy control不同，swap并不是必要的，但对于分配了资源的类，定义swap可能是一种很重要的优化手段。

**swap函数中调用的不是std::swap，内置类型会调用std::swap，如果一个类的成员由自己类型特定的swap，不应调用std::swap。**

```c++
void swap(Foo& lhs, Foo& rhs) {
  using std::swap;
  swap(lhs.h, rhs.h); 	// 特定版本的swap匹配程度会优于std版本，如果不存在类型特定的版本，则会使用std中的版本。
}
```

## 右值引用&&

右值引用只能绑定到临时对象（接管资源）：

1.所引用的对象将要被销毁。

2.该对象没有其他用户。

<utility> std::move():显式的将一个左值转换为对应的右值引用类型。不使用using声明，直接调用std::move，而非move。

**右值引用后就意味着承诺：除了对原对象赋值或销毁之外，不再使用它，此时对象处于可以安全析构的状态（值未知）。**

## move constructor

第一个参数是该类的一个右值引用，与拷贝构造函数一样，任何额外的参数都必须有默认实参。

Foo::Foo(Foo &&s) noexcept 

​	: elements(s.element), cap(s.cap) {}

与拷贝构造函数不同，移动构造函数不分配任何新内存，而是接管资源，因此通常不会抛出异常。

不抛出异常的话，声明和定义时都必须指定noexcept。

如果自定义的类没有标注noexcept，那么标准库可能会使用拷贝构造函数而非移动构造函数。因为标准库容器要能对异常发生时自身的行为提供保障。

与拷贝操作不同，编译器根本不会为某些类合成移动操作，如果一个类定义了自己的拷贝构造函数、拷贝赋值运算符或析构函数，编译器就不会为它合成移动构造函数和移动赋值运算符了。类会使用对应的拷贝操作来代替移动操作。

只有当一个类没有定义任何自己版本的拷贝控制成员，且类的每个非static数据成员都可以移动时，编译器才会为它合成移动构造函数或移动赋值运算符。

定义一个移动构造函数或者移动赋值运算符的类必须也定义自己的拷贝操作，否则，这些成员默认地被定义为删除的。

移动右值，拷贝左值，但如果没有移动构造函数，右值也被拷贝。

移动迭代器：make_move_iterator(iter) 传递给算法uninitialized_copy...construct将使用移动构造函数来构造。

成员函数也可以通过移动受益，形参const T& 和 T&&。

this指针：指出this指针左值\右值属性的方式与const相同。引用限定符。 void memfunc() const &/&&;必须同时出现在声明和定义中。

如果定义两个或两个以上具有相同名字和相同参数列表的成员函数，就必须对所有函数都加上引用限定符，或者都不加。

# 重载运算与类型转换

重载的运算符是具有特殊名字的函数，重载运算符函数的参数数量与该运算符作用的运算对象数量一样多。一元运算符有一个参数，二元运算符有两个。对于二元运算符来说，左侧运算对象传递给第一个参数，而右侧运算对象传递给第二个参数。除了重载的函数调用运算符operator()之外，其他重载运算符不能含有默认实参。如果一个运算符函数是成员函数，则它的第一个运算对象绑定到隐式的this指针上，因此，成员运算符函数的参数数量比运算符的运算对象总数少一个。

对于一个运算符函数来说，它或者是类的成员，或者至少含有一个类类型的参数。**内置类型的运算符不可重载**。

只能重载已有的运算符。对于一个重载的运算符来说，其优先级和结合律与对应的内置运算符保持一致。

**不能被重载的运算符：    ：：    .*     .     ? : **

**不应该被重载的运算符：会丢失求值顺序规则和/或短路求值属性 ，  &&   ||   在类类型中有特殊含义 ，  &**

重载运算符的调用方式：

Data1 + data2;

operator+(data1, data2);

Data1.operator+=(data2); memfunc

是否作为成员函数：

赋值（=），下标（[ ]），调用（（ ））和成员访问箭头（->）运算符必须是成员。

复合赋值运算符一般来说应该是成员，但并非必须，这一点与赋值运算符略有不同。

改变对象状态的运算符或者与给定类型密切相关的运算符，如递增，递减和解引用运算符，通常应该是成员。

具有对称性的运算符可能转换任意一端的运算对象，例如算术、相等性、关系和位运算符等，因此它们通常应该是普通的非成员函数。

string s = "world";

string t = s + "!";	// 正确：可以把一个const char*加到一个string对象中，非成员函数，每个实参可以转换为形参，唯一要求是至少有一个运算对象是类类型

String u = "hi" + s;	// 如果+是string的成员函数，则发生错误，因为const char*没有这个成员函数

## <<

```c++
ostream& operator<<(ostream& os, const Sales_data& item) {
  os << ... ;
  return os;
}
```

输出运算符尽量减少格式化操作，不应该打印换行符（用户灵活性差）。

非成员函数，一般声明为友元。

## >>

```c++
istream& operator>>(istream& is, Sales_data& item) {
  double price;
  is >> ...;
  if (is) // 检查是否输入成功
    ...
  else
    item = Sales_data(); // 输入失败：对象被赋予默认的状态
  return is;
}
```

输入运算符必须处理输入可能失败的情况，而输出运算符不需要。当读取操作发生错误时，输入运算符应该负责从错误中恢复。

## +

```c++
Sales_data operator+(const Sales_data& lhs, const Sales_data& rhs) {
  Sales_data sum = lhs;
  sum += rhs;
  return sum;
}
```

如果类同时定义了算术运算符和相关的复合赋值运算符，则通常情况下应该使用复合赋值来实现算术运算符。

## ==

```c++
bool operator==(const Foo& lhs, const Foo& rhs) {
  return lhs.x == lhs.x && rhs.y == rhs.y;
}
bool operator!=(const Foo& lhs, const Foo& rhs) {
  return !(lhs == rhs);
}
```

如果某个类在逻辑上具有相等性的含义，则该类应该定义operator==，这样做可以使得用户更容易使用标准库算法来处理这个类，!=使用==定义，通常需要==也需要!=，反之亦然。

## <

如果存在唯一一种逻辑可靠的<关系，则应该考虑为这个类定义<运算符。如果类同时还包含==，则当且仅当<的定义和==产生的结果一致时才定义<运算符。如果类有了<，则也应该定义其他关系运算符。

关联容器和一些算法要用到。

## [ ]

下标运算符必须是成员函数。

为了与下标运算符的原始定义兼容，下标运算符通常以所访问元素的引用作为返回值，这样做的好处是下标可以出现在赋值运算符的任意一端。进一步，我们最好同时定义下标运算符的常量版本和非常量版本，当作为一个常量对象时，下标运算符返回常量引用以确保我们不会给返回的对象赋值。

```c++
string& operator[](size_t n) { return elements[n]; }
const string& operator[](size_t n) const { return elements[n]; }
```

## ++

定义递增和递减运算符的类应该同时定义前置版本和后置版本。这些运算符通常应该被定义成类的成员。

为了与内置版本保持一致，前置运算符应该返回对象的引用，后置运算符应该返回对象的原值。

使用一个int形参区分，这个形参并不使用，无须为其命名。

```c++
Foo& Foo::operator++() {
  check(...);
  ++cur;
  return *this;
}
Foo Foo::operator++(int) {
  Foo ret = *this;
  ++*this;
  return ret;
}
```

显式调用:p.operator++(); p.operator++(0);

## ->   *

箭头运算符必须是类的成员。解引用运算符通常也是类的成员，尽管并非必须如此。

重载的箭头运算符必须返回类的指针或者自定义了箭头运算符的某个类的对象。

## 函数对象

```c++
struct absInt {
  int operator()(int val) const {
    return val < 0 ? -val : val;
  }
}
absInt absObj;
int ui = absObj(-42);
```

必须是成员函数，一个类可以定义多个不同版本的调用运算符，互相之间应该在参数数量或类型上有所区别。

函数对象常常作为泛型算法的实参。

lambda也是函数对象。

```c++
[sz](const string& a) { return a.size() >= sz; }
类似于
class SizeComp {
  SizeComp(size_t n) : sz(n) {}
  bool operator() (const string& s) const {
    return s.size() >= sz;
  }
  private:
  	size_t sz;
}
SizeComp(sz)  类似于 greater<int>()
```

标准库规定其Functor（greater，less...）对指针同样适用，lambda则无。

## 可调用对象与function

C++中的几种可调用对象：函数，函数指针，lambda表达式，bind创建的对象，仿函数。

不同类型的可调用对象可能共享同一种调用形式。如int(int, int)。

定义一个函数表用于存储指向这些可调用对象的“指针”。当程序需要执行某个特定的操作时，从表中查找该调用的函数。

### function

<functional>

function可以把具有相同调用形式的可调用对象存储在一个function<int (int, int)>类型中。

函数表可以表示为：

map<string, function<int(int, int)>> binops;

不能直接将重载函数的名字存入function类型的对象中，即使只有一个调用形式符合function。

解决上述二义性问题的一条途径是存储函数指针，而非函数名字。

int (*fp) (int, int) = add;

## 重载类型转换运算符

operator type() const;

eg: operator int() const { return val; }

**编译器只会执行一次用户定义的类型（构造，重载）转换，但是隐式重载类型转换可以置于一个内置类型转换之前或之后，并与其一起使用。**

避免过度使用类型转换函数，在实践中，类很少提供类型转换运算符。除了bool。

显式的类型转换运算符：

Explicit operator type() const {...}

编译器不会自动执行，例外是，如果表达式被用作条件，则编译器会将显式的类型转换自动应用于它。向bool的类型转换通常用在条件部分，因此operator bool一般定义成explicit的。

**避免二义性**:

显式调用可以避免二义性。

1.2.通常情况下，不要为类定义相同的类型转换（A转B，B转A，a = b时二义性），也不要在类中定义两个及两个以上转换源或转换目标是算术类型的转换（转int， double，目标类型为long double）。

当我们使用两个用户定义的类型转换时，如果转换函数之前或之后存在标准类型转换，则标准类型转换将决定最佳匹配到底是哪个。

尽量避免使用隐式构造函数。

3.如果在调用重载函数时需要使用构造函数或者强制类型转换来改变实参的类型，则这通常意味着程序的设计存在不足。只有当重载函数能通过同一个类型的转换函数得到匹配时，才会考虑其中出现的标准类型转换。即使其中一个调用需要额外的标准类型转换而另一个能精确匹配，编译器也会将调用标志为错误。

4.如果对同一个类既提供了转换目标是算术类型的类型转换，也提供了重载的运算符，则将会遇到重载运算符与内置运算符的二义性问题。

a sym b could be:

a.operatorsym(b);

Operatorsym(a,b);

# 面向对象程序设计

面向对象程序设计基于三个基本概念：数据抽象（类的接口和实现分离）、继承和动态绑定。

**在c++语言中，当我们使用基类的引用\指针调用一个虚函数时将发生动态绑定。**基类的指针或引用的静态类型可能与其动态类型不一致。

使用普通类型（非引用和指针）调用虚函数时，在编译时就会将调用的版本确定下来。

在容器中放置（智能）指针而非对象。

OOP的核心思想是多态性。把具有继承关系的多个类型称为多态类型，可以使用这些类型的“多种形式”而无须在意它们的差异。

基类通常都应该定义一个虚析构函数，即使该函数不执行任何实际操作也是如此。

成员函数如果没有被声明为虚函数，则其解析过程发生在编译时而非运行时。

派生类中虚函数的返回类型必须与基类函数匹配，例外是，返回类本身的指针或引用，但要求从D到B的类型转换是可访问的。

**虚函数可以拥有默认实参。如果某次函数调用使用了默认实参，则该实参值由本次调用的静态类型决定。如果通过B引用指针调用函数，则使用B中的默认实参，即使动态绑定到D的函数版本也是如此。如果虚函数要使用默认实参，则B类和D类中默认实参最好一致。**

回避虚函数机制：作用域运算符指定运行函数版本，将在编译时完成解析。baseP->Quote::func();



每个类控制它自己的成员初始化过程。首先初始化基类的部分，然后按照声明的顺序依次初始化派生类的成员。派生类构造函数只初始化它的直接基类，层层委托构造。

派生类析构函数只负责销毁由派生类自己分配的资源，对象销毁的顺序正好与其创建的顺序相反：派生类析构函数首先执行，然后是基类的析构函数，以此类推，沿着继承体系的反方向直至最后。

如果构造函数或析构函数调用了某个虚函数，则应该执行与构造函数或析构函数所属类型相对应的虚函数版本。

静态成员：基类定义了一个静态成员，则在整个继承体系中只存在该成员的唯一定义。private的静态成员，继承类无法访问。

基类必须已经定义而非仅仅声明.

static type, dynamic type:静态类型编译时已知；动态类型则是变量或表达式表示在内存中的对象的类型。运行时才可知。如果表达式既不是引用也不是指针，则它的动态类型永远与静态类型一致。

不能继承友元，每个类负责控制各自成员的访问权限。

在类内使用using声明语句，可以将该类的直接或间接基类中的任何**可访问成员**标记出来。using声明语句中名字的访问权限由该using声明语句之前的访问说明符来决定。

public:

​	using Base::size; // public

protected:

​	using Base::n; // pretected

虚函数如果重载，则派生类可以覆盖它的0个或多个实例。如果派生类希望所有的重载版本对于它来说都是可见的，那么它就需要覆盖所有的版本，或者一个都不覆盖。只想覆盖其中几个的话，可以为重载的成员提供一条using声明语句，using声明语句指定一个名字而不指定形参列表。using规则同上。

默认的继承保护级别，class private，struct public。

## 基类，派生类转换

**derived-to-base类型转换：隐式地执行，意味着可以把派生类对象或者派生类对象的引用用在需要基类引用的地方，也可以把派生类的指针用在需要基类指针的地方。在派生类对象中含有与其基类相应的组成部分，这一事实是继承的关键所在。**受访问控制影响。

智能指针类也支持派生类向基类的类型转换，这意味着可以将一个派生类对象的指针存储在一个基类的智能指针内。

不存在从基类向派生类的隐式类型转换，编译器在编译时无法确定向下转换是否安全，因为编译器只能通过检查指针或引用的静态类型来推断该转换是否合法。可以使用dynamic_cast请求一个类型转换，该转换的安全检查将在运行时执行。如果我们已知某个基类向派生类的转换是安全的，则我们可以使用static_cast来强制覆盖掉编译器的检查工作。

隐式转换和动态绑定仅发生在derived-to-base，在对象之间不存在类型转换，如果使用派生类构造或者赋值，会使用基类的构造和assignment operator，意味着会忽略掉派生类独有的成员，这部分被切掉（sliced down）。

可访问性的影响：

只有当D公有的继承B时，用户代码才能使用派生类向基类的转换，如果D继承B的方式是受保护的或者私有的，则用户代码不能使用该转换。

不论D以什么方式继承B，D的成员函数和友元都能使用派生类向基类的转换：派生类向其直接基类的类型转换对于派生类的成员和友元来说永远是可访问的。

如果D继承B的方式是公有的或者受保护的，则**D的派生类**的成员和友元可以使用D向B的转换：反之，如果D继承B的方式是私有的，则不能使用。

## final override

final防止继承发生。

class NoDerived final { ... };

class Last final : public Base { ... };

把函数指定为final，则之后任何尝试覆盖该函数的操作都将引发错误。

void f1(int) const final;

override关键词说明D类中的虚函数，使得程序员的意图更加清楚，同时使编译器可以为我们发现一些错误（如与B类中函数形参不同）。

final 和 override说明符出现在形参列表\const\\&\\&&\尾置返回类型之后。

## 抽象基类

纯虚函数表示一个通用概念。

double func() = 0;

无须定义，也可以定义，但要在类外。

含有纯虚函数的类是抽象基类，抽象基类负责定义接口，而后续的其他类可以覆盖接口。

不能直接创建抽象基类的对象，派生类如果没有覆盖所有纯虚函数也不能创建，派生类也是抽象的。

## 访问控制与继承

类内protected：

和私有成员类似，受保护的成员对于类的用户来说是不可访问的。

和公有成员类似，受保护的成员对于派生类的成员和友元来说是可访问的。

**派生类的成员或友元只能通过派生类对象来访问基类的受保护成员，派生类对于一个基类对象中的受保护成员没有任何访问特权。**

```c++
class Base {
 protected:
  int prot_mem;
};
class Sneaky : public Base {
  friend void clobber(Sneaky&);
  friend void clobber(Base&);
}
void clobber(Sneaky& s) { s.prot_mem... } // right
void clobber(Base& b) { b.prot_mem... } // wrong
```

public,protected,private继承：

将基类视为派生类中的一个public，private，protected对象，对于派生类的成员能否访问直接基类的成员没什么影响，但对派生类的用户有影响，用户包括派生类的派生类。

公有继承的特点是基类的公有成员和保护成员作为派生类的成员时，它们都保持原有的状态，而基类的私有成员仍然是私有的，不能被这个派生类的子类所访问。

私有继承的特点是基类的公有成员和保护成员都作为派生类的私有成员，并且不能被这个派生类的子类所访问。　

保护继承的特点是基类的所有公有成员和保护成员都成为派生类的保护成员，并且只能被它的派生类成员函数或友元访问，基类的私有成员仍然是私有的。

## 继承中的类作用域

当存在继承关系时，派生类的作用域嵌套在其基类的作用域之内。如果一个名字在派生类的作用域内无法正确解析，则编译器将继续在外层的基类作用域中寻找该名字的定义。

查找从静态类型所属类向上查找。

派生类的成员将隐藏同名的基类成员，除了覆盖继承而来的虚函数以外，派生类最好不要重用其他定义在基类中的名字。

**名字查找与继承**：

1.首先确定p（或obj）的静态类型。

2.在静态类型对应的类中查找mem。如果找不到，则依次在直接基类中不断查找直至到达继承链的顶端。如果仍找不到，编译器将报错。

3.一旦找到了mem，就进行常规的类型检查以确定对于当前找到的mem，本次调用是否合法。即使派生类成员和基类成员的形参列表不一致，基类成员也仍然会被隐藏掉。

4.假如调用合法，则编译器将根据调用的是否是虚函数而产生不同的代码：

--如果mem是**虚函数**且是通过**引用和指针进行的调用**，则编译器产生的代码将在运行时确定到底运行该虚函数的哪个版本，依据是对象的**动态类型**。

--反之，如果mem不是虚函数或者不是通过引用和指针调用的，则编译器将产生一个常规函数调用。

## constructor  copy control

如果基类的析构函数不是虚函数，则delete一个指向派生类对象的基类指针将产生未定义的行为。

虚析构函数将阻止合成移动操作。当我们确实需要执行移动操作时应该首先在基类中进行定义。一旦定义了自己的移动操作，那么它必须同时显式的定义拷贝操作。

当派生类定义了拷贝或移动操作时，该操作负责拷贝或移动包括基类部分成员在内的整个对象。

当为派生类定义拷贝或移动构造函数时，通常使用对应的基类构造函数初始化对象的基类部分。

```c++
D(const D& d): Base(d),... { ... }
D(D&& d): Base(std::move(d)),... { ... }
D& D::operator=(const D& rhs) {
  Base::operator=(rhs); // 为基类部分赋值
  // 按照过去的方式为派生类的成员赋值
  // 酌情处理自赋值及释放已有资源等情况
  return *this;
}
```

## 使用直接基类的构造函数

派生类能够重用其直接基类的构造函数。

using B::B;

using作用于构造函数，会令编译器产生代码，对于基类的每个构造函数，编译器都在派生类中生成一个形参列表完全相同的构造函数。

派生类中自己定义的构造函数与基类有相同的形参列表时，不会生成这个构造函数。生成的构造函数不会影响派生类中合成默认构造函数的生成。

derived(parms) : base(args) {} 如果派生类含有自己的数据成员，则这些成员将被默认初始化。

using使用规则与前述不同，构造函数的using声明不会改变该构造函数的访问级别，访问级别始终与基类中的构造函数相同。不能指定explicit或constexpr，与基类相同。

实参并不会被继承。派生类将获得多个继承的构造函数，其中每个构造函数分别省略掉一个含有默认实参的形参。

## 模板与泛型编程

在泛型编程中，在编译时就能获知类型。模板是C++中泛型编程的基础。一个模板就是一个创建类或函数的蓝图。

## 函数模板

非类型模板参数，当一个模板被实例化时，非类型参数被一个用户提供的或编译器推断出的值所替代。非类型模板参数的模板实参必须是常量表达式。

```c++
template<unsigned N, unsigned M>
int compare(const char (&p1)[N], const char (&p2)[M]) {
  return strcmp(p1, p2);
}
compare("hi", "mom");
编译器会使用字面常量的大小来替代N和M，N = 3， M = 4.
```

模板程序应该尽量减少对实参类型的要求。

## 模板编译

当编译器遇到一个模板定义时，它并不生成代码。实例化出模板的一个特定版本时，编译器才会生成代码。

函数模板和类模板成员函数的定义通常放在头文件中。

大多数编译错误在实例化期间报告。

保证传递给模板的实参支持模板所要求的操作，以及这些操作在模板中能正确工作，是调用者的责任。

## 类模板

类模板是用来生成类的蓝图。与函数模板的不同之处是，编译器不能推断其参数类型。必须在模板名后的尖括号中提供额外信息。vector<int>

一个类模板的每个实例都形成一个**独立的类**。类模板的名字不是一个类型名，实例化后才是。

类模板成员函数定义：

template<typename T>

void Foo<T>::Foo(...) { ... }

默认情况下，对于一个实例化了的类模板，其成员只有在使用时才被实例化。即使某种类型不能完全符合模板操作的要求，仍然能用该类型实例化类。

在一个类模板的作用域内，我们可以直接使用模板名而不必指定模板实参。

```c++
template <typename T>
// 类外
BlobPrt<T> BlobPrt<T>::operator++(int) {
  BlobPrt ret = *this; // 类内
  ...
}
```

template <typename T> T calc(...); // 声明

一个特定文件所需要的所有模板的声明通常一起放置在文件开始位置，出现于任何使用这些模板的代码之前。

### 友元

```c++
template <typename T> class Pal;
class C {	// 非模板类
  friend class Pal<C>;	// 用类C实例化的Pal是一个友元
  template <typename T> friend class Pal2;	// Pal2的所有实例都是C的友元，无须前置声明
};
template <typename T> class C2 {	// 类模板
  friend class Pal<T>;	// 授予相同类型实例化的Pal访问权限，常用
  template <typename X> friend class Pal2;	// Pal2的每个实例都是C2每个实例的友元，无须前置声明
  friend class C3;	// 非模板类C3是C2每个实例的友元
  friend T;	// 可以将模板参数声明为友元
};
```

### 别名

typedef Foo<string> StrFoo;	// 模板不是类型，实例化后才可typedef

template<typename T> using twin = pair<T, T>;	// 允许为类模板定义一个别名

twin<double> ...;

### static

每个类的实例都有其自己的static成员实例。所有Foo<X>类型的成员共享一个static成员实例。

### ::

在模板类中，无法确定T::mem是一个对象还是一个类，当表示它是一个类的时候前面需要加typename（class不行）。

typename T::value_type ...;

### 默认模板实参

template <typename T, typename F = less<T>>

默认了一个比较操作，也可以提供自己定义的比较操作，与函数默认实参一样，对于一个模板参数，只有当它右侧的所有参数都有默认实参时，它才可以有默认实参。

template <class T = int> class Numbers { ... };

Numbers<> n;	// 默认用int，尖括号不能丢

### 成员模板

一个类可以包含本身是模板的成员函数，称为成员模板，不能是虚函数。

eg：适用于任何类型的deleter。

```c++
class DebugDelete {
public:
  DebugDelete(std::ostream &s = std::cerr) : os(s) {}
  template <typename T> void operator()(T* p) const {
    os << "delete unique_prt" << std::endl; delete p;
  }
private:
  std::ostream &os;
};

double* p = new double;
DebugDelete d;
d(p);
int* ip = new int;
DebugDelete()(ip);
unique_prt<int, DebugDelete> p(new int, DebugDelete());
unique_prt<string, DebugDelete> p(new string, DebugDelete());
```

## 类模板的成员模板

成员模板是函数模板。

```c++
// 在类外定义时
template <typename T>
template <typename It>
	Blob<T>::Blob(It b, It e): data(std::make_shared<std::vector<T>>(b, e)) {}
```

为了实例化一个类模板的成员模板，必须同时提供类和函数模板的实参。编译器通过传递给成员函数的函数实参来推断它的模板实参。

## 控制实例化

当模板被使用时才会进行实例化，相同实例可能出现在多个对象文件中。

显式实例化。对每个实例化声明，在程序的某个位置必须有其显式的实例化定义。显式实例化会实例化所有成员函数，类型必须能用于模板的所有成员函数。

extern template declaration;

template declaration;

eg：

extern template class Blob<string>;

template class Blob<string>;

## 类型转换与模板类型参数

只有很有限的几种类型转换会自动应用于模板类型参数的实参，编译器通常不是对实参进行类型转换，而是生成一个新的模板实参。

顶层const会被忽略。

底层const 转换：可以将一个非const对象的引用/指针传递给一个const的引用/指针形参。

数组或函数指针转换。如果形参是一个引用，则数组不会转换为指针。

使用相同模板参数类型的函数形参，不能接受两个不同类型的实参，如果希望允许对函数实参进行正常的类型转换，可以将函数模板定义为两个类型参数。

如果函数参数类型不是模板参数，则对实参进行正常的类型转换。

## 函数模板显式实参

在某些情况下，编译器无法推断出模板实参的类型。在另一些情况下，希望允许用户控制模板实例化。当函数返回类型与参数列表中任何类型都不相同时，这两种情况最常出现。

```c++
template <typename T1, typename T2, typename T3>
T1 sum(T2, T3);

auto val3 = sum<long long>(i, lng); // long long sum(int, long);
```

显式模板实参按由左至右的顺序与对应的模板参数匹配，只有尾部的参数的显式模板实参才可以忽略，而且前提是它们可以从函数参数推断出来。

**对于模板参数已经显式指定了的函数实参，允许进行正常类型转换。**

## 尾置返回类型与类型转换

当希望用户确定返回类型时，用显式模板实参表示模板函数的返回类型是很有效的。但在其他情况下，要求显式指定模板实参会给用户增添额外负担，而且不会带来什么好处。

```c++
// 使用尾置返回类型
template <typename It>
auto fcn(It beg, It end) -> decltype(*beg) {
  return *beg;
}
```

## 进行类型转换的标准库模板类

类型转换模板。<type_traits>

```c++
template <typename It> // typename告知编译器type是一个类型
auto fcn2(It beg, It end) -> typename remove_reference<decltype(*beg)>::type {
  return *beg;
}
```

Mod<T>:   Mod.   T.   Mod<T>::type.

remove_reference   X&/X&&  X   其他 T

add_const  X&/const X/函数  T   其他  const T

add_lvalue_reference X&   T    X&&   X&   其他    T&

add_rvalue_reference X&/X&&    T   其他    T&&

remove_pointer  X*    X       其他     T

add_pointer    X&/X&&    X*    其他    T*

make_signed    unsigned X   X   其他   T

make_unsigned    带符号类型   unsigned X    其他     T

remove_extent    X[n]    X   其他    T

remove_all_extents    X[n1] [n2]...     X    其他   T

## 引用折叠和右值引用参数

两个例外规则：

1.将一个左值传递给函数的右值引用参数，且此右值引用指向模板类型参数(如T&&)时，编译器推断模板类型参数为实参的左值引用类型。

2.引用折叠:X& &/X& &&/X&& & 折叠成类型X&   类型X&& &&折叠成X&&。引用折叠只能应用于间接创建的引用的引用，如类型别名或模板参数。

如果一个函数参数是指向模板参数类型的右值引用（如，T&&），则可以传递给它任意类型的实参。如果将一个左值传递给这样的参数，则函数参数被实例化为一个普通的左值引用（T&）。

在实际中，右值引用通常用于两种情况：

1.模板重载。

```c++
template <typename T> void f(T&&);
template <typename T> void f(const T&);
```

2.实参转发。

```c++
void f(int v1, int& v2) { ... }
// Q1左值引用属性丢失
template <typename F, typename T1, typename T2>
void flip1(F f, T1 t1, T2 t2) {
  f(t2, t1);
}
// 引用折叠，如果一个函数参数是指向模板类型参数的右值引用，它对应的实参的const属性和左值/右值属性将得到保持。
template <typename F, typename T1, typename T2>
void flip2(F f, T1&& t1, T2&& t2) {
  f(t2, t1);
}
void g(int&& i, int& j) { ... }
// Q2右值引用丢失
template <typename F, typename T1, typename T2>
void flip2(F f, T1&& t1, T2&& t2) {
  f(t2, t1);
}
filp2(g, i, 42);
// 函数参数是lval，42rval传递给flip2后t2是lval.
// 在调用中使用std::forward保持类型信息
template <typename F, typename T1, typename T2>
void filp3(F f, T1&& t1, T2&& t2) {
  f(std::forward<T2>(t2), std::forward<T1>(t1));
}
// <utility>,必须显式实例化，forward返回该实参类型的右值引用，通过引用折叠，保持左值/右值属性。当用于一个指向模板参数类型的右值引用函数参数（T&&）时，forward会保持实参类型的所有细节。
```

### std::move

```c++
template <typename T>
typename remove_reference<T>::type&& move(T&& t) {
  return static_cast<typename remove_reference<T>::type&&>(t);
}
```

static_cast针对右值引用的一条特许规则：

虽然不能隐式地将一个左值转换为右值引用，但可以用static_cast显式地将一个左值转换为一个右值引用。对于操作右值引用的代码来说，将一个右值引用绑定到一个左值的特性允许它们截断左值。

## 重载与模板

当有多个重载模板对一个调用提供同样好的匹配时，应选择最特化的版本。

对于一个调用，如果一个非函数模板与一个函数模板提供同样好的匹配，则选择非模板版本。

在定义任何函数之前，记得声明所有重载的函数版本，这样就不必担心编译器由于未遇到希望调用的函数而实例化一个并非所需的版本。

## 模板特例化

```c++
// 函数模板特例化
template<>
int compare(const char* const &p1, const char* const& p2) {
  return strcmp(p1, p2);
}
// 定义一个特例化版本时，函数参数类型必须与一个先前声明的模板中对应的类型匹配
```

特例化的本质是实例化一个模板，而非重载它。因此，特例化不影响函数匹配。当一个非模板函数提供与函数模板同样好的匹配时，编译器会选择非模板版本。

普通作用域规则应用于特例化。模板机器特例化版本应该声明在同一个头文件中。所有同名模板的声明应该放在前面，然后是这些模板的特例化版本。

类模板可以部分特例化(偏特化)，函数模板不能部分特例化。

特例化成员函数：

template<>

void Foo<int>::Bar() { ... };

// 用于int的特例化成员函数

## 可变参数模板

模板参数包typename... Args   Args是一个模板参数包 零或多个

函数参数包Args& ... rest rest是一个函数参数包 零或多个

sizeof...(Args) // 类型参数的数目  sizeof...(args) // 函数参数的数目 

```c++
// 编写可变参数函数模板
// 用来终止递归并打印最后一个元素的函数，必须定义在可变参数版本之前
template<typename T>
ostream& print(ostream& os, const T& t) {
  return os << t;
}
template <typename T, typename... Args>
ostream& print(ostream& os, const T& t, const Args&... rest) {
  os << t << ",";
  return print(os, rest...);
}
// 非可变参数版本更特化，最后一次调用选择非可变参数版本。
```

包扩展（expand）：模式（pattern），...触发扩展，扩展中的模式会独立地应用于包中的每个元素。

const Args&... rest

debug_rep(rest)...

std::forward<Args>(args)...

# 高级主题

## tuple <tuple>

ops:

tuple<T1, T2, ..., Tn> t;

tuple<T1, T2, ..., Tn> t(v1, v2, ...,vn);

make_tuple(v1, v2, ...,vn);

t1 == t2, t1 != t2, t1 relop t2   t1和t2中每个成员必须一一对应的可使用其关系运算符。可以将tuple传递给算法，并可以在无序容器中用tuple作key。

get<i>(t)  返回t的第i个数据成员的引用，t是左值，则返回左值引用，t是右值，则返回右值引用。tuple中所有成员都是public的。

tuple_size<tupletype>::value  通过一个实例化的tuple类型来初始化，返回value(size_t),表示给定tuple类型中成员的数量。

tuple_element<i, tupletype>::type  tuple类型中指定成员的类型。

可以将tuple看作一个“快速而随意的数据结构”。

eg:

auto book = get<0>(item); // rval

get<2>(item) *= 0.8; // lval

typedef decltype(item) trans;

size_t sz = tuple_size<trans>::value;

tuple_element<1, trans>::type cnt = get<1>(item);

## bitset <bitset>

位操作。

ops:

bitset<n> b;  b有n位，均为0

bitset<n> b(u); unsigned数初始化b，长度不足高位补0，长度超过截断，signed当作unsigned。

bitset<n> b(s, pos, m, zero, one); string从pos开始m位，zero(char)转0，one(char)转1。zero和one默认为'0','1'。若string中包含其他字符，会异常。 **string中下标最大的字符初始化bitset的低位。**

bitset<n> b(cp, pos, m, zero, one);

b.any() 是否存在置位的二进制位。

b.all() 是否全部置位

b.none()

b.count() 置位的位数

b.size() n

b.test(pos) pos位置是否置位

b.set(pos, v) b.set() 将pos处/所有位置置为v，v默认true

b.reset(pos) b.reset() 复位

b.flip(pos) b.flip() 改变位置pos处的位的状态或改变b中每一位的状态

b[pos]

b.to_ulong() b.to_ullong() b无法全部放入会抛出异常

b.to_string(zero, one)

os << b 打印为字符

is >> b 从is读取字符存入b，当下一个字符不是0或1，或b已满，读取停止

## TODO:regex,rand,IO库再探

## 异常处理exception handling

throw：抛出异常，throw后面的语句将不再被执行，使用方式类似return，在条件语句后面使用，或者作为某个函数最后一条语句。

栈展开：throw出现在一个try语句块内时，检查与该try块关联的catch子句，如果找到了匹配的catch，就用该catch处理，否则继续检查与外层try匹配的catch子句，如果找不到，就调用调用terminate退出程序。一个异常如果没有被捕获，则它将终止当前的程序。

**栈展开过程中对象被自动销毁：**局部对象的析构函数会被自动调用，而释放资源的语句可能被跳过（eg：throw之后的delete）。

析构函数不应该抛出不能被自身处理的异常，一旦在栈展开过程中析构函数抛出了异常，并且析构函数本身没能捕捉异常，则程序被终止。在实际的编程过程中，因为析构函数仅仅是释放资源，所以它不太可能抛出异常，所有标准库类型都能保证它们的析构函数不会引发异常。

异常对象(expection object):编译器使用throw对异常对象进行**拷贝初始化**。抛出一个指向局部对象的指针几乎肯定是一种错误的行为。静态编译时的类型决定了异常对象的类型，基类指针实际指向派生类，派生类会被切掉一部分，只有基类部分被抛出。

catch：参数类型，可以忽略，非引用的话不改变原对象，可以是左值引用，不能是右值引用。静态类型决定catch语句能执行的操作，如果catch的参数是基类类型，则catch无法使用派生类特有的任何成员。

越是专门的catch应该置于整个catch列表的前端。

catch声明的类型允许的类型转换仅有：允许非常量向常量转换，允许派生类向基类转换，数组被转换成指向数组类型的指针，函数被转换成指向该函数类型的指针。

重新抛出：throw；一个重新抛出语句并不指定新的表达式，而是将当前的异常对象沿着调用链向上传递。

捕获所有异常的处理代码：catch(...)，如果与其他几个catch语句一起出现，则应该出现在最后的位置。

try与构造函数：

```c++
template <typename T>
Blob<T>::Blob(...) try : ... { ... } catch(...) { ... }
// try出现在表示构造函数初始值列表的冒号以及函数体之前，catch可以处理成员初始化列表的异常也能处理函数体的异常
```

处理构造函数初始值异常的唯一方法是将构造函数写成函数try语句块。

**noexcept:**跟在函数的参数列表后面，尾置返回类型之前，const/&&/&之后，final/override/=0之前。

编译器并不会在编译时检查noexcept，noexcept函数抛出异常程序会调用terminate。

noexcept(true) // 不抛出异常    noexcept(false) // 可能会抛出异常

noexcept作为运算符：noexcept(e) e做了noexcept说明且无throw语句时true，否则false。

noexcept与指针、虚函数和拷贝控制：

不抛出异常的函数指针只能指向不抛出异常的函数，可能抛出异常的函数可以指向任何函数。

虚函数不抛出异常承诺，派生出来的虚函数也必须不抛出异常承诺，基类允许抛出异常，派生出来的对应函数既可以允许抛出异常，也可以不允许抛出异常。

如果所有成员和基类的所有操作都承诺了不会抛出异常，则合成的成员是noexcept的。否则是noexcept（false）。如果我们定义了一个析构函数但是没有为它提供异常说明，则编译器将合成一个。合成的异常说明将与假设由编译器为类合成析构函数时所得的异常说明一致。

