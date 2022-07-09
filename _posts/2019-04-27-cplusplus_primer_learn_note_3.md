---
layout:     post
title:      C++ Primer 学习笔记 (三)
subtitle:   C++ Primer 学习记录 (三)
date:       2019-4-25
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - C++
    - 基础编程
---

>2019-4-27 20:17:29 编写笔记如下

## 第五章 语句
没什么好写的。

## 第六章 函数

###  函数基础

在函数中可以使用`static`来进行静态局部变量的全局作用域。使得局部变量的生存周期可以一直持续到程序结束。
注意当使用指针作为函数传入参数的时候，函数内部会拷贝传入参数指针，指针不同但是指向的地址和变量相同。当使用引用时，是传给函数使用对象的另外一个名字。因此在函数参数传递时应该，尽量使用引用参数。如下：

```c++
bool isShorter(const string &sl;const string &s2)
{
    return s1.size()<s2.size();
}

int  main(int argc,char *argv[])
{

}

//第二个形参argv是一个数组，它的元素是指向c风格字符串的指针；因为第二个形参是数组，所以main函数也可以定义成：

int main(int argc, char const **argv) {
    /* code */
    return 0;
}


```


### initialzer_list 形参

当函数的实参数量未知；但是全部实参的类型相同，我们可以使用initializer_list类型的形参。详细描述如下表：

|操作|解释|
|:---|:---|
|`initializer_list <T> lst`|默认初始化；类型的空列表|
|`initializer_list <T> lst {a,b,c...}`|lst的元素数量和初始值一样多；lst的元素是对应初始值的副本；列表中的元素是const|
|`lst2(lst) or lst2=lst`|拷贝复制一个元素|
|`lst.size()`|列表中的元素数量|
|`lst.begin()`|返回指向lst中首元素的指针|
|`lst.end()`|返回指向lst中尾元素下一位置的指针|


下面是代码示例：
```c++
void error_msg(initializer_list<string> il)
{
    for(auto beg=il.begin();beg!=il.end();++beg)
    {
        cout<<*beg<<" ";//连续输出错误的函数信息
    }
    cout<<endl;

}

```

#### 省略符形参

为了方便c++程序访问某些特殊的c代码而设置的，这些大妈使用了名为`varargs`的c标准库功能。

```c++

void foo(parm_list,...);
void foo(...);

```
#### 值是如何被返回的
返回复制一个临时变量，该临时变量就是函数调用的结果。
注意：
1. 不要返回局部对象的引用或者指针，因为局部变量的引用和指针会随着局部变量的结束而终止，因此，返回的引用和指针会不在有效的内存区域内。
2. 函数的返回类型决定函数是否是左值，调用一个返回引用的函数得到左值，其它返回类型得到右值。
3. 数组不能被返回，但是函数可以通过返回数组指针来进行返回操作。
4. c++11新标准，允许使用尾置返回类型；或者使用`decltype`声明返回指针的类型；例如：
    
```c++
    //func 接受一个int类型的实际参数，返回一个指针，该指针指向含有10个整数的数组
    
    auto funct(int i) -> int(*)[10];
    int odd[]={1,3,5,7,9};
    int odd[]={2,4,6,8,10};

    decltype(odd) *arrPtr(int i)
    {
        return (i%2)?&odd:&even;//返回一个指向数组的指针

    }
```

#### 函数重载(overloaded 和over)

_参考链接：_ [C++的重载(overload)与重写(override)](https://www.cnblogs.com/luxiaoxun/archive/2012/08/09/2630751.html)；[C++中 overload 、override、overwrite 之间的区别](https://blog.csdn.net/qq_34793133/article/details/80938099);

因为c++是强类型语言，因此当相同函数名称处理不同的输入数据时需要设置多个函数，实现相同函数名称的查找匹配最佳值。这一点c++中的模板很好的解决了这个问题，但是函数的重载也很好的解决了这个问题。例如：

```c++
//定义重载函数

void print(const char *cp);//函数1

void print(const int *beg,const int *end);//函数2

void print(const int ia[],size_t size);//函数3

//接受参数不同，使用也不相同

int j[2]={0,1};

print("Hello word ");//调用函数1

print(j,end(j),-begin(j));//调用函数2

print(begin(j),end(j));//调用函数3

```
注意：

1. `main`函数不能重载。
2. c++中函数名字查找发生在类型检查之前。因此建议，函数重载，针对不同输入直接取名不同。
3. 当函数传入参数是拷贝传递时`const int a`与`int a`是同一个函数，即重写了前一个函数。当使用`&`作为引用参数时，使用`const`为新参数。例如`int &a`与`const int &a`是两个不同的函数。
4. 注意当查找同名函数时，编译器首先查找当前作用域(局部作用域)内的同名函数。

##### overload 、override、overwrite 之间的区别

##### Overload  重载
在C++程序中，可以将语义、功能相似的几个函数用同一个名字表示，但参数不同（包括类型、顺序不同），即**函数重载**。条件：

- 相同的范围(在同一个类中)
- 函数名字相同
- 参数不同
- 重载解析中不考虑返回类型，而且在不同的作用域里声明的函数也不算是重载。重载可以理解为一个类内部的函数重载

#####  Override 覆盖
是指派生类函数覆盖基类函数，实际上是c++多态的衍生品；特征是：

- 不同的范围(分别位于派生类与基类)
- 函数名字相同；
- 参数名字相同
- 基类函数必须有`virtual`关键字。

示例：

```c++
#include <iostream>
using namespace std;
class base
{
  public:
  virtual void Fun1()
    {
        
        cout<<"Base Fun1..."<<endl;
     }
 
      virtual void Fun2()
     {
         cout<<"Base Fun2..."<<endl;
     }
 
      void Fun3()
    {
        
        cout<<"Base Fun3..."<<endl;
     }
};
 
class Derived:public Base
{
   public:
   void Fun1()
    {
        
        cout<<"Derived Fun1..."<<endl;
     }
 
    void Fun1()
    {
        
        cout<<"Derived Fun2..."<<endl;
     }
 
      void Fun3()
    {
        
        cout<<"Derived Fun3..."<<endl;
     }
};
 
 
int main()
{
  Base* p;
  Derived d;
  p=&d;
 
 p->Fun1();  //因为Fun1是虚函数，所以调p指向的对象的Fun1

 p->Fun2();   //同Fun1
 
 p->Fun3();   //Fun3不是虚函数，所以根据指针的类型，是基类指针，调基类的Fun3
  
return 0;
}
/*
结果：
Derived Fun1...
Derived Fun2...
Base Fun3...
*/
```
#####  overwrite：重定义
是指派生类的函数屏蔽了与其同名的基类函数，规则如下：
    
- 如果派生类的函数与基类的函数同名，但是参数不同。此时，不论有无`virtual`关键字，基类的函数将被隐藏。
- 如果派生类的函数与基类的函数同名，并且参数也相同，但是基类函数没有`virtual`关键字。此时，基类的函数被隐藏（注意别与覆盖混淆)
- 重定义分两种：
    + 对基类数据成员的重定义：不改变基类的数据成员，改变派生类的数据成员。
    + 对基类成员函数的重定义
        * 派生类的成员函数与基类完全相同；基类中的函数被隐藏
        * 派生类的成员函数与语言基类成员函数名相同但参数不同；使用派生类的函数，如果要方位基类方法使用`<class_name>.Base::<function>`或者`<class_name>.Base::<成员变量名>`；


#### 特殊用途语言实参
c++中存在特殊用途的语言实参：

##### 默认实参
函数可以使用默认初始值，这点称为默认实参例如：`string function1(int hz=24,int wid=80,char backgrnd='n');`
注意：

1. 默认实参最好放在头文件中
2. 已经给予初始值的默认实参不得再定义初始值，只能给未定义的给予初始值；重复声明会发生错误。

##### 内联函数和`constexpr`函数

内联函数说明只是向编译器发出的一个请求，编译器可以选择忽略这个请求，在编译时将其替换。可以多次定义

`constexpr`函数用于指示常量表达式。

##### 帮助调试
c++中有许多帮助调试的信息；其中包括`assert`预处理宏；在`<assert>`头文件中定义。`assert(expr)`中判断表达式为假时，函数终止。
同时还存在NDEBUG预处理变量;使用静态预处理变量，说明当前文件信息。

|关键字|作用|
|:---:|:---|
|`__FILE__`|存放文件名字的字符串字面值|
|`__LINE__`|存放当前行号整形字面值|
|`__TIME__`|存放编译时间字符串字面值|
|`__DATE__`|存放编译日期字符串字面值|

例如：

```c++

#include <iostream>
using namespace std;

int main(void)
{
    std::cout<<"function name"<<__func__<<"\n";
    std::cout<<"file name "<<__FILE__<<"\n";
    std::cout<<"line "<<__LINE__<<"\n";
    std::cout<<"time"<<__TIME__<<"\n";
    return 0;
}
/*输出：
function namemain
file name test.cpp
line 8
time20:39:05
*/

```
#### 函数指针
指针的实质是指向内存的地址的一个变量，函数存在于堆栈中，因此指针也可以指向函数，成为函数指针。例如：

```c++

//定义函数function2

bool function2(const string &,const string &);
//定义指针指定输入参数的指针

bool *pf(const string &,const string &);

pf=function2;//将指针pf指向lengthCompare的函数

auto b1=pf("hello","goodbye");//调用函数

auto b2=(*pf)("hello","goodbye");//一个等价的调用

//使用指针函数，方便我们在某些状况下使用指定的重载函数，避免产生隐式转换的错误

void ff(int* )//重载函数1

void ff(unsigned int)//重载函数2

//定义函数指针，并初始化
void (*pf1)(unsigned int )=ff;

```
我们也可以使用函数指针，作为函数返回值，指向一个函数；只要返回类型写成指针形式。使用类型别名可以声明一个返回函数指针的函数。

```c++
using F=int(int*,int);//F是函数类型，不是指针

using PF=int(*)(int *,int);//PF是指针类型


PF f1(int);//正确：PF是指向函数的指针，f1返回指向函数的指针

F f1(int); //错误：F是函数类型，f1不能返回一个函数

F *f1(int)；//正确：显式地指定返回类型是指向函数的指针

```

