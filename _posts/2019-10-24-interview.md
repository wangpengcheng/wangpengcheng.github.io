---
layout:     post
title:      武海拾遗录
subtitle:   面试问题与知识点
date:       2019-10-24
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - 面经
    - 考试
---

> 2019-10-24 20:25:49


这篇文章主要是各方面零散问题的日常收集

# C++ 

1. 类指针指向实例化对象；在这个对象程序的运行过程中，程序崩溃，发现是这个类指针的虚函数表被破坏了;如何定位这个问题。
> 2019-10-31 09:53:40
_参考链接：_ [腾讯实习生面试，这两道题目该怎么回答](https://www.zhihu.com/question/43416744/answer/95944740);[虚函数、虚表的生成，虚表的修改](https://blog.csdn.net/alegriabaile/article/details/100169837);[C++虚函数表详细解释及实例分析](https://blog.csdn.net/leo115/article/details/8035078)

可能的情况和答案：

C++可以改变指针指向的虚表，但不能改变虚表里面的内容，即虚函数的地址。virtual table在Linux下GCC4.9的实现就是放在read only段.rodata。因此最大的可能性是存在，内存溢出，其它地方发生了修改。使用gdb和Valgrind进行调试复现.也可能是写内存时越界了，破坏了虚函数表

linux是放在只读区，windows是放在全局静态区，windows下可能出现这种情况。

所以只有可能是虚函数表指针被修改了。因此，对象崩溃的可能原因有下面几个:([原文链接](https://www.zhihu.com/question/43416744/answer/95560471))

1. 访问对象时，对象的构造函数尚未执行完毕。
   
多线程调用时，第一个线程会等待instance的构造函数执行完毕后再返回instance的地址，但由于static的存在，instance的构造函数只会执行一次，如果instance的构造函数尚未执行完毕，有第二个线程使用了GetInstance()函数，此时便会访问一个尚未构造完成的对象。（Ps：据说新的C++标准修复了这个问题，但至少VS2013中这个问题是存在的。）这个错误会造成程序中断，此时直接根据函数调用堆栈定位到出错的指针或对象即可。

2. 访问对象前，某处代码手动执行了对象的析构函数。

在多态的情况下，手动调用对象的析构函数，虽然对象的内存并不会被释放，但成员变量中的指针会变为野指针，此时访问它们自然会造成程序崩溃。此外，执行析构函数时，C++还会做一些额外的工作：当执行完派生类的析构函数后，C++会将对象的虚函数表指针从派生类的虚函数表指向基类的虚函数表，因此，虽然派生类的内存空间还没有被释放，但此时已经无法再访问派生类中定义的虚函数了。
这个错误会造成程序中断，此时直接根据函数调用堆栈定位到出错的指针或对象即可。

3. 内存越界访问

虚函数表指针是由编译器管理的，正常情况下开发者不会访问或修改它的值。因此，若虚函数表指针被破坏了，说明程序中发生了内存越界访问，造成了虚函数表指针被意外修改。这个错误虽然会造成程序中断，但出错的位置往往不是错误发生的位置：A. 如果对象不是通过new创建的此时需要排查出错对象声明处的代码，观察在它附近声明的对象是否发生了内存越界访问。（尤其要关注数组对象，内存越界访问通常是由于数组下标越界导致的）。B. 如果对象是通过new创建的此时需要排查整个程序中所有的指针是否发生了越界访问（仍然优先关注数组）。一种做法是重载new / delete操作符，分配内存时在返回的内存前后各自多分配一个int作为首尾标记，并将它们各自设定为一个特殊的值。每次释放内存时，检查首尾标记的值是否合法。如果不合法，说明该内存指针发生了越界访问。

对于C++岗位来说，把汇编弄弄明白；修修GCC或者LLVM的bug；参加参加GSoC。到时候随便飘点calling convention，vectorization，devirtualization，LTO，搞好气场震慑欺软怕硬的傻逼很重要。虚表更是不在话下，你可以反问面试官“你看你对虚表这么热情啊，你知不知道multiple inheritance虚表怎么实现的？static_cast此种情况又是怎么实现的？virtual inheritance又是怎么实现的？知道multiple dispatch么？怎么在C++里用模拟实现unbounded multiple dispatch或者bounded multiple dispatch（std::experimental::variant<>::visit）？不知道不要紧，我给您讲讲”。其实这些虚表实现细节只要老实巴交地读点wiki page就行了。

作者：Tim Shen
链接：https://www.zhihu.com/question/43416744/answer/95524998
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
# 操作系统



# 多线程与并行编程

## 1. 请问单核机器上写多线程程序，是否需要考虑加锁，为什么？
> 2019-10-24 21:19:48

参考链接:
- [多线程程序在多核和单核上运行的不同](https://blog.csdn.net/ZIV555/article/details/52036841);
- [请问单核机器上写多线程程序](https://blog.csdn.net/N1314N/article/details/93652404);
- [多线程在单核cpu与多核cpu下如何工作](https://blog.csdn.net/qq_28397259/article/details/80810731)

答：在单核机器上写多线程程序，仍然需要线程锁。因为线程锁通常用来实现线程的同步和通信。在单核机器上的多线程程序，仍然存在线程同步的问题。因为在抢占式操作系统中，通常为每个线程分配一个时间片，当某个线程时间片耗尽时，操作系统会将其挂起，然后运行另一个线程。如果这两个线程共享某些数据，不使用线程锁的前提下，可能会导致共享数据修改引起冲突。

在单核上，多个线程执行锁或者临界区时，实际上只有一个线程在执行临界区代码，而核心也只支持一个线程执行，因此不存在冲突。如果某个线程持有锁，那只有其他线程不会被调度到CPU上执行，影响的只是持有和释放锁的时间，处理器时刻在运行着。但是在多核上运行时，锁或临界区会导致其余处理器空闲而只允许一个处理器执行持有锁的那个线程，这是一个串行的过程，会影响性能。

## 2. 在多线程和大量并发环境下，如果有一个平均运行一百万次出现一次的bug， 你如何调试这个bug(PS:这个bug很难重现，这个时候你要怎么处理或者重现呢)
> 2019-10-31 09:53:40

作者：大丶便一箩筐
链接：https://www.zhihu.com/question/43416744/answer/95560471
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

引发bug的可能性有很多，形形色色的debug方法也有很多，它们各有各的优势，并不存在通用的最优解，我目前用过的调试方法有下面几种：

1. 人肉调试：对于某些bug，直接根据程序的异常表现，就可以知道问题代码的具体位置，心里逆推演一下相关代码，就可以找到问题产生的原因。例：刚给客户端加了个多线程模块，F5运行，等了30s。。。咦？客户端界面怎么还没显示出来？任务管理器一看，客户端进程CPU占用为0：八成是刚写的代码死锁了，直接Review代码吧。
2. 中断调试依赖于IDE的调试方法（写C++一般用的都是VS吧），在可能出问题的代码位置打个断点，或者等程序自己出异常中断，或者手动加判断中断。程序中断后，追溯函数调用堆栈，找到产生异常数据的代码。这是最方便的定位bug的方法，但前提是能够在开发环境重现bug。例：策划突然跑过来说：“新做的技能怎么没伤害啊，是不是代码里的伤害计算公式写错了？balabalabala”。。。计算技能伤害的代码位置打个断点，一看数据，有个乘积因子加载以后的数值是0，“卧槽，你自己回去查下技能表是不是漏填了数据。” 
3. Log调试在经常出错、或极有可能出错的代码位置打印log，从而定位问题的原因。如果bug产生的代码没有被log覆盖到，可以通过临时log排查可能导致出错的问题模块。例：测试：“刚发布的测试客户端怎么XX界面打不开啊，程序看下呗！”程序猿：“log文件发过来”。看完log：“界面里有个资源文件找不到，是不是美术没上传到SVN？”
4. Dump调试利用Windows API，在程序运行不正常时中断，将此时的程序的内存镜像输出到一个dump文件里，然后利用WinDbg获取中断时的函数调用堆栈，从而定位出bug的代码，使用的前提是bug不会导致程序闪退，否则无法保存dump文件。例：客服：“刚才有外网玩家反映切换地图的时候程序报错了。”程序猿：“有dump文件吗？”。分析完客服收集的dump文件：“new内存的时候失败了，加个内存池吧。”
5. 工具调试除了上面几种通用的调试方法以外，对于某些特定的问题，可以使用特定的工具进行调试。例：PIX可以用来调试着色器；LeakDiag可以用来调试内存泄漏；Vtune可以用来调试性能。Ps：由于这些工具通常会对程序性能产生比较明显的影响，大型程序（比如游戏引擎）通常会直接在代码层面集成相应的模块，并通过log将结果打印出来。

上面的五种方法基本是按照使用的困难程度升序排列的，对于具体的bug，在可以解决问题的情况下采用难度最低的debug方法才是最优解。而题目没有给出bug的具体表现，所以这是一个开放性问题。
不过根据问题的四个关键词：
- 关键词一、 “多线程”
- 关键词二、 “大量并发”
- 关键词三、 “一百万次出现一次”
- 关键词四、“很难重现”

可以看出面试官为这个bug设定的属性是：
- 很难定位
- 几乎不可能在开发环境中重现

那么基本可以pass掉人肉调试、中断调试和工具调试。所以此时只能通过收集外网环境中log或者dump文件来分析。

Ps：产生bug的原因有很多，问题中并没给出bug的具体表现，根本没有办法判断bug产生的具体原因。题主和部分答主将答题思路往“临界区”与“多线程同步”之类的方向靠，我觉得有点答非所问了，毕竟面试官的问题不是“造成bug的原因”，而是“如何debug”。

1. 在一个几十万行的程序中，O0没有问题，但是O3挂了，然而在某一个地方加入一条printf就过了。
> 2019-10-31 09:53:40

多线程时间同步问题，O0时，编译器没有开启乱序选项，所有都顺序执行，关键信号和锁能在时间上对应。当开启O3时，编译器执行生成CPU乱序指令，造成线程同步不及时；添加printf后造成了时间延迟。同步灭有问题。多线程冲突，因为输出占用的线程时间避免了冲突.

----

## 3 简述多线程(并行)编程的常用设计模式

_参考链接：_ 
- [多线程服务器的常用编程模型](https://blog.csdn.net/SimonxxSun/article/details/90452302)
- [八个常用的并行设计模式](https://blog.csdn.net/evilswords/article/details/37929299)
- [多线程模型](https://blog.csdn.net/qq_26704959/article/details/79281796)
答：

1. 线程模型：一般有两种常用的线程模型：
   1.  生产者-消费者模型：就是由一个线程生产任务，而另外一个线程执行任务，二个线程之间有一个共享数据区，这种数据结构可以用队列来表示，但是必须是并发同步的，也就是就共享数据队列同一时间只能允许一个线程进行访问。这种机制叫做同步访问。
   2.  线程池模型:就是说开始由值守线程创建N个工作线程，并启动它们，它们的状态初始为空闲。然后值守线程到工作队列中取出一个工作任务，同时从线程池中取出一空闲线程来执行此工作任务，执行完该任务后，把该工作线程由运行变为空闲状态，这样不断的从工作队列中取出任务由线程池中的空闲线程进行执行完成。线程池模型不用为每个任务都创建一个线程，只需初始时创建N个线程，然后一直用这N个线程去执行工作队列中的任务，大大的减少了线程的启动，终止的开销。
       1.  工作组模型:多个worker共享一个数据队列，每次取出时，需要检查队列是否为空。不为空才能返回消息。
       2.  C/S模型：只有一个线程处理消息，然后把处理后的消息通过客户端的消息队列同客户端互交
       3.  流水线模型：各个线程之间分别执行，不同的任务；后面节点的阻塞会导致前面节点的阻塞。每个消息节点构成了消息队列的一环
2. 多线程设计模式：
   1. Single Threaded Execution Pattern：同一时刻只允许一个线程操作；在多个线程同时要访问的方法上加上synchronized关键字。
   2. Immutable Pattern：变量赋值一次后只能读取，不能改变；将多线程共享的变量用final关键字修饰。
   3. Guarded Suspension Pattern：判断某个条件是否为真，如果条件成立则继续执行下一步，如果不成立用wait()方法挂起当前线程
   4. Producer-Consumer Pattern：生产者消费者模型。
   5. Read-Write Lock Pattern：直到最后一个线程读完，他才可以释放
   6. Thread-Per-Message Pattern:一任务一线程;
   7. Worker Thread Pattern:同Producer-Consumer Pattern
   8. Future Pattern:调用某个方法时，这个方法可能需要请求其它系统，这个过程比较耗时，为了提高客户的体验需要方法立即返回，过一段时间再查询结果。
   9. Two-Phase Termination Pattern:一个线程在while(!isInterrupted()){..}循环中执行，另外一个线程判断某个条件达到后获得准备被结束线程的句柄，调用interrupt()设置线程的中断状态。
   10. Thread-Specific Storage Pattern：一个方法可能会被同一个线程访问多次，如果每访问一次就要声明一个数据库连接的Connection变量，则对程序的性能有影响。将Connection放在ThreadLocal里，这样每次访问就不必再产生一个Connection,同一个线程对应同一个Connection.
3. 并行设计模式：
   1. Agent and Repository:代理存储模式；创建一个集中管理的数据仓库（data repository），然后定义一组自治的agent来操作这些数据；
   ![模式](http://images.cnblogs.com/cnblogs_com/kaige/201204/201204100015269439.jpg)
   2. Map Reduce:图合并模式：第一步和最后一步串行（主要是为了数据的输入和输出）。中间并行。
   ![Map Reduce](http://images.cnblogs.com/cnblogs_com/kaige/201204/201204100015276309.jpg)
   3. Data Parallelism：数据并行；并行编程常用模式；使用不同线程操作不同的数据。
   ![Data Parallelism](http://images.cnblogs.com/cnblogs_com/kaige/201204/201204100015337044.gif)
   4. Task Parallelism:任务并行：数据独立，线程任务独立；
   ![Task Parallelism](http://images.cnblogs.com/cnblogs_com/kaige/201204/201204100015344502.gif)
   ![Task Parallelism1](http://images.cnblogs.com/cnblogs_com/kaige/201204/201204100015344154.jpg)
   5. Recursive Splitting:为了解决一个大问题，把它分解为可以独立求解的小问题。分解出来的小问题，可能又可以进一步分解为更小的问题。把问题分解到足够小的规模后，就可以直接求解了;比如归并排序
   ![分解问题](http://images.cnblogs.com/cnblogs_com/kaige/201204/20120410001534533.jpg)
   6. Pipeline：流水线模式；使用流水线进行任务的划分和执行。
   ![Pipeline](http://images.cnblogs.com/cnblogs_com/kaige/201204/201204100015362319.jpg)
   7. Geometric decomposition:对于一些线性的数据结构（例如数组），我们可以把数据切分成几个连续的子集。因为这种切分模式看上去和把一块几何区域切分成连续的几块很类似，我们就把它叫做”Geometric decomposition”。例如矩阵乘法可以分成不同的小块进行计算。
   ![Geometric decomposition](http://images.cnblogs.com/cnblogs_com/kaige/201204/201204100015369222.jpg)
   1. Non-work-efficient Parallelism:当处理过程必须依赖于对数据的有序访问时，使用此方法；比如深度优先搜索；查找根节点；我们可以给每一个节点定义一个successor（后继结点），successor的初始值都是其前向节点。然后我们可以同步的更新每一个节点的successor，令其等于“successor的successor”，直到successor的值不再变化为止。进行节点的查找。

## 4. 简述两种web请求处理模式
_参考链接：_ [Reactor模式](https://www.jianshu.com/p/eef7ebe28673)

在处理web请求时，通常有两种体系结构，分别为：thread-based architecture（基于线程）、event-driven architecture（事件驱动）
### 4.1 thread-based architecture
基于线程的体系结构通常会使用多线程来处理客户端的请求，每当接收到一个请求，便开启一个独立的线程来处理。这种方式虽然是直观的，但是仅适用于并发访问量不大的场景，因为线程需要占用一定的内存资源，且操作系统在线程之间的切换也需要一定的开销，当线程数过多时显然会降低web服务器的性能。并且，当线程在处理I/O操作，在等待输入的这段时间线程处于空闲的状态，同样也会造成cpu资源的浪费。一个典型的设计如下：
![线程体系](https://upload-images.jianshu.io/upload_images/10345180-faaebf9335592620.png?imageMogr2/auto-orient/strip|imageView2/2/w/661/format/webp)

### 4.2 event-driven architecture

事件驱动体系结构是目前比较广泛使用的一种。这种方式会定义一系列的事件处理器来响应事件的发生，并且将服务端接受连接与对事件的处理分离。其中，事件是一种状态的改变。比如，tcp中socket的new incoming connection、ready for read、ready for write。

### 4.3 reactor模式

reactor设计模式是event-driven architecture的一种实现方式，处理多个客户端并发的向服务端请求服务的场景。每种服务在服务端可能由多个方法组成。reactor会解耦并发请求的服务并分发给对应的事件处理器来处理。目前，许多流行的开源框架都用到了reactor模式，如：netty、node.js等，包括java的nio。

![车市](https://upload-images.jianshu.io/upload_images/10345180-fdaf4d307916cd8f.png?imageMogr2/auto-orient/strip|imageView2/2/w/640/format/webp)

主要成员角色有：
- Handle：事件的发源地，表明网络链接的状态和事件信号。Linux中一般为网络文件描述符。
- Synchronous Event Demultiplexer:同步事件分离器，本质上是系统调用。比如linux中的select、poll、epoll等。比如，select方法会一直阻塞直到handle上有事件发生时才会返回。
- Event Handler：事件处理器，其会定义一些回调方法或者称为钩子函数，当handle上有事件发生时，回调方法便会执行，一种事件处理机制。
- Concrete Event Handler：具体的事件处理器，实现了Event Handler。在回调方法中会实现具体的业务逻辑。
- Initiation Dispatcher：初始分发器，也是reactor角色，提供了注册、删除与转发event handler的方法。当Synchronous Event Demultiplexer检测到handle上有事件发生时，便会通知initiation dispatcher调用特定的event handler的回调方法。

# 数据库

## 1. Redis为什么用跳表而不用平衡树？
> 2019-11-07 18:50:48
参考链接：[Redis为什么用跳表而不用平衡树？](https://blog.csdn.net/u010412301/article/details/64923131);[Redis为什么用跳表而不用平衡树？](https://www.cnblogs.com/dailidong/p/7571089.html);[redis为何单线程效率还这么高,为何使用跳表不使用B+树做索引(阿里)](https://www.cnblogs.com/aspirant/p/11704530.html)
)

答：

- 在做范围查找的时候，平衡树比skiplist操作要复杂。在平衡树上，我们找到指定范围的小值之后，还需要以中序遍历的顺序继续寻找其它不超过大值的节点。如果不对平衡树进行一定的改造，这里的中序遍历并不容易实现。而在skiplist上进行范围查找就非常简单，只需要在找到小值之后，对第1层链表进行若干步的遍历就可以实现。
- 平衡树的插入和删除操作可能引发子树的调整，逻辑复杂，而skiplist的插入和删除只需要修改相邻节点的指针，操作简单又快速。
- 从内存占用上来说，skiplist比平衡树更灵活一些。一般来说，平衡树每个节点包含2个指针（分别指向左右子树），而skiplist每个节点包含的指针数目平均为1/(1-p)，具体取决于参数p的大小。如果像Redis里的实现一样，取p=1/4，那么平均每个节点包含1.33个指针，比平衡树更有优势。
- 从算法实现难度上来比较，skiplist比平衡树要简单得多。
- B+树的原理是 叶子节点存储数据，非叶子节点存储索引，B+树的每个节点可以存储多个关键字，它将节点大小设置为磁盘页的大小，充分利用了磁盘预读的功能。每次读取磁盘页时就会读取一整个节点,每个叶子节点还有指向前后节点的指针，为的是最大限度的降低磁盘的IO;因为数据在内存中读取耗费的时间是从磁盘的IO读取的百万分之一。**而Redis是 内存中读取数据，不涉及IO，因此使用了跳表**

## 2. 为什么Redis使用单线程而不是多线程
> 2019-11-07 19:11:52
_参考链接：_ [redis为何单线程效率还这么高,为何使用跳表不使用B+树做索引(阿里)](https://www.cnblogs.com/aspirant/p/11704530.html)

答：

- 对于MySQL等数据库来说，数据主要存储在磁盘中，因此I/O情况较多。需要使用多线程来进行上下文切换，等待I/O；提高使用效率。
- Redis是数据内存数据库，不需要进行磁盘I/O因此，没有必要使用多线程来，造成上下文切换的资源浪费。
- 不用去考虑各种锁的问题，不存在加锁释放锁操作，没有因为可能出现死锁而导致的性能消耗

注意：**Redis的性能瓶颈，一般在于机器内存大小和网络带宽。为了尽量提高效率，Redis一般就近部署**

## 3. Mysql事务和并发控制

### 3.1 事务的特性ACID
- 原子性（atomicity）：一个事务必须被视为一个不可分割的最小工作单元。
- 一致性（consistency）：数据库总是从一个一致性状态转换到另一个一致性状态。
- 隔离性（isolation）：一个事务所做的修改在最终提交以前，对其他事务是不可见的。可重复读（REPEATABLE READ）是Mysql的默认事务隔离级别。
- 持久性（durabilty）：一旦事务提交，则其所做的修改就会永久保存到数据库中。

### 3.2 存储引擎InnoDB的MVCC
InnoDB的MVCC采用系统版本号（此处是两个版本号，一个是删除版本号，一个是创建版本号），每开始一个新事务，系统版本号都会自动递增。而当前的快照版本号会和当前的版本号进行比较来操作。

- 查询：
  - InnoDB只查找版本号早于当前事务版本的数据行（行的版本号小于等于事务的系统版本号）。这样就会避免出现脏读。
  - 行的删除版本号要么未定义，要么大于当前事务版本号（保证事务读取的行，在读之前未被删除），这样就不会出现脏读。
- 插入：
  - InnoDB为新插入的每一行保存当前系统版本号作为行版本号。
- 删除：
  - InnoDB为删除的每一行保存当前系统版本号作为删除标识。
- 更新：
  - InnoDB为插入一行新纪录，保存当前系统版本号作为行版本号，同时保存当前系统版本号到原来的行作为行删除标识符。

### 3.3 Mysql并发控制
- 读写锁:并发控制方式多是采用锁机制。通常是共享锁（读锁）和排他锁（写锁）。多个客户在同一个时刻可以同时读取同一个资源，互不干扰。写锁是排他的，也就是说在写的同时不允许其他的写锁来写和读锁来读。这样就能防止脏数据的进入。
- 锁粒度:提高资源的并发性关键在于锁粒度。在给定的资源上，锁定的数据量越少，则系统的并发程度越高，只要相互不冲突即可。但是一味的细化锁，让锁的数量上升也会增加系统对资源的消耗。所以真正的锁策略是在锁的开销和数据的安全性之间寻求一种平衡。
  - 表锁：表锁是Mysql中开销最小的锁。它的策略是会锁定一张表，如果用户对表写入（插入，更新，删除），就会将表锁住，同时阻塞其他的用户对这张表的读和写。在某些场景下，写锁比读锁更高的优先级。写锁请求会插入到读锁队列的前面
  - 行锁：行锁是最大程度支持并发处理（锁开销最大）。像默认的存储引擎InnoDB等实现了行锁。行锁只在存储引擎层实现。

### 3.4 隔离度
- **read uncommitted(读未提交)**：能够读未提交的数据；回滚造成脏读
- **read committed(读已提交)**：不会读取还未提交的数据，但可读取事务进行之前的数据以及事务完成之后的数据--解决了脏读，造成不可重复读。
- **repeatable read(可重复读)**：不能读取还未提交的数据，也就是类似于对读的那一行加锁（行级锁），不允许读。保障重复读，但插入新行会导致幻读(select count(*))。
- **serializable(可串行化)**：为表加锁（表级锁）。

## 4 简述第一～三范式
_参考链接：_ [第一范式，第二范式，第三范式，BCNF范式理解](https://blog.csdn.net/u013164931/article/details/79692402)

- 第一范式：列的属性均为原子属性，列不可再分
- 第二范式：第二范式建立在第一范式的基础上，非主属性完全依赖于码(可能包含多个属性)--消除部分依赖。
- 第三范式：满足第二范式的条件下不存在传递函数依赖。各个属性之间完全独立，不存除主键外的在部分依赖的情况。
- BCNF范式：BCNF是3NF的改进形式；满足条件如下:
  1. 所有非主属性对每一个码都是完全函数依赖。
  2. 所有的主属性对每一个不包含它的码,也是完全函数依赖。
  3. 没有任何属性完全函数依赖于非码的任何一组属性。



# 网络编程
## 1. 简述TCP的三次握手
_参考链接：_ [面试官，不要再问我三次握手和四次挥手](https://zhuanlan.zhihu.com/p/85557012)

TCP数据报中的关键标志位
在TCP层，有个FLAGS字段，这个字段有以下几个标识：SYN, FIN, ACK, PSH, RST, URG.

其中，对于我们日常的分析有用的就是前面的五个字段。

它们的含义是：

- SYN表示建立连接，

- FIN表示关闭连接，

- ACK表示响应，

- PSH表示有 DATA数据传输，

- RST表示连接重置。

其中，ACK是可能与SYN，FIN等同时使用的，比如SYN和ACK可能同时为1，它表示的就是建立连接之后的响应，

如果只是单个的一个SYN，它表示的只是建立连接。

TCP的几次握手就是通过这样的ACK表现出来的。

但SYN与FIN是不会同时为1的，因为前者表示的是建立连接，而后者表示的是断开连接。

RST一般是在FIN之后才会出现为1的情况，表示的是连接重置。

一般地，当出现FIN包或RST包时，我们便认为客户端与服务器端断开了连接；而当出现SYN和SYN＋ACK包时，我们认为客户端与服务器建立了一个连接。

PSH为1的情况，一般只出现在 DATA内容不为0的包中，也就是说PSH为1表示的是有真正的TCP数据包内容被传递。

TCP的连接建立和连接关闭，都是通过请求－响应的模式完成的。

概念补充-TCP三次握手：

TCP(Transmission Control Protocol)传输控制协议

TCP是主机对主机层的传输控制协议，提供可靠的连接服务，采用三次握手确认建立一个连接：

位码即tcp标志位，有6种标示：
- SYN(synchronous建立联机)
- ACK(acknowledgement 确认)
- PSH(push传送)
- FIN(finish结束)
- RST(reset重置) 
- URG(urgent紧急)
- Sequence number(顺序号码) 
- Acknowledge number(确认号码)

第一次握手：主机A发送位码为syn＝1，随机产生seq number=1234567的数据包到服务器，主机B由SYN=1知道，A要求建立联机；

第二次握手：主机B收到请求后要确认联机信息，向A发送ack number=(主机A的seq+1)，syn=1，ack=1，随机产生seq=7654321的包；

第三次握手：主机A收到后检查ack number是否正确，即第一次发送的seq number+1，以及位码ack是否为1，若正确，主机A会再发送ack number=(主机B的seq+1)，ack=1，主机B收到后确认seq值与ack=1则连接建立成功。

完成三次握手，主机A与主机B开始传送数据。


在TCP/IP协议中，TCP协议提供可靠的连接服务，采用三次握手建立一个连接。
第一次握手：建立连接时，客户端发送syn包(syn=j)到服务器，并进入SYN_SEND状态，等待服务器确认；
第二次握手：服务器收到syn包，必须确认客户的SYN（ack=j+1），同时自己也发送一个SYN包（syn=k），即SYN+ACK包，此时服务器进入SYN_RECV状态；
第三次握手：客户端收到服务器的SYN＋ACK包，向服务器发送确认包ACK(ack=k+1)，此包发送完毕，客户端和服务器进入ESTABLISHED状态，完成三次握手。完成三次握手，客户端与服务器开始传送数据.

## 2. TCP最大连接数限制：

> 2019-12-14 20:53

参考链接：
- [Linux下高并发socket最大连接数所受的各种限制](https://blog.csdn.net/slqgenius/article/details/88045033)
- [Linux下高并发socket最大连接数所受的各种限制](https://www.cnblogs.com/wanghuaijun/p/7214319.html)
- [4G内存服务器epoll并发量最大能达到多少](https://blog.csdn.net/libaineu2004/article/details/72822479)
- [linux百万并发之 tcp_mem](https://www.cnblogs.com/sky-cheng/p/10570376.html)

### 2.1 首先对于服务器端：
理想数量：server端tcp连接4元组中只有remote ip（也就是client ip）和remote port（客户端port）是可变的，因此最大tcp连接为客户端ip数×客户端port数，对IPV4，不考虑ip地址分类等因素，
最大tcp连接数约为2的32次方（ip数）×2的16次方（port数），也就是server端单机最大tcp连接数约为2的48次方。大概281万亿

select和poll是根据四元组创建文件句柄。因此受到：
1. Linux的最大文件数目的限制；一般为1024上限；可以通过修改/etc/security/limits.conf更改最高文件描述符上限。**注意：poll使用链表，因此无此1024上限**
2. Linux系统文件数硬限制，一般为791795；可以通过对/proc/sys/fs/file-max进行查看。更改`/etc/sysctl.conf`内容如下进行修改([参考链接](https://www.cnblogs.com/wanghuaijun/p/7214319.html))：
```sh
#该参数设置系统的TIME_WAIT的数量，如果超过默认值则会被立即清除
net.ipv4.tcp_max_tw_buckets = 20000
#能够更快地回收TIME-WAIT套接字。此选项会导致处于NAT网络的客户端超时，建议为0
net.ipv4.tcp_tw_recycle = 0
#系统所有进程一共可以打开的文件数量
fs.file-max = 6815744
#防火墙跟踪表的大小。注意：如果防火墙没开则会提示error: "net.netfilter.nf_conntrack_max" is an unknown key，忽略即可
net.netfilter.nf_conntrack_max = 2621440
net.ipv4.ip_local_port_range = 1024 65536
net.core.rmem_max=16777216
net.core.wmem_max=16777216
# TCP读写页的大小；按照顺序是low,pressure,high
#low：当TCP使用了低于该值的内存页面数时，TCP不会考虑释放内存。
#pressure：当TCP使用了超过该值的内存页面数量时，TCP试图稳定其内存使用，进入pressure模式，当内存消耗低于low值时则退出pressure状态。
#high：允许所有tcp sockets用于排队缓冲数据报的页面量，当内存占用超过此值，系统拒绝分配socket，后台日志输出“TCP: too many of orphaned sockets”。
# 这里分别设置为最大值设置为3G 8G 16G
net.ipv4.tcp_rmem=4096 87380 16777216
net.ipv4.tcp_wmem=4096 65536 16777216
net.ipv4.tcp_fin_timeout = 10
net.ipv4.tcp_tw_recycle = 1
net.ipv4.tcp_timestamps = 0
net.ipv4.tcp_window_scaling = 0
net.ipv4.tcp_sack = 0
#在每个网络接口接收数据包的速率比内核处理这些包的速率快时，允许送到队列的数据包的最大数目
net.core.netdev_max_backlog = 30000
net.ipv4.tcp_no_metrics_save=1
# 定义了系统中每一个端口最大的监听队列的长度，这是个全局的参数
net.core.somaxconn = 262144
net.ipv4.tcp_syncookies = 0
net.ipv4.tcp_max_orphans = 262144
# 对于还未获得对方确认的连接请求，可保存在队列中的最大数目
net.ipv4.tcp_max_syn_backlog = 262144
net.ipv4.tcp_synack_retries = 2
net.ipv4.tcp_syn_retries = 2
```
3. Linux内核的tcp_mem限制：
4. 内存限制：网络包的大小都可以控制在4K以下4+4+4+4=16K，4G内存理想情况旋可以实现**4G/16K=26.2万并发**；可以控制在8K以下，一个socket的内存占用介于 24K ~ 32K之间, 保守的按照32K算4G/32K=13.1万并发；把socket buffer size设置系统最低。那么是4G/8K = 52.4万并发 这个应该是极限值了。
### 2.2 对于客户端：
1. 除了服务器主要限制外，主要是端口号的限制，系统给我们的可用端口的范围是32768-60999；大概是28232.可以通过`sysctl -a | grep port`进行查看,通过`/etc/sysctl.conf`修改:`net.ipv4.ip_local_port_range = 1024 65000`。

### 2.3 使用支持高并发网络I/O的编程技术
过多的线程又会因系统对线程的调度造成巨大开销。因此，在高TCP并发的情形下使用同步 I/O是不可取的，这时可以考虑使用非阻塞式同步I/O或异步I/O。非阻塞式同步I/O的技术包括使用select()，poll()，epoll等机制。异步I/O的技术就是使用AIO。

### 2.4 [Linux内核TCP/IP、Socket参数调优](https://www.cnblogs.com/zengkefu/p/5749009.html)

socket的结构如下图：

![socket结构](https://images2015.cnblogs.com/blog/99941/201608/99941-20160808133949949-1115097165.png)

### 2.5 UDP的最大缓冲值
参考链接：[获取linux下，tcp、udp的系统默认缓存大小和最大值](https://blog.csdn.net/qiuchang008/article/details/52629005);[关于linux udp收发包缓冲区大小](https://blog.csdn.net/cjsycyl/article/details/8668464)

## 3. 关于linux的tcp/udp缓存
> 2019-12-14 21:44:53

参考链接:[关于linux的tcp/udp缓存](https://blog.csdn.net/sphone89/article/details/16116025)

通过`cat /proc/sys/net/core/udp_*`查看个更改UDP的限制。
一般tcp 或 udp 接收缓冲区最大可设置为`/proc/sys/net/core/rmem_max`值的一半。

## 4.HTTP的状态码
> 2019-12-15 10:50:53

_参考链接：_ [常见的HTTP状态码](https://www.cnblogs.com/xflonga/p/9368993.html);[HTTP状态码](https://tool.lu/httpcode/)

### 4.1三至七种最基本的响应代码
- 200("OK")：一切正常。实体主体中的文档（若存在的话）是某资源的表示。
- 301("Moved Permanently")：当客户端触发的动作引起了资源URI的变化时发送此响应代码。另外，当客户端向一个资源的旧URI发送请求时，也发送此响应代码。
- 400("Bad Request")：客户端方面的问题。实体主题中的文档（若存在的话）是一个错误消息。希望客户端能够理解此错误消息，并改正问题。
- 409("Conflict")：当客户端试图执行一个”会导致一个或多个资源处于不一致状态“的操作时，发送此响应代码。
- 404("Not Found") 和410("Gone")：当客户端所请求的URI不对应于任何资源时，发送此响应代码。404用于服务器端不知道客户端要请求哪个资源的情况；
- 410：用于服务器端知道客户端所请求的资源曾经存在，但现在已经不存在了的情况。
- 500("Internal Server Error")：服务期方面的问题。实体主体中的文档（如果存在的话）是一个错误消息。该错误消息通常无济于事，因为客户端无法修复服务器方面的问题。
1xx：指示信息，表示请求已接收，继续处理

### 4.2 详细状态码解析

<table class="tbl" width="100%" cellspacing="0" cellpadding="0">
            <thead>
            <tr>
                <th width="7%">状态码</th>
                <th width="14%">原因短语</th>
                <th width="66%">代表含义</th>
                <th width="13%">HTTP 版本</th>
            </tr>
            </thead>
            <tbody>
            <tr>
                <th colspan="4">消息响应</th>
            </tr>
            <tr>
                <td id="100">100</td>
                <td>Continue<br> (继续)</td>
                <td>
                    客户端应当继续发送请求.这个临时响应是用来通知客户端它的部分请求已经被服务器接收,且仍未被拒绝.客户端应当继续发送请求的剩余部分,或者如果请求已经完成,忽略这个响应.服务器必须在请求完成后向客户端发送一个最终响应.
                </td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <td id="101">101</td>
                <td>Switching Protocol<br> (切换协议)</td>
                <td>服务器已经理解了客户端的请求，并将通过Upgrade消息头通知客户端采用不同的协议来完成这个请求。在发送完这个响应最后的空行后，服务器将会切换到 在Upgrade消息头中定义的那些协议。:
                    只有在切换新的协议更有好处的时候才应该采取类似措施。例如，切换到新的HTTP版本比旧版本更有优势，或者切换到一个实时且同步的协议以传送利用此类特 性的资源。
                </td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <th colspan="4">成功响应</th>
            </tr>
            <tr>
                <td id="200">200</td>
                <td>OK<br> (成功)</td>
                <td>请求成功.成功的意义根据请求所使用的方法不同而不同.
                    <ul>
                        <li>GET: 资源已被提取,并作为响应体传回客户端.</li>
                        <li>HEAD: <span><span>实体</span><span>头</span><span>已作为响应头传回客户端</span></span></li>
                        <li>POST: 经过服务器处理客户端传来的数据,适合的资源作为响应体传回客户端.</li>
                        <li>TRACE: <span><span>服务器</span><span>收到</span><span>请求消息</span></span>作为响应体传回客户端.</li>
                    </ul>
                </td>
                <td>HTTP/0.9 可用</td>
            </tr>
            <tr>
                <td id="201">201</td>
                <td>Created<br> (已创建)</td>
                <td>请求成功，而且有一个新的资源已经依据请求的需要而建立，通常这是 PUT 方法得到的响应码.</td>
                <td>HTTP/0.9 可用</td>
            </tr>
            <tr>
                <td id="202">202</td>
                <td>Accepted<br> (已创建)</td>
                <td>
                    服务器已接受请求，但尚未处理。正如它可能被拒绝一样，最终该请求可能会也可能不会被执行。在异步操作的场合下，没有比发送这个状态码更方便的做法了。:返回202状态码的响应的目的是允许服务器接受其他过程的请求（例如某个每天只执行一次的基于批处理的操作），而不必让客户端一直保持与服务器的连接直到批处理操作全部完成。在接受请求处理并返回202状态码的响应应当在返回的实体中包含一些指示处理当前状态的信息，以及指向处理状态监视器或状态预测的指针，以便用户能够估计操作是否已经完成。
                </td>
                <td>HTTP/0.9 可用</td>
            </tr>
            <tr>
                <td id="203">203</td>
                <td>Non-Authoritative Information<br> (未授权信息)</td>
                <td><p>服务器已成功处理了请求,但返回的实体头部元信息不是在原始服务器上有效的确定集合，而是来自本地或者第三方的拷贝,如果不是上述情况,使用200状态码才是最合适的.</p></td>
                <td>HTTP/0.9 and 1.1</td>
            </tr>
            <tr>
                <td id="204">204</td>
                <td>No Content<br> (无内容)</td>
                <td>该响应没有响应内容,只有响应头,响应头也可能是有用的.用户代理可以根据新的响应头来更新对应资源的缓存信息.</td>
                <td>HTTP/0.9 可用</td>
            </tr>
            <tr>
                <td id="205">205</td>
                <td>Reset Content<br> (重置内容)</td>
                <td>告诉用户代理去重置发送该请求的窗口的文档视图.</td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <td id="206">206</td>
                <td>Partial Content<br> (部分内容)</td>
                <td>当客户端通过使用range头字段进行文件分段下载时使用该状态码</td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <th colspan="4">重定向</th>
            </tr>
            <tr>
                <td id="300">300</td>
                <td>Multiple Choice<br> (多种选择)</td>
                <td>该请求有多种可能的响应,用户代理或者用户必须选择它们其中的一个.服务器没有任何标准可以遵循去代替用户来进行选择.</td>
                <td>HTTP/1.0 and later</td>
            </tr>
            <tr>
                <td id="301">301</td>
                <td>Moved Permanently<br> (永久移动)</td>
                <td>该状态码表示所请求的URI资源路径已经改变,新的URL会在响应的<code>Location</code>:头字段里找到.</td>
                <td>HTTP/0.9 可用</td>
            </tr>
            <tr>
                <td id="302">302</td>
                <td>Found<br> (临时移动)</td>
                <td>该状态码表示所请求的URI资源路径临时改变,并且还可能继续改变.因此客户端在以后访问时还得继续使用该URI.新的URL会在响应的<code>Location:</code>头字段里找到.</td>
                <td>HTTP/0.9 可用</td>
            </tr>
            <tr>
                <td id="303">303</td>
                <td>See Other<br> (查看其他位置)</td>
                <td>服务器发送该响应用来引导客户端使用GET方法访问另外一个URI.</td>
                <td>HTTP/0.9 and 1.1</td>
            </tr>
            <tr>
                <td id="304">304</td>
                <td>Not Modified<br> (未修改)</td>
                <td>告诉客户端,所请求的内容距离上次访问并没有变化. 客户端可以直接从浏览器缓存里获取该资源.</td>
                <td>HTTP/0.9 可用</td>
            </tr>
            <tr>
                <td id="305">305</td>
                <td>Use Proxy<br> (使用代理)</td>
                <td>所请求的资源必须统过代理才能访问到.由于安全原因,该状态码并未受到广泛支持.</td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <td id="306">306</td>
                <td><em>unused</em><br> (未使用)</td>
                <td>
                    <span><span>这个</span><span>状态码</span><span>已经不再被使用</span><span>,</span><span>当初它被用</span><span>在</span><span>HTTP 1.1规范</span><span>的</span><span>旧版本</span><span>中.</span></span>
                </td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <td id="307">307</td>
                <td>Temporary Redirect<br> (临时重定向)</td>
                <td><p>服务器发送该响应用来引导客户端使用相同的方法访问另外一个URI来获取想要获取的资源.新的URL会在响应的<code>Location:</code>头字段里找到.与302状态码有相同的语义,且前后两次访问必须使用相同的方法(GET
                        POST).</p></td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <td>308</td>
                <td>Permanent Redirect<br> (永久重定向)</td>
                <td><p>所请求的资源将永久的位于另外一个URI上.新的URL会在响应的<code>Location:</code>头字段里找到.与301状态码有相同的语义,且前后两次访问必须使用相同的方法(GET
                        POST).</p></td>
                <td><p>HTTPbis <br> (试验草案)</p></td>
            </tr>
            <tr>
                <th colspan="4">客户端错误</th>
            </tr>
            <tr>
                <td id="400">400</td>
                <td>Bad Request<br> (错误请求)</td>
                <td>因发送的请求语法错误,服务器无法正常读取.</td>
                <td>HTTP/0.9 可用</td>
            </tr>
            <tr>
                <td id="401">401</td>
                <td>Unauthorized<br> (未授权)</td>
                <td>需要身份验证后才能获取所请求的内容,类似于403错误.不同点是.401错误后,只要正确输入帐号密码,验证即可通过.</td>
                <td>HTTP/0.9 可用</td>
            </tr>
            <tr>
                <td id="402">402</td>
                <td>Payment Required<br> (需要付款)</td>
                <td><span><span>该状态</span><span>码</span><span>被保留以</span><span>供将来使用.</span><span>创建此代码</span><span>最初的目的是</span><span>为</span><span>数字</span><span>支付系统而用</span><span>,</span><span>然而,到现在也没投入使用</span><span>.</span></span>
                </td>
                <td>HTTP/0.9 and 1.1</td>
            </tr>
            <tr>
                <td id="403">403</td>
                <td>Forbidden<br> (禁止访问)</td>
                <td>客户端没有权利访问所请求内容,服务器拒绝本次请求.</td>
                <td>HTTP/0.9 可用</td>
            </tr>
            <tr>
                <td id="404">404</td>
                <td>Not Found<br> (未找到)</td>
                <td>服务器找不到所请求的资源.由于经常发生此种情况,所以该状态码在上网时是非常常见的.</td>
                <td>HTTP/0.9 可用</td>
            </tr>
            <tr>
                <td id="405">405</td>
                <td>Method Not Allowed<br> (不允许使用该方法)</td>
                <td>该请求使用的方法被服务器端禁止使用,RFC2616中规定, <code>GET</code> 和 <code>HEAD</code> 方法不能被禁止.</td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <td id="406">406</td>
                <td>Not Acceptable<br> (无法接受)</td>
                <td>在进行<span>服务器驱动内容协商</span>后,没有发现合适的内容传回给客户端.</td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <td id="407">407</td>
                <td>Proxy Authentication Required<br> (要求代理身份验证)</td>
                <td><p>类似于状态码 401,不过需要通过代理才能进行验证.</p></td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <td id="408">408</td>
                <td>Request Timeout<br> (请求超时)</td>
                <td>客户端没有在服务器预备等待的时间内完成一个请求的发送.这意味着服务器将会切断和客户端的连接. 在其他浏览器中,这种响应更常见一些, 例如Chrome 和 IE9, 目的是为了使用HTTP
                    预连机制<span><span>加快浏览速度</span></span>. 同时注意,一些服务器不发送此种响应就直接切断连接.
                </td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <td id="409">409</td>
                <td>Conflict<br> (冲突)</td>
                <td>该请求与服务器的当前状态所冲突.</td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <td id="410">410</td>
                <td>Gone<br> (已失效)</td>
                <td>所请求的资源已经被删除.</td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <td id="411">411</td>
                <td>Length Required<br> (需要内容长度头)</td>
                <td>因服务器在本次请求中需要 <code>Content-Length</code> 头字段,而客户端没有发送.所以,服务器拒绝了该请求.</td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <td id="412">412</td>
                <td>Precondition Failed<br> (预处理失败)</td>
                <td>服务器没能满足客户端在获取资源时在请求头字段中设置的先决条件.</td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <td id="413">413</td>
                <td>Request Entity Too Large<br> (请求实体过长)</td>
                <td>请求实体大小超过服务器的设置的最大限制,服务器可能会关闭HTTP链接并返回<code>Retry-After</code> 头字段.</td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <td id="414">414</td>
                <td>Request-URI Too Long<br> (请求网址过长)</td>
                <td>客户端请求所包含的URI地址太长,以至于服务器无法处理.</td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <td id="415">415</td>
                <td>Unsupported Media Type<br> (媒体类型不支持)</td>
                <td>服务器不支持客户端所请求的媒体类型,因此拒绝该请求.</td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <td id="416">416</td>
                <td>Requested Range Not Satisfiable<br> (请求范围不合要求)</td>
                <td>请求中包含的<code>Range</code>头字段无法被满足,通常是因为<code>Range</code>中的数字范围超出所请求资源的大小.</td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <td id="417">417</td>
                <td>Expectation Failed<br> (预期结果失败)</td>
                <td>在请求头<code> Expect</code> 中指定的预期内容无法被服务器满足.</td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <th colspan="4">服务器端错误</th>
            </tr>
            <tr>
                <td id="500">500</td>
                <td>Internal Server Error<br> (内部服务器错误)</td>
                <td>服务器遇到未知的无法解决的问题.</td>
                <td>HTTP/0.9 可用</td>
            </tr>
            <tr>
                <td id="501">501</td>
                <td>Implemented<br> (未实现)</td>
                <td>服务器不支持该请求中使用的方法,比如<code>POST</code> 和 <code>PUT.只有</code><code>GET</code> 和 <code>HEAD</code>
                    是RFC2616规范中规定服务器必须实现的方法.
                </td>
                <td>HTTP/0.9 可用</td>
            </tr>
            <tr>
                <td id="502">502</td>
                <td>Bad Gateway<br> (网关错误)</td>
                <td>服务器作为网关且从上游<span>服务器获取到了一个无效的HTTP响应</span>.</td>
                <td>HTTP/0.9 可用</td>
            </tr>
            <tr>
                <td id="503">503</td>
                <td>Service Unavailable<br> (服务不可用)</td>
                <td>由于临时的服务器维护或者过载,服务器当前无法处理请求.这个状况是临时的,并且将在一段时间以后恢复.如果能够预计延迟时间,那么响应中可以包含一个<code>Retry-After:</code>头用以标明这个延迟时间.如果没有给出这个<code>Retry-After:</code>信息，那么客户端应当以处理500响应的方式处理它.同时,这种情况下,一个友好的用于解释服务器出现问题的页面应当被返回,并且,缓存相关的HTTP头信息也应该包含,因为通常这种错误提示网页不应当被客户端缓存.
                </td>
                <td>HTTP/0.9 可用</td>
            </tr>
            <tr>
                <td id="504">504</td>
                <td>Gateway Timeout <br> (网关超时)</td>
                <td>服务器作为网关且不能从上游<span>服务器</span>及时的得到响应返回给客户端.</td>
                <td>HTTP/1.1 可用</td>
            </tr>
            <tr>
                <td id="505">505</td>
                <td>HTTP Version Not Supported<br> (HTTP版本不受支持)</td>
                <td>服务器不支持客户端发送的HTTP请求中所使用的HTTP协议版本.</td>
                <td>HTTP/1.1 可用</td>
            </tr>
            </tbody>
        </table>

# 数据结构与算法

# 1. 无锁队列
> 2019-12-014 22:04 
_参考链接:_ 
- [C++原子性实现无锁队列](https://blog.csdn.net/mijichui2153/article/details/81149309)
- [并发无锁队列](https://www.cnblogs.com/alantu2018/p/8469168.html)


# 2. 简述B树，B+树，红黑树的结构和插入删除操作

