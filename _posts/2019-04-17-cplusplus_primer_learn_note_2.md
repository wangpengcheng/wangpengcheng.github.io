---
layout:     post
title:      C++ Primer 学习笔记 (二)
subtitle:   C++ Primer 学习记录 (二)
date:       2019-4-25
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - C++
    - 基础编程
---

>2019-4-25 12:31:29 编写笔记如下

## 第三章 字符串、向量和数组

### 使用using name space
c++中使用`using name space ` 来进行命名空间的选择，但是一般不建议直接在声明中使用。建议使用`spacename:: function`的格式；例如`std::cout`;并且头文件中，不应该包括`using`声明。

### 标准库类型 string
下面展示string的常用初始化方法：

```c++
    std::string s1; //默认初始化，s1市一个空字符
    std::string s2=s1; //s2是s1的副本
    std::string s3="hiya";//s3市该字符串字面值的副本
    std::string s5("hiya");//s3是字面值“value”的副本，除了字面值最后的那个空字符除外
    std::string s4(10,'c'); //s4 的内容是cccccccccc
```
`os<<s`、`is>>s`；从字符串的输入，输出流。例如：

```c++
#include <iostream>
using namespace std;
int main(int argc, char const *argv[])
{
    string s;
    cin>>s; //将string 对象读入s,遇到空白停止
    cout<<s<<endl;//输出
    return 0;
}
```
`getline()`:读取函数整行，直到遇到换行符为止。
注意`string 对象的size()`函数返回值类型是`string::size_type`;当他与一个具有负号的n比较时，肯定为true，因此建议使用`auto`来进行返回值的定义。防止`int`和`unsigned`可能带来的问题。

#### 标准库string重点字符处理--<cctype>头文件

|函数名称|功能|
|:---:|:---|
|`isalnum(c)`|当c是字母或数字时为真|
|`isalpha(c)`|当c是字母时为真|
|`iscntrl(c)`|当c是控制字符时为真|
|`isdigit(c)`|当c是数字时为真|
|`isgraph(c)`|当c不是空格但可以打印时为真|
|`isalower(c)`|当c是小写字母时为真|
|`isprint(c)`|当c是可打印字符时为真|
|`ispunct(c)`|当c是标点符号时为真|
|`isspace(c)`|当c是空白时为真（空格，制表符、回车符、换行符、进纸符中的一种）|
|`issupper(c)`|当c是大写字母时为真|
|`isxdigit(c)`|当c是十六进制数字时为真|
|`tolower(c)`|转换字母为小写|
|`toupper(c)`|转换字母为大写|

使用示例：

```c++
string s("hello word !!!");
decltype(s.size()) punct_cnt=0;
for (auto c : s)//注意这里auto是拷贝无法改变其中的char的值，可以使用&c进行值的改变。
{
    if(ispunct(c))
    {
        ++punct_cnt;//标点负号计数值加1；
    }
    cout<<punct_cnt
        <<"punctuation characters in"<<s<<endl;
}
//输出结果
3 punctuation characters in hello word!!!
```
将输入十进制数字转化为十六进制：

```c++
#include <iostream>
#include <stack>
#include <sstream>
#include <string>
using namespace std;

int main(int argc, char const *argv[]) {
    const string test="0123456789ABCDEF";
    cout<<"please input number between 0 and 15"<<endl;
    string::size_type n, temp;//用于保存从输入流读取的数;
    while((cin.peek()!=EOF)&&(cin.peek()!='\n'))
    {
        cin>>n;
        string result;//用于保存十六进制的字符串
       //else if(n>test.size()){
            for(int i=n;i>0;){
                temp=i%16;
                auto s1 = test[temp];//转化为十六进制
                result=s1+result;
                i=i>>4;
            }
        printf("%X\n",n);
        std::cout<<"this result is :"<<result<<std::endl;
        //result.clear();
    }
    printf("hello word");
    return 0;
}
```
### vector 向量介绍
#### 迭代器(iterator)介绍
迭代器类似于指针但是不同于指针；利用地址进行一次间接的迭代访问。
标准迭代器的运算符：

|运算符|功能|
|:---:|:---:|
|*iter|返回迭代器iter所指元素的引用|
|iter->mem|解引用iter并获取该元素的名为mem的成员，等价于(*iter).mem|
|++iter|令iter指示容器的下一个元素|
|--iter|令iter指示容器的上一个元素|
|iter1==iter2|判断是否指向同一个元素|
|iter1!=iter2|判断是否指向同一个元素|

```c++
string s("some string ");
if(s.begin()!=s.end())
{
    auto it=s.begin();//it表示s的第一个字符
    *it=toupper(*it);//当前字符改写成大写格式
}
```
注意：
1. c++中定义了箭头运算符：(`->`)把解引用和成员访问两个操作结合在一起，`it->mem`和`(*it).mem`表达的意思相同。
2. 两个迭代器相减结果是它们之间的距离。迭代器加整数还是迭代器。

### 数组

使用数组下标的时候，通常将其定义为`size_t`类型；
```c++
unsigned scores[11]={};//11个分数段，全部初始化为0
unsigned grade;
while(cin>>grade) {
    /* code */
    if (grade<=100)
    {
        /* code */
        ++scores[grade/10];//将当前分数段的计数值加1
    }
}
```
注意：
1. 数组中的 `int *parr[10]`表示含有10个整型指针的数组;`int (*parr)[10]`表示指向含有10个整数的数组的指针。`int *(&array)[10]`表示含有10个int型指针的数组的引用。

#### c 风格字符串
c++中支持c风格字符串，但是在c++程序中最好还是不要使用他们。因为字符串使用起来不太方便，而且极易产生程序内存泄露
c++中string对象使用c_str()函数，实现string对象到 char*[]的转换。
尽量使用标准类库而非数组。

#### 多维数组的初始化：
```c++
int ia[3][4]={
    //内嵌`{`并不是必须的但是，可以似的文件更加整洁，代码更加规范
    {0,1,2,3},
    {4,5,6,7},
    {8,9,10,11}
}
```

## 第四章 表达式
### 左值和右值问题
_参考链接_：[理解C和C++中的左值和右值](https://blog.csdn.net/xuwqiang1994/article/details/79924310);[C++11 左值、右值、右值引用详解](https://blog.csdn.net/hyman_yx/article/details/52044632);

左值：代表一个在内存中占有确定位置的对象(存在地址);例如`int a=0;`中`a`就是一个左值
右值：通过排他性来定义，每个表达式不是lvalue就是rvalue。因此从上面的lvalue的定义，rvalue是在不在内存中占有确定位置的表达式。

**左值引用**就是对一个左值进行引用的类型。**右值引用**就是对一个右值进行引用的类型，事实上，由于右值通常不具有名字，我们也只能通过引用的方式找到它的存在。

右值引用和左值引用都是属于引用类型。无论是声明一个左值引用还是右值引用，都必须立即进行初始化。而其原因可以理解为是引用类型本身自己并不拥有所绑定对象的内存，只是该对象的一个别名。左值引用是具名变量值的别名，而右值引用则是不具名（匿名）变量的别名。

左值引用通常也不能绑定到右值，但常量左值引用是个“万能”的引用类型。它可以接受非常量左值、常量左值、右值对其进行初始化。不过常量左值所引用的右值在它的“余生”中只能是只读的。相对地，非常量左值只能接受非常量左值对其进行初始化。

```c++
int &a = 2; // 左值引用绑定到右值，编译失败 
int b = 2; // 非常量左值 
const int &c = b; // 常量左值引用绑定到非常量左值，编译通过 
const int d = 2; // 常量左值 
const int &e = c; // 常量左值引用绑定到常量左值，编译通过 
const int &b =2; // 常量左值引用绑定到右值，编程通过
```
右值值引用通常不能绑定到任何的左值，要想绑定一个左值到右值引用，通常需要std::move()将左值强制转换为右值，例如：

```c++
int a;
int &&r1 = c;             # 编译失败
int &&r2 = std::move(a);  # 编译通过
```
![可引用类型值](https://img-blog.csdn.net/20160727131907698)

```c++
 // lvalues:
  //
  int i = 42;
  i = 43; // ok, i is an lvalue
  int* p = &i; // ok, i is an lvalue
  int& foo();
  foo() = 42; // ok, foo() is an lvalue
  int* p1 = &foo(); // ok, foo() is an lvalue

  // rvalues:
  //
  int foobar();
  int j = 0;
  j = foobar(); // ok, foobar() is an rvalue
  int* p2 = &foobar(); // error, cannot take the address of an rvalue
  j = 42; // ok, 42 is an rvalue
```

### 运算符优先级顺序

运算符优先级顺序如下表所示：

|运算符|功能|用法|
|:---:|:---:|:---:|
|`+`|一元正号|`+ expr`|
|`-`|一元负号|`- expr`|
|`*`|乘法|`expr * expr`|
|`/`|除法|`expr / expr`|
|`%`|求余|`expr % expr`|
|`+`|加法|`expr + expr`|
|`-`|减法|`expr - expr`|

注意

1. 使用数据类型赋值的时候不要超出类型的上界；例如：
```c++
short short_value=32767；//short类型占16位，则能表示的最大值是32767
short_value+=1;//该计算导致溢出，实际值：-32768
```
2. 如果`m%n`不等于0，则它的负号和`m`相同
3. 赋值运算符`=`的左侧运算对象必须是一个可以修改的左值。

### 位运算符
位运算符主要使用方法如下表：

|运算符|功能|用法|
|:---:|:---:|:---:|
|`~`|位求反|`~ expr`|
|`<<`|左移，相当于乘`2^n`|`expr1 >> expr2`|
|`>>`|右移，相当于除`2^n`|`expr1 >> expr2`|
|`&`|位与|`expr & expr`|
|`^`|位异或|`expr ^ expr`|
|`"|"`|位或|`"expr|expr"` |


### `sizeof`运算符

`sizeof`来进行对象或者类型名称所占用的字节数目

### 类型转换
_参考链接：_ [C++强制类型转换：static_cast、dynamic_cast、const_cast、reinterpret_cast](https://www.cnblogs.com/chenyangchun/p/6795923.html);[C++ 类型转换（C风格的强制转换）](https://www.cnblogs.com/Allen-rg/p/6999360.html);

c++中的隐式类型转换，已经在之前介绍过了，同时c++中还存在显示的强制类型转换`cast-name<type>(expression)`；其中`cast-name`是指：`static_cast`、`dynamic_cast`、`const_cast`和`reinterpret_cast`

|名称|区别|
|:---:|:---:|
|`static_cast`|静态强制转换，编译时就转换|
|`dynamic_cast`|动态强制转换，运行时转换|
|`const_cast`|编译时进行检查，强制消除对象的常量性|
|`reinterpret_cast`|编译时进行检查，是特意用于底层的强制转型，主要用于二进制的强制类型转换|

* `static_cast` 

`static_cast`相当于传统的C语言里的强制转换，该运算符把expression转换为new_type类型，用来强迫隐式转换，例如non-const对象转为const对象，编译时检查，用于非多态的转换，可以转换指针及其他，但没有运行时类型检查来保证转换的安全性。它主要有如下几种用法：
  1. 用于类层次结构中基类（父类）和派生类（子类）之间指针或引用的转换。
      - 进行上行转换（把派生类的指针或引用转换成基类表示）是安全的；
      - 进行下行转换（把基类指针或引用转换成派生类表示）时，由于没有动态类型检查，所以是不安全的。
  2. 用于基本数据类型之间的转换，如把int转换成char，把int转换成enum。这种转换的安全性也要开发人员来保证。
  3. 把空指针转换成目标类型的空指针。
  4. 把任何类型的表达式转换成void类型。

注意：static_cast不能转换掉expression的const、volatile、或者__unaligned属性。

* `dynamic_cast`

主要用于执行“安全的向下转型(safe downcasting)”，也就是说，要确定一个对象是否是一个继承体系中的一个特定类型。支持父类指针到子类指针的转换，这种转换时最安全的转换。它 是唯一不能用旧风格语法执行的强制类型转换，也是唯一可能有重大运行时代价的强制转换。
  1. 其他三种都是编译时完成的，`dynamic_cast`是运行时处理的，运行时要进行类型检查。
  2. 不能用于内置的基本数据类型的强制转换。
  3. `dynamic_cast`转换如果成功的话返回的是指向类的指针或引用，转换失败的话则会返回NULL。
  4. 使用`dynamic_cast`进行转换的，基类中一定要有虚函数，否则编译不通过。B中需要检测有虚函数的原因：类中存在虚函数，就说明它有想要让基类指针或引用指向派生类对象的情况，此时转换才有意义。这是由于运行时类型检查需要运行时类型信息，而这个信息存储在类的虚函数表（关于虚函数表的概念，详细可见<Inside c++ object model>）中。只有定义了虚函数的类才有虚函数表。
  5. 要求<>内部所描述的目标类型必须为指针或引用。
  6. 在类的转换时，在类层次间进行上行转换时，`dynamic_cast`和`static_cast`的效果是一样的。在进行下行转换时，`dynamic_cast`具有类型检查的功能，比`static_cast`更安全。
    - 向上转换，即为子类指针指向父类指针（一般不会出问题）；向下转换，即将父类指针转化子类指针。
    - 向下转换的成功与否还与将要转换的类型有关，即要转换的指针指向的对象的实际类型与转换以后的对象类型一定要相同，否则转换失败。
    - 在C++中，编译期的类型转换有可能会在运行时出现错误，特别是涉及到类对象的指针或引用操作时，更容易产生错误。Dynamic_cast操作符则可以在运行期对可能产生问题的类型转换进行测试。

* const_cast
  
 而`const_cast`则正是用于强制去掉这种不能被修改的常数`const`特性，但需要特别注意的是`const_cast`不是用于去除变量的常量性，而是去除指向常数对象的指针或引用的常量性，其去除常量性的对象必须为指针或引用。
 1. 该运算符用来修改类型的const或volatile属性。除了const 或volatile修饰之外， type_id和expression的类型是一样的。
 2. 常量指针被转化成非常量指针，并且仍然指向原来的对象；
 3. 常量引用被转换成非常量引用，并且仍然指向原来的对象；常量对象被转换成非常量对象。
 4. const_cast强制转换对象必须为指针或引用
 5. const_cast一般用于修改底指针。如const char *p形式

* reinterpret_cast

是特意用于底层的强制转型，导致实现依赖（就是说，不可移植）的结果，例如，将一个指针转型为一个整数。这样的强制类型在底层代码以外应该极为罕见。操作 结果只是简单的从一个指针到别的指针的值得二进制拷贝。在类型之间指向的内容不做任何类型的检查和转换。
new_type必须是一个指针、引用、算术类型、函数指针或者成员指针。它可以把一个指针转换成一个整数，也可以把一个整数转换成一个指针（先把一个指针转换成一个整数，再把该整数转换成原类型的指针，还可以得到原先的指针值）。

##### 运算符优先级表格如下


<table>
    <thead>
        <tr>
            <th>优先级 </th>
            <th>运算符 </th>
            <th>说明  </th>
            <th>结合性 </th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>1</td>
            <td>::</td>
            <td>范围解析</td>
            <td rowspan="6">自左向右</td>
        </tr>
        <tr>
            <td rowspan="5">2</td>
            <td>++ - - </td>
            <td>后缀自增/后缀自减</td>
        </tr>
        <tr>
            <td>()</td>
            <td>括号</td>
        </tr>
        <tr>
            <td>[]</td>
            <td>数组下标</td>
        </tr>
        <tr>
            <td>.</td>
            <td>成员选择(对象)</td>
        </tr>
        <tr>
            <td>-&gt;</td>
            <td>成员选择(指针)</td>
        </tr>
        <tr>
            <td rowspan="9">3</td>
            <td>++ - -</td>
            <td>前缀自增/前缀自减</td>
            <td rowspan="9">自右向左</td>
        </tr>
        <tr>
            <td>+ -</td>
            <td>加减</td>
        </tr>
        <tr>
            <td>! ~</td>
            <td>逻辑非/按位取反</td>
        </tr>
        <tr>
            <td>(type)</td>
            <td>强制类型转换</td>
        </tr>
        <tr>
            <td>*</td>
            <td>取指针指向的值</td>
        </tr>
        <tr>
            <td>&amp;</td>
            <td>某某的地址</td>
        </tr>
        <tr>
            <td>sizeof</td>
            <td>某某的大小</td>
        </tr>
        <tr>
            <td>new,new[]</td>
            <td>动态内存分配/动态数组内存分配</td>
        </tr>
        <tr>
            <td>delete,delete[]</td>
            <td>动态内存分配/动态数组内存释放</td>
        </tr>
        <tr>
            <td>4</td>
            <td>.* -&gt;* - -</td>
            <td>成员对象选择/成员指针选择</td>
            <td rowspan="12">自左向右</td>
        </tr>
        <tr>
            <td>5</td>
            <td>* / %</td>
            <td>乘法/除法/取余</td>
        </tr>
        <tr>
            <td>6</td>
            <td>+ -</td>
            <td>加号/减号</td>
        </tr>
        <tr>
            <td>7</td>
            <td>&lt;&lt; &gt;&gt;</td>
            <td>位左移/位右移</td>
        </tr>
        <tr>
            <td rowspan="2">8</td>
            <td> &lt; &lt;=</td> 
            <td>小于/小于等于</td>
        </tr>
        <tr>
            <td> &gt; &gt;=</td> 
            <td>大于/大于等于</td>
        </tr>
        <tr>
            <td>9</td>
            <td>== !=</td>
            <td>等于/不等于</td>
        </tr>
        <tr>
            <td>10</td>
            <td>&amp;</td>
            <td>按位与</td>
        </tr>
        <tr>
            <td>11</td>
            <td>^</td>
            <td>按位异或</td>
        </tr>
        <tr>
            <td>12</td>
            <td>|</td>
            <td>按位或</td>
        </tr>
        <tr>
            <td>13</td>
            <td>&amp;&amp;</td>
            <td>与运算</td>
        </tr>
        <tr>
            <td>14</td>
            <td>||</td>
            <td>或运算</td>
        </tr>
        <tr>
            <td>15</td>
            <td>?:</td>
            <td>三目运算符</td>
            <td rowspan="7">自右向左</td>
        </tr>
        <tr>
            <td rowspan="5">16</td>
            <td>=</td>
            <td>赋值</td>
        </tr>
        <tr>
            <td>+= -=</td>
            <td>相加后赋值/相减后赋值</td>
        </tr>
        <tr>
            <td>*= /= %=</td>
            <td>相乘后赋值相值/取余后赋值</td>
        </tr>
        <tr>
            <td>&lt;&lt;= &gt;&gt;=</td>
            <td>位左移赋值/位右移赋值</td>
        </tr>
        <tr>
            <td>&amp;= ^= |=</td>
            <td>位与运算后赋值/位异或运算后赋值/位或运算后赋值</td>
        </tr>        
        <tr>
            <td>17</td>
            <td>throw</td>
            <td>抛出异常</td>
        </tr>        
        <tr>
            <td>18</td>
            <td>,</td>
            <td>逗号</td>
            <td>自左向右</td>
        </tr>
    </tbody>
</table>












