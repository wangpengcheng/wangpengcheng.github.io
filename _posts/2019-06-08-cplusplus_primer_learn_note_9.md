---
layout:     post
title:      C++ Primer 学习笔记(九)
subtitle:   C++ Primer 学习记录(九)
date:       2019-06-08
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - C++
    - 基础编程
---
> 2019-06-08 18:25:56

# 第IV部分高级主题

## 第17 章标准库特殊设施
标准库设施是:`tuple` 、`bittest`、随机生成数及正则表达式。

### 17.1 tuple 类型
tuple类似于pair是将一些数据组合成单一的对象。可以将其看做一个“快速随意”的数据结构

**tuple支持的操作**

|操作|含义|
|:---|:---|
|`tuple<T1,T2,...,Tn> t;`|t是一个tuple，成员数为n，第i个成员的类型为Ti。所有成员都进行值初始化|
|`tuple<T1,T2,...,Tn> t(v1,v2,...,vn);`|t是一个tuple，成员数为n，第i个成员的类型为Ti。所有成员都使用vi进行值初始化|
|`make_tuple(v1,v2,...,vn)`|返回一个给定初始值初始化的tuple。tuple的类型从初始值的类型推断|
|`t1=t2`|当两个tuple具有相同数量的成员且成员对应相等时|
|`t1!=t2`|当两个tuple具有相同数量的成员且成员对应相等时之外的情况|
|`t1 relop t2`|tuple的关系运算符使用字典序。两个tuple必须具有相同数量的成员。使用<运算符比较t1的成员和t2中的对应成员|
|`get<i>(t)`|返回t的第i个数据成员的引用；如果t是一个左值，结果是一个左值引用；否则，结果是一个右值引用。tuple的所有成员都是public的|
|`tuple_size<tupleType>::value`|一个类模板，可以通过一个tuple类型来初始化。它有一个名为value的public constecpr static 数据成员，类型为size_t,表示给定tuple类型中成员的数量。|
|`tuple_element<i,tupleType>::type`|一个类模板，可以通过一个整形常量和一个tuple类型来初始化。它有一个名为`type`的`public`成员，表示给定`tuple`类型中指定成员的类型。|

使用示例：

```c++
//定义和初始化

tuple<size_t,size_t,size_t> threeD;  //成员都初始化为0

tuple<string,vector<double>,int,list<int> > someVal("constants",{3.14,2.718},42,{0,1,2,3,4,5});

tuple<size_t,size_t,size_t> threeD2{1,2,3};

auto item=make_tuple("0-999-78345-X",x,20.00); 

//成员的访问

auto book=get<0>(item);  //返回第一个成员

//细节信息访问

typedef decltype(item) trans;  // trans 是item的类型

//返回trans类型对象中成员的数量

size_t sz=tuple_size<trans>::value; //返回3

//获取第二个成员的类型

tuple_element<1,trans>::type cnt=get<1>(item);  // cnt 是一个int

```

**tuple常见用法是，函数返回多个值**

### 17.2 bitset类型
标准库定义了`bitset`类，使得位运算的使用更为容易，并且能够处理超过最长整形类型大小的位集合。`bitset`类定义在头文件`bitset`中。`bitset`类是一个模板，它类似`array`类，具有固定的大小。可以在定义时声明：

```c++
bit<32> bitvec(1U);  // 32位；低位为1，其他位为0
```
**大小必须是一个常量表达式**

**初始化bitset方法**

|操作|含义|
|:---|:---|
|`bitset<n> b;`|b有n位；每位均为0.此构造函数是一个`constexpr`|
|`bitset<n> b(u);`|b是`unsigned long long`值u的低n位的拷贝。如果n大于`unsigned long long `的大小，则b中超出的高位被置为0.此构造函数是一个`constexpr`|
|`bitset<n> b(s,pos,m,zero,one);`|b是`string s`从位置`pos`开始m个字符符拷贝。s只能包含字符`zero`或`one`;如果s包含任何其它字符，构造函数会抛出`invalid_argument`异常。字符在b中分别保存为`zero`和`one`。pos默认为0，m默认为string::npos,zero 默认为`0`,`one`默认为`1`|
|`bitset<n> b(cp,pos,m,zero,one);`|与上一个构造函数相同，但从`cp`指向的字符串数组中拷贝字符，如果m未提供，则cp必须指向一个c风格字符串，如果提供了m,则从cp开始必须至少有m个zero或one字符|

使用示例:

```c++
// bitvec1初始值小；初始值中的高位被丢弃

bitset<13> bitvec1(0xbeef);  // 二进制位序列为 1111011101111

//比初始值大；高位被置为0

bitset<20> bitvec2(0xbeef); // 二进制位序列为 0000101111011101111

//64 位机中， long long 0ULL 是64个0比特，因此～0ULL是64个1

bitset<128> bitvec3(~0ULL); // 0~63 位为1; 63~127位为0

//string 初始化 bitset

bitset<32> bitvec4("1100"); //2、3 两位为1，剩余两位为0；高位被置为0。

```

注意：

string 的下标编号习惯与bitset恰好相反；string中下标最大的字符(最右字符)用来初始化`bitset`中的低位(下标为0的二进制位)。

```c++
string str("1111111000000011001101");

bitset<32> bitvec5(str,5,4); // 从str[5]开始的四个二进制位，1100

bitset<32> bitvec6(str,str.size()-4); //使用最后四个字符
```

![初始化流程](wangpengcheng.github.io/img/2019-06-09-bitset-string.png)

#### 17.2.2 bitset操作

**bitset操作表**

|操作|含义|
|:---|:---|
|`b.any()`|b中是否存在置位的二进制位|
|`b.all()`|b中所有位都置位了吗|
|`b.none()`|b中不存在位置的二进制位吗|
|`b.count()`|b中置位的位数|
|`b.size()`|一个`constexpr`函数，返回b中的位数|
|`b.test(pos)`|若pos位是置位的，则返回true,否则返回false|
|`b.set(pos,v)`|将位置pos处的位设置为bool值v。v默认为true。如果未传递实参，将b中所有位置复位|
|`b.set()`||
|`b.reset(pos)`|将位置pos处的位复位或将b中所有位复位|
|`b.reset()`||
|`b.flip(pos)`|改变位置`pos`处的位的状态或改变b中每一位的状态|
|`b.flip()`||
|b[pos]|访问b中位置pos处的位，如果b是const的，则当该位置位时b[pos]返回一个bool值true,否则返回`fasle`|
|`b.to_ulong()/b.to_ullong()`|返回一个unsigned long 或者一个unsigned long long值，其位模式与b相同。如果b中位模式不能放入指定的结果类型，则抛出一个`overflow_error`异常|
|`b.to_string(zero,one)`|返回一个string,表示b中的位模式。zero和one的默认值分别为0和1，用来表示b中的0和1|
|`os<<b`|将b中二进制位打印为字符1或者0，打印到流`os`|
|`is>>b`|从is读取字符存入b。当下一个字符不是1或者0时，或是已经读入b.size()个位时，读取过程停止|

**提取bitset的值**

```c++
unsigned long ulong=bitvec3.to_ulong();

cout<<"ulong ="<<ulong<<endl;

```
注意：

如果bitset中的值不能放入给定类型中，则这两个操作会抛出一个`overflow_error异常`

**bitset的IO运算符**

```c++
bitset<16> bits;
cin>> bits;  // 从cin读取最多16个0或1

cout<<"bits: "<<bits<<endl; //打印刚刚读取的内容

```
#### 17.3 正则表达式 

_参考链接：_ [regex](http://www.cplusplus.com/reference/regex/);

正则表达式是一种描述字符序列的方法，是一种极其强大的计算工具。本章重点介绍正则表达式库(RE)

**正则表达式库组件**

|组件|含义|
|:---|:---|
|`regex`|表示有一个正则表达式的类|
|`regex_match`|将一个字符序列与一个正则表达式匹配|
|`regex_search`|寻找第一个与正则表达式匹配的子序列|
|`regex_replace`|使用给定格式替换一个正则表达式|
|`sregex_iterator`|迭代适配器，调用`regex_sreach`来遍历一个`string`中所有匹配的子串|
|`smatch`|容器类，保存在`string`中搜索的结果|
|`ssub_match`|`string`中匹配的子表达式的结果|

**regex_search和regex_match的参数**

|参数|解释|
|:---|:---|
|`(seq,m,r,mft)`|在字符序列`seq`中查找`regex`对象r中的正则表达式。seq可以是一个string、表示范围的一对迭代器以及一个指向空字符皆为的字符数组的指针|
|`(seq,r,mft)`|m是一个match对应，用来保存匹配结果的相关细节，m和seq必须具有兼容的类型|
||mft是一个可选的`regex_constants::match_flag_type`值。他们会影响匹配过程|

正则表达式简单使用示例

```c++
//查找不在字符c之后的字符串ei

string pattern("[^c]ei");
//包含pattern的整个单词

pattern="[[:alpha:]]*"+pattern+"[[:alpha:]]*";
regex r(pattern); //构造一个用于查找模式的regex

smatch result;  //定义一个对象保存搜索结果

//定义一个string保存于模式匹配和不匹配的文本

string test_str="receipt freind theif receive";

//查找匹配的字符串

if(regex_search(test_str,result,r)){
    std::cout<<result.str()<<endl; //打印匹配的单词

}
```
**regex(和wregex)选项**

|选项|含义|
|:---|:---|
|`regex r(re)`|re表示一个正则表达式，它可以是一个`string`、一个表示字符范围的迭代器对、一个指向空字符结尾的字符数组的指针、一个字符指针和一个计数器或是一个花括符包围的字符列表。|
|`regex r(re,f)`|f是指出对象如何处理的标志。通过下面列出的值来设置。如果未指定f，其摩恩值为`ECMAScript`|
|`r1=re`|将r1中的正则表达式替换为`re`。`re`表示一个正则表达式，它可以是另外一个`regex`对象、一个`string`、一个指向空字符结尾的字符串数组的指针或是一个花括号保卫的字符串列表|
|`r1.assign(re,f)`|与使用赋值运算符(=)效果相同；可选的标志`f`也与`regex`的构造函数中对应的参数含义相同|
|`r.mark_count()`|r中子表达式的数目|
|`r.flags`|返回`r`的标志|

**定义regex时指定的标志**
定义则regex和regex_constants::syntax_option_type中

|选项|含义|
|:---|:---|
|`icase`|在匹配过程中忽略大小写|
|`nosubs`|不保存匹配的子表达式|
|`optimize`|执行速度优先于构造速度|
|`ECMAScript`|使用`ECMA-262`指定的语法|
|`basic`|使用POSIX基本的正则表达式语法|
|`extended`|使用POSIX扩展的正则表达式语法|
|`awk`|使用POSIX版本的awk语言的语法|
|`grep`|使用POSIX版本的grep的语法|
|`egrep`|使用POSIX版本的egrep的语法|

std::regex_match使用示例

```c++
// regex_match example

#include <iostream>
#include <string>
#include <regex>

int main ()
{


    if (std::regex_match ("subject", std::regex("(sub)(.*)") ))
        std::cout << "string literal matched\n";

    const char cstr[] = "subject";
    std::string s ("subject");
    std::regex e ("(sub)(.*)");

    if (std::regex_match (s,e))
        std::cout << "string object matched\n";

    if ( std::regex_match ( s.begin(), s.end(), e ) )
        std::cout << "range matched\n";

    std::cmatch cm;    // same as std::match_results<const char*> cm;
    
    std::regex_match (cstr,cm,e);
    std::cout << "string literal with " << cm.size() << " matches\n";

    std::smatch sm;    // same as std::match_results<string::const_iterator> sm;
    std::regex_match (s,sm,e);
    std::cout << "string object with " << sm.size() << " matches\n";

    std::regex_match ( s.cbegin(), s.cend(), sm, e);
    std::cout << "range with " << sm.size() << " matches\n";
    // using explicit flags:

    std::regex_match ( cstr, cm, e, std::regex_constants::match_default );

    std::cout << "the matches were: ";
    for (unsigned i=0; i<cm.size(); ++i) {
        std::cout << "[" << cm[i] << "] ";
    }

    std::cout << std::endl;

    return 0;
}

/*

result:

string literal matched
string object matched
range matched
string literal with 3 matches
string object with 3 matches
range with 3 matches
the matches were: [subject] [sub] [ject]


 */ 

```

注意：
一个正则表达式的语法是否正确是在运行时解析的。可以使用`regex_error`来抛出异常。

**异常类型表**

|异常类型|含义|
|:---|:---|
|`error_collate` |  The expression contained an invalid collating element name.|
|`error_ctype`| The expression contained an invalid character class name.|
|`error_escape` |   The expression contained an invalid escaped character, or a trailing escape.|
|`error_backref`|The expression contained an invalid back reference.|
|`error_brack`| The expression contained mismatched brackets ([ and ]).|
|`error_paren`| The expression contained mismatched parentheses (( and )).|
|`error_brace`| The expression contained mismatched braces ({ and }).|
|`error_badbrace`| The expression contained an invalid range between braces ({ and }).|
|`error_range`| The expression contained an invalid character range.|
|`error_space`| There was insufficient memory to convert the expression into a finite state machine.|
|`error_badrepeat`| The expression contained a repeat specifier (one of *?+{) that was not preceded by a valid regular expression.|
|`error_complexity` |The complexity of an attempted match against a regular expression exceeded a pre-set level.|
|`error_stack`| There was insufficient memory to determine whether the regular expression could match the specified character sequence.|

注意： 正则表达式的编译是一个非常慢的操作，特别是你在使用了扩展的正则表达式语法或者复杂的正则表达式的时候。应该尽量避免使用。

**正则表达式库类**

|如果输入序列类型|则使用正则表达式类|
|:---|:---|
|`string `|`regex、smatch、ssub_match和sregex_iterator`|
|`const char* `|`regex、smatch、ssub_match和cregex_iterator`|
|`wstring `|`wregex、wsmatch、wssub_match和wsregex_iterator`|
|`const wchar_t*`|`wregex、wsmatch、wcsub_match和wcregex_iterator`|

ECMAScript正则表达式语句基本特性

- `\{d}`表示单个数字而`\{d}{n}`则表示一个`n`个数字的序列。如，`\{d}{3}`匹配三个数组的序列。
- 在方括号中的字符集和表示匹配这些字符中任意一个。如`[-. ]`匹配一个`-`或`.`或` `。
- 后接'?'的组件是可以选的。如，`\{d}{3}[-. ]?\{d}{4}`匹配这样的序列：开始是三个数字，后接一个可选的短横线或点或空格，然后是四个数字。
- 使用反斜线`\`表示一个字符本身而不是其特殊含义。因此必须使用`\(`和`\)`来表示括号是我们模式的一部分而不是特殊字符。
- 由于反斜线是c++中的特殊字符，在模式中，每次出现`\`的地方，我们都必须使用一个额外的反斜线来告诉c++我们需要一个反斜线字符而不是特殊符号。例如`\\{d}{3}`来表示正则表达式`\{d}{3}`

一个正则表达式的字符串分析：
```c++
"(\\()?(\\d{3})(\\))?([-. ])?(\\d{3})([-. ]?)(\\d{4})";
```
整个正则表达式包含一个字表达式： (ddd)分隔符ddd分隔符dddd。剥离子表达式：

1. `(\\()?`表示区号可选的左括号
2. `(\\d{3})`表示区号
3. `(\\))?`表示区号部分可选的右括号
4. `([-. ])?`表示区号部分可选的分隔符
5. `(\\d{3})`表示号码的下三位数字
6. `[-. ]?`表示可选的分隔符
7. `(\\d{4})`表示号码的最后四位数字

使用示例：

```c++
string phone="(\\()?(\\d{3})(\\))?([-. ])?(\\d{3})([-. ]?)(\\d{4})";

regex r(phone); // regex对象，用于查找我们的模式

smatch m;
string s;

//从文件中读取每条记录
while (getline(cin,s)){
    //对每个匹配的电话号码
    
    for(sregex_iterator it(s.begin(),s.end(),r),end_it;it!=end_it;++it){
        //检查号码的格式是否合法
        
        if(valid(*it)){
            std::cout<<"valid:"<<it->str()<<endl;
        }else{
            cout<<"not valid:"<< it->str()<<endl;
        }
    }

}

```
##### regex_replace
_参考链接：_ [regex_replace](http://www.cplusplus.com/reference/regex/regex_replace/);

**正则表达式替换操作**

```c++
// regex_replace example
#include <iostream>
#include <string>
#include <regex>
#include <iterator>

int main ()
{
  std::string s ("there is a subsequence in the string\n");
  std::regex e ("\\b(sub)([^ ]*)");   // matches words beginning by "sub"
  // using string/c-string (3) version:

  std::cout << std::regex_replace (s,e,"sub-$2");
  // using range/c-string (6) version:
 
  std::string result;
  std::regex_replace (std::back_inserter(result), s.begin(), s.end(), e, "$2");
  std::cout << result;
  // with flags:
  
  std::cout << std::regex_replace (s,e,"$1 and $2",std::regex_constants::format_no_copy);
  std::cout << std::endl;

  return 0;
}
/*

result :
there is a sub-sequence in the string
there is a sequence in the string
sub and sequence

 */

```

**正则表达式匹配选项**

|flag* | effects| notes|
|:---|:---|:---|
|`match_default` |  Default| Default matching behavior. This constant has a value of zero**.|
|`match_not_bol`  | Not Beginning-Of-Line  | The first character is not considered a beginning of line ("^" does not match).|
|`match_not_eol`  | Not End-Of-Line| The last character is not considered an end of line ("$" does not match).|
|`match_not_bow`|   Not Beginning-Of-Word |  The escape sequence "\b" does not match as a beginning-of-word.|
|`match_not_eow` |  Not End-Of-Word| The escape sequence "\b" does not match as an end-of-word.|
|`match_any` |  Any match  | Any match is acceptable if more than one match is possible.|
|`match_not_null `| Not null  |  Empty sequences do not match.|
|`match_continuous` | Continuous | The expression must match a sub-sequence that begins at the first character.Sub-sequences must begin at the first character to match.|
|`match_prev_avail` |   Previous Available | One or more characters exist before the first one. (match_not_bol and match_not_bow are ignored)|
|`format_default` | Default formatting | Uses the standard formatting rules to replace matches (those used by ECMAScript's replace method). This constant has a value of zero**.|
|`format_sed`|sed formatting | Uses the same rules as the sed utility in POSIX to replace matches.|
|`format_no_copy`| No copy| The sections in the target sequence that do not match the regular expression are not copied when replacing matches.|
|`format_first_only`|  First only | Only the first occurrence of a regular expression is replaced.|

```c++

string fmt="$2.$5.$7"; //将号码格式改为 ddd.ddd.dddd

regex r(phone);  //用来寻找模式的regex对象

string number="(908) 555-1800";
cout<<regex_replace(number,r,fmt)<<endl;

```

### 17.4 随机数

_参考链接：_ [random](http://www.cplusplus.com/reference/random/);

c++中的随机数标准库定义在`random`头文件中。包含 **随机数引擎类(random-number engines)**和 **随机数分布类(random-number distribution)**。一个引擎类可以生成`unsigined`随机数序列，一个分布类使用一个引擎类生成指定类型的、在戈丁范围内的、服从特定概率分布的随机数。

注意：
c++程序不应该使用库函数`rand`,而应该使用`default_random_engine`类和恰当的分布类对象。

##### 17.4.1 随机数引擎和分布

注意:
- 使用随机数引擎应该使用`static`关键字修饰。从而每次都生成新的数，否则产生的两次数可能相同。
- 一个给定的随机数发生器一直会生成相同的随机数序列，一个函数如果定义了局部的随机数发生器，应该将其（包括引擎和分布对象）定义为static的。否则，每次调用函数都会生成相同的序列。

```c++
vector<unsigned> good_randVec()
{
    //定义为static的，从而每次调用都生成新的数
    
    static default_random_engine e;
    static uniform_int_distribution<unsigned> u(0,9);
    vector<unsigned> ret;
    for(size_t i=0;i<100;++i){
        ret.push_back(u(e));
    }
    return ret;
}
```
**随机数引擎基本操作**

|操作|含义|
|:---|:---|
|`Engine e`|默认构造函数；使用该引擎类型默认的种子|
|`Engine e(s)`|使用整型值s作为种子|
|`e.seed(s)`|使用种子s重置引擎的状态|
|`e.min()`|此引擎可生成的最大和最小值|

**设置随机数种子** 
通过设置随机数种子引擎可以产生不同的随机数结果。

```c++
default_random_engine e1; //使用默认的种子

default_random_engine e2(2147483646); //使用给定的种子值

default_random_engine e3;  
e3.seed(32767);
default_random_engine e4(32767);  //e3和e4种子相同将会产生相同的随机数。

default_random_engine e5(time(0));  //稍微随机些的种子

```

注意： 

- time生成的随机种子，作为一个自动的过程反复运行，将time的返回值作为种子的方式就无效了；它可能多次使用的都是相同的种子。
- 引擎必须在循环外，否则可能每步循环都产生相同的随机值。

可以使用`uniform_real_distribution`来实现函数的映射。使用示例如下：

```c++
default_random_engine e; //生成无符号的随机数

uniform_real_distribution<double> u(0,1); // 0到1(包含)的均匀分布

for(size_t i=0;i<10;++i){
    cout<<u(e)<<"";
}
```

**非均匀分布的随机数**

|函数方法|说明|
|:---|:---|
|`normal_distribution(a,b)`|标准随机分布，a为均值，b为标准差|
|`bernoulli_distribution(m)`|布尔分布,不接受任何额外的参数时，都是50/50的机会，当m为0.55时则双方机会不均等|

### 17.5 IO库再探

当操作符改变流的格式状态的时候，通常改变后的状态对所有后续IO都生效。

对于bool值的输出需要添加关键字`boolalpha`；如：

```c++
cout<<"default bool values: "<<true<<" "<<false
    <<"\nalpha bool values: "<<boolalpha
    <<true<<" "<< false<<endl;
/*

result:
default bool values: 1 0
alpha bool value: true false


*/
```
**指定整型的进制**

```c++


cout<<showbase;  //打印的时候显示进制

cout<<"default: "<<20<<" "<<1024<<endl;
cout<<"octal: "<<oct<<20<<" "<<1024<<endl;
cout<<"hex: "<<hex<<20<<" "<<1024<<endl;
cout<<"decimal: "<<dec<<20<<" "<<1024<<endl;

cout<<noshowbase; //恢复流状态
/*

default: 20 1024
octal: 024 2000
hex: 0x14 0x400
decimal: 20 1024

 */
```

**指定打印精度**

可以通过cout的`precision()`函数和`setprecision()`函数设置显示的精度。

**输入输出流中的格式控制符**

|控制符| 说明 | 示例|
|:---|:---|:---|
|`boolalpha`  | 设置 bool 类型在数据流中显示为 true 或 false。默认情况下，bool 类型变量显示为1或0。|  `cout << boolalpha` ; 或 `boolalpha (cout)`;|
|`noboolalpha` |设置 bool 类型在数据流中显示为 1 或 0 （恢复 bool 类型的显示状态至默认）。 | cout << noboolalpha ; 或 hex(noboolalpha );|
|`scientific` | 设置浮点数以科学计数法显示。 | cout << scientific ; 或 hex(scientific );|
|`fixed` |  设置浮点数以标准显示法显示。|  cout << fixed ; 或 hex(fixed );|
|`hex` |设置整数类型以十六进制显示。  |cout << hex; 或 hex(cout);|
|`dec` |设置整数类型以十进制显示（默认情况下，整数类型是以十进制显示）。 |   cout << dec; 或 dec(cout);|
|`cot` |设置整数类型以八进制显示   | cout << oct; 或 oct(cout);|
|`internal`  |  数值的符号位在域宽内左对齐，数值右对齐，中间由填充字符填充。 | cout << internal ; 或 oct(internal );|
|`left`| 设置输出数值或文本的显示形式为左对齐。| cout << left ; 或 oct(left );|
|`right`  | 设置输出数值或文本的显示形式为右对齐。 |cout << right ; 或 oct(right );|
|`showbase`  |  设置数值前缀不显示  | cout << showbase ; 或 oct(showbase );|
|`noshowbase` | 将数值显示的形式恢复至默认状态。 |   cout << noshowbase ; 或 oct(noshowbase );|
|`showpoint` |  即使小数部分为零，也显示浮点数的整数部分和小数点右侧的数字 |  cout << showpoint ; 或 oct(showpoint );|
|`noshowpoint`| 仅显示数值的整数部分。| cout << noshowpoint ; 或 oct(noshowpoint );|
|`showpos` |显示正数值的 + 号。| cout << showpos ; 或 oct(showpos );|
|`noshowpos`  | 忽略正数值前面的 + 号。 |  cout << noshowpos ; 或 oct(noshowpos );|
|`skipws`  |使 cin 获取输入时忽略空格。   | cout << skipws ; 或 oct(skipws );|
|`noskipws`    |使输入流读取空格  |  cout << noskipws ; 或 oct(noskipws );|
|`unitbuf`| 每次执行输出操作后均会清空缓冲区。 |  cout << unitbuf ; 或 oct(unitbuf );|
|`nounitbuf`  | 将 unitbuf 复位至默认状态。 | cout << nounitbuf ; 或 oct(nounitbuf );|
|`uppercase`  | 设置十六进制数值和科学计数法中的指数以大写形式显示。 | cout << uppercase ; 或 oct(uppercase );|
|`nouppercase` |设置十六进制数字和科学计数法的指数以小写形式显示   | cout << nouppercase ; 或 oct(nouppercase );|
|`flush`| 刷新缓冲区  | cout << “str” << flush; 或 flush(cout);|
|`endl`| 刷新缓冲区并插入一个换行符 |  cout << “str” << endl; 或 endl(cout);|



**定义在iomanip中的操作符**

| 操作符 | 含义 |
|:------ |:----|
|`setfill(ch)`|用`ch`填充空白|
|`setprecision(n)`|将浮点精度设置为n|
|`setw(w)`|读或写值的宽度为w个字符|
|`setbase(b)`|将整数输出为b进制|

##### 17.5.2 未格式化的输入/输出操作

标准库提供了一组低层操作，支持未格式化IO.这些操作允许我们将一个流当做一个无解释的字节序列来处理。

```c++
char ch;
while(cin.get(ch)) {
    cout.put(ch);
}
```

**单字节低层IO操作**

|操作|含义|
|:---|:---|
|`is.get(ch)`|从`istream is`读取下一个字节存入字符`ch`中。返回`is`|
|`os.put(ch)`|将字符`ch`输出到`ostream os`。返回`is`|
|`is.get()`|将`is`的下一个字节作为`int`返回|
|`is.putback(ch)`|将字符`ch`放回`is`。返回`is`|
|`is.unget()`|将`is`向后移动一个字节。返回`is`|
|`is.peek()`|将下一个字节作为`int`返回,但不从流中删除它|


**将字符放回输入流**

标准库提供了三种方法退回字符：

- `peek`: 返回输入流中一个字符的副本，但不会将它从流中删除，`peek`返回的值仍然留在流中。
- `unget`:输入流向后移动，从而最后读取的值又回到流中。即使我们不知道最后从流中读取什么值，仍然可以调用`unget`。
- `putpack`: 退回从流汇总读取的最后一个值，但它接受一个参数，此参数必须与最后读取的值相同。

这些函数返回整型的主要原因是可以返回文件尾标记符。
```c++
//检测是否达到文件尾部

int ch; //使用一个int,而不是一个char来保存get()的返回值

//循环读取并输出输入中的所有数据

while((ch==cin.get())!=EOF){
    cout.put(ch);
}
```

**多字节操作**

多字节操作要求我们自己分配管理用来保存和提取数据的字符组操作

**多字节低层IO操作**
|操作|解释|
|:---:|:---|
|`is.get(sink,size,delim)`|从`is`中读取最多`size`个字节，并保存在字符数组中，字符数组的其实地址由`sink`给出。读取过程直至遇到字符`delim`或读取了`size`个字节或遇到文件末尾时停止。如果遇到了`delim`,则将其留在输入流中，不读取出来存入`sink`|
|`is.getline(sink,size,delim)`|与接受三个参数的get版本类似，但会读取并丢弃`delim`|
|`is.read(sink,size)`|读取最多`size`个字节，存入字符数组`sink`中。返回`is`|
|`is.gcount()`|返回上一个未格式化读取操作从is读取的字节数|
|`os.write(source,size)`|将字符数组`source`中的`size`个字节写入`os`。返回`os`|
|`is.ignore(size,delim)`|读取并忽略最多`size`个字节，包括`delim`。与其他未格式化函数不同，`ignore`有默认参数:size的默认值为1，`delim`的默认值为文件尾|

注意： 
- 一个常见的错误是本想从流中删除分隔符，但却忘了做。
- get和getline读取字符时，get将分隔符留作istream中的下一个字符，而getline则读取并丢弃分隔符。无论哪个函数都不会将分隔符保存在`sink`中

##### 17.5.3 流随机访问

**随机IO本质上是依赖于系统的。为了理解如何使用这些特性，必须查询系统相关文档**

**seek和tell函数**
一个函数通过将标记seek到一个给定位置来重定位它；另外一个函数tell我们标记的当前位置。后缀`g`表示正在"获得"，后缀`p`表示正在放入

|函数|定义|
|:---|:---|
|`tellg()/tellp()`|返回一个输入流中(tellg)或输出流中(tellp)标记的当前位置|
|`seekg(pos)/seekp(pos)`|根据标定的`pos`值，重定位到输入/输出流中的绝对地址|
|`seekp(off,from)/seekg(off,from)`|在一个输入流或输出流中将标记定位到from之前或之后`off`个字符，`from`可以是下列值之一：`beg`,偏移量相对于流开始位置，`cur`,偏移量相对于流当前结束位置；`end`,偏移量相对于流结尾位置 |

注意：由于只有单一的标记，因此只要我们在读写操作间切换，就必须进行`seek`操作来重定位标记。

使用示例：

```c++
#include <iostream>
#include <fstream>
using namespace std;

int main(){
    //以读写方式打开文档
    
    fstream inOut("copyout",fstream::ate|fstream::in|fstream::out);

    if(!inOut){
        cerr<<"Unable to open file"<<endl;
        return 0;
    }
    auto end_mark=inOut.tellg(); //记住原文件尾位置

    inOut.seekg(0,fstream::beg); //重定位带文件开始

    size_t cnt=0; //字节数累加器

    string line;  //保存输入中的每行

    // 还未遇到错误且未到末尾，持续读取
    while(inOut&& inOut.tellg()!=end_mark
        &&getline(inOut,line))
    {
        cnt+=line.size()+1; //+1表示换行

        auto mark=inOut.tellg(); //记住读取位置

        inOut.seekp(0,fstream::end); // 将写标记移动到问价尾

        inOut<<cnt;   //输出累计长度

        if(mark!=end_mark){
            inOut<<" ";
        }
        inOut.seekp(0,fstream::end); //定位到文件尾
    }

    inOut<<"\n";  //文件尾部输出换行符
    return 0;

}
```


