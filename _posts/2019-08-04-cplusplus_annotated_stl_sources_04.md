---
layout:     post
title:      STL 源码剖笔记(四)
subtitle:   STL 源码剖析笔记(四)
date:       2019-07-06
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - C++
    - 基础编程
    - STL源码解析
---

> 2019-08-04 21:47:52


## 第七章 仿函数

仿函数也叫对象函数；在STL提供的各种算法中，都允许用户自定义相关算法。以结果的false或者true来进行相关的排序操作，用来执行函数的就是仿函数。一般步骤是先设计一个函数，在将函数的相关指针指向函数对应的结果。

![STL仿函数与STL算法之间的关系](https://wangpengcheng.github.io/img/2019-08-11-10-20-39.png)

仿函数的分类：

- 操作数的个数：一元和二元仿函数
- 功能划分:算术运算、关系运算、逻辑运算。

使用STL内建的仿函数，都必须含有<functional>头文件。

### 7.2 可配接(Adaptable)的关键

- unary_function 用来呈现一元函数的参数型别和返回值类型。
- binary_function 用来呈现二元函数的参数型别和返回值类型。

```c++

template <class Arg1,class Arg2,class Result>

struct binary_function
{
    typdef Arg1 first_argument_type;
    typdef Arg2 second_argument_type;
    typdef Result result_type;
};

```

### 7.3 算术类(Arithmetic)仿函数

- 加法：plus<T>
- 减法：minus<T>
- 乘法:multiplies<T>
- 除法：divides<T>
- 膜取(modulus):modulus<T>
- 否定(negation):negate<T>

```c++

template <class T>
struct plus:public binary_function<T,T,T>
{
    T operator()(const T& x,const T& y) const {return x+y;}
};

```

**证同元素**

数值A若与该元素做OP运算，会得到A自己。加法的同证元素为0，因为任何匀速加上0仍为自己。乘法的同证元素为1，因为任何元素乘以1任然为自己

### 7.4 关系运算类仿函数
他们都是二元的运算函数

- 等于：equal_to<T>
- 不等于：not_equal_to<T>
- 大于: greater<T>
- 大于或者等于：greater_equal<T>
- 小于：less<T>
- 小于或者等于:less_equal<T>

### 7.5 逻辑运算类仿函数

其中And和Or为二元运算符，Not为一元运算符

- And:logical_and<T>
- Or:logical_or<T>
- Not:logical_not<T>

### 7.6 证同(identity)、选择(select)、投射(project)

- 证同：任何数值通过此函数后，不会有任何改变，此式运用于<stl_set.h>用来指定RB-tree所需要的KeyOfValue op ，因为set元素的键值即实值，所以采用identity

```c++

template <class T>
struct identity:public unary_function<T,T>
{
    const T& operator(){const T& x} const {return x;}
};
```
- 选择： 接受一个pair,传回第一元素。此式运用于<stl_map.h>用来指定RB-tree所需要的KeyOfValue op ，因为map系以pair元素的第一个元素为其键值，所以采用select1st。

- 投射函数：传回第一参数，忽略第二参数

## 第8章 配接器

Adapter实际上是一种设计模式，将一个class的借口转换为另外一个class的接口。

### 8.1 配接器之概观与分类

- function adapter:改变仿函数(functors)接口
- container adapter:改变容器(container)接口
- iterators adapter:改变迭代器借口

![function adapters及其辅助函数](https://wangpengcheng.github.io/img/2019-08-11-13-58-31.png)

### 8.2 container adapter

stack和queue的底层都是使用deque构成。

### 8.3 iterator adapters
insert  iterator底层的调用是push_front()或者push_back()或者insert()操作函数。

#### 8.3.2 reverse iterators


```c++
deque<int> id={32,26,99,1,0,1,2,3,4,0,1,2,5,3};
//32

cout <<*(id.begin())<<endl;
//3

cout <<*(id.rbegin())<<endl;
//0

cout <<*(id.end())<<endl;
//0

cout <<*(id.rend())<<endl;
//查找对应的值

deque<int>::iterator ite=find(id.begin(),id.end(),99);
reverse_iterator<deque<int>::iterator> rite(ite);
//99

cout<<*ite<<endl;
//26

cout<<*rite<<endl;
```

注意，由于迭代器区间的“前闭后开”的习惯，当迭代器被逆向方向时，虽然实体位置(真正的地址)不变，但其逻辑位置(迭代器所代表的元素)改变了(必须如此改变)：因此正向和逆向迭代器所取出的是不同的元素：

![当迭代器被逆转](https://wangpengcheng.github.io/img/2019-08-11-14-20-30.png)

对逆向迭代器取值，就是将“对应之正向迭代器”后退一格而后取值。

#### 8.3.3 stream iterators

可以将一个迭代器绑定到一格stream对象上。绑定到istream对象例如(std::cin)者，就是istream_iterator拥有输入能力，同理输出对象上有输出能力。

![copy istream_iterator](https://wangpengcheng.github.io/img/2019-08-11-15-13-25.png)

![copy ostream_iterator](https://wangpengcheng.github.io/img/2019-08-11-15-14-45.png)

### 8.4 function adapters

容器是以class templates完成，算法是以function templates完成，仿函数是一种将operator()重载的class template,迭代器则是一种将operator++和operator*等指针习惯性常行为重载的class template。

function adapters也内藏了一格member object

![](https://wangpengcheng.github.io/img/2019-08-11-15-22-14.png)

因此function adapters多是在内部实现函数的合成与操作

#### 8.4.4 用于函数指针： ptr_fun

这种配接器使得我们能够将一般函数当做仿函数使用。一般函数当做仿函数传给STL算法。其实质就是把一个函数指针包裹起来。

```c++
//当仿函数被使用时，就调用该函数指针,这里是一元函数指针的封装

template <class Arg,class Result>
class pointer_to_unary_function:public unary_function<Arg,Result>
{
protected:
    //内部成员，一个函数指针

    Result (*Ptr)(Arg);
public:
    pointer_to_unary_function(){}
    //以下constructor将函数指针记录于内部成员之中

    explicit pointer_to_unary_function(Result (*x)(Arg)):ptr(x){}
    //以下，通过函数指针指向函数

    Result operator()(Arg x) const {return ptr(x);}
};
//辅助函数，让我们得以方便运用

template <class Arg,class Result>
inline pointer_to_unary_function<Arg,Result> ptr_fun(Result (*x)(Arg))
{
    return pointer_to_unary_function<Arg,Result>(x);
}

//二元函数指针的封装

template <class Arg1,class Arg2,class Result>
class pointer_to_binary_function:public binary_function<Arg1,Arg2,Result>{
protected:
    //内部成员，一个函数指针

    Result (*ptr)(Arg1,Arg2);
public:
    pointer_to_binary_function(){}
    //将函数指针记录在内部成员之中

    explicit pointer_to_binary_function(Result (*x)(Arg1,Arg2)):ptr(x){}
    //函数指针执行函数

    Result operator()(Arg1 x,Arg2 y) const {return ptr(x,y);}
};
//辅助函数，方便函数的使用

template<class Arg1,class Arg2,class Result>
//定义返回类型

inline pointer_to_binary_function<Arg1,Arg2,Result>
ptr_fun(Result (*x)(Arg1,Arg2))
{
    return pointer_to_binary_function<Arg1,Arg2,Result>(x);
}
```

#### 8.4.5 用于成员函数指针：mem_fun,mem_fun_ref；

这种配接器使得我们能够将成员函数当做仿函数来进行使用，使得成员函数可以搭配各种泛型算法。

![类阶层体系](https://wangpengcheng.github.io/img/2019-08-11-15-48-03.png)