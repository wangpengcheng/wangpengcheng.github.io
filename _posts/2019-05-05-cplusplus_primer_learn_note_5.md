---
layout:     post
title:      C++ Primer 学习笔记 (五)
subtitle:   C++ Primer 学习记录 (五)
date:       2019-5-5
author:     敬方
header-img: img/post-bg-miui6.jpg
catalog: true
tags:
    - C++
    - 基础编程
    - std 标准库
---

> 2019-5-5 21:25:45 终于开始进入c++标准库的学习了

# 第II部分 C++ 标准库

## 第8章 IO库

C++中没有直接的输入和输出库，而是使用标准库的IO库来进行IO操作，下面让我们进入IO的世界吧

### 8.1 IO类
IO基本类和头文件如下表：

IO库的类型和对应头文件

|头文件|类型|
|:---|:---|
|`iostream`|`istream,wistream` 从流读取数据；`ostream,wostream` 从流输出数据；`iostream,wiostream`读写流|
|`fstream`|`ifstream,wifstream` 从文件读取数据；`ofstream,wofstream` 从文件写入数据；`fstream,wfstream` 读写文件；|
|`sstream`|`istringstream,wistringstream`从`string`读取数据;`ostringstream,wostringstream`向`string`写入数据;`stringstream,wstringstream` 读写`string`|

注意：

- IO对象无拷贝或者赋值--通过重载拷贝构造函数和赋值操作，禁止拷贝和赋值；或者**将拷贝构造函数与赋值函数，声明为private，并且不给出实现**
- 在函数后面使用`=delete`；例如：`NoCopyable(const NoCopyable&) = delete;`;其中
delete表明函数已删除。使用会报错："尝试使用已删除的函数";（参考：[C++禁止对象拷贝](https://blog.csdn.net/qqqqqqqqqq1qqqqqqqqq/article/details/46006791)）

**c++中的浅拷贝和深拷贝**

_参考链接：_ [C++本质：类的赋值运算符=的重载，以及深拷贝和浅拷贝](https://www.cnblogs.com/zsq1993/p/5791130.html)；[C++的拷贝构造函数、operator=运算符重载，深拷贝和浅拷贝、explicit关键字](https://blog.csdn.net/shine_journey/article/details/53081523)；

c++默认的拷贝构造函数和赋值运算符都是简单的浅拷贝，直接使用其它变量初始化对象，并没有重新分配内存，**但是如果实行浅拷贝，也就是把对象里的值完全复制给另一个对象，如A=B。这时，如果B中有一个成员变量指针已经申请了内存，那A中的那个成员变量也指向同一块内存。这就出现了问题：当B把内存释放了（如：析构），这时A内的指针就是野指针了，出现运行错误。**因此类的成员变量需要动态开辟堆内存，此时，深拷贝尤为重要。默认类的缺省函数都是浅拷贝函数。简单示例如下：

```c++
//原始浅拷贝

#include <iostream>
using namespace std;

class Data {
public:
    Data() {}
    Data(int _data) :data(_data)
    {
        cout << "constructor" << endl;
    }
    //简单赋值浅拷贝

    Data& operator = (const int _data)
    {
        cout << "operator = (const int _data)" << endl;
        data = _data;
        return *this;
    }
private:
    int data;   
};

int main()
{
    Data data1(1);
    Data data2, data3;
    data2 = 1;
    data3 = data2;//调用编译器提供的默认的赋值运算符重载函数，当自定义析构函数的时候会存在问题
 
    return 0;
}

//深拷贝

#include <iostream>
#include <string>
using namespace std;

class MyStr {
public:
    MyStr() {}
    MyStr(int _id, char *_name)
    {
        cout << "constructor" << endl;
        id = _id;
        name = new char[strlen(_name) + 1];
        strcpy_s(name,strlen(_name) + 1,_name); 
    }
    //深拷贝构造函数

    MyStr(const MyStr &str)
    {
        cout << "copy constructor" << endl;
        id = str.id;
        if (name != NULL)
            delete name;
        name = new char[strlen(str.name) + 1];
        //拷贝数据

        strcpy_s(name,strlen(str.name) + 1,str.name);
    }

    MyStr& operator=(const MyStr& str)
    {
        cout << "operator=" << endl;
        if (this != &str)
        {
            if (name != NULL)
                delete name;
            this->id = str.id;
            name = new char[strlen(str.name) + 1];
            //拷贝字符长度

            strcpy_s(name,strlen(str.name) + 1,str.name);

            return *this;
        }
    }
    ~MyStr() 
    {
        cout << "deconstructor" << endl;
        delete name;
    }
private:
    char *name;
    int id;
};

void main()
{
    MyStr str1(1,"Jack");
    MyStr str2;
    str2 = str1;
    MyStr str3 = str2;
    return;

}


```

IO库条件状态：

|状态|含义|
|:---|:---|
|`strm::iostate`|strm是一种IO类型。iostream是一种机器相关的整型提供了表达条件状态的完整功能|
|`strm::badbit`|`strm::badbit`用来指出流已崩溃|
|`strm::failbit`|`strm::failbit`用来指出一个IO操作失败了|
|`strm::eofbit`|`strm::eofbit`用来指出流达到了文件结束|
|`strm::goodbit`|`strm::goodbit`用来指出流未处于错误状态。此值保证为零|
|`s.eof()`|流 s 的 `eofbit` 置位，则返回 `true`|
|`s.fail()`|流 s 的 `failbit` 或 `badbit` 置位，则返回 true|
|`s.bad()`|流 s 的 badbit 置位，则返回 true|
|`s.good()`|若流 s 处于有效状态，则返回 true|
|`s.clear()`|将流 s 中的所有条件状态位复位，将流的状态设置为有效。返回void|
|`s.clear(flag)`|根据给定的 flags 标志位，将流 s 中对于条件状态位复位。 flag 的类型是`strm::iostate`。返回 void|
|`s.setstate(flag)`|根据给定的 flags 标志位，将流 s 中对于条件状态位置位。 flag 的类型是strm::iostate。返回 void|
|`s.rdstate()`|返回流 s 的当前条件，返回值类型为 `strm::iostate`|

**四种条件状态**

|状态|含义|数值|`good()`|`eof()`|`bad()`|`fail()`|`rdstate()`|
|:---|:---|:---|:---|:---|:---|:---|:---|
|`ios::goodbit`|流状态完全正常|0|1|0|0|0|`goodbit`|
|`ios::eofbit`|已达到文件结束|2|0|1|0|0|`eofbit`|
|`ios::badbit`|输入（输出）流出现非致命错误，可挽回|1|0|0|1|0|`badbit`|
|`ios::failbit`|输入（输出）流出现致命错误，不可挽回|4|0|0|0|1|`failbit`|

**文件结束**

|状态|解释|
|:---|:---|
|`strm::eofbit`|`strm::eofbit`用来指出流达到了文件结束|
|`s.eof()`|流 s 的 `eofbit` 置位，则返回 `true`|

注意：

- 一个流一旦发生错误，其上后续的IO操作都会失败。因此一般使用`while(cin>>word)`来检测是否成功。

iostate 类型用来表示流状态的完整功能。使用示例如下：

```c++
auto old_state=cin.restate(); //记住cin的状态

cin.clear();//使得cin有效

process_input(cin); //使用cin

cin.setstate(old_state);//设置cin为原有状态。

cin.clear(cin.rdstate() & ~cin.failbit & ~cin.badbit);//复位failbit和badbit,保持其他标志位不变。

```

#### 管理输出缓冲

每个输出流都管理一个缓冲区，用来保存程序读写的数据；例如`os<<"Please enter a value:";`中文本串可能被立即打印出来，但也有可能被操作系统保存在缓冲区中，随后打印。

注意：如果程序异常崩溃，输出缓冲区不会更新，数据很可能停留在缓冲区中等待打印。因此当程序崩溃后，需要确定已经输出的数据确实已经刷新了，否则可能将大量时间浪费在追踪代码为什么没有执行上面。

输入和输出流也是可以正常关联的：利用`ostream`的`tie`实现相关操作。`tie()`当前关联到的输出流，`tie(&o)`关联到o输出流。多个流可以关联到同一个`ostream`;下面是使用示例：

```c++
cin.tie(&cout); //将标准cin与cout关联在一起。

ostream *old_tie=cin,tie(nullptr);//关联空流

cin.tie(&cerr); //读取cin会刷新cerr而不是cout

cin.tie(old_tie); //重建cin和cout间的正常关联。

```

#### 文件输入输出
_参考链接:_ [fstream参考](http://www.cplusplus.com/reference/fstream/fstream/)；

fstream特有操作：

|名称|操作|
|:---|:---|
|`fstream fstrm`|创建一个未绑定的文件流|
|`fstream fstrm（s）`|创建一个fstream，并打开名为s的文件。默认的文件模式mode依赖于fstream的类型|
|`fstream fstrm（s，mode）`|与前一个构造函数类似，但按指定mode打开文件|
|`fstrm.open(s)`|打开名为s的文件，并将文件与fstrm绑定，默认的文件mode依赖于fstream的类型，返回void|
|`fstrm.close()`|关闭与fstrm绑定的文件，返回void|
|`fstrm.is_open()`|返回一个bool值，指出与fstrm关联的文件是否成功打开且尚未关闭|

文件的打开有两种方式：

-  在初始化输入输出对象时，直接构建；如： `ifstream in(ifile)`
-  先声明，再使用`open`函数关联文件；例如：

```c++
std::string ifile="c:/Windows/assembly/test";
ifstream in(ifile); //构筑一个ifstream并打开给定文件

ofstream out; //输出文件流未与任何文件相关联。

out.open(ifile+".txt");//打开指定文件

in.close(); //关闭文件


```

注意： 
- 当一个`fstream`对象呗销毁时,`close`会自动被调用。
- 默认模式打开文件进行输出将自动打开文件的长度截为零，即 **删除已有内容**。 （细节！！） 

文件读写模式：

|模式|作用|
|:---|:---|
|`in`|读方式打开|
|`out`|写方式打开|
|`app`|每次写操作前均定位到文件末尾|
|`ate`|打开文件后立即定位到文件末尾|
|`trunc`|截断文件|
|`binary`|二进制方式进行io|

注意：以`out`模式打开文件会丢弃已有数据，因此常需要附加`app`模式；例如：

```c++
ofstream out("file1");// 输出模式打开文件并截断文件

ofstream out2("file1",ofstream::out);//隐含第截断文件

ofstream out3("file1",ofstream::out | ofstream::trunc);
//保留文件内容显式指定app模式。

ofstream app("file2",ofstream::app);
//隐含为输出模式 

ofstream app2("file2",ofstream::out | ofstream::app);
```

每次使用open函数都可以再次更改文件模式。
文件流使用示例：

```c++
# include<iostream>
# include<fstream>
# include<string>
using namespace std;
 
int main()
{
    //读取初始文件内容，并输出显示
    
    ifstream in;
    in.open("guest.txt");
    char ch;
    if (in.is_open())
    {
        cout << "Here are the current contents of thepeople.text's file:" <<endl;
        while (in.get(ch))
            cout << ch;
        cout << endl;
        in.close();
 
    }
 
    // 添加内容到文件
    
    ofstream out;
    out.open("D:guest.txt", ofstream::app);  
    //out.open("people.txt", ios_base::app);
    
    if (!out.is_open())
    {
        cout << "failed to open file" << endl;
    }
    cout << "Please enter content:";
    string str;
    while (getline(cin, str) && str.size() > 0)
    {
        out << "\n"<<str << endl;
    }
    out.close();
    //读取修改后的文件内容，并输出显示

    in.open("guest.txt");
    if (in.is_open())
    {
        cout << "Here are the revised contents of the people.text's file:" << endl;
        while (in.get(ch))
            cout << ch;
        cout << endl;
        in.close();
    }
 
    return 0;
 
}

```

### string 流

string相关流继承关系如下图：

![相关继承关系](http://www.pconline.com.cn/pcedu/empolder/gj/c/0504/pic/08cppios01.gif)

stringstream的特有操作

|操作|含义|
|:---|:---|
|`sstream strm`|定义字符串头文件类型|
|`sstream strm(s)`|保存string s的一个拷贝|
|`strm.str()`|返回strm所保存的`string`的拷贝|
|`strm.str(s)`|将`string s` 拷贝到`strm`中。返回`void`|

## 第 9 章顺序容器

容器是一些特定类型对象的集合。顺序容器为程序员提供了控制元素存储和访问顺序的能力。下面是简介表

|类型|特点|
|:---|:---|
|`vector`|可变大小数组。支持快速随机访问。在尾部之外的位置插入或者删除元素|
|`deque`|双端队列。支持快速随机访问。在头尾插入/删除速度很快|
|`list`|双向链表。只支持双向顺序访问。在`list`中任何位置进行插入/删除操作速度都很快|
|`forward_list`|单向链表。只支持单向顺序访问。在链表任何位置进行插入/删除操作速度都很快|
|`array`|固定大小数组。支持快速随机访问。不能添加或者删除元素|
|`string`|与`vector`相似的容器，但是专门保存字符。随机访问快。在尾部插入/删除速度很快|

下面是一些选择容器的基本原则：

- 除非有很好的理由，否则应使用`vector`
- 如果你的程序有很多小元素，且额外开销很重要，则不要使用`list`或者`forward_list`。
- 如果程序要求随机访问元素，应该使用`vector`或者`deque`。
- 如果程序要求在容器的中间插入或者删除元素，应该使用`list`或者`forward_list`。
- 如果程序需要在头尾位置插入或者删除元素，但不会在中间位置进行插入，则使用`deque`。
- 如果程序只有在读取输入时才需要在容器中间位置插入元素，随后需要随机访问元素，则：
    + 首先，确定是否真的需要在容器中间位置添加元素，当处理输入数据时，通常可以很容易地向`vector`追加数据，然后再调用标准库`sort`函数来重新排列容器中的元素，从而避免在中间位置添加元素。
    + 如果必须在中间位置插入元素，考虑在输入阶段使用`list`，一旦输入完成，将`list`中的内容拷贝到一个`vector`中。

容器操作：


|类型别名|操作|
|:---|:---|
|`iterator`|此容器类型的迭代器类型|
|`const_iterator`|可以读取元素，但不能修改元素的迭代器类型|
|`size_type`|无符号整数类型，足够保存此种容器类型最大可能容器的大小|
|`difference_type`|带符号整数类型，足够保存两个迭代器之间的距离|
|`value_type`|元素类型|
|`reference`|元素的左值类型；与`value_type&`含义相同|
|`const_reference`|元素的`const`左值类型(即，`const value_type&`)|


|构造函数|操作|
|:---|:---|
|`C c`|默认构造函数，构造空容器(array)|
|`C c1(c2)`|构造`c2`的拷贝`c1`|
|`C c(b,e)`|构造`c`,将迭代器`b`和`e`指定的范围内的元素拷贝到`c`|
|`C c{a,b,c ...}`|列表初始化`c`|


|赋值与`swap`|操作|
|:---|:---|
|`c1=c2`|将`c1`中的元素替换为`c2`中元素|
|`c1={a,b,c...}`|将`c1`中的元素替换为列表中元素(不适用于array)|
|`a.swap(b) or swap(a,b) `|交换a和b的元素|


|大小|操作|
|:---|:---|
|`c.size()`|c中元素的数目(不支持`forward_list`)|
|`c.max_size()`|c可保存的最大元素数目|
|`c.empty()`|若`c`中存储了元素，返回`fasle`,否则返回`true`|

|添加/删除元素|操作|
|`c.insert(args)`|将`args`中的元素拷贝进`c`|
|`c.emplace(inits)`|适用`inits`构造`c`中的一个元素|
|`c.clear()`|删除`c`中所有的元素，返回`void`|

|关系运算符|解释|
|:---|:---|
|`==,!=`|所有容器都支持相等(不等)运算符|
|`<,<=,>,>=`|关系运算符(无序关联容器不支持)|

|获取迭代器|解释|
|:---|:---|
|`c.begin(),c.end()`|返回指向`c`的首元素和尾元素之后位置的迭代器|
|`c.cbegin(),c.cend()`|返回`const_iterator`|

|反向容器的额外成员(不支持`forward_list`)|解释|
|:---|:---|
|`reverse_iterator`|按逆序寻址|
|`const_reverse_iterator`|不能修改元素的逆序迭代器|
|`c.rbegin(),c.rend()`|返回指向cz的尾元素和首元素之前位置的迭代器|
|`c.crbegin(),c.crend()`|返回`const_reverse_iterator`|

**迭代器**

迭代器范围是标准库的基础，标砖库的很多操作都是基于迭代器产生的。`begin`迭代器迭代器指向迭代器开头的元素；`last`迭代器指向尾元素之后的元素；即不是指向尾部元素，元素范围为**左闭右开**

代码示例：

```c++
list<string> a={"string1","string2","string3"};
auto it1=a.begin(); //lsit<string>::iterator;

auto it2=a.rbegin();//list<string>::reverse_iterator;

auto it3=a.cbegin(); //list<string>::const_iterator;

auto it4=a.crbegin();//list<string>::const_reverse_iterator
```

注意：

- 顺序容器不需要写访问时，应该尽量使用`cbegin`和`cend`；
- 只有顺序容器不包括(array)的构造函数才能接受大小参数
- 拷贝构造只有当容器类型匹配时才能成立，
- 当将一个容器初始化为另外一个容器的拷贝的时候，两个容器的容器类型和元素类型都必须相同。

**标准库array具有固定大小**
与内置数组一样，标准库array的大小也是类型的一部分。当定义一个`array`时，除了指定元素类型，还要指定容器大小；使用array类型，也必须指定元素类型的大小；array初始化会自动给与0值。内置数组不允许拷贝，但是array类型允许拷贝。

```c++
array<int ,42>; //类型为：保存42个int的数组

array<string,10>; //类型为：保存10个 string的数组

array<int,10>::size_type i;//数组类型包括元素类型和大小

array<int>::size_type j; //错误:array<int>不是一个类型

```

**替换操作 `assign`**

|函数|功能|
|:---|:---|
|`seq.assign(b,e)`|将seq中的元素替换为迭代器b和e所示到范围中的元素。迭代器b和eb不能指向seq中的元素|
|`seq.assign(li)`|将seq中的元素替换为初始化列表li中的元素|
|`seq.assign(n,t)`|将seq中的元素替换为n个值为t的元素|

使用：`vector<int > b(10,5);vector<int> a;a.assgin(b.cbegin(),b.cend());`

注意：赋值运算会导致指向左边容器内部的迭代器、引用和指针失效。而`swap`操作将容器内容交换不会导致容器的迭代器、引用和指针失效(容器类型为array和string的情况除外)；例如，假定`iter`在`swap`之前指向`svecl[3]`中的string,那么在`swap`之后它指向`svec2[3]`的元素。但是array会真正交换他们的元素。因此交换两个array所需的时间与array中元素的数目成正比。

##### 关系运算符
每个容器类型都支持相等运算符(==和!=)除了无序关联容器外，都支持关系运算符(>、>=、<、<=)。关系运算符两边必须是相同类型的容器，且必须保存相同类型的元素。元素相等比较规则如下：

1. 如果两个容器具有相同大小且所有元素都两两对应相等，则这个容器相等，否则不相等。
2. 如果两个容器大小不相同，但较小容器中每个元素都等于较大容器中的对应元素，则较小容器小于较大容器
3. 如果两个容器都不是另外一个容器的前缀子序列，则他们的比较结果取决于第一个不相等的元素的比较结果。
4. 只有当其元素类型也定义了相应的比较运算符的时候，我们才可以使用关系运算符来，比较两个容器。

##### 向顺序容器中添加元素

|操作名称|作用|
|:---|:---|
|`c.push_bcak(t)`|在c的尾部创建并插入一个元素|
|`c.push_back(t)`|在c的头部创建并插入一个元素|
|`c.insert(p,t)`|在迭代器p指向的元素之前创建插入一个元素，返回指向新添加元素的迭代器|
|`c.insert(p,n,t)`|在迭代器p指向的元素之前创建插入n个元素，返回指向新添加元素第一个元素的迭代器|
|`c.insert(p,b,e)`|将迭代器b和e指定的范围内的元素插入到迭代器p指向的元素之前，返回指向新添加元素第一个元素的迭代器；若p为空则返回p|
|`c.insert(p,il)`|il是一个花括号包围的元素值列表。将这些给定值插入到迭代器p指向的元素之前，返回指向新添加的第一个元素的迭代器|

注意： 

- 向一个`vector`、`string`或者`deque`插入元素会使所有指向容器的迭代器、引用和指针失效。
- 容器元素插入和初始化都是拷贝
- 使用插入`insert`的时候会返回插入的值

**使用emplace操作**

新标准中引入了三个新成员函数-`emplace_front`、`emplace`和`emplace_back`它们与`insert`系列操作相同，
注意：

- `emplace`函数在容器中直接构造元素，传递给`emplace`函数的参数必须与元素类型的构造函数相匹配。

例如：
```c++
c.emplace_back("99999",25,15.99);//错误没有接受三个参数的`push_back`版本

c.push_back("99999",25,15.99);//正确创建一个临时的`push_back`版本
```

##### 顺序容器中访问元素操作

|操作|含义|
|:---|:---|
|`c.back()`|返回c中尾元素的引用。若c空，函数行为未定义|
|`c.front()`|返回c中首元素的引用。若c空，函数行为未定义|
|`c[n]`|返回c中下标为n元素的引用。若c空，n>=c.size()函数行为未定义|
|`c.at(n)`|返回c中下标为n元素的引用。若越界抛出异常|

建议：尽量使用at函数，避免下标越界


##### 顺序容器中删除元素操作

|操作|含义|
|:---|:---|
|`c.pop_back()`|删除c中尾部元素，若c空，函数行为未定义|
|`c.pop_front()`|删除c中首元素的引用。若c空，函数行为未定义|
|`c.erase(n)`|删除迭代器p所指的元素，返回一个指向被删除元素之后元素的迭代器|
|`c.erase(b,e)`|删除迭代器b和e所指定范围内的元素。返回指向最后一个被删除元素之后元素的迭代器|
|`c.clear()`|删除c中所有元素。返回`void`|

注意对于单链表而言有许多不同之处

![链表操作](http://wangpengcheng.github.io/img/fowrd_list_insert.png)

当在forword_list中添加或者删除元素时，我们必须关注两个迭代器-一个指向我们要处理的元素，另外一个指向其前驱元素。例如：

```c++
forward_list<int> flst={0,1,2,3,4,5,6,7,8,9}；
auto prev=flst.before_begin(); //表示flst的首前元素

auto curr=flst.begin();   //表示flst中的第一个元素

while(curr!=flst.end()) {
    if(*curr%2){
        curr=flst.erase_after(prev); //删除并移动curr

    }else{
        prev=curr;//移动迭代器curr,指向下一个元素，prev指向curr之前的元素
        
        ++curr;
    }
}


```
##### 容器操作可能会使迭代器失效

_参考链接：_ [C++迭代器失效的几种情况总结](https://www.cnblogs.com/fnlingnzb-learner/p/9300073.html)；[C++之迭代器失效及解决](https://blog.csdn.net/Yinghuhu333333/article/details/80744440)；

在向容器中添加元素之后：

- 如果容器是`vector`或者`string`，且存在存储空间被重新分配，则指向容器的迭代器、指针和引用内存都会失效。如果没有重新分配内存，指向插入位置之前的元素的迭代器、指针和引用仍然有效，但是指向插入之后的元素迭代器、指针和引用都会失效
- 对于`deque`插入到除首尾位置之外的任何位置都会导致迭代器、指针和引用失效。如果在首尾位置添加元素，迭代器会失效，但是指向存在的元素的引用和指针不会失效。
- 对于`list`和`forward_list`指向容器的迭代器（包括尾后迭代器和首前迭代器）、指针和引用仍有效。

在删除一个元素后

- 对于`list`和`forward_list`指向容器其它位置的迭代器、引用指针任然有效。
- 对于`deque`如果在首尾之外的任何位置删除元素，那么指向被删除元素之外其它元素的迭代器、引用或者指针也会失效。如果删除的是尾部元素，则尾后的迭代器也会失效，但其它迭代器、引用和指针不受影响；如果删除首尾元素，这些也不会受影响。
- 对于`vector`和`string`,指向被删除元素之前元素的迭代器、引用和指针仍然有效。注意当我们删除元素时，尾后迭代器总是会失效。

因为添加删除原来的元素后，`end`迭代器总是会失效，因此尽量不要保存`end`返回的迭代器

vector和string 的容器大小管理操作
`capacity()`容器在不扩张内存空间的情况下可以容纳多少个元素，`reserve`允许我们通知容器它应该准备保存多少个元素


|操作|含义|
|:---|:---|
|`c.shrink_to_fit()`|将`capacity（）`减小为和`size`相同大小|
|`c.capacity()`|不重新分配内存空间的话，c可以保存多少元素|
|`c.reserve(n)`|分配至少能容纳n个元素的内存空间|

注意： reserve并不改变容器元素中的数量，它仅仅影响vector预先分配多大的内存空间。


##### 额外的string操作

|操作|含义|
|:---|:---|
|`s.insert(pos,args)`|在迭代器pos之前插入args指定的字符|
|`s.erase(pos,len)`|删除从位置`pos`开始的`len`个字符。如果len 被省略则删除从`pos`开始至s末尾的所有字符。返回一个指向`s`的引用|
|`s.assign(args)`|将`s`中的字符串替换为`args`指定的字符。返回一个指向`s`的引用|
|`s.append(args)`|将`s`后添加`args`指定的字符。返回一个指向`s`的引用|
|`s.replace(range,args)`|删除`range`范围内的字符，替换为`args`指定的字符，返回`s`的引用|


**string搜索操作**

|操作|含义|
|:---|:---|
|`s.find(args)`|查找`s`中第一次出现的位置|
|`s.rfind(args)`|查找`s`中最后一次出现的位置|
|`s.find_frist_of(args)`|在`s`中查找`args`中任何一个字符，第一次出现的位置|
|`s.find_last_of(args)`|在`s`中查找`args`中任何一个字符，最后一次出现的位置|
|`s.find_frist_not_of(args)`|在`s`中查找第一个不在`args`中的字符|
|`s.find_last_not_of(args)`|在`s`中查找最后一个不在`args`中的字符|

```c++
//循环查找下一个数

string::size_type pos=0;
while((pos=name.find_frist_of(number,pos))!=string::npos) {
    cout<<"found number at index: "<<pos
        <<"element is "<<name[pos]<<endl;
        ++pos;//移动到下一个字符    
}

```

`compare`比较字符串；`to_string()`将数字转化为字符串。

**string和数值之间的转换**

|操作|含义|
|:---|:---|
|`to_string(val)`|将任意一种算术类型`val`转化为`string`|
|`stoi(s,p,b)`|返回s的起始字符子串(整数内容)的数值 `int`,b是转换基数，p是`size_t`指针|
|`stol(s,p,b)`|返回s的起始字符子串(整数内容)的数值 `long`,b是转换基数，p是`size_t`指针|
|`stoul(s,p,b)`|返回s的起始字符子串(整数内容)的数值 `unsigned long`,b是转换基数，p是`size_t`指针|
|`stoll(s,p,b)`|返回s的起始字符子串(整数内容)的数值 `long long`,b是转换基数，p是`size_t`指针|
|`stoull(s,p,b)`|返回s的起始字符子串(整数内容)的数值 `unsigned long long`,b是转换基数，p是`size_t`指针|
|`stof(s,p)`|返回s的起始字符子串(整数内容)的数值 `float`,b是转换基数，p是`size_t`指针|
|`stod(s,p)`|返回s的起始字符子串(整数内容)的数值 `long long`,b是转换基数，p是`size_t`指针|
|`stold(s,p)`|返回s的起始字符子串(整数内容)的数值 `long double`,b是转换基数，p是`size_t`指针|

### 容器适配

出来标准容器外还有三个顺序容器适配器： `stack`、`queue`和`proiority_queue`。本质上，一个适配器是一种机制，能使某种事物的行为看起来像另外一种事物一样。例如`stack`适配器接受一个顺序容器，并使其操作像一个stack一样。下面是使用示例：

```c++
stack<int> stk(deq);//从deq拷贝元素到stk

stack<string ,std::vector<string> > str_stk;//在vector上实现的空栈

stack<string ,vector<string > > str_stk2(svec);//str_stk2在vector上实现，初始化时保存svec的拷贝

```


