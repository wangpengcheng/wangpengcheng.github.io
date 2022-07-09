---
layout:     post
title:      C++ Primer 学习笔记(八)
subtitle:   C++ Primer 学习记录(八)
date:       2019-05-31
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - C++
    - 基础编程
---

## 第15章 面向对象程序设计

### 15.1 oop:概述
面向对象程序设计的核心思想是数据抽象、继承和动态绑定。

**虚函数：** 基类希望它的派生类各自定义适合自身版本，将这些函数声明为**虚函数**；派生类必须通过派生类列表明确指明他是从那个基类继承而来的。即 **基类希望派生类能进行覆盖的函数**

**动态绑定**
通过**动态绑定**，我们能用同一段代码分别处理不同的对象。相同函数，根据动态绑定的对象实质进行区别。在运行时选择函数的版本，所以动态绑定有时又被称为 **运行时绑定**

注意：

- 在c++语言中，当我们使用基类的引用(或者指针)调用一个虚函数时将发生动态绑定。

### 15.2 定义基类和派生类

注意：

- 基类同城都应该定义一个虚析构函数，计时该函数不执行任何实际操作也是如此。
- 如果一个派生类没有覆盖其基类中的某个虚函数，则该函数的行为类似于其他成员，派生类会直接继承其在基类中的版本。
- 同一个对象中，继承自基类的部分和派生类自定义的部分不一定是连续存储的。

```c++
class Quote{
public:
    Quote()=default;
    Quote(const std::string &book,double sales_price):bookNo(book),price(sales_price){}
    std::string isbn() const {return bookNo;}
    //返回给定数量的书籍的销售总额
    
    //派生类负责改写并使用不同的折扣计算算法
    
    virtual double net_price(std::size_t n) const
    {return n*price;}

    virtual ~Quote()=default; //对析构函数进行动态绑定
private:
    std::string bookNo;  //书籍编号

protected:
    double price=0.0   //普通状态下不打折扣的价格 

}

```

可以将派生类的对象当成基类来使用，而且也能将基类的指针或者引用绑定到派生类对象中的基类部分上。

```c++
Quote item;  //基类对象

Bulk_quote bulk;  //派生类对象

Quote *p=&item;  //p指向Quote对象

p=&bulk; //p指向bulk的Quote的部分

Quote &r=bulk;  //r绑定到bulk的Quote部分

```
上述转换可以把派生类对象的指针用在需要基类指针的地方。
注意：

- 在派生类对象中含有与基类对应的组成部分，这一事实是继承的关键。
- 派生类不能直接初始化继承的基类成员，必须使用基类的构造函数来初始化它的基类部分；每个类控制它自己的成员初始化过程。
- 除非我们特别指出，否则派生类的基类部分会像数据成员一样执行默认初始化。如果需要使用基类的构造函数需要，使用`基类名(参数1，参数2)` 的形式进行显式调用。否则进行一般空参数的默认初始化。
- 首先初始化基类的部分，然后按照声明的顺序依次初始化派生类的成员。
-  **c++中类，是在实例化时才会查找相关代码，没有使用就不会生成对应代码，函数无论使用都会生成。**
-  类会自动生成一个`namespace`,其中的静态成员和静态变量，相当于`namespace`中的变量和函数。
-  如果基类定义了一个静态成员，则在整个继承体系中只存在该成员的唯一定义。不论从基类中派生出来多少个派生类，对于每个静态成员来说都只存在唯一的实例。
-  如果我们想要某个类用作基类，则该类函数必须已经定义而非仅仅声明。主要是构造函数和成员变量必须定义，因为子类的构造函数必须使用父类的构造函数。
-  在类后面添加关键字`final`可以有效防止类被继承。
-  和内置指针一样，智能指针类也支持派生类向基类的类型转换，意味着我们可以将一个派生类对象的指针存储在一个基类指针的只能指针内。

**基类和派生类**

不存在基类向派生类的隐式类型转换，但是当编译器无法确定某个特定的转换在运行时是否安全的时候，则可以，但这是很危险的，并且基类函数的析构函数最好是虚析构函数。([C++中虚析构函数的作用](https://www.cnblogs.com/lit10050528/p/3874703.html))

```c++
Bulk_quote bulk;
Quote *itemP=&bulk;  //正确；动态类型是Bulk_quote

Bulk_quote *bulkP=itemP;  //错误，不能将基类转换成派生类

```

派生类向基类的自动类型转换，支队指针或者引用类型有效，在派生类类型和基类类型之间不存在这样的转换。

当我们用一个派生类对象为一个基类对象初始化或赋值时，只有该派生类对象中的基类部分会被拷贝、移动或赋值，它的派生类部分将被忽略掉。

存在继承关系的类型之间的转换规则

- 从派生类向基类的类型转换只对指针或引用类型有效
- 基类向派生类不存在隐式类型转换
- 和任何其他成员一样，派生类向基类的类型转换也可能会由于访问受限而变得不可行。

### 15.3 虚函数

**虚函数的调用可能在运行时才被解析**

注意：

- 一旦某个函数被声明成虚函数，则在所有派生类中它都是虚函数。
- 一个派生类的函数如果覆盖了继承来的虚函数，则它的形参类型必须被它覆盖的基类函数完全一致;返回类型也必须相同。形参列表不同时会产生新的函数，继承的基类函数仍旧有效。
- 使用`override`关键字可以明确重载，原函数中没有函数，或者参数不对应则都会产生错误。
- 如果虚函数使用默认实参，则基类和派生类中定义的默认实参最好一致。

**回避虚函数的机制**

可以使用作用域运算符，实现虚函数的强行绑定，而非动态绑定；例如：

```c++
double undiscounted=baseP->Quote::net_price(42);
//强行调用基类中定义的函数版本而不管baseP的动态类型到底是什么

```

通常情况下，只有成员函数(或者友元)中的代码才需要使用作用域运算符来回避虚函数的机制。通常是一个派生类的虚函数调用它覆盖的基类的虚函数版本时。

注意：如果一个派生类虚函数需要调用它的基类版本，但是没有使用作用域运算符，则在运行时该调用将被解析为对派生类版本自身的调用，从而导致无限递归。

### 15.4 抽象基类

含有纯虚函数的类是抽象基类；不能创建抽象基类的对象，只能被继承

重构：负责重新设计类的体系以便将操作或数据从一个类移动到另外一个类中。

### 15.5 访问控制与继承

protect:希望派生类分享但是不想被其他公共访问使用的成员。

- 受保护的成员对于类的用户来说是不可访问的。
- 受保护的成员，对于派生类的成员和友元来说是可以访问的
- 派生类或友元只能通过派生类对象来访问基类的受保护成员。派生类对于一个基类的受保护成员没有任何访问特权。

```c++
class Base{
protected:
    int prot_mem;  //protected 成员

};
class Sneaky:public Base{
    friend void clobber(Sneaky&);  //能访问Sneaky::prot_mem

    friend void clobber(Base&);  //不能访问Base::prot_mem

    int j;  
}
void clobber(Sneaky& s) {s.j=s.prot_mem=0;} //正确能访问Sneaky对象的private和protected成员

void clobber(Base& b){b.prot_mem=0;}//错误不能访问protected的成员

```
private 不影响派生类的访问权限，主要影响，相关函数的使用。

**派生类向基类转换的可访问性**

- 只有当D公有地继承B时，用户代码才能够使用派生类向基类的转换；如果D继承B的方式是保护的或者私有的，则用户代码不能使用该转换。
- 不论D以什么方式继承B,D的成员函数和友元都能使用派生类向基类的的转换；派生类向其会直接基类的类型转换对于派生类的成员和友元来说是永远可以访问的。
- 如果D继承B的方式是公有的或者受保护的，则D的派生类的成员和友元可以使用D向B的类型转换；反之，如果D继承B的方式是私有的，则不能使用
- 对于代码中的某个给定节点来说，如果基类的公有成员是可以访问的，则派生类向基类的类型转换也是可访问的；反之则不行。

**友元与继承**

友元关系不能继承，友元关系也不能传递，基类的友元在访问派生类成员时，不具有特殊性，类似的，派生类的友元也不能随意访问基类的成员。-- **不能继承友元关系，每个类负责控制各自成员的访问权限**

**改变个别成员的可访问性**

通过`using`声明可以改变派生类继承的某个名字的访问级别。

```c++
class Base{
public:
    std::size_t size() const {return n;}
protected:
    std::size_t;
};

class Derived: private Base{
public:
    using Base::size;  //保持对象尺寸相关的成员的访问级别

protected:
    using Base::n; //使用using关键字改变成员变量的访问级别。

};

```

- `private using` 该名字能被类的成员和友元访问；
- `public using` 类的所有成员都能访问。
- `protectde using` 类的成员、友元和派生类是可访问的。

### 15.6 继承中的类作用域

每个类定义自己的作用域，；当存在继承关系时，派生类的作用域嵌套在其基类的作用域之内。如果一个名字在派生类的作用域内无法正常解析，则编译器将继续在外层的基类作用域中寻找该名字的定义。例如：

```c++
Bulk_quote bulk;
cout<<bulk.isbn();

//查找步骤：先找自身作用域内函数，再找父类，和父类的父类

```
静态类型：在编译时总是已知的，它是变量声明时的类型或表达式生成的类型
动态类型：变量或表达式表示的内存中的对象的类型。

注意：

- 派生类的成员将隐藏同名的基类成员
- 可用通过作用域运算符来使用一个呗隐藏的基类成员


```c++
struct Derived:Base{
    int get_base_mem(){
        return Base::mem;
    }
}

```

- 除了覆盖继承而来的虚函数之外，派生类最好不要中庸其它定义在基类中的名字。

**关键概念:名字夜找与继承**

------

理解函数调用的解析过程刘一于理解C++的继承至关重要，假定我们调用`p->mem()`(或者obj.mem())，则依次执行以下4个步骤:

- 首先确定p(或obj)的静态类型因为我们调用的是一个成员，所以该类型必然是类类型    
- 在p(或。bj)的静态类型对应的类中查找mem如果找不到，则依次在直接基类中不断查找直至到达继承链的顶端如果找遍了该类及其基类仍然找不到，则编译器将报错
- 一旦找到了mem,就进行常规的类型检查(参见6.1节，第183页)以确认对于当前找到的mem，本次调用是否合法
- 假设调用合法，则编译器将根据调用的是否是虚函数而产生不同的代码:      一如果mem是虚函数且我们是通过引用或指针进行的调用，则编译器产生的代          码将在运行时确定到底运行该虚函数的哪个版本，依据是对象的动态类型、      一反之，如果mem不是虚函数或者我们是通过对象(而非引用或指针)进行的          调用，则编译器将产生一个常规函数调用。

```c++
struct Base{      
int  memfcn();
};
struct DerivPd:Base{
    int memfcn(int);  //隐藏基类的memfn

};

Derived d;Base b;

b.memfcn();  //调用Base::memfn

d.memfcn(10);  //调用Derived::memfcn

d.memfcn();   //错误：参数列表为空的memfcn被隐藏了

d.Base::memfcn();  //正确：调用Base::memfcn()

```
**通过基类调用隐藏的虚函数**

```c++
class Base{
public:
    virtual int fcn();
};
class D1:public Base{
public:
    //隐藏基类的fcn,这个fcn不是虚函数
    
    //D1继承了Base::fcn()的定义

    int fcn(int);  //形参列表与Base中的fcn不一致

    virtual void f2();  //是一个新的虚函数，在Base中不存在
};

class D2:public D1{
public:
    int fcn(int);   //一个非虚函数，隐藏了D1::fcn(int)

    int fcn();    //覆盖了Base的虚函数fcn

    void f2();  // 覆盖了D1的虚函数f2
}



Base bobj;
D1 d1obj;
D2 d2obj;

Base *bp1=&bobj,*bp2=&d1obj,*bp3=&d2obj;

bp1->fcn();  //虚调用，将在运行时调用Base::fcn()

bp2->fcn();  //虚调用，将在运行时调用Base::fcn()

bp2->fcn();  //虚调用，将在运行时调用D2::fcn()

Base *pd=&d2obj;
D1 *p2=&d2obj;
D2 *p3=&d2obj;

p1->fcn(42);  //错误：Base中没有接受一个int的fcn

p2->fcn(42);  //静态绑定，调用D1::fcn(int)

p3->fcn(42);  //静态绑定，调用D2::fcn(int)

```

类内using声明的一般规则同样适用于重载函数的名字，基类函数的每个实例在派生类中都必须是可访问的，对派生类没有重新定义的重载版本的访问，实际上是对using 声明点的访问。

### 15.7 构造函数与拷贝控制

虚析构函数将阻止合成移动操作：
如果一个类定义了析构函数，即使它通过`=default`的形式使用了合成的版本，编译器也不会为这个类合成移动操作。

**派生类中删除的拷贝控制与基类的关系**

某此定义基类的万式也可能导致有的派产仁类成员成为被删除的函数：

- 如果基类中的默认构造函数、拷贝构造函数、拷贝赋值运算符或析构函数是被删除的函数或艺不可访问，则派生类中对应的成员将是被删除的，原因是编译器小能使用基类成员来执行派生类对象基类部分的构造、赋值或销毁操作。
- 如果在基类中有一个不可访问或删除掉的析构函数，则派生类中合成的默认拷贝和构造函数将是被删除的，因为编译器无法销毁派生类对象的基类部分。
- 和过去一样，编译器将不会合成一个删除掉的移动操作。当我们使用=default请求一个移动操作时，如果基类中的对应操作是删除的或不可访问的，那么派生类中该函数将是被删除的，原因是派生类对象的基类部分不可移动，如果基类的析构函数是删除或不可访问的，则派牛类的移动构造函数也将是被删除的。

注意：

- 当派生类定义了拷贝或者移动操作时，该操作负责拷贝或移动包括基类部分成员在内的整个对象。
- 在默认情况下，基类默认构造函数初始化派生类对象的基类部分如果我想拷贝(或移动)基类部分，则必须在派生类的构造函数初始值列表中显地使用基类的拷贝(或移动)构造函数。
- 如果构造函数或析构函数调用了某个虚函数，则我们应该执行与构造函数或析构函数所属类型相对应的虚函数版本。
- 构造函数的using 声明不会改变该构造函数的访问级别。
- 基类构造函数的默认实参，并不会被继承。相反，派生类将获得多个继承的构造函数，其中每个构造函数分别省略掉一个含有默认实参的形参。如果一个构造函数接收两个形参后一个由模式实参，则派生类将获得两个构造函数一个函数接受两个形参(没有默认实参)，另外一个构造函数只接受一个形参，它对应于基类中最左侧的没有默认值的那个形参。
- 如果派生类定义的构造函数与基类的构造函数具有相同的参数列表，则该构造函数将不会被继承。定义在派生类中的构造函数将替换继承来的构造函数
- 继承的构造函数不会被作为用户定义的构造函数来使用，如果一个类只含有继承的构造函数，则他也将拥有一个合成的默认构造函数。

### 15.8 容器与继承

**当派生类对象被赋值给基类对象的时候，其中的派生部分将被“切掉”，因此容器和存在继承关系的类型无法兼容；因此最好在容器中放置(智能)指针而非对象**

**模拟拷贝**
给类添加一个虚拷贝函数，该函数将申请一份当前对象的拷贝。处理动态的内存分配

```c++
class  Quote{
public:
    //该虚函数返回当前对象的一份动态分配的拷贝
    
    virtual Quote* clone() const& {return new Quote(*this);} // const& 是对this的修饰,标明这个this是 const引用
    virtual Quote* clone() && {return new Quote(std::move(*this));}  // && 也是对this的修饰,表示由对象this指针生成的一个右值
}

```


## 第16章 模板与泛型编程

通用函数模板实现函数的泛化([函数模板的定义实现分别放在.h和cpp中将会遭遇symbol问题参考](https://blog.csdn.net/Raptor_2017/article/details/77689009))：
```c++
template <typename T>
int compare(const T &v1,const T &v2){
    if(v1<v2) return -1;
    if(v2<v1) return 1;
    return 0; 
}

cout<<compare(1,0)<<endl;// T为int

cout<<compare(vec1,vec2)<<endl;  // T 为vector<int >
```
注意： 在模板定义中模板参列表不能为空

当使用模板的时候，我们指定模板实参，将其绑定到模板参数上。
模板类型参数，可以像内置类型或者类类型说明符一样使用。

```c++
template <typename T,class u> T foo (T* p,u test)
{
    T tmp=*p;  //tmp的类型将是指针p指向的类型
    //...
    
    return tmp;
}
```
**非类型模板参数**

可以在模板中定义非类型模板参数。一个非类型模板参数表示一个值而非一个类型。

```c++
template <unsigned N,unsigned M>

int compare(const char (&p1)[N],const char (&p2)[M])
{
    return strcmp(p1,p2);
}

compare("hi","mom");
//编译器实例化版本

int compare(const char (&p1)[3],const char (&p2)[4]);
```
注意： 

- 非类型模板参数的模板实参必须是常量表达式。
- 模板程序应该尽量减少对实参类型的要求。
- 编译器遇到一个模板定义的时，并不生成代码。只有当我们实例化出模板的一个特定版本时，编译器才会生成代码。
- 保证传递给模板的实参支持模板所要求的操作，以及这些操作在模板中能够正确工作，是调用者的责任。

**inline和constexpr的函数模板**

```c++
//正确：inline说明符跟在模板参数列表之后

template <typename T> inline T min(const T&,const T&);
//错误：inline说明符的位置不正确

inline template <typename T> T min(const T&,const T&);
```
#### 16.1.2 类模板
编译器不能为类模板推断模板参数类型，为了使用类模板，我们必须在模板名后的尖括号中提供额外的信息。用来代替模板参数的模板实参列表。这也决定了使用模板的时候需要我们提供 **显示模板实参**；

注意：

一个类模板的每个实例都形成了一个独立的类。类型`Blob<string>`与任何其他Blob类型都没有关联，也不会对任何其它Blob类型的成员有特殊访问权限。

```c++
//定义模板类

template <typename T> class Blob
{
public:
    typedef T value_type;
    typedef typename std::vector<T>::size_type size_type;
    Blob(std::initializer_list<T> il);
}


//模板类成员

template <typename T>
T& Blob<T>::back()
{
    check(0,"back on empty Blob");
    return data->back();
} 
template <typename T>
T& Blob<T>::operator[](size_type i)
{
    //如果i太大，check会抛出异常，阻止访问一个不存在的元素
    
    check(i,"subscript out of range");
    return (*data)[i];
}

```
默认情况下，一个类模板的成员函数只有当程序用到它时才进行实例化。如果成员函数没有被使用，则它不会被实例化。

**在类模板自己的作用域中，我们可以直接使用模板名而不提供实参**

**在类模板外使用类模板名时，我们并不在类的作用域中，直到遇到类名才表示进入类的作用域**

```c++
template <typename T>
BlobPtr<T> BlobPtr<T>::operator++(int)
{
    //此处无需检查；调用前置递增时会进行检查

    BlobPtr ret=*this;  //保存当前值

    ++*this; //推进一个元素；前置++检查递增是否合法

    return ret;  //返回保存的状态
}

```
在一个类模板的作用域内，我们可以指直接使用模板名而不必指定模板实参。

可以使用typedef来定义模板类的别名。
一个`static`成员函数只有在使用的时候才会实例化。

#### 16.1.3 模板参数
一个模板参数名的可用范围是在其声明之后，至模板声明或定义结束之前。模板参数会隐藏外层作用域中声明的相同的名字。与大多数其他上下文不同，在模板内不能重用模板参数名。

```c++
typedef double A;
template <typename A,typename B> void f(A a,B b)
{
    A tmp=a;// tmp的类型为模板参数A的类型，而非double

    double B; //错误： 重声明模板参数
}
```
**模板声明**

```c++
//声明但不定义compare和Blob

template <typename T> int compare(const T &,const T&)

template <typename T> class Blob;
```
一个特定文件所需要的所有模板的声明通常一起放置在文件开始位置，出现与任何使用这些模板的代码之前

注意：
当我们希望通知班一起一个名字表示类型的时候，必须使用关键字`typename`,而不能使用`class`；

对于static修饰的函数使用函数模板的时候，为了处理模板，编译器必须知道名字是否表示一个类型。默认情况下，c++语言假定通过作用域运算访问的名字不是类型。使用一个模板类型参数的类型成员，就必须显示告诉编译器该名字是一个类型，使用`typename`实现
```c++
template <typename T>
typename T::value_type top(const T& c)
{
    if(!c.empty())
        return c.back();
    else
        retrun typename T::value_type();
}

```
**默认模板实参**
c++11允许为函数和类模板提供默认实参。例如：

```c++
template <typename T,typename F=less<T> >
int compare(const T &v1,const T &v2, F f=F())
{
    if(f(v1,v2)) return -1;
    if(f(v2,v1)) return 1;
    return 0;
}
//compare 有一个默认模板实参 less<T> 和一个默认函数实参F()

template <class T=int> class Numbers{ //T 默认为int
    
public:
        Numbers(T v=0):val(v){}
}

```
#### 16.1.4 成员模板
一个类可以包含本身就是模板的成员函数。这种成员被称为 **成员模板**。成员模板不能是函数。

```c++
class DebugDelete{
public:
    DebugDelete(std::ostream &s=std::cerr):os(s){}
    //与任何函数模板相同，T的类型由编译器推断

    template <typename T> void operator() (T *p) const
    {os<<"deleting unique_ptr"<<std::endl;delete p;}

private:
    std::ostream &os;

}

```
#### 16.1.5 控制实例化
模板使用的时候会通过编译器推断，生成对应类型的函数。当两个或者多个独立编译的源文件使用了相同的模板，并提供了相同的模板参数时，每个文件中就都会有该模板的一个示例，使得相同模板的额外开销特别严重。在新标准中通过 **显示实例化**来避免这种开销。 先声明再实例化，但是`extern` 声明必须出现在任何使用实例化版本的代码之前。[参考链接](https://blog.csdn.net/Raptor_2017/article/details/77689009)

```c++
extern template declaration;  //示例化声明

template declaration;  //实例化定义

//下面的这些模板类型必须在程序其它位置进行实例化

extern template class Blob<string>;
extern template int compare(const int&.const int&);
Blob<string> sa1,sa2;  //示例化会出现在其它位置

Blob<int> a1={0,1,2,3,4,5,6,7,8,9};
Blob<int> a2(a1);  //拷贝构造函数在本文件中实例化

int i=compare(a1[0],a2[0]);  //实例化出现在其它位置

//templateBuild.cc

//实例化文件必须为每个在其它文件中声明为extern的类型和函数提供一个(非extern)的定义

template int compare(const int&,const int&);
template class Blob<string>;  
//实例化类模板的所有成员

```
当编译器遇到一个实例化定义(与声明相对)时，它为其生成代码。需要将生成的.o文件链接到一起。

注意： 
- 对每个实例化声明，在程序中某个位置必须有其显示的实例化定义。
- 一个类模板的示例化定义会实例化该模板的所有成员，包括内联的成员函数。
- 在一个类模板的实例化定义中，所用类型必须能用于模板的所有成员函数。

### 16.2 模板实参推断

编译器利用调用中的函数实参来确定其模板参数的过程被称为 **模板实参推断**。在模板实参推断过程中，编译器使用函数调用中的实参类型来寻找模板实参，用这些模板实参生成的函数版本与给定的函数调用最为匹配。

- 顶层`const`无论是在形参还是实参中，都会被忽略。
- const转换：可以将一个非const对象的引用（或指针）传递给一个const的引用(或指针)形参。
- 数组或函数指针转换：如果函数形参不是引用来兴，则可以对数组或者函数类型的实惨应用正常的指针转换。一个数组实参可以转化为一个指向其首元素的指针。类似的，一个函数实参可以转换为一个该函数类型的指针。

**不能应用与函数模板的转换**

- 算术转换
- 派生类向基类的转换
- 用户定义的转换
使用示例：

```c++
template <typename T> T fobj(T,T); //实参拷贝

template <typename T> T fref(const T&,const T&); //引用

string s1("a value");

const string s2("nihao ");
fobj(s1,s2); //调用fobj(string，string)；const 被忽略

fref(s1,s2);//调用fref(const string&,const string&) 将s1转换为const是允许的

int a[10],b[42];

fobj(a,b);  //调用 f(int*,int*)

fref(a,b);  //错误：数组类型不匹配；如果形参是一个引用，则数组不会转换为指针，大小不匹配因此不合法。

```

注意： 
- 将实参传递给带模板类型的函数形参时，能够自动应用的类型转换只有const转换及数组或函数到指针的转换。
- 如果函数参数类型不是模板参数，则对实参进行正常的类型转换。

#### 16.2.2 函数模板显示实参

**指定显式模板实参**

定义返回类型的第三个模板参数，从而允许用户控制返回类型：

```c++

template <typename T1,typename T2,typename T3>
T1 sum(T2,T3); 
//编译器无法推断T1，它未出现在函数参数列表中

//显示指定T1帮助实参进行类型推断。

auto val3=sum<long long>(i,lng); // long long sum(int ,long)

//用户必须指定所有三个模板参数

template <typename T1,typename T2, typename T3>
T3 alternative_sum(T2,T1);
//错误：不能推断前几个模板参数

auto val3=alternative_sum<long long>(i,lng);
//正确： 显示指定了所有三个参数

auto val2=alternative_sum<long long ,int,long>(i,lng);


```
**显式模板实参按照由左至右的顺序与对应的参数模板匹配**

#### 16.2.3 尾置返回类型与类型转换

尾置返回允许我们在参数累彪之后声明返回类型；编译器会根据实例化的输入，动态判断返回类型。
```c++
template <typename It>
auto fcn(It beg,It end)->decltype(*beg)
{
    //处理序列

    return *beg; // 返回序列中一个元素的引用
}

template <typename It>
auto fcn2(It beg,It end)->
    typename remove_reference<decltype(*beg)>::type //获取元素类型

    //decltype(*beg)返回元素类型的引用类型
    
    //remove_reference::type 脱去引用，剩下元素类型本身

{
    //处理序列

    return *beg; // 返回序列中一个元素的拷贝
}

```
**标准类型转换模板**

|对`Mod<T>`,其中`Mod`为|若`T`为|则`Mod<T>::type`为|
|:---|:---|:---|
|`remove_reference`|`X&`或`X&&`|`X`|
||否则|`T`|
|`add_const`|`X&`或`const X`或函数|`T`|
||否则|`const T`|
|`add_lvalue_reference`|`X&`|`T`|
||`X&&`|`X&`|
||否则|`T&`|
|`add_rvalue_reference`|`X&`或`X&&`|`T`|
||否则|`T&&`|
|`remove_pointer`|`X*`|`X`|
||否则|`T`|
|`add_pointer`|`X&`或`X&&`|`X*`|
||否则|`T*`|
|`make_signed`|`unsigned X` |`X`|
||否则|`T`|
|`make_unsigned`|带符号类型|`unsigned X`|
||否则|`T`|
|`remove_extent`|`X[n]`|`X`|
||否则|`T`|
|`remove_all_extent`|`X[n1][n2]...`|`X`|
||否则|`T`|

#### 16.2.4 函数指针和实参推断
可以使用指针指向模板函数的实例;
```
template <typename T> int compare(const T&,const T&);
//指针pf1指向实例 int compare(const int&, const int&)

int (*pf1)(const int& ,const int&)=compare
//pf1中参数的类型决定了T的模板实参的类型


```

当参数是一个函数模板实例的地址时，程序上下文必须满足对每个模板参数，能唯一确定其类型或值。

#### 16.2.5 模板实参推断和引用

**从左值引用函数参数推断类型**
```c++

template <typename T> void f1(T&); //实参必须是一个左值
//实参类型为模板参数类型

f1(i); // i是int T是int

f1(ci);//ci 是const int;模板参数T是const int

f1(5); // 错误传递给&参数的实参必须是一个左值 

template <typename T> void f2(const T&); // 可以接受一个右值

//f2中的参数是const &;实参中的const 是无关的。

//在每个调用中，f2的函数参数都被推断为 const int&

f2(i);  //模板参数是int

f2(ci); // 模板参数是 int 

f2(5); //一个const &参数可以绑定到一个右值；T是int

```

**从右值引用函数参数推断类型**

类型推断过程类似普通左值引用函数参数的推断过程。

**引用折叠和右值引用参数**

- 当文将一个左值传递给函数的右值引用参数，且此右值引用指向模板类型参数(T&&)时，编译器推断模板类型参数为实参的左值引用类型。因此，当使用f3(i)时，编译器推断T的类型为int&,而非int
- 如果我们间接创建一个引用的引用，则这些引用形成了“折叠”。在所有情况下，引用会折叠成一个普通的左值引用类型。
- 特殊情况下，引用会折叠成右值引用：右值引用的右值引用。即，对于一个给定类型X:
    + X& &、X& &&和X&& &都折叠成类型X&
    + 类型X&& && 折叠成X&&
- 引用折叠只能应用于间接创建的引用的引用，如类型别名或模板参数。

折叠规则和右值引用的特殊类型推断规则结合在一起的时候，可以左值调用`f3`,编译器推断`T`为一个左值引用类型：

```c++
f3(i); //实参是一个左值；模板参数T是 int&

f3(ci); //实参是一个左值；模板参数T是一个const int&

void f3<int&>(int& &&); //当T是int&时，函数参数为int& &&

void f3<int&>(int&); //当T是int&时，函数参数折叠为 int& 

```
结果：

- 如果一个函数参数是一个指向模板类型参数的右值引用(如，T&&),则它可以被绑定到一个左值；可以传递给它任意类型的实参。
- 如果一个左值传递给这样的参数，则函数参数被示例化为一个普通的左值引用(T&)。 

接受右值引用参数的模板函数
```c++
template <typename T> void f3(T&& val)
{
    T t=val; //右值调用f3，f3(42),T为int ;左值i调用f3，T为int&；t类型为 int& .t的初始化值绑定到了val，对t赋值时，也改变了val的值。在下面的判断中永远得到true

    t=fcn(t);//当T为右值时，职高部t，当T为左值时，都改变

    if(val==t){
        /*
        若T是引用类型，则一直为true
        */

    }

}

template <typename T> void f(T&&);  //绑定到非const右值

template <typename T> void f(const T&);  //左值和const右值

```
#### 16.2.6 理解std::move

std::move的定义：

```c++

template <typename T>

typename remove_reference<T>::type&& move(T&& t)
{
    return static_cast<typename remove_reference<T>::type&& >(t);
}
// move的函数参数T&&是一个指向模板类型参数的右值引用。通过引用折叠，参数可以与任何类型的实参匹配。

//接下来使用remove_reference获取T的真实类型

//最后使用静态指针变量使得获得指针的右值引用。

```

#### 16.2.7 转发
- 如果一个函数参数是指向模板类型参数的右值引用(如T&&),它对应的实参的const属性和左值/右值属性将得到保持。
- 当用于一个指向模板参数类型的右值引用函数参数(T&&)时，forward会保持实参类型的所有细节。
- 与`std::move`相同，对`std::forward`不使用`using`声明是一个号主意。

对于模板函数，尽量使用右值引用，避免被使用时，末班类型的推导失去`&`符号，造成拷贝使用。相关参数不能使用。

### 16.3 重载与模板

函数匹配规则与影响：

- 对于一个调用，其候选函数包括所有模板实参推断成功的函数模板实例。
- 候选的函数模板总是可行的，因为模板实参推断会排除任何不可行的模板
- 可行函数(模板与非模板)按照类型转换来排序；可以用于函数模板调用的类型转换是非常有限的。
- 如果存在多个匹配函数则：
    + 同样号的函数中只有一个是非模板函数，选择此函数
    + 没有非模板函数，其中一个模板比其它模板更特例话，则选择此模板
    + 不符合上述两条规则，此调用有歧义。

注意： 正确定义一组重载的函数模板需要对类型间的关系及模板函数允许的有限的实参类型转换有深刻的理解。

**编写重载模板**

例如:`debug_rep(const T&)`;本质上可以用于任何类型，包括指针类型。

注意：
- 当有多个重载模板对一个调用提供童颜好的匹配时，应该选择最特例化的版本。
- 对于一个调用，如果一个非函数模板与一个函数模板提供同样好的匹配，则选择非模板版本。

对于`cout<<debug_rep("hi word!")<<endl;`而言有：

- `debug_rep(const T&)` ,T被绑定到`char[10]`。
- `debug_rep(T*)` ,T被绑定到`const char`。
- `debug_rep(const string&)` ,要求从`const char*`到`string`的类型转换。

`T*`版本更加特例化，编译器会选择它。

注意:
在定义任何函数之前，记得声明所有重载的函数版本。这样就不必担心编译器由于未遇到你希望调用的函数而实例化一个非你所需要的版本。

### 16.4 可变参数模板

**可变参数模板**：接受一个可变数目参数的模板函数或模板类。可变数目的参数被称为 **参数包**。

参数包分为两种：

- 模板参数包：表示0或者对个模板参数。
- 函数参数包：表示0或者多个函数参数。

使用`class...`或者`typename...`指出接下来的参数表示0个或者多个类型的列表；一个类型名后面跟一个省略号表示0或者多个给定类型的非类型参数的列表。例如：

```c++
//Args 是一个模板参数包；rest 是一个函数参数包

//Args 表示0个或多个模板类型参数

//rest 表示0个或者多个函数参数

template <typename T,typename... Args>
void foo(const T &t,const Args& ... rest);
//使用

int i=0;
double d=3.14;
string s="how now brown cow";
foo(i,s,42,d);//包中有三个参数

foo(s,42,"hi");//包中有两个参数

foo(d,s); //包中有一个参数

foo("hi"); //空包

//编译器会分别为他们实例化不同的版本。

//可以使用sizeof...运算符，计算包中有多少个元素

template<typename ... Args> void g(Args ...args){
    cout<<sizeof...(Args)<<endl; //类型参数的数目

    cout<<sizeof...(args)<<endl; //函数参数的数目
}

```
##### 16.4.1 编写可变参函数模板

可变参函数通常是递归的。第一步调用处理包中的第一个实参，然后用剩余实参调用自身。我们的print函数也是这样的模式，每次递归调用将第二个实参打印到第一个实参表示的流中。为了终止递归，我们还需要定义一个非可变参数的print 函数，它接受一个流和一个对象。

```c++
template <typename T>
//最后一次会优先调用它

ostream &print(ostream &os,const T &t)
{
    return os<<t; //包中最后一个元素之后不打印分隔符

}

//包中除了最后一个元素之外的其它元素都会调用这个版本的print

template <typename T,typename... Args>
ostream &print(ostream &os,const T &t,const Args&... rest)
{
    os<<t<<","; //打印第一个实参

    return print(os,rest...);//递归调用，打印其它实参

}
```
当定义可变参数版本的`print`时，非可变参数版本的声明必须在作用域中。否则，可变参数版本会无限递归。

#### 16.4.2 包扩展

包括展：将它分解为构成的元素，对每个元素应用模式，获得扩展后的列表。

```c++
template <typename T,typename... Args>
ostream &print(ostream &os,const T &t,const Args&... rest)//扩展Args

{
    os<<t<<",";
    return print(os,rest...);  //扩展rest

}


```
注意：

扩展中的模式会独立地应用与包中的每个元素。

##### 16.4.3 转发参数包
_参考链接：_ [std::forward](https://zh.cppreference.com/w/cpp/utility/forward);[c++11 完美转发 std::forward()](https://blog.csdn.net/coolwriter/article/details/80970718); [C++11 std::move和std::forward](https://blog.csdn.net/guoxiaojie_415/article/details/79902278)

使用可变参数模板与`forward`机制来编写函数，实现将实参不变地传递给其它函数。使用示例如下：
```c++
class StrVec{
public:
    template <class... Args> void emplace_back(Args&&...);
    ...
};
//函数实现

template <class... Args>
inline void StrVec::emplace_back(Args&&... args)
{
    chk_n_alloc(); //如果需要的话，重新分配StrVec内存空间

    alloc.construct(frist_free++,std::forward<Args>(args)...);
}

```

使用扩展`std::forward<Args>(args)...`它即扩展了模板参数包`Args`，也扩展了函数参数包`args`。生成如下形式元素：
    
    `std::forward<Ti>(ti)`

对于`std::forward<Ti>(ti)`;其中Ti表示模板参数包中第i个元素的类型，ti表示函数参数包中第i个元素。例如：
```c++
svec.emplace_back(10,'c'); //将c*10添加为新的尾部元素

```
construct调用中的模式会扩展出：
`std::forward<int>(10),std::forward<char>(c)`。

### 16.5 模板特例化

特例化版本就是模板的一个独立定义，在其中一个或多个模板参数被指定为特定的类型。
**定义函模板特例话**

特例话一个模板时，必须为原模板中的每个模板参数都提供实参。为了指出我们正在实例化一个模板，应使用关键字`template`后跟一个空尖括号对(`<>``)。指出我们将为原模板的所有模板参数提供实参：

```c++
//compare 的特殊版本，处理字符串数组的指针

template <>
int compare(const char* const &p1, const char* const &p2)
{
    return strcmp(p1,p2);
}
```
**在使用特例化版本时，函数参数类型必须与一个先前声明的模板中对应的类型匹配。**

```c++
template <typename T> int compare(const T&,const T&);
```
特例化版本的本质上是一个实例化模板，而非重载它。因此，特例化不影响函数匹配。
特例化版本主要是为了，提供特殊函数优先级，方便在使用函数时，优先搜索。
**类模板特例化**
示例：
```c++
namespace std{
    template <> //定义特例化版本，模板参数为Sales_data

    struct hash<Sales_data> //特例化模板名为hash

    {
        ...
    };
}

```

注意：特例化版本应该尽量在头文件中事先定义。

**类模板部分特例化**

我们只能部分特例化类模板函数，而不能部分特例化函数模板。

```c++
//原始的最通用版本

template <class T> struct remove_reference
{
    typedef T type;
};
// 部分特例化版本，将用于左值引用和右值引用

template <class T> struct remove_reference<T&> //左值引用

{
    typedef T type;
};

template <class T> struct remove_reference<T&&> //右值引用

{
    typedef T type;    
};

//使用

int i;
//decltype(42)为int,使用原始模板

remove_reference<decltype(42)>::type a;
//decltype(i)为int&,使用第一个(T&)部分特例化版本

remove_reference<decltype(i)>::type b;
//decltype(std::move(i))为int&& ,使用第二个(即 T&&)部分特例化版本

remove_reference<decltype(std::move(i)>::type c;

```
**特例化成员**

```c++
template <typename T> struct Foo
{
    Foo(const T &t=T()):mem(t) { }
    void Bar() {/*...*/}

    T mem;

    ...
    
};
//特例化模板函数

template <>
void Foo<int>::Bar()
{
    //进行应用于int的特例化处理

}
//实例化操作

Foo<string > fs; //实例化Foo<string>::Foo()

fs.Bar();  //实例化 Foo<string>::Bar()

Foo<int> fi; //实例化 Foo<string>::Foo()

fi.Bar();  //使用我们特例化版本的Foo<int>::Bar()



```
