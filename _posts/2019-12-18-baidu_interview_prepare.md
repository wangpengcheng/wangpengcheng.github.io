---
layout:     post
title:      百度后台电话面准备
subtitle:   2020寒假实习
date:       2019-12-17
author:     王鹏程
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - 寒假实习
    - 后台开发
    - 简历投递
    - 百度
---

# 1. C++ 基础

## 1.1 说一下static关键字的作用

1. 全局静态变量
	- 在全局变量前加上关键字static，全局变量就定义成一个全局静态变量.
	- 内存中的位置：静态存储区，在整个程序运行期间一直存在。
	- 初始化：未经初始化的全局静态变量会被自动初始化为0（对于自动对象,如果没有显示初始化,会调用零参数构造函数,如不存在则编译失败);
	- 作用域：**全局静态变量在声明他的文件之外是不可见的，准确地说是从定义之处开始，到文件结尾。**
2. 局部静态变量
	- 在局部变量之前加上关键字`static`，局部变量就成为一个局部静态变量。
	- 内存中的位置：静态存储区
	- 初始化：未经初始化的全局静态变量会被自动初始化为`0`(对于自动对象,如果没有显示初始化,会调用零参数构造函数,如不存在则编译失败)；
	- 作用域：作用域仍为局部作用域，
		- 当定义它的函数或者语句块结束的时候，作用域结束。
		- 但是当局部静态变量离开作用域后，并没有销毁，而是仍然驻留在内存当中，只不过我们不能再对它进行访问，直到该函数再次被调用，并且值不变；

	---
	- `C语言`中局部静态变量不能使用变量进行初始化的, 因为它在编译期间就被编译器初始化了.
	- `C++`因为引入了对象,必须调用构造函数,所以编译器把对局部静态变量的初始化推迟至第一次调用之前,也就是可以使用变量进行初始化.

	---

3. 静态函数
	- 在函数返回类型前加static，函数就定义为静态函数。**函数的定义和声明在默认情况下都是extern的**，但**静态函数只是在声明他的文件当中可见，不能被其他文件所用**。
	- 函数的实现使用static修饰，**那么这个函数只可在本cpp内使用，不会同其他cpp中的同名函数引起冲突；**
	- warning：不要再头文件中声明static的全局函数，不要在cpp内声明非static的全局函数，如果你要在多个cpp中复用该函数，就把它的声明提到头文件里去，否则cpp内部声明需加上static修饰；

4.  类的静态成员
	- 在类中，静态成员可以实现多个对象之间的数据共享，并且使用静态数据成员还不会破坏隐藏的原则，即保证了安全性。
	- 因此，**静态成员是类的所有对象中共享的成员，而不是某个对象的成员**。对多个对象来说，静态数据成员只存储一处，供所有对象共用

5. 类的静态函数
	- **静态成员函数和静态数据成员一样，它们都属于类的静态成员，它们都不是对象成员**。因此，对静态成员的引用不需要用对象名。
	- **在静态成员函数的实现中不能直接引用类中说明的非静态成员，可以引用类中说明的静态成员（这点非常重要）**。*如果静态成员函数中要引用非静态成员时，可通过对象来引用。从中可看出，调用静态成员函数使用如下格式：<类名>::<静态成员函数名>(<参数表>);*
	- 不能被virtual修饰,静态成员函数没有this 指针，虚函数的实现是为每一个对象分配一个vptr 指针，而vptr 是通过this 指针调用的，所以不能为virtual；虚函数的调用关系，this->vptr->ctable->virtual function

## 1.2 `C++`和`c`的区别

- 设计思想上：
	- `C++`是面向对象(封装, 继承, 多态)的语言，而C是面向过程的结构化编程语言
	- C中的函数编译时不会保留形参列表, 也不能重载; 而C++中的函数在编译时会保留形参列表, 有重载
- 语法上：
	- `C++`具有`封装`、`继承`和`多态`三种特性
	- `C++`相比`C`，增加许多类型安全的功能，比如强制类型转换、
	- `C++`支持范式编程，比如模板类、函数模板等
- 具体的:
	- 动态管理内存的方法不一样: `malloc/free` 和 `new/delete`
	- struct
		- C中: struct是自定义数据类型; 是变量的集合, 不能添加拥有成员函数; 没有访问权限控制的概念; 结构体名称不能作为参数类型使用, 必须在其前加上struct才能作为参数类型
		- C++中: struct是抽象数据类型, 是一个特殊的类, 可以有成员函数, 默认访问权限和继承权限都是public, 结构体名可以作为参数类型使用
	- C语言没有引用的概念, 更没有左值引用, 右值引用
	- C语言不允许只读数据(const修饰)用作下标定义数组, C++允许
	- C语言的局部静态变量初始化发生于编译时，所以在函数中不能使用变量对局部静态变量进行初始化， 而C++因为增加了对象的概念，而对象需要调用构造函数进行初始化，所以编译器将局部静态变量的初始化推迟至该变量使用之前，也就是说可以使用变量来初始化局部静态变量。

---
- 参考: [2019-11-26-面经汇总-C++基础再探-c和c++的区别](https://zhaostu4.github.io/2019/11/28/%E9%9D%A2%E7%BB%8F%E6%B1%87%E6%80%BB-C++%E5%9F%BA%E7%A1%80%E5%86%8D%E6%8E%A2/#59%E8%AF%B4%E8%AF%B4%E4%BD%A0%E5%AF%B9c-%E5%92%8Cc%E7%9A%84%E7%9C%8B%E6%B3%95c-%E5%92%8Cc%E7%9A%84%E5%8C%BA%E5%88%AB)

---
## 1.3 `c++`中四种`cast`转换

> `C++`中四种类型转换是：`static_cast`, `dynamic_cast`, `const_cast`, `reinterpret_cast`

- `const_cast`: 
	- 用来移除`const`或`volatile` 属性。但需要特别注意的是`const_cast`不是用于去除变量的常量性，而是去除**指向常数对象的指针或引用**的常量性，其去除常量性的对象必须为**指针或引用**。
	- 如果对一个指向常量的指针,通过`const_cast`移除`const`属性, 然后进行修改, 编译通过,但是运行时会报段错误
- `static_cast`: 静态类型转换(不能移除`const/volatile`属性)是最常看到的类型转换, 几个功能. 
	- **内置类型之间的转换**, 精度耗损需要有程序员把握
	- **继承体系中的上下行转换**(上行:子类转父类,安全转换; 下行:父类转子类, 不安全转换)
	- **指针类型转换**: `空指针转换成目标类型的空指针`, `把任何类型转换成void 类型`。
- `dynamic_cast`: 主要用在继承体系中的安全向下转型
	- 它能安全地将指向基类的`指针/引用`转型为指向子类的`指针/引用`, 转型失败会返回`null`(转型对象为指针时)或抛出异常`bad_cast`(转型对象为引用时)。
	- `dynamic_cast` 会利用运行时的信息`(RTTI)`来进行动态类型检查，因此dynamic_cast 存在一定的效率损失。
	- 而且`dynamic_cast`进行动态类型检查时, 利用了虚表中的信息, 所以只能用于函数虚函数的类对象中. 
- `reinterpret_cast` 强制类型转换,**非常不安全**
	- 它可以把一个指针转换成一个整数，也可以把一个整数转换成一个指针(先把一个指针转换成一个整数，在把该整数转换成原类型的指针，还可以得到原先的指针值)。

***
- 参考: 
	- [C++开发必看 四种强制类型转换的总结](https://www.cnblogs.com/lidabo/p/3651049.html)
	- [C++系列总结——volatile关键字](https://www.cnblogs.com/yizui/archive/2019/03/30/10628020.html)
	
	- [2019-11-26-面经汇总-C++基础再探-C++类型转换有四种](https://zhaostu4.github.io/2019/11/28/%E9%9D%A2%E7%BB%8F%E6%B1%87%E6%80%BB-C++%E5%9F%BA%E7%A1%80%E5%86%8D%E6%8E%A2/#93c%E7%B1%BB%E5%9E%8B%E8%BD%AC%E6%8D%A2%E6%9C%89%E5%9B%9B%E7%A7%8D)

***

## 1.4 C/C++ 中指针和引用的区别
- 初始化:
	- 引用在定义的时候必须进行初始化，并且不能够改变
	- 指针在定义的时候不一定要初始化，并且指向的空间可变

- 访问逻辑不同:
	- 通过指针访问对象, 用户需要使用间接访问
	- 通过引用访问对象, 用户只需使用直接访问, 编译器负责将其处理为间接访问

- 运算结果不同:
	- 自增运算结果不同
	- `sizeof`运算的结果不同
	- 下标运算: 
		- 指针通过下标运算结果是指针所指值为基地址加上偏移, 且基地址可变.
		- 引用通过下标运算结果是引用的是数组才能有这个操作.
	- 函数参数:
		- 传指针的实质是传值，传递的值是指针内储存的变量地址；
		- 传引用的实质是传地址，传递的是变量的地址。
	- 多级: 有多级指针，但是没有多级引用，只能有一级引用。


***
- 参考: 
	- [C++ 引用占用内存？ - toyijiu的专栏 - CSDN博客](https://blog.csdn.net/toyijiu/article/details/99729949)
	- [浅谈返回函数内部new分配的内存的引用_C 语言_脚本之家](https://www.jb51.net/article/100823.htm)
	- [2019-11-26-面经汇总-C++基础再探-1、引用和指针的区别？](https://zhaostu4.github.io/2019/11/28/%E9%9D%A2%E7%BB%8F%E6%B1%87%E6%80%BB-C++%E5%9F%BA%E7%A1%80%E5%86%8D%E6%8E%A2/#1%E5%BC%95%E7%94%A8%E5%92%8C%E6%8C%87%E9%92%88%E7%9A%84%E5%8C%BA%E5%88%AB)

***

## 1.5 C++中的四个智能指针

- 为什么要使用智慧指针
	- 智慧指针的作用是管理指针,因为在堆上申请的内存空间,是需要手动释放的,如果未释放就会造成内存泄漏,而智慧指针的使用可以很大程度的避免这个问题. 
	- 因为使用智慧指针管理内存的本质是: 栈对象管理堆内存, 而栈对象是超出作用域或者程序意外终止,都会自动调用析构函数,而智慧指针的析构函数会自动释放资源,从而避免内存泄漏. 
	- `RAII`:资源获取就是初始化
- `auto_ptr`: 采用所有权模式, 但是其允许所有权剥离,所以有内存崩溃的风险, 在`c++11`中已经放弃了。
- `unique_ptr`:采用独占式拥有,保证同一时间只有一个智慧指针可以指向该对象. `unique_ptr`不允许所有权剥离,除非本身是临时变量或者使用move指令. 
- `shared_ptr`:采用共享式拥有,多个共享指针可以指向相同的对象,该对象和其相关的资源会在`最后一个共享指针`被销毁时释放. 注意`shared_ptr`有交叉引用相互锁死的问题,即两个对象相互持有对方的共享指针, 造成用不释放的问题
- `weak_ptr`: 是一种不控制对象生命周期的智慧指针,他是共享指针的附属品, 主要是为了避免`shared_ptr`的死锁问题,具体表现为`weak_ptr`只能从`shared_ptr`或另一个`weak_ptr`构造,持有它不会造成`shared_ptr`的引用计数增加,以及并不能通过`weak_ptr`直接访问推向.

## 1.6 野指针是什么

- 野指针：指向内存被释放的内存或者没有访问权限的内存的指针。
- “野指针”的成因主要有`3` 种：
	- **指针变量没有被初始化**。任何指针变量刚被创建时不会自动成为`NULL` 指针，它的缺省值是随机的。
	- **指针被`free`** 或者`delete` 之后，没有置为`NULL`；
	- **指针操作超越了变量的作用范围**。
- 如何避免野指针：
	- 对指针进行初始化, 或指向有效地址空间
	- 指针用完后释放内存，将指针赋`NULL`。
		- `char * p = NULL;`
		- `char * p = (char * )malloc(sizeof(char));`
		- `char num[ 30] = {0}; char *p = num;`
		- `delete(p); p = NULL;`

## 1.7 智慧指针内存泄漏的情况
- 当两个对象使用`shared_ptr`相互引用,就会使计数器失效,从而导致内存泄漏,解决方案是使用`weak_ptr`打破循环引用

## 1.8 什么情况下会发生segmet
1. 堆栈溢出
2. 非法的数据修改;如static上的数据
3. 访问了不存在的内存
4. 内存越界，数组越界，变量类型不一致等。
可以使用gdb进行调试修改

## 1.9 为什么析构函数必须是虚函数? 为什么C++默认的析构函数不是虚函数
- 将可能被继承的基类的析构函数设置为虚函数,可以保证当我们使用基类指针指向派生类对象时,依旧可以正常的`delete`, 不至于引起内存泄漏.
- 在类中定义虚函数会增加额外的开销,包括虚函数表和虚表指针,对于非基类而言,构造函数如果设置为虚函数会浪费内存.

## 1.10 `C++` 中的析构函数的作用
- 析构函数名与类名相同,只是在函数名前增加了取反符号`~`以区别于构造函数,其不带任何参数, 也没有返回值.  也不允许重载. 
- 析构函数与构造函数的相反, 当对象生命周期结束的时候,如对象所在函数被调用完毕时,析构函数负责结束对象的生命周期. **注意如果类对象中分配了堆内存一定要在析构函数中进行释放.** 
- 和拷贝构造函数类似,如果用户未定义析构函数, 编译器**并不是一定**会自动合成析构函数, 只有在**成员变量或则基类**拥有析构函数的情况下它才会自动合成析构函数. 
- 如果**成员变量或则基类**拥有析构函数, 则编译器一定会合成析构函数, 负责调用成员变量或则基类的析构函数, 此时如果用户提供了析构函数,则编译器会在用户析构函数之后添加上述代码.
- 类析构的顺序为: 派生类析构函数, 对象成员析构函数, 基类析构函数. 

## 1.11 C++中自动添加构造函数的情况

1. 包含有带默认构造函数的对象成员的类(**某成员变量为带默认构造函数的类的对象**)：
   - 若一个类X没有定义任何构造函数，但却包含一个或以上定义有默认构造函数的对象成员，此时编译器会为X合成默认构造函数，该默认函数会调用对象成员的默认构造函数为之初始化。
   - 如果对象的成员没有定义默认构造函数，那么编译器合成的默认构造函数将不会为之提供初始化。
2. 继承自带有默认构造函数的基类的类(**基类带有默认构造函数**)
3. 带有虚函数(**来自声明或继承**):因为它多了一个vptr，而vptr的设置是由编译器完成的，**因此编译器会为类的每个构造函数添加代码来完成对vptr的初始化。***
4. 带有一个虚基类的类:在这种情况下，编译器要将虚基类在类中的位置准备妥当，提供支持虚基类的机制。也就是说要在所有构造函数中加入实现前述功能的的代码。**没有构造函数将合成默认构造函数。**

## 1.12 编译器合成拷贝构造函数的情况

- 编译器在必要的时候会合成拷贝构造函数：**必要的时刻是指编译器在普通手段无法完成解决“当一个类对象以另一个同类实体作为初值”时，才会合成拷贝构造函数。也就是说，当常规武器能解决问题的时候，就没必要动用非常规武器。**
- 如果**一个类没有定义拷贝构造函数，通常按照“成员逐一初始化(Default Memberwise Initialization)”的手法来解决“一个类对象以另一个同类实体作为初值”.**
- **成员逐一初始化(Default Memberwise Initialization)具体的实现方式则是位逐次拷贝（Bitwise copy semantics）**[^6]。[^6]: Bitwise copy semantics 是Default Memberwise Intializiation的具体实现方式。[别人的解释]

下面是没有定义时默认合成的情况：
1. 类**内含一个声明有拷贝构造函数成员对象**(不论是设计者定义的还是编译器合成的）。
2. 类**继承自一个声明有拷贝构造函数的类**（不论拷贝构造函数是被显示声明还是由编译器合成的）。
3. 类中**声明有虚函数**。
4. 当类的派生串链中**包含有一个或多个虚基类**。

## 1.13 重载和覆盖
- [C++中重载、重写（覆盖）和隐藏的区别](https://blog.csdn.net/zx3517288/article/details/48976097)
- **重载**: 同一可访问区域内声明了多个具有不同参数列表的同名函数, 根据参数列表确定那个函数被调用, 重载不关心函数返回类型. 
- **覆盖/重写**: 是指派生类中重新定义了基类中的`virtual`函数. 其函数名，参数列表，返回值类型，所有都与基类中被重写的函数一致。派生类对象通过**派生类指针或则基类指针**调用时都会调用派生类的重写函数。
- **隐藏**：是指派生类的函数屏蔽了与其同名的基类函数，只要函数名相同，基类函数都会被隐藏. 不管参数列表是否相同。

***
- 重载和重写的区别：
	1) 范围区别：重写和被重写的函数在不同的类中，重载和被重载的函数在同一类中。
	2) 参数区别：重写与被重写的函数参数列表一定相同，重载和被重载的函数参数列表一定不同。
	3) `virtual`区别：重写的基类必须要有`virtual`修饰，重载函数和被重载函数可以被`virtual`修饰，也可以没有。
- 隐藏和重写，重载的区别：
	1) 与重载范围不同：隐藏函数和被隐藏函数在不同类中。
	2) 参数的区别：隐藏函数和被隐藏函数参数列表可以相同，也可以不同，但函数名一定同；

## 1.14 在main()之前执行前运行
- 使用`static`lambda函数：
```cpp
//关闭流输出
static auto static_lambda = []()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    return 0;
}();
```
- `GCC`编译器
```cpp
	// 在main之前
	__attribute((constructor)) void before_main(){
		printf("befor\n");
	}
	// 在main之后
	__attribute((deconstructor)) void after_main(){
		printf("befor\n");
	}
```

## 1.15 字面值常量和左右值
- 简述四行代码的区别
	```c
	const char* arr = "123"; // "123" 为字符串类型字面值, 其储存于常量区, 其值不可修改, arr表示一个指向字符串类型的const指针, 如果试图通过此指针修改字符串的指,编译器会组织这一行为,导致编译失败
	char * brr = "123"; // "123" 为字符串类型字面值, 其储存于常量区, 其值不可修改, brr表示一个指向字符串类型的指针, 这里潜在的逻辑错误, 当我们试图通过brr修改所指向的字符串时, 编译器并不会阻止这一行为, 可编译通过, 但是运行时候,可能发生 DEADLYSIGNAL(致命错误)
	const char crr[] = "123" // 声明了一个以"123"为初值的常量数组, 通常情况下应该是存在于栈区, 但是使用了const修饰,编译器可能会将其放在常量区
	char drr[] = "123"; // 声明了一个以"123"为初值的字符串数组, 应该是储存于栈区, 可通过drr对数组进行修改
	```

***
- [C/C++的四大内存分区和常量的存储位置](https://blog.csdn.net/weixin_30312557/article/details/96680356)
- `std::forward`的作用: [C++完美转发为什么必须要有std::forward](https://www.zhihu.com/question/48367350)
- 左值右值的区别: [C++中的左值与右值(二)](https://www.cnblogs.com/niuxichuan/p/5844109.html)

***

## 1.16 `C++`如何处理返回值
- 函数的返回值用于初始化在调用函数时创建的临时对象:
	1) 返回值为非引用类型：会将函数的返回值复制给临时对象。
	2) 返回值为引用类型：没有复制返回值，返回的是对象本身。(但是不能返回局部变量的引用, 可以是主函数以引用方式传递的对象,或则是堆对象的引用-堆对象的引用可能会造成内存溢出)

- 关于返回值: 
	- 如果 `返回值 <= 4字节`，则返回值通过寄存器`eax`带回。
	- 如果 `4< 返回值 <=8字节`，则返回值通过两个寄存器`eax`和`edx`带回。
	- 如果 `返回值 > 8字节`，则返回值通过产生的临时量带回。

## 1.17 const?

- 一般可以分为如下六种类型
	- `const`变量(`const int *p1`): 表明标了为`const`类型, 通常需要被初始化否则后面将不能被修改, 对该变量的修改操作都会被编译器阻止.(起始就是`top-level const`)
	- `const`指针对象(`int * const p2`): 标明该指针为普通的左值类型可以进行修改, 但是不能通过该变量修改做指向的对象, 则通过该指针只能访问`const`类型的成员函数.(`bottom-level const`)
	- `const`引用: 它所绑定的对象不能被修改
	- `const`形参: 和普通的实参分类一样分为const 变量, const指针对象, const 引用, 作用也类似,表示不能修改该变量.
	- `const`返回值: 通常是为了表明返回值是一个**const类型防止返回值被修改**, 或则**被当做左值放在赋值运算的左边**
	- `const`成员函数: 是指成员函数不会修改类对象的任何成员变量, 如果返回值为对象成员的引用则必须返回`const`引用, 同时`const`成员函数不能调用非`const`函数, 其主要是因为`const`成员函数所持有的`this`指针是一个`const`类型的指针, 因为不能调用非`const`类型的成员函数.
- const顶层指针和底层指针：
  - **(常指针对象)** `int *const p2 `中`const `修饰`p2 `的值,所以理解为`p2 `的值不可以改变，即`p2 `只能指向固定的一个变量地址，但可以通过`*p2 `
  - **(常指针)** `int const *p1 `或者`const int *p1 `两种情况中`const `修饰`*p1`，所以理解为`*p1 `的值不可以改变，即不可以给`*p1 `赋值改变`p1 `指向变量的值，但可以通过给`p `赋值不同的地址改变这个指针指向。底层指针表示指针所指向的变量是一个常量。
***
- 参考: [c++函数返回类型什么情况带const - A_zhu - 博客园](https://www.cnblogs.com/Azhu/p/4352613.html)


## 1.18 `C `语言`struct `和`C++` `struct `区别

- `struct `在`C语言`中:
	- 是**用户自定义数据类型`(UDT)`**；
	- 只能是一些**变量的集合体**, 成员不能为函数
	- 没有权限设置
	- 一个**结构标记**声明后，在`C`中必须在**结构标记**前加上`struct`，才能做**结构类型名**;


***
- `struct `在`C++`中:
	- 是**抽象数据类型`(ADT)`**，支持成员函数的定义，(能继承，能实现多态)。
	- 增加了访问权限, 默认访问限定符为`public`(为了与`C `兼容)，`class `中的默认访问限定符为`private`
	- 定义完成之后, 可以直接使用**结构体名字**作为**结构类型名**
	- 可以使用模板

***

## 1.19 类成员的构造函数
一个派生类构造函数的执行顺序如下：
	- **虚基类**的构造函数(多个虚拟基类则按照继承的顺序执行构造函数)。
	- **基类**的构造函数(多个普通基类也按照继承的顺序执行构造函数)。
	- **类类型的成员对象**的构造函数(按照初始化顺序)
	- 派生类**自己的构造函数**。
- **对于类对象类型成员变量, 则是先调用零参数构造函数, 如果零参数构造函数不存在编译器将会报错.**

***

## 1.20 `构造函数`, `析构函数`可否抛出异常

- 构造函数异常
	- 后果:
		- **(原因):**`C++`拒绝为**没有完成构造函数的对象**调用**析构函数**，原因是避免开销
		- 构造函数中发生异常，控制权转出构造函数。如果构造函数中申请了堆内存, 则堆内存将无法释放, 从而造成内存泄漏
		- 例如: 在对象`b `的构造函数中发生异常，对象`b `的析构函数不会被调用。**因此会造成内存泄漏**。
	- 解决方案:
		- 使用**智慧指针**来管理堆内存. 其不需要在析构函数中手动释放资源. 在发生异常时, 智慧指针会自动释放资源从而避免了内存泄漏. 
		- **一般建议不要在构造函数里做过多的资源分配。**
- 析构函数异常
	- 后果:
		- 在异常传递的**堆栈辗转开解**的过程中, 如果发生析构异常, `C++`会调用`terminate`终止程序
		- 如果析构函数发生发生异常，则异常后面的代码将不执行，无法确保完成我们想做的清理工作。
	- 解决方法：
		- 如果异常不可避免，则应在析构函数内捕获，而不应当抛出。
		- 在析构函数中使用`try-catch`块屏蔽所有异常。
	- 附加说明:
		-  **(后果1):** 如果某一个异常发生,某对象的析构函数被调用,而此时析构发生了异常并流出了函数之外,则函数会被立即terminate掉(函数外有catch也不能拯救)

## 1.21、什么是类的继承？
- 类与类之间的关系
	- `(has-A)`包含关系，即一个类的成员属性是另一个已经定义好的类
	- `(use-A)`使用关系, 一个类使用另一个类，通过类之间的成员函数相互联系，定义友元或者通过传递参数的方式实现；
	- `(is-A)`继承关系, 继承关系，关系具有传递性；
- 继承的相关概念
	- 所谓的继承就是一个类继承了另一个类的属性和方法，这个新的类包含了上一个类的属性和方法，
	- 被称为子类或者派生类，被继承的类称为父类或者基类；
- 继承的特点
	- 子类拥有父类的所有属性和方法，子类对象可以当做父类对象使用；
	- 子类可以拥有父类没有的属性和方法；
- 继承中的访问控制
	- `public`、`protected`、`private`
- 继承中的构造和析构函数
	- 子类中构造函数的调用顺序为: 基类构造函数, 成员对象构造函数, 派生类构造函数
	- 子类中析构函数的调用顺序为: 派生类析构函数, 成员对象析构函数, 基类析构函数
- 继承中的兼容性原则
	- 类型兼容规则是指在需要基类对象的任何地方，都可以使用公有派生类的对象来替代。
	- 参考: [继承中的类型兼容性原则 - Say舞步 - 博客园](https://www.cnblogs.com/zhangyaoqi/p/4591571.html)

## 1.22 什么是组合？
- 一个类里面的数据成员是另一个类的对象，即内嵌其他类的对象作为自己的成员；
- 如果内嵌类没有零参数构造函数， 则必须使用初始化列表进行初始化
- 构造函数的执行顺序：
	- 按照内嵌对象成员在组合类中的定义顺序调用内嵌对象的构造函数。
	- 然后执行组合类构造函数的函数体，析构函数调用顺序相反。

## 1.23 C 语言的编译链接过程？
- 源代码－－>预处理－－>编译－－>优化－－>汇编－－>链接-->可执行文件
- 参考: [源码到可执行文件的过程](https://blog.csdn.net/zzxiaozhao/article/details/102990773#_1128)

## 1.24 说说你对c 和c++的看法，c 和c++的区别？
- 面向过程 / 面向对象
- C中的函数编译时不会保留形参列表, 也不能重载; 而C++中的函数在编译时会保留形参列表, 有重载
- struct
	- C中: struct是自定义数据类型; 是变量的集合, 不能添加拥有成员函数; 没有访问权限控制的概念; 结构体名称不能作为参数类型使用, 必须在其前加上struct才能作为参数类型
	- C++中: struct是抽象数据类型, 是一个特殊的类, 可以有成员函数, 默认访问权限和继承权限都是public, 结构体名可以作为参数类型使用
- 动态管理内存的方法不一样: `malloc/free` 和 `new/delete`
- C语言没有引用的概念, 更没有左值引用, 右值引用
- C语言不允许只读数据(const修饰)用作下标定义数组, C++允许
- C语言的局部静态变量初始化发生于编译时，所以在函数中不能使用变量对局部静态变量进行初始化， 而C++因为增加了对象的概念，而对象需要调用构造函数进行初始化，所以编译器将局部静态变量的初始化推迟至该变量使用之前，也就是说可以使用变量来初始化局部静态变量。
- C++相比C，增加多许多类型安全的功能，比如强制类型转换
- C++支持范式编程，比如模板类、函数模板等


***
- `PS:`C/C++的全局变量默认连接属性都是`extern`的啊, 参考:[C语言：链接属性与存储类型](https://blog.csdn.net/sinat_27706697/article/details/47679329)
- 参考: 
	- [C语言：链接属性与存储类型](https://blog.csdn.net/sinat_27706697/article/details/47679329)
	- [为什么用C语言中const常量定义数组大小会报错？](https://blog.csdn.net/weixin_43054397/article/details/90417740)
	- [15、C 语言struct 和C++ struct 区别](https://blog.csdn.net/zzxiaozhao/article/details/103188945#15_C_struct_C_struct__228)

***

## 1.25 `new`和`delete`的实现原理，`delete`是如何知道释放内存的大小的额？
- `new` 
	- 简单类型直接调用`operator new` 分配内存；
	- 对于复杂结构，先调用`operator new` 分配内存，然后在分配的内存上调用构造函数；
- `delete` 
	- 简单数据类型默认只是调用`free` 函数；
	- 复杂数据类型先调用析构函数再调用`operator delete`；

***
- `new[]`
	- 对于简单类型，`new[]`计算好大小后调用`operator new`；
	- 对于复杂数据结构
		- `AA* P = new AA[10];`
		- `new[]`先调用`operator new[]`分配内存, **分配内存时多分配四个字节用于存放元素个数.**, 返回地址为`p`
		- `p`的最开始的`4`个字节用于存放元素个数`n`, 然后从调用`n`次构造函数从`p-4`开始构造对象. 
		- 返回地址,也就是`P`, 即为`p-4`
- `delete[]`
	- 对于简单类型, 直接调用`free`进行释放(注意简单类型并没有利用`4`个字节保存元素个数, 由编译器自行优化)
	- 对于复制类型, 
		- 首先将指针前移`4`个字节获得元素个数`n`, 然后执行`n`次析构函数, 最后并释放掉内存. 
		- 因为指针指向的是`p-4`并不是内存的起始地址, 所以使用`delete`将无法完成释放, 因为`free`需要通过起始地址进行释放, 而`p-4`不是起始地址

***
- 参考: **一定要看看:**[深入理解C++ new/delete, new/delete  动态内存管理](https://imgconvert.csdnimg.cn/aHR0cHM6Ly93d3cuY25ibG9ncy5jb20vdHAtMTZiL3AvODY4NDI5OC5odG1s?x-oss-process=image/format,png)	

***

## 1.26 内存对齐？位域？
- 字节对齐的原因:
	- 更快: 如果数据未对齐自然边界, 则处理器需要两次寻址才能得到完整的数据
	- 通用: 部分硬件平面不支持访问未对齐的数据, 会抛出硬件异常
- 具体操作
	- 自定义对齐系数
		- 可以通过预编译命令`#pragma pack(n)`，`n=1`,`2`,`4`,`8`,`16`来改变这一系数，其中的`n`就是指定的“对齐系数”
	- 数据成员对齐规则: 
		- 结构`(struct)(`或联合`(union)`)的数据成员，第一个数据成员放在`offset`为`0`的地方，以后每个数据成员的对齐按照`#pragma pack`指定的数值和这个数据成员自身长度中，比较小的那个进行。
		- 结构体作为成员：
			- 如果一个结构里有某些结构体成员，则结构体成员要从其内部最大元素大小的整数倍地址开始存储。
	- 结构(或联合)的整体对齐规则：
		- 在数据成员完成各自对齐之后，结构(或联合)本身也要进行对齐，对齐将按照`#pragma pack`指定的数值和结构(或联合)最大数据成员长度中，比较小的那个进行。

- 位域
	- 有些信息在存储时，并不需要占用一个完整的字节， 而只需占几个或一个二进制位。
	- `Ｃ语言`又提供了一种数据结构，称为“位域”或“位段”。
	- 所谓“位域”是把一个字节中的二进位划分为几 个不同的区域，并说明每个区域的位数。
	- 位段成员必须声明为`int`、`unsigned int`或`signed int`类型`(short char long)`。
```c
	struct 位域结构名{ 
		位域列表 // 其中位域列表的形式为： 类型说明符 位域名：位域长度
	};  
	
	struct bs {     
		int a:8;
		int b:2;
		int c:6;
	};  
```

## 1.27 hello world 程序开始到打印到屏幕上的全过程?
- 应用程序
- 应用程序载入内存变成进程
- 进程获取系统的标准输出接口
- 系统为进程分配CPU
- 触发缺页中断
- 通过puts系统调用, 往标准输出接口上写字符串
- 操作系统将字符串发送到显示器驱动上
- 驱动判断该操作的合法性, 然后将该操作变成像素, 写入到显示器的储存映射区
- 硬件将该像素值改变转变成控制信号控制显示器显示

*** 
- 用户告诉操作系统执行`HelloWorld` 程序(通过键盘输入等)
- 操作系统：找到`helloworld` 程序的相关信息，检查其类型是否是可执行文件；并通过程序首部信息，确定代码和数据在可执行文件中的位置并计算出对应的磁盘块地址。
- 操作系统：创建一个新进程，将`HelloWorld` 可执行文件映射到该进程结构，表示由该进程执行`helloworld` 程序。
- 操作系统：为`helloworld` 程序设置`cpu` 上下文环境，并跳到程序开始处。
- 执行`helloworld` 程序的第一条指令，发生缺页异常
- 操作系统：分配一页物理内存，并将代码从磁盘读入内存，然后继续执行`helloworld` 程序
- `helloword` 程序执行`puts` 函数(系统调用)，在显示器上写一字符串
- 操作系统：找到要将字符串送往的显示设备，通常设备是由一个进程控制的，所以，操作系统将要写的字符串送给该进程
- 操作系统：控制设备的进程告诉设备的窗口系统，它要显示该字符串，窗口系统确定这是一个合法的操作，然后将字符串转换成像素，将像素写入设备的存储映像区
- 视频硬件将像素转换成显示器可接收和一组控制数据信号
- 显示器解释信号，激发液晶屏
- `OK`，我们在屏幕上看到了`HelloWorld`

# 2. C++ STL

## 2.1 `map`和`set`有什么区别，分别又是怎么实现的？
- `map`和`set`都是`STL`中的关联容器，其底层实现都是红黑树(`RB-Tree`)。由于 `map` 和`set`所开放的各种操作接口，`RB-tree` 也都提供了，所以几乎所有的`map`和`set`的操作行为，都只是转调`RB-tree`的操作行为。

- `map`和`set`区别在于：
	- `map`中的元素是`key-value(关键字—值)对`：关键字起到索引的作用，值则表示与索引相关联的数据；`set`只是关键字的简单集合，它的每个元素只包含一个关键字。
	- `set`的迭代器是`const`的，不允许修改元素的值；而`map`虽然不允许修改`关键字(Key)`,但是允许修改`value`。
		 其原因是`map`和`set`都是根据关键字排序来保证其有序性的，如果允许修改`key`的话，那么首先需要删除该键，然后调节平衡，再插入修改后的键值，调节平衡，如此一来，严重破坏了`map`和`set`的结构，导致`iterator`失效。所以`STL`中将`set`的迭代器设置成`const`，不允许修改迭代器的值；而`map`的迭代器则不允许修改`key`值，允许修改`value`值。
	- `map`支持下标操作，`set`不支持下标操作。
		`map`可以用`key`做下标，`map`的下标运算符`[ ]`将关键码作为下标去执行查找，如果关键码不存在，则插入一个具有该关键码和`mapped_type`类型默认值的元素至`map`中，因此下标运算符`[ ]`在map应用中需要慎用，`const_map`不能用，只希望确定某一个关键值是否存在而不希望插入元素时也不应该使用，`mapped_type`类型没有默认值也不应该使用。如果`find`能解决需要，尽可能用`find`。

## 2.2 请你来介绍一下`STL`的`allocator`
- `STL`的分配器用于封装`STL`容器在内存管理上的底层细节。
- 在`C++`中，其内存配置和释放包括两个关键之: `new`和`delete`：
	- `new`运算分两个阶段：1) 调用`::operator new`配置内存;2) 调用对象构造函数初始化对象
	- `delete`运算分两个阶段：1) 调用对象析构函数；2) 调用`::operator delete`释放内存
- 在`STL allocator`将以上阶段分作四个函数分别负责: `allocate函数`负责分配内存空间, `deallocate函数`负责内存释放, `construct`负责对象构造, `destroy`负责对象析构. 
- 为了提升内存管理效率, 减少申请小内存造成的内存碎片化问题, `SGI STL`采用两级分配至, 当分配空间的大小超过`128B`的时候,会使用第一级空间配置器, 当分配空间大小小于`128B`时,采用第二级空间配置器. 
	- 一级空间配置器直接使用`malloc`, `realloc`, `free`函数进行内存空间分配和释放.
	- 二级空间配置器使用内存池技术管理内存, 使用`16`个链表维护`8-128byte`的16级别的小内存块.

## 2.3 `STL`迭代器删除元素
- 这个主要考察的是迭代器失效的问题。
	- 对于序列容器`vector`,`deque`来说，使用`erase(itertor)`后，后边的每个元素的迭代器都会失效，但是后边每个元素都会往前移动一个位置，但是`erase`会返回下一个有效的迭代器
	- 对于关联容器map set来说，使用了erase(iterator)后，当前元素的迭代器失效，但是其结构是红黑树，删除当前元素的，不会影响到下一个元素的迭代器，所以在调用erase之前，记录下一个元素的迭代器即可。
	- 对于list来说，它使用了不连续分配的内存，并且它的erase方法也会返回下一个有效的iterator，因此上面两种正确的方法都可以使用。
- 注意：**map的底层结构是红黑树，`unordered map`底层结构是哈希表**

## 2.4 `STL`有什么基本组成
- `STL`主要由六大部分组成：`配置器`, `容器`, `迭代器`, `算法`, `适配器`, `仿函数`
- 他们之间的关系：
	- 配置器为容器提供空间, 它是对空间动态分配，管理和释放的实现
	- 迭代器实现了容器和算法的衔接, 算法通过迭代器获取容器中的内容
	- 仿函数可以协助算法完成各种操作，适配器用来套接适配仿函数
- **注意:迭代器不是指针是类末班，通过模拟指针的功能；封装了指针**

## 2.5 `vector`和`list`的区别，应用，越详细越好
- `vector`: 在堆上分配空间, 连续存储的容器, 支持动态调整空间大小
	- 底层实现：数组(`array`)
	- 容器内存空间增长:
		- `vector` 增加(插入)新元素时，如果未超过此时的容量(还有剩余空间)，那么直接添加到最后(插入指定位置), 然后调整迭代器。
		- 如果没有剩余空间了，则会重新配置原有元素个数的两倍空间，然后将原空间元素通过复制的方式初始化新空间，再向新空间增加元素，最后析构并释放原空间，之前的迭代器会失效。
	- 性能：
		- 访问：O(1)
		- 插入：
			- 在最后插入（空间够）：很快
			- 在最后插入（空间不够）：需要内存申请和释放，以及对之前数据进行拷贝。
			- 在中间插入（空间够）：内存拷贝
			- 在中间插入（空间不够）：需要内存申请和释放，以及对之前数据进行拷贝。
		- 删除：
			- 在最后删除：很快
			- 在中间删除：内存拷贝
	- 适用场景：经常随机访问，且不经常对非尾节点进行插入删除。
- `List`动态链表: 在堆上分配空间，每插入一个元数都会分配空间，每删除一个元素都会释放空间。
	- 底层：双向链表
	- 性能：
		- 访问：随机访问性能很差，只能快速访问头尾节点。
		- 插入：很快，一般是常数开销
		- 删除：很快，一般是常数开销
	- 适用场景：经常插入删除大量数据

- 区别：`底层`, `内存的连续性`, `插入和删除的影响`, `内存分配时机`, `随机访问性能`
	- `vector`底层实现是数组；`list`是双向 链表。
	- `vector`支持随机访问，`list`不支持。
	- `vector`是连续的内存空间，`list`不是。
	- `vector`在中间节点进行插入删除会导致内存拷贝，`list`不会。
	- `vector`一次性分配好内存，不够时才进行`2`倍扩容；`list`每次插入新节点都会进行内存申请。
	- `vector`随机访问性能好，插入删除性能差；`list`随机访问性能差，插入删除性能好。

- 应用
	- `vector`拥有连续的内存空间，因此支持随机访问，如果需要高效的随即访问，而不在乎插入和删除的效率，使用`vector`。
	- `list`拥有不连续的内存空间，如果需要高效的插入和删除，而不关心随机访问，则应使用`list`。

## 2.6 `STL vector`的实现，删除其中的元素，迭代器如何变化？为什么是两倍扩容？释放空间？

- vector相关函数:
	- `size / capacity`: 已用空间 / 总空间
	- `resize / reserve`: 改变容器的元素数目 / 概念容器的空间大小
	- `push_back / pop_back`: 尾插 / 尾减
	- `insert / erase`: 任意位置插入 / 任意位置删除
- 迭代器失效问题:
	- 在`capacity`内`insert`和`erase`都会导致在后续元素发生移动, 进而迭代器失效或则改变
	- 如果`insert`或则`push_back`导致空间不足, 则会发生整体的移动操作, 所有迭代器都将失效.
- 两倍扩容问题:
	- 为什么呈倍数扩容(时间复杂度更优)
		- 对于`n`次插入操作, 采用成倍方式扩容可以保证时间复杂度`O(n)`, 而指定大小扩容的时间复杂度为`O(n^2)`
	- 为什么是`1.5`倍扩容(空间可重用)
		- 当`k == 2`时:
			- 第n次扩容的时候需要分配的内存是：`an = a1*q^(n-1) = 4*2^(n-1)`
			- 而前n-1项的内存和为：`Sn = a1*(1-q^(n-1))/(1-q) = 4*(1-2^(n-1)) /(1-2) = 4*2^(n-1)-4`
			- `差值 = an - Sn = 4 > 0`
			- 所以第`n`次扩容需要的空间恰好比前`n-1`扩容要求的空间总和要大，那么即使在前`n-1`次分配空间都是连续排列的最好情况下，也无法实现之前的内存空间重用
		- 当`k = 1.5`时:
			- 第`n`次扩容的时候需要分配的内存是：`an = a1*q^(n-1) = 4*1.5^(n-1)`
			- 而前`n-1`项的内存和为：`Sn = a1*(1-q^(n-1))/(1-q) = 4*(1-1.5^(n-1)) /(1-1.5) = 8*1.5^(n-1)-8`
			- `差值 = an - Sn = 8 - 4*1.5^(n-1)`
			- 当`n`增长到一定的数值后，差值就会变为小于`0`，那么如果前`n-1`次分配的空间都是连续的情况下, 就可以实现内存空间复用
- 释放空间:
	- 使用`swap`: `vector<int>().swap(a);`

***
- 参考: 
	- [STL中vector 扩容为什么要以1.5倍或者2倍扩容？ - Bryant_xw Is Growing~~ - CSDN博客](https://blog.csdn.net/bryant_xw/article/details/89524910)
	- [C++ STL中vector内存用尽后，为啥每次是两倍的增长，而不是3倍或其他数值？ - 知乎](https://www.zhihu.com/question/36538542/answer/67929747)

***