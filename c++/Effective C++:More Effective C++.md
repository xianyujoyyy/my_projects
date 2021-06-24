# 尽量以const,enum,inline替换#define

对于单纯常量，最好以const对象或enums替代#defines。

对于形似函数的宏（macros），最好改用inline函数替换#defines。

the enum hack: class内 enum{ n = 5 }; // enum hack的行为某方面来说更像#define而不像const（不能获取地址，不会导致非必要的内存分配）

# 尽可能使用const

const Foo operator* { ... } // 防止a * b = c；这种错误

non-const function和const function有重复的代码段，使用static_cast<const...> 转换使用const function，再使用const_cast去const返回non-const形式，令non-const版本调用const版本是安全的，反之则不是。

# 确定对象被使用前已先被初始化

对于大多数类型而言，比起先调用default构造函数然后再调用copy assignment操作符，只调用一次copy构造函数是比较高效的。

成员变量是const或references，它们就一定需要初值，不能被赋值。

不同编译单元/多线程之间的初始化顺序，可以使用Singleton模式解决。

为内置型对象进行手工初始化，因为C++不保证初始化它们。

构造函数最好使用成员初值列，而不要在构造函数本体内使用赋值操作。初值列列出的成员变量，其排列次序应该和它们在class中的声明次序相同。

为免除“跨编译单元之初始化次序”问题，请以local static对象替换non-local static对象。(static ref to object)

# 析构函数

polymorphic base classes应该声明一个virtual析构函数。如果class带有任何virtual函数，它就应该拥有一个virtual析构函数。

Classes的设计目的如果不是作为base classes使用，或不是为了具备多态性，就不该声明virtual析构函数。

C++不喜欢析构函数吐出异常，如果抛出异常就直接abort结束程序。或者log下来吞下异常。或者把可能抛出异常的部分抽出成新的函数，主动调用而不是在析构函数中调用。

不要在构造和析构过程中调用virtual函数，此时virtual函数不是virtual函数。

# operator =

返回ref to this(lvalue)

处理自赋值，证同测试，copy and swap。

# 复制对象时勿忘其每一个成分

复制所有local成员变量，调用所有base classes内的适当的copying函数。

copy constructor和copy assignment constructor有相近的代码，消除重复代码的做法是，建立一个新的成员函数给两者调用。不要尝试用一个copy function实现另一个。

# 资源管理

以对象管理资源：智能指针，容器。

普遍而常见的RAII class copying行为是：抑制copying、施行引用计数法。

new/delete形式要相同 new int [] / delete int []

以独立的语句将资源置于智能指针内，如果不这样，如果有异常被抛出，可能资源泄漏（智能指针来不及接管资源）。

eg：processWidget(shared_prt<Widget>(new Widget), priority()); 如果priority执行于new和智能指针构造之间且发生异常。

使用智能指针的deleter可以防范DLL问题（cross-DLL problem），也可用来自动解除互斥锁。

# 设计与声明

尽量以pass-by-ref-to-const替换pass-by-value。前者通常比较高效，并可以避免切割问题。

内置类型，STL迭代器和函数对象，pass-by-value往往更好。

封装性与“当其内容改变时可能造成的代码破坏量”成反比，public会影响所有用户码，protected会影响所有derived classes。其实只有两种访问权限：private和其他。

protected并不比public更具有封装性。

non-member function封装性比member function高，因为愈多函数可访问它，数据的封装性就愈低。

提供便利的non-member non-friend函数可以和class定义在一个namespace里。

如果需要为某个函数的所有参数（包括this）进行类型转换，那么这个函数必须是个non-member(二元operator)。

# 实现

尽可能延后变量定义式的出现时间，尽量少做转型动作，避免返回handles指向对象内部成分（封装性最多只等于返回其ref的函数的访问级别，对象可能被析构）。

dynamic_cast:在一个认定为D类的对象上执行D类的操作，却只有一个指向B类的pointer或ref时。

## 异常安全

不泄漏任何资源，不允许数据破坏。

基本承诺：如果异常被抛出，程序内的任何事物仍然保持在有效状态下。没有任何对象或数据结构会因此而破坏。

强烈保证：如果异常被抛出，程序状态不改变。往往能够以copy-and-swap来实现。

nothrow：承诺绝不抛出异常，因为它们总是能够完成它们原先承诺的功能。作用于内置类型身上的所有操作都提供nothrow保证。这是异常安全码中一个必不可少的关键基础。

函数能提供的“异常安全保证”通常最高只等于其所调用之各个函数的“异常安全保证”中的最弱者。

## inline

inline只是对编译器的一个申请，不是强制命令。定义在class内是隐式inline。

如果template没有理由要求它所具现的每一个函数inline，就应该避免将这个template声明为inline。

太过复杂的函数，virtual函数，构造析构函数不应该inline。

inline函数无法随着程序库的升级而升级，non-inline函数只需要重新连接，inline需要重新编译。

大多数调试器都无法调试inline。

## 将文件间的编译依存关系降至最低

pimpl idiom(pointer to implementation) 把实现藏在指针后面。

让头文件尽量自我满足，做不到的话，则让它与其他文件内的声明式（而非定义式）相依：（减少头文件依赖）

尽量使用object ref或object pointers完成任务，而不要使用objects。

如果能够，尽量以class声明式替换class定义式。

为声明式和定义式提供不同的头文件，<iosfwd>。

handle classes和interface classes（纯虚函数）解除了接口和实现之间的耦合关系，从而降低文件内的编译依存性。

增减间接性会增加运行期成本。

**程序库头文件应该以“完全且仅有声明式”的形式存在，这种做法不论是否涉及templates都适用。**

# 继承

public inheritance means **is-a** relationship.

B比D表现出更一般化的概念，而D比B表现出更特殊化的概念。“B对象可派上用场的任何地方，D对象一样可以派上用场（slice），因为每一个D对象是一个B对象，反之不成立，B不是D。”适用于base classes身上的每一件事情，一定也适用于derived classes。

composition：应用域的对象之间，has-a关系。实现域内，is-implemented-in-terms-of关系。

private inheritance: 不存在D到B的转换，means implemented-in-terms-of（尽可能复合（private里直接放一个继承的class对象指针），必要时才使用private继承。），当derived class需要访问protected base class member或需要重新定义继承来的virtual函数时，是合理的。可以造成empty base最优化。

class HoldsAnInt:private Empty { private: int x; } sizeof(HoldAnInt) == sizeof(int) //EBO,empty base optimization,no padding to alignment

继承后D类可能遮盖继承来的名称，重载函数被遮掩（review cpp pirmir继承作用域节名字查找部分），适用using B::func;，避免遮掩。

private继承，public使用B类中的函数时，不能使用using B::func；会暴露所有同名函数，使用转交函数。

```c++
class Derived:private Base{
public:
  virtual void mf1() {
    Base::mf1();
  }
};
```

pure virtual：接口。

impure virtual：接口和缺省实现。

接口和缺省实现的两种方式：

public：纯虚函数接口，protected：defaultfunc（用户自己在D类中B::default...调用）。

public：纯虚函数接口，class外定义纯虚函数，使用纯虚函数的定义作为defaultfunc，此时接口和实现必须都是public。

non-virtual：不变性。

## 考虑virtual函数以外的其他选择

```c++
// non-virtual interface NVI
class GameCharacter {
public:
  int healthValue() const {	// B中保留“虚函数”何时被调用的权利
    ...	// 做一些事前工作
      int retVal = doHealthValue(); // 做真正的工作，virtual function
    ... // 做一些事后工作
  }
private:
  virtual int doHealthValue() const {
    ...	// D中重新定义某些事“如何被完成”
  }
};
```

将virtual函数替换为“函数指针成员变量”，Strategy模式。用function<...>保存调用对象。non-member function可能会降低封装性。

## 绝不重新定义继承而来的non-virtual函数

non-virtual函数静态绑定。

## 绝不重新定义继承而来的缺省参数值

virtual函数是动态绑定，缺省参数值是静态绑定。

可以使用NVI在事前工作中指定default value。

## 多重继承

base class里有同名function，要直接使用作用域符号指出，避免二义性。

菱形继承，virtual base，编译器会为virtual继承付出代价，复杂度，如果virtual base classes不带任何数据，将是最具实用价值的情况。

多重继承确有正当用途。其中一个是public继承某个interface class，private继承某个协助实现的class的情况。

# templates

多态：显式接口（函数形式），运行期多态

template：隐式接口（有效表达式），编译期多态（重载函数）

typename作为指出嵌套从属类型名称的前缀词这一规则的例外是：typename不可以出现在base classes list内的嵌套从属类型名称之前，也不可在member initialization list中作为base class修饰符。

```c++
template<typename T>
class Derived:public Base<T>::Nested{ // base classes list中不允许typename
public:
  explicit Derived(int x)
    : Base<T>::Nested(x) { // mem.init.list中不允许typename
      ...
      typename Base<T>::Nested temp;	// 非以上两者，须加typename
      ...
    }
};
typename std::iterator_traits<IterT>::value_type temp; // 须加typename
```

模板做基类时，其某些全特化版本可能没有某些成员函数，所以不在基类中做名字查找（默认遮掩），使用using Base::func;使用基类中的函数。

因非类型模板参数而造成的代码膨胀，往往可以消除，做法是以函数参数或class成员变量替换template参数。

```c++
// template满足转型要求
template<typename T>
class SmartPtr {
public:
  template<typename U>
  SmartPtr(const SmartPtr<U>& other)	// 泛化copy
  : heldPtr(other.get()) { ... }
  T* get() const { return heldPtr; }
  // 同时需要声明正常的copy构造函数和copy assignment，否则编译器会合成default
  ...
private:
  T* heldPtr; // U*能隐式转换为T*时才能通过编译，对泛化copy加以限制
};
```

```c++
// 需要类型转换时请为模板定义非成员函数
template<typename T> class Rational;

template<typename T>
const Rational<T> doMultiply(const Rational<T>& lhs, const Rational<T>& rhs);	// helper template

template<typename T>
class Rational {
public:
  ...
  friend const Rational<T> operator*(const Rational<T>& lhs,	// 为了让类型转换发生在所有实参上，non-member。为了让函数自动具现化，定义在class内部。
                                    const Rational<T>& rhs) {
    return doMultiply(lhs, rhs);	// 为了避免inline开销，helper template
  }
  ...
};
```

```c++
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag:public input_iterator_tag {};
struct bidirectional_iterator_tag:public forward_iterator_tag {};
struct random_access_iterator_tag:public bidirectional_iterator_tag {};
```

# new/delete

operator new operator delete operator new[] operator delete[]

operator new异常->new_handler(调用set_new_handler设置)

一个设计良好的new_handler应该做以下事情：让更多内存可被使用、安装另一个new_handler（如果这个new_handler无法使更多内存可用）、卸除new_handler、抛出bad_alloc、不返回（abort或exit）。

何时替换：用来检测运用上的错误、为了强化效能、为了收集使用上的统计数据。

operator new应该内含一个无穷循环，并在其中尝试分配内存，如果它无法满足内存需求，就该调用new_handler。它也应该有能力处理0 bytes申请。class专属版本则还应该处理“比正确大小更大的（错误）申请”。

operator delete应该在收到null指针时不做任何事。class专属版本则还应该处理“比正确大小更大的（错误）申请”。

写placement new时也写对应的placement delete，在构造函数失败时调用，避免内存泄漏。

声明placement new和placement delete时不要遮掩其原始版本。

# pointers references

都是间接参考其他对象。

没有null reference，一个reference总是代表另一个对象，必须有初值，类内必须被初始化。pointer可为null。

ref使用前不需要测试其有效性，pointer需要。

ref总是指向最初的对象，而pointer可以被重新赋值。

当需要指向某个东西，而且绝不会改变指向其他东西，或是当实现一个操作符（operator=）而其语法需求无法由pointer达成，应该使用ref，否则应该使用pointer。

# 绝对不要以多态方式处理数组

传递指针指针和数组大小之后，B类仍无法确定D类数组中每个元素的size，由B类执行D类数组的操作和删除都会有问题。

# 异常

Expection objects必定会复制，不论pass by value还是pass by reference。

throw;和throw w;重新抛出expection和抛出被捕捉的expection的一个副本。

catch by value 和 catch by ref 临时对象+复制到value 两个副本 临时对象 一个副本

注意传递pointer时，出了作用域的pointer可能空悬，pointer指向的对象应为global或static或heap，但是删除仍是个问题。

expection与catch子句相匹配的过程中，仅有两种转换可能发生，expection的继承体系内的转换和const void*可以捕捉任何指针类型的expection。

expection catch first fit，virtual function best fit。

catch by value可能会造成切割问题。

**catch by reference是最优解。**

# Efficiency

lazy evaluation:区分读和写(copy on write)，lazy fetching(缓式取出，需要的时候再请求数据)，lazy expression evaluation(需要的时候再计算)。

over-eager evalution:caching(需要min，max值时，每次存取数据时都更新cache值，需要的时候直接取出)，prefetching（局部性，取出相邻的数据。空间换时间，预先多分配空间，vector）。

临时变量：未命名的non-heap object,构造和析构可能会对性能造成影响，隐式类型转换（把const char* by value 赋给string形参），函数返回对象（operator+的retval，+=就避免了临时变量）。

利用函数重载可以避免隐式类型转换（也要注意避免不必要的重载，同时重载运算符时必须有一个自定义类型（cpp pirmir））。

```c++
// 以op=取代op
const T operator+(const T& lhs, const T& rhs) {
  return T(lhs) += rhs;	// 匿名变量总是比命名变量容易消除，编译器优化
}
```

每个class只需要一个vtbl，常见的设计是，放在内含其第一个non-inline,non-pure虚函数定义式的目标文件中。如果所有虚函数都是inline就会有麻烦，所以编译器一般忽略虚函数的inline请求，运行期多态本来就不应该inline，编译期会把inline函数直接展开在调用点。

type_info放在vtbl[0]。

# Techniques, Idioms, Patterns

## 将constructor和non-member functions虚化

```c++
// virtual copy function
class NLComponent {
public:
	// 声明virtual copy constructor
  virtual NLComponent* clone() const = 0;
  ...
};
class TextBlock:public NLComponent {
public:
  virtual TextBlock* clone() const {
    return new TextBlock(*this);
  }
  ...
};
class Graphic:public NLComponent {
public:
  virtual Graphic* clone() const {
    return new Graphic(*this);
  }
  ...
};
// 调用真正的copy constructor
```

```c++
class NLComponent {
public:
  virtual ostream& print(ostream& s) const = 0;
  ...
};
class TextBlock:public NLComponent {
public:
  virtual ostream& print(ostream& s) const;
  ...
};
class Graphic:public NLComponent {
public:
  virtual ostream& print(ostream& s) const;
  ...
};
inline
ostream& operator<<(ostream& s, const NLComponent& c) {
  return c.print(s);
}
```

smart pointer使用pass-by-reference比较好。

```c++
// smart pointer解引用符
template<class T>
T& SmartPtr<T>::operator*() const {
  return *pointee;
}
// operator->
template<class T>
T* SmartPtr<T>::operator->() const {
  return pointee;
}
// 自然的测试smart pointer是否为nullprt
template<class T>
class SmartPtr {
public:
  ...
  operator void*();
  ...
};
if (ptn == 0) ... // it's ok
// 问题是 ptn1 == ptn2; 也会成立，即使ptn1和ptn2不是一类smart pointer
// 重定义operator!()
template<class T>
class SmartPtr {
public:
  ...
  bool operator!() const;
  ...
};
if (!ptn) ... // it's ok
// 不要提供smart pointer到dumb pointer的隐式转换操作符，除非不得已
// 用template进行smart pointer的转换
template<class T>
class SmartPtr {
public:
  SmartPtr(T* realPtr = 0);
  T* operator->() const;
  T& operator*() const;
  
  template<class newType>
  operator SmartPtr<newType>() {
    return SmartPtr<newType>(pointee);
  }
  ...
};
```

reference-counting

double-dispatch RTTI,vitual function+reload function,function map,...

## 混用C和C++

确定C++和C编译器产出兼容的目标文件。

将双方都使用的函数声明为extern “C”。

如果可能，尽量在C++中撰写main。

总是以delete删除new返回的内存；总是以free释放malloc返回的内存。

将两个语言间的“数据结构传递”限制于C所能了解的形式；C++ structs如果内含非虚函数，则不受此限制。

