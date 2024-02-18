---
layout:     post
title:      go面经
subtitle:   go 语言常见面经汇总
date:       2023-04-17
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - 后台开发
    - 面试
    - go 
---

# HTTP面试总结

_参考链接:_

- [go语言常见面试题](https://zhuanlan.zhihu.com/p/434629143)
- [Go 语言笔试面试题汇总](https://geektutu.com/post/qa-golang.html)
- [GO 程序员面试笔试宝典](https://golang.design/go-questions/)
- [Interview面试指南](https://interview.apachecn.org/)
- [go面试题集合](https://github.com/lifei6671/interview-go)
- [go面试题](https://github.com/shomali11/go-interview)
- [go面试题收集](https://github.com/yqchilde/Golang-Interview)
- [go面经](https://github.com/go-share-team/go_interview)
- [牛客go面经](https://www.nowcoder.com/search/all?query=go%20%E9%9D%A2%E7%BB%8F&type=all&searchType=%E9%A1%B6%E9%83%A8%E5%AF%BC%E8%88%AA%E6%A0%8F)
- [常见算法题总结](https://codetop.cc/home)

## 基础语法

### 01 = 和 := 的区别？
### 02 指针的作用
### 03 Go 允许多个返回值吗？
### 04 Go 有异常类型吗？
### 05 什么是协程（Goroutine）
### 06 如何高效地拼接字符串
### 07 什么是 rune 类型
### 08 如何判断 map 中是否包含某个 key ？
### 09 Go 支持默认参数或可选参数吗？
### 10 defer 的执行顺序
### 11 如何交换 2 个变量的值？
### 12 Go 语言 tag 的用处？
### 13 如何判断 2 个字符串切片(slice) 是相等的？
### 14 字符串打印时，%v 和 %+v 的区别
### 15 Go 语言中如何表示枚举值(enums)？
### 16 空 struct{} 的用途

## 实现原理

### 01 init() 函数是什么时候执行的？
### 02 Go 语言的局部变量分配在栈上还是堆上？
### 03 2 个 interface 可以比较吗 ？
### 04 2 个 nil 可能不相等吗？
### 05 简述 Go 语言GC(垃圾回收)的工作原理
### 06 函数返回局部变量的指针是否安全？
### 07 非接口非接口的任意类型 T() 都能够调用 *T 的方法吗？反过来呢？

## 并发编程

### 01 无缓冲的 channel 和有缓冲的 channel 的区别？
### 02 什么是协程泄露(Goroutine Leak)？
### 03 Go 可以限制运行时操作系统线程的数量吗？

## 代码输出
### 变量与常量
### 作用域
### defer延迟调用



## 基础

### 1. golang 中 make 和 new 的区别？（基本必问）

### 2. 数组和切片的区别 （基本必问）

### 3. for range 的时候它的地址会发生变化么？

### 4. go defer，多个 defer 的顺序，defer 在什么时机会修改返回值？

### 5. uint 类型溢出

### 6. 介绍 rune 类型

### 7. golang 中解析 tag 是怎么实现的？反射原理是什么？

### 8. 调用函数传入结构体时，应该传值还是指针？ （Golang 都是传值）


## context相关

### 1. context 结构是什么样的？

### 2. context 使用场景和用途

## channel相关

### 1. channel 是否线程安全？锁用在什么地方？

### 2. go channel 的底层实现原理 （数据结构）

### 3. nil. 关闭的 channel. 有数据的 channel，再进行读. 写. 关闭会怎么样？（各类变种题型）

### 4. 向 channel 发送数据和从 channel 读数据的流程是什么样的？

## map相关

### 1. map 使用注意的点，并发安全？

### 2. map 循环是有序的还是无序的？

### 3. map 中删除一个 key，它的内存会释放么？

### 4. 怎么处理对 map 进行并发访问？有没有其他方案？ 区别是什么？

### 5. nil map 和空 map 有何不同？

### 6. map 的数据结构是什么？是怎么实现扩容？

## GMP相关

### 1. 什么是 GMP？（必问）

### 2. 进程. 线程. 协程有什么区别？

### 3. 抢占式调度是如何抢占的？

### 4. M 和 P 的数量问题？

## 锁相关

### 1. 除了 mutex 以外还有那些方式安全读写共享变量？

### 2. Go 如何实现原子操作？

### 3. Mutex 是悲观锁还是乐观锁？悲观锁. 乐观锁是什么？

### 4. Mutex 有几种模式？

### 5. goroutine 的自旋占用资源如何解决

## 并发相关

### 1. 怎么控制并发数？

### 2. 多个 goroutine 对同一个 map 写会 panic，异常是否可以用 defer 捕获？

### 3. 如何优雅的实现一个 goroutine 池（百度. 手写代码）

## GC相关

### 1. go gc 是怎么实现的？（必问）

### 2. go 是 gc 算法是怎么实现的？ （得物，出现频率低）

### 3. GC 中 stw 时机，各个阶段是如何解决的？ （百度）

### 4. GC 的触发时机？

## 内存相关

### 1. 谈谈内存泄露，什么情况下内存会泄露？怎么定位排查内存泄漏问题？

### 2. 知道 golang 的内存逃逸吗？什么情况下会发生内存逃逸？

### 3. 请简述 Go 是如何分配内存的？

### 4. Channel 分配在栈上还是堆上？哪些对象分配在堆上，哪些对象分配在栈上？

### 5. 介绍一下大对象小对象，为什么小对象多了会造成 gc 压力？



## 算法相关

### 标准输入问题
- [go标准输入问题](https://www.nowcoder.com/feed/main/detail/c707c89102eb4fd4b54919546f1a26a4?sourceSSR=search)
