---
layout:     post
title:      奔跑吧Linux内核(一)
subtitle:   奔跑吧Linux内核阅读笔记(一)
date:       2022-10-17
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - Linux kernel
    - C
    - 阅读笔记
---

## _参考链接_

- [知乎书籍地址](https://www.zhihu.com/pub/reader/120286010/chapter/1486773903774748672)
- [源码地址](http://github.com/wangpengcheng/linux)
- [linux kernel document](https://www.kernel.org/doc/html/latest/index.html)
- [目标版本(Linux5.0)](https://mirrors.edge.kernel.org/pub/linux/kernel/v5.x/linux-5.0.tar.xz)
- [linux-insides](https://0xax.gitbooks.io/linux-insides/content/index.html)
- [linux-insides-zh](https://xinqiu.gitbooks.io/linux-insides-cn/content/index.html)
- [linux 启动概述](https://0xax.gitbooks.io/linux-insides/content/Booting/linux-bootstrap-1.html)
- [X64汇编](https://0xax.github.io/asm_2/)

## 前言
- 实际使用Linux版本为5.19

### 辅助阅读工具

- [在线阅读工具](https://elixir.bootlin.com/linux/v5.19/C/ident/readonly)
- [内核源码树map](https://makelinux.github.io/kernel/map/)

# x64 汇编概述

- [X86/X64处理器体系结构及寻址模式](https://zhuanlan.zhihu.com/p/549874101)

