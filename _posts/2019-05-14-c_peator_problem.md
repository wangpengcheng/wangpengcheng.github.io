---
layout:     post
title:      C/C++ 的一个符号操作问题
subtitle:   深入思考操作符
date:       2019-05-12
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - C++
    - C
    - java
    - 汇编
    - 反汇编
    - 基础操作符：
---

>2019-5-14 19:54:52

# C/C++ 的一个符号操作问题

今天发现一串奇异的代码，和师兄们一起讨论研究之后，汇总成这篇文章。

首先先亮出代码：

```c
//a.c

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int a=10;
    //注意下面的操作
    
    a+=a-=a*=a;
    printf("%d\n", a);
    return 0;
}
//gcc 和g++编译结果都是0
```
上述代码`g++`和`gcc`编译结果都是0;使用
`gcc -S a.c -o gcc_a.s`和`g++ -S a.c -o g_a.s`得到其汇编代码相同如下：

```
    .file   "a.c"
    .section    .rodata
.LC0:
    .string "%d\n"
    .text
    .globl  main
    .type   main, @function
main:
.LFB2:
    .cfi_startproc
    pushq   %rbp
    .cfi_def_cfa_offset 16
    .cfi_offset 6, -16
    movq    %rsp, %rbp
    .cfi_def_cfa_register 6
    subq    $16, %rsp
    movl    $10, -4(%rbp)
    movl    -4(%rbp), %eax
    imull   -4(%rbp), %eax
    movl    %eax, -4(%rbp)
    movl    -4(%rbp), %eax
    subl    %eax, -4(%rbp)
    movl    -4(%rbp), %eax
    addl    %eax, -4(%rbp)
    movl    -4(%rbp), %eax
    movl    %eax, %esi
    movl    $.LC0, %edi
    movl    $0, %eax
    call    printf
    movl    $0, %eax
    leave
    .cfi_def_cfa 7, 8
    ret
    .cfi_endproc
.LFE2:
    .size   main, .-main
    .ident  "GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.11) 5.4.0 20160609"
    .section    .note.GNU-stack,"",@progbits

```
接下来就是对这一串汇编代码的解读了。

## GCC指令
_参考链接：_ [GCC](http://gcc.gnu.org/);[GCC online documentation](https://gcc.gnu.org/onlinedocs/);[GCC C++ Command Options](https://gcc.gnu.org/onlinedocs/gcc-5.5.0/gcc/#toc-GCC-Command-Options);[GCC参数详解](https://www.runoob.com/w3cnote/gcc-parameter-detail.html)

### GCC 编译步骤
_参考链接：_ [gcc程序的编译过程和链接原理](https://blog.csdn.net/czg13548930186/article/details/78331692);

GCC编译分为4步；

![编译流程图](https://img-blog.csdn.net/20180911135651696?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3pzajExMjY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

1. `gcc -E`:预处理阶段(preprocessing)，对包含的头文件（#include)和宏定义（#define，#ifdef等）进行处理。在上述的代码处理过程中，编译器将包含的头文件stdio.h编译进来，并且让用户使用选项”-E“
进行查看，该选项的作用是让gcc在预处理结束后停止编译过程。”.i“文件为已经过预处理的c程序。一下列出部分;处理操作： `gcc -E a.c -o  gcc_a_e.i -std=c99`
2. `gcc -S`:编译操作(compilation)，将C/C++或者`.i`文件编译成为汇编代码。处理操作：` gcc -S gcc_a_e.i -o  gcc_a_e.s -std=c99` 
3. `gcc -c`:汇编操作(assembly),将`.c`或者`.s`文件转化为对应的二进制文件`.o`或者`.obj`;处理操作：`gcc -c gcc_a_e_s.s -o gcc_a.o`;
4. `gcc -o a a.o`：链接操作(link);链接不同的`.o`文件，生成可执行的文件。

常用选项表

|选项|含义|
|:---|:---|
|`-v` | 查看gcc编译器的版本，显示gcc执行时的详细过程|
|`-o <file>`|Place the output into <file>;指定输出文件名为file，这个名称不能跟源文件名同名|
|`-E` |Preprocess only; do not compile, assemble or link；只预处理，不会编译、汇编、链接|
|`-S`|Compile only; do not assemble or link；只编译，不会汇编、链接|
|`-c`|Compile and assemble, but do not link； 编译和汇编，不会链接|
|`-On`|编译优化选项，默认等级`-O1`,`-O2`告诉GCC除了完成`-O1`级别的优化之外，同时还要进行一些额外的调整工作，如处理器指令调度等,选项`-O3`则除了完成`-O2`级别的优化之外，还包括循环展开和其他一些与处理器特性相关的优化工作|
|`-l`|`-lLIBRARY` 连接时搜索指定的函数库LIBRARY|
|`-I`|`Include` 添加`include`头文件|
|`-Ldir`|制定编译的时候，搜索库的路径。比如你自己的库，可以用它制定目录，不然编译器将只在标准库的目录找。这个dir就是目录的名称。|
|`-w/Wall`|不生成/生成所有警告信息|

#### 链接原理

使用  `gcc -V -o gcc_a_e gcc_a_e.o`查看链接详细操作如下：

```
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/5/lto-wrapper
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 5.4.0-6ubuntu1~16.04.11' --with-bugurl=file:///usr/share/doc/gcc-5/README.Bugs --enable-languages=c,ada,c++,java,go,d,fortran,objc,obj-c++ --prefix=/usr --program-suffix=-5 --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --with-sysroot=/ --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-libmpx --enable-plugin --with-system-zlib --disable-browser-plugin --enable-java-awt=gtk --enable-gtk-cairo --with-java-home=/usr/lib/jvm/java-1.5.0-gcj-5-amd64/jre --enable-java-home --with-jvm-root-dir=/usr/lib/jvm/java-1.5.0-gcj-5-amd64 --with-jvm-jar-dir=/usr/lib/jvm-exports/java-1.5.0-gcj-5-amd64 --with-arch-directory=amd64 --with-ecj-jar=/usr/share/java/eclipse-ecj.jar --enable-objc-gc --enable-multiarch --disable-werror --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
Thread model: posix
gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.11) 
COMPILER_PATH=/usr/lib/gcc/x86_64-linux-gnu/5/:/usr/lib/gcc/x86_64-linux-gnu/5/:/usr/lib/gcc/x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/5/:/usr/lib/gcc/x86_64-linux-gnu/
LIBRARY_PATH=/usr/lib/gcc/x86_64-linux-gnu/5/:/usr/lib/gcc/x86_64-linux-gnu/5/../../../x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/5/../../../../lib/:/lib/x86_64-linux-gnu/:/lib/../lib/:/usr/lib/x86_64-linux-gnu/:/usr/lib/../lib/:/usr/lib/gcc/x86_64-linux-gnu/5/../../../:/lib/:/usr/lib/
COLLECT_GCC_OPTIONS='-v' '-o' 'gcc_a_e' '-mtune=generic' '-march=x86-64'
 /usr/lib/gcc/x86_64-linux-gnu/5/collect2 -plugin /usr/lib/gcc/x86_64-linux-gnu/5/liblto_plugin.so -plugin-opt=/usr/lib/gcc/x86_64-linux-gnu/5/lto-wrapper -plugin-opt=-fresolution=/tmp/ccXTxtp9.res -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lgcc_s -plugin-opt=-pass-through=-lc -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lgcc_s --sysroot=/ --build-id --eh-frame-hdr -m elf_x86_64 --hash-style=gnu --as-needed -dynamic-linker /lib64/ld-linux-x86-64.so.2 -z relro -o gcc_a_e /usr/lib/gcc/x86_64-linux-gnu/5/../../../x86_64-linux-gnu/crt1.o /usr/lib/gcc/x86_64-linux-gnu/5/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/5/crtbegin.o -L/usr/lib/gcc/x86_64-linux-gnu/5 -L/usr/lib/gcc/x86_64-linux-gnu/5/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/5/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/5/../../.. gcc_a_e.o -lgcc --as-needed -lgcc_s --no-as-needed -lc -lgcc --as-needed -lgcc_s --no-as-needed /usr/lib/gcc/x86_64-linux-gnu/5/crtend.o /usr/lib/gcc/x86_64-linux-gnu/5/../../../x86_64-linux-gnu/crtn.o

```

注意：

- crt1.o、crti.o、crtbegin.o、crtend.o、crtn.o是gcc加入的系统标准启动文件，对于一般应用程序，这些启动是必需的。
- -lc：链接libc库文件，其中libc库文件中就实现了printf等函数
- gcc默认使用动态链接库进行链接，生成的程序在执行的时候需要加载所需要的动态库才能运行，动态库体积小，但是必须依赖所需的动态库否则无法执行。
- 添加`-static`选项可以实现，静态库的链接，生成的程序包含所需要的所有库可以直接运行。不过体积较大。操作：` gcc -v -static  -o a_static a.c`.
- `-nostartfiles`:不链接标准启动库文件，而标准库文件仍然可以正常使用。
- `-nostdlib`:不链接系统标准启动文件和标准库文件。

#### 生成汇编指令结果：

**`gcc -S -O3 -o a1.s a.c`**

```
//a1.s

    .file   "a.c"
    .section    .rodata.str1.1,"aMS",@progbits,1
.LC0:
    .string "%d\n"
    .section    .text.unlikely,"ax",@progbits
.LCOLDB1:
    .section    .text.startup,"ax",@progbits
.LHOTB1:
    .p2align 4,,15
    .globl  main
    .type   main, @function
main:
.LFB23:
    .cfi_startproc
    subq    $8, %rsp
    .cfi_def_cfa_offset 16
    xorl    %edx, %edx
    movl    $.LC0, %esi
    movl    $1, %edi
    xorl    %eax, %eax
    call    __printf_chk
    xorl    %eax, %eax
    addq    $8, %rsp
    .cfi_def_cfa_offset 8
    ret
    .cfi_endproc
.LFE23:
    .size   main, .-main
    .section    .text.unlikely
.LCOLDE1:
    .section    .text.startup
.LHOTE1:
    .ident  "GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.11) 5.4.0 20160609"
    .section    .note.GNU-stack,"",@progbits

```

### Intel x86 基本汇编指令

_参考链接：_ [深入浅出GNU X86-64 汇编](https://blog.csdn.net/pro_technician/article/details/78173777);[x86汇编程序基础(AT&T语法)](http://www.cnblogs.com/orlion/p/5765339.html)；[汇编语言入门教程](http://www.ruanyifeng.com/blog/2018/01/assembly-language-primer.html)

在此简单的介绍x86_64基本汇编指令，不做过多介绍。


#### 程序加载时系统的内存分配

程序启动过程：
程序-------> 程序加载器| ----------> Flash memory
                     | ----------> SDRAM 
                     | ----------> BBRAM

**系统区块特性**

|名称|种类|
|:---|:---|
|`SHT_NULL`|无效的区块|
|`SHT_PROGBITS`|带有数据(机械语和初始值等)的区块|
|`SHT_NOBITS`|不带有数据|
|`SHT_RELA`|带有可再分配的数据|
|`SHT_REL`|带有可再分配的数据且不依赖与内存的代码|
|`SHT_SYMTAB`|带有符号表的区块|


|名称|属性|
|:---|:---|
|`SHF_ALLOC` |  应该放在内存上的区块|
|`SHF_WRITE` |  应该放在可读写区域的区块|
|`SHF_EXECINSTR` |  应该放在可执行区域的区块|

归类

|文件 | 种类 | 属性|说明|
|:---|:---|:---|:---|
|`.bbs`  |  `SHT_NOBITS` | `SHF_ALLOC + SHF_WRITE`|主要存放0或者无初始值的全局变量和0或者无初始值的静态局部变量|
|`.data` |  `SHT_PROGBITS` |   `SHF_ALLOC + SHF_WRITE`|主要存放初始值是0以外的全局变量和初始值为0以外的静态局部变量|
|`.text` |  `SHT_PROGBITS`  |  `SHF_ALLOC + SHF_EXECINSTR`|机械语跟代码|
|`.rodata` |`SHT_PROGBITS`   | `SHF_ALLOC` |字符串，或者定数（const）|

linux各个字段含义：
_参考链接：_ [Linux段管理](https://www.cnblogs.com/lcchuguo/p/5105316.html)

|字段名称|意义|
|:---|:---|
|`.bbs/BSS`|BSS是英文Block Started by Symbol的简称。属于静态内存分配;用于存储未初始化的全局变量或者是默认初始化为0的全局变量，它不占用程序文件的大小，可是占用程序执行时的内存空间。|
|`.data/data`|该段用于存储初始化的全局变量，初始化为0的全局变量出于编译优化的策略还是被保存在BSS段|
|`.rodata/rodata`|该段也叫常量区，用于存放常量数据，ro就是Read Only之意。1.些马上数与指令编译在一起直接放在代码段。2.对于字符串常量，编译器会去掉反复的常量，让程序的每一个字符串常量仅仅有一份。3.有些系统中rodata段是多个进程共享的，目的是为了提高空间的利用率。|
|`.text/text`|用于存放程序代码的，编译时确定,仅仅读。更进一步讲是存放处理器的机器指令，当各个源文件单独编译之后生成目标文件。经连接器链接各个目标文件并解决各个源文件之间函数的引用，与此同一时候，还得将全部目标文件里的.text段合在一起，但不是简单的将它们“堆”在一起就完事，还须要处理各个段之间的函数引用问题。|
|`.stack/stack`|是由系统负责申请释放，其操作方式类似stack，用于存储參数变量及局部变量，事实上函数的运行也是stack的方式，所以才有了递归|
|`.heap/heap`|它由用户申请和释放。申请时至少分配虚存，当真正存储数据时才分配对应的实存，释放时也并不是马上释放实存。而是可能被反复利用。|

![64位内存布局](http://img.blog.csdn.net/20141028122011240?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2R4aW4xMzIy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

#### 汇编程序基本元素组成

- **指示（Directives）**：以点号开始，用来指示对编译器，连接器，调试器有用的结构信息。指示本身不是汇编指令。例如：
    + `.file`：只是记录原始源文件名。
    + `.data`：表示数据段(section)的开始地址。
    + `.text`：表示实际程序代码的起始。
    + `.string`：表示数据段中的字符串常量。 
    + `.globl main`：指明标签main是一个可以在其它模块的代码中被访问的全局符号。
    + `.section` :定义内存段,声明段的类型，例如`.section .rodata`
        * `.bbs`
        * `.rodata`
        * `.text`
        * `.stack`
        * `.heap`
- **标签（Labels）**：以冒号结尾，用来把标签名和标签出现的位置关联起来。按照惯例， 以点号开始的标签都是编译器生成的临时局部标签，其它标签则是用户可见的函数和全局变量名称。例如：
    + `.LC0:`：表示紧接着的字符串的名称是`.LC0.`标签。
    + `.cfi_` 开头的汇编指示符用来告诉汇编器生成相应的 DWARF 调试信息([.cfi指令解读](https://blog.csdn.net/jtli_embeddedcv/article/details/9321253);[官方文档](https://source.winehq.org/source/dlls/dbghelp/cpu_x86_64.c#L739))
        * `.cfi_startproc`:定义函数开始
        * `.cfi_endproc`：定义函数结束
        * `.cfi_def_cfa_offset`:定义此处距离CFA(Canonical Frame Address)标准框架地址--**在前一个调用框架中调用当前函数时的栈顶指针**
    + `main:`：表示指令 `pushq %rbp`是main函数的第一个指令。
- **指令（Instructions）**：实际的汇编代码 (pushq %rbp), 一般都会缩进，以便和指示及标签区分开来。

#### CPU寄存器
X86-64中，所有寄存器都是64位，相对32位的x86来说，标识符发生了变化，比如：从原来的%ebp变成了%rbp。为了向后兼容性，%ebp依然可以使用，不过指向了%rbp的低32位。X86-64寄存器的变化，不仅体现在位数上，更加体现在寄存器数量上。新增加寄存器%r8到%r15。加上x86的原有8个，一共16个寄存器。寄存器集成在CPU上，存取速度比存储器快好几个数量级，寄存器多了，GCC就可以更多的使用寄存器，替换之前的存储器堆栈使用，从而大大提升性能。

![x86_64寄存器地址](https://img-blog.csdn.net/20171008191643821)

注意：随着设计的进展，新的指令和寻址模式被添加进来，使得很多寄存器变成了等同的。少数留下来的指令，特别是和字符串处理相关的，要求使用`%rsi` 和`%rdi`。两个寄存器被保留下来分别作为栈指针 (`%rsp`) 和基址指针 (`%rbp`)。最后的8个寄存器是编号的并且没有特殊限制。

多年来，体系结构从8位扩展到16位，32位，因此每个寄存器都有一些内部结构：

![%rax 寄存器内部结构](https://img-blog.csdn.net/20171008192750274)

%rax的低8位是8位寄存器%al,仅靠的8位是%ah。低16位是%ax，低32位是%eax，整个64位是%rax。
寄存器%r8-%r15也有相同结构，但命名方式稍有不同：

![%r8-%r15寄存器内部结构](https://img-blog.csdn.net/20171008193142962)

**注意**：大多数编译器产品混合使用32和64位模式。32位寄存器用来做整数计算，因为大多数程序不需要大于 2^32 的整数值。64位一般用来保存内存地址(指针)，使得可以寻址到16EB虚拟内存。

#### 寻址模式
x86_64使用复杂指令集(CISC)([risc与cisc](https://www.cnblogs.com/createyuan/p/4626235.html))所以MOV指令有很多不同的变种以便在不同的存储单元之间移动不同的数据类型。与大多数指令相同，有着决定移动大多数数据的单字母前缀：

![单字母前缀](https://img-blog.csdn.net/20171008200505370)

不同的数据有着不同的寻址模式和内存偏移:

- 全局值(全局变量和函数)的引用直接使用名字，例如`x`或者`printf`
- 常数使用带有`$`的立即数，例如$56。
- 寄存器值的引用：使用寄存器的名称，例如:`%rbx`
- 间接寻址：使用与寄存器中保存的地址值相对应的内存中的值，例如：(%rsp)表示%rsp指向的内存中的值。
- 相对基址寻址：把一个常数加到寄存器上，例如`-16(%rcx)`表示把`%rcx`指向的地址前移16个字节后对应的内存值。`-16(%rbx,%rcx8)`表示`-16+%rbx+%rcx*8`对应的地址的内存值。

使用各种寻址模式加载一个64位值到%rax:

![添加一个64位值到%rax](https://img-blog.csdn.net/20171008200403991)


>注意：
>注意GNU工具使用传统的AT&T语法。类Unix操作系统上，AT&T语法被用在各种处理器上。Intel语法则一般用在DOS和Windows系统上。下面是AT&T语法的指令： 
>
>movl %esp, %ebp 
>
>movl是指令名称。%则表明esp和ebp是寄存器.在AT&T语法中, 第一个是源操作数,第二个是目的操作数。
在其他地方，例如interl手册，你会看到是没有%的intel语法,它的操作数顺序刚好相反。下面是Intel语法: 
>
>`MOVQ EBP, ESP` 
>
>当在网页上阅读手册的时候，你可以根据是否有%来确定是AT&T 还是 Intel 语法。

#### 基本算术指令
编译器会使用基本的算术指令：`add`、`sub`、`imul`和`idiv`.其中`ADD`和`SUB`有两个操作数：

- `add`:加操作；例：`ADDQ %rbx, %ras`；把%rbx加到%rax，结果存在%rax中，会覆盖之前的值。值的内存大小为8bit
- `sub`:减操作；例如`SUBQ %rbx,%rax`;%rax值存储减去%rbx，结果存储在%rax中。
- `imul`:乘操作；例如`IMULQ %rbx %rax`;它把%rax的值乘以操作数，把结果的低64位存在%rax，高64位放在%rdx(两个64位值相乘的结果是128位)。
- `idiv`:除操作：把128bit值(低64位在 %rax ，高64位在%rdx)除以指令中的操作数为了正确处理负数,用CDQO 指令把%rax符号扩展到%rdx)，商存储在%rax，余数在%rdx。
- `AND`：布尔操作，与
- `OR`:布尔操作，或
- `OR`：布尔操作，非
- `jmp`:跳转指令；跳转到制定地点。
- 跳转比较指令。比较并跳转

![比较跳转指令](https://img-blog.csdn.net/20171013211645843)

```
//%rax从0累加到5:

        MOVQ $0, %rax
loop:
        INCQ %rax
        CMPQ $5, %rax
        JLE  loop

```

- `inc`:目标操作数`+1`;使得目标操作数添加1.
- `dec`:目标操作数`-1`；使得目标操作数减1

#### 栈 statck
栈是一个辅助的数据结构，主要用来记录函数的调用历史和相关的局部变量(没有放到寄存器的)。一般栈是从高地址到低地址向下生长的。`%rsp`是栈指针，指向栈最底部(其实是平常所说的栈顶)元素。所以，`push %rax`（8字节），会把`%rsp`减去8，并把%rax写到 %rsp指向的位置。

- push:


``` 
SUBQ $8, %rsp #%rsp指针指向地址减去8个字节
MOVQ %rax, (%rsp) #将%rax移动到%rsp指向地址

```
- pop:操作与push刚好相反

```
MOVQ (%rsp), %rax #将%rsp指针指向值，移动到%rax
ADDQ $8, %rsp #%rsp指针值减8
```

#### 函数调用
_参考链接：_ [32位汇编语言学习笔记(13)--函数的调用](https://blog.csdn.net/swordmanwk/article/details/41626127);[数据传送指令详解](https://blog.csdn.net/shanyongxu/article/details/47726015)

在大多数汇编程序中(X86-64不是)，调用约定是简单的把每个参数都压栈，然后调用函数。被调用的函数从栈中获取参数，完成操作，把返回值保存到寄存器中并返回。调用方再把参数从栈pop出来(其实X86 32就是这样的)。

X86-64(System V ABI)。 基本方法：

- 整数参数(包含指针）依次放在%rdi, %rsi, %rdx, %rcx, %r8, 和 %r9 寄存器中。
- 浮点参数依次放在寄存器%xmm0-%xmm7中。
- 寄存器不够用时，参数放到栈中。
- 可变参数哈函数(比如printf), 寄存器%eax需记录下浮点参数的个数。
- 被调用的函数可以使用任何寄存器，但它必须保证%rbx, %rbp, %rsp, and %r12-%r15恢复到原来的值（如果它改变了它们的值）。
- 返回值存储在 %eax中.

**基本函数指令**

- `call`: 调用函数指令；例如：`call main`;调用`main`函数 
- `ret`:终止当前函数的执行，将运行权交还给上层函数。当前函数的帧将被回收

![函数调用操作方法](https://img-blog.csdn.net/20171015115531621)

调用函数前，先要把参数放到寄存器中。然后，调用方要把寄存器%r10 和%r11的值保存到栈中。之后，执行CALL指令，把IP指针的值保存到栈中，并跳到函数的起始地址执行。从函数返回后，恢复%r10 和%r11，并从%eax获取返回值。下面进行一个函数举例：

```c++
nt func( int a, int b, int c )
{
        int x, y;
        x = a+b+c;
        y = x*5;
        return y;
}

```

对下面的复杂函数有：

```
.globl func #定于全局函数
func:
    ##################### preamble of function sets up stack
#int a, int b, int c
    pushq %rbp          # save the base pointer
    movq  %rsp, %rbp    # set new base pointer to esp

    pushq %rdi          # save first argument (a) on the stack
    pushq %rsi          # save second argument (b) on the stack
    pushq %rdx          # save third argument (c) on the stack

    subq  $16, %rsp     # allocate two more local variables

    pushq %rbx          # save callee-saved registers
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    ######################## body of function starts here

    movq  -8(%rbp),  %rbx  #a # load each arg into a scratch register
    movq  -16(%rbp), %rcx  #b
    movq  -24(%rbp), %rdx  #c

    addq  %rdx, %rcx   #b=b+c    # add the args together 
    addq  %rcx, %rbx   #a=a+b
    movq  %rbx, -32(%rbp)   # store the result into local 0 (x)

    movq  -32(%rbp), %rbx #a=x  # load local 0 (x) into a scratch register.
    imulq  $5, %rbx   #a*=x     # multiply it by 5
    movl  %rbx, -40(%ebp) #y=a   # store the result in local 1 (y)

    movl  -20(%ebp), %eax   # move local 1 (y) into the result register

    #################### epilogue of function restores the stack

    popq %r15          # restore callee-saved registers
    popq %r14
    popq %r13
    popq %r12
    popq %rbx

    movq %rbp, %rsp    # reset stack to base pointer.
    popq %rbp          # restore the old base pointer

    ret                # return to caller

```

**main 简单代码分析**

```c++
//test2.cpp

long x=0;
long y=10;

int main()
{
    x = y;
    printf("value: %d",y);
    return 0;
}
```
```
    .file   "test2.cpp" #文件名 test2.cpp
    .globl  x   #全局变量 x
    .bss        #静态存储区域
    .align 8    #内存对齐，插入8位，一个字节
    .type   x, @object
    .size   x, 8 #x 8字节
x:
    .zero   8 
    .globl  y #设置全局变量  y
    .data   
    .align 8
    .type   y, @object
    .size   y, 8
y:
    .quad   10
    .section    .rodata
.LC0:
    .string "value: %d"
    .text
    .globl  main
    .type   main, @function
main:
.LFB2:
    .cfi_startproc   #主函数开始
    pushq   %rbp     
    .cfi_def_cfa_offset 16 #
    .cfi_offset 6, -16
    movq    %rsp, %rbp    #设置栈指针
    .cfi_def_cfa_register 6
    movq    y(%rip), %rax #将值10移动到%rax
    movq    %rax, x(%rip) #将%rax移动到栈中，并分配8个字节
    movq    y(%rip), %rax #将y值复制到%rax寄存器
    movq    %rax, %rsi #将参数压入%rsi
    movl    $.LC0, %edi 
    movl    $0, %eax # 记录浮点参数个数
    call    printf #使用printf函数
    movl    $0, %eax # 返回值0
    popq    %rbp  #指针出站
    .cfi_def_cfa 7, 8
    ret
    .cfi_endproc
.LFE2:
    .size   main, .-main
    .ident  "GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.11) 5.4.0 20160609"
    .section    .note.GNU-stack,"",@progbits

```



### 无优化汇编代码分析

有了前面的基础知识，让我们再来看一下最开始的`a.s`

```
    .file   "a.c"  #指明源文件名
    .section    .rodata #定义静态数据，只读
.LC0: #定义标签LC0
    .string "%d\n" #定义string 
    .text #定义代码段
    .globl  main #定义全局标签main
    .type   main, @function #定义标签main 为一个函数
main:
.LFB2: #定义标签LFB2
    .cfi_startproc #定义函数开始
    pushq   %rbp #栈的基底地址
    .cfi_def_cfa_offset 16 #CFA偏移16个字节
    .cfi_offset 6, -16 #寄存器先前值保存在CFA 16字节偏移处
    movq    %rsp, %rbp #栈顶指针指向栈底
    .cfi_def_cfa_register 6 #定义新的标准地址为6
    subq    $16, %rsp #栈指针向下分配16个字节
    movl    $10, -4(%rbp) #将10移动到栈底四个字节处
    movl    -4(%rbp), %eax #将10 复制到%eax-> int a=10
    imull   -4(%rbp), %eax #10*10-> a*=a; a=100；(%eax)=100;
    movl    %eax, -4(%rbp) #-4(%rbp)=100;
    movl    -4(%rbp), %eax # 复制-4(%rbp)的值到%eax;即(%eax)=100;
    subl    %eax, -4(%rbp) # -4(%rbp)=-4(%rbp)减(%eax);即100-100=0；-4(%rbp)=0；
    movl    -4(%rbp), %eax # 复制-4(%rbp)到%eax,即(%eax)=0
    addl    %eax, -4(%rbp) # -4(%rbp)=%eax+(-4(%rbp));即-4(%rbp)=0；
    movl    -4(%rbp), %eax # 将-4(%rbp)复制到%eax;%eax=0;
    movl    %eax, %esi     #%eax复制到%esi,结果存储在%esi中
    movl    $.LC0, %edi   #载入LC0标签，到%edi
    movl    $0, %eax      #将立即数0复制到%eax
    call    printf      #调用打印函数
    movl    $0, %eax   #复制0到%eax;return 0；
    leave #返回main函数
    .cfi_def_cfa 7, 8  #定义新的帧计算规则
    ret #结束当前函数
    .cfi_endproc #函数结束
.LFE2:
    .size   main, .-main
    .ident  "GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.11) 5.4.0 20160609"
    .section    .note.GNU-stack,"",@progbits

```

从上面可以看出式`a+=a-=a*=a;`的执行顺序是：

- `a*=a`:a=100
- `a-=a`:a=0;
- `a+=a`:a=0;

分析开启`-O3`优化后的代码如下：
```
//a1.s

    .file   "a.c"
    .section    .rodata.str1.1,"aMS",@progbits,1 
.LC0:
    .string "%d\n"  #声明 string 放在.text 区域 
    .section    .text.unlikely,"ax",@progbits 
.LCOLDB1:
    .section    .text.startup,"ax",@progbits
.LHOTB1:
    .p2align 4,,15
    .globl  main
    .type   main, @function
main:
.LFB23:
    .cfi_startproc
    subq    $8, %rsp #设置栈顶指针值为8
    .cfi_def_cfa_offset 16 #设置框架偏移
    xorl    %edx, %edx #异或运算为0
    movl    $.LC0, %esi #将字符串参数，添加到%esi寄存器
    movl    $1, %edi #(%edi)寄存器赋值为1
    xorl    %eax, %eax #继续进行异或运算，将结果参数放到eax寄存器
    call    __printf_chk #输出函数
    xorl    %eax, %eax #异或运算为0
    addq    $8, %rsp #将8压入%rsp
    .cfi_def_cfa_offset 8
    ret    #返回
    .cfi_endproc #函数结束
.LFE23:
    .size   main, .-main
    .section    .text.unlikely
.LCOLDE1:
    .section    .text.startup
.LHOTE1:
    .ident  "GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.11) 5.4.0 20160609"
    .section    .note.GNU-stack,"",@progbits

```

通过上面我们可以发现，编译器直接进行了一次亦或运算a=(a!|a)=0;迷之编译优化。

下面是上面计算式的java代码

```java
//a.java

public class a{
      public static void main(String[]agrs)
      {
          int a=10;
          a+=a-=a*=a;
            System.out.println("a value is:"+a);
      }

}

//编译指令：javac a.java

//执行：java a

//结果：a value is:-80

```
可以看出同样的式子，和C++计算结果完全不同
_参考链接：_ [Java反汇编及JVM指令集](https://blog.csdn.net/wangfantastic/article/details/79736860);[查看Java的汇编指令](https://blog.csdn.net/ljheee/article/details/82218156)

指令生成汇编代码` javap -c a.class`

```
Compiled from "a.java"
public class a {
  public a();
    Code:
       0: aload_0 
       1: invokespecial #1                  // Method java/lang/Object."<init>":()V
       4: return

  public static void main(java.lang.String[]);
    Code:
       0: bipush        10 //将10压入栈
       2: istore_1 //将int类型值存入局部变量1
       3: iload_1  //读取局部变量1 注意这里读取了四个局部变量，每个都为10
       4: iload_1   //读取局部变量1
       5: iload_1   //读取局部变量1
       6: iload_1   //读取局部变量1
       7: imul    //执行乘法 10*10=100
       8: dup     //复制一个栈顶内容 100
       9: istore_1 //存入局部变量1 100
      10: isub    //执行减法-> 10-100=-90
      11: dup   //复制栈顶元素 -90
      12: istore_1 //存储元素 -90
      13: iadd //加法： -90+10=-80；注意10一直在栈中没有取出
      14: istore_1 //将-80存入局部变量1
      15: getstatic     #2                  // Field java/lang/System.out:Ljava/io/PrintStream;
      18: new           #3                  // class java/lang/StringBuilder
      21: dup //复制栈顶内容 -80
      22: invokespecial #4                  // Method java/lang/StringBuilder."<init>":()V 
      25: ldc           #5                  // String a value is:
      27: invokevirtual #6                  // Method java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder;
      30: iload_1  //加载局部变量1 -80
      31: invokevirtual #7                  // Method java/lang/StringBuilder.append:(I)Ljava/lang/StringBuilder;
      34: invokevirtual #8                  // Method java/lang/StringBuilder.toString:()Ljava/lang/String;
      37: invokevirtual #9                  // Method java/io/PrintStream.println:(Ljava/lang/String;)V
      40: return
}

```
从上面汇编代码可以看出；Java执行是直接将四个初始化临时的变量，每个给予初始值10；依次进行`*`、`-`、`+`运算；每次的值变化为：100，-90，-80；因此最终结果是-80；所以c++和java的不同，是由于其内部的编译器语法解析方式所决定的。也可以看出，Java真的比较耗性能。

