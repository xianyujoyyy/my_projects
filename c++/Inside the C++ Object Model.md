# The Semantics of Data

决定class大小的因素：

instance data members

vptr/bptr/...

alignment

static data memebers只有一份实例，在global data segment中。

noninline member function和static member function也只有一份实例，在class之外。

static data member存取路径直接来自data segment，不论继承关系多么复杂。

&Point3d::chunkSize;(static data member)  内存地址是const int*,与class无关。

如果两个static data member重名，编译器对每一个编码，以获得独一无二的程序识别代码，name-mangling。

继承与data member：

D类中直接包含B类的data members（B类的padding仍保留）。

多态：

vptr放在data members之前或之后（与C兼容）。

多重继承：

D d; B* pb;

pb = &d;

// 编译器需要做内部转化

pb = (B*)((char\*)&d) + sizeof(d); // base + offset

 pv = pv3d ? (Vetex*) ((char\*)pv3d) + sizeof(Point3d) : 0;

// 指针要作0判断，ref不需要，必不为0

虚拟继承：

virtual base class不变区域，直接存取。

共享区域，1.D类中安插指针，指向virtual base class，指针间接完成。2.virtual base class table。3.在virtual function table中放置virtual base class的offset。正值索引function，副值索引virtual class。

指向Data Members的指针：

取data members的地址，得到的是它在class里的offset。

为了区分没有指向任何data member的指针和指向第一个data member的指针，data member指针+1.

&origin._y实际为：&origin + (&Class::\_y - 1);

# The Semantics of Function

非静态成员函数：

至少和非成员函数有相同的效率。member function 被内化为nonmember形式。借助this指针。

```c++
void normalize_7Point3dFv(register const Point3d* const this, Point3d &_result) {
  register float mag = this->magnitude();
  __result.Point3d::Point3d();
  
  __result._x = this->_x/mag;
  __result._y = this->_y/mag;
  __result._z = this->_z/mag;
  
  return;
}
```

静态成员函数：

转换为一般的nonmember函数调用。

&Point3d::object_count(); 得到 unsigned int (*) (); 而非 unsigned int (Point3d::\*) ();

虚拟成员函数：

单继承 一个class只会有一个virtual table。每一个virtual table内含其对应之class object中所有active virtual functions函数实例的地址。每一个virtual function被指派一个固定的索引值，这个索引在整个继承体系中保持与特定的virtual function的关系。（slot x）

table中的函数有三种可能性：1.它可以继承base class所声明的virtual functions的函数实例。该函数实例地址会被拷贝到derived class的virtual table的相应slot中。2.它可以使用自己的函数实例。它自己的实例必须放在相应的slot中。3.它可以加入一个新的virtual function。这时virtual table的尺寸会增大一个slot，而新的函数实例地址会被放进该slot中。

在执行时，prt指向的地址取对应slot的函数就得到实际执行的函数实例。

多重继承：

Base2 *pbase2 = new Derived;

新的Derived对象的地址必须调整以指向Base2 subobject。编译期间会产生以下代码：

Derived *temp = new Derived;

Base2 *pbase2 = temp ? temp + sizeof(Base1) : 0;

如果不进行如上调整，指针的任何“非多态运用”都将失败。

当程序员要删除pbase2所指的对象时：delete pbase2； 指针必须被再一次调整，以求再一次指向Derived对象的起始处。然而不能在编译期直接设定，因为pbase2所指的真正对象只有在执行期才能确定。

Thunk技术：一个derived class内涵n - 1个额外的virtual tables。每一个virtual tables，derived对象中有相应的vptr。对象当前指向的virtual table会在相应slot间接指向真正需要执行的函数操作。

