---
layout:     post
title:      深度探索Linux系统虚拟化(一)
subtitle:   深度探索Linux系统虚拟化阅读笔记(一)
date:       2023-02-16
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - Linux
    - kvm
    - C
    - 阅读笔记
---

# 深度探索Linux系统虚拟化

> 2023-02-17 01:50:00

# 第一章 CPU 虚拟化

## 1.1 x86架构CPU虚拟机化
虚拟化三个条件：

1. 等价性：VMM需要模拟出本质上与物理机一致的环境。
2. 高效性：指令执行并无明显性能损耗。
3. 资源控制：可以通过VMM完全控制资源。

## 1.2 陷入和模拟模型ß