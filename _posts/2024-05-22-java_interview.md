---
layout:     post
title:      java面经
subtitle:   Java面经学习
date:       2024-05-22
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - Java
    - 面试
    - 学习笔记
---

# Java常见面经验

## 参考链接

- [JavaGuide](https://javaguide.cn/)
- [二哥的JVM进阶之路](https://javabetter.cn/jvm/)
- [深入java虚拟机](https://github.com/heibaiying/Full-Stack-Notes/blob/master/notes/Java_%E8%99%9A%E6%8B%9F%E6%9C%BA.md)
- [Minor GC、Young GC、Full GC、Old GC、Major GC、Mixed GC 一文搞懂](https://juejin.cn/post/7085174576950280200)


## 1. 包装类型的缓存机制了解么？

Java 基本数据类型的包装类型的大部分都用到了缓存机制来提升性能。Byte,Short,Integer,Long 这 4 种包装类默认创建了数值 [-128，127] 的相应类型的缓存数据，Character 创建了数值在 [0,127] 范围的缓存数据，Boolean 直接返回 True or False

```java
Integer i1 = 40;
Integer i2 = new Integer(40);
System.out.println(i1==i2); // false
```

Integer i1=40 这一行代码会发生装箱，也就是说这行代码等价于 Integer i1=Integer.valueOf(40) 。因此，i1 直接使用的是缓存中的对象。而Integer i2 = new Integer(40) 会直接创建新的对象。

因此，答案是 false 。你答对了吗？

记住：**所有整型包装类对象之间值的比较，全部使用 equals 方法比较。**

## 2. 如何解决浮点数运算的精度丢失问题？

BigDecimal 可以实现对浮点数的运算，不会造成精度丢失。通常情况下，大部分需要浮点数精确运算结果的业务场景（比如涉及到钱的场景）都是通过 BigDecimal 来做的。

```java
BigDecimal a = new BigDecimal("1.0");
BigDecimal b = new BigDecimal("0.9");
BigDecimal c = new BigDecimal("0.8");

BigDecimal x = a.subtract(b);
BigDecimal y = b.subtract(c);

System.out.println(x); /* 0.1 */
System.out.println(y); /* 0.1 */
System.out.println(Objects.equals(x, y)); /* true */

```

## 3. JVM GC 讲一下
_参考链接：_

- [深入理解垃圾回收机制](https://javabetter.cn/jvm/gc.html#survivor-%E5%8C%BA)
- [垃圾收集器详解](https://javabetter.cn/jvm/gc-collector.html#g1)

JVM的核心还是在于内存池和线程池


要了解JVM GC 关键在于以下三个点：

1. JVM 中内存分步模型--内存管理是什么样子的
2. JVM 中内存GC方法(内存管理模型)--怎样去回收的
3. JVM 中的GC时机()--什么时候去回收

就从上面三个方向进行分析

### 3.1 JVM 中的内存分布

JVM 中的主要内存分步如下所示：

![JVM内存分布](https://pdai.tech/images/jvm/java-jvm-overview.png)

关键内存如下：
- 线程内内存(线程独占)：
    - 本地方法栈：调用外部动态链接库的对应方法栈
    - 程序计数器：JVM 代码解释中的程序计数器
    - JVM 栈：JVM代码本身的堆栈
- 线程共享内存：
    - 方法区：.class信息，编译代码
    - 堆区域(常量池静&态变量)
    - 新生区：大部分堆内存分配区域。回收频繁
        - Eden 区：新生对象初始分配区域，空间不足时会发起一次 Minor GC，回收大多数对象，剩余存活的移动到 From Survivor
        - Survivor: Eden区和old区中间的缓冲区(黄灯)，实现预筛选，精细化管理，避免老年区被快速填满。经历16次  Minor GC 还能在新生代中存活的对象，才会被送到老年区。同时为了解决内存碎片化，使用swap清理的方式，划分为了两块内存，方便快速进行Survivor 清理。避免内存碎片化。
            - From Survivor: 初次移动过来的区域，清除时，直接域To Survivor 复制交换
            - To Survivor: 每次GC时，将写入指针指向To, 将Eden、From中的存活对象复制到To区域，并交换两个区域。清空From
    - 老年区：占据2/3的堆内存空间，只有Major GC的时候才会清理。每次清理时都会触发“STW(时停)”。大对象、长期对象(MaxTenuringThreshold)、动态年龄对象(超过Survivor一半)
- [直接内存(元数据区)](https://luoyoubao.gitbooks.io/jvm/content/chapter1/zhi-jie-nei-cun.html)：mmap 直接内存映射区域，用于IO交互


### 3.2 JVM 中的垃圾收集器
前面讲述了JVM中的内存分步，下面就是关键的堆内存回收了，即java中的垃圾收集器。JAVA中的垃圾收集器使用了染色标记算法，进行对象染色回收。

就目前来说，JVM 的垃圾收集器主要分为两大类：分代收集器和分区收集器，分代收集器的代表是 CMS，分区收集器的代表是 G1 和 ZGC，下面我们来看看这两大类的垃圾收集器([JVM垃圾收集器](https://javabetter.cn/jvm/gc-collector.html#cms))。

![Java垃圾收集器](https://cdn.tobebetterjavaer.com/stutymore/gc-collector-20231227143820.png)


#### G1收集器

类似于slab伙伴系统，它将堆内存分为多个大小相等的区域（Region）(可以通过 -XX:G1HeapRegionSize=n 来设置 Region 的大小)。每个块可以分属于不同的区。使用管理器进行连接。同时设置了`Humongous` 进行大对象管理，对象大小超过50%就会放入其中。大对象可能会横跨多个`Region`来存放。

![](https://cdn.tobebetterjavaer.com/stutymore/gc-collector-20231228213824.png)

G1回收机制特点如下：
- 分代：
- 增量：
- 并行：
- 标记整理：
- STW：


G1中的GC模式如下

1. Young GC(Minor GC): Eden区空间不足时触发，指对新生代区域进行回收。比较频繁

2. Mixed GC: 收集整个新生代和部分老年代，只有G1有这个模式。
当需要分配对象到 Humongous 区域或者堆内存的空间占比超过 `-XX:G1HeapWastePercent` 设置的 InitiatingHeapOccupancyPercent 值时，G1 会触发一次 concurrent marking，它的作用就是计算老年代中有多少空间需要被回收，当发现垃圾的占比达到 -XX:G1HeapWastePercent 中所设置的 G1HeapWastePercent 比例时，在下次 Young GC 后会触发一次 Mixed GC。

3. Full GC(Major GC): 堆浪费百分比超过`G1HeapWastePercent`时进行full gc。收集新生代、老年代的所有对象。耗时较高，需要避免。


![](https://cdn.tobebetterjavaer.com/stutymore/gc-collector-20231228215108.png)


#### ZGC（The Z Garbage Collector）

JDK11 推出的一款低延迟垃圾收集器，适用于大内存低延迟服务的内存管理和回收

## 为什么Survivor 有 2 个区域

1. 使用两个交替使用，保证在执行Minor GC时，Eden 对象能够正常移动到Survivor区。减少内存锁的使用，提高GC速度，用空间换时间。
2. 不分更多块呢？比方说分成三个、四个、五个？：Survivor 区再细分下去，每一块的空间就会比较小，容易导致 Survivor 区满，两块 Survivor 区可能是经过权衡之后的最佳方案


## [JVM方法区和元空间的关系到底是怎样的](https://zhuanlan.zhihu.com/p/344537168)


![方法区](https://pic3.zhimg.com/v2-ff3818b561f7cc42b17f312fc2dd116a_r.jpg)

- 方法区：存放方法类定义(.class)

## 如何进行JVM错误排查调试

![相关错误调试](https://pdai.tech/images/jvm/java-jvm-debug.png)
