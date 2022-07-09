---
layout:     post
title:      C++ Primer 学习笔记(六)
subtitle:   C++ Primer 学习记录(六)
date:       2019-05-12
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - C++
    - 基础编程
---

>>2019-5-12 :19:44:52

## 泛型算法

泛型算法是指使用迭代器为媒介的通用型算法；泛型算法本身不会执行容器的操作，他们只会运行于迭代器智商，执行迭代器的操作--算法永远不会改变底层容器的大小。算法可能改变容器中保存的元素值。

**`accumulate` 求和**

```c++

int sum =accumulate(vec.cbegin(),vec.cend(),0);//对元素求和初值是0

string sum=accumulate(v.cbegin(),v.cend(),string(''));//字符串求和

equal(roster1.cbegin(),roster1.cend(),rosterl2.cbegin());//比较元素，数量相同且一样多的时候才返回`true`

fill(vec.begin(),vec.begin()+vec.size()/2,10);//将容器的一个子序列设置为10

back_inserter();//插入元素，检查是否存在

copy(begin(a1),end(a1),a2);//把a1的内容拷贝给a2

replace(ilist.begin(),ilist.end(),0,42);//查找所有元素为0的值，并将它更改为42

replace_copy(ilis.cbegin(),ilist.cend(),back_inserter(ivec),0,42);//ilist指出调整序列之后的保存位置。

sort(word.begin(),word.end());//按照字典排序

auto end_unique=unique(words.begin(),words.end());//除去重复单词

``` 

注意：
 
- 算法不检查写操作
- 向目的位置迭代器写入数据的算法，假定目的位置足够大，能容纳要写入的元素。

### 定制操作
许多算法的默认关键操作符，并不能满足我们的需求，因此，需要我们来重载`sort`的默认行为。

#### 向算法传递函数
sort可以接受第三个参数，即谓词--一个可用的表达式，其返回结果是一个能用作条件的值。标准库中分为
**一元谓词**(只接受单一参数)和**二元谓词**(他们有两个参数)。例如下面的代码，重载了sort函数的基础比较操作

```c++
//排序函数

bool isShorter(const string &s1,const string &s2){
    return s1.size()<s2.size();
}
//按照长度由短到长排序
sort(words.begin(),words.end(),isShorter);

elimDups(words);//将words按照字典重新排序，并消除重复单词

stable_sort(words.begin(),words.end(),isShorter)；//按照长度重新排序，长度相同的单词维持字典序

//无需拷贝字符串

for (const auto &s :words)
{
    cout<<s<<"";//打印每个元素，以空格分割

}
cout<<endl;

```
### lambda 表达式

_参考链接：_ [c++11 lambda 表达式](https://www.cnblogs.com/DswCnblog/p/5629165.html);[c++ lambda表达式](https://www.cnblogs.com/jimodetiantang/p/9016826.html);

**lambda表达式的声明**

`[capture list] (params list) mutable exception-> return type { function body }`
各项含义如下：

1. `capture list`: 捕获外部变量列表
2. `params list`: 形参列表
3. `mutable`:用来说明是否可以修改捕获的变量
4. `exception`:异常设定
5. `return type`:返回类型
6. `function body`:函数体

省略的常见表达式有：

|序号|格式|
|:---|:---|
|1|`[capture list] (params list) -> return type {function body}`|
|2|`[capture list] (params list) {function body}`|
|3|`[capture list] {function body}`|

其中：

- 格式1声明了const类型的表达式，这种类型的表达式不能修改捕获列表中的值。
- 格式2省略了返回值类型，但编译器可以根据以下规则推断出Lambda表达式的返回类型： （1）：如果function body中存在return语句，则该Lambda表达式的返回类型由return语句的返回类型确定； （2）：如果function body中没有return语句，则返回值为void类型。
- 格式3中省略了参数列表，类似普通函数中的无参函数。

```c++
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

bool cmp(int a, int b)
{
    return  a < b;
}

int main()
{
    vector<int> myvec{ 3, 2, 5, 7, 3, 2 };
    vector<int> lbvec(myvec);

    sort(myvec.begin(), myvec.end(), cmp); // 旧式做法

    cout << "predicate function:" << endl;
    for (int it : myvec)
        cout << it << ' ';
    cout << endl;

    sort(lbvec.begin(), lbvec.end(), [](int a, int b) -> bool { return a < b; });   // Lambda表达式
    cout << "lambda expression:" << endl;
    for (int it : lbvec)
        cout << it << ' ';
}

```
lambde表达式的外部捕获

Lambda表达式通过在最前面的方括号[]来明确指明其内部可以访问的外部变量，这一过程也称过Lambda表达式“捕获”了外部变量。

值捕获方式：

|捕获形式  |  说明|
|:---|:---|
|`[]`| 不捕获任何外部变量|
|`[变量名, …]`    |默认以值得形式捕获指定的多个外部变量（用逗号分隔），如果引用捕获，需要显示声明（使用&说明符）|
|`[this]` | 以值的形式捕获this指针|
|`[=]`| 以值的形式捕获所有外部变量|
|`[&]`| 以引用形式捕获所有外部变量|
|`[=, &x]`| 变量x以引用形式捕获，其余变量以传值形式捕获|
|`[&, x] `| 变量x以值的形式捕获，其余变量以引用形式捕获|

注意：如果一个lambda体包含`return`之外的任何语句，则编译器假定lambda返回`void`；被推断`void`的lambda不能返回值。因此当重写排序等操作的时候，只能有一句`return`语句；例如：

```c++
transform(vi.begin(),vi.end(),vi.begin(),
    [](int i){
        return i<0?-i:i;//返回绝对值

    });

transform(vi.begin(),vi.end(),vi.begin(),
    [](int i){
        if(i<0){
            return -i;
        }else{
            return i;
        }
        //错误，有多余语句，判定返回void

    });

```

因此当我们使用一个lambda定义返回类型的时候，必须使用尾置返回类型，声明返回类型；例如：

```c++
transform(vi.begin(),vi.end(),vi.begin(),
    [](int i)->int 
    {
        if(i<0){
            return -i;
        }else{
            return i;
        }
        //返回绝对值

    });
```

#### lambda 参数绑定
lambda表达式，虽然可以进行简单的参数绑定，但是对于复杂的参数绑定而言，还是存在许多问题，因此可以利用函数对lambda表达式进行参数绑定。大致步骤如下：
 
 1. 使用`std:bind`函数对函数参数和函数进行绑定
 2. 使用`placeholderd`进行参数占位符的使用。

调用`bind`一般形式为：
`auto newCallable=bind(callable,arg_list);`

`newCallable`本身是一个可以调用对象，`arg_list`是一个逗号分隔的参数列表，对应给定的`callable`的参数，即调用的实际参数。
`arg_list`中的参数可能包含形如`_n`的名字，其中`n`是一个整数。表示占位符，数值`n`表示生成的可调用对象中参数的位置：`_1`为`newCallble`的第一个参数，`_2`为第二个参数。`_n`为第`n`个参数。
使用：
`auto g=bind(f,a,b,_2,c,_1);`
生成一个新的可调用函数对象，它有两个参数，分别使用占位符`_2`和`_1`表示。使得原本需要输入5个参数的函数变为，只需要2个参数的函数，对函数进行了再一次封装。`f(a,b,_1,c,_2)`等价于`g(_1,_2)`减少了函数的复杂程度。

`std::ref()`：传递给`bind`一个对象但是不拷贝它

例如：

```c++
//1. 定义比较string大小函数

bool check_size(const string &s,string::size_type sz){
    return s.size()>=sz;
}

//2. 使用bind绑定函数和参数
//将输入比较顺序颠倒，将单词长度由长至短排序

sort(words.begin(),words.end(),bind(isShorter,_2,_1));

//使用lambda表达式，输出表达式，os是一个局部变量，引用一个输出流

for_each(word.begin(),words.end(),[&os,c](const string &s){
    os<<s<<c;
});
//bind相同的效果 

ostream &print(ostream &os,const string &s,char c){
    return os<<s<<c;
}
//ref返回一个对象，包含给定的引用，此对象是可以拷贝的

for_each(words.begin(),words.end(),
        bind(print,std::ref(os),_1,' '));


``` 

#### 再探迭代器

##### 插入迭代器
插入迭代器有三种基本类型

- `back_inserter`: 创建一个使用`push_back`的迭代器。
- `front_inserter`:创建一个使用`push_front`的迭代器。 
- `inserter`:创建一个使用`inserter`的迭代器。此函数接受第二个参数，这个参数必须是一个指向容器的迭代器。元素将被插入到给定迭代器所表示的元素制之前。

注意： 只有在容器支持`push_front/push_back`的情况下，我们才可以使用`front_inserter/back_inserter`。

使用示例：

```c++

*it=val;
it=c.insert(it,val);//it指向插入的新元素

++it;//递增it使它指向原来的元素

list<int> lst={1,2,3,4};
list<int> lst2，lst3;//空list
//拷贝完成之后，lst3包含1 2 3 4

copy(lst.cbegin(),lst.cend(),front_inserter(lst2));
//拷贝完成之后，lst3包含1 2 3 4

copy(lst.cbegin(),lst.cend(),inserter(lst3,lst3.begin()));

```

##### iostream 迭代器

- `istream_iterator`:读取输入流。
- `ostream_iterator`:向输出流写数据。

**`istream_iterator`操作** 

|操作|含义|
|:---|:---|
|`istream_iterator<T> in(is)`|`in`从输入流`is`读取类型为`T`的值|
|`istream_iterator<T> end`|读取类型为`T`的值的`istream_iterator`迭代器，表示尾后位置|
|`in1=(!)=in2`|`in1`和`in2`必须读取相同类型。如果他们都是尾后迭代器，或绑定到相同输入，则二者相等|
|`*in`|返回从流中读取的值|
|`in->mem`|与`(*in).mem`的含义相同|
|`++in,in++`|使用元素类型所定义的`>>`运算符从输入流中读取下一个值，并且前置版本返回一个指向递增后迭代器的引用，后置版本返回旧值|

操作示例：

```c++
istream_iterator<int > int_it(cin); //从cin读取int

istream_iterator<int> int_eof; //尾后迭代器

ifstream in("afile");  
istream_iterator<string> str_it(in); //从"afile"读取字符串

//读入数据并存储到vector向量组

while(int_it!=eof) {
    //后置递增运算读取流，返回迭代器的旧值
    //解引用迭代器，获得从流读取的前一个值

    vec.push_back(*in_iter++);
}
//循环读取其中的int值存储在vec中

//使用算法操作流迭代器

istream_iterator<int > in(cin),eof;
cout<<accumulate(in,eof,0)<<endl;
//输入：23 109 45 89 6 34 12 90 34 23 56 23 8 89 23

//输出：664


```
注意：当我们将一个`istream_iterator`绑定到一个流时，标准库并不保证迭代器立即从流中读取数据。直到使用迭代器时才正真读取。标准库中的实现所保证的是，我们第一次解引用迭代器之前，从六中的数据操作已经完成了。

##### `ostream_iterator`操作

|操作|含义|
|:---|:---|
|`ostream_iterator<T> out(os)`|`out`将类型为`T`的值写入到流`os`中|
|`ostream_iterator<T> out(os,d)`|将类型为`T`的值写到输出流`os`中，每个值后面都输出一个`d`。`d`指向一个空字符结尾的字符数组|
|`out=val`|`out`和`val`必须类型兼容。用运算符`<<`将`val`写入到`out`输出流中|
|`*out,++out,out++`|这些运算符是存在的，但是不对`out`做任何事情。每个运算符都返回`out`|

### 泛型算法结构
对于向一个算法传递错误类别的迭代器的问题，很多编译器不会给出任何警告或者提示。因此需要迭代器类型的支持和认证。

**迭代器类别**

|类别|含义|
|:---|:---|
|输入迭代器|只读，不写；单遍扫描，只能递增|
|输出迭代器|只写，不读；单遍扫描，只能递增|
|前向迭代器|可读写；多遍扫描，只能递增|
|双向迭代器|可读写；多遍扫描，可递增递减|
|随机访问迭代器|可读写；多遍扫描，支持全部迭代运算|

## 第11章 关联容器

关联容器支持高效的关键字查找和访问。两个主要的关联容器类型是`map`和`set`；关联容器不支持顺序容器的位置相关的操作。也不支持构造函数或者插入操作这些接受一个元素值和一个数量值的操作。

使用示例：
```c++
std::map<string, size_t;> word_count;//空容器

set<string> exclude={"the","but","and","or","an","a"};

//三个元素；authors将姓映射为名

map<string ,string> authors={
    {"Joyce","James"},
    {"Austen","Jane"},
    {"Dickens","Charles"}
};
```

注意：一个`map`或者`set`中的关键字必须是唯一的。但是`multimap`和`multiset`没有这个限制。允许多个元素，拥有相同的关键字。

#### 11.2.2 关键字类型要求
传递给排序算法的可调用对象，必须满足于关联容器中关键字一样的类型要求。

#### 11.2.3 `pair`类型

`pair`主要保存，两个数据成员，必须提供2个类型名，`pair`的数据成员，将具有应对的类型。

```c++
pair<string ,string > anon; //保存两个string

pair<string ,size_t> word_count; //保存一个string和一个size_t

pair<string ,vector<int>> line; //保存string和vector<int>

```

**`pair`上的操作**

|操作|含义|
|:---|:---|
|`pair <T1,T2> p`|p是一个`pair`，两个类型分别为`T1`和`T2`的成员都进行了初始化|
|`pair <T1,T2> p(v1,v2)`|p是一个`pair`，两个类型分别为`T1`和`T2`的`v1`和`v2`都进行了初始化|
|`pair <T1,T2> p={v1,v2}`|意义同上|
|`make_pair(v1,v2)`|返回一个用`v1`,`v2`初始化的`pair`。`pair`的类型从`v1`和`v2`的类型中推断出来|
|`p.first`|返回`p`的名称为`first`的公有数据成员|
|`p1 relop p2`|关系运算符(<、>、<=、>=)按照字典序定义：例如对应成立时返回`true`|

### 11.3 关联容器操作

|操作类型|含义|
|:---|:---|
|`key_type`|此容器类型的关键字类型|
|`mapped_type`|每个关键字关联的类型；只适用于`map`|
|`value_type`|对于`set`,与`key_type`相同；对于`map`，为`pair<const key_type,mapped_type>`|

```c++
set<string>::value_type v1; //v1是一个string

set<string>::key_type v2; //v2是一个string

map<string,int>::value_type v3;//v3是一个pair <const string ,int>

map<string,int>::key_type v4; //v4是一个string

map<string,int>::mapped_type v5; //v5是一个int

```
注意：

- 一个`map`的`value_type`是一个`pair`,我们可以改变`pair`的值，但不能改变关键字成员的值。
- `set`的迭代器是`const`的，只能读取，不能修改。

#### 11.3.1 关联容器迭代器
使用示例：

```c++
auto map_it=word_count.begin();  //*map_it 是一个指向pair<const string,size_t>对象的引用

cout<<map_it->first; //打印此元素的关键字

cout<<""<<map_it->second; //打印此元素的值

map_it->first="new key"; //错误：关键字是const的

++map_it->second; //正确：我们可以通过迭代器改变元素

```
因为关联容器`set`的迭代元素是`const`的，`map`中的元素是`pair`，其第一个成员是`const`的，因此关联容器，通常用于值读取元素的算法,多为搜索排序。
同时使用`insert`来添加元素

```c++
vector<int> ivec={2,4,6,8,10};//ivec有8个元素

set<int> set2; //空集合

set2.insert(ivec.cbegin(),ivec.cend());//set2有4个元素

set2.insert({1,3,5,7,9});//set2现在有8个元素

```
关联容器操作：
`emplace(args)`:对于`map`和`set`,只有当元素的关键字不在`c`中时才插入(或者构造)元素。函数返回一个`pair`,包含一个迭代器，指向具体有指定关键字的元素，以及一个指示插入是否成功的bool值。

注意插入操作返回的是一个`pair`对象，第一个元素是差诶迭代器的坐标，第二个值是一个`bool`值，确定是否插入成功。

```c++
//统计每个单词在输入中出现的次数

map<string ,size_t> word_count;//从string到size_t的空map

string word;

while(cin>>word){
    //插入一个元素，关键字等于word，值为1；
    //若word已载word_count中，insert什么也不做
    auto ret=word_count.insert({word,1});

    if(!ret.second)  //word已经在word_count中

        ++ret.first->second; //递增计数器

/*

等价形式 ++((ret.first)->second);
ret 保存insert返回的值，是一个pair
ret.first 是pair的第一个成员，是一个map迭代器，指向具有给定关键字的元素
ret.first-> 解引用此迭代器，提取map中的元素，元素也是一个pair
ret.first->second; map中元素值的部分
++ ret.first->second; 递增此值
*/
}
```
#### 11.3.3 删除元素
使用`erase`函数来删除和释放元素;
```c++
auto cnt=authors.erase("Barth,Johns");
```

#### 11.3.4 map的下表操作

|操作|含义|
|:---|:---|
|`c[k]`|返回关键字为`k`的元素；如果`k`不在`c`中，添加一个关键字为`k`的元素，对其进行值初试化|
|`c.at(k)`|访问管家字为`k`的元素|

示例：
```c++
map <string ,size_t> word_count; //empty map

//插入一个关键字为Anna的元素，关联值进行值初试化；然后将1赋予它

word_count["Anna"]=1;

```
注意：

- 对一个`map`使用下表操作，使用一个不在容器中的关键字作为下表，会添加一个具有此关键字的元素到`map`中
- 与`vector`与`string`不同，`map`的下标运算符，返回的类型与解引用`map`迭代器得到的类型不同。
- `map`使用`find`代替下标操作
- `lower_bound`和`upper_bound`不适用于无序容器
- 下标和`at`操作只适用于非`const`的`map`和`unordered_map`

**关联容器中查找元素的操作**

|操作|含义|
|:---|:---|
|`c.find(k)`|返回一个迭代器，指向第一个关键字为`k`的元素，若`k`不在容器中则返回尾后迭代器|
|`c.count(k)`|返回关键字等于`k`的元素的数量。对于不允许重复关键字的容器，返回值永远是0或者1|
|`c.lower_bound(k)`|返回一个迭代器，指向第一个关键字不小于`k`的元素|
|`c.upper_bound(k)`|返回一个迭代器，指向第一个关键字不大于`k`的元素|
|`c.equal_range(k)`|返回一个迭代器`pair`，指向第一个关键字等于`k`的元素|

注意：
 
- 当我们遍历一个`multimap`或者`multiset`时，保证可以得到序列中所有具有给定关键字的元素。
- `lower_bound`返回的迭代器可能指向一个具有给定关键字的元素，但也可能不指向。如果关键字不在容器中，则`lower_bound`会返回关键字的第一个安全插入点--不影响容器中元素顺序的插入位置。
- 如果`lower_bound`和`upper_bound`返回相同的迭代器，则给定关键字不在容器中。

### 11.4 无序容器
无序关联容器总共有4个：`unordered_map`和`unordered_set`;

**桶管理**
无序容器在存储组织上为一组桶，每个桶保存0个或者多个元素。无序容器使用一个哈希函数将元素映射到桶。

**无序容器管理操作**

|操作|含义|
|:---|:---|
|**桶接口**|  |
|`c.bucket_count()`|正在使用的桶的数目|
|`c.max_bucket_count()`|容器能容纳的最多的桶的数量|
|`c.bucket_size(n)`|第n个桶中有多少个元素|
|`c.bucket(k)`|关键字为`k`的元素在那个桶中|
|**桶迭代**||
|`local_iterator`|可以用来访问桶中元素的迭代器类型|
|`const_local_iterator`|桶迭代器的`const`版本|
|`c.begin(n),c.end(n)`|与前两个函数类似，但返回`const_local_iterator`|
|**哈希策略**||
|`c.load_factor()`|每个桶的平均元素数量。返回`float`值|
|`c.max_load_factor()`|c维护桶的大小，返回`float`值。c会在需要时添加新的桶。以使得`load_factor<=max_load_factor`|
|`c.rehash(n)`|重组存储，使得`bucket_count>=n`且`bucket_count>size/max_load_factor`|
|`c.reserve(n)`|重组存储，使得c可以保存`n`个元素且不必`rehash`|

