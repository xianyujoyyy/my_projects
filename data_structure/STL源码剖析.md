# allocator

SGI STL default allocator:std::alloc

* new
  	- ::operator new配置内存。
  	- constructor构造。

* delete
  	- destructor析构。
  	- ::operator delete释放内存。  

STL allocator分两阶段：

* 内存：alloc::allocate() alloc::deallocate()
* 构造/析构：::constructor() ::destroy()

<stl_construct.h>:construct(),destroy() 全局函数

* Construct():泛化  **placement new**

* destroy():1. 泛化

  2.特化（T* pointer）,(iter1,iter2) trivial destructor?__false_type逐个析构

  __true_type no-op,(char\*,char\*),(wchar_t\*, wchar_t\*).

  typedef __type_traits<T>::has_trivial_destructor trivial_destructor

<std_alloc.h>:::operator new(),::operator delete()相当于C的malloc(),free()

双层配置器：第一层(>128bytes)malloc(),free().第二层memory pool.

Cpp ::operator new 有new-handler机制  C malloc要自己写“内存不足处理例程”

Memory pool:free_list

​						union obj{

​							union obj* free_list_link;

​							char client_data[1];

​							}

Chunk_alloc():1.空间足够，取出，1个交给user，剩下的并入free_list。2.可以取出>1个块，1个交给user，剩下的并入free_list。3.1个都取不出，先将剩下的空间并入free_list，然后检查free_list中是否有空间给user，有的话从free_list取出足够空间给user，剩下的标记为memory_pool中的空间，然后调用chunk_alloc整理空间。4.仍然不足就调用malloc，调用chunk_alloc整理空间。

<stl_uninitialized>:stl有5个全局函数作用于未初始化的空间上，construct（），destroy（），uninitialized_copy(),uninitialized_fill(),uninitialized_fill_n(),对应于copy(),fill(),fill_n().

uninitialized_copy(),uninitialized_fill(),uninitialized_fill_n():*commit or rollback*

typedef typename __type_traits<T>::is_POD_type is_POD;

POD:plain old data,C struct,必然拥有trivial ctor/dtor/copy/assignment函数

泛化，特化POD？is_POD(copy(...),fill(...),fill_n(...))is_not_POD(每个分别construct(...)),uninitialized_copy()特化char\*、wchar_t\* memmove()

# iterators

胶合泛型化的containers和algorithms。

在containers内部定义，重载operator*，operator->,operator++,operator++(int)等。

## Traits

大量运用于STL中，利用“内嵌型别”的编程技巧与编译器template参数推导能力，增强C++未能提供的关于型别认证方面的能力。

**Partial Specialization偏特化**：特化程度高于泛化，弱于特化。

typename T::type func(); typename指示T::type为class。

template\<class I\>

struct iterator_traits{

typedef typename I::iterator_categoty iterator_category;

typedef typename I::value_type value_type;

typedef typename I::difference_type difference_type;

typedef typename I::pointer pointer;

typedef typename I::reference reference;

};

* Value type:所指对象的型别。

* different type:两个迭代器之间的距离。

* pointer，reference：所指对象的指针和引用，const T *，T *...。

* Iterator_categoty:

  struct input_iterator_tag{};

  struct output_iterator_tag{};

  struct forward_iterator_tag:public input_iterator_tag{};

  struct bidirectional_iterator_tag:public forward_iterator_tag{};

  struct random_access_iterator_tag:public bidirectional_iterator_tag{};

  //针对原生指针pointer-to-const而设计的traits偏特化版本

  template\<class T\>

  struct iterator_traits\<const T\* \>{

  typedef  random_access_iterator_tag  iterator_category;

  typedef T  value_type;

  typedef ptrdiaff_t  difference_type;

  typedef const T\*  pointer;

  typedef const T&  reference;

  };

**\_\_type_traits**:编译期间确定函数派送（function dispatch），提高效率。

`struct __true_type {};  struct __false_type {};`

```c++
template <class type>
struct __type_traits{
	typedef __true_type this_dummy_member_must_be_first;
	typedef __false_type have_trivial_default_constructor;
	typedef __false_type have_trivial_copy_constructor;
	typedef __false_type have_trivial_assignment_constructor;
	typedef __false_type have_trivial_destructor;
	typedef __false_type is_POD_type;
};
```

在构造时动态分配内存的为non-trivial-xxx。

# Container

## sequence containers

adapter:stack,queue,base on deque,no iterator

binary heap（完全二叉树）:priority_queue的底层实现。

priority_queue:adapter container, queue,no iterator

## associative containers

Set,map,multiset,multimap:RB tree

Hash_set,hash_map,hash_multiset,hash_multimap:hashtable

# Algorithms

```c++
template<class Iterator, class T>
Iterator find(Iterator begin, Iterator end, const T& value) {
  while (begin != end && *begin != value) ++begin;
  return begin;
}
```

借助iterator和pass by reference实现泛型算法。

质变算法，xxx_copy版本，先复制一个副本，然后再进行操作。

copy：完全泛化版本

```c++
template<class InputIterator, class OutputIterator>
  inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
  return __copy_dispatch<InputIterator, OutputIterator>()
    (first, last, result);
}
```

特化版本`const char*,const wchar_t*`使用`memmove(bufprt, first, length(bytes))`

`__copy_dispatch()`：完全泛化版本

```c++
template <class InputIterator, class OutputIterator>
  struct __copy_dispatch {
    OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
      return __copy(first, last, result, iterator_category(first));
    }
  };
```

迭代器为前向迭代器时，以迭代器相等判断赋值范围，迭代器为随机访问迭代器时，以n（last-first）确定赋值范围，速度更快。

两个偏特化版本`T*,const T*`参数为原生指针，有trivial assignment constructor

memmove（），non-trivial assignment constructor使用__copy()的随机访问版本（原生指针是随机访问指针）。

set相关算法：`OutputIterator set_xxx(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)`

`set_union并集，set_intersection交集，set_difference差集，set_symmetric_difference对称差集`

Introsort:quick sort（分割，median-of-3,取头尾中三点，取三个值中的中间值）分割层数超过限制时，局部调用heap sort，元素数小于16时，使用Insertion sort。 *迭代器为随机访问迭代器，无随机访问迭代器的container sort自己实现*

# function objects

<functional>

“行为类似函数”的对象，可作为algorithms op参数。

`greater<int> ig; ig(4,6);greater<int>()(6,4);`

仿函数配接型别：unary_function 一元函数 binary_function 二元函数

```c++
template <class Arg, class Result>
  struct {
    typedef Arg argument_type;
    typedef Result result_type;
  };
typename Functor::augument_type
  
template <class Arg1, class Arg2, class Result>
  struct binary_function{
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
  };
template <class T>
  struct Op : public binary_function<T, T, T>{
    ...
  }
template <class Op>
  class binder1st{
    ...
    protected:
			Op op;
			typename Op::first_argument_type value;
    public:
			typename Op::result_type
      operator() (const typename Op::second_argument_type& x)   const   {return op(x, value);                                                                 }};
```

常见仿函数：

`plus<T> minus<T> multiplies<T> divides<T> modulus<T> negate<T>`

`equal_to<T> not_equal_to<T> greater<T> greater_equal<T> less<T> less_equal<T>`

`logical_and<T> logical_or<T> logical_not<T>`

# adapters

将一个class的接口转换为另一个class的接口，使原本因接口不兼容而不能合作的classes，可以一起运作。function adapter，container adapter，iterator adapter。

Stack, queue,insert iterators,reverse iterators，bind1st<Op>(op, arg1_type(x)),bind2nd(op, arg2_type(x))

Bind2nd(plus<int>(), 2)

特点：类中含有被配接对象，调用被配接对象成员函数实现，接口与被配接对象不一致。

Function adapter:

 * 对返回值进行逻辑否定：not1，not2。

 * 对参数进行绑定：bind1st，bind2nd。

 * 用于函数合成：compose1，compose2.

 * 用于函数指针：ptr_fun operator()通过函数指针执行函数

 * 用于成员函数指针：mem_fun,mem_fun_ref.

   `for_each(v.begin(), v.end(), mem_fun(&superclass::display));`

函数指针void (*prt) (arg1, arg2,...)  执行prt(arg1, arg2,...)

成员函数指针void (class::*pf) (arg1, arg2,...) 执行(p->\*f)() (r.\*f)()