---
layout:     post
title:      C++ Primer  学习笔记 (一)
subtitle:   C++ Primer  学习记录 (一)
date:       2019-4-17
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - C++
    - 基础编程
---

# C++ Primer学习笔记
> 2019-4-17 12:31:29 编写记录如下；

## 第1章 开始
### std::cin 中的循环流使用
_参考链接：_[while(cin >> num)循环输入问题](https://bbs.csdn.net/topics/390809866)[C++ cin输入流 详细用法](https://blog.csdn.net/imkelt/article/details/51925479);[C++怎么解决输入两行数据的](https://www.nowcoder.com/discuss/35189);[C++从键盘读入数组并存储](https://www.cnblogs.com/zhuruibi/p/9458107.html)

C++ 中可以使用`while(std::cin >> value){//Code }`的方式来进行循环数据的读入，直到没有输出为止；示例代码如下：
```c++
/*
输入样例： 3 4 5 6
输出 ： Sum is :18
 */

#inlude <iostream>
int main()
{
    int sum =0;value=0;
    //循环读取数据

    while(std::cin>>value){
        sm+=value;
    }
    std::out<<"Sum is:"<<sum<<std:endl;
    return 0;
}
```
注意：

* 当键盘向程序中输入数据时，对于文件结束；Windows是`Ctrl+Z`然后 `Enter`或者`Return`;UNIX 中是`Ctrl+D` 然后再加`enter`;
* 当缓冲区中有残留数据时，`std::cin`会直接去读取缓冲区的数据而不会请求键盘输入。重要的是，回车符也会被存在输入缓冲区中。
* 当程序中有多个等待循环输入时，需要使用`cin.clear()`来重置循环状态，方便再次输入; 

# 第二章 变量和基本类型

## C++变量和基本类型
### C++中的基本类型
_参考链接：_[C/C++中基本数据类型所占内存大小](https://blog.csdn.net/zcyzsy/article/details/77935651);[C/C++中基本数据类型在不同系统中所占空间大小](https://blog.csdn.net/yimingsilence/article/details/54730438);

C++中定义了算术类型(arithmetic type)和空类型(void)在内的基础数据结构算术类型表如下：

| 类型 | 含义 | 最小尺寸 |
|:------------:|:---------------:|:-----:|
| bool         | 布尔类型| 未定义 |
| char | centered | 8位 |
| wchart_t | 宽字符 | 16位 |
| chart16_t | Unicode字符 | 16位 |
| chart32_t | Unicode字符 | 32位 |
| short | 短整型 | 16位 |
| int | 整型 | 16位 |
| long | 长整型 | 32位 |
| long long | 长整型 | 64位 |
| float | 单精度浮点数 | 6位有效数字 |
| double | 双精度浮点数 | 10位有效数字 |
| long double | 扩展精度浮点数 | 10位有效数字 |

注意：

* 关于不同类型，字节内存分配的问题，不同的操作系统存在不同的内存分配策略；因此不一定按照上面的进行分配(倍数规则和整圆规则)；详细内容参看参考链接。
* 对于C++中的字节对齐内容需要重点考虑([C++ 字节对齐的总结(原因和作用)](https://blog.csdn.net/sweetfather/article/details/78487563);[C/C++ 字节对齐](https://blog.csdn.net/chengonghao/article/details/51674166);[从硬件到语言，详解C++的内存对齐（memory alignment）](https://www.cnblogs.com/zhao-zongsheng/p/9099603.html));
* c++中除去布尔类型和扩展的字符类型之外，其它类型可以划分为带符号的(signed)和无符号的(unsigned)两种；无符号仅能表示大于0的值。器字节内存分配也有所不同；（[C/C++ unsigned 详细探讨](https://blog.csdn.net/zhenlin123/article/details/81062635)）;

### C++中的声明和定义
C++中使用分离式编译机制，允许每个文件被单独编译；为了支持这种模式；C++语言将声明和定义区分开来；声明(declaration) 使得名字为程序所知；定义（definition）负责创建与名字关联的实体。
如果想要声明一个变量而非定义它，就在变量名字前添加关键字`extern`;而且不要显示的初始化变量：
```c++
extern int i ;//声明i 而非定义i

int j;//声明并定义j
```
### C++中的类型转换：
C++中的类型转换分为显式转换和隐式转换2种；显式转换直接在代码中注明其转换类型;如`double a=1.002;int i=(int)a;`将类型进行显式转换；同时也存在隐式转换；如`int i=10/1.0`;其中`10/1.0`即包含隐式的`int`到`double`的转换，`=`又进行了一次`double`到`int`的隐式转换；隐式转换在编码规范中不推荐；应该劲量使用显式转换；

```c++
bool b=42;      //b为真

int i=b;        //i的值为1

i=3.14;         //i的值为3

double pi=i;    //pi的值为3.0

unsigned char c=-1; //假设char占a比特，c的值为255

signed char c2=256; //假设char:占8比特，c2的值是未定义的
```
* 当数字加减超过数据类型的取值范围的时候，就会按照位运算进行取模，输出结果是取模之后的结果（[C++ 带符号和无符号char类型赋值超出表示范围的情况](https://blog.csdn.net/eastlhu/article/details/72809256)）；

例如：8比特大小的unsigned char可以表示0至255区间内的值，如果我们赋了一个区间以外的值，则实际的结果是该值对256（总数）取模后所得的余数。
因此，把-1赋给8比特大小的unsigned char所得的结果是255，使用2种方法计算：

> 有整数a和b，a对b进行取模或取余运算
> 
> 1、求整数商：c=a/b
> 
> * 取模运算在计算商值向负无穷方向舍弃小数位
> * 取余运算在计算商值向0方向舍弃小数位
>
> 2、计算模或者余数：r=a-(c*b)
> 
> * 取模运算遵循尽可能让商小，取余运算遵循尽可能让余数的绝对值小。因此，取模和取余的结果不同。
> * mod为取模，rem为取余，取模和取余所得的结果在a和b(同为整数) 符号相同 的时候是相等的
> * 当a和b符号一致时，求模运算和求余运算所得的c的值一致，因此结果一致。但是当符号不一致的时候，结果不一样。
> * 具体来说，求模运算结果的符号和b一致，求余运算结果的符号和a一致

在本例中，将-1和256带入a和b，c=-1/256，向负无穷方向舍弃小数得-1，计算得r=255.
> 计算机中带符号的整数采用二进制的补码进行存储;
> 正数的补码等于其二进制编码;
> 负数的补码等于其绝对值的二进制编码，取反，再加1;
> 在本例中，-1的绝对值是1，二进制编码为0000 0001，取反加1就是1111 1111;
> unsigned是无符号数，会把1111 1111看成正数，刚好是255的二进制编码。

```c++

unsigned u=10;
int i=-42;
std::cout<<i+i<<std::endl; //正确：输出32

std::cout<<u+i<<std::endl; //如果int占32位，输出4294967264
//上面讲-42转化为unsigned int 负数转化为无符号函数，类似于直接给无符号数赋值一个负值；等于这个负数加上无符号数的模。  

unsigned ul=42,u2=10;
std::cout<<u1-u2<<std::endl;//正确：输出32；

std::cout<<u2-u1<<std::endl;//正确：不过，结果是取模后的值；

```

GCC编译器32位机和64位机各个类型变量所占字节数

|C类型| 32位机器(字节)| 64位机器(字节)|
|:---|:---|:---|
|`char` | 1 |1|
|`short`| 2| 2|
|`int`| 4 |4|
|`long int`|  4| 8|
|`long long`| 8| 8|
|`char *` | 4 |8|
|`float`| 4| 4|
|`double`|  8| 8|




### 字面常量值：
字面常量值就是常量；如`10/*十进制*/`；`014/*八进制*/`；`0x14/*十六进制*/`；包含整数和字符；C++11标准中允许使用`{}`进行数据对象的初始化，但是C++98中并不允许；例
```c++
vector<int > main_test={1,2,3};
```
### C++中的关键字
_参考链接：_ [C++关键字详解](https://blog.csdn.net/scmuzi18/article/details/53696778);

C++中存在预定义的关键字；如下图所示

![C++关键字表格](https://wangpengcheng.github.io/img/cplusplus_key_words.png)

注：上表中为C++98/03中的63个关键字，其中红色标注为C语言中的32个关键字。C++11中有73个关键字，新增加的10个为：`alignas、alignof、char16_t、char32_t、constexpr、decltype、noexpect、nullptr、static_assert、thread_local`

1. asm
   _asm是一个语句的分隔符。不能单独出现，必须接汇编指令。一组被大括号包含的指令或一对空括号。
例：
```c++
_asm
{
  mov al,2
  mov dx,0xD007
  out al,dx
}
```
    也可以在每个汇编指令前加`_asm`
```c++
  _asm mov al,2
  _asm mov dx,0xD007
  _asm out al,dx
```
2. auto 
   auto关键字会根据初始值自动推断变量的数据类型。不是每个编译器都支持auto。
   例：
```c++
    auto  x = 7;  //使用整数7对变量x进行初始化，可推断x为int型。

    auto  y=1.234;  //使用浮点数1.234对变量y进行初始化，可推断y为double型。
```

3. *_cast
   即 `const_cast`、`dynamic_cast`、`reinterpret_cast`、`static_cast`。
   C++类型风格来性转换。`const_cast`删除const变量的属性，方便赋值；`dynamic_cast`用于将一个父类对象的指针转换为子类对象的指针或引用；`reinterpret_cast`将一种类型转换为另一种不同的类型；`static_cast`用于静态转换，任何转换都可以用它，但他不能用于两个不相关的类型转换。
4. `bool、true、false`

`bool`即为布尔类型，属于基本类型中的整数类型，取值为真和假。`true`和`false`是具有布尔类型的字面量，为右值，即表示真和假。
注`：`字面量`：`用于表达源代码中一个固定值的表示法。
5. `break`、`cotinue`、`goto` 
   `break`用于跳出`for`、`while`循环或`switch`语句。`continue`用于调到一个循环的起始位置。goto用于无条件跳转到函数内得标号处。一般情况不建议使用`goto`，风险较大。

6. `switch`、`case`、`default`
   `switch`分支语句的起始，根据`switch`条件跳转到`case`标号或`defalut`标记的分支上。

7. `catch`、`throw`、`try`
   用于异常处理。`try`指定`try`块的起始，`try`块后的`catch`可以捕获异常。异常由`throw`抛出。`throw`在函数中还表示动态异常规范。
8、`char`、`wchar_t`
   表示字符型和宽字符型这些整数类型（属于基本类型），但一般只专用于表示字符。`char`（和`signed char`、`unsigned char`一起）事实上定义了字节的大小。`char`表示单字节字符，`wchar_t`表示多字节字符。
9. `const`、`volatile`
   `const`和`volatile`是类型修饰符，语法类似，用于变量或函数参数声明，也可以限制非静态成员函数。`const`表示只读类型（指定类型安全性，保护对象不被意外修改），`volatile`指定被修饰的对象类型的读操作是副作用（因此读取不能被随便优化合并，适合映射I/O寄存器等）。
  * `volatile`:
        * 当读取一个变量时，为提高存取速度，编译器优化时有时会先把变量读取到一个寄存器中，以后再取变量值时，就直接从寄存器中取值。
        * 优化器在用到`volatile`变量时必须每次都小心地重新读取这个变量的值，而不是使用保存到寄存器里的备份。
        `volatile`适用于多线程应用中被几个任务共享的变量。
10. `struct`、`class`、`union`
  用于类型声明。`class`是一般的类类型。`struct`在C++中是特殊的类类型，声明中仅默认隐式的成员和基类访问限定与`class`不同（`struct`是`public`，`class`是`private`）。`union`是联合体类型。满足特定条件类类型——`POD struct`或`POD union`可以和C语言中的`struct`和`union`对应兼容。
注：POD类型（Plain Old Data）,plain---代表普通类型，old---代表可以与C语言兼容。
11. `new`、`delete`
   `new`、`delete`属于操作符，可以被重载。`new`表示向内存申请一段新的空间，申请失败会抛出异常。`new`会先调用`operator new`函数，再在`operator new`函数里调用`malloc`函数分配空间，然后再调构造函数。`delete`不仅会清理资源，还会释放空间。`delete`县调用析构函数，其次调用`operator delete`函数，最后在`operator delete`函数里面调用`free`函数。`malloc`申请内存失败会返回空。`free`只是清理了资源，并没有释放空间。
12. `do`、`for`、`while`
  循环语句的组成部分，C和C++都支持这3种循环。
13. 数值类型，如 `int`、`double`、`float`、`short`、`long`、`signed`、`unsigned`
　　`signed`和`unsigned`作为前缀修饰整数类型，分别表示有符号和无符号。`signed`和`unsigned`修饰`char`类型，构成`unsigned char`和`signed char`，和`char`都不是相同的类型；不可修饰`wchar_t`、`char16_t`和`char32_t`。其它整数类型的`signed`省略或不省略，含义不变。`signed`或`unsigned`可单独作为类型，相当于`signed int`和`unsigned int`。
　　`double`和`float`专用于浮点数，`double`表示双精度，精度不小于`float`表示的浮点数。`long double`则是C++11指定的精度不小于`double`的浮点数。
14. `if`和`else`
   条件语句的组成部分。if表示条件，之后else表示否定分支。
15. `enum`
  构成枚举类型名的关键字。
16. `explicit`
    该关键字的作用就是避免自定义类型隐式转换为类类型。
17. `export`
　　使用该关键字可实现模板函数的外部调用。对模板类型，可以在头文件中声明模板类和模板函数；在代码文件中，使用关键字export来定义具体的模板类对象和模板函数；然后在其他用户代码文件中，包含声明头文件后，就可以使用该这些对象和函数。
18. `extern`
  当出现`extern "C"`时，表示 `extern "C"`之后的代码按照C语言的规则去编译；当`extern`修饰变量或函数时，表示其具有外部链接属性，即其既可以在本模块中使用也可以在其他模块中使用。
19. `friend`
  友元。使其不受访问权限控制的限制。例如，在1个类中，私有变量外部是不能直接访问的。可是假如另外1个类或函数要访问本类的1个私有变量时，可以把这个函数或类声明为本类的友元函数或友元类。这样他们就可以直接访问本类的私有变量。
20. `inline`
     内联函数，在编译时将所调用的函数代码直接嵌入到主调函数中。各个编译器的实现方式可能不同。
21. `mutable`
　　`mutable`也是为了突破`const`的限制而设置的。被`mutable`修饰的变量，将永远处于可变的状态，即使在一个`const`函数中。
22. `namespace`
　　C++标准程序库中的所有标识符都被定义于一个名为`std`的`namespace`中。命名空间除了系统定义的名字空间之外，还可以自己定义，定义命名空间用关键字`namespace`，使用命名空间时用符号`::`指定。
23. `operator`　　
   和操作符连用，指定一个重载了的操作符函数，比如，operator+。
24. `public`、`protected`、`private`
   这三个都为权限修饰符。`public`为公有的，访问不受限制；`protected`为保护的，只能在本类和友元中访问；`private`为私有的，只能在本类、派生类和友元中访问。
25. `register`
　　提示编译器尽可能把变量存入到CPU内部寄存器中。
26. `return`：`return`表示从被调函数返回到主调函数继续执行，返回时可附带一个返回值，由return后面的参数指定。return通常是必要的，因为函数调用的时候计算结果通常是通过返回值带出的。如果函数执行不需要返回计算结果，也经常需要返回一个状态码来表示函数执行的顺利与否（-1和0就是最常用的状态码），主调函数可以通过返回值判断被调函数的执行情况.
27. `static`：可修饰变量（静态全局变量，静态局部变量），也可以修饰函数和类中的成员函数。static修饰的变量的周期为整个函数的生命周期。具有静态生存期的变量，只在函数第一次调用时进行初始化，在没有显示初始化的情况下，系统把他们初始化微0.
28. `sizeof`
   返回类型名或表达式具有的类型对应的大小。
29. `template`
    声明一个模板，模板函数，模板类等。模板的特化。
30. `this`
    每个类成员函数都隐含了一个`this`指针，用来指向类本身。`this`指针一般可以省略。但在赋值运算符重载的时候要显示使用。静态成员函数没有`this`指针。
31. `typedef`
    `typedef`声明，为现有数据类型创建一个新的名字。便于程序的阅读和编写。
32. `virtual`
    声明虚基类，虚函数。虚函数=0时，则为纯虚函数，纯虚函数所在的类称为抽象类。
33. `typeid`
    `typeid`是一个操作符，返回结果为标准库种类型的引用。
34. `typename`
    `typename`关键字告诉编译器把一个特殊的名字解释为一个类型。
35. `using`
     (1)、在当前文件引入命名空间，例`using namespace std`;
     (2)、在子类中使用，`using`声明引入基类成员名称。
36. `void`
    特殊的"空"类型，指定函数无返回值或无参数。

### 复合类型
C++中的复合类型是指基于其他类型定义的类型；C++语言中基本的复合类型有：引用(&)和指针(*);

引用(&)主要是为对象起了另外一个名字；注意引用类型的初始值必须是一个对象。
#### 指针
指针(*)也是间接指向另外一种类型的复合类型；但其相对引用有一下不同点：
    * 指针本身是一个对象，允许对指针赋值和拷贝;而且在指针的声明周期内它可以先后指向几个不同的对象
    * 指针无需在定义时赋值。和其它的内置类型一样，在块作用域内定义的指针如果没有初始化，也将拥有一个不确定的值。
指针操作中`&`操作符，表示取地址作用
```c++
  int ival=42;
  int *p=&ival;//p存放变量ival的地址,即p指针指向ival;

```
指针的值：
1. 指向一个对象
2. 指向紧邻对象所占空间的下一个位置
3. 空指针，意味着指针没有指向任何对象
4. 无效指针，上述情况之外的其它值。
注意：
1. 如果指针指向了一个对象；则允许使用解应用符`*`来访问对象。
2. 对于指针变量，初始化时可以使用NULL来方便内存分配判断；但是在C++11标准中对象指针使用nullptr;对于未初始化的int 等基本数据类型可以使用NULL;

#### void* 指针
void* 指针是C语言中的保留项目，它是一种特殊的指针类型；可以用于存放任意对象的地址；
但是因为void* 指针的不确定性，也意味着我们无法确定能够在这个对象上进行哪些操作。
注意：
```c++
  int* p1,p2;//p1指向int的指针；p2是int类型 

  int** pi//一个指向int指针的指针
```
#### const 关键字
##### const * int 和int *const、const int * cosnt、const int &
_参考链接：_[const int、const int *、int *cosnt、const int * const、const int &的区别](https://blog.csdn.net/itworld123/article/details/78967080);

- `int const * `:该指针变量指向的是常量，即该指针变量的内容可以改变，但是该内容指向的内容不可改变！;即底层const(常量指针)；（其与`const int *`相同）；
- `int *const`:声明该指针变量为常变量，即指针变量里面的内容不可改变，但是该内容指向的内容可以改变;即为常指针。
- `const int * cosnt`:指向一个内容不可变的指针，且指向对象地址不能变；
- `const int &`:在引用前面加上const，代表该引用为常引用，即被引用的对象不可改变。若是在形参中使用，则不可达到在函数里面修改变量值的目的。

`const`关键字修饰的变量会在编译的时候将定义的字符串替换掉，为了提高编译效率和防止文件冲突，默认状态下const对象仅在文件内有效；
注意：

 1. `const`是变量的值无法改变（有待商榷）；`static`是指变量直接在堆上分配内存，内存不会销毁，但是值可以改变；
 2. 当某一个文件中的const变量希望它能够在其它文件之间共享的时候；即在一个问价中定义const，而在其它多个文件中使用它，需要在`const`关键字前添加`extern`关键字；
 
> 我们始终记住若const限定符在`*`之前，则const限定的是`*ptr`而不限定ptr。也就是说，ptr可以改变其所指向的对象，但不能通过该指针修改其所指向对象的值。

> 若const限定符在`*`之后，则const限定的是`ptr`而不限定`*ptr`。也就是说，ptr不可以改变其所指向的对象，但能通过该指针修改其所指向对象的值。

> 若在*之前有const限定符且在*之后也有const限定符，则ptr与*ptr都被限定。也就是说，ptr既不可以改变其所指向的对象，也不能通过该指针修改其所指向对象的值。

**只要在*之后有const 限定符，则该指针在声明时必须初始化。**


##### constexpr和常量表达式
**常量表达式:（const expression）**是指不会改变并且在编译过程中就能得到计算结果的表达式；一个对象/表达式是不是常量表达式；由它的数据类型和初始值共同决定。例：
```c++
const int max_files=20;//max_files 是常量表达式

const int limit=max_files+1;//limit 是常量表达式

int staff_size=27;//staff_size 不是常量表达式

const int sz=get_size();//运行时才知道值，因此不是常量；
```
**constexpr变量**
C++11允许将变量声明为`constexpr`类型以便由编译器来验证变量的值是否是一个常量表达式。声明为constexpr的变量移动是一个产量，而且必须用常量表达式初始化：
```c++
constexpr int mf=20;//20是常量表达式

constexpr int limit=mf+1;//mf+1是常量表达式

constexpr int sz=size();//只有当size是一个constexpr函数时才是一条正确的声明语句
```
**指针和constexpr**
在constexpr声明中如果定义了一个指针，限定符constexpr仅对指针有效，与指针所指对象无关；
```c++
const int *p=nullptr;//p是一个指向整型常量的指针

constexpr int *q=nullptr;//q是一个指向整数的常量指针；constexpr把她所东一的对象置为了顶层const
```

#### 类型别名typedef关键字
- `typedef int my_int`:将`int `取别名为`my_int`;
- `typede 函数`：定义函数类型(常用语C语言中；C++慎用)；例：

```c++
typedef long SetStringPtr(char *);//预定于函数输入输出类型

typedef long GetStringPtr(char *, long);//预定于函数输入输出类型

typedef struct {
   SetStringPtr * SetString;//初始化函数指针

   GetStringPtr * GetString;//初始化函数指针
   DWORD          count;
   char           buffer[80];
} IExample;
//实例化函数
long SetString(char * str)
{
   return(0);
}

IExample * example;//使用结构体对象

example->SetString = SetString;//使用函数；
```

#### auto关键字；
auto关键字在C++11中得到了广泛使用；但是他是基于C++模板类型推断的，因此需要慎重使用；多用于循环迭代中；例：
```c++
#include <iostream>
#include <vector>
using namespace std;
int main(int arc,char const *argv[])
{
    std::vector<int> v={1,1,12,3};
    for(auto temp : v){
        std::cout<<temp<<'\t'<<std::endl;;
    }
    return 0;
}
```

#### decltype类型指示符
decltype 是C++11新引入的关键字，帮助从表达式推断定义的变量的类型。
注意：
1. decltype 处理顶层const和引用的方式与auto有些许不同；
2. 如果decltype使用的表达式不是一个变量，则decltype返回表达式结果对应的类型，当有时返回一个引用类型的时候；表达式的结果对象，能够作为一条赋值语句的左值。
3. dectype((variable))(注意是双层括号)的结果永远是引用，而decltype(variable)结果只有当variable本身就是一个引用时才是。

```c++
const int ci=0,&cj=ci;
decltype(cj) x=0;//x 类型是const int

decltype(cj) y=x;//y的类型是const int&,y绑定到变量x

decltype(cj) z;//错误，z是一个引用必须初始化

//decltype 的结果可以是引用类型

int i=42，*p=&i,&r=i;
decltype(r+0) b;//加法的结果是int，因此b是一个（未初始化的）int

decltype(*p) c;//错误：c是int&,必须初始化

```
