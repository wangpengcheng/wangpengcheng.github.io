---
layout:     post
title:      并行应用开发和优化概论阅读笔记
subtitle:   并行应用开发和优化概论之系统优化
date:       2019-5-4
author:     敬方
header-img: img/post-bg-miui6.jpg
catalog: true
tags:
    - 高性能计算
    - 并行计算
    - 理论知识
---

> 2019-6-18: 20:47:56

# 并行应用开发和优化概论阅读笔记

_参考链接：_ [并行应用开发及优化方法概论]()

## 现代计算机体系架构下的并行化

- 体系架构选择优化成本（制造与功耗）与性能 
- 指令级并行 (ILP)
    + 流水线 (Pipelining)
    + 多指令发射
    + 乱序执行
- 向量化 (SIMD)
    + 单条指令执行能够同时处理多个数据片
- 多线程 (MIMD)
    + 多个程序实例能同时运行
- 多核

## CPU架构发展：处理器发展重要技术

_参考链接：_ [CPU的内部架构和工作原理](https://blog.csdn.net/stpeace/article/details/80101441);[处理器体系结构（了解CPU的基本运行原理）](https://blog.csdn.net/yang_yulei/article/details/22529437);

- 位宽增大 8bit -> 16bit -> 32bit ->64 bit
- 流水线，使用多级流水提高主频：
    +  超流水
    +  超标量
    +  超长指令字
- 基于Tomasulo算法的乱序执行
- SSE (Streaming SIMD Extensions) 指令集
- 集成内存控制器
- 超线程技术，进一步利用闲置计算部件
- 多核技术。

## 性能关键指标

- 网络
    + 以太网(千兆、万兆)
    + Infiniband网络([参考链接](https://baike.baidu.com/item/Infiniband/1963979?fr=aladdin))
    + 当机器配置多种网络时，需要查看是否使用高速的网络。
- CPI(Cycles Per Instruction)
    + CPI表示每条指令完成所需要的周期数。
    + CPU使用多发射技术，每个周期最多4条指令。
        * 理想状态 CPI ＝ 0.25
    + CPI过高：
        * 所选代码进一步优化的可能性高。
- 浮点运算峰值
    + CPU浮点计算理论峰值计算方法。
        * 例如：2x Intel Xeon processor E5-2670 2.6GHz：8 Flops/clock * 8 cores/socket * 2 sockets * 2.6GHz = 332.8 GF/s
    + 科学计算以浮点数为主
    + DGEMM ~93%
    + LINPACK ~91%
- 向量化比率
    + 向量化是CPU峰值计算的倍数因子。
    + 对应用程序性能影响很大。
    + 取值范围为0%～100%。
    + 向量化指令需要根据应用的逻辑。
- Cache Miss
    + 数据的存储层次（访问延迟）：
        * 寄存器 (1cycle)
        * cache: L1 (~4cycles), L2(~10cycles), LLC(~50cycles)
        * 内存(~300 cycles)
        * 本地硬盘（固态 50~150us，旋转 1~10ms）
        * 网络传输
    + cache 利用数据访问局部性原理，缓存最近常访问的数据。
    + L1-D cache miss range

## 查看分析应用性能指标工具

**Paramon和Paratune**

- Paramon直观可视化程序运行过程中的系统级、微架构级和函数级等性能指标，为程序开发者清晰指明程序优化方向。
- Paratune用于分析优化程序性能，尤其针对大规模并行计算程序，通过多节点间系统级和微架构级等性能指标定性与定量对比分析，帮助程序开发者快速定位程序性能瓶颈点，优化编程提高程序性能。
