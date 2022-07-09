---
layout:     post
title:      C++ Primer 学习笔记 (四)
subtitle:   C++ Primer 学习记录 (四)
date:       2019-5-4
author:     敬方
header-img: img/post-bg-miui6.jpg
catalog: true
tags:
    - C++
    - 基础编程
---
> 2019-5-4: 16:03

## 类

### 定义抽象数据
c++中类的实质就是数据的抽象实现和封装。抽象数据依赖于接口和实现，分离的编程技术；**定义在函数内部的函数是隐式的inline函数**

#### 类中的this关键字
_参考链接：_ [C++类中this指针的理解](https://www.cnblogs.com/liushui-sky/p/5802981.html); [C++ this 指针](https://www.runoob.com/cplusplus/cpp-this-pointer.html);[关于this指针](https://blog.csdn.net/shidantong/article/details/80104710);

首先声明`this`指针是c++为了方便管理类中的函数而产生的，每个对象的`this`指针都指向对象本身，可以使用对象的非静态方法。`this`指针是编译器给予的每个成员函数的隐含参数，用来指向调用对象。
注意：

1. 友元函数没有 this 指针，因为友元不是类的成员。只有成员函数才有 this 指针。
2. this指针其本身的内容是不能被改变的，其类型为：**类类型 * const**
3. this指针不是对象本身的一部分，不影响该对象的大小
4. this指针的作用域在类的 **非静态成员函数**的内部，只能在其内部进行使用，其他任何函数都不能，静态成员函数内部无this指针，后面会详述。
5.  this指针是类中非静态成员函数的第一个默认隐含参数，编译器自动传递和维护，用户不可显示传递
6.  this指针可以为空但是类内部的成员变量数据不能为空，因此当调用使用成员变量时，请确保成员变量不为空。

函数调用约定如下：

![函数调用约定表](https://img-blog.csdn.net/20180426163408707?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaWRhbnRvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

_thiscall关键调用：

- 它只能用在类的成员函数上
- 参数从右向左进行压栈
- 若参数个数确定，this指针通过ecx寄存器传递给被调用者；若参数不确定，this指针在所有参数被压栈后压入堆栈
- 对于参数不确定的函数，调用者清理堆栈，否则函数自己清理堆栈。

##### 构造函数
构造函数主要是c++中对于类对象的实例化的函数,这里需要强调一下c++中`new A`、`new A()`与`new A[]`的区别；

_参考链接：_ [C++中new 一个对象的时候加括号和不加括号的区别](https://blog.csdn.net/ywending/article/details/51096547);[C++ new A 和 new A() 的区别详解以及引申的各种初始化类型](https://blog.csdn.net/qq_34228570/article/details/79549958);

先说结论：

-  加括号调用没有参数的构造函数，不加括号调用默认构造函数或唯一的构造函数，看需求。
-  **C++在new时的初始化的规律可能为：对于有构造函数的类，不论有没有括号，都用构造函数进行初始化；如果没有构造函数，则不加括号的new只分配内存空间，不进行内存的初始化，而加了括号的new会在分配内存的同时初始化为0。**

POD(Plain old data):它是一个struct或者类，且不包含构造函数、析构函数以及虚函数。POD class没有用户定义的析构函数、拷贝构造函数和非静态的非POD类型的数据成员。而且，POD class必须是一个aggregate，没有用户定义的构造函数，没有私有的或者保护的非静态数据，没有基类或虚函数。它只是一些字段值的集合，没有使用任何封装以及多态特性。

对于`[new] T [object] {};`初始化一般有三种处理方式：

- 如果T有用户定义的缺省构造函数，直接调用；
- 如果T有编译器生成的缺省构造函数，先0值初始化再调用；
- 如果T根本不是类，直接0值初始化。

对于`[new] T object;`如果T是非class类型，则给出非确定值（不赋值），比如：`int i; double d; bool b;`；
0值初始化也单独作用于静态(或者线程局部）变量：`static T object;`

五种初始化类型：

- list initialization (since C++11) 
- aggregate initialization 这是list initialization对aggregate类型的特例 
- value initialization 值初始化 
- default initialization 缺省初始化 
- zero initialization 0值初始化

示例代码：

```c++
#include <iostream>  

using namespace std;

class A { public:int m; }; // POD  

class B { public:~B() {}; int m; }; // non-POD, compiler generated default ctor  

class C { public:C() :m() {}; ~C() {}; int m; }; // non-POD, list-initialising m

class D { public:D() {}; ~D() {}; int m; };  // non-POD，default-initialising m

int main()
{
    A *aObj1 = new A;
    A *aObj2 = new A();
    cout << aObj1->m << endl;
    cout << aObj2->m << endl;

    B *bObj1 = new B;
    B *bObj2 = new B();
    cout << bObj1->m << endl;
    cout << bObj2->m << endl;

    C *cObj1 = new C;
    C *cObj2 = new C();
    cout << cObj1->m << endl;
    cout << cObj2->m << endl;

    D *dObj1 = new D;
    D *dObj2 = new D();
    cout << dObj1->m << endl;
    cout << dObj2->m << endl;

    delete aObj1;
    delete aObj2;
    delete bObj1;
    delete bObj2;
    delete cObj1;
    delete cObj2;

    getchar();
    return 0;
}
/*result


-842150451
0
-842150451
0
0
0
-842150451
-842150451
注意不同编译器可能产生不同的结果，GCC中结果：
0
0
38158368
0
0
0
38158400
0
结论（以GCC为准）：
new A：0

new A()：0

new B：不确定的值

new B()：0

new C：0

new C()：0

new D：不确定的值

new D()：0

*/


```

注意：
1. 只有当类没有声明任何构造函数时，编译器才会自动地生成默认构造函数。
2. 可以在构造函数之后添加`=default`来要求编译器生成默认构造函数。

### 访问控制与封装
访问说明符`public`、`private`、`protect`来加强类的封装性。注意`struct`关键字中定义第一个访问说明符之前的成员是`public`，如果使用`class`关键字，成员是`private`的。因此当希望所有的类成员是`public`时使用`struct`；希望是`private`时使用`class`。

**友元(friend)**:类允许其他类或者函数访问它的非公有成员，将函数成为友元只需要增加一条`friend`关键字就可以了。注意友元函数，必须在友元声明之外再专门对函数进行一次声明。

定义类型的成员必须先定义后使用例如：

```c++
class Screen
{
public:
    //使用类型别名等价地声明一个类型名字

    typedef std::string::size_type pos;
    Screen();
    ~Screen();
private:
    pos cursor=0;
    pos height=0,width=0;
    std::string contents;
    
};
```
类的友元使用示例如下

```c++
//screen.cpp

class Screen
{
public:
    //使用类型别名等价地声明一个类型名字

    typedef std::string::size_type pos;
    Screen();
    ~Screen();
    //定义友元

    //Windows类可以访问Screen类的私有部分

    friend class Windows;
    //成员函数作为友元

    friend void Windows::clear(ScreenIndex);

private:
    pos cursor=0;
    pos height=0,width=0;
    std::string contents;
    
}

//windows.cpp

class Windows
{
public:
    //窗口中每个屏幕的编号

    using ScreenIndex=std::vector<Screen>::size_type;
    //按照编号将指定的Screen重置为空白
private:
    
    std::vector<Screen> screen{screen(24,80,'')};

public:
    void clear(ScreenIndex i)
    {
        //s是一个Screen的引用，指向我们想清空的那个平米

        Screen &s=screen[i];

        //将那个选定的Screen重置为空白

        s.contents=string(s.height*s.width,'');
    }
}

```
注意：

1. 友元的函数未声明直接出现时，我们隐式的假定该名字在当前作用域中是可见的，要想正常使用，该函数必须先被定义过。


**`mutable`**关键字： 类成员变量使用`mutable`关键字即便是`const`对象也能更改类的成员变量。

类的成员函数后添加`const`关键字，表示不会修改类中的成员变量；并且有一下几点规则：

1. 在类中被const声明的成员函数只能访问const成员函数，而非const函数可以访问任意的成员函数，包括const成员函数
2. 在类中被const声明的成员函数不可以修改对象的数据，不管对象是否具有const性质，它在编译时，以是否修改成员数据为依据，进行检查。
3. 加上mutable修饰符的数据成员，对于任何情况下通过任何手段都可修改，自然此时的const成员函数是可以修改它的

**向前声明：** 仅仅声明类而不去定义它。对于类而言，当类的名字出现过后，它就被认为是声明过了（但是尚未定义），因此类允许包含指向它自身类型的引用或者指针。例如：

```c++
class Link_screen
{
    Screen windows;
    Link_screen *next;
    Link_screen *prev;
}

```

**类中普通块作用域的名字查找**

- 首先，成员函数内查找该名字的声明，只有在函数使用之前出现的声明才被考虑。
- 如果在成员函数内没有找到，则在类内继续查找，这时类的所有成员都可以被考虑
- 如果类内也没有找到该名字的声明，在成员函数定义之前的作用域内继续查找。
- 类作用域之后，在外围的作用域中查找；如果编译器在函数和类的作用域中都没有找到名字，它将接着在外围的作用域中查找

### 构造函数再探

如果成员是是const、引用，或者属于某种未提供默认构造函数的类类型，我们必须通过构造函数初始值列表为这些成员提供初值。最好令构造函数初始值的顺序与成员声明的顺序保持一致；而且如果有可能的话，尽量避免使用某些成员变量初始化其他成员。

注意：成员的初始化顺序与他们在类中定义的出现顺序一致；第一个成员先被初始化，然后第二个

```c++
class X
{
    int i;
    int j;
public:
    X(int val):j(val),i(j){}

    //错误：i在j之前被初始化，因此初始化i之前发生错误，j未定义。
}

```

#### 委托构造函数

C++中可以使用委托构造函数，即预定义一个基础的构造函数其它的构造函数可以调用这个构造函数。

#### 隐式的类类型转换与转换避免。
c++语言之间存在隐式的转换规则；构造函数实际上也定义了转换为此类类型的隐式转换机制。我们将这种构造函数称之为 **转换构造函数**；隐式转换，相当于构造了一个临时的变量对象。再给类一个临时成员变量。但是这种转换只允许一步类型转换，并且类型转换并不是总有效。
为了防止这种状况的发生，我们在单参数构造函数前面添加`explicit`关键字使得构造函数只能直接初始化。但是使用`static_cast`强制类型转化可以使用带有`explicit`关键字的构造函数。

#### 聚合类
聚合类中可以使得用户直接访问其成员，并且具有特殊的初始化语法形式。聚合类的特征：

- 所有成员都是public的
- 没有定义任何构造函数
- 没有类内初始值
- 没有基类，也没有`virtual`函数

例如：

```c++
struct Data
{
    int ival;
    string a;
};
//这就是一个聚合类
```

注意：显示地初始化类的对象的成员存在三个明显的缺点

- 要求类的所有成员都是`public`的。
- 将正确初始化每个对象的每个成员的重任交给了类的用户。因为用户很容易忘掉某个初始值，或者提供一个不恰当的初始值，所以这样的初始化过程冗长乏味且容易出错。
- 添加或者删除一个成员之后，所有的初始化语句都需要重新更新。

#### 字面常量类

要求：

- 数据成员都必须是字面类型值
- 类必须至少含有一个`constexpr`构造函数
- 如果一个数据成员含有类内初始值，则内置类型成员的初始值必须是一条常量表达式；如果成员属于某种类型，则初始值必须使用成员自己的`constexpr`构造函数。
- 类必须使用析构函数的默认定义，该成员负责销毁类的对象。

#### `constexpr`构造函数

首先需要明确构造函数不能是`const`的，因为const修饰函数表示该函数的返回值是const类型的，改返回值只能赋值给同类型的const变量，不符合类的构造函数的需求。const是可以修饰类的成员函数，但是该函数不能修改数据成员。构造函数也属于类的成员函数，但是构造函数是要修改类的成员变量，所以类的构造函数不能申明成const类型的。

但是字面常量类的构造函数时可以`constexpr`的函数，实际上一个字面值常量类必须至少提供一个`constexpr`构造函数。并且该构造函数一般是空的。因为它必须符合构造函数的要求--不能存在返回值；又符合`constexpr`函数要求--能拥有的唯一可执行语句就是返回语句。示例如下;

```c++
class Debug {
private:
    /* data */
public:
    constexpr Debug(bool b=true):hw(b),io(b),other(b){};
    constexpr Debug(bool h,bool i,bool o):hw(h),io(i),other(o){};
    constexpr bool any(){return hw || io || other;}
    //.....
};

```

#### 类的静态成员

##### c++中的内存分配
_参考链接：_ [C/C++程序内存的分配](https://blog.csdn.net/cherrydreamsover/article/details/81627855)；[【C++】动态内存分配详解（new/new[]和delete/delete[]）](https://blog.csdn.net/qq_40416052/article/details/82493916);

首先c/c++中内存分为以下几种类型

- 栈(stack):由编译器自动分配与释放，存放为运行时函数分配的局部变量、函数参数、返回数据、返回地址等。其操作类似于数据结构中的栈。
- 堆区（heap）：一般由程序员自动分配，如果程序员没有释放，程序结束时可能有OS回收。其分配类似于链表。
- 全局区（静态区static）：存放全局变量、静态数据、常量。程序结束后由系统释放。全局区分为已初始化全局区（data）和未初始化全局区（bss）。
- 常量区（文字常量区）：存放常量字符串，程序结束后有系统释放。
- 代码区：存放函数体（类成员函数和全局区）的二进制代码。

具体的内存分配如下：

![程序内存分配](https://img-blog.csdn.net/20180813110942795?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NoZXJyeWRyZWFtc292ZXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70);

注意：栈的地址由高到低，堆的地址由低到高。

**内存分配的方式**

1. 从静态存储区分配：内存在程序编译的时候已经分配好，这块内存在程序的整个运行期间都存在。例如全局变量，static变量。
2. 在栈上创建：在执行函数时，函数内局部变量的存储单元可以在栈上创建，函数执行结束时，这些内存单元会自动被释放。栈内存分配运算内置于处理器的指令集，效率高，但是分配的内存容量有限。
3. 从堆上分配： 亦称为动态内存分配。
 - 程序在运行的时候使用malloc或者new申请任意多少的内存，程序员自己负责在何时用free或delete释放内存。
 - 动态内存的生命周期有程序员决定，使用非常灵活，但如果在堆上分配了空间，既有责任回收它，否则运行的程序会出现内存泄漏，频繁的分配和释放不同大小的堆空间将会产生内存碎片。
4. 在 C 语言中，全局变量又分为初始化的和未初始化的（未被初始化的对象存储区可以通过 void* 来访问和操纵，程序结束后由系统自行释放），在 C++ 里面没有这个区分了，他们共同占用同一块内存区。

**堆和栈的区别**

1. **管理方式不同**：栈是由编译器自动申请和释放空间，堆是需要程序员手动申请和释放；
2. **空间大小不同**：栈的空间是有限的，在32位平台下，VC6下默认为1M，堆最大可以到4G；
3. **能否产生碎片**：栈和数据结构中的栈原理相同，在弹出一个元素之前，上一个已经弹出了，不会产生碎片，如果不停地调用malloc、free对造成内存碎片很多；
4. **生长方向不同**：堆生长方向是向上的，也就是向着内存地址增加的方向，栈刚好相反，向着内存减小的方向生长。
5. **分配方式不同**：堆都是动态分配的，没有静态分配的堆。栈有静态分配和动态分配。静态分配是编译器完成的，比如局部变量的分配。动态分配由 malloc 函数进行分配，但是栈的动态分配和堆是不同的，它的动态分配是由编译器进行释放，无需我们手工实现。
6. **分配效率不同**： 栈的效率比堆高很多。栈是机器系统提供的数据结构，计算机在底层提供栈的支持，分配专门的寄存器来存放栈的地址，压栈出栈都有相应的指令，因此比较快。堆是由库函数提供的，机制很复杂，库函数会按照一定的算法进行搜索内存，因此比较慢。

**静态全局变量、全局变量区别**

- 静态全局变量和全局变量都属于常量区 
- 静态全局区只在本文件中有效，别的文件想调用该变量，是调不了的，而全局变量在别的文件中可以调用。
- 如果别的文件中定义了一个该全局变量相同的变量名，是会出错的。

**静态局部变量、局部变量的区别**

- 静态局部变量是属于常量区的，而函数内部的局部变量属于栈区
- 静态局部变量在该函数调用结束时，不会销毁，而是随整个程序结束而结束，但是别的函数调用不了该变量，局部变量随该函数的结束而结束； 
- 如果定义这两个变量的时候没有初始值时，静态局部变量会自动定义为0，而局部变量就是一个随机值
- 静态局部变量在编译期间只赋值一次，以后每次函数调用时，不在赋值，调用上次的函数调用结束时的值。局部变量在调用期间，每调用一次，赋一次值。

通过以上基础知识我们知道c++的静态成员都是直接在静态全局区域分配内存。因此在编译时就已经内存就已经开始分配成功了，因此不需要实例化类并且解引用，可以直接使用。



