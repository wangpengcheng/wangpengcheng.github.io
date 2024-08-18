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
- [鹅厂微创新Golang缓存组件TCache介绍](https://www.iminho.me/wiki/blog-39.html)
- [使用go设置多缓存驱动](https://cloud.tencent.com/developer/article/2171041)
- [kratos中使用rockscache介绍](https://www.cnblogs.com/paulwhw/p/17120966.html)
- [分布式多级缓存系统设计与实战](https://xie.infoq.cn/article/58e07307f546badafb6ac6b38)
- [jetcache](https://github.com/alibaba/jetcache/blob/master/introduce_CN.md)
- [使用Go设计多缓存驱动](https://cloud.tencent.com/developer/article/2171041)
- [gocache](http://github.com/eko/gocache)


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

jetcache 在多级缓存上进行了进一步的缓存封装，并允许通过自定义函数的方式进行，调用下级函数进行实际的值填充。

cache 核心接口代码

```go
// https://github.com/mgtv-tech/jetcache-go/blob/7570bd5d950b4e016cf241b45e3536c50e49cd56/cache.go
type (
    // Cache interface is used to define the cache implementation.
    Cache interface {
        // Set sets the cache with ItemOption
        Set(ctx context.Context, key string, opts ...ItemOption) error
        // Once gets the opts.value for the given key from the cache or
        // executes, caches, and returns the results of the given opts.do,
        // making sure that only one execution is in-flight for a given key
        // at a time. If a duplicate comes in, the duplicate caller waits for the
        // original to complete and receives the same results.
        Once(ctx context.Context, key string, opts ...ItemOption) error
        // Delete deletes cached val with key.
        Delete(ctx context.Context, key string) error
        // DeleteFromLocalCache deletes local cached val with key.
        DeleteFromLocalCache(key string)
        // Exists reports whether val for the given key exists.
        Exists(ctx context.Context, key string) bool
        // Get gets the val for the given key and fills into val.
        Get(ctx context.Context, key string, val any) error
        // GetSkippingLocal gets the val for the given key skipping local cache.
        GetSkippingLocal(ctx context.Context, key string, val any) error
        // TaskSize returns Refresh task size.
        TaskSize() int
        // CacheType returns cache type
        CacheType() string
        // Close closes the cache. This should be called when cache refreshing is
        // enabled and no longer needed, or when it may lead to resource leaks.
        Close()
    }

    jetCache struct {
        sync.Mutex // 分布式锁
        Options  // 选择函数
        group          singleflight.Group // 避免同时执行重复的函数
        safeRand       *util.SafeRand     // 线程安全的随机数生成器
        refreshTaskMap sync.Map           // 更新任务map
        eventCh        chan *Event        // 事件管道
        stopChan       chan struct{}      // 停止管道
    }
)
```
其中 `Options` 使用了操作链模式，进行了依赖翻转，可以通过函数链，进行快速的参数设置，值得学习， 定义如下：

```go
type (
    // Options are used to store cache options.
    Options struct {
        name                       string             // Cache name, used for log identification and metric reporting
        remote                     remote.Remote      // Remote is distributed cache, such as Redis.
        local                      local.Local        // Local is memory cache, such as FreeCache.
        codec                      string             // Value encoding and decoding method. Default is "msgpack.Name". You can also customize it.
        errNotFound                error              // Error to return for cache miss. Used to prevent cache penetration.
        remoteExpiry               time.Duration      // Remote cache ttl, Default is 1 hour.
        notFoundExpiry             time.Duration      // Duration for placeholder cache when there is a cache miss. Default is 1 minute.
        offset                     time.Duration      // Expiration time jitter factor for cache misses.
        refreshDuration            time.Duration      // Interval for asynchronous cache refresh. Default is 0 (refresh is disabled).
        stopRefreshAfterLastAccess time.Duration      // Duration for cache to stop refreshing after no access. Default is refreshDuration + 1 second.
        refreshConcurrency         int                // Maximum number of concurrent cache refreshes. Default is 4.
        statsDisabled              bool               // Flag to disable cache statistics.
        statsHandler               stats.Handler      // Metrics statsHandler collector.
        sourceID                   string             // Unique identifier for cache instance.
        syncLocal                  bool               // Enable events for syncing local cache (only for "Both" cache type).
        eventChBufSize             int                // Buffer size for event channel (default: 100).
        eventHandler               func(event *Event) // Function to handle local cache invalidation events.
    }

    // Option defines the method to customize an Options.
    Option func(o *Options)  // Option 修改函数

)


func WithName(name string) Option {
    return func(o *Options) {
        o.name = name
    }
}

func WithRemote(remote remote.Remote) Option {
    return func(o *Options) {
        o.remote = remote
    }
}

func WithLocal(local local.Local) Option {
    return func(o *Options) {
        o.local = local
    }
}
func newOptions(opts ...Option) Options {
    var o Options
    for _, opt := range opts {
        opt(&o)
    }
}

// 使用代码示例如下：
// o := newOptions(
//    WithOffset(30 * time.Second),
//    WithRefreshConcurrency(16),
//    WithCodec(json.Name),
// )
```

下面让我们以值插入和查询为例进行核心的代码分析:
1. 写入示例， 对应的写入示例如下：

```go
func Example_advancedUsage() {
    // 创建redis ring
    ring := redis.NewRing(&redis.RingOptions{
        Addrs: map[string]string{
        "localhost": ":6379",
        },
    })
    // 创建cache
    mycache := cache.New(
        cache.WithName("any"),                                          // 指定名称
        cache.WithRemote(remote.NewGoRedisV8Adaptor(ring)),             // 添加远程redis缓存
        cache.WithLocal(local.NewFreeCache(256*local.MB, time.Minute)), // 添加本地缓存
        cache.WithErrNotFound(errRecordNotFound),                       // 指定无数据异常
        cache.WithRefreshDuration(time.Minute),                         // 指定刷新时间
    )

    ctx := context.TODO()
    key := "mykey:1"
    obj := new(object)
    // 查询对应值
    if err := mycache.Once(
        ctx,
        key,
        cache.Value(obj),     // 设置值类型
        cache.TTL(time.Hour), // 设置TTL时间
        cache.Refresh(true),  // 是否进行刷新
        cache.Do(
        func(ctx context.Context) (any, error) {
            return mockDBGetObject(1)
        }), // 值处理函数--当出现缓存not found 时调用此函数
    ); err != nil {
        panic(err)
    }
    // 输出最终对象
    fmt.Println(obj)
    // Output: &{mystring 42}
    // 关闭缓存块
    mycache.Close()
}
```

2. once 函数实现：使用once 函数进行值设置和读取。核心关键代码如下：
```go
// https://github.com/mgtv-tech/jetcache-go/blob/7570bd5d950b4e016cf241b45e3536c50e49cd56/cache.go

func (c *jetCache) Once(ctx context.Context, key string, opts ...ItemOption) error {
    // 创建新的item
    item := newItemOptions(ctx, key, opts...)
    // 更新刷线任务
    c.addOrUpdateRefreshTask(item)
    // 设置item
    b, cached, err := c.getSetItemBytesOnce(item)
    if err != nil {
        return err
    }

    if bytes.Compare(b, notFoundPlaceholder) == 0 {
        return c.errNotFound
    }

    if item.value == nil || len(b) == 0 {
        return nil
    }

    if err := c.Unmarshal(b, item.value); err != nil {
        // 解析异常的，需要先进行删除
        // 再次尝试获取
        if cached {
            _ = c.Delete(ctx, item.key)
            return c.Once(ctx, key, opts...)
        }
        return err
    }

    return nil
}

```
这里是整体的获取逻辑，关键在于获取失败的会自动进行重试。值查询的核心在于`getSetItemBytesOnce` 进行核心的值查询

3. `getSetItemBytesOnce` 兼顾了值查询与写入，核心的处理逻辑如下：

```go
func (c *jetCache) getSetItemBytesOnce(item *item) (b []byte, cached bool, err error) {
    if !item.skipLocal && c.local != nil {
        // 先统local 进行值查询
        b, ok := c.local.Get(item.key)
        // 更新对应命中率
        if ok {
            c.statsHandler.IncrHit()
            c.statsHandler.IncrLocalHit()
            if bytes.Compare(b, notFoundPlaceholder) == 0 {
                return nil, true, c.errNotFound
            }
            return b, true, nil
        }
    }
    // 使用group.Do 保证一次只有一个再执行
    v, err, _ := c.group.Do(
        item.key, // key 用于锁住函数
        // 核心的执行函数
        func() (any, error) {
            // 查询对应值--
            b, err := c.getBytes(item.Context(), item.key, item.skipLocal)
            if err == nil {
                cached = true
                return b, nil
            } else if errors.Is(err, c.errNotFound) {
                cached = true
                return nil, c.errNotFound
            }
            // 缓存中不存在该值--缓存穿透了--执行对应是饥饿中加密手机
            b, ok, err := c.set(item)
            if ok {
                c.send(EventTypeSetByOnce, item.key)
                return b, nil
            }

        return nil, err
    })

    if err != nil {
        return nil, false, err
    }

    return v.([]byte), cached, nil
}
```

这里核心还是去调用了set 函数，进行值的更新和设置,getValue 会主动调用`Do`钩子，通过用户注入函数，进行值的获取

```go
func (c *jetCache) set(item *item) ([]byte, bool, error) {
    // 先查询对象是否存在
    val, err := item.getValue()
    // 设置函数非空，新增参数
    if item.do != nil {
        c.statsHandler.IncrQuery()
    }

    if c.IsNotFound(err) {
        if e := c.setNotFound(item.Context(), item.key, item.skipLocal); e != nil {
        logger.Error("setNotFound(%s) error(%v)", item.key, err)
        }
        return notFoundPlaceholder, true, nil
    } else if err != nil {
        c.statsHandler.IncrQueryFail(err)
        return nil, false, err
    }

    b, err := c.Marshal(val)
    if err != nil {
        return nil, false, err
    }
    // 设置本地缓存
    if c.local != nil && !item.skipLocal {
        c.local.Set(item.key, b)
    }

    if c.remote == nil {
        if c.local == nil {
            return b, true, ErrRemoteLocalBothNil
        }
        return b, true, nil
    }
    // 查询剩余存在时间
    ttl := item.getTtl(c.remoteExpiry)
    if ttl == 0 {
        return b, true, nil
    }
    // 设置远端缓存
    if item.setXX {
        _, err := c.remote.SetXX(item.Context(), item.key, b, ttl)
        return b, true, err
    }
    if item.setNX {
        _, err := c.remote.SetNX(item.Context(), item.key, b, ttl)
        return b, true, err
    }
    return b, true, c.remote.SetEX(item.Context(), item.key, b, ttl)
}

func (item *item) getValue() (any, error) {
    if item.do != nil {
        // 执行数据处理--使用用户注入钩子进行处理
        return item.do(item.Context())
    }
    if item.value != nil {
        return item.value, nil
    }
    return nil, nil
}

```


### spring cloud cache

- [spring-cache源码分析图文详解](https://juejin.cn/post/7066990990715781151)


### [gocache](https://github.com/eko/gocache)

### 本地缓存方案



## 核心业务处理

### 缓存查询

1. 分级查询
2. 查询到后更新前几级的值
3. 最终查询不到，直接异常

![缓存查询更新](https://camo.githubusercontent.com/1e62128676297eb2c3703e2550e5349bfec8de23f41bea7f5ae8fe7e418a0fbf/68747470733a2f2f692e6c6f6c692e6e65742f323032302f30392f32372f64624d69596877544275725a4b34792e706e67)

### 缓存更新

1. 更新最后一层缓存
2. 删除二级缓存
3. 删除一级缓存


### 缓存添加

1. 添加二级缓存

### 缓存删除



## 存在问题

- [什么是缓存雪崩、击穿、穿透](https://xiaolincoding.com/redis/cluster/cache_problem.html)


### 缓存击穿

热点数据过期，导致无法从缓存中访问数据，直接从数据库中访问。导致数据库被请求冲垮。

#### 解决方案

1. 互斥锁：保证同一时间只有一个业务线程更新缓存，未能获取互斥锁的请求。等待锁释放后重新读取缓存。要么返回空值或者默认值
2. 不设置过期时间：由后台异步更新缓存，在热点数据要过期前，提前通知后台线程更新缓存以及重新设置过期时间。


### 缓存雪崩

缓存数据同一时间大量过期，导致数据大量击中mysql导致异常。

#### 解决方案

1. 均匀设置过期时间
给缓存数据设置过期时间，可以在对数据设置过期时间时，添加随机数，保证数据不会在同一时间过期

2. 互斥锁

发现访问数据不在Redis里，就加个互斥锁，保证同一时间内只有一个请求来构建缓存(从数据库读取数据，再将数据更新到redis中)。构建完成后再进行锁释放。未能获取互斥锁的请求，等锁释放后重新读取缓存。

3. 后台更新缓存

- 设置缓存“永久有效”，**并将更新缓存的工作交给后台线程定时更新**
- 通过消息队列发送缓存失效数据，保证缓存失效后，重新进行更新。即：缓存预热

### 缓存穿透

用户访问的数据，**既不在缓存中，也不再数据库中**。导致请求在访问缓存时，发现缓存缺失。访问数据库中也没有构建缓存数据。大量请求时导致缓存数据库压力增加。


### 一级缓存(本地缓存)一致性问题

使用一级缓存后，在分布式场景下，存在redis数据已经更新，但是另外一个实例的一级缓存并未更新的情况。导致本地缓存与redis数据不一致。

#### 解决方法

使用redis发布订阅模式，实现多级缓存key的删除。


### 一级缓存(本地缓存)数据污染问题


缓存污染问题指的是留存在缓存中的数据，实际不会再被访问了，但是又占据了缓存空间。


### 热key问题

部分数据访问量较高，缓存过期之后，会存在瞬间的穿透。

#### 解决方案
- 设置查询延期，针对不同层的参数，可以指定key的默认存在时间。每次查询后，对重新增强当前key的过期时间
- 查询频率延期：在查询延期的基础上，设置查询频率延期的策略，越是高热点数据，默认缓存延期时间越长


#### 解决方法

设置本地缓存超时时间，对超时的缓存进行清理。

使用定时清理的方式进行缓存清除

## 综合解决方案

1. 缓存雪崩：涉及到key过期问题，应由使用方自行解决。
    - key过期时间梯度设置：将key的过期时间进行梯度设置，保证前一层的过期时间小于后一层。进行层层拦截
    - 随机过期时间：使用参数，决定是否加入随机过期时间，防止缓存雪崩。
2. 缓存穿透：
    - 多层缓存中，key 设置下层对应的miss 次数和limit_qps, 当miss 次数超过limit下一层的limit 时不再进行，下层访问：而是直接返回empty
    - 层访问qos limit: 每层设置下一层的访问与当前访问qps,  qps limit。超过此限制时，直接返回异常
    - 最大QPS: mysql: 1800, redis(ba): 100,000(3-5万)。单机（nginx）:120w~150w,

3. 缓存击穿：互斥锁使用，保证同一时间只有一个会进行更新，加层限制后避免

4. 本地缓存一致性：
    - 使用redis发布订阅，删除本地缓存。
    - 给本地缓存设置一个较短的生存时间，尽量降低污染影响


5. 本地缓存选型：
综合考虑，本地缓存选型为freecache 与ristretto 缓存

6. 层级锁：
    - 更新(删除/更新)下级缓存时，需要先拿到层级锁后再进行操作。