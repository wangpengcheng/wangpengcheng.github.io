---
layout:     post
title:      Linux程序设计 学习笔记 (一)
subtitle:   Linux程序设计 学习笔记 (一)
date:       2019-08-13
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - C/C++
    - Linux
    - 操作系统
    - 程序设计
---

# Linux 程序设计 阅读笔记

## 参考链接：

- [Linux内核文档首页](https://www.kernel.org/doc/Documentation/)
- [Linux文档](https://linux.die.net/)
- [Linux c 开发手册](https://legacy.gitbook.com/book/wizardforcel/linux-c-api-ref/details)
- [Linux Kernel API](https://www.kernel.org/doc/htmldocs/kernel-api/index.html)
- [书中代码地址](http://www.wrox.com/WileyCDA/WroxTitle/Beginning-Linux-Programming-4th-Edition.productCd-0470147628,descCd-DOWNLOAD.html)


## 第一章 入门

#### 1.1.1  unix
_参考链接：_ [POSIX标准总体分析](https://blog.csdn.net/novagx/article/details/2077561);[POSIX](https://baike.baidu.com/item/POSIX/3792413?fr=aladdin)

unix系统特点

- 简单性：
- 集中性：
- 可重用组件
- 过滤器
- 开放的文件格式
- 灵活性

几乎所有的UNIX编写的程序都可以在Linux上编译运行

GNU项目软件：

- GCC:GNU编译器集，包括GUN c
- G++：c++编译器，是GCC的一部分。
- GDB:源代码级别的调试器
- GNU make:NUIX make 的免费版本
- Bison:与UNIX yacc兼容的语法分析生成器。
- bash:命令与解释器
- GNU Emacs:文本编辑器计环境

linux支持的编程语言：

![linux支持的编程语言](https://wangpengcheng.github.io/img/2019-08-14-19-05-38.png)

普通用户可执行程序搜索路径

- `/bin`：二进制文件目录，用于存放启动系统时用到的程序
- `/usr/bin`：用户二进制文件目录，用于存放启动系统时用到的程序
- `/usr/local/bin`：本地二进制目录，用于存放软件安装的程序
- `/usr/sbin`或`/sbin`：root登录后的PATH变量和程序存在的路径
- `/opt`:第三方应用程序，可能被安装的目录。

注意当操作系统升级的时候，只有目录`/opt`和`/usr/local`里面的内容需要保留，对于系统级应用程序，建议放在`/usr/local`中，个人应用程序放在`home`文件夹中。

#### 1.2.4 开发系统引导
_参考链接：_ [Linux系统目录结构说明](https://blog.csdn.net/mzl87/article/details/79673012);[Linux 系统目录结构](https://www.runoob.com/linux/linux-system-contents.html);[ubuntu获取源码方法](https://blog.csdn.net/Oliverlyn/article/details/55272286)


头文件位置:/usr/include,特定版本依赖，一般都在`/usr/include/sys`或者`/usr/include/linux`中

库文件:一般存储在`/lib`和`/usr/lib`目录中。包括`.a`静态函数库和`.so`动态函数库。

注意：当静态库被多个应用程序使用时，内存中就会存在同一函数的多个副本，将大量消耗内存和磁盘空间。

额外的搜索路径可以在/etc/ld.so.conf中配置。

## 第二章 shell程序设计

Linux中一般工具的开发都是先用shell实现，当追求速度时，再使用c/c++,Perl或者其它语言实现。

![内核应用程序关系](https://wangpengcheng.github.io/img/2019-08-14-19-49-19.png)

shell中*匹配字符串，`?`匹配单个字符，`[]`和`{}`匹配多个字符串。

```shell
more `grep -l POSIX`

#等价于

more ${grep -l POSIX *}
```
脚本中第一行的`#!/bin/bash`告诉脚本它的执行软件。注意绝对路径不要超过32个字符，因为老版本的识别不了。

shell中变量直接声明，使用时使用`$`，赋值时等号两边不能有空格。
shell中"'"只是输出字符串，"""进行变量解析。

**环境变量**

![环境变量表格](https://wangpengcheng.github.io/img/2019-08-14-20-08-07.png)

**参数变量**
当脚本运行时带有参数时，一些额外的变量就会被创建。环境变量`$#`始终存在，只是当无参数传递时为0；

![输入参数变化](https://wangpengcheng.github.io/img/2019-08-14-20-10-42.png)

![$@和$*之间的区别](https://wangpengcheng.github.io/img/2019-08-14-20-13-14.png)

注意：$#是未知变量的个数

下面是简单的测试脚本程序

```shell
#!/bin/bash

#shell file name try_var

salutation="hello"
echo $salutation
echo "The parameter 0 is : $0"
echo "The parameter 1 is : $1"
echo "The parameter 2 is : $2"
echo "The parameter list is : $*"
echo "The parameter list is : $@"
echo "The user's home directory is $HOME "

echo "Please enter a new greeting "
read salutation
echo $salutation
echo "The script is now complete"
exit 0


```
输入： ./try_var.sh 1 2 3
输出：
```
hello
The parameter 0 is : ./try_var.sh
The parameter 1 is : 1
The parameter 2 is : 2
The parameter list is : 1 2 3
The user's home directory is /home/wangpengcheng 
Please enter a new greeting 
123
123
The script is now complete
```

#### 2.6.2 条件

`test`或`[`命令，使用`[`时，通常以`]`符号来结尾。`test`用的较少。例如：

```shell
#!/bin/bash
#检查是否存在文件
if test -f fred.c
then
    ...
fi

#还可以写成下面这样

if [ -f fred.c ]
then
...
fi
```

注意命令使用时后面必须存在空格。

**常见比较表达式**

![常见比较](https://wangpengcheng.github.io/img/2019-08-14-20-29-38.png)

![文件测试](https://wangpengcheng.github.io/img/2019-08-14-20-30-25.png)

#### 2.6.3 控制结构

```shell

if condition
then 
    statements1
elif condition
    statements2
else
    statements3
fi


```
注意对百年来那个字符串进行比较时，最好使用`""`让变量初始化。

可以使用`printf`代替`echo`就可以自定义，换行符。

**for语句**

```shell
for variable in value
do
    statements
done
```

示例：
```shell
#!/bin/bash

for file in $(ls f*.sh);
do
    lpr $file
done
exit 0

```
**while语句**
```shell

while condition do
    statements
done
```

**untill语句**

循环执行，直到满足条件

```shell

until [[ condition ]]; do
    statements
done
```

**case语句**

```shell

case variable in
    pattern [ | pattern ] ...) statements;;
    pattern [ | pattern ] ...) statements;;
    ...
esac
```

注意每个模式都应该以双分号结尾。

使用示例:
```shell
#!/bin/bash

echo "Is it morning? Please answer yes or no"
read timeofday

case "$timeofday" in
    yes | y | Yes | TES )   
        echo "test"
        echo "Good morning";;
    n* | N* )               echo "Good Afternoon";;
    * )                     echo "Sorry,answer not reconized";;
esac
exit 0
```
**列表命令**

- AND 列表：`&&`执行and条件，注意前后有空格
- OR 列表： `|| `直到有一条命令成功运行。

**语句块**

使用`{}`来构造执行语句块。

#### 2.6.4 函数
格式：
```
function_name()
{
    statements
}

```

注意：当一个函数被调用时，脚本程序的位置参数($*、$@、$#、$1、$2等)会被替换为函数的参数。这也是读取传递给函数的参数的办法。当函数执行完毕后，参数会恢复为它们先前的值。

函数操作字符串，一般是提前声明变量，或者使用`local`关键子在shell中声明局部变量，说明变量范围。否则都是按照全局变量处理。
下面是一个简单的示例：

```shell
#!/bin/bash

# file name yes_or_no 

yes_or_no(){
    echo "Is your name $* ?"
    while [[ true ]]; do
         echo -n "Enter yes or no: "
         read x
         case "$x" in
             y | yes ) return 0;;
             n | no ) return 1;;
             * ) echo "Answer yes or no"
         esac
     done 
}

echo "Original parameter are $* "
#将脚本输入的第一个参数，函数的第一个参数

if yes_or_no "$1"
then
    echo "Hi $1 , nice name"
else
    echo "Never mind"
fi

exit 0
# 输入： ./yes_or_no.sh  Rick Neil
# 输出

# Original parameter are Rick Neil 
# Is your name Rick ?
# Enter yes or no: y
# Hi ./yes_or_no.sh  Rick Neil, nice name

```

#### 2.6.5 命令

shell脚本中可以执行的命令分为两类:

- 内置命令：shell中内置的命令,linux中大多数内部命令都提供了外部版本
- 外置命令:非shell中的内置命令。

**常用命令**

- `break`:跳出一层循环。
- `:`：是一个空命令。相当于true的别名，运行的比true快。例如`:${var:=value}`没有':'，shell将试图把$var作为一条命令来处理。
- `continue`：跳过剩下的直接进行下一次循环。
- `.`：表示在当前shell中执行命令`.`命令和`source`命令功能相同，可以避免创建shell执行时，由于新的环境所造成的变量丢失。例如:`. ./shell_script.sh`脚本中执行的变量会在当前shell中生效。
- `echo`: 现代脚本中推荐使用`printf`命令。
- `eval`：允许你对参数进行求值,相当于一个额外的`$`

```shell
#!/bin/bash

foo=10
x=foo
y='$'$x
echo $y

eval z='$'$x
echo $z 
# 输出： $foo 10
```
注意：`z='$'$x`单独的输出是`foo`基本无效。

- `exec`:
    + 将当前shell替换为一个不同的程序，exec后面的代码不会执行。如:`exec wall "Thanks for all"`
    + 修改当前文件描述符:`exec 3< afile`。
- `exit n`：脚本以结束码`n`退出。0表示成功退出，`1-125`是脚本可以使用的错误代码。其余数字有保留含义。
- `export`：将作为它参数的变量导出到子shell中，并使之在子shell中有效。主要是设置环境变量，这个环境变量可以被当前程序调用的其它脚本程序看到。
- `expr`：将他的参数，当做一个表达式来进行求值；如:

```
x=`expr $x +1 ` 

#或

x=$(expr $x + 1)
```
可以使用`$()`代替反引号。在比较新的脚本程序送`expr`命令被`$(())`命令所代替。

![常见表达式操作](https://wangpengcheng.github.io/img/2019-08-15-14-46-09.png)

- `printf`：字符输出命令;例如`prinf "%s \n" hello`

![支持的转义序列](https://wangpengcheng.github.io/img/2019-08-15-14-47-34.png)

![字符转换限定符](https://wangpengcheng.github.io/img/2019-08-15-14-49-16.png)

- `return`:函数的返回参数，没有指定参数则默认返回最后一条命令的退出码。
- `set`：设置shell参数变量，可以结合`$*`和`$2`等来进行参数变量的设置。
- `shift`：所有变量左移一个位置，将`$2`变为`$1`，`$3`变为`$2`。
- `trap`:`tarp command signal`,接收signal信号，再采取`command`行动。

![signal信号](https://wangpengcheng.github.io/img/2019-08-15-14-59-31.png)

- `unset`：从环境中删除变量或者函数。
- `find`:`find [path] [option] [testss] [actions]`找寻。

![find命令参数选项](https://wangpengcheng.github.io/img/2019-08-15-15-06-50.png)

![find测试部分](https://wangpengcheng.github.io/img/2019-08-15-15-08-40.png)

![find操作符](https://wangpengcheng.github.io/img/2019-08-15-15-09-28.png)

![find动作](https://wangpengcheng.github.io/img/2019-08-15-15-13-23.png)

- `grep`：`grep [option] PATTERN [FILES]`

![grep命令参数](https://wangpengcheng.github.io/img/2019-08-15-15-13-23.png)

##### 正则表达式

![正则表达式含义](https://wangpengcheng.github.io/img/2019-08-15-15-17-31.png)

![特殊匹配模式](https://wangpengcheng.github.io/img/2019-08-15-15-18-44.png)
![特殊匹配模式](https://wangpengcheng.github.io/img/2019-08-15-15-19-42.png)

`-E`扩展匹配选项：

![匹配选项](https://wangpengcheng.github.io/img/2019-08-15-15-21-47.png)

例如：

```shell
#查找以e结尾的字符

grep e$ words2.txt
#查找a-z重复10次的匹配字符

grep -E [a-z]\{10\} words2.txt

```

#### 2.6.6 命令的执行

脚本中获取命令结果可以使用`$()` ，例如`$(pwd)`获取当前输出文件夹。

##### 2.6.6.1 算术扩展

使用`expr`或者`$(())`可以进行算术扩展。

`${}`的使用：

![扩展参数列表](https://wangpengcheng.github.io/img/2019-08-15-15-32-32.png)

#### 2.6.8 调试脚本程序

![脚本程序调试选项](https://wangpengcheng.github.io/img/2019-08-15-15-35-31.png)

### 2.7 迈向图形化:dialog工具

使用dialog可以在shell中创建图形工具并且实现交互；

```
dialog --msgbox "hello word" 9 18
```

![对话框主要类型](https://wangpengcheng.github.io/img/2019-08-15-15-39-49.png)

![对话框参数](https://wangpengcheng.github.io/img/2019-08-15-15-41-11.png)

使用示例:

```
dialog --title "Check me" --checklist "Pick Numbers" 15 25 3 1 "one" "off" 2 "two" "on" 3 "tree" "off"
```

### 2.8 综合应用

[代码地址](https://github.com/wangpengcheng/Learning-Note/blob/master/code/Begining_linux_Proramming_/ch02/app/cd_db)

