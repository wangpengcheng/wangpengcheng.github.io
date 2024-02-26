---
layout:     post
title:      go语言设计与实现(一)
subtitle:   go语言设计与实现阅读笔记(一)
date:       2021-08-30
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - go
    - 源码分析
    - 阅读笔记
---

# go语言设计与实现阅读笔记(-)

> 2021-08-31 00:45:58

_参考链接：_
- [书籍代码地址](https://github.com/draveness/go-internal)
- [在线书籍地址](https://draveness.me/golang/)
- [Go 语言原本](https://golang.design/under-the-hood/)
- [深入解析go](https://tiancaiamao.gitbooks.io/go-internals/content/zh/01.1.html)
  
## 前言

本书主要分为：
- 编译原理
- 运行时
- 基础知识
- 进阶知识

详细文件结构如下:

![文章主要架构图](https://img.draveness.me/2020-02-07-15810644766743-contents-mindnode.png)


# 第一章 go语言的前世今生
## 1.2 go语言综述

### 1.2.1 基础类型和值
go 语言的基础类型如下：
```bash
bool, 
uint, uint8, uint16, uint32, uint64, 
int, int8, int16, int32, int64, 
float32, float64
```