---
layout:     post
title:      Java编程日常学习笔记
subtitle:   Java编程日常学习笔记
date:       2020-05-08
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - Java
    - 基础编程
    - 读书笔记
---

> 2020-07-11 19:20:58

# 阿里实习Java日常笔记

## 2020-07-11

1. 参数校验方法:在Manager层中使用参数校验时；可以使用Google Guava中Preconditions方法使得参数校验变得优雅，可靠:
   1. 参考链接:
      1. [Preconditions优雅的参数校验](http://www.linuxcoming.com/blog/2019/08/29/gavua_preconditions.html)
      2. 
2. Java8 Optional:
3. Mockito :    
   - 参考链接: 
     - [Java Mockito](http://www.testclass.net/mockito)
     - [官方网站](http://site.mockito.org/)
     - [mockito中文文档](https://github.com/hehonghui/mockito-doc-zh)
4. SpringBoot中使用在Controller层中对参数进行校验
   - 参考链接:
     - [这么写参数校验(validator)就不会被劝退了](https://juejin.im/post/5d3fbeb46fb9a06b317b3c48)
     - [@RequestBody配合@Valid 校验入参参数](https://blog.csdn.net/lzhcoder/article/details/98870852)
5. 使用`ToStringBuilder.reflectionToString(this, ToStringStyle.SHORT_PREFIX_STYLE);`重载toString可以让所有子类都可以实现字符串化。避免每个类都实现toString方法。