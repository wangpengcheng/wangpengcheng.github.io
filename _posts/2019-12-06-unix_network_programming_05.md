---
layout:     post
title:      UNIX网络编程 学习笔记 (五)
subtitle:   UNIX网络编程 学习笔记 (五) 
date:       2019-12-05
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - C/C++
    - UNIX
    - 网络编程
---

# UNIX网络编程 学习笔记
_参考链接：_

- [《Unix网络编程》卷1 初级](https://blog.csdn.net/zzxiaozhao/article/details/102637708)
- [《Unix网络编程》卷1 中级](https://blog.csdn.net/zzxiaozhao/article/details/102662861)
- [《Unix网络编程》卷1 高级](https://blog.csdn.net/zzxiaozhao/article/details/102771985)
- [《UNIX网络编程卷一》读书笔记](https://zevvez.github.io/2017/12/13/unp/#%E7%AC%AC%E4%B8%80%E7%AB%A0-%E7%AE%80%E4%BB%8B)

> 2019-12-05 21:10:53

## 第 17 章 ioctl函数

这个是一个系统调用接口，主要是为文件写入和输出信息

```c
#include <unistd.h>
/* 若成功则为0，若出错就为-1 */
int ioctl(int fd,int request,.../* void *arg */);
```
和网络相关的请求总结如下:

![网络相关iotcl请求总结](https://wangpengcheng.github.io/img/2019-12-06-16-29-30.png)

### 17.5 接口配置

调用iotcl之前先分配一个缓冲区和一个ifconf结构，然后初始化，后者。初始化结果和返回值如下：

![](https://wangpengcheng.github.io/img/2019-12-06-16-33-28.png)

![](https://wangpengcheng.github.io/img/2019-12-06-16-33-58.png)


### 17.8 ARP 高速缓存操作

使用ioctl进行ARP的高速缓存操作。使用路由域的系统，往往改用路由套接字访问ARP高速缓存。可以使用get_ifi_info函数，获取本主机上所有的IP地址


### 17.9 路由表操作

使用ioctl SIOCADDRT向路由表中添加一个表项；
SIOCDELRT 从路由表中删除一个表项。

### 第 18 章 
