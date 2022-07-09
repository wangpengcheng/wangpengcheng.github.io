---
layout:     post
title:      C++ Primer 学习笔记(七)
subtitle:   C++ Primer 学习记录(七)
date:       2019-05-26
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - C++
    - 基础编程
---

>>2019-5-26 :19:50:52

## 第 12 章 动态内存

### 12.1 动态内存与智能指针
在c++中动态内存管理是通过一对运算符来完成的：`new`和`delete`;同时为了更加安全的管理指针；标准库提供了两种 **智能指针** ：**shared_ptr**、**unique_ptr** ;前者允许多个指针指向同一个对象；后者独占所指对象。同时还有 **weak_ptr**的伴随类，他是一种弱引用，指向shared_ptr所管理的对象。三种类都定义在头文件memory中

当我们创建指针时，必须提供指针可以指向的类型。

```c++
shared_ptr<string> p1; //shared_ptr,可以指向string 

shared_ptr<list<int>> p2; //shared_ptr,可以指向int的list 
```

**shared_ptr和unique_ptr都支持的操作**

|操作|含义|
|:---|:---|
|`shared_ptr<T> sp unique_ptr<T> up`|空智能指针，可以指向类型为`T`的对象|
|`p`|将p用作一个条件判断，若`p`指向一个对象，则为`true`|
|`*p`|解引用p,获得它指向的对象|
|`p->mem`|等价于`(*p).mem`|
|`p.get()`|返回`p`中保存的指针，要小心使用；若智能指针释放了其对象，返回的指针所指向的对象也就消失了|
|`swap(p,q)/p.swap(q)`|交换`p`和`q`中的指针|

**share_ptr独有的操作**

|操作|含义|
|:---|:---|
|`make_shared<T>(args)`|返回一个`shared_ptr`,指向一个动态分配的类型为`T`的对象。使用`args`初始化此对象|
|`shared_ptr<T> p(q)`|p是`shared_ptr q`的拷贝；此操作会递增`q`中的计数器。`q`中的智能指针必须转化为`*T`|
|`p=q`|p和q都是`shared_ptr`，所保存的指针必须能够相互转换。此操作会递减`p`的引用计数，递增`q`的引用计数；若`p`的引用计数变为0，则将其管理的原内存释放|
|`p.unique()`|若`p.use_count()`为1，返回`true`;否则返回`false`|
|`p.use_count()`|返回与`p`共享对象的智能指针数量；可能很慢，主要用于调试|

注意：
- 我们通常使用`auto`作为`make_shared`指针函数所对应的值。
- 不要使用引用传递，在函数体内对智能指针`shared_ptr`进行赋值操作，计数会-1，离开函数作用域，局部变量销毁，计数可能为0.

计数递增情况：

- 一个`shared_ptr`初始化另外一个`shared_ptr`
- 作为参数传递给一个函数
- 作为函数的返回值

计数减少情况：

- `share_ptr`赋予一个新值
- `shared_ptr`被销毁--局部的`shared_ptr`离开其作用域。

注意：
- 如果将`shared_ptr`存放于一个容器中，而后不再需要全部元素，而只使用其中一部分，要记得`erase()`删除不再需要的那些元素。
- 使用动态内存的一个常见原因是允许多个对象，共享相同的状态。

#### 12.1.2 直接管理内存
c++可以使用`new`和`delete`直接管理内存；尽量在分配内存时，对动态分配内存的对象进行初始化操作。

```c++

int *p1=new int; //如果分配失败，new抛出std::bad_alloc;

int *p2=new (nothrow) int; //如果分配失败，new 返回一个空指针

delete p1,p2;
```
注意： 

- 我们传递给delete的指针必须指向动态分配的内存。或者是一个空指针。
- 不要随便删除一个空指针，可能在指针所指向的内存，被其它程序所使用，删除空指针，会造成极大的问题。
- 动态对象的生存周期是直到被释放为止。
- 由内置指针(而不是智能指针)管理的动态内存在被显式释放之前一直都会存在。
- 尽量坚持使用智能指针，防止内存分配错误
- 在删除指针之后使用`NULL/nullptr`使得指针悬空。
- 悬空只真对当前指针，对于和当前其它指向销毁内存对象的指针是没有作用的，尽量减少使用指针赋值和拷贝，或者直接使用shared_ptr智能指针。例如：

```c++
shared_ptr<int> p1=new int(1024); //错误；必须使用直接初始化形式。

shared_ptr<int> p2(new int(1024)); //正确：使用了直接初始化形式。

```

**定义和改变shared_ptr的其它方法**

|操作|含义|
|:---|:---|
|`shared_ptr<T> p(q)`|p管理内置指针q所指的对象；q必须指向`new`分配的内存，且能够转换为`T*`类型|
|`shared_ptr<T> p(u)`|p从unique_ptr u中接管了对象的所有权；将`u`置为空|
|`shared_ptr<T> p(q,d)`|p接管了内置指针q所指向的对象的所有权。q必须能转换为`T*`类型。p将使用可调用对象`d`来代替`delete`|
|`shared_ptr<T> p(p2,d)`|p是`shared_ptr p2`的拷贝,唯一的区别是`p`将使用可调用对象`d`来代替`delete`|
|`p.reset()`|若p是唯一指向其对象的`shared_ptr`，reset会释放此对象。|
|`p.reset(q)`|释放原有对象的同时，若传递了可选的内置参数指针`q`，会令`p`指向`q`，否则会将p置为空|
|`p.reset(q,d)`|若还传递了参数`d`,将会调用`d`而不是`delete`来释放`q`|

注意：

1. 不要混合使用普通指针和智能指针，很容易造成错误的析构
2. 不要使用get初始化另外一个智能指针或者为智能指针赋值，赋值给内置指针时，需要注意，不要让它管理的指针被释放。
3. 使用get返回指针的代码不能`delete`此指针。防止智能指针失效，成为野指针。
4. get用来将指针的访问权限传递给代码，只有确定代码不会delete指针的情况下，才能使用get。永远不要使用get初始化另外一个智能指针或者为另外一个智能指针赋值。
5. 智能指针指向新指针时，需要检查自己是否是当前对象仅有的用户。如果不是，在改变之前要制作一份新的拷贝。

```c++

if(!p.unique())
    p.reset(new string(*p)); //我们不是唯一的用户；分配新的拷贝

*p+=newVal; //可以改变对象的值

```
#### 12.1.4 智能指针和异常

注意，**智能指针陷阱**

基本规范：

- 不使用相同的内置指针值初始化(或reset)多个智能指针
- 不delete get()返回的指针
- 不使用 get()初始化reset另外一个指针
- 如果你使用get()返回的指针，记住，当最后一个指针对应的智能指针销毁后，你的指针就变为无效了
- 如果你使用智能指针管理的资源不是new分配的内存，记住传递给它一个删除器。

#### 12.1.5 unique_ptr

**unique_ptr操作**

|操作|含义|
|:---|:---|
|`unique_ptr <T> u1`|空unique_ptr，可以指向类型为T的对象，u1会使用delete来释放它的指针|
|`unique_ptr <T,D> u2`|u2使用一个类型为D的可调用对象来释放它的指针|
|`unique_ptr <T,D> u(d)`|空unique_ptr，指向类型为T的对象，用类型为D的对象d代替delete|
|`u=nullptr`|释放u指向的对象，将u置为空|
|`u.release()`|u放弃对指针的控制权，返回指针，并将u置为空|
|`u.reset()`|释放u指针对象|
|`u.reset(q)/u.reset(nullptr)`|如果提供了内置指针q,令u指向这个对象；否则将`u`置为空|

unqiue_ptr不能共享所有权，但是可以将所有权，进行转移

```c++
//将所有权从一个unique_ptr转移给另一个unique

unique_ptr<string> p2(p1.release()); //release 将p1置为空

unique_ptr<string> p3(new string("Text"));
//将所有权从p3转移给p2

p2.reset(p3.release()); //reset释放了p2原来指向的内存

```

#### 12.1.6 weak_ptr

weak_ptr是一种不控制所指向兑现生存周期的智能指针，它指向一个由shared_ptr管理的对象。

**weak_ptr**

|操作|含义|
|:---|:---|
|`weak_ptr <T> w`|空`weak_ptr`可以指向类型为`T`的对象|
|`weak_ptr <T> w(sp)`|与`shared_ptr sp`指向相同对象的`weak_ptr`，T必须可以指向类型为`sp`的对象|
|`w=p`|p可以是一个`shared_ptr`或者一个`weak_ptr`。赋值后w与p共享对象|
|`w.reset()`|将w置为空|
|`w.use_count()`|与w共享对象的`shared_ptr`的数量|
|`w.expired()`|若`w.use_count()`为0，返回true，否则返回`false`|
|`w.lock()`|如果`expired`为`true`，返回一个空`shared_ptr`；否则返回一个指向`w`的对象的`shared_ptr`|

我们创建一个`weak_ptr`时，要用一个`shared_ptr`来初始化它：

```c++
auto p=make_shared<int>(42);

weak_ptr<int> wp(p);//wp 弱共享p;p的引用计数未改变，因此使用时必须使用函数lock()

if(shared_ptr<int> np=wp.lock()){//如果np不空则条件成立
    
    //在if中，np与p共享对象
}
```
### 12.2 动态数组

数组主要是为了更好的快速分配内存；大多数应用应该使用标准库容器而不是动态分配的数组。使用容器更为简单、更不容易出现内存管理错误并且拥有更好的性能。

注意：

- 分配数组，会得到一个元素类型的指针。
- 动态分配一个空数组是合法的。
- 释放动态数组使用`delete []`,数组中的元素按逆序销毁。
- 数组指针需要使用`delete []`,单个对象需要使用`delete`,换用的行为都是未定义的。
- 未使用匹配的delete时，编译器很可能不会给出警告。程序可能在执行过程中没有任何警告的情况下行为异常。

初始化动态分配对象的数组

```c++
int *pia=new int[10]; //10个没有初始化的int

int *pia2=new int[10](); //10个初始化为0的int

string *psa=new string[10]; //10个空string

string *psa2=new string[10](); //10个空string

string *psa3=new string[10]{"a","an","the",sting(3,'x')}; //10个string初始化

```

**指针和动态数组**
```c++
unique_ptr<int []> up(new int[10]); //up指向10个未初始化int的数组

up.release(); //自动调用delete销毁其指针。
```

#### 12.2.2 allocator类

通常因为指针数组的定额分配可能产生内存浪费，因此需要`allocator`来进行内存分配。类似vector，allocator是一个模板，为了定义一个allocator对象，我们必须指明这个allocator可以分配的对象类型。当一个allocator对象分配内存时，它会根据给定的对象类型来确定恰当的内存大小和对齐位置：

```c++
allocator<string > alloc; //可以分配stringd allocator对象

auto const p=alloc.allocate(n); //分配n个未初始化的string
```
**标准库allocator类及其算法**

|算法|解释|
|:---|:---|
|`allocator <T> a`|定义了一个名为`a`的`allocator`对象，他可以为类型T的对象分配内存|
|`a.allocate(n)`|分配一段原始的、未构造的内存，保存n个类型为`T`的对象|
|`a.deallocate(n)`|释放从`T*`指针p中地址开始的内存，这块内存保存了n个类型为T的对象；p必须是一个先前由`allocator`返回的指针，且n必须是p创建时所要求的大小。调用`dealocator`之前，用户必须对每个在这块内存中创建的对象调用`destroy`|
|`a.construct(p,args)`|p必须是一个类型为`T*`的指针，指向一块原始内存；arg被传递给类型为T的构造函数，用来在p指向的内存中构造一个对象|
|`a.destroy(p)`|p为`T*`类型的指针，此算法对p指向的对象执行析构函数|

注意：

- allocator分配的内存时未构造函数的。我们按照需要在此内存中构造对象。
- 为了使用allocator返回的内存，我们必须使用construct构造对象。未使用构造的内存，其行为是未定义的。
- 我们只能对真正构造了的元素进行destory操作。

```c++
auto q=p;  // q指向最后构造的元素之后的位置

alloc.construct(q++);  //*q为空字符串

alloc.construct(q++,10,'c'); // *q为cccccccccc

std::cout<<*p<<endl; //正确：使用string 的输出运算符

cout<<*q<<endl;  // 灾难： q指向未构造的内存。

//使用完成必须对每个构造的元素调用destory来销毁它们

while (q!=p)
    alloc.destory(--q);  //释放我们真正构造的string，此时元素被销毁，可以重新使用这部分内存来保存其它string

alloc.deallocate(p,n);  //释放内存操作

```
**allocator 算法**

|算法|含义|
|:---|:---|
|`uninitialized_copy(b,e,b2)`|将迭代器b和e之间的输入，拷贝到迭代器b2指定的未构造的原始内存中，b2指向的内存必须足够大，能够容纳输入序列中元素的拷贝|
|`uninitialized_copy_n(b,n,b2)`|同上，从b开始拷贝n个元素到b2|
|`uninitialized_fill(b,e,t)`|在迭代器b和e指定的原始内存范围中创建对象，对象的值，均为t的拷贝|
|`uninitialized_fill_n(b,n,t)`|从b指向的内存地址开始创建n个对象，b必须指向足够大的内存|

使用示例：
```c++
//分配比vi中元素所占用空间大一倍的动态内存

auto p=alloc.allocate(vi.size()*2);
//通过拷贝vi中的元素来构造从p中开始的元素

auto q=uninitialized_copy(vi.begin(),vi.end(),p);
//将剩余元素初始值为42
uninitialized_fill_n(q,vi.size(),42);

```

# 第III 部分类设计者的工具

##第13 章拷贝控制
类的五种特殊成员函数:

- 拷贝构造函数
- 拷贝赋值构造函数
- 移动构造函数
- 移动赋值运算符
- 析构函数

注意：定义任何C++类的时候，拷贝控制操作都是必要部分。即使我们不显示定义这些操作，编译器也会为我们定义，但是编译器定义的版本的行为可能并非我们所想象。

### 13.1 拷贝、赋值与销毁

**合成拷贝构造函数**：用来阻止我们拷贝该类类型的对象。一般情况，合成的拷贝构造函数会将其参数的成员逐个拷贝到正在创建的对象中。编译器给定对象中依次将每个非static成员拷贝到正在创建的对象中。

拷贝初始化发生情况：

- 使用`=`定义变量时会发生
- 将一个对象作为实参传递给一个非引用类型的形参
- 从一个返回类型为非引用类型的函数返回一个对象
- 用`{}`列表初始化一个数组中的元素或者一个聚合类中的成员
- 接收参数的构造函数，一般是explicit的
- 编译器可以绕过拷贝构造函数

```c++
vector<int> v1(10); //正确：直接初始化

vector<int> v2=10; //错误：接受大小参数的构造函数时explicit的

void f(vector<int>); //f的参数进行拷贝初始化

f(10); //错误；不能用一个explicit的构造函数拷贝一个实参

f(vector<int>(10));正确：从一个int直接构造一个临时的vector

string null_book="9-99-99";//拷贝初始化

string null_book("9-99-99"); //编译器略过了拷贝构造函数
//重载赋值运算符

class Foo{
public:
    Foo& operator=(const Foo&); //赋值运算符
    ...
}

Foo& Foo::operator=(const Sales_data &rhs){
    bookNo=rhs.bookNo;  //调用 string::opeator=

    units_sold=rhs.units_sold; //使用内置的int赋值

    ...
    return *this;
}
```
赋值运算符通常应该返回一个指向其左侧运算对象的引用

如果类没有定义自己的拷贝赋值运算符，编译器会为它生成一个 **合成拷贝赋值运算符**

析构函数调用情况

- 变量在理考作用域时被销毁
- 一个对象在被销毁时，其成员被销毁
- 容器(无论是标准库容器还是数组) 被销毁时，其元素被销毁
- 对于动态分配的对象，当对指向它的指针应用delete运算符进行销毁
- 临时对象，当创建它的完整表达式结束时被销毁。

注意：

- 析构函数调用时，先调用父类，再调用子类；与构造函数正好相反。
- 当指向一个对象的引用或者指针离开作用域时，析构函数不会执行。
- delete 删除指针，是删除指针指向的那块内存，指针本身仍旧存在，因此需要，键指针指向NULL防止错误

**三/五法则**
三个基本操作可以控制类的拷贝操作：

- 拷贝构造函数
- 拷贝赋值运算符
- 析构函数
- 如果一个类需要自定义析构函数，几乎可以肯定它也需要自定义拷贝赋值运算符和拷贝构造函数；反之亦然。
- 可以通过使用`=default`来显示地要求编译器生成合成的版本(默认构造函数)。
- 可以使用`=delete`来阻止默认的拷贝个赋值函数。
- 将拷贝和赋值拷贝设为私有函数，将使得用户代码不能拷贝这个类型的函数，用户代码在编译时错误，成员函数或者有元函数在连接诶时错误。
- 希望阻止拷贝的类应该使用`=delete`来定义他们自己的拷贝构造函数和拷贝赋值运算符，而不应该将他们声明为`private`的。

```c++
HasPtr HasPtr::operator=(const HasPtr &rhs)
{
    auto newp=new string(*rhs.ps) ;// 拷贝底层string

    delete ps; //释放旧内存

    ps=newp; //从右侧运算对象拷贝数据到本对象

    i=rhs.i;
    return this; //返回对象本身

}
```
当编写一个赋值运算符的时候，最好先将右侧运算对象拷贝到一个局部临时对象。当拷贝完成后，销毁左侧运算对象的现有成员就是安全的了。一旦左侧运算对象的资源被销毁，就只剩下将数据从临时对象拷贝到左侧运算对象的成员中了。

拷贝赋值运算符通常执行拷贝构造函数和析构函数也要做的工作，公共的工作应该放在`private`的工具函数中。

### 13.5 动态内存管理

简单标准库vector类的简化实现版本。

```c++
//stdvec.h
//类vector类内存分配策略的简化实现

class StrVec{
public:
    StrVec():
        elements(nullptr),first_free(nullptr),cap(nullptr){}
    StrVec(const StrVec&); //拷贝构造函数

    StrVec &operator=(const StrVec&); //拷贝赋值运算符

    ~StrVec();  //析构函数

    void push_back(const std::string&);  //拷贝元素

    size_t size() const {return first_free-elements;}
    size_t capacity() const {return cap-elements;}
    std::string *begin() const {return elements;}
    std::string *end() const {return first_free;}
    // ...
private:
    Static std::allocator<std::string> alloc; //分配元素
    //被添加元素的函数所使用
    
    void chk_n_alloc() {if(size()==capacity()) reallocate();}
    //工具函数，被拷贝构造函数、赋值运算符和析构函数所使用
    
    std::pair<std::string*,std::string*> alloc_n_copy(const std::string*,const std::string*);
    void free();  //销毁元素并释放内存

    void reallocate();  //获得更多内存并拷贝已有元素

    std::string *elemets;  //指向数组首元素的指针

    std::string *frist_free; //指向数组第一个空闲元素的指针

    std::string *cap; // 指向数组尾后位置的指针

};

//strvec.cpp

void Strvec::push_back(const string& s)
{
    chk_n_alloc();  //确保有空间容纳新元素

    //在frist_free指向的元素中构造s的副本
    
    alloc.construct(first_free++,s);
};

std::pair<std::string*,std::string*> alloc_n_copy(const std::string* b,const std::string* e)
{
    //分配空间保存给定范围中的元素
    
    auto data=alloc.allocate(e-b);
    //初始化并返回一个pair,该pair由data和uninitialized_copy的返回值构成
    
    return {data.uninitialized_copy(b,e,data)};
}

void StrVec::free()
{
    //不能传递给deallocate 一个空指针，如果elements为0，函数什么也不做
    
    if(elements){
        //逆序销毁旧元素
        
        for(auto p=first_free;p!=elements;)
        {
            alloc.destory(--p);
            alloc.deallocate(elements,cap-elements);
        }
    }
}

StrVec::StrVec(const StrVec &s)
{
    //调用alloc_n_copy 分配内存空间以容纳与s中一样多的元素
    
    auto newdata=alloc_n_copy(s.begin(),s.end());
    elements=newdata.frist;
    frist_free=cap=newdata.second;
}

StrVec::~StrVec(){free();}

StVec &StrVec::operator=(const StrVec &rhs)
{
    //调用alloc_n_copy 分配内存，大小与rhs中元素占用空间一样多
    
    auto data=alloc_n_copy(rhs.begin(),rhs.end());
    free();
    elements=data.first;
    frist_free=cap=data.second;
    return *this;
}

void StrVec::reallocate()
{
    //我们将分配当前大小两倍的内存空间
    
    auto newcapacity=size()?2*size():1;
    //分配新内存
    
    auto newdata=alloc.allocate(newcapacity);
    //将数据从旧内存移动到新内存
    auto dest=newdata;  //指向新数组中下一个空闲位置

    auto elem=elements; //指向旧数组中下一个元素

    for(size_t i=0;i!=size();++i)
    {
        alloc.construct(dust++,std::move(*elem++));
    }
    free(); //一旦我们移动完成元素就释放旧内存空间

    //更新我们的数据结构，执行新元素
    elements=newdata;
    first_free=dest;
    cap=element+newcapacity;

}
```

### 13.6 对象移动

注意：标准容器库、string和shared_ptr类既支持移动也支持拷贝。IO类和unique_ptr类可以移动但不能拷贝。

使用`&&`来进行右值引用。右值引用--只能绑定到一个将要销毁的对象。**左值持久；右值短暂**。右值引用只能绑定到临时对象。

对于右值有：

- 所引用的对象，将要被销毁
- 该对象没有其他用户
- 使用右值引用的代码可以自由地接管所引用的对象的资源
- 变量是左值，因此我们不能将一个右值引用直接绑定到一个变量上，即使这个变量是右值引用类型。
- 可以销毁一个移后对象，也可以赋予它新值，但不能够使用一个移后源对象的值。

使用move来获得绑定到左值上的右值引用`int &&rr3=std::move(rr1);`

注意：

- 不抛出异常的移动构造函数和移动赋值运算符必须标记为`noexcept`。
- 只有当一个类没有定义任何自己版本的拷贝控制成员，且它的所有数据成员都能移动构造或者移动赋值时，编译器才会为它合成移动构造函数或者移动赋值运算符。
- 定义了一个移动构造函数或者移动赋值运算符符类，必须也定义自己的拷贝构造操作。否则，这些成员默认地被定义为删除的。
- 如果一个类有一个可用的拷贝构造函数而没有移动构造函数，则其对象是通过拷贝构造函数来“移动”的。拷贝赋值运算符和移动赋值运算符的情况类似。
- 移动赋值函数，相对拷贝构造函数，更减少资源的使用。
- 对象移动数据并不会销毁此对象，但有时在移动完之后，源对象会被销毁。编写移动操作时，必须保证源对象可以析构。
- 一个类集邮移动构造，又有拷贝构造，则移动是右值，拷贝是左值。
- 没有移动构造函数，右值也会被拷贝。但是编译器不会合成移动构造函数。即便是使用std::move也是调用的拷贝构造函数。
- 不要随意使用移动操作，一个移动源对象具有不确定的状态。当我们使用`move`时，必须绝对确认移后源对象没有其它用户。
- 移动接收`T&&`,拷贝接收`T&`

**左值和右值引用成员函数**
c++中允许右值进行赋值例如：

```c++
auto s1="hello";
auto s2="word";
s1+s2="wow!";
```
为了阻止这种情况的发生，使用 **引用限定符**：`&` 来强制指定左侧运算对象(即，this指向的对象)是一个左值。或者使用`&&`，强制指明，左侧运算对象是一个右值，例如：

```c++
class Foo{
public:
    Foo &operator=(const Foo&) &; //只能向可修改的左值赋值

}

class Foo{
public:
    Foo someMem() & const; //错误限定符const 必须在前
    
    Foo anotherMem() const &; //正确
}

Foo &retFoo();  //返回一个引用；retFoo调用时一个左值

Foo retVal();  // 返回一个值；retVal调用时一个右值

Foo i,j; //i,j均是左值

i=j; //正确：i是左值

retFoo()=j ;// 正确:retFoo()返回一个左值

retVal()=j; //错误:retVal()返回一个右值

i=retVal();  //正确：我们将一个右值作为赋值操作的右侧运算对象

//成员函数可以根据const区分其重载版本，引用也可以区分重载版本

class Foo{
public:
    Foo sorted() &&;  //可用于可改变的右值

    Foo sorted() const &; //可用于任何类型的Foo
}

//本对象为右值，因此可以原址排序

Foo Foo::sorted() &&
{
    sort(data.begin(),data.end());
    return *this;
}

//本对象是const或者是一个左值，不论何种情况我们都不能对其进行原址排序

Foo Foo::sorted() const & {
    Foo ret(*this);  //拷贝一个副本

    sort(ret.data.begin(),ret.data.end());  //排序副本

    return ret;  //返回副本

}

//这里编译器会根据sorted 的对象的左值/右值属性来确定使用那个sorted版本


```
注意： 如果一个成员函数有引用限定符，则具有相同参数列表的所有版本都必须有引用限定符。

## 第14 章 重载运算符与类型转换

重载的运算符是具有特殊名字的函数；他们的名字由关键字，operator和其后要定义的运算符共同组成。
注意：
- 一个类成员运算符函数，第一个运算对象绑定到隐式的`this`指针上。因此成员函数运算符函数的显示参数数量比运算符的运算对象，总少一个。
- 我们无法改版内置类型的运算符含义。
- 我们只能重载已有的运算符，而无权发明新的运算符号。例如我们不能提供operator**来执行幂操作。

**可重载运算符**

|运算符类型|运算符种类|
|:--|:---|
|双目算术运算符| `+ (加)，-(减)，*(乘)，/(除)，% (取模)`|
|关系运算符  | `==(等于)，!= (不等于)，< (小于)，> (大于>，<=(小于等于)，>=(大于等于)`|
|逻辑运算符  | `//(逻辑或)，&&(逻辑与)，!(逻辑非)`|
|单目运算符  | `+ (正)，-(负)，*(指针)，&(取地址)`|
|自增自减运算符 | `++(自增)，--(自减)`|
|位运算符 |   `/ (按位或)，& (按位与)，~(按位取反)，^(按位异或),，<< (左移)，>>(右移)`|
|赋值运算符  | `=, +=, -=, *=, /= , % = , &=, /(其实是竖着的)=, ^=, <<=, >>=`|
|空间申请与释放| `new, delete, new[ ] , delete[]`|
|其他运算符 |  `()(函数调用)，->(成员访问)，,(逗号)，[](下标)`|

**不可重载的运算符列表**

|运算符|含义|
|:---|:---|
|`.`|成员访问运算符|
|`.*, ->*`|成员指针访问运算符|
|`::`|域运算符|
|`sizeof`|长度运算符|
|`?:`|条件运算符|
|`#`| 预处理符号|

注意：

1. 运算重载符不可以改变语法结构。
2. 运算重载符不可以改变操作数的个数。
3. 运算重载符不可以改变优先级。
4. 运算重载符不可以改变结合性。

**类重载、覆盖、重定义之间的区别：**

重载指的是函数具有的不同的参数列表，而函数名相同的函数。重载要求参数列表必须不同，比如参数的类型不同、参数的个数不同、参数的顺序不同。如果仅仅是函数的返回值不同是没办法重载的，因为重载要求参数列表必须不同（发生在同一个类里）。

覆盖是存在类中，子类重写从基类继承过来的函数。被重写的函数不能是static的。必须是virtual的。但是函数名、返回值、参数列表都必须和基类相同（发生在基类和子类）

重定义也叫做隐藏，子类重新定义父类中有相同名称的非虚函数 ( 参数列表可以不同 ) （发生在基类和子类）。

操作符的等价调用：
```c++
data1+data2; //普通的表达式

operator+(data1,data2); //等价的函数调用

data1.operator+=(data2); //等价的函数调用

```

注意：尽量明智地使用运算符重载。只有当操作的含义对于用户磊说清晰明了时才能使用运算符。当其存在二意性时，最好不要使用。

### 14.2 输入和输出运算符

输出`<<`重载示例:

```c++
ostream &operator<<(ostream &os,const Sales_data &item)
{
    os<<item.isbn()<<" "<<item.units_sold<<""
    <<item.revenue<<" "<<item.avg_price();

    return os;
}
```
注意： 
- 输出运算符应该主要负责打印对象的内容而非控制格式，输出运算符不应该打印换行符。
- 输入输出运算符必须是非成员函数，不能是类的成员函数。否则，他们左侧运算对象将是我们的类的一个对象。

输入`>>`重载示例：

```c++
istream &opertaor>>(istream &is,Sales_data &item)
{
    double price;// 不需要初始化，因为我们将先读入数据到`price`,之后才使用它

    is>>item.bookNo>>iten.units_sold>>price;
    //检查输入是否成功

    if(is)
    {
        item.revenue=item.units_sold*price;
    }else{
        item=Sales_data(); //输入失败：对象呗赋予默认的状态

    }
    return is;


}
```
注意：

1. 当流含有错误类型的数据时，读取操作可能失败。之后的其他使用都将失败
2. 当读取操作达到文件末尾或者遇到输入流的其它错误时，也会失败
3. 当读取操作发生错误时，输入运算符应该负责从错误中恢复。

### 14.3 算术和关系运算符

注意：

- 如果同时定义了算术运算符和相关的复合赋值运算符，则通常情况下应该使用复合赋值来实现算术运算符。
- 如果某个类在逻辑上有相等性的含义，则该类应该定义`operator==`。
- 如果存在唯一一种可靠的`<`定义，则应该考虑为这个类定义`<`运算符。如果类同时还包含`==`，则当且仅当`<`的定义和`==`产生的结果一致时才睡定义`<`运算符。
- 重载赋值运算符。不论形参的类型是什么，赋值运算符都必须定义为成员函数。
- 赋值运算符必须定义为类的成员，复合赋值运算符通常情况下也应该这样做。这两类运算符都应该返回左侧运算对象的引用。

使用示例：

```c++
//重载赋值运算符
StrVec &StrVec::operator=(initializer_list<string> il)
{
    //alloc_copy分配内存空间，并从给定安慰内拷贝元素
    
    auto data=alloc_n_copy(il.begin(),il.end());
    free();  // 销毁对象中的元素并释放内存空间

    elements=data.frist;//更新数据成员，使其指向新空间

    first_free=cap=data.second; 
    return *this;  

} 

//重载复合赋值运算符

StrVec &StrVec::operator+=(initializer_list<string> il)
{
    unit_sold+=rhs.units_sold;
    revenue+=rhs.revenue;
    return *this;
}
```
### 14.5 下标运算符 `operator [] `
注意：

-  下标运算符必须是成员函数。
-  如果一个类包含下标运算符，则它通常会定义两个版本；一个返回普通引用，一个是类的常量成员并且返回常量引用。

使用示例:

```c++
class StrVec{
public:
    std::string& operator [](std::size_t n){return elements[n];}

    const std::string& operator[] (std::size_t n) const
    { return element[n];}

private:
    std::string *elements;  //指向数组首元素的指针
}

const StrVec cvec=svec;  //把svec的元素拷贝到cvec中

//如果svec中含有元素，对第一个元素运行string的empty函数

if(svec.size()&&svec[0].empty){
    svec[0]="zero";  //正确：下标运算符返回string的引用

    svec[0]="Zip";  //错误；对cvec取下标返回的是常量引用
}
```

### 14.6 递增和递减运算符

注意：

- 定义递增和递减运算符的类应该同时定义前置版本和后置版本。这些运算符通常应该被定义成类的成员。
- 为了与内置版本保持一致，前置运算符应该返回递增或者递减后对象的引用。
- 为了与内置版本保持一致，后置运算符应该返回对象的原值(递增或者递减之前的值)，返回的形式是一个值而非引用。
- 因为我们不会用到`int`形参，所以无需为其命名。

```c++
//前置版本：返回递增/递减对象的引用

StrBlobPtr& StrBlobPtr::operator++()
{
    //如果curr已经指向了容器的尾后位置，则无法递增它
    
    check(curr,"xxxx");
    ++curr;  //将curr在当前状态下向前移动一个元素

    return *this;
} 

//后置版本:递增/递减对象的值，但是返回原值
StrBlobPtr StrBlobPtr::operator++(int)
{
    //此处无需检查有效性，调用前置递增运算时才需要检查
    
    StrBlobPtr ret=*this;  //记录当前的值

    ++*this   //向前移动一个元素，前置++需要检查递增的有效性

    return ret;  //返回之前记录的状态

}

```
**C++中 i++ 与++i 的区别**

- i++ 是指先使用i，只用之后再讲i的值加一，
- ++i 是将i的值先加一，然后在使用i；

如果i是一个整型变量那么i++ 与++i 几乎是没有区别的，在学习C++的后面会有迭代器，迭代器是一个对象，当i是迭代器时，那么++i的效率运行速度就比i++快；所以我们在一般的for循环语句中当i++与++i都可以使用时，可以考虑使用++i，养成一种良好的风格。

### 14.7 成员访问运算符

成员访问运算符，在 迭代器及智能指针类中常常见到解引用运算符`*`和箭头运算符`->`。我们以如下形式向`StrBlobPtr`类添加这两种运算符：

```c++
class StrBlobPtr{
public:
    std::string& operator*() const
    {
        auto p=check(curr,"dereference past end");
        return (*p)[curr];  //(*p)是对象所指的vector

    }
    std::string* operator->() const
    {
        //将实际工作委托给解引用运算符
        return& this->operator*();
    }
    //将两个运算符定义成了const成员，这是因为与递增和递减预算符不一样，获取一个元素并不会改变StrBlobPtr对象的状态
}
```
注意：

- 箭头运算符必须是类的成员。解引用运算符通常也是类的成员，尽管并非必须如此。
- 重载的箭头运算符必须返回类的指针或者自定义了箭头运算符的某个类的对象

### 14.8 函数调用运算符
如果类重载了函数调用运算符，则我们可以像使用函数一样灵活使用该类的对象。因为这样的类同时也能存储状态，所以与普通函数相比它们更加灵活。例如：

```c++
strcut absInt{
    int operator()(int val) const
    {
        return val<0?-val:val;
    }
};

//使用

int i=-42; 
absInt absObj;  // 含有函数调用运算符符对象

int ui=absObj(i);  //将i传递给absObj.operator()
```

注意：
- 函数调用运算符必须是成员函数。一个类可以定义多个不同版本的调用运算符，相互之间应该在参数数量或者类型上有所区别。
- 如果类定义了调用运算符，则该类的对象称作 **函数对象**。因为可以调用这种对象，所以我们说这些对象的“行为就像函数一样”。
- lambda是就是一个典型的函数对象。但是lambda表达式的类不含默认构造函数、赋值运算符及默认析构函数；它是否含有默认的拷贝/移动构造函数则通常要视捕获的数据成员类型而定。
- 对于比较两个无关指针的内存地址，将产生未定义的行为，标准函数库提供了相关函数兑现进行定义。

**标准库函数对象**

|算术|关系|逻辑|
|:---|:---|:---|
|`plus<Type>`|`equal_to<Type>`|`logical_and<Type>`|
|`minus<Type>`|`not_equal_to<Type>`|`logical_or<Type>`|
|`multiplies<Type>`|`greater<Type>`|`logical_not<Type>`|
|`divides<Type>`|`greater_equal<Type>`||
|`modulus<Type>`|`less<Type>`||
|`negate<Type>`|`less_equal<Type>`||

```c++
vector<string  *> nameTable; //指针的vector
//错误会产生未定义的行为

sort(nameTable.begin(),nameTable.end(),[](string *a,string *b){return a<b;});
//正确：标准库规定指针的less是定义良好的

sort(nameTable.begin(),nameTable.end(),less<string *>());

```

**可调用对象与function**
c++中的可调用对象：

- 函数
- 函数指针
- lambda表达式
- bind创建的对象
- 重载了函数调用运算符的类

对于相似操作但是输入参数不同的情况，我们可以使用一个名为`function`的新标准库类型解决上述问题,`function`定义在`functional`头文件中。

**function的操作**

|操作|含义|
|:---|:---|
|`function<T> f`|f是一个用来存储可调用对象的空`function`,这些课调用对象的调用形式应该与`T`相同|
|`function<T> f(nullptr)`|显示构造一个空`function`|
|`function<T> f(obj)`|在f中存储可调用对象`obj`的副本|
|` f`|将f作为条件:当f含有一个可调用对象时为真；否则为假|
|`f(args)`|调用f中的对象，参数时`args`|

**定义为function<T>的成员类型**
|类型|含义|
|:---|:---|
|`result_type`|该function类型的可调用对象返回的类型|
|`argument_type`|当T有一个或者两个实参时定义的类型。T只有一个实参|
|`frist_argument_type`|当T有一个或者两个实参时定义的类型。T只有两个实参,第一个参数|
|`second_argument_type`|当T有一个或者两个实参时定义的类型。T只有一个实参,第二个参数|

使用示例：

```c++
function<int(int,int)> f1=add;  //函数指针

function<int(int,int)> f2=divide();  //函数对象类的指针

function<int(int,int)> f3=[](int i,int l){return i*j;};  //函数指针

cout<<f1(4,2)<<endl;  //打印

cout<<f2(4,2)<<endl;  //打印

cout<<f3(4,2)<<endl;  //打印8

//使用map映射

map<string,function<int(int,int)> > binops={
    {"+",add},  //函数指针

    {"-",std::minus<int>()},  //标准库函数对象

    {"/",divide()},  // 用户定义的函数对象

    {"*",[](int i,int j){return i*j;}},  //未命名的lambda

    {"%",mod} //已命名的lambda对象
};
```
注意：
新版本标准库中的`function`类与旧版本中的`unary_function`和`binary_function`没有关联，后两个类已经被更加通用的`bind`函数代替了。

### 14.9 重载、类型转换和运算符

**类类型转换：** 将实参类型对象隐式转换为类类型，转换构造函数和类型转换运算符共同定义类类型转换，也称作 **用户定义的类类型转换**。

例如：`double b=3.141516;int a=(int)b;`

**类型转换运算符：** 将一个类类型的值转换成其它类型。一般形式如下:
`operator type() const`


注意：

- 一个类型转换函数必须是类的成员函数；它不能声明返回类型，形参类表也必须为空。类型转换函数通常应该是`const`。
- 类型转换运算符可能产生意外结果，例如bool类型转换，能被隐式地转换为int类型输出。
- 为了防止上一条情况发撒恒，c++11定义了显示的类型转换运算符；例如：

```c++
class SmallInt{
public:
    //编译器不会自动执行这一类型转换

    explicit operator int() const {return val;}
}
//显式地请求类型转换

SmallInt si=3;
static_cast<int>(si)+3;
```
- 向bool的类型转换通常用在条件部分，因此`operator bool`一般定义成`explicit`的。
- 通常情况下，不要为类定义相同的类型转换，也不要在类中定义两个及两个以上转换源或者转换目标是算术类型的转换。
- 当我们使用两个用户定义的类型转换时，如果转换函数之前或者之后存在标准类型转换，则标准类型转换将决定最佳匹配到底是哪个
- 除了显式向bool类型的转换为，应该尽量避免定义类型转换函数并尽可能地限制“显然正确”的非显式构造函数。
- 如果在调用重载函数时，需要构造函数或者强制类型转换来改变实参类型，则这通常意味着程序设计存在不足。
- 在调用重载函数时，如果需要额外的标准类型转换，则该转换的级别只有当所有可行函数都请求同一个用户定义的类型转换时才有用。如果所需的用户定义的类型不止一个，则该调用具有二义性。

#### 14.9.3 函数匹配与重载运算符

注意：

- 表达式中运算符的候选函数集即应该包括成员函数，也应该包括非成员函数。
- 如果我们对同一个类既提供了转换目标是算术类型的类型转换，也提供了重载的运算符，则将会遇到运算符与内置运算符符二义性问题。

例如：
```c++
class SmallInt{
    friend SmallInt operator+ (const SmallInt&,const SmallInt&);
public:
    SmallInt(int =0);  // 转换源为int的类型转换

    operator int() const {return val;} //转换目标为int的类型转换

private:
    std::size_t val;
};

SmallInt s1,s2;
SmallInt s3=s1+s2;   //使用重载的operator+

int i=s3+0;  //二义性错误

```
















