---
layout:     post
title:      java简单学习笔记(一)
subtitle:   java简单学习笔记(一)
date:       2020-04-21
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - Java学习
    - 后台开发
    - 网络编程
---

> 2020-05-06 20:20:56

# 参考链接
- [菜鸟教程JAVA](https://www.runoob.com/java/java-tutorial.html)
# 1. JAVA简介
Java分为三个体系：
- JavaSE（J2SE）（Java2 Platform Standard Edition，java平台标准版）
- JavaEE(J2EE)(Java 2 Platform,Enterprise Edition，java平台企业版)
- JavaME(J2ME)(Java 2 Platform Micro Edition，java平台微型版)。
- 2005年6月，JavaOne大会召开，SUN公司公开Java SE 6。此时，Java的各种版本已经更名以取消其中的数字"2"：J2EE更名为Java EE, J2SE更名为Java SE，J2ME更名为Java ME。

Java的特点:
- 简单
- 面向对象
- 分布式(优秀的网络编程库)：在基本的Java应用编程接口中有一个网络应用编程接口（java net），它提供了用于网络应用编程的类库，包括URL、URLConnection、Socket、ServerSocket等。Java的RMI（远程方法激活）机制也是开发分布式应用的重要手段。
- 健壮的：Java的强类型机制、异常处理、垃圾的自动收集等是Java程序健壮性的重要保证。对指针的丢弃是Java的明智选择。Java的安全检查机制使得Java更具健壮性。
- 安全:Java对通过网络下载的类具有一个安全防范机制（类ClassLoader），如分配不同的名字空间以防替代本地的同名类、字节代码检查，并提供安全管理机制（类SecurityManager）让Java应用设置安全哨兵。
- 体系结构中立:Java程序（后缀为java的文件）在Java平台上被编译为体系结构中立的字节码格式（后缀为class的文件），然后可以在实现这个Java平台的任何系统中运行。这种途径适合于异构的网络环境和软件的分发。
- 可移植的:因为中立性，因此可以在任何平台中运行
- 解释型的:Java程序在Java平台上被编译为字节码格式，然后可以在实现这个Java平台的任何系统中运行。在运行时，Java平台中的Java解释器对这些字节码进行解释执行，执行过程中需要的类在联接阶段被载入到运行环境中。
- 高性能的：与那些解释型的高级脚本语言相比，Java的确是高性能的。事实上，Java的运行速度随着JIT(Just-In-Time）编译器技术的发展越来越接近于C++。
- 多线程：
- 动态的：

# 2 Java开发环境搭建

- 注意:Java文件名需要和类名一致

![java程序的区别](https://www.runoob.com/wp-content/uploads/2013/12/ZSSDMld.png)


# 3 Java中的异常
![Java中的异常](https://images2017.cnblogs.com/blog/858860/201709/858860-20170911125844719-1230755033.png)
