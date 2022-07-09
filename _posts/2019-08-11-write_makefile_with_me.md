---
layout:     post
title:      跟我一起写makefile 学习笔记
subtitle:   跟我一起写makefile 
date:       2019-07-06
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - C++
    - 基础编程
    - Linux
    - MakeFile
---

> 2019-08-11 16:04:58

# 跟我一起写makefile

_参考链接：_ [GNU make](http://www.gnu.org/software/make/manual/make.html); [GUN Make指南](https://www.cnblogs.com/itech/archive/2009/09/15/1567001.html)
## 1 概述
make工具：

- Delphi-make
- Visual C++ nmake
- GNU make

文章主要针对GNU的make 

## 1 make 的使用

```shell
用法：make [选项] [目标] ...
选项：
  -b, -m                      忽略兼容性。
  -B, --always-make           无条件 make 所有目标。
  -C DIRECTORY, --directory=DIRECTORY
                              在执行前先切换到 DIRECTORY 目录。
  -d                          打印大量调试信息。
  --debug[=FLAGS]             打印各种调试信息。
  -e, --environment-overrides
                              环境变量覆盖 makefile 中的变量。
  --eval=STRING               Evaluate STRING as a makefile statement.
  -f FILE, --file=FILE, --makefile=FILE
                              从 FILE 中读入 makefile。
  -h, --help                  打印该消息并退出。
  -i, --ignore-errors         Ignore errors from recipes.
  -I DIRECTORY, --include-dir=DIRECTORY
                              在 DIRECTORY 中搜索被包含的 makefile。
  -j [N], --jobs[=N]          同时允许 N 个任务；无参数表明允许无限个任务。
  -k, --keep-going            当某些目标无法创建时仍然继续。
  -l [N], --load-average[=N], --max-load[=N]
                              在系统负载高于 N 时不启动多任务。
  -L, --check-symlink-times   使用软链接及软链接目标中修改时间较晚的一个。
  -n, --just-print, --dry-run, --recon
                              Don not actually run any recipe; just print them.
  -o FILE, --old-file=FILE, --assume-old=FILE
                              将 FILE 当做很旧，不必重新生成。
  -O[TYPE], --output-sync[=TYPE]
                              Synchronize output of parallel jobs by TYPE.
  -p, --print-data-base       打印 make 的内部数据库。
  -q, --question              Run no recipe; exit status says if up to date.
  -r, --no-builtin-rules      禁用内置隐含规则。
  -R, --no-builtin-variables   禁用内置变量设置。
  -s, --silent, --quiet       Don not echo recipes.
  -S, --no-keep-going, --stop
                              关闭 -k。
  -t, --touch                 touch 目标而不是重新创建它们。
  --trace                     Print tracing information.
  -v, --version               打印 make 的版本号并退出。
  -w, --print-directory       打印当前目录。
  --no-print-directory        关闭 -w，即使 -w 默认开启。
  -W FILE, --what-if=FILE, --new-file=FILE, --assume-new=FILE
                              将 FILE 当做最新。
  --warn-undefined-variables  当引用未定义变量的时候发出警告。

该程序为 x86_64-pc-linux-gnu 编译
报告错误到 <bug-make@gnu.org>

```

## 2 关于程序的编译与链接

具体的过程可以参照:[C/C++ 的一个符号操作问题](https://wangpengcheng.github.io/2019/05/12/c_peator_problem/)

编译时(.c->.o)：编译器需要的是语法正确，函数与变量的声明的正确。
链接时(.o->exe):主要是链接函数和全局变量。

## 3 Makefile介绍

为了方便使用gcc等编译器进行编译和链接，对复杂项目使用Makefile文件来，记录编译指令的顺序，相当于另外的针对gcc等编译器的shell脚本。使用make命令来执行相关的命令。

### 3.1 Makefile的规则

基本指令格式：

```makefile
target... : prerequisites ...
    command
    ...
    ...

#或者

targets : prerequisites ; command
    command
...
```

- target:编译目标文件；可以使用ObjectFile、label或者可执行文件
- prerequisites 生成所需要的文件或者目标
- command 就是make需要执行的命令(任意的shell命令)

注意这里的`command`如果不是和target在同一行，则必须以[Tab]开头。命令行可以使用`\`作为续行符。

简单示例：
```makefile
edit : main.o kbd.o command.o display.o \
    insert.o search.o files.o utils.o
    cc -o edit main.o kbd.o command.o display.o \
    insert.o search.o files.o utils.o
main.o : main.c defs.h
    cc -c main.c
kbd.o : kbd.c defs.h command.h
    cc -c kbd.c
command.o : command.c defs.h command.h
    cc -c command.c
display.o : display.c defs.h buffer.h
    cc -c display.c
insert.o : insert.c defs.h buffer.h
    cc -c insert.c
search.o : search.c defs.h buffer.h
    cc -c search.c
files.o : files.c defs.h buffer.h command.h
    cc -c files.c
utils.o : utils.c defs.h
    cc -c utils.c
clean :
    rm edit main.o kbd.o command.o display.o \
    insert.o search.o files.o utils.o

```
注意这里的clean是一个label相当于一个记录动作的变量

### 3.3 make工作流程

- 查找当前目录下的“Makefile”或者“makefile”文件。
- 找到文件，查找文件中的第一个目标文件(target)。并将其设置为最终的目标文件
- 如果文件不存在，或者相关依赖需要其它的动作生成，或者依赖文件的文件修改时间要比edit这个文件新，则执行后面的命令生成相关目标文件。
- 一层层的查找相关依赖，进行递归，执行程序。

### 3.4 makefile中使用变量

可以直接使用`=`来进行变量赋值。如下：
```makefile
objects = main.o kbd.o command.o display.o \
    insert.o search.o files.o utils.o
edit : $(objects)
    cc -o edit $(objects)
main.o : main.c defs.h
    cc -c main.c
kbd.o : kbd.c defs.h command.h
    cc -c kbd.c
command.o : command.c defs.h command.h

    cc -c command.c
display.o : display.c defs.h buffer.h
    cc -c display.c
insert.o : insert.c defs.h buffer.h
    cc -c insert.c
search.o : search.c defs.h buffer.h
    cc -c search.c
files.o : files.c defs.h buffer.h command.h
    cc -c files.c
utils.o : utils.c defs.h
    cc -c utils.c
clean :
    rm edit $(objects)
```
可以使用`$(变量名)`来对变量进行使用。

### 3.5 makefile 的自动推导

makefile可以自动推导文件，以及文件依赖关系后面的命令，可以使用makefile中的自动推导(看到xxx.o就会将xxx.c文件添加在依赖关系中)。

```makefile

objects = main.o kbd.o command.o display.o \
    insert.o search.o files.o utils.o
edit : $(objects)
    cc -o edit $(objects)
main.o : defs.h
kbd.o : defs.h command.h
command.o : defs.h command.h
display.o : defs.h buffer.h
insert.o : defs.h buffer.h
search.o : defs.h buffer.h
files.o : defs.h buffer.h command.h
utils.o : defs.h
#别名
.PHONY : clean
clean :
    rm edit $(objects)

#也可以这样写

objects = main.o kbd.o command.o display.o \
    insert.o search.o files.o utils.o
edit : $(objects)
    cc -o edit $(objects)
$(objects) : defs.h
  kbd.o command.o files.o : command.h
  display.o insert.o search.o files.o : buffer.h

.PHONY : clean

clean :
    rm edit $(objects)
```

## 4 Makefile总述

Makefile主要包括内容

- 显式规则:直接明文书写的规则
- 隐式规则：自动推导出来的隐晦规则
- 变量的定义:一系列字符串变量的定义
- 文件指示：根据条件的外部makefile的引用导入
- 注释：脚本注释文件

make查找当前目录下的文件顺序是：

-  GNUmakefile
-  makefile
-  Makefile
推荐使用Makefile，最好不要使用GNUMakefile因为这个只能被GNU的make识别。
可以使用`make -f file_name`来指定makefile文件，例如`make -f Make.Linux`。

makefile中可以使用`include`引入其它makefile文件
注意：include前不能加入[Tab]键，可以使用空格

```makefile
bar=a.mk b.mk c.mk e.mk 

include foo.make *.mk $(bar)

```
include的查找路径如下：

- 存在`-I`或者`--include-dir`参数会在这个参数指定的目录下继续寻找
- 存在`<prefix>/include`存在，make也会去寻找。

注意：当没有找到时，make会生成一条警告，并在最终时再次尝试寻找，如果还是没找到就是error。可以使用`-include`告诉make **忽略这个的读取错误**，直接继续执行。当然不建议这样做。

**环境变量**

在当前的环境变量中定义了，那么所有的make都会使用，因此建议不要使用。

**make的工作方式**

1. 读入所有的 Makefile 。
2. 读入被 include 的其它 Makefile 。
3. 初始化文件中的变量。
4. 推导隐晦规则,并分析所有规则。
5. 为所有的目标文件创建依赖关系链。
6. 根据依赖关系,决定哪些目标要重新生成。
7. 执行生成命令

### 5.3 在规则中使用通配符

make与shell相同支持三种通配符号：

- `*`:通用匹配符号，真实的'*'可以使用`\*`来表示
- `?`:判断符号:
- `[...]`:
- `$@`:表示规则中的目标文件集。
    + 在模式规则中，如果有多个目标，那么，"$@"就是匹配于目标中模式定义的集合。
- `$%`:仅当目标是函数库文件中，表示规则中的目标成员名。
    + 例如，如果一个目标是"foo.a (bar.o)"，那么，"$%"就是"bar.o"，"$@"就是"foo.a"。如果目标不是函数库文件（Unix下是[.a]，Windows下是 [.lib]），那么，其值为空。
- `$<`:依赖目标中的第一个目标名字。
    + 如果依赖目标是以模式（即"%"）定义的，那么"$<"将是符合模式的一系列的文件集。注意，其是一个一个取出来的。
- `$?`:所有比目标新的依赖目标的集合。以空格分隔。
- `$^`:所有的依赖目标的集合。以空格分隔。
    + 如果在依赖目标中有多个重复的，那个这个变量会去除重复的依赖目标，只保留一份。
- `$+`:这个变量很像"$^"，也是所有依赖目标的集合。只是它不去除重复的依赖目标。
- `$*`:这个变量表示目标模式中"%"及其之前的部分。
    + 如果目标是"dir/a.foo.b"，并且目标的模式是"a.%.b"，那么，"$*"的值就是"dir /a.foo"。这个变量对于构造有关联的文件名是比较有较。如果目标中没有模式的定义，那么"$*"也就不能被推导出，但是，如果目标文件的后缀是 make所识别的，那么"$*"就是除了后缀的那一部分。例如：如果目标是"foo.c"，因为".c"是make所能识别的后缀名，所以，"$*"的值 就是"foo"。这个特性是GNU make的，很有可能不兼容于其它版本的make，所以，你应该尽量避免使用"$*"，除非是在隐含规则或是静态模式中。如果目标中的后缀是make所不 能识别的，那么"$*"就是空值。
例如：

```makefile

print:*.c
    lpr -p $?
    touch print
```

- `$(@D)`:表示`$@`的目录部分，如果 "$@" 值是 "dir/foo.o" ,那么 "$(@D)" 就是 "dir" ,而如果 "$@" 中没有包含斜杠的话,其值就是 "." (当前目录)。
- `$(@F)`:表示`$@`的文件部分。
剩下的$(操作符D)和$(操作符F)功能一次类推。

### 5.4文件查询

makefile中可以使用VPATH来指定文件查找目录。使用方法如下：

- 1.vpath <pattern> <directories>:为符合模式<pattern>的文件指定搜索目录<directories>。
- 2.vpath <pattern> ：清除符合模式<pattern>的文件的搜索目录。
- vpath:清除所有已经被设置好了的文件搜索目录

```makefile
#指定文件搜索在../headers和当前目录下，查找所有以`.h`结尾的文件

vpath %.h ../headers
#这里是先在foo中搜索，然后在blish,最后是bar

vpath %.c foo:bar
vpath % blish
```

### 5.5 伪目标

makefile中可以使用`.PHONY:xxx`的方式来设置伪目标，类似于shell中的`alias`操作别名
伪目标一般没有依赖文件，主要是相关操作。也可以使用依赖来化简操作。

```makefile
all:prog1 prog2 prog3
.PHONY:all

prog1:prog1.o utils.o
        cc -o prog1 prog1.o utils.o
prog2:...

#也可以使用伪目标作为依赖，实现操作如下
cleanall : cleanobj cleandiff
    rm program
cleanobj:
    rm *.o
cleandiff:
    rm *.diff

```
因为makefile会将第一个目标作为默认目标，因此可以使用伪目标依赖，来实现makefile的多目标依赖。

### 5.7 静态模式

使用静态模式可以更加容易地定义多目标的规则。使用语法如下

```makefile
<targets ...>: <target-pattern>: <prereq-patterns ...>
    <commands>
    ...

```
target:定义了一系列目标，可以有通配符，是目标的集合
target-pattern:指明了target的模式，也是目标集模式
prereq-patterns:是目标依赖模式对target-pattern的再次依赖定义

使用示例：

```makefile

    objects = foo.o bar.o
    all: $(objects)

#将后缀中的.o变为.c

$(objects): %.o: %.c
    $(CC) -c $(CFLAGS) $< -o $@
    # $(CC) -c $(CFLAGS) foo.c bar.c -o foo.o bar.o

```

### 5.8 自动生成依赖性

gcc中可以使用-M编译选项开启自动依赖查找。

## 6 书写命令
使用`@`在命令前，这个命令将不会被make显示出来。例如:

```makefile
@echo 正在编译XXX模块
#输出：正在编译XXX模块

echo 正在编译XXX模块
#输出：

#    echo 正在编译XXX模块

#   正在编译XXX模块

```

可以在make后面带入参数`-n`或者`--just-print`，只执行显示命令，但是不会执行命令。方便对Makefile进行调试。

使用`-s`或者`--slient`则是全面禁止命令的显示。

### 6.2 命令执行

makefile中的命令会，一条一条的执行其后面的的命令，当需要上一命令结果应用在下一命令时，应该使用分号，分割相关命令。当命令相关联时，应该写在一行上。

```makefile

exec:
    cd /home/
    pwd
# 打印执行文件夹路径，cd无作用。

exec:
    cd /home/;pwd
# 打印 /home/ cd 有作用
```

### 6.3 命令错误
make中命令错误会立即停止执行，因此需要，使用make clean进行重新的make; 可以在命令前使用`-`或者添加make的`-i/ignore-errors`参数实现忽略错误。可以使用`-k/--keep-going`保证make行为不停止。

### 6.4 make的嵌套执行

可以使用shell的脚本访问，执行子目录中的make，对与方便第三方依赖的单独编译。
```makefile
subsystem:
    cd subdir && $(MAKE)
#等价于

subsystem:
    $(MAKE) -C subdir
```
使用export 和unexport来进行变量的下级传递和使用。

注意：
- SHELL和MAKEFLAGS是一定会传递到下一层Makefile中的。
- 可以使用`-w/--print-directory`；输出信息，让你看到当前的工作目录。

### 6.5 定义命令包

Makefile中可以使用相同命令序列来定义一个变量。以`define`开始,`endef`结束。例如:

```makefile
define run-yacc
yacc $(firstword $^)
mv y.tab.c $@
endef
```

## 7 使用变量

makefile中的变量，可以是数字开头，但不应该含有`:`、`#`、`=`或是空字符(空格、回车等)。并且变量的大小写敏感。

### 7.1 变量基础

可以定义变量，并用`${变量名}/$(变量名)`来使用变量，可以精确使用和展开，字符串之间的链接等。

```makefile
foo=c
prog.o:prog.$(foo)
    $(foo)$(foo) -$(foo) prog.$(foo)

#翻译结果

prog.o : prog.c
    cc -c prog.c
```

### 7.2 变量中的变量

makefile中的变量，不像c++需要预先声明，可以直接使用，只要make能在后面找到就行了，但是很容易引起嵌套的循环使用。

变量的赋值

|操作|含义|
|:---|:---|
|`=`|变量的直接赋值,可以使用后面定义的变量|
|`:=`|赋值操作，前面的变量不能使用后面的变量，只能使用前面已经定义好了的变量|
|`?=`|如百年来那个没有被定义过，则进行赋值，如果事先被定义过，就什么都不做|
|`+=`|追加变量，给变量`+=`操作，可以追加新的变量|

注意：谨慎使用`#`注释，在行后直接注释无`\n`换行符时，会引起变量中存在多余的空格；例如`dir := /foo/bar # directory to put the frobs in`中dir会多4个空格。

### 7.3 变量的高级用法

- 变量的替换：替换变量中的共有的部分，格式是`$(var:a=b)`或者`${var:a=b}`把变量`var`中所有以子没有`a`结尾(空格或者结束符)的`a`换成`b`。这样就可以执行简单的变量替换。

```
foo:=a.o b.o c.o
bar:=$(foo:.o=.c)

#bar是a.c b.c c.c
```
也可以使用静态模式进行替换。

- 将变量值作为变量：将变量结果作为字符串使用，获取新变量的名称。

```
x=y
y=z
a:=$($(x))
#最终a的结果是z

```

也可以进行多层次的嵌套。

### 7.5 override 指示符号

如果变量是通过make的命令行参数进行设置的，那么Makefile中对这个变量的赋值将会被忽略。可以使用`override`指示符，在Makefile中设置这类参数的值，进行重载

```
override <variable>=<value>
override <variable>:=<value>
override <variable>+=<value>
```

### 7.6 多行变量

使用define关键字设置变量的值可以换行，只要define没有使用[Tab]开头，那么make就不会把其认为是命令

```
define two-lines
echo foo
echo ${bar}
endef
```

### 7.7 环境变量

make运行时，系统环境变量可被载入到Makefile文件中，但是如果Makefile已经定义，则会被覆盖，可以使用`-e`参数方式系统环境变量被覆盖。

嵌套调用时，上层变量会以系统环境变量的方式传递到下层Makefile。默认情况只有通过命令行设置的变量会被传递。定义在文件中的变量，需要使用export关键字来声明。

### 7.8 目标变量

可以通过伪目标标签，设置目标局部变量，这个变量可以额全局变量同名，因为它的作用范围只在这条规则以及连带规则中。语法如下：

```
<target ...>:<variable-assignment>
<target ...>:overide <variable-assignment>
```

使用示例：
```makefile
all: tran test test2
my_test:=/usr/binb
my_test:=${my_test:inb=a} /usr/include/boost 
tran:
    @echo "hello word"
test2:my_test=test2
test2:
    echo ${my_test}
test:
    echo "test";echo $(SHELL) ;echo $(MAKEFLAGS) echo ${my_test}

#result
hello word
echo "test";echo /bin/sh ;echo  echo /usr/ba /usr/include/boost 
test
/bin/sh
echo /usr/ba /usr/include/boost
echo test2
test2

```

### 7.9 模式变量

在GNU的make中还支持模式变量，使用一种模式，在模式中定义变量，使得变量可以根据模式进行判断赋值。使用格式和目标变量相同，例如：

```
%.o:CFLAGS = -O
```

## 8 使用条件判断

使用条件判断根据不同的分枝情况选择执行分支。

下面是根据编译器选择相关编译参数

```makefile
libs_for_gcc=-lgnu
normal_libs=
foo:$(objects)
#判断编译器类型

ifeq($(CC),gcc)
    $(CC) -o foo $(objects) $(libs_for_gcc)
else
    $(CC) -o foo $(object) $(normal_libs)
endif
```

判断语句的基本语法是：
```makefile
<conditional-directive>
<text-if-true>
endif
#或者

<conditional-directive>
<text-if-true>
else
<text-if-false>
endif

```
条件判断是ifeq或者ifneq、ifdef和ifndef

```
ifeq(<arg1>,<arg2>)
ifeq '<arg1>' '<arg2>'
ifeq "<arg1>" "<arg2>"
ifeq "<arg1>" '<arg2>'
ifeq '<arg1>' "<arg2>"
#还可以这样判断

ifeq ($(strip $(foo)),)
<text-if-empty>
endif
```

ifneq:

```makefile
ifneq (<arg1>, <arg2>)
ifneq '<arg1>' '<arg2>'
ifneq "<arg1>" "<arg2>"
ifneq "<arg1>" '<arg2>'
ifneq '<arg1>' "<arg2>"
```

ifdef和ifndef
```
ifdef <variable-name>
endif

ifndef <variable-name>
endif
```

## 9 使用函数

Makefile可以使用函数来处理变量，变量以`$`进行标识，其基本语法如下：

```
$(<function> <arguments>)
#或者

${<function> <arguments>}

```

中间参数以`,`进行分割。
为了风格统一，建议都使用`()`的方式；如：`$(subst a,b,$(x))`

```makefile
comma:=,
empty:=
space:=$(empty) $(empty)
foo:=a b c
bar:=$(subst $(space),$(comma),$(foo))
```
上面的函数是指，将$(foo)中的$(space)全部替换成为$(comma)。
最终$(bar)的结果是"a,b,c"。

### 9.2 字符串处理函数

|函数名称|函数|功能|返回|示例|注意|
|:---|:---|:----|:-----|:------|:--------|
|`subst`|`$(subst <from>,<to>,<text>)`|把字串 <text> 中的 <from> 字符串替换成 <to>|函数返回被替换过后的字符串|`$(subst ee,EE,feet on the street)`||
|`patsubst`|`$(patsubst <pattern>,<replacement>,<text>)`|查找其中的匹配字符串并替换|替换过后的字符串|`$(patsubst %.c,%.o,x.c.c bar.c)`||
|`strip`|`$(strip <string>)`|去掉<string>字符串中开头和结尾的空字符串|返回去除的值|`$(strip a b c )`|示例中去除了结尾的一个空格|
|`findstring`|`$(findstring <find>,<in>)`|在字串 <in> 中查找 <find> 字串。|如果找到,那么返回 <find> ,否则返回空字符串。|`$(findstring a,a b c)`|示例返回"a"字符串|
|`filter`|`$(filter <pattern...>,<text>)`|以 <pattern> 模式过滤 <text> 字符串中的单词,保留符合模式 <pattern> 的单词。可以有多个模式。|返回符合模式 <pattern> 的字串。|`$(filter %.c %.s,foo.c bar.c baz.s ugh.h)`|示例返回值是`foo.c bar.c baz.s`|
|`filter-out`|`$(filter-out <pattern...>,<text>)`|以 <pattern> 模式过滤 <text> 字符串中的单词,去除符合模式 <pattern> 的单词。可以有多个模式。|返回不符合模式 <pattern> 的字串。|`$(filter-out main1.o main2.o,main1.o foo.o main2.o bar.o)`|返回值是`foo.o bar.o`|
|`sort`|`$(sort <list>)`|给字符串 <list> 中的单词排序(升序)。|返回排序后的字符串。|`$(sort foo bar lose)`返回`bar foo lose`|sort 函数会去掉 <list> 中相同的单词。|
|`word`|`$(word <n>,<text>)`|取字符串 <text> 中第 <n> 个单词。(从一开始)|返回字符串 <text> 中第 <n> 个单词。如果 <n> 比 <text> 中的单词数要大,那么返回空字符串。|`$(word 2, foo bar baz)`|示例返回值是"bar"|
|`wordlist`|`$(wordlist <s>,<e>,<text>)`|字符串 <text> 中取从 <s> 开始到 <e> 的单词串。 <s> 和 <e> 是一个数字|返回字符串 <text> 中从 <s> 到 <e> 的单词字串。如果 <s> 比 <text> 中的单词数要大,那么返回空字符串。如果 <e> 大于 <text> 的单词数,那么返回从 <s> 开始,到 <text> 结束的单词串|`$(wordlist 2, 3, foo bar baz)`|返回值是`bar baz`|
|`words`|`$(words <text>)`|统计 <text> 中字符串中的单词个数。|返回 <text> 中的单词数|`$(words, foo bar baz)`返回值为"3"|如果我们要取 <text> 中最后的一个单词,我们可以这样: $(word $(words <text>),<text>) 。|
|`firstword`|`$(firstword <text>)`|取字符串 <text> 中的第一个单词。|返回字符串 <text> 的第一个单词|`$(firstword foo bar)`返回"foo"||

### 9.3 文件名操作函数

- dir: `$(dir <name...>)`
    + 功能:从文件序列<names>中取出目录部分(最后一个“/”之前的部分)。
    + 返回：返回文件名序列 <names> 的目录部分。
    + 示例：`$(dir src/foo.c hacks)`返回值是`src/ ./`
- notdir:`$(notdir <names...>)`
    + 功能：从文件名序列 <names> 中取出非目录部分。非目录部分是指最后一个反斜杠(“ / ”)之后的部分。
    + 返回：返回文件名序列 <names> 的非目录部分。
    + 示例：`$(notdir src/foo.c hacks)`，返回值是"foo.c hacks"。
- suffix:`$(suffix <names...>)`
    + 功能：从文件名序列 <names> 中取出各个文件名的后缀。
    + 返回：返回文件名序列 <names> 的后缀序列,如果文件没有后缀,则返回空字串。
    + 示例：`$(suffix src/foo.c src-1.0/bar.c hacks)`；返回值是".c .c"。
- basename:`$(basename <names...>)`
    + 功能：从文件名序列 <names> 中取出各个文件名的前缀部分。
    + 返回：返回文件名序列 <names> 的前缀序列,如果文件没有前缀,则返回空字串。
    + 示例：`$(basename src/foo.c src-1.0/bar.c hacks)`；返回值是"src/foo src-1.0/bar hacks"。
- addsuffix：`$(addsuffix <suffix>,<names...>)`
    + 功能：把后缀 <suffix> 加到 <names> 中的每个单词后面。
    + 返回：返回加过后缀的文件名序列。
    + `$(addsuffix .c,foo bar)`,返回值是"foo.c bar.c"。
- addprefix：`$(addprefix <prefix>,<names...>)`
    + 功能：把前缀 <prefix> 加到 <names> 中的每个单词后面。
    + 返回：返回加过前缀的文件名序列。
    + 示例：`$(addprefix src/,foo bar)`；返回值是"src/foo src/bar"
- join:`$(join <list1>,<list2>)`
    + 功能：把 <list2> 中的单词对应地加到 <list1> 的单词后面。如果 <list1> 的单词个数要比 <list2> 的多,那么,<list1> 中的多出来的单词将保持原样。如果 <list2> 的单词个数要比 <list1> 多,那么, <list2> 多出来的单词将被复制到 <list2> 中。
    + 返回：返回连接过后的字符串。
    + 示例：`$(join aaa bbb , 111 222 333)`；返回值是`aaa111 bbb222 333`

### 9.4 foreach函数

- 语法：`$(foreach <var>,<list>,<text>)`
- 功能：将list中的单词逐一去除，放到var指定的变量中，然后再执行<text>所包含表达式。
- 示例：

```makefile
names:=a b c d
files:=$(foreach n,$(names),$(n).o)

#files返回值是 a.o b.o c.o d.o

```
### 9.6 call函数

call函数是唯一一个可以用来创建新的参数化的函数，可以使用call向一个复杂函数中传递参数。其语法是：
`$(call <expression>,<parm1>,<parm2>,<parm3>...)`

执行时，后面的参数将函数中的变量进行取代。

```
reverse=$(1) $(2)

foo=$(call reverse,a,b)
#最后foo的结果是“a b”

```

### 9.7 origin 函数

这个函数返回变量的来源；语法：`$(origin <variable>)`;来源类型如下：
- undefined：从来就没有定义过
- default:默认定义变量，如CC
- environment:环境变量(注意make 没有“-e”参数)
- file: 这个变量定义在Makefile中
- command line :这个变量是被命令行定义的。
- override :是被override重新定义的。
- automatic: 是命令中定义的自动化变量。

### 9.8 shell函数

执行shell命令，它和"`"是一样的功能。将执行操作系统命令后的输出作为函数返回。使用示例：
```
contents:=$(shell cat foo)
files:=$(shell echo *.c)
```
注意这个函数会新生成一个shell程序来执行命令。不应该被大量使用。

### 9.9 控制make的函数

$(error <text...>):输出错误，并退出
$(warning <text...>)：输出一段警告信息，而make继续执行。

## 10 make的运行

### 10.1 make的退出码

- 0：表示成功执行
- 1：运行时产生错误，返回1。
- 2：如果使用了“-q”参数，并且make使得一些目标不需要更新，那么返回2。

使用`-f`可以指定make file文件，也可以在make后面添加指定的伪目标。

一般伪目标函数

- `all`：这个伪目标是所有目标的目标,其功能一般是编译所有的目标。
- `clean`：这个伪目标功能是删除所有被 make 创建的文件。
- `install`：这个伪目标功能是安装已编译好的程序,其实就是把目标执行文件拷贝到指定的目标中去。
- `print`：这个伪目标的功能是例出改变过的源文件。
- `tar`：这个伪目标功能是把源程序打包备份。也就是一个 tar 文件。
- `dist`：这个伪目标功能是创建一个压缩文件,一般是把 tar 文件压成 Z 文件。或是 gz 文件。
- `TAGS`：这个伪目标功能是更新所有的目标,以备完整地重编译使用。
- `check`：check ” 和 “test”这两个伪目标一般用来测试 makefile 的流程。

### 10.4 检查规则

我们不想makefile中的规则执行起来，只想检查一下命令，可以使用一下相关参数：
- `- n`
- `--just-print`
- `--dry-run`
- `--recon`：不执行参数,这些参数只是打印命令,不管目标是否更新,把规则和连带规则下的命令打印出来,但
不执行,这些参数对于我们调试 makefile 很有用处。
- `- t` `--touch`:这个参数的意思就是把目标文件的时间更新,但不更改目标文件。也就是说, make 假装编译目标,但不是真正的编译目标,只是把目标变成已编译过的状态。
- `- q` `--question`这个参数的行为是找目标的意思,也就是说,如果目标存在,那么其什么也不会输出,当然也不会执行编译,如果目标不存在,其会打印出一条出错信息。
- `-W <file>`
- `--what-if=<file>``
- `--assume-new=<file>`
- `--new-file=<file>`
这个参数需要指定一个文件。一般是是源文件(或依赖文件), Make 会根据规则推导来运行依赖于这
个文件的命令,一般来说,可以和“ -n ”参数一同使用,来查看这个依赖文件所发生的规则命令。
另外一个很有意思的用法是结合“ -p ”和“ -v ”来输出 makefile 被执行时的信息(这个将在后面讲述)。

### 10.5 make参数请查看相关文档。

## 11 隐含规则

隐含规则主要是make的自动推导。
会为.o自动配置生成依赖生成项。
注意
- 因为隐藏规则的存在，当执行混合编译的时候，可能存在隐藏规则先后性的问题，例如`foo:foo.p`如果文件夹下存在`foo.c`隐含规则就会找到foo.c执行过后就不再向下找了。
- 可以使用`-r/--no-builtin-rules`取消所有隐含参数设置。
- 编译c时，<n>.o会自动添加目标依赖推导<n>.c，并生成指令`$(CC) –c $(CPPFLAGS) $(CFLAGS)`
- 编译c++时，会自动推导为<n>.cc或者<n>.C,生成命令`$(CXX) $(CPPFLAGS) $(CFLAGS)`
- 编译Pascal时，自动推导为<n>.p,生成指令为`$(PC) –c $(PFLAGS)`;
- 编译Fortran/Ratfor时,自动推导为 “<n>.r” 或 “<n>.F” 或 “<n>.f”,其生成命令是:
```
".f" "$(FC) –c $(FFLAGS)"
".F" "$(FC) –c $(FFLAGS) $(CPPFLAGS)"
".f" "$(FC) –c $(FFLAGS) $(RFLAGS)"
```
- 汇编和汇编预处理：自动推导为<n>.s默认使用编译器“as”，生成命令：`$(AS) $(ASFLAGS)`。
- 链接Object文件：运行C的编译器来运行链接程序(一般是“ld”),生成命令：`$(CC) $(LDFLAGS) <n>.o $(LOADLIBES) $(LDLIBS)`

### 11.3 隐含规则使用的变量
make中会预先定义一些执行的变量

- AR:函数库打包程序。默认命令是“ ar ”
- AS:汇编语言编译程序。默认命令是“ as ”。
- CC:C语言编译程序，默认命令是“cc”。
- CXX:C++ 语言编译程序。默认命令是“ g++ ”。
- CO:从 RCS 文件中扩展文件程序。默认命令是“ co ”。
- CPP：C 程序的预处理器(输出是标准输出设备)。默认命令是“ $(CC) – E ”。
- FC:Fortran 和 Ratfor 的编译器和预处理程序。默认命令是“ f77 ”。
- GET：从 SCCS 文件中扩展文件的程序。默认命令是“ get ”。
- LEX：Lex 方法分析器程序(针对于 C 或 Ratfor )。默认命令是“ lex ”。
- PC：Pascal 语言编译程序。默认命令是“ pc ”。
- YACC：Yacc 文法分析器(针对于 C 程序)。默认命令是“ yacc ”。
- YACCR:Yacc 文法分析器(针对于 Ratfor 程序)。默认命令是“ yacc – r ”。
- MAKEINFO:转换 Texinfo 源文件( .texi )到 Info 文件程序。默认命令是“ makeinfo ”。
- TEX:从 TeX 源文件创建 TeX DVI 文件的程序。默认命令是“ tex ”。
- TEXI2DVI:从 Texinfo 源文件创建军 TeX DVI 文件的程序。默认命令是“ texi2dvi ”。
- WEAVE:转换 Web 到 TeX 的程序。默认命令是“ weave ”。
- CWEAVE:转换 C Web 到 TeX 的程序。默认命令是“ cweave ”。
- TANGLE:转换 Web 到 Pascal 语言的程序。默认命令是“ tangle ”。
- CTANGLE:转换 C Web 到 C 。默认命令是“ ctangle ”。
- RM:删除文件命令。默认命令是“ rm – f ”。

对应编译器参数变量：
基本都是编译器名+FLAGS；如：CXXFLAGS

注意隐含规则链：
- 当存在编译器的相互准话你，例如一个.o文件的生成，可能是先将.y文件生成.c再由C的编译器生成。make会进自己最大的努力进行编译转换和推导。为了安全起见，需要使用伪目标“.INTERMEDIATE”来强制声明中间件。
- 禁止同一个目标出现两次或者以上。防止自动推导时出现无限递归的情况。
- make会对简单的中间产物进行优化，可能不会产生中间文件。

### 11.5 定义模式规则
模式规则中一般包含“%”,主要代表任意性的推导。例如

```makefile
%.o : %.c
    $(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
#将所有.c文件编译成为.o文件
# $<表示了所有依赖目标的挨个值，$@表示所有的目标的挨个值

%.tab.c %.tab.h: %.y
    bison -d $<

# 将所有的[.y]文件都以bison -d $<执行。生成对应的<n>.tab.c和<n>.tab.h文件

```

#### 11.5.3 自动化变量

5.3节中已有描述，不再赘述。


可以通过在隐含规则和后面不添加命令，来覆盖隐含规则。或者添加命令，重建隐含规则。

make注意使用后缀来进行推导，因此可以更改`.SUFFIXES`来进行默认后缀的更改。make 的参数 "-r" 或 "-no-builtin-rules" 也会使用得默认的后缀列表为空。

### 11.7 隐含规则搜索算法

对于目标T的搜索算法如下所示：

- 把 T 的目录部分分离出来。叫 D ,而剩余部分叫 N 。(如:如果 T 是 "src/foo.o" ,那么, D 就是 "src/" ,N 就是 "foo.o" )
- 创建所有匹配于 T 或是 N 的模式规则列表。
- 如果在模式规则列表中有匹配所有文件的模式,如 "%" ,那么从列表中移除其它的模式。
- 移除列表中没有命令的规则。
- 对于第一个在列表中的模式规则：
    + 推导其 " 茎 "S , S 应该是 T 或是 N 匹配于模式中 "%" 非空的部分。
    + 计算依赖文件。把依赖文件中的 "%" 都替换成 " 茎 "S 。如果目标模式中没有包含斜框字符,而把 D 加在第一个依赖文件的开头。
    + 测试是否所有的依赖文件都存在或是理当存在。(如果有一个文件被定义成另外一个规则的目标文件,或者是一个显式规则的依赖文件,那么这个文件就叫 " 理当存在 " )
    + 如果所有的依赖文件存在或是理当存在,或是就没有依赖文件。那么这条规则将被采用,退出该算法。
- 如果经过第 5 步,没有模式规则被找到,那么就做更进一步的搜索。对于存在于列表中的第一个模式规则：
    + 如果规则是终止规则,那就忽略它,继续下一条模式规则。
    + 计算依赖文件。(同第 5 步)
    + 测试所有的依赖文件是否存在或是理当存在。
    + 对于不存在的依赖文件,递归调用这个算法查找他是否可以被隐含规则找到。
    + 如果所有的依赖文件存在或是理当存在,或是就根本没有依赖文件。那么这条规则被采用,退出该算法。
- 如果没有隐含规则可以使用,查看 ".DEFAULT" 规则,如果有,采用,把 ".DEFAULT" 的命令给 T
使用。

当规则被找到，就会执行其相当的命令，此时自动化变量才会生成。

## 12 使用 make 更新函数库文件

函数库文件也就是对 Object 文件(程序编译的中间文件)的打包文件。在 Unix 下,一般是由命令 "ar"
来完成打包工作。

### 12.1 函数库文件的成员

一个函数库文件由多个文件组成。你可以以如下格式指定函数库文件及其组成:
`archive(member)`

这个主要是对目标和依赖的定义。主要是为了“ar”来进行服务。如:
```
foolib(hack.o) : hack.o
    ar cr foolib hack.o

```

### 12.2 函数库成员的隐含规则

当目标是"a(m)"，形式时，其会把目标转换为(m),例如"make foo.a(bar.o)"的形式调用Makefile时，隐含规则会去找"bar.o"的规则，如果没有定义相关规则，那么内建隐含规则生效。make会去找对应的文件来生成目标，找到则成功执行。

注意： "$%"是专属函数库文件的自动化变量。

### 12.3 函数库文件的后缀规则

使用后缀规则和隐含规则来生成函数库打包文件，如：
```
.c.a:
    $(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $*.o
    $(AR) r $@ $*.o
    $(RM) $*.o

```
等价于：

```
(%.o) : %.c
    $(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $*.o
    $(AR) r $@ $*.o
    $(RM) $*.o

```

注意：

进行函数库打包文件生成时,请小心使用 make 的并行机制( "-j" 参数)。如果多个 ar 命令在同一时
间运行在同一个函数库打包文件上,就很有可以损坏这个函数库文件。