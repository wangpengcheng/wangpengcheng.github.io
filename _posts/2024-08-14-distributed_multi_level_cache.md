---
layout:     post
title:      分布式多级缓存
subtitle:   分布式多级缓存学习笔记
date:       2024-08-14
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - 分布式
    - 多级缓存
    - redis/cache
---

> 2024-08-14 15:20:58

_参考链接_：


- [gin-cache](https://github.com/gin-gonic/contrib/tree/master/cache)
- [beego-cache](https://github.com/beego/beego-cache)
- [bigcache](https://github.com/allegro/bigcache)
- [分布式多级缓存SDK设计的思考](https://cloud.tencent.com/developer/article/2337543)
- [实战干货 | 分布式多级缓存设计方案](https://developer.aliyun.com/article/1126957)
- [常见缓存更新策略](https://zhuanlan.zhihu.com/p/543438986)
- [jetcache-go多级缓存](https://juejin.cn/post/7278246015194447887)
- [统一缓存帝国 - 实战 Spring Cache](https://www.51cto.com/article/668874.html)
- [Golang 高性能无 GC 的缓存库 bigcache 是怎么实现的？](https://www.51cto.com/article/782076.html)
- [请收藏！golang本地缓存选型对比及原理总结](https://www.sohu.com/a/532687715_100093134)
- [Go 语言分布式缓存 Groupcache – 用法，源码深度解读](https://marksuper.xyz/2022/08/26/groupcache/)
- [spring boot cache 多级缓存](https://developer.aliyun.com/article/1238415)


## 开源方案调研

### gin-cache 

gin cache 中使用统一的 cache store 接口来进行本地和redis的缓存设置。关键核心代码如下：

```go
https://github.com/gin-contrib/cache/master/blob/persistence/cache_store.go
type CacheStore interface {
    // Get retrieves an item from the cache. Returns the item or nil, and a bool indicating
    // whether the key was found.
    Get(key string, value interface{}) error

    // Set sets an item to the cache, replacing any existing item.
    Set(key string, value interface{}, expire time.Duration) error

    // Add adds an item to the cache only if an item doesn't already exist for the given
    // key, or if the existing item has expired. Returns an error otherwise.
    Add(key string, value interface{}, expire time.Duration) error

    // Replace sets a new value for the cache key only if it already exists. Returns an
    // error if it does not.
    Replace(key string, data interface{}, expire time.Duration) error

    // Delete removes an item from the cache. Does nothing if the key is not in the cache.
    Delete(key string) error

    // Increment increments a real number, and returns error if the value is not real
    Increment(key string, data uint64) (uint64, error)

    // Decrement decrements a real number, and returns error if the value is not real
    Decrement(key string, data uint64) (uint64, error)

    // Flush deletes all items from the cache.
    Flush() error
}

```

gin-cache 通过统一定义缓存接口的方式实现缓存抽象，但是并没有进行统一的缓存管理，更没有做多级缓存管理，没有解决缓存穿透、雪崩、击穿等问题。

### jetcache-go

jetcache 在多级缓存上进行了进一步的缓存封装，并允许通过自定义函数的方式进行，调用下级函数进行实际的值充填





### spring cloud cache

- [spring-cache源码分析图文详解](https://juejin.cn/post/7066990990715781151)





### 本地缓存方案



## 核心业务处理

### 缓存查询更新

![缓存查询更新](https://camo.githubusercontent.com/1e62128676297eb2c3703e2550e5349bfec8de23f41bea7f5ae8fe7e418a0fbf/68747470733a2f2f692e6c6f6c692e6e65742f323032302f30392f32372f64624d69596877544275725a4b34792e706e67)

### 缓存更新

### 缓存添加

### 缓存删除



## 存在问题

### 缓存击穿


### 缓存雪崩

### 缓存穿透