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
- [golang 快速入门](https://www.zhihu.com/column/c_1212699838745346048)
- [Go 语言笔试面试题汇总](https://geektutu.com/post/qa-golang.html)
- [GO 程序员面试笔试宝典](https://golang.design/go-questions/)
- [Interview面试指南](https://interview.apachecn.org/)
- [go面试题集合](https://github.com/lifei6671/interview-go)
- [go面试题](https://github.com/shomali11/go-interview)
- [go面试题收集](https://github.com/yqchilde/Golang-Interview)
- [go面经](https://github.com/go-share-team/go_interview)
- [牛客go面经](https://www.nowcoder.com/search/all?query=go%20%E9%9D%A2%E7%BB%8F&type=all&searchType=%E9%A1%B6%E9%83%A8%E5%AF%BC%E8%88%AA%E6%A0%8F)
- [常见算法题总结](https://codetop.cc/home)
- [技术摘抄](https://learn.lianglianglee.com/)
- [go专家编程](https://books.studygolang.com/GoExpertProgramming/)

## 基础语法

### go 的基础变量类型有哪些

| 类型分类 |类型|默认值| 描述|
|:---:|:---:|:---:|:---:|
|布尔型|`bool`|false|基础bool类型|
|整数类型|`uint8`|0|无符号8位整型(0~255)|
|^^     |`uint16`|0|无符号16位整型(0~65535)|
|^^     |`uint32`|0|无符号32位整型(0~4294967295)|
|^^     |`uint64`|0|无符号64位整型(0~18446744073709551615)|
|^^     |`int8`|0|有符号8位整型(-128~127)|
|^^     |`int16`|0|有符号16位整型(-32768~32767)|
|^^     |`int32`|0|有符号32位整型 (-2147483648~2147483647)|
|^^     |`int64`|0|有符号32位整型(-9223372036854775808~9223372036854775807)|
|浮点类型|`float32`|0.0|IEEE-754 32位浮点型数|
|^^  |`float64`|0.0|IEEE-754 64位浮点型数|
|^^  |`complex64`|0.0|32位实数和虚数|
|^^  |`complex128`|0.0|64 位实数和虚数|
|其它数字类型|`byte`|0|与uint8一致，一个字节8位|
|^^  |`rune`|0|char默认类型，与int32类似|
|^^     |`int`|0|有符号32/64整型，字节长度与操作系统类型一致|
|^^     |`uint`|0|无符号32/64整型，字节长度与操作系统类型一致|
|^^     |`uintptr`|0|与uint一致，用于存放指针地址|
|派生类型|`pointer`|nil|指针类型，用于指向对应的内存地址|
|^^ |`[]T`|nil|数组类型，用于进行数组存储|
|^^ |`slice`|nil|切片类型，用于进行go切片管理|
|^^ |`Channel`|nil|管道，统一的消息管理|
|^^ |`func`|nil|函数，类似于c语言的函数指针|
|^^ |`interface/any` |nil|接口类型，类似于C++中的虚函数指针|
|^^ |`map`|nil|map类型，hash map基础类型|

___

- 参考:[go语言基础类型](https://www.runoob.com/go/go-data-types.html);[go语言基本类型](https://fasionchan.com/golang/tour/basic-types/);[Golang中uint、int, int8, int16, int32, int64区别](https://blog.csdn.net/FromTheWind/article/details/105862844)


### 01 = 和 := 的区别？
- `=`:
    - 语义不同：仅仅进行赋值，要求左侧对象必须已经声明
    - 使用语法：需要与`var`进行配合使用
    - 使用位置：可以用于函数中和函数外

- `:=`: 
    - 语义：声明+赋值，声明变量的同时进行自动赋值，中间涉及到自动推断
    - 语法：**左侧必须有新的变量生成**，否则会有语法错误
    - 使用位置：只能用于函数中，不能用于函数外
___

- 参考: [Go语言学习之:=与=的区别](https://juejin.cn/post/6947897494059614215);[golang快速入门[8.2]-自动类型推断的秘密](https://zhuanlan.zhihu.com/p/115085755)

### 02 指针的作用

1. 保存变量地址，通过`&` 与`*` 操作符间接访问内存地址，减少值传递损耗

### 03 Go 允许多个返回值吗？

允许，同时允许命名返回值。命名后的返回值，相当于预定义变量。
返回值注意事项：
- 返回值不能用容器对象接收：只能用多个变量或者用`_` 忽略
- 命名返回参数：可看做形参类似局部变量，最后由return 隐式返回
- 局部变量遮蔽：命名返回参数可被同名局部变量遮蔽，此时需要显式返回
- defer作用：defer 函数作用在空return之前。返回值无命名是会生成不同的临时变量。避免异常

```go
func add1(x, y int) (z int) {
    { // 不能在一个级别，引发 "z redeclared in this block" 错误。
        var z = x + y
        // return   // Error: z is shadowed during return
        return z // 必须显式返回。
    }
}

func add2(x, y int) (z int) {
    defer func() {
        z++
        println(z) // 输出: 203
    }()

    z = x + y
    return z + 200 // 执行顺序: (z = z + 200) -> (call defer) -> (return)
}

func add3(x,y int) int {
    z := x+y
    defer func() {
        z++
        println(z) // 输出: 204
    }
    return z + 200 // 执行顺序：(s = z + 200) -> (call defer) -> (return )
}


func main() {
    println(add1(1, 2)) // 输出：3
    println(add2(1, 2)) // 输出: 204
    println(add3(1, 2)) // 输出: 203
}
```
___

- 参考：[go返回值](https://www.topgoer.com/%E5%87%BD%E6%95%B0/%E8%BF%94%E5%9B%9E%E5%80%BC.html)


### 04 Go 有异常类型吗？
go 没有异常类型，只有错误类型，通常用返回值来表示异常状态
```go
f, err := os.Open("test.txt")
if err != nil {
    log.Fatal(err)
}
```

### 05 什么是协程（Goroutine）
Goroutine 是与其他函数或方法同时运行的函数或方法。 Goroutines 可以被认为是轻量级的线程。 与线程相比，创建 Goroutine 的开销很小。 Go应用程序同时运行数千个 Goroutine 是非常常见的做法。
___
参考：[说一说协程](https://wangpengcheng.github.io/2019/12/17/baidu_interview_prepare/#418-%E8%AF%B7%E4%BD%A0%E6%9D%A5%E8%AF%B4%E4%B8%80%E8%AF%B4%E5%8D%8F%E7%A8%8B);[协程与线程区别](https://wangpengcheng.github.io/2019/12/17/baidu_interview_prepare/#418-%E8%AF%B7%E4%BD%A0%E6%9D%A5%E8%AF%B4%E4%B8%80%E8%AF%B4%E5%8D%8F%E7%A8%8B)

### 06 如何高效地拼接字符串
Go语言中，字符串是只读的，每次修改操作都会创建一个新的字符串(与java类似)，如果需要拼接多次，使用strings.Builder，最小化内存拷贝次数

```go
var str strings.Builder
for i := 0; i < 1000; i++ {
    str.WriteString("a")
}
fmt.Println(str.String())
```

___

- 参考：[深入理解 Go 中的字符串](https://blog.51cto.com/yuzhou1su/5268459);[深入理解Go语言的string 类型](https://blog.csdn.net/sinat_31862487/article/details/134044392);[你不知道的Go 之string](https://segmentfault.com/a/1190000040023113)

### 07 什么是 rune 类型
go 语言中rune是unicode 编码代称，是int32的别名，string 可以转换为rune数组。
但是字符串的底层是byte(8 bit)序列，而非rune(32bit)。如下：
```go
package main

import (
    "fmt"
)
func main() {
    a := `Go语言`
    // 这里string底层为byte，`语言`为utf-8编码因此使用6个字节
    fmt.Println(len(a)) // 8
    // 这里转换为rune数组，每个字符都是一个rune，长度为4，内存为16字节
    fmt.Println(len([]rune(a))) // 4
    // 输出内存中第3个字节内容，长度为1字节
    fmt.Println(a[2]) // 232
    // 输出第二个rune内容，长度为4字节
    fmt.Println([]rune(a)[2]) // 35821
    fmt.Println("Hello W3Cschool!")
}
```
- 注意：
    - 字符串遍历**通过下标索引字符串将会产生一个字节**,字符串中含有UTF-8编码字符时，会出现乱码
    - 使用range 遍历：会将字符串转换为rune类型

___

参考:[Go 语言中的字符与字符串遍历](https://zhuanlan.zhihu.com/p/93052559);[你不知道的Go之string](https://segmentfault.com/a/1190000040023113)

### 08 如何判断 map 中是否包含某个 key ？
- 普通情况下： 在map不为`nil` 的情况下，直接根据key取值第二个，检查是否存在即可，如下：
```go
if val, ok := dict["foo"]; ok {
    //do something here
}
```
- 高并发情况下：原始map并非并发安全，高并发情况下，应该使用读写锁/分片锁/sync.map(仅仅适用于一写多读情况)

___

参考：[Go并发之三种线程安全的map](https://zhuanlan.zhihu.com/p/356739568);[Golang sync.Map原理（两个map实现 读写分离、适用读多写少场景）](https://developer.aliyun.com/article/1172753)

### 09 Go支持默认参数或可选参数吗？
- 不支持：GO不支持可选参数和默认参数，也不支持方法重载。可以通过struct的方式，添加默认参数。支持可变参数，可以通过可变参数判断，间接实现参数可选
- 间接支持：
    - 可用通过struct配合反射实现可变参数的间接支持
    - 通过选择器模式与适配器模式实现可变参数的间接支持

___

参考: [golang中函数如何设置参数默认值](https://cloud.tencent.com/developer/article/2025794);[golang函数中的参数为什么不支持默认值?-知乎](
https://www.zhihu.com/question/24368980);[Golang中设置函数默认参数的优雅实现](https://www.cnblogs.com/smartrui/p/10324320.html)

### 10 defer 的执行顺序
- 多个defer 后进先出：多个defer遵循后进先出的原则，最后声明的最先得到执行
- 已代码块为分界点：代码块中defer作用在代码块上下文中
- defer 在return之后，函数退出之前执行：将`return i` 拆分为`s=i; return `两个语句分析接口

```go
func test() int {
	i := 0

    {
        defer func() {
            fmt.Println("defer0")    
        }()
    }

	defer func() {
		fmt.Println("defer1")
	}()
	defer func() {
		i += 1
		fmt.Println("defer2")
	}()
	return i
}


func test1() (i  int) {
    i = 0
	defer func() {
		i += 1
		fmt.Println("defer4")
	}()
	return i
}

func main() {
	fmt.Println("return", test())
    fmt.Println("return", test1())
}
// defer0
// defer2
// defer1
// return 0
// defer4
// return 1
```
___

- 参考：[Golang的defer与return的执行顺序](https://juejin.cn/post/7095631673865273352)

### 11 如何交换 2 个变量的值？

go 中无swap函数，也不像java交换复杂(需要借助临时变量)，直接交换即可。如下：

```go 
a, b := "A", "B"
a, b = b, a
fmt.Println(a, b) // B A
```

### 12 Go 语言 tag 的用处？

本质：tag的本质是struct字段注解：用于定义字段的一个或者多个属性，通过反射获取tag进行解析
由本质衍生了如下作用：
- 序列化和反序列化：通过标签控制字段与输出值的映射，控制其序列化与反序列化，如json转换：通过`json:"xxx"`定义结构体字段与json字段的转换关系
- 数据验证：通过标签中的`default` 进行字段值的验证和添加
- 数据库 ORM 映射： 有些数据库 ORM（对象关系映射）库允许你使用标签来定义数据库表和结构体之间的映射关系
- HTTP 路由和处理： 在某些 Web 框架中，标签可以被用来定义 HTTP 路由规则或者请求处理逻辑

```go
package main

import (
	"fmt"
	"reflect"
)

type User struct {
	Name string `json:"name" mytag:"myName"`
	Age  int    `json:"age" mytag:"myAge"`
}

func main() {
	user := User{"Bob", 30}
	// 获取值反射
	v := reflect.ValueOf(user)
	// 获取类型
	t := v.Type()
	for i := 0; i < t.NumField(); i++ {
		// 字段
		field := t.Field(i)
		// 输出字段和值
		fmt.Println(field.Name, field.Tag.Get("json"), field.Tag.Get("mytag"), v.FieldByName(field.Name))
	}
}

// 输出：
// Name name myName Bob
// Age age myAge 30

```
___

- 参考：[GoTag标签](https://learnku.com/articles/78000)

### 13 如何判断 2 个字符串切片(slice) 是相等的？
不能直接使用`==` 这样只能说明两个slice指向同一个底层数组

- 反射比较：直接使用反射reflect.DeepEqual(a, b) 判断 a、b两个切片是否相等。但是性能较低
- 直接比较：遍历比较切片中的每一个元素（注意处理越界的情况），性能较好

```go
func StringSliceEqualBCE(a, b []string) bool {
    if len(a) != len(b) {
        return false
    }

    if (a == nil) != (b == nil) {
        return false
    }

    b = b[:len(a)]
    for i, v := range a {
        if v != b[i] {
            return false
        }
    }
    return true
}
```
___

- 参考: [Golang中如何判断两个slice是否相等？](https://zhuanlan.zhihu.com/p/615613789);[你不知道的Go之slice](https://darjun.github.io/2021/05/09/youdontknowgo/slice/)

### 14 字符串打印时，%v 和 %+v 的区别
%v 和 %+v 都可以用来打印 struct 的值，区别在于 %v 仅打印各个字段的值，%+v 还会打印各个字段的名称。
```go
type Stu struct {
	Name string
}

func main() {
	fmt.Printf("%v\n", Stu{"Tom"}) // {Tom}
	fmt.Printf("%+v\n", Stu{"Tom"}) // {Name:Tom}
}
```
|格式|描述|
|:---:|:---|
|`%v`|按值的本来值输出|
|`%+v`|在 %v 基础上，对结构体字段名和值进行展开|
|`%#v`|输出 Go 语言语法格式的值|
|`%T`|输出 Go 语言语法格式的类型和值|
|`%%`|输出 % 本体|
|`%b`|整型以二进制方式显示|
|`%o`|整型以八进制方式显示|
|`%d`|整型以十进制方式显示|
|`%x`|整型以十六进制方式显示|
|`%X`|整型以十六进制、字母大写方式显示|
|`%U`|Unicode 字符|
|`%f`|浮点数,如`%6.2f`，指定整数与小数位长度|
|`%p`|指针，十六进制方式显示|
|`%s`|字符串，指定字符串输出，`%10s`/`%-10s`，宽度为10左/右对齐|

___

- 参考：[Gofmt.Sprintf格式化字符串](https://www.runoob.com/go/go-fmt-sprintf.html);[深入理解fmt包](https://studygolang.com/articles/17400)


### 15 Go 语言中如何表示枚举值(enums)？

使用const 常量来表示枚举值 `iota` 表示0
```go
type StuType int32

const (
	Type1 StuType = iota
	Type2
	Type3
	Type4
)

func main() {
	fmt.Println(Type1, Type2, Type3, Type4) // 0, 1, 2, 3
}
```


### 16 空 struct{} 的用途
主要用于占位符号，表明这里并不需要一个值，来进行内存节省。主要用于以下场景
1. map中value占位符，用于表示set
2. 管道(channel)中占位符，仅仅表示信号
3. 基础工具类：仅仅用于继承接口声明方法

```go
package main 

// 1. 输出值
func PrintTest() {
    fmt.Println(unsafe.Sizeof(struct{}{})) // 0
}

// 2. 构造set
type Set map[string]struct{}

func SetTest() {
    set := make(Set)

	for _, item := range []string{"A", "A", "B", "C"} {
		set[item] = struct{}{}
	}
	fmt.Println(len(set)) // 3
	if _, ok := set["A"]; ok {
		fmt.Println("A exists") // A exists
	}
}

// 3. 使用管道
func ChannelTest() {
    ch := make(chan struct{}, 1)
	go func() {
		<-ch
		// do something
	}()
	ch <- struct{}{}
}

// 4. 声明方法继承
type Lamp struct{}

func (l Lamp) On() {
        println("On")

}
func (l Lamp) Off() {
        println("Off")
}

func main() {
    PrintTest()
    SetTest()
    ChannelTest()
}
```

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
