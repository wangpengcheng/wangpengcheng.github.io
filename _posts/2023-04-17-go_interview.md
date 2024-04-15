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

# Golang面试总结

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
- [Go安全指南](https://github.com/Tencent/secguide/blob/main/Go%E5%AE%89%E5%85%A8%E6%8C%87%E5%8D%97.md)
- [Go 编码规范建议](https://cloud.tencent.com/developer/article/1911268)
- [Go语言高性能编程](https://geektutu.com/post/hpg-escape-analysis.html)
- [Go语言设计与实现](https://draveness.me/golang/)
- [Go语言原本](https://golang.design/under-the-hood/)

## 基础语法

### go 的基础变量类型有哪些

| 类型分类 |类型|默认值| 描述|
|:---:|:---:|:---:|:---:|
|布尔型|`bool`|false|基础bool类型(一个字节)|
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
- init 与man函数一样，不能有入参与返回值
- init 函数由runtime初始化每个包的导入，按照解析的依赖关系，没有依赖的包最先初始化
- 包初始化顺序： 包作用域常量-> 包作用域变量-> 包内init()函数。即`import –> const –> var –> init() –> main()`
- 同包内多个`init()`：按照它们呈现给编译器的顺序被调用
    - 同文件内：按照函数顺序进行调用
    - 同包不同源文件：是根据文件名的字典序来确定
- 不同包：
    - 无相互依赖：按照main 包中import的顺序调用其包中的init函数
    - 存在依赖：调用顺序为最后被依赖的最先被初始化，如导入顺序`main > a > b >c`, 初始化顺序`c > b > a > main`

- 使用建议：应当尽量避免使用`init()`，避免`init` 依赖其他顺序。 

![](https://cdn.learnku.com/uploads/images/202007/13/1/hVMYyqi6EU.png!large)

```go
// a 包
// a.go
package a

import _ "main/b"

func init() {
	println("init a")
}

// b 包
// b.go
package b

import _ "main/c"

func init() {
	println("init b")
}

// c 包
// c.go
package c

func init() {
	println("init c")
}

// main 包
// main.go
package main

import (
	_ "main/a"
)

func init() {
	println("init main")
}

func main() {
	println("main")
}

```
对应输出：

```bash
$ go build && ./main
init c
init b
init a
init main
main
```

___

- 参考：[一张图了解 Go 语言中的 init () 执行顺序
](https://learnku.com/go/t/47135);[一文读懂Golang init函数执行顺序](https://cloud.tencent.com/developer/article/2138066)

### 02 Go 语言的局部变量分配在栈上还是堆上？

由编译器决定。编译器经过逃逸分析，发现变量作用域未超出函数范围时，分配在栈上。反之必须分配在堆上。可以使用`-gcflags=-m` 编译参数，查看逃逸分析

关键在于go的逃逸分析，go 逃逸(堆上分配)原则如下：
1. 指针逃逸：函数返回对象指针时，内存分配在堆上
2. `interface{}`动态类型逃逸：interface{}为指针别名，也会发生逃逸
3. 栈空间不足：栈使用超过操作系统内核线程栈限制(64位通常为8M,可以用`ulimit -a`查看)或者切片长度无法确定时，将存在逃逸
4. 闭包：闭包中内层函数会访问外层函数作用域。访问的外部变量会逃逸

```go
// 1. 返回指针逃逸
// d 在堆上分配内存
func createDemo(name string) *Demo {
	d := new(Demo) // 局部变量 d 逃逸到堆
	d.name = name
	return d
}

// 2. interface 逃逸
// 局部变量demo 不会发生逃逸，但是demo.name 会逃逸
func test(demo *Demo) {
	fmt.Println(demo.name)
}

// 3. 内存不足逃逸
// 3.1 超过64KB发生逃逸
func generate8192() {
	nums := make([]int, 8192) // = 64KB
	for i := 0; i < 8192; i++ {
		nums[i] = rand.Int()
	}
}
// 3.2 不确定大小逃逸
func generate(n int) {
	nums := make([]int, n) // 不确定大小
	for i := 0; i < n; i++ {
		nums[i] = rand.Int()
	}
}

// 4. 闭包逃逸
// Increase() 返回值是一个闭包函数
// 函数访问了外部变量n,n会一直存在直到in 被销毁
func IncreaseTest() {
    in := Increase()
	fmt.Println(in()) // 1
}

func Increase() func() int {
	n := 0
	return func() int {
		n++
		return n
	}
}
```
使用建议：
- 指针传递增加逃逸负担：传值会拷贝整个对象，而指针只会拷贝指针地址，对应的对象一直是同一个，传指针可以有效减少值的拷贝。但是会导致内存分配逃逸到堆中，增加GC负担。在对象频繁创建和删除的场景下，传递指针导致的 GC 开销可能会严重影响性能。
    - **对于需要修改原对象值，或占用内存比较大的结构体，选择传指针**
    - **只读的占用内存较小的结构体，直接传值能够获得更好的性能**

___

- 参考：[go内存管理](https://draveness.me/golang/docs/part3-runtime/ch07-memory/golang-memory-allocator/);[Go逃逸分析](https://geektutu.com/post/hpg-escape-analysis.html);[10分钟掌握golang内存管理机制](https://zhuanlan.zhihu.com/p/523215127)

### 03 2 个 interface 可以比较吗？
- 可以：go interface 内部实现包含了类型`T` 和值`v`。可进行比较，相等时存在如下情况
1. 均为nil(v和T都处于unset状态)
2. 类型相同，并且对应的值相等
interface底层使用2个struct表示的：eface和iface，其实际类型定义如下：

```go
// runtime/runtime2.go
// 空数据类型指针--不带方法
type eface struct {
	_type *_type  // 类型指针
	data  unsafe.Pointer // 数据值指针
}

// runtime/runtime2.go
// 非空接口--带方法
type iface struct {
    tab  *itab          // 对应的虚拟函数表
    data unsafe.Pointer //指向原始数据指针
}
```

```go
package main

type TestStruct struct{}

// Go 语言的接口类型不是任意类型
// 这里进行了类型转换，将类型转换为了*TestStruct类型
// 包含了*TestStruct的类型信息
func NilOrNot(v interface{}) bool {
	return v == nil
}

func main() {
	var s *TestStruct
	fmt.Println(s == nil)      // #=> true
	fmt.Println(NilOrNot(s))   // #=> false
}

$ go run main.go
true
false
```

___ 

- 参考: [深入理解Go语言(01): interface源码分析](https://www.cnblogs.com/jiujuan/p/12653806.html);[golang之interface接口源码解析](https://github.com/friendlyhank/toBeTopgopher/blob/master/golang/source/golang%E4%B9%8Binterface%E6%8E%A5%E5%8F%A3%E6%BA%90%E7%A0%81%E8%A7%A3%E6%9E%90.md);[go语言接口](https://github.com/friendlyhank/toBeTopgopher/blob/master/golang/source/golang%E4%B9%8Binterface%E6%8E%A5%E5%8F%A3%E6%BA%90%E7%A0%81%E8%A7%A3%E6%9E%90.md)


### 04 2 个 nil 可能不相等吗？

可能: 类型值不同时，可能不相等
- nil指针含义：接口(interface) 是对非接口值(指针/struct等)的封装，内部实现包含`T` 和`v`。一个接口为nil，仅仅表明`T`/`V` 都处于`unset` 状态(`T=nil, V = unset`)。
- 接口值比较：先比较`T`, 再比较`V`
- 接口值与非接口值比较：将非接口值转换为接口值，再进行比较

```go


type MyInterface interface {
    DoSomething()
}

type MyType struct{}

func (mt *MyType) DoSomething() {}

func test2() {
    var i MyInterface
    var t *MyType

    fmt.Println(i == nil)  // 输出：true
    fmt.Println(t == nil)  // 输出：true
 	fmt.Println(i == t)  // 输出：false
	
    i = t
    fmt.Println(i == nil)  // 输出：false
}



func test1() {
    var p *int = nil 
    // 此时i 中的T = *int, V= nil
    var i interface{} = p
    // 类型相同与值相同--相等
    fmt.Println(i == p) // true
    // p 值为nil--相等
	fmt.Println(p == nil) // true
	// 将nil 转换为接口(T=nil, V=nil), i T值不相同--不相等
    fmt.Println(i == nil) // false -- i 存在类型值
}

```

___

- 参考：[在Golang中,两个nil可能不相等吗？](https://www.iamshuaidi.com/23121.html);

### 05 简述 Go 语言GC(垃圾回收)的工作原理

常见的垃圾回收算法有**标记清除(Mark-Sweep)**和**引用计数(Reference Count)(如c++ share_ptr)**，GO语言采用标记清除算法，同时为了提高效率保证准确性，使用了三色标记法和写屏障技术。

标记清除法分为如下两个阶段：
- 标记(Mark)阶段：从根对象出发，查找并标记堆中所有存活的对象。需要使用**暂停程序(Stop The World, STW)**
- 清除(Sweep)阶段：遍历堆中的全部对象，回收未标记的垃圾对象并将回收的内存加入空闲链表
Go 为了能够异步执行，减少STW时间，采用了三色标记法。通过三色标记算法降低标记的需要时间。主要是将算法程序中的对象分为黑、白、灰三类，只有白色会被清除：
- 白色：不确定对象--可以清除
- 灰色：存活对象，子对象需要进一步处理
- 黑色：存活对象

主要清除标记流程如下：

1. 程序初始化：初始状态下，所有对象加入白色集合(需要STW)，将**根对象(指赋值器不需要通过其他对象就可以直接访问到的对象,如全局变量/协程执行栈)**标记为灰色。
2. 颜色扫描：从灰色独享集合中取出(ROOT)对象开始扫描
    a. 将直接可达的对象标记为灰色
    a. 将取出对象标记为黑色
    b. 将其指向的所有对象标记为灰色
    c. 重复上述步骤，直到无灰色对象
3. 对象清除：清除白色对象 

![初始三色标记法](https://data.eolink.com/2022-01-14/1642152085-909098-image.png)

但是三色标记法强依赖 STW(性能较差)， 否则标记阶段程序的逻辑改变对象引用关系,会影响标记结果的正确性，可能会出现如下情况：

![非STW 三色标记异常](https://data.eolink.com/2022-01-14/1642152123-524607-image.png)

为了优化性能，尽量减少STW使用，Golang 对三色标记法进行了优化。
上面的标记异常问题出现，存在两个条件
1. **白色对象被黑色对象引用** 
2. **灰色对象与白色对象之间的可达关系遭到破坏**

只要上述条件任意破坏一下，就可以保证对象不丢失。由此提出了破坏方式：
1. **强三色不变式：** 不允许黑色对象引用白色对象--破坏条件一
2. **弱三色不变式：** 黑色对象可以引用白色对象，但是白色对象的上游必须存在灰色对象--保证白色对象一定可以被扫描到

为了尝试保证上述两点，有如下方式，详细操作流程如下：

- **插入写屏障**(强三色)：当一个对象引用另外一个对象时，将另外一个对象标记为灰色。
    - ![总体机制](https://data.eolink.com/2022-01-14/1642152399-611837-image.png)
    - 由于栈上对象没有插入写机制，在扫描完成后需要对栈上空间进行STW，防止对象错误删除。如下：
    - ![](https://data.eolink.com/2022-01-14/1642152399-611837-image.png)
    - ![](https://data.eolink.com/2022-01-14/1642152408-446949-image.png)
    - ![](https://data.eolink.com/2022-01-14/1642152415-201031-image.png)
    - ![](https://data.eolink.com/2022-01-14/1642152430-572346-image.png)
    - ![](https://data.eolink.com/2022-01-14/1642152437-364823-image.png)
    - ![](https://data.eolink.com/2022-01-14/1642152443-909009-image.png)
    - **缺点：** 栈上对象无法操作, 需要对栈空间进行stw保护，然后rescan保证引用的白色对象存活。增加了GC时间
- **删除写屏障**(弱三色)：在删除引用时，如果被删除引用的对象自身为灰色或者白色，那么被标记为灰色。
    - ![删除写屏障](https://data.eolink.com/2022-01-14/1642152473-725532-image.png)
    - ![删除写屏障2](https://data.eolink.com/2022-01-14/1642152488-972239-image.png)
    - ![删除写屏障3](https://data.eolink.com/2022-01-14/1642152483-907048-image.png)
    - ![删除写屏障4](https://data.eolink.com/2022-01-14/1642152488-972239-image.png)
    - **缺点：** 中间变更的无引用对象，会被保留，存活到下一轮，降低了回收精度
        - [删除机制](https://data.eolink.com/2022-01-14/1642152499-445418-image.png)
- **混合写屏障机制(v1.8)**：栈上可达对象全部标黑(避免栈引用丢失)，堆上删除/添加对象标记为灰色。具体步骤如下
    - GC刚开始的时候，会将栈上的可达对象全部标记为黑色。
    - GC期间，任何在栈上新创建的对象，均为黑色。
    - 堆上被删除的对象标记为灰色
    - 堆上新添加的对象标记为灰色
    - ![流程图1](https://data.eolink.com/2022-01-14/1642152632-450577-image.png)
    - ![流程图2](https://data.eolink.com/2022-01-14/1642152632-450577-image.png)
    - ![流程图3](https://data.eolink.com/2022-01-14/1642152638-980225-image.png)
    - ![流程图4](https://data.eolink.com/2022-01-14/1642152643-404267-image.png)
    - ![流程图5](https://data.eolink.com/2022-01-14/1642152647-868470-image.png)
    - ![流程图6](https://data.eolink.com/2022-01-14/1642152652-807003-image.png)

最终go 使用**插入写机制**，在用户程序读取对象、创建新对象以及更新对象时，执行一段临界区代码(内存屏障)。当对象新增或者更新时，将其着色为灰色。这样即使与用户程序并发执行，对象的引用发生改变时，垃圾收集器也能正确处理了。


- 总结，
    - 完整GC阶段如下：
        - 1）标记准备(Mark Setup，需 STW)，打开写屏障(Write Barrier)
        - 2）使用三色标记法标记（Marking, 并发）
        - 3）标记结束(Mark Termination，需 STW)，关闭写屏障。
        - 4）清理(Sweeping, 并发)
    - 版本演进：
        - v1.3：采用传统采取标记-清除法，需要STW，暂停整个程序的运行。
        - v1.5：引入了三色标记法和插入写屏障机制，其中插入写屏障机制只在堆内存中生效。但在标记过程中，最后需要对栈进行STW。
        - v1.8：结合删除写屏障机制，推出了混合屏障机制，屏障限制只在堆内存中生效。避免了最后节点对栈进行STW的问题，提升了GC效率

___

- 参考：[go垃圾收集器](https://draveness.me/golang/docs/part3-runtime/ch07-memory/golang-garbage-collector/);[一文弄懂 Golang GC、三色标记、混合写屏障机制](https://community.apinto.com/d/34057-golang-gc)

### 06 函数返回局部变量的指针是否安全？

安全：Golang 会对每个变量进行逃逸分析，发现其作用域超出函数时，会直接在堆上分配内存

### 07 非接口的任意类型 T() 都能够调用 *T 的方法吗？反过来呢？
- **`T()`调用`*T`仅仅在T可寻址的情况下成立**：编译器在调用指针属主方法之前，会自动取此T值的地址。因为不是每个T都可以进行寻址，所以并非任何T值都能够调用为类型*T声明的方法。存在如下情况(编译前确定的值)时不可寻址
    - 字符串中的字节
    - map 对象中的元素（slice 对象中的元素是可寻址的，slice的底层是数组）
    - 常量
    - 包级别的函数
- `*T` 类型的值**可以调用**`T`声明方法：
    - 因为解引用指针总是合法的。
    - 每一个类型`T` 声明的方法，编译器都会为类型`*T` 自动隐式声明一个同名和同签名的方法。

```go
type T string

func (t *T) hello() {
	fmt.Println("hello")
}

func main() {
	var t1 T = "ABC"
	t1.hello() // hello
    // t2 为常量--非*T 类型
	const t2 T = "ABC"
	t2.hello() // error: cannot call pointer method on t
}
```

## 并发编程

### 01 无缓冲的 channel 和有缓冲的 channel 的区别？
- 无缓冲channel: 发送方将阻塞该信道，直到接收方从该信道收到数据为止，接收方同样需要阻塞该信号，直到发送方将数据发送到该信道中为止
- 有缓冲channel: 信号再存在缓冲中，在缓冲区用尽的情况下阻塞

可以用于进行协程交替打印字符串

```go
ch1 := make(chan struct{})
ch2 := make(chan struct{})
var wg sync.WaitGroup
wg.Add(2)

go func() {
   defer wg.Done()
   for i := 1; i <= 10; i += 2 {
      <-ch1
      fmt.Println(i)
      ch2 <- struct{}{}
   }
   <-ch1
}()

go func() {
   defer wg.Done()
   for i := 2; i <= 10; i += 2 {
      <-ch2
      fmt.Println(i)
      ch1 <- struct{}{}
}

ch1 <- struct{}{}
// 等待协程执行完毕
wg.Wait()
```

___

- 参考: [面试题 - 两个协程交替打印奇偶数（内含三种方法）](https://ayang.ink/go_%E9%9D%A2%E8%AF%95%E9%A2%98-%E4%B8%A4%E4%B8%AA%E5%8D%8F%E7%A8%8B%E4%BA%A4%E6%9B%BF%E6%89%93%E5%8D%B0%E5%A5%87%E5%81%B6%E6%95%B0%E5%86%85%E5%90%AB%E4%B8%89%E7%A7%8D%E6%96%B9%E6%B3%95/);[golang两个协程交替打印](https://juejin.cn/post/7209319092515635261);[Golang两个协程交替输出](https://studygolang.com/articles/35057?fr=sidebar)

### 02 什么是协程泄露(Goroutine Leak)？
- 定义：协程创建后长时间不释放，并且还在不断的创建新的协程，最终导致内存耗尽，程序崩溃。
- 原因：导致协程泄漏的主要场景有以下几种：
    - 缺少接收器/缺少发送器:，导致对应线程阻塞，无法正常进行退出
    - 死锁(dead lock):两个或两个以上的协程在执行过程中，由于竞争资源或者由于彼此通信而造成阻塞，这种情况下，也会导致协程被阻塞，不能退出。
  - 无限循环(infinite loops): 为了避免网络等问题，采用了无限重试的方式，发送 HTTP 请求，直到获取到数据。那如果 HTTP 服务宕机，永远不可达，导致协程不能退出，发生泄漏

```go
// 缺少接收器导致死锁
func query() int {
	ch := make(chan int)
	for i := 0; i < 1000; i++ {
		go func() { ch <- 0 }()
	}
	return <-ch
}

func queryTest() {
	for i := 0; i < 4; i++ {
		query()
		fmt.Printf("goroutines: %d\n", runtime.NumGoroutine())
	}
}
// goroutines: 1001
// goroutines: 2000
// goroutines: 2999
// goroutines: 3998


// 这里死循环请求
func request(url string, wg *sync.WaitGroup) {
    i := 0
    for {
        if _, err := http.Get(url); err == nil {
            // write to db
            break
        }
        i++
        if i >= 3 {
            break
        }
        time.Sleep(time.Second)
    }
    wg.Done()
}

func waitTest() {
    var wg sync.WaitGroup
    for i := 0; i < 1000; i++ {
        wg.Add(1)
        go request(fmt.Sprintf("https://127.0.0.1:8080/%d", i), &wg)
    }
    wg.Wait()
}
```
___

- 参考：[什么是协程泄露(Goroutine Leak)？](https://zhuanlan.zhihu.com/p/383138313);[Golang http请求忘记调用resp.Body.Close()而导致的协程泄漏问题](https://blog.csdn.net/qq_37102984/article/details/129326866)

### 03 Go 可以限制运行时操作系统线程的数量吗？

- 可以；可以使用环境变量 GOMAXPROCS 或 runtime.GOMAXPROCS(num int) 设置，例如：

```go
runtime.GOMAXPROCS(1) // 限制同时执行Go代码的操作系统线程数为 1
```
从官方文档的解释可以看到，GOMAXPROCS 限制的是同时执行用户态 Go 代码的操作系统线程的数量，但是对于被系统调用阻塞的线程数量是没有限制的。GOMAXPROCS 的默认值等于 CPU 的逻辑核数，同一时间，一个核只能绑定一个线程，然后运行被调度的协程。因此对于 CPU 密集型的任务，若该值过大，例如设置为 CPU 逻辑核数的 2 倍，会增加线程切换的开销，降低性能。对于 I/O 密集型应用，适当地调大该值，可以提高 I/O 吞吐率。

___

- 参考：[Golang 在 runtime 中的知识点有哪些？](https://www.zhihu.com/question/584554235/answer/3033310978?utm_id=0);[万字长文深入浅出 Golang Runtime](https://zhuanlan.zhihu.com/p/95056679?from=groupmessage);[go-runtime](https://pkg.go.dev/runtime)

## 代码输出

### 1. 变量与常量

1. 下面代码的输出是：

```go
func main() {
	const (
		a, b = "golang", 100
		d, e
		f bool = true
		g
	)
	fmt.Println(d, e, g)
}
// golang 100 true

```
在同一个 const group 中，如果常量定义与前一行的定义一致，则可以省略类型和值。编译时，会按照前一行的定义自动补全。即等价于：

```go
func main() {
    const (
        a, b = "golang", 100
        d, e = "golang", 100
        f bool = true
        g bool = true
    )
    fmt.Println(d, e, g)
}
```

2. 下列代码的输出是：

```go

func main() {
    const N = 100
    var x int = N

    const M int32 = 100
    var y int = M
    fmt.Println(x, y)
}

// 编译失败：cannot use M (type int32) as type int in assignment
```

Go 语言中，常量分为无类型常量和有类型常量两种，const N = 100，属于无类型常量，赋值给其他变量时，如果字面量能够转换为对应类型的变量，则赋值成功，例如，var x int = N。但是对于有类型的常量 const M int32 = 100，赋值给其他变量时，需要类型匹配才能成功，所以显示地类型转换：`var y int = int(M)`

3. 下列代码的输出是：

```go

func main() {
    var a int8 = -1
    var b int8 = -128 / a
    fmt.Println(b)
}
// -128
```

int8 能表示的数字的范围是 [-2^7, 2^7-1]，即 [-128, 127]。-128 是无类型常量，转换为 int8，再除以变量 -1，结果为 128，常量除以变量，结果是一个变量。**变量转换时允许溢出**，符号位变为1，转为补码后恰好等于 -128。
对于有符号整型，最高位是是符号位，计算机用补码表示负数。补码 = 原码取反加一。

```txt
-1 :  11111111
00000001(原码)    11111110(取反)    11111111(加一)
-128：    
10000000(原码)    01111111(取反)    10000000(加一)

-1 + 1 = 0
11111111 + 00000001 = 00000000(最高位溢出省略)
-128 + 127 = -1
10000000 + 01111111 = 11111111

```
- **正数以原码形式存在，负数以补码形式存在()**
- **正数最高位为0，负数最高位为1**
- **最高位为1的需要当作补码转换为负数**

4. 下列代码的输出是：

```go
func main() {
    const a int8 = -1
    var b int8 = -128 / a
    fmt.Println(b)
}
```

编译失败：constant 128 overflows int8

-128 和 a 都是常量，在编译时求值，-128 / a = 128，两个常量相除，结果也是一个常量，常量类型转换时不允许溢出，因而编译失败。

___

- 参考：[负数在计算机中的储存方式](https://zhuanlan.zhihu.com/p/129361870)


### 2. 作用域

下面的代码输出是

```go
func main() {
    var err error
    if err == nil {
        err := fmt.Errorf("err")
        fmt.Println(1, err)
    }
    if err != nil {
        fmt.Println(2, err)
    }
}
// 1 err
```

`:=` 表示声明并赋值，`=` 表示仅赋值。

变量的作用域是大括号，因此在第一个 `if` 语句 `if err == nil` 内部重新声明且赋值了与外部变量同名的局部变量 `err`。对该局部变量的赋值不会影响到外部的 `err`。因此第二个 `if` 语句 `if err != nil` 不成立。所以只打印了 `1 err`。


### 3. defer延迟调用

1. 下面的代码输出是

```go
type T struct{}

func (t T) f(n int) T {
    fmt.Print(n)
    return t
}

func main() {
    var t T
    defer t.f(1).f(2)
    fmt.Print(3)
}
// 132
```



defer 延迟调用时，需要保存函数指针和参数，因此链式调用的情况下，除了最后一个函数/方法外的函数/方法都会在调用时直接执行。也就是说 t.f(1) 直接执行，然后执行 fmt.Print(3)，最后函数返回时再执行 .f(2)，因此输出是 132。-- **语法解析，将defer看作一个函数，函数指针和参数，由最后一个决定**

2. 下面的代码输出是

```go

package main

import "fmt"

func f(n int) {
    // defer 执行语句与函数，在开始时就确定
    // 这里n进行了值拷贝，输出为1
    defer fmt.Println(n)
    n += 100
}

func f3(n *int) {
    // 这里*n一开始输入就确定了
    // 输出为3
    defer fmt.Println(*n)
    n += 100
}

func f2(n int) {
    // 设置输出为最终的n
    // 因此n输出为102
    defer func() {
        fmt.Println(n)
    }()
    // 这里直接输出n
    // n 在设置时就确定了，因此为2
    defer func(n int ) {
        fmt.Println(n)
    }(n)
    // 设置参数为n指针，最终输出为n的引用值
    // 102
    defer func(n *int ) {
        fmt.Println(*n)
    }(&n)
    n += 100

}

func main() {
    f(1)
    f2(2)
    a := 3
    f3(&a)
}

// 1  
// 102
// 2
// 102
// 3
```

defer 语句执行时，会将需要延迟调用的函数和参数保存起来，也就是说，执行到 defer 时，参数 n(此时等于1) 已经被保存了。因此后面对 n 的改动并不会影响延迟函数调用的结果。

3. 下列代码的输出是：

```go

func main() {
    n := 1
    if n == 1 {
        defer fmt.Println(n)
        n += 100
    }
    fmt.Println(n)
}
// 101
// 1
```
先打印 101，再打印 1。defer 的作用域是函数，而不是代码块，因此 if 语句退出时，defer 不会执行，而是等 101 打印后，整个函数返回时，才会执行。


## 基础

### 1. golang 中 make 和 new 的区别？（基本必问）

1. 作用不同：
    - new: 根据传入的类型分配一片内存空间并返回指向这片内存空间的指针
    - make: 初始化内置的数据结构，也就是我们在前面提到的切片、哈希表和 Channel，并不负责变量的产生
2. 返回值不同：new 返回的是类型的指针，make返回类型的引用
3. 使用范围不同：
    - new：用于制定类型`T` 如`struct` 类的初始化
    - make: 只能用于 slice，map，channel

___


- 参考：[Go make 和 new 的区别](https://sanyuesha.com/2017/07/26/go-make-and-new/);[5.5 make 和 new](https://draveness.me/golang/docs/part2-foundation/ch05-keyword/golang-make-and-new/)

### 2. 数组和切片的区别 （基本必问）

相同点：
- 都是顺序存储的数据结构

不同点：
1. 长度定义不同：数组是一个长度固定的数据类型，其长度在定义时就已经确定，不能动态改变；切片是一个长度可变的数据类型，其长度在定义时可以为空，也可以指定一个初始长度。
2. 内存空间分配方式不同：数组的内存空间是在定义时分配的，其大小是固定的；切片的内存空间是在运行时动态分配的，其大小是可变的，在添加时进行[自动扩容](https://draveness.me/golang/docs/part2-foundation/ch03-datastructure/golang-array-and-slice/#324-%E8%BF%BD%E5%8A%A0%E5%92%8C%E6%89%A9%E5%AE%B9)
3. 操作变更不同：当数组作为函数参数时，函数操作的是数组的一个副本，不会影响原始数组(写时拷贝)；当切片作为函数参数时，函数操作的是切片的引用，会影响原始切片。
4. 容量不同：切片还有容量的概念，它指的是分配的内存空间。

___

- 参考：[GO中数组与切片的区别](http://www.hangdaowangluo.com/archives/1938);[go数组](https://draveness.me/golang/docs/part2-foundation/ch03-datastructure/golang-array/);[Go 语言数组和切片的区别](https://www.51cto.com/article/750465.html);[数组和切片有什么异同](https://golang.design/go-questions/slice/vs-array/)

### 3. for range 的时候它的地址会发生变化么？

for range 时**地址不会发生变化**，range 会产生一个临时变量，将数组/切片/map 中的值进行拷贝复制，然后进行操作。因此地址不会发生变化，也不应该在range 中对对象进行操作。如果需要修改其值，应当使用指针进行修改，并重新赋值回写。

```go
package main

import "fmt"

type girl struct {
	Name string
	Age int
}

func main() {
	gl := make(map[string]*girl)
	studs := []girl{
		{Name: "Lili", Age: 23},
		{Name: "Lucy", Age: 24},
		{Name: "Han Mei", Age: 21},
	}

	for _, v := range studs {
        fmt.Printf("%s addr: %v\n", v.Name, &v.Age)
		gl[v.Name] = &v
	}

	for mk, mv := range gl {
		fmt.Println(mk, "=>", mv.Age)
	}
}
// main.go
// Lili addr: 0xc0000a4028
// Lili => 21
// Lucy addr: 0xc0000a4028
// Lucy => 21
// Han Mei addr: 0xc0000a4028
// Han Mei => 21

```




### 4. go defer，多个 defer 的顺序，defer 在什么时机会修改返回值？

defer 按照栈的顺序，先入后出。按照定义顺序逆序执行。defer 在返回值定义后，最终return 之前修改返回值

### 5. uint 类型溢出

uint 最大值为 2^31-1 当期超过最大值时，会从最小值开始循环。为了防止溢出可以采用以下方案：
1.  使用更大的数据类型：例如，如果你正在使用uint32，你可以尝试升级到uint64。这将提供更大的值范围，从而减少溢出的可能性。
2. 添加溢出检查：在每次运算之后，你可以检查结果是否小于任一操作数（假设我们只在正数上进行操作）。如果是这样，那么就发生了溢出。
3. 使用 math/big 包：对于非常大的数值，你也可以考虑使用 math/big 包中的 Int 类型。这个类型可以处理任意大小的数值，但是运算速度会慢一些。

### 6. 介绍 rune 类型

rune 类型是go 针对字符集产生的特殊类型，基本定义为int32 别名，长度为4个字节，可以任意字符值。用于解决多字符集编码问题(utf-8)

![字符编码](https://mmbiz.qpic.cn/mmbiz_png/rJDC5vuwJCNoUJI5HH4uiaAYjK4vIgnu7W7h9Y0LezHISIzDTxDCE6FVVxzUb6gefUYo2X5mraBO4p9uiaK0QRuA/640?wx_fmt=png)

rune 类型只是一种名称叫法，表示用来处理长度大于 1 字节（ 8 位）、不超过 4 字节（ 32 位）的字符类型。但万变不离其宗，我们使用函数时，无论传入参数的是原始字符串还是 rune，最终都是对字节进行处理。


___

- 参考：[理解go中rune数据类型](https://zhuanlan.zhihu.com/p/137339284);[详解 Go 中的 rune 类型](https://www.cnblogs.com/cheyunhua/p/16007219.html)


### 7. golang 中解析 tag 是怎么实现的？反射原理是什么？

- tag实现： golang 中的tag 是通过反射进行解析实现的，tag解析方法如下：

```go
func main() {
    type User struct {
        Name string `testTag:"name"`
        Age int `testTag:"age"`
    }
    v := refect.Valueof(user)
    t := v.Type()
    for i:=0; i < t.NumField(); i++ {
        field := t.Fieid(i)
        tagName :=  field.Tag.Get("testTag")
        fieldVal := v.FieldByName(field.Name)
        fmt.Printf("tagName %v val %v", tagName, fieldVal)
    }
}
```
- 反射原理：通过接口的隐式转换将普通类型转换为interface{}，在将interface{}转换为反射类型来实现，通过反射基础类型操作，实现各种复杂操作。

![反射流程](https://pic3.zhimg.com/v2-e0bb3510209442e76b2a244cec85b3fa_r.jpg)

- 反射三定律：
    - Golang对象可以转换成反射对象
    - 反射对象可以转换成Golang对象
    - 可寻址的reflect对象可以更新值

___

- 参考：[go反射原理](https://draveness.me/golang/docs/part2-foundation/ch04-basic/golang-reflect/);[Golang反射原理详解](https://zhuanlan.zhihu.com/p/382424874);[go语言如何实现发射](https://golang.design/go-questions/stdlib/reflect/how/)


### 8. 调用函数传入结构体时，应该传值还是指针？ （Golang 都是传值）

传入的是值，golang都是值传递，即便是指针，也进行了一次值拷贝

### 9. go 空结构体为啥内存为0

这个是go的编译时优化，因为空结构不包含任何字段，因此不能容纳任何数据。如果空结构不包含任何数据，则无法确定两个结构{} 值是否不同。它们实际上是可替代的

空结构体主要有以下几个特点：
- 零内存占用
- 地址相同
- 无状态

使用场景：
- 实现 Set 集合类型
- 用于通道信号
- 作为方法接收器

```go
// /go/src/runtime/malloc.go
// base address for all 0-byte allocations
var zerobase uintptr

func mallocgc(size uintptr, typ *_type, needzero bool) unsafe.Pointer {
   
    ······

    if size == 0 {
   
       return unsafe.Pointer(&zerobase)
    }
    ······
```

根据 malloc.go 源码的部分内容，当要分配的对象大小 size 为 0 时，会返回指向 zerobase 的指针。zerobase 是一个用于分配零字节对象的基准地址，它不占用任何实际的内存空间。

___

- 参考：[Go空结构体:零内存的魔力](https://developer.aliyun.com/article/1230762);[翻译||总结-Go语言中的空结构体(The empty struct)](https://nemo.cool/955.html)

### 10 能说说uintptr和unsafe.Pointer的区别吗？

- unsafe.Pointer还会被gc追踪而uintptr不会；
- uintptr是整形而unsafe.Pointer是指针类型，uintptr可以和任何类型的整形互转，unsafe.Pointer可以和任何类型的指针互转，然后它们俩之间可以互转。


___

参考：[GO 普通指针&unsafe.Pointer&uintptr](https://mp.weixin.qq.com/s?__biz=MzkzODY1MDQyMA==&mid=2247483656&idx=1&sn=1cc955dbf2b2a25fe93d55b6a14406db&chksm=c2fdbf53f58a36450086724482507d0f7bcf6cc204fb95b98e8a6ce5aed6c2b88b649073728b&token=1922909448&lang=zh_CN#rd);[知乎-uintptr和unsafe.Pointer区别](https://www.zhihu.com/question/584325156)


## context相关

### 1. context 结构是什么样的？
context 是go 定义的官方上下文接口，其本质是一个标准接口定义如下

```go
// context/context.go
type Context interface {
	// Deadline returns the time when work done on behalf of this context
	// should be canceled. Deadline returns ok==false when no deadline is
	// set. Successive calls to Deadline return the same results.
	Deadline() (deadline time.Time, ok bool)

	// Done returns a channel that's closed when work done on behalf of this
	// context should be canceled. Done may return nil if this context can
	// never be canceled. Successive calls to Done return the same value.
	// The close of the Done channel may happen asynchronously,
	// after the cancel function returns.
	//
	// WithCancel arranges for Done to be closed when cancel is called;
	// WithDeadline arranges for Done to be closed when the deadline
	// expires; WithTimeout arranges for Done to be closed when the timeout
	// elapses.
	//
	// Done is provided for use in select statements:
	//
	//  // Stream generates values with DoSomething and sends them to out
	//  // until DoSomething returns an error or ctx.Done is closed.
	//  func Stream(ctx context.Context, out chan<- Value) error {
	//  	for {
	//  		v, err := DoSomething(ctx)
	//  		if err != nil {
	//  			return err
	//  		}
	//  		select {
	//  		case <-ctx.Done():
	//  			return ctx.Err()
	//  		case out <- v:
	//  		}
	//  	}
	//  }
	//
	// See https://blog.golang.org/pipelines for more examples of how to use
	// a Done channel for cancellation.
	Done() <-chan struct{}

	// If Done is not yet closed, Err returns nil.
	// If Done is closed, Err returns a non-nil error explaining why:
	// Canceled if the context was canceled
	// or DeadlineExceeded if the context's deadline passed.
	// After Err returns a non-nil error, successive calls to Err return the same error.
	Err() error

	// Value returns the value associated with this context for key, or nil
	// if no value is associated with key. Successive calls to Value with
	// the same key returns the same result.
	//
	// Use context values only for request-scoped data that transits
	// processes and API boundaries, not for passing optional parameters to
	// functions.
	//
	// A key identifies a specific value in a Context. Functions that wish
	// to store values in Context typically allocate a key in a global
	// variable then use that key as the argument to context.WithValue and
	// Context.Value. A key can be any type that supports equality;
	// packages should define keys as an unexported type to avoid
	// collisions.
	//
	// Packages that define a Context key should provide type-safe accessors
	// for the values stored using that key:
	//
	// 	// Package user defines a User type that's stored in Contexts.
	// 	package user
	//
	// 	import "context"
	//
	// 	// User is the type of value stored in the Contexts.
	// 	type User struct {...}
	//
	// 	// key is an unexported type for keys defined in this package.
	// 	// This prevents collisions with keys defined in other packages.
	// 	type key int
	//
	// 	// userKey is the key for user.User values in Contexts. It is
	// 	// unexported; clients use user.NewContext and user.FromContext
	// 	// instead of using this key directly.
	// 	var userKey key
	//
	// 	// NewContext returns a new Context that carries value u.
	// 	func NewContext(ctx context.Context, u *User) context.Context {
	// 		return context.WithValue(ctx, userKey, u)
	// 	}
	//
	// 	// FromContext returns the User value stored in ctx, if any.
	// 	func FromContext(ctx context.Context) (*User, bool) {
	// 		u, ok := ctx.Value(userKey).(*User)
	// 		return u, ok
	// 	}
	Value(key any) any
}

```

go 官方包中提供了默认的emptyCtx 进行常规使用

```go
type emptyCtx int
// 执行deadline
func (*emptyCtx) Deadline() (deadline time.Time, ok bool) {
	return
}

// 执行相关操作
func (*emptyCtx) Done() <-chan struct{} {
	return nil
}

// 对应错误信息
func (*emptyCtx) Err() error {
	return nil
}

// 查询对应值
func (*emptyCtx) Value(key any) any {
	return nil
}

// 字符串
func (e *emptyCtx) String() string {
	switch e {
	case background:
		return "context.Background"
	case todo:
		return "context.TODO"
	}
	return "unknown empty Context"
}

var (
	background = new(emptyCtx)
	todo       = new(emptyCtx)
)

// Background returns a non-nil, empty Context. It is never canceled, has no
// values, and has no deadline. It is typically used by the main function,
// initialization, and tests, and as the top-level Context for incoming
// requests.
func Background() Context {
	return background
}

```



### 2. context 使用场景和用途

context 主要用于协程之间进行数据传递和信号发送，基于此的主要使用场景和用途如下：

1. 上下文传递数据：函数之间的值传递
2. 超时控制：http 定时器等超时控制、IO 耗时操作控制超时

```go
func httpRequest(ctx context.Context) {
	for {
		// 处理http请求
		select {
		case <- ctx.Done():
			fmt.Println("Request timed out")
			return
		case <- time.After(time.Second):
			fmt.Println("Loading...")
		}
	}
}

func main() {
	fmt.Println("start TestTimeoutContext")
	ctx, cancel := context.WithTimeout(context.Background(), time.Second * 3)
	defer cancel()
	httpRequest(ctx)
	time.Sleep(time.Second * 5)
}

//start TestTimeoutContext
//Loading...
//Loading...
//Request timed out
```


3. 信号发送(取消控制) 
- 控制子协程退出：goroutine发送取消信号，保证自己这个逻辑中发散出去的goroutine全部成功取消


```go

func gen(ctx context.Context) <-chan int {
	ch := make(chan int)
	go func() {
		var n int
		for {
			select {
			case ch <- n:
				n++
				time.Sleep(time.Second)
			case <-ctx.Done():
				return
			}
		}
	}()
	return ch
}

func main() {
	// 创建一个Cancel context
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()
	for n := range gen(ctx) {
		fmt.Println(n)
		if n == 5 {
			// 达到要求之后触发cancel
			cancel()
			break
		}
	}
}
//0
//1
//2
//3
//4
//5
```

___

- 参考：[Golang Context的使用场景](https://juejin.cn/post/7140872374534012964)

## channel相关

### 1. channel 是否线程安全？锁用在什么地方？

- channel 是线程安全的：
    - 设计需要：channel 本身设定为golang 中多协程通信语法糖，使用场景就是多线程，为了保证数据的一致性，必须实现线程安全。
    - 设计实现：底层实现中`hchan` 使用`mutex`互斥锁保证数据读写安全， 在对循环数组buf中的数据进行入队和出队操作时，必须先获取互斥锁，才能操作channel数据。同时使用堆上内存共享，保证通信高效性
- go 中的锁分为：
    - sync.Mutex (互斥锁)：只包含`lock()`与`unlock`的基础互斥锁
    - sync.RWMutex (读写锁):针对读多写少的环境进行。包含`RLock()/RUnlock()`(读锁)、`Lock()/Unlock()`(写锁方法)
    - sync.Map(安全锁)：并发map安全锁
    - sync.WaitGroup(删栏)：用`Add(int)/Done()` 用于增加持有次数。`Wait()` 进行持有等待 

___

参考：[go笔记记录——channel](https://blog.csdn.net/qq_52563729/article/details/126093532);[Go中的三种锁包括:互斥锁,读写锁,sync.Map的安全的锁.](https://www.kancloud.cn/yuankejishu/golang/2785000)

### 2. go channel 的底层实现原理 （数据结构）

Go中的channel是一个队列，遵循先进先出的原则，负责协程之间的通信(Go语言提倡不要通过共享内存来通信，而要通过通信来实现内存共享，CSP(CommunicatingSequentiall Process)并发模型，就是通过goroutine和channel来实现的)

通过var声明或者make函数创建的channel变量是一个存储在函数栈帧上的指针，占用8个字节，指向堆上的hchan结构体


![](https://img-blog.csdnimg.cn/4c5a4b62c9dd4522abeae054e11456f1.png)


底层数据结构如下：

```go
type hchan struct {
	qcount   uint           // 队列中的总元素个数
	dataqsiz uint           // 环形队列大小，即可存放元素的个数
	buf      unsafe.Pointer // 环形队列指针
	elemsize uint16 //每个元素的大小
	closed   uint32 //标识关闭状态
	elemtype *_type // 元素类型
	sendx    uint   // 发送索引，元素写入时存放到队列中的位置
	recvx    uint   // 接收索引，元素从队列的该位置读出
	recvq    waitq  // 等待读消息的goroutine队列
	sendq    waitq  // 等待写消息的goroutine队列

	// lock protects all fields in hchan, as well as several
	// fields in sudogs blocked on this channel.
	//
	// Do not change another G's status while holding this lock
	// (in particular, do not ready a G), as this can deadlock
	// with stack shrinking.
	lock mutex //互斥锁，chan不允许并发读写
}

type waitq struct {
	first *sudog
	last  *sudog
}

// sudog represents a g in a wait list, such as for sending/receiving
// on a channel.
//
// sudog is necessary because the g ↔ synchronization object relation
// is many-to-many. A g can be on many wait lists, so there may be
// many sudogs for one g; and many gs may be waiting on the same
// synchronization object, so there may be many sudogs for one object.
//
// sudogs are allocated from a special pool. Use acquireSudog and
// releaseSudog to allocate and free them.
type sudog struct {
	// The following fields are protected by the hchan.lock of the
	// channel this sudog is blocking on. shrinkstack depends on
	// this for sudogs involved in channel ops.

	g *g

	next *sudog
	prev *sudog
	elem unsafe.Pointer // data element (may point to stack)

	// The following fields are never accessed concurrently.
	// For channels, waitlink is only accessed by g.
	// For semaphores, all fields (including the ones above)
	// are only accessed when holding a semaRoot lock.

	acquiretime int64
	releasetime int64
	ticket      uint32

	// isSelect indicates g is participating in a select, so
	// g.selectDone must be CAS'd to win the wake-up race.
	isSelect bool

	// success indicates whether communication over channel c
	// succeeded. It is true if the goroutine was awoken because a
	// value was delivered over channel c, and false if awoken
	// because c was closed.
	success bool

	parent   *sudog // semaRoot binary tree
	waitlink *sudog // g.waiting list or semaRoot
	waittail *sudog // semaRoot
	c        *hchan // channel
}
```

___

- 参考：[channel 底层的数据结构是什么](https://golang.design/go-questions/channel/struct/)


### 3. 关闭的 channel. 有数据的 channel，再进行读. 写. 关闭会怎么样？（各类变种题型）

在Go的并发模型中，关闭channel是为了告诉接收者，发送者没有更多的数据要发送了。因此，一旦channel被关闭，就意味着不能再向其发送新的数据。这种约定帮助程序员在处理并发逻辑时，能够更好地理解数据流的生命周期。

- 关闭的channel：
    - 读：无异常，如果在关闭前，通道内部有元素，会正确读到元素的值；如果关闭前通道无元素，则会读取到通道内元素类型对应的零值。range 操作可能会导致死锁错误
    - 写/关闭：panic

- 有数据channel:
    - 读：正常读取到数据
    - 写/关闭：panic

___

- 参考：[《Go题库·2》对已经关闭的channel进行读写操作会发生什么?](https://studygolang.com/topics/15595);[对已经关闭的的chan进行读写，会怎么样？为什么？](https://www.iamshuaidi.com/23380.html)

### 4. 向 channel 发送数据和从 channel 读数据的流程是什么样的？

- 发送数据：
    1. 若等待接收队列 recvq 不为空，则缓冲区中无数据或无缓冲区，将直接从 recvq 取出 G ，并把数据写入，最后把该 G 唤醒，结束发送过程。
    2. 若缓冲区中有空余位置，则将数据写入缓冲区，结束发送过程。
    3. 若缓冲区中没有空余位置，则将发送数据写入 G，将当前 G 加入 sendq ，进入睡眠，等待被读 goroutine 唤醒。

![进行数据写入](https://golang.design/go-questions/channel/assets/2.png)

- 读取数据：
    1. 若等待发送队列 sendq 不为空，且没有缓冲区，直接从 sendq 中取出 G ，把 G 中数据读出，最后把 G 唤醒，结束读取过程；
    2. 如果等待发送队列 sendq 不为空，说明缓冲区已满，从缓冲区中首部读出数据，把 G 中数据写入缓冲区尾部，把 G 唤醒，结束读取过程；
    3. 如果缓冲区中有数据，则从缓冲区取出数据，结束读取过程；
    4. 将当前 goroutine 加入 recvq ，进入睡眠，等待被写 goroutine 唤醒；

- 关闭 channel
关闭 channel 时会将 recvq 中的 G 全部唤醒，，本该写入 G 的数据位置为 nil。将 sendq 中的 G 全部唤醒，但是这些 G 会 panic。

panic 出现的场景还有：
- 关闭值为 nil 的 channel
- 关闭已经关闭的 channel
- 向已经关闭的 channel 中写数据


___

- 参考：[从 channel 接收数据的过程是怎样的](https://golang.design/go-questions/channel/recv/);[向 channel 发送数据的过程是怎样的](https://golang.design/go-questions/channel/send/);[Go 语言 chan 实现原理，彻底搞懂 chan 读写机制](https://xie.infoq.cn/article/49526fb0dde758d663dfe0cd0)


## map相关

### 1. map 使用注意的点，并发安全？

1. map为引用类型，需要注意底层数据变换：如果两个map同时指向一个底层，那么一个map的变动会影响到另一个map
2. 初始化：map 默认为nil,需要使用make进行初始化，对nil map进行任何添加元素的操作都会触发运行时错误（panic）。因此，使用前必须先创建map，使用make函数，例如：m := make(map[string]int)。
3. map的键必须可比较： map的键可以是任何可以用==或!=操作符比较的类型：如字符串，整数，浮点数，复数，布尔等。但是，slice，map，和function类型不可以作为map的键，因为这些类型不能使用==或!=操作符进行比较。
4. map在使用过程中不保证遍历顺序：map的遍历结果顺序可能会不一样，所以在需要顺序的场合，要自行处理数据并排序。map底层使用hash桶进行数据存储，扩容过程中不保证数据有效性。不能边遍历，边修改key
5. 非线程安全：map进行的所有操作，包括读取，写入，删除，都是不安全的，也就是说，如果你在一个goroutine中修改map，同时在另一个goroutine中读取map，可能会触发“concurrent map read and map write”的错误。需要使用锁或者sync.Map 进行数据操作

___

- 参考：[请说一下Golang map 使用注意的点，以及是否并发安全？](https://www.iamshuaidi.com/23354.html)

### 2. map 循环是有序的还是无序的？

- 无序的：无序有两个关键点：
 1. map扩容无序性：map 在扩容后，会发生 key 的搬迁，原来落在同一个 bucket 中的 key，搬迁后，有些 key 就要远走高飞了（bucket 序号加上了 2^B）。而遍历的过程，就是按顺序遍历 bucket，同时按顺序遍历 bucket 中的 key。搬迁后，key 的位置发生了重大的变化，有些 key 飞上高枝，有些 key 则原地不动。这样，遍历 map 的结果就不可能按原来的顺序了。
 2. map 遍历随机性：遍历 map 时，并不是固定地从 0 号 bucket 开始遍历，每次都是从一个随机值序号的 bucket 开始遍历，并且是从这个 bucket 的一个随机序号的 cell 开始遍历。这样，即使你是一个写死的 map，仅仅只是遍历它，也不太可能会返回一个固定序列的 key/value 对了


### 3. map 中删除一个 key，它的内存会释放么？

- 不会：仅仅修改可用标记，不会真正释放内存。只有真正进行删除后，才会进行内存的释放

___

- 参考：[map 的删除过程是怎样的](https://qcrao91.gitbook.io/go/map/map-de-shan-chu-guo-cheng-shi-zen-yang-de);[面试官：map删除元素会释放内存吗](https://juejin.cn/post/7195528153905184829);[Go删除Map对内存的影响](https://www.mofan.life/2022/11/14/Go/Go%E5%88%A0%E9%99%A4Map%E5%AF%B9%E5%86%85%E5%AD%98%E7%9A%84%E5%BD%B1%E5%93%8D/)


### 4. 怎么处理对 map 进行并发访问？有没有其他方案？ 区别是什么？

1. 使用锁：使用RWLock 或者Mutex锁进行读写访问控制
2. 使用sync.Map线程安全锁:
3. 分片加锁：将这个 map 分成 n 块，每个块之间的读写操作都互不干扰，从而降低冲突的可能性。

___

- 参考：[实现map并发安全的三种方式](https://blog.csdn.net/weixin_43973689/article/details/127986224);[【Golang】 关于Go 并发之三种线程安全的 map](https://www.cnblogs.com/chenpingzhao/p/16322422.html)

### 5. nil map 和空 map 有何不同？

- nil map: 未初始化的map,不能进行读写操作
- 空map：已经初始化的map。只是没有数据，可以正常读写。
- map初始化注意点：
    - 希表中的元素数量少于或者等于 25 个时，编译器会将字面量初始化的结构体转换成对应代码，直接一次加入到hash表中
    - 超过25个：创建两个数组存储键和值，通过循环进行加入


### 6. map 的数据结构是什么？是怎么实现扩容？

#### map数据结构--hmap
Golang的map就是使用哈希表作为底层实现，map 实际上就是一个指针，指向hmap结构体。其主要数据结构如下：

```go
type hmap struct {
  count     int              // 存储的键值对数目,调用len时直接返回此值
  flags     uint8            // 状态标志（是否处于正在写入的状态等）
  B         uint8            // 桶的数目为 2^B
  noverflow uint16           // 使用的溢出桶的数量
  hash0     uint32           // 生成hash的随机数种子

  buckets    unsafe.Pointer  // bucket数组指针，数组的大小为2^B（桶）
  oldbuckets unsafe.Pointer  // 扩容阶段用于记录旧桶用到的那些溢出桶的地址
  nevacuate  uintptr         // 记录渐进式扩容阶段下一个要迁移的旧桶编号
  extra *mapextra            // 指向mapextra结构体里边记录的都是溢出桶相关的信息
}


//  buckets 指向的真实指针地址
type bmap struct {
    topbits  [8]uint8   // 高位字段，用于进行快速的hash查找
    keys     [8]keytype  // key数组
    values   [8]valuetype // val数组
    pad      uintptr // 字节对齐扩充字段
    overflow uintptr // 溢出桶指针地址
}

```

![hash map](https://golang.design/go-questions/map/assets/0.png)


#### 扩容实现

**扩容条件**
在向 map 插入新 key 的时候，会进行条件检测，符合下面这 2 个条件，就会触发扩容：
- 装载因子超过阈值，源码里定义的阈值是 6.5。
- overflow 的 bucket 数量过多：当 B 小于 15，也就是 bucket 总数 2^B 小于 2^15 时，如果 overflow 的 bucket 数量超过 2^B；当 B >= 15，也就是 bucket 总数 2^B 大于等于 2^15，如果 overflow 的 bucket 数量超过 2^15。

由此引发出两种扩容方式：
- 翻倍扩容：直接进行翻倍扩容
- 等量扩容：创建新桶保存数据，清理溢出桶


扩容过程的关键代码如下：

```go

// map扩容
func mapassign(t *maptype, h *hmap, key unsafe.Pointer) unsafe.Pointer {
	...
again:
	bucket := hash & bucketMask(h.B)
    // 已经处于扩容
	if h.growing() {
        // 执行真正的扩容函数
		growWork(t, h, bucket)
	}
	b := (*bmap)(add(h.buckets, bucket*uintptr(t.bucketsize)))
	top := tophash(hash)

    
    ...
	if !h.growing() && (overLoadFactor(h.count+1, h.B) || tooManyOverflowBuckets(h.noverflow, h.B)) {
		hashGrow(t, h)
		goto again
	}
	...
}

func growWork(t *maptype, h *hmap, bucket uintptr) {
	// make sure we evacuate the oldbucket corresponding
	// to the bucket we're about to use
	evacuate(t, h, bucket&h.oldbucketmask())

	// evacuate one more oldbucket to make progress on growing
	if h.growing() {
		evacuate(t, h, h.nevacuate)
	}
}

// overflow buckets 太多
func tooManyOverflowBuckets(noverflow uint16, B uint8) bool {
	if B < 16 {
		return noverflow >= uint16(1)<<B
	}
	return noverflow >= 1<<15
}

// 创建新桶内存空间
func hashGrow(t *maptype, h *hmap) {
	bigger := uint8(1)
    // 检查是否为装载因子过高
	if !overLoadFactor(h.count+1, h.B) {
		bigger = 0
		h.flags |= sameSizeGrow
	}
	oldbuckets := h.buckets
    // 进行数据扩容
    // 1. 创建新桶和溢出桶
    // 2. 设置oldbuckets和buckets 值
	// 为新桶分配内存
    newbuckets, nextOverflow := makeBucketArray(t, h.B+bigger, nil)
    // 扩容倍数
	h.B += bigger
	h.flags = flags
    // 设置新旧桶
	h.oldbuckets = oldbuckets
	h.buckets = newbuckets
	h.nevacuate = 0
	h.noverflow = 0

	h.extra.oldoverflow = h.extra.overflow
	h.extra.overflow = nil
	h.extra.nextOverflow = nextOverflow
}

// 进行数据桶数据迁移，将数据迁移到新桶中
// 将一个旧桶中的数据分流到两个新桶，所以它会创建两个用于保存分配上下文的 runtime.evacDst 结构体，这两个结构体分别指向了一个新桶：
// map 扩容函数
func evacuate(t *maptype, h *hmap, oldbucket uintptr) {
	// 定位老的 bucket 地址
	b := (*bmap)(add(h.oldbuckets, oldbucket*uintptr(t.bucketsize)))
	// 结果是 2^B，如 B = 5，结果为32
	newbit := h.noldbuckets()
	// key 的哈希函数
	alg := t.key.alg
	// 如果 b 没有被搬迁过--执行数据搬迁
	if !evacuated(b) {
		var (
			// 表示bucket 移动的目标地址
			x, y   *bmap
			// 指向 x,y 中的 key/val
			xi, yi int
			// 指向 x，y 中的 key
			xk, yk unsafe.Pointer
			// 指向 x，y 中的 value
			xv, yv unsafe.Pointer
		)
		// 默认是等 size 扩容，前后 bucket 序号不变
		// 使用 x 来进行搬迁
		x = (*bmap)(add(h.buckets, oldbucket*uintptr(t.bucketsize)))
		xi = 0
		xk = add(unsafe.Pointer(x), dataOffset)
		xv = add(xk, bucketCnt*uintptr(t.keysize))、

		// 如果不是等 size 扩容，前后 bucket 序号有变
		// 使用 y 来进行搬迁
		if !h.sameSizeGrow() {
			// y 代表的 bucket 序号增加了 2^B
			y = (*bmap)(add(h.buckets, (oldbucket+newbit)*uintptr(t.bucketsize)))
			yi = 0
			yk = add(unsafe.Pointer(y), dataOffset)
			yv = add(yk, bucketCnt*uintptr(t.keysize))
		}

		// 遍历所有的 bucket，包括 overflow buckets
		// b 是老的 bucket 地址
		for ; b != nil; b = b.overflow(t) {
			k := add(unsafe.Pointer(b), dataOffset)
			v := add(k, bucketCnt*uintptr(t.keysize))

			// 遍历 bucket 中的所有 cell
			for i := 0; i < bucketCnt; i, k, v = i+1, add(k, uintptr(t.keysize)), add(v, uintptr(t.valuesize)) {
				// 当前 cell 的 top hash 值
				top := b.tophash[i]
				// 如果 cell 为空，即没有 key
				if top == empty {
					// 那就标志它被"搬迁"过
					b.tophash[i] = evacuatedEmpty
					// 继续下个 cell
					continue
				}
				// 正常不会出现这种情况
				// 未被搬迁的 cell 只可能是 empty 或是
				// 正常的 top hash（大于 minTopHash）
				if top < minTopHash {
					throw("bad map state")
				}

				k2 := k
				// 如果 key 是指针，则解引用
				if t.indirectkey {
					k2 = *((*unsafe.Pointer)(k2))
				}

				// 默认使用 X，等量扩容
				useX := true
				// 如果不是等量扩容
				if !h.sameSizeGrow() {
					// 计算 hash 值，和 key 第一次写入时一样
					hash := alg.hash(k2, uintptr(h.hash0))

					// 如果有协程正在遍历 map
					if h.flags&iterator != 0 {
						// 如果出现 相同的 key 值，算出来的 hash 值不同
						if !t.reflexivekey && !alg.equal(k2, k2) {
							// 只有在 float 变量的 NaN() 情况下会出现
							if top&1 != 0 {
								// 第 B 位置 1
								hash |= newbit
							} else {
								// 第 B 位置 0
								hash &^= newbit
							}
							// 取高 8 位作为 top hash 值
							top = uint8(hash >> (sys.PtrSize*8 - 8))
							if top < minTopHash {
								top += minTopHash
							}
						}
					}

					// 取决于新哈希值的 oldB+1 位是 0 还是 1
					// 详细看后面的文章
					useX = hash&newbit == 0
				}

				// 如果 key 搬到 X 部分
				if useX {
					// 标志老的 cell 的 top hash 值，表示搬移到 X 部分
					b.tophash[i] = evacuatedX
					// 如果 xi 等于 8，说明要溢出了
					if xi == bucketCnt {
						// 新建一个 bucket
						newx := h.newoverflow(t, x)
						x = newx
						// xi 从 0 开始计数
						xi = 0
						// xk 表示 key 要移动到的位置
						xk = add(unsafe.Pointer(x), dataOffset)
						// xv 表示 value 要移动到的位置
						xv = add(xk, bucketCnt*uintptr(t.keysize))
					}
					// 设置 top hash 值
					x.tophash[xi] = top
					// key 是指针
					if t.indirectkey {
						// 将原 key（是指针）复制到新位置
						*(*unsafe.Pointer)(xk) = k2 // copy pointer
					} else {
						// 将原 key（是值）复制到新位置
						typedmemmove(t.key, xk, k) // copy value
					}
					// value 是指针，操作同 key
					if t.indirectvalue {
						*(*unsafe.Pointer)(xv) = *(*unsafe.Pointer)(v)
					} else {
						typedmemmove(t.elem, xv, v)
					}

					// 定位到下一个 cell
					xi++
					xk = add(xk, uintptr(t.keysize))
					xv = add(xv, uintptr(t.valuesize))
				} else { // key 搬到 Y 部分，操作同 X 部分
					// ……
					// 省略了这部分，操作和 X 部分相同
				}
			}
		}
		// 如果没有协程在使用老的 buckets，就把老 buckets 清除掉，帮助gc
		if h.flags&oldIterator == 0 {
			b = (*bmap)(add(h.oldbuckets, oldbucket*uintptr(t.bucketsize)))
			// 只清除bucket 的 key,value 部分，保留 top hash 部分，指示搬迁状态
			if t.bucket.kind&kindNoPointers == 0 {
				memclrHasPointers(add(unsafe.Pointer(b), dataOffset), uintptr(t.bucketsize)-dataOffset)
			} else {
				memclrNoHeapPointers(add(unsafe.Pointer(b), dataOffset), uintptr(t.bucketsize)-dataOffset)
			}
		}
	}

	// 更新搬迁进度
	// 如果此次搬迁的 bucket 等于当前进度
	if oldbucket == h.nevacuate {
		// 进度加 1
		h.nevacuate = oldbucket + 1
		// Experiments suggest that 1024 is overkill by at least an order of magnitude.
		// Put it in there as a safeguard anyway, to ensure O(1) behavior.
		// 尝试往后看 1024 个 bucket
		stop := h.nevacuate + 1024
		if stop > newbit {
			stop = newbit
		}
		// 寻找没有搬迁的 bucket
		for h.nevacuate != stop && bucketEvacuated(t, h, h.nevacuate) {
			h.nevacuate++
		}
		
		// 现在 h.nevacuate 之前的 bucket 都被搬迁完毕
		
		// 所有的 buckets 搬迁完毕
		if h.nevacuate == newbit {
			// 清除老的 buckets
			h.oldbuckets = nil
			// 清除老的 overflow bucket
			// 回忆一下：[0] 表示当前 overflow bucket
			// [1] 表示 old overflow bucket
			if h.extra != nil {
				h.extra.overflow[1] = nil
			}
			// 清除正在扩容的标志位
			h.flags &^= sameSizeGrow
		}
	}
}

```

- 哈希在存储元素过多时会触发扩容操作，每次都会将桶的数量翻倍，扩容过程不是原子的，而是通过 runtime.growWork 增量触发的，
- 在扩容期间访问哈希表时会使用旧桶，向哈希表写入数据时会触发旧桶元素的分流。
- 除了这种正常的扩容之外，为了解决大量写入、删除造成的内存泄漏问题，哈希引入了 sameSizeGrow 这一机制，在出现较多溢出桶时会整理哈希的内存减少空间的占用。

**扩容流程**
1. 使用`hashGrow` 进行新buckets分配。将老buckets挂载到`oldbuckets`。读取时直接从oldbuckets进行数据读取。
2. 写入/删除时：因为`oldbuckets`存在，触发`evacuate`开始进行数据复制--这里每次均只进行一次同搬迁
3. 根据扩容类型：存在数据扩容时，key进行重hash，将数据和元素拷贝到新的bucket中
4. 清除`overflow bucket`, 让其被GC回收
5. 检查`oldbuckets`数据是否都被清除，清除完成。将`oldbuckets`设置为nil。方便进行GC回收

___

- 参考：[map 的扩容过程是怎样的](https://qcrao91.gitbook.io/go/map/map-de-kuo-rong-guo-cheng-shi-zen-yang-de);[进阶】Golang中map的数据结构是什么？是怎么实现扩容？](https://www.getcoder.cn/archives/-jin-jie-golang-zhong-map-de-shu-ju-jie-gou-shi-shen-me--shi-zen-me-shi-xian-kuo-rong-);[Go｜map底层实现、扩容规则、特性](https://blog.csdn.net/qq_44577070/article/details/129770410);[Go Map 扩容](https://draveness.me/golang/docs/part2-foundation/ch03-datastructure/golang-hashmap/#%E6%89%A9%E5%AE%B9)

## GMP相关

### 1. 什么是 GMP？（必问）

GMP模型是go 调度器的核心模型，go 通过g,m,p的基础结构体实现`goroutine` 的调度。同时包含全局可运行队列（GRQ）和本地可运行队列（LRQ）。 LRQ 存储本地（也就是具体的 P）的可运行 goroutine，GRQ 存储全局的可运行 goroutine，这些 goroutine 还没有分配到具体的 P。


![基础模式](https://ask.qcloudimg.com/http-save/yehe-6900791/c781d4d1154dc4c38b02064888e6927b.png)


![GMP模型](https://golang.design/go-questions/sched/assets/9.png)
 

G：goroutine 协程，是用户态的轻量级线程。为go中为有栈协程，会保存 CPU 寄存器的值
M: machine 表示系统线程。
P: processor 处理器上下文用于连接G和P，数量默认等于开机器的cpu核心数，若想调小，可以通过 GOMAXPROCS 这个环境变量设置。

M 会从与它绑定的 P 的本地队列获取可运行的 G，也会从 network poller 里获取可运行的 G，还会从其他 P 偷 G。

其关系图片如下：

![GPM关系](https://golang.design/go-questions/sched/assets/14.png)

![GPM关系图](https://img.draveness.me/2020-02-02-15805792666185-go-numa-scheduler-architecture.png)

___

- 参考：[GPM是什么](https://golang.design/go-questions/sched/gpm/);[go 调度器](https://draveness.me/golang/docs/part3-runtime/ch06-concurrency/golang-goroutine/);[说一下 GMP 模型的原理](https://go-interview.iswbm.com/c02/c02_07.html)

### 2. 进程. 线程. 协程有什么区别？

详见：[协程与线程区别](https://wangpengcheng.github.io/2019/12/17/baidu_interview_prepare/#418-%E8%AF%B7%E4%BD%A0%E6%9D%A5%E8%AF%B4%E4%B8%80%E8%AF%B4%E5%8D%8F%E7%A8%8B)

### 3. 抢占式调度是如何抢占的？

#### 3.1 基于协作的抢占式调度

Go 语言会在分段栈的机制上实现抢占调度，利用编译器在分段栈上插入的函数，所有 Goroutine 在函数调用时都有机会进入运行时检查是否需要执行抢占。主要工作原理如下：


1. 编译器会在调用函数前插入 `runtime.morestack`；
2. Go 语言运行时会在垃圾回收暂停程序、系统监控发现 `Goroutine` 运行超过 10ms 时发出抢占请求 `StackPreempt`；
3. 当发生函数调用时，可能会执行编译器插入的 `runtime.morestack`，它调用的 `runtime.newstack` 会检查 Goroutine 的 `stackguard0` 字段是否为 `StackPreempt`;
4. 如果 `stackguard0` 是 `StackPreempt`，就会触发抢占让出当前线程；


#### 3.2 基于信号的抢占式调度


![](https://ask.qcloudimg.com/http-save/yehe-6900791/3bde101235263c6efc7f4f52464b7246.png)


Goroutine 在 system call 和 channel call 时都可能发生阻塞，当程序发生 system call，M 会发生阻塞，同时唤起（或创建）一个新的 M 继续执行其他的 G

当程序发起一个 channel call，程序可能会阻塞，但不会阻塞 M,G 的状态会设置为 waiting，M 继续执行其他的 G, 当 G 的调用完成，会有一个可用的 M 继续执行它

整体抢占逻辑如下：
1. M1 发送中断信号（signalM(mp, sigPreempt)）
2. M2 收到信号，操作系统中断其执行代码，并切换到信号处理函数（sighandler(signum, info, ctxt, gp)）
3. M2 修改执行的上下文，并恢复到修改后的位置（asyncPreempt）
4. 重新进入调度循环进而调度其他 Goroutine（preemptPark 和 gopreempt_m）

主要处理流程如下：

1. 程序启动时，在 `runtime.sighandler` 中注册 `SIGURG` 信号的处理函数 `runtime.doSigPreempt`；
2. 在触发垃圾回收的栈扫描时会调用 `runtime.suspendG` 挂起 `Goroutine`，该函数会执行下面的逻辑：
    1. 将 `_Grunning` 状态的 `Goroutine` 标记成可以被抢占，即将 `preemptStop` 设置成 true；
    2. 调用 `runtime.preemptM` 触发抢占；
3. `runtime.preemptM` 会调用 `runtime.signalM` 向线程发送信号 `SIGURG`；
4. 操作系统会中断正在运行的线程并执行预先注册的信号处理函数 `runtime.doSigPreempt`；
5. `runtime.doSigPreempt` 函数会处理抢占信号，获取当前的 SP 和 PC 寄存器并调用`runtime.sigctxt.pushCall`；
6. `runtime.sigctxt.pushCall` 会修改寄存器并在程序回到用户态时执行 `runtime.asyncPreempt`；
7. 汇编指令 `runtime.asyncPreempt` 会调用运行时函数 `runtime.asyncPreempt2`；
8. `runtime.asyncPreempt2` 会调用 `runtime.preemptPark`；
9. `runtime.preemptPark` 会修改当前 Goroutine 的状态到 _Gpreempted 并调用 `runtime.schedule` 让当前函数陷入休眠并让出线程，调度器会选择其它的 `Goroutine` 继续执行；

___

- 参考：[抢占式调度器](https://draveness.me/golang/docs/part3-runtime/ch06-concurrency/golang-goroutine/#%e6%8a%a2%e5%8d%a0%e5%bc%8f%e8%b0%83%e5%ba%a6%e5%99%a8);[工作窃取](https://golang.design/go-questions/sched/work-steal/);[抢占式调度](https://tiancaiamao.gitbooks.io/go-internals/content/zh/05.5.html);[抢占式调度](https://golang.design/under-the-hood/zh-cn/part2runtime/ch06sched/preemption/#682-)

### 4. M 和 P 的数量问题？

M：Thread，也就是操作系统线程，go runtime 最多允许创建 10000 个操作系统线程，超过了就会抛出异常。最多只会有 GOMAXPROCS 个活跃线程能够正常运行

P：Processor，处理器，数量默认等于开机器的cpu核心数，若想调小，可以通过 GOMAXPROCS 这个环境变量设置。


### 5. goroutine 调度时机有哪些?

|情形|说明|
|:---:|:---|
|使用关键字`go`|go 创建一个新的 goroutine，Go scheduler 会考虑调度|
|GC|由于进行 GC 的 goroutine 也需要在 M 上运行，因此肯定会发生调度。当然，Go scheduler 还会做很多其他的调度，例如调度不涉及堆访问的 goroutine 来运行。GC 不管栈上的内存，只会回收堆上的内存|
|系统调用|当 goroutine 进行系统调用时，会阻塞 M，所以它会被调度走，同时一个新的 goroutine 会被调度上来|
|内存同步访问|atomic，mutex，channel 操作等会使 goroutine 阻塞，因此会被调度走。等条件满足后（例如其他 goroutine 解锁了）还会被调度上来继续运行|

___

- 参考：[goroutine 调度时机有哪些](https://golang.design/go-questions/sched/when/)

## 锁相关

### 1. 除了 mutex 以外还有那些方式安全读写共享变量？

1. Channel: 使用channel 来进行消息发送，来读写共享变量。channel 本身就使用了mutex。保证线程安全
2. 读写锁：使用读写锁可以有效进行变量共享
3. 原子操作：使用`atomic` 包中的数值和bool 类型，可以使用原子操作读写共享变量。但是需要注意操作顺序问题

```go
var sharedVar int32

// Writer
go func() {
   atomic.StoreInt32(&sharedVar, 10)
}()

// Reader
go func() {
   fmt.Println(atomic.LoadInt32(&sharedVar))
}()
```
4. sync.Once：如果你的共享变量只需要被初始化一次，那么可以使用sync.Once来确保初始化的并发安全性。
5. 内存屏障：go语言中可以使用`sync.MemoryBarrier`设置内存屏障
6. sync.WaitGroup: 使用等待信号，也可以实现共享变量的读写

___

- [Golang中除了加Mutex锁以外还有哪些方式可以安全读写共享变量？](https://www.iamshuaidi.com/23382.html);[一文读懂原子操作、内存屏障、锁(偏向锁、轻量级锁、重量级锁、自旋锁)、Disruptor、Go Context之上半部分](https://www.cnblogs.com/dojo-lzz/p/16183006.html);[Go 并发编程核心篇 —— 内存可见性](https://www.qiyacloud.cn/2020/12/2020-12-28-0/)

### 2. Go 如何实现原子操作？

go中的原子操作，主要是通过CAS CPU原子性指令来实现。CAS操作修改共享变量时候不需要对共享变量加锁，而是通过类似乐观锁的方式进行检查，本质还是不断的占用CPU 资源换取加锁带来的开销（比如上下文切换开销）。

原子操作由底层硬件支持，而锁则由操作系统的调度器实现。锁应当用来保护一段逻辑，对于一个变量更新的保护，原子操作通常会更有效率，并且更能利用计算机多核的优势，如果要更新的是一个复合对象，则应当使用atomic.Value封装好的实现。

__

- 参考：[Go语言的原子操作atomic](https://www.cnblogs.com/ricklz/p/13648859.html)


### 3. Mutex 是悲观锁还是乐观锁？悲观锁. 乐观锁是什么？

mutex 是悲观锁，会在一开始就尝试进行锁的持有。但是go为了防止最多个锁抢占导致的饿死现象，设置了饥饿模式。当线程最后一个进入时，进入到饥饿模式中。

**加锁流程**
互斥锁的加锁过程比较复杂，它涉及自旋、信号量以及调度等概念：

- 如果互斥锁处于初始化状态，会通过置位 mutexLocked 加锁；
- 如果互斥锁处于 mutexLocked 状态并且在普通模式下工作，会进入自旋，执行 30 次 PAUSE 指令消耗 CPU 时间等待锁的释放；
- 如果当前 Goroutine 等待锁的时间超过了 1ms，互斥锁就会切换到饥饿模式；
- 互斥锁在正常情况下会通过 runtime.sync_runtime_SemacquireMutex 将尝试获取锁的 Goroutine 切换至休眠状态，等待锁的持有者唤醒；
- 如果当前 Goroutine 是互斥锁上的最后一个等待的协程或者等待的时间小于 1ms，那么它会将互斥锁切换回正常模式；

**解锁流程**

互斥锁的解锁过程与之相比就比较简单，其代码行数不多、逻辑清晰，也比较容易理解：

- 当互斥锁已经被解锁时，调用 sync.Mutex.Unlock 会直接抛出异常；
- 当互斥锁处于饥饿模式时，将锁的所有权交给队列中的下一个等待者，等待者会负责设置 mutexLocked 标志位；
- 当互斥锁处于普通模式时，如果没有 Goroutine 等待锁的释放或者已经有被唤醒的 Goroutine 获得了锁，会直接返回；在其他情况下会通过 sync.runtime_Semrelease 唤醒对应的 Goroutine；

___

- 参考：[面试必备之乐观锁与悲观锁](https://zhuanlan.zhihu.com/p/40211594);[go同步原语](https://draveness.me/golang/docs/part3-runtime/ch06-concurrency/golang-sync-primitives/)

### 4. Mutex 有几种模式？
mutex 有两种模式，普通模式与饥饿模式
- 普通模式：正常进行锁的持有与释放的模式，锁的等待者会按照先进先出的顺序获取锁。没有获取到锁的协程会尝试自旋等待。
- 饥饿模式：一旦协程超过1ms没有获取到锁，就切换到饥饿模式。让新的协程直接在队列末尾等待，防止其自旋导致资源浪费。同时锁释放后也会直接将锁移交给下一个协程，避免重复释放和加锁，导致性能损耗

饥饿模式的能避免 Goroutine 由于陷入等待无法获取锁而造成的高尾延时。


### 5. goroutine 的自旋占用资源如何解决

自旋锁是指当一个线程在获取锁的时候，如果锁已经被其他线程获取，那么该线程将循环等待，然后不断地判断是否能够被成功获取，直到获取到锁才会退出循环。

自旋条件：

1. 锁已被占用，并且锁不处于饥饿模式。
2. 积累的自旋次数小于最大自旋次数（active_spin=4）。
3. CPU 核数大于 1。
4. 有空闲的 P。
5. 当前 Goroutine 所挂载的 P 下，本地待运行队列为空。

mutex 会让当前的 goroutine 去空转 CPU，在空转完后再次调用 CAS 方法去尝试性的占有锁资源，直到不满足自旋条件，则最终会加入到等待队列里。

___

- 参考：[goroutine 的自旋占用资源如何解决？](https://www.kancloud.cn/lyj18810711213/golang/3027774#goroutine__50)

### 6. sync.Pool 有什么作用

对于很多需要重复分配、回收内存的地方，sync.Pool 是一个很好的选择。频 繁地分配、回收内存会给 GC 带来一定的负担，严重的时候会引起 CPU 的毛 刺。而 sync.Pool 可以将暂时将不用的对象缓存起来，待下次需要的时候直 接使用，不用再次经过内存分配，复用对象的内存，减轻 GC 的压力，提升系 统的性能。

___

- 参考：[sync.Pool 有什么用](https://www.kancloud.cn/lyj18810711213/golang/3027774#syncPool__61)

### 7. RWMutex 实现 与使用注意

**RWMutex实现**
RWMutex 主要通过Mutex的封装进行实现，其主要结构体如下：

```go
type RWMutex struct {
	w           Mutex    // 统一写锁
	writerSem   uint32   // 写等待信号
	readerSem   uint32   // 读等待信号
	readerCount int32    // 当前正在执行的读操作统计
	readerWait  int32    // 当写操作被阻塞时等待的读操作个数；
}

// 写锁
func (rw *RWMutex) Lock() {
    // 1. 持有写锁
	rw.w.Lock()
    // 2. 阻塞读操作
    // 2.1 将读者设置为负数--避免新增读者
    // 2.2 统计当前读者数量
	r := atomic.AddInt32(&rw.readerCount, -rwmutexMaxReaders) + rwmutexMaxReaders
    // 3. 存在其它正在读的--等待其释放
	if r != 0 && atomic.AddInt32(&rw.readerWait, r) != 0 {
        // 等待writerSem信号将其唤醒
		runtime_SemacquireMutex(&rw.writerSem, false, 0)
	}
    // 成功持有返回
}

// 解除写锁
func (rw *RWMutex) Unlock() {
    // 函数将 readerCount 变回正数，释放读锁；
	r := atomic.AddInt32(&rw.readerCount, rwmutexMaxReaders)
	if r >= rwmutexMaxReaders {
		throw("sync: Unlock of unlocked RWMutex")
	}
    // 遍历读者
	for i := 0; i < int(r); i++ {
		// 发送信号，唤醒对应读者
        runtime_Semrelease(&rw.readerSem, false, 0)
	}
	rw.w.Unlock()
}


// 读锁
func (rw *RWMutex) RLock() {
    // 当前读者数++&&检查是否存在写者
	if atomic.AddInt32(&rw.readerCount, 1) < 0 {
        // 为负数表示存在写入，需要等待锁释放
        // 休眠等待读锁信号
		runtime_SemacquireMutex(&rw.readerSem, false, 0)
	}
}

// 解除读锁
func (rw *RWMutex) RUnlock() {
    // 减少读者数目
	if r := atomic.AddInt32(&rw.readerCount, -1); r < 0 {
		// 返回负数 — 其他 Goroutine 获得了写锁
        // 进行读锁慢释放
        rw.rUnlockSlow(r)
	}
}

func (rw *RWMutex) rUnlockSlow(r int32) {
	if r+1 == 0 || r+1 == -rwmutexMaxReaders {
		throw("sync: RUnlock of unlocked RWMutex")
	}
    // 减少读者等待数 && 判断是否没有读者
	if atomic.AddInt32(&rw.readerWait, -1) == 0 {
        // 唤醒写者进行工作
		runtime_Semrelease(&rw.writerSem, false, 1)
	}
}

```

虽然读写互斥锁 sync.RWMutex 提供的功能比较复杂，但是因为它建立在 sync.Mutex 上，所以实现会简单很多。我们总结一下读锁和写锁的关系：

- 调用 sync.RWMutex.Lock 尝试获取写锁时；
    - 每次 sync.RWMutex.RUnlock 都会将 readerCount 其减一，当它归零时该 Goroutine 会获得写锁；
    - 将 readerCount 减少 rwmutexMaxReaders 个数以阻塞后续的读操作；
- 调用 sync.RWMutex.Unlock 释放写锁时，会先通知所有的读操作，然后才会释放持有的互斥锁；

读写互斥锁在互斥锁之上提供了额外的更细粒度的控制，能够在读操作远远多于写操作时提升性能。

**使用注意事项**

1. RWMutex 是单写多读锁，该锁可以加多个读锁或者一个写锁
2. 读锁占用的情况下会阻止写，不会阻止读，多个 Goroutine 可以同时获取 读锁
3. 写锁会阻止其他 Goroutine（无论读和写）进来，整个锁由该 Goroutine 独占
4. 适用于读多写少的场景
5. RWMutex 类型变量的零值是一个未锁定状态的互斥锁
6. RWMutex 在首次被使用之后就不能再被拷贝
7. RWMutex 的读锁或写锁在未锁定状态，解锁操作都会引发 panic
8. RWMutex 的一个写锁去锁定临界区的共享资源，如果临界区的共享资源已 被（读锁或写锁）锁定，这个写锁操作的 goroutine 将被阻塞直到解锁
9. RWMutex 的读锁不要用于递归调用，比较容易产生死锁
10. RWMutex 的锁定状态与特定的 goroutine 没有关联。一个 goroutine 可 以 RLock（Lock），另一个 goroutine 可以 RUnlock（Unlock）
11. 写锁被解锁后，所有因操作锁定读锁而被阻塞的 goroutine 会被唤醒，并 都可以成功锁定读锁
12. 读锁被解锁后，在没有被其他读锁锁定的前提下，所有因操作锁定写锁而 被阻塞的 Goroutine，其中等待时间最长的一个 Goroutine 会被唤醒

___

- 参考：[RWMutex](https://draveness.me/golang/docs/part3-runtime/ch06-concurrency/golang-sync-primitives/#rwmutex);[RWMutex 实现](https://www.kancloud.cn/lyj18810711213/golang/3027774#syncPool__61)


## 并发相关

### 1. 怎么控制并发数？

1. 使用`channel`: 使用`channel`在协程创建时进行阻塞。
2. 使用协程池：目前有很多第三方库实现了协程池，可以很方便地用来控制协程的并发数量，如:[Jeffail/tunny](https://github.com/Jeffail/tunny),[panjf2000/ants](https://github.com/panjf2000/ants)

```go
package main

import (
	"log"
	"time"

	"github.com/Jeffail/tunny"
)

func main() {
    // 创建一个大小为三的协程池
    // 并定义函数
	pool := tunny.NewFunc(3, func(i interface{}) interface{} {
		log.Println(i)
		time.Sleep(time.Second)
		return nil
	})
    // 关闭对应协程池
	defer pool.Close()
    // 将参数i传递给协程池定义好的worker处理
	for i := 0; i < 10; i++ {
		go pool.Process(i)
	}
    // 等待一段时间
	time.Sleep(time.Second * 4)
}
```
3. 调整系统资源上限：
可以直接调整对应的系统资源的上限
- ulimit：使用 `ulimit -n 999999` 调整最大的文件句柄数
- 虚拟内存：使用`mkswap /mnt/.swapfile` 创建交换分区，增加内存上限

___

- 参考：[控制协程(goroutine)的并发数量](https://geektutu.com/post/hpg-concurrency-control.html)


### 2. 多个 goroutine 对同一个 map 写会 panic，异常是否可以用 defer 捕获？

**不能**使用defer 进行recover 捕获

- go语言中的错误分为三种：
    - error: 常用错误，直接由业务进行处理
    - panic: 异常，数组越界、空指针等都会触发panic。也可以由业务代码主动触发。可以被recover 进行捕获
    - fatal error : 由系统触发。这类错误一般都是跟系统资源相关的。程序无法从这类错误中恢复正常。如：fatal error 就是无法从系统申请内存。

- map 并发读写会触发fatal error
    - map会检测是否存在并发写：并发写入时 map panic 捕获意义不大，在Go 1.6 之后直接抛出fatal error
    - 如果检测到并发写会调用runtime.throw()，无法被recover()，直接GG
    - 如果要并发写map必须在业务层面上加锁（sync.Mutex或sync.RWMutext）或使用sync.Map等同步容器

- 无法捕获的异常(throw):
    - 内存溢出(`runtime: out of memory`): alloc调用内存不足时，直接throw
    - map并发读写：map的并发读写也会导致throw。避免异常问题难以排查
    - 内存耗尽(`fatal error: stack overflow`): 创建协程过多会导致内存耗尽
    - `go nil`: go关键字会检查，传入函数，如果为nil会直接crash。
    - 永久阻塞：go 检测出没有 goroutine 可以运行了，就会直接将程序 crash 掉

```go
func main() {
	defer errorHandler()
	go func() {
		for true {
			fmt.Println("alive")
			time.Sleep(time.Second*1)
			select {}
		}
	}()
	<-make(chan int)
}

// fatal error: all goroutines are asleep - deadlock!
```

- 可以被捕获的异常：
    - 数组(slice)下标越界(`index out of range`):数组下标越界时，可以正常抛出异常
    - 空指针异常：常规的空指针异常也会可以被正常捕获
    - 往已经close的chan中发送数据:
    - 类型断言错误：

___

- 参考: [Go 并发写map产生错误能够通过recover()恢复吗？](https://juejin.cn/post/7053109648223633438);[Go 语言 map 的并发安全问题](https://taoshu.in/go/go-map-concurrent-misue.html#google_vignette);[探究 Go 源码中 panic & recover 有哪些坑？](https://www.cnblogs.com/luozhiyun/p/15585415.html)

### 3. 如何优雅的实现一个 goroutine 池（百度. 手写代码）

线程池主要包含如下对象：
- 线程池管理器：线程池的统一管理抽象，包含创建线程、启动线程、调配任务
- 执行worker： 主要的工作执行线程、
- 工作队列：待处理的函数队列，等待调度处理
- 调度进程(非必须)：统一的调度进程，从工作队列中选取对应的任务进行处理

主要处理逻辑与核心操作如下：


1. 检查当前 Worker 队列中是否有可用的 Worker，如果有，取出执行当前的 task；
2. 没有可用的 Worker，判断当前在运行的 Worker 是否已超过该 Pool 的容量：{是 —> 再判断工作池是否为非阻塞模式：[是 ——> 直接返回 nil，否 ——> 阻塞等待直至有 Worker 被放回 Pool]，否 —> 新开一个 Worker（goroutine）处理}；
3. 每个 Worker 执行完任务之后，放回 Pool 的队列中等待。

![核心调度流程](https://res.strikefreedom.top/static_res/blog/figures/66396519-7ed66e00-ea0c-11e9-9c1a-5ca54bbd61eb.png)


```go
type sig struct{}
 
// 定义基础执行函数
type f func() error
 
// Pool accept the tasks from client,it limits the total
// of goroutines to a given number by recycling goroutines.
// 协程池
type Pool struct {
	// capacity of the pool.
	capacity int32  // 支持容量
 
	// running is the number of the currently running goroutines.
	running int32  // 当前运行的数量
 
	// expiryDuration set the expired time (second) of every worker.
	expiryDuration time.Duration  // 每个worker的过期时间
 
	// workers is a slice that store the available workers.
	workers []*Worker // 运行队列
 
	// release is used to notice the pool to closed itself.
	release chan sig     // 是否释放
 
	// lock for synchronous operation.
	lock sync.Mutex // 异步锁，用于提交队列
 
	once sync.Once  // 用于执行一次
}

// NewPool generates a instance of ants pool
func NewPool(size int) (*Pool, error) {
	return NewTimingPool(size, DefaultCleanIntervalTime)
}
 
// NewTimingPool generates a instance of ants pool with a custom timed task
func NewTimingPool(size, expiry int) (*Pool, error) {
	if size <= 0 {
		return nil, ErrInvalidPoolSize
	}
	if expiry <= 0 {
		return nil, ErrInvalidPoolExpiry
	}
    // 初始化默认pool 
	p := &Pool{
		capacity:       int32(size),
		freeSignal:     make(chan sig, math.MaxInt32),
		release:        make(chan sig, 1),
		expiryDuration: time.Duration(expiry) * time.Second,  // 设置超时时间
	}
	// 启动定期清理过期worker任务，独立goroutine运行，
	// 进一步节省系统资源
	p.monitorAndClear()
	return p, nil
}

// Submit submit a task to pool
func (p *Pool) Submit(task f) error {
	if len(p.release) > 0 {
		return ErrPoolClosed
	}
	w := p.getWorker()
    // 将任务写入到worker中
	w.task <- task
	return nil
}

// getWorker returns a available worker to run the tasks.
func (p *Pool) getWorker() *Worker {
	var w *Worker
	// 标志变量，判断当前正在运行的worker数量是否已到达Pool的容量上限
	waiting := false
	// 加锁，检测队列中是否有可用worker，并进行相应操作
	p.lock.Lock()
	idleWorkers := p.workers
	n := len(idleWorkers) - 1
	// 当前队列中无可用worker
	if n < 0 {
		// 判断运行worker数目已达到该Pool的容量上限，置等待标志
		waiting = p.Running() >= p.Cap()
  
	// 当前队列有可用worker，从队列尾部取出一个使用
	} else {
		w = idleWorkers[n]
		idleWorkers[n] = nil
		p.workers = idleWorkers[:n]
	}
	// 检测完成，解锁
	p.lock.Unlock()
	// Pool容量已满，新请求等待
	if waiting {
		// 利用锁阻塞等待直到有空闲worker
		for {
			p.lock.Lock()
			idleWorkers = p.workers
			l := len(idleWorkers) - 1
			if l < 0 {
				p.lock.Unlock()
				continue
			}
			w = idleWorkers[l]
			idleWorkers[l] = nil
			p.workers = idleWorkers[:l]
			p.lock.Unlock()
			break
		}
	// 当前无空闲worker但是Pool还没有满，
	// 则可以直接新开一个worker执行任务
	} else if w == nil {
		w = &Worker{
			pool: p,
			task: make(chan f, 1),
		}
		w.run()
        // 运行worker数加一
		p.incRunning()
	}
	return w
}

// worker 回收 
func (p *Pool) putWorker(worker *Worker) {
	// 写入回收时间，亦即该worker的最后一次结束运行的时间
	worker.recycleTime = time.Now()
	p.lock.Lock()
	p.workers = append(p.workers, worker)
	p.lock.Unlock()
}

// ReSize change the capacity of this pool
func (p *Pool) ReSize(size int) {
	if size == p.Cap() {
		return
	}
	atomic.StoreInt32(&p.capacity, int32(size))
	diff := p.Running() - size
	if diff > 0 {
		for i := 0; i < diff; i++ {
			p.getWorker().task <- nil
		}
	}
}

// 清理过期worker
// clear expired workers periodically.
func (p *Pool) periodicallyPurge() {
	heartbeat := time.NewTicker(p.expiryDuration)
	for range heartbeat.C {
		currentTime := time.Now()
		p.lock.Lock()
		idleWorkers := p.workers
		if len(idleWorkers) == 0 && p.Running() == 0 && len(p.release) > 0 {
			p.lock.Unlock()
			return
		}
		n := 0
		for i, w := range idleWorkers {
			if currentTime.Sub(w.recycleTime) <= p.expiryDuration {
				break
			}
			n = i
			w.task <- nil
			idleWorkers[i] = nil
		}
		n++
		if n >= len(idleWorkers) {
			p.workers = idleWorkers[:0]
		} else {
			p.workers = idleWorkers[n:]
		}
		p.lock.Unlock()
	}
}
```

worker.go 

```go
// Worker is the actual executor who runs the tasks,
// it starts a goroutine that accepts tasks and
// performs function calls.
type Worker struct {
	// pool who owns this worker.
	pool *Pool
 
	// task is a job should be done.
	task chan f
 
	// recycleTime will be update when putting a worker back into queue.
	recycleTime time.Time
}
 
// run starts a goroutine to repeat the process
// that performs the function calls.
func (w *Worker) run() {
	go func() {
		// 循环监听任务列表，一旦有任务立马取出运行
		for f := range w.task {
			if f == nil {
                // 退出goroutine，运行worker数减一
				w.pool.decRunning()
				return
			}
			f()
			// worker回收复用
			w.pool.putWorker(w)
		}
	}()
}

```

___

- 参考: [如何设计一个优雅的goroutine池子](https://doraemonabcd.xyz/post/ood/how-deisgn-a-goroutine-pool/#%e5%a6%82%e4%bd%95%e8%ae%be%e8%ae%a1%e4%b8%80%e4%b8%aa%e4%bc%98%e9%9b%85%e7%9a%84goroutine%e6%b1%a0%e5%ad%90);[C++ 线程池](https://wangpengcheng.github.io/2019/05/17/cplusplus_theadpool/);[高性能协程池](https://strikefreedom.top/archives/high-performance-implementation-of-goroutine-pool)

### 4. Go在什么情况下会panic

见的有10种情况：

- 数组/切片索引越界；
- 引用空指针；
- 除以零；
- 向已经关闭的通道发消息；
- 关闭一个已关闭的通道；
- 关闭未初始化的通道；
- 写入未初始化的map；
- 跨协程的panic处理；
- sync计数为负值 ：wg.Add(-1)

___

- 参考：[Go的Panic相关问题](https://juejin.cn/post/7308620787329384499)

## GC相关

### 1. go gc 是怎么实现的？（必问）

详见：[05 简述 Go 语言GC(垃圾回收)的工作原理](https://wangpengcheng.github.io/2023/04/17/go_interview/#05-%E7%AE%80%E8%BF%B0-go-%E8%AF%AD%E8%A8%80gc%E5%9E%83%E5%9C%BE%E5%9B%9E%E6%94%B6%E7%9A%84%E5%B7%A5%E4%BD%9C%E5%8E%9F%E7%90%86)

### 2. go 是 gc 算法是怎么实现的？ （得物，出现频率低）

被称为"并发三色标记清扫"（Concurrent Tri-color Mark and Sweep，简称CTMS）算法。

以下是该算法的基本步骤：

1. **标记开始（Mark Start）**：这个阶段会停止所有的Goroutine，这是一个STW（Stop The World）阶段，但该阶段通常非常短。
2. **标记（Mark）**：在这个阶段，GC会从一组根对象（比如全局变量，当前活跃的goroutine的栈变量等）开始，标记所有可以直接或间接访问到的对象。这个阶段是并发的，可以与Goroutine的运行并行进行。
3. **标记终止（Mark Termination）**：这是第二个STW阶段，主要完成一些清理和状态恢复工作。
4. **清扫（Sweep）**：在这个阶段，GC会清扫并回收所有未被标记的对象，即那些在标记阶段确定为不可达的对象。这个阶段也是可以与Goroutine的运行并行进行的。

这种GC算法的优点是大部分工作都可以在Goroutine运行的时候并行进行，只有非常短的时间需要停止所有Goroutine，这样可以减少GC对程序性能的影响。同时，Go的GC实现还包含了很多优化措施，比如分代GC、写屏障等，以进一步提高GC的效率。

不过，需要注意的是，尽管Go的GC算法可以并发运行，但它仍然会消耗一些CPU和内存资源，这可能会影响程序的性能。因此，在编写Go程序时，我们仍然应该尽量减少内存分配和垃圾产生的量，以减少GC的负担。

___

- 参考：[Go 是 GC 算法是怎么实现的？](https://www.iamshuaidi.com/23440.html)

### 3. GC 中 stw 时机，各个阶段是如何解决的？ （百度）

1）在开始新的一轮 GC 周期前，需要调用 gcWaitOnMark 方法上一轮 GC 的标记结束（含扫描终止、标记、或标记终止等）。

2）开始新的一轮 GC 周期，调用 gcStart 方法触发 GC 行为，开始扫描标记阶段。

3）需要调用 gcWaitOnMark 方法等待，直到当前 GC 周期的扫描、标记、标记终止完成。

4）需要调用 sweepone 方法，扫描未扫除的堆跨度，并持续扫除，保证清理完成。在等待扫除完毕前的阻塞时间，会调用 Gosched 让出。

5）在本轮 GC 已经基本完成后，会调用 mProf_PostSweep 方法。以此记录最后一次标记终止时的堆配置文件快照。

6）结束，释放 M。

____

- 参考：[GC 中 stw 时机，各个阶段是如何解决的？](https://www.kancloud.cn/lyj18810711213/golang/2747138);[10. Go 语言中 GC 的流程是什么？](https://www.bookstack.cn/read/qcrao-Go-Questions/spilt.10.GC-GC.md)

### 4. GC 的触发时机？

1. 主动触发：调用 runtime.GC
2. 被动触发：
    - 使用系统监控，该触发条件由 runtime.forcegcperiod 变量控制，默认为 2 分 钟。当超过两分钟没有产生任何 GC时，强制触发 GC。
    - 使用步调（Pacing）算法，其核心思想是控制内存增长的比例。如 Go 的 GC 是一种比例 GC, 下一次 GC 结束时的堆大小和上一次 GC 存活堆大小成比例.


- 参考：[GC 触发时机](https://www.kancloud.cn/lyj18810711213/golang/2747138);[11. 触发 GC 的时机是什么？](https://www.bookstack.cn/read/qcrao-Go-Questions/spilt.11.GC-GC.md)


### 5. GC 如何调优
通过 go tool pprof 和 go tool trace 等工具

控制内存分配的速度，限制 Goroutine 的数量，从而提高赋值器对 CPU 的利用率。
减少并复用内存，例如使用 sync.Pool 来复用需要频繁创建临时对象，例 如提前分配足够的内存来降低多余的拷贝。
需要时，增大 GOGC 的值，降低 GC 的运行频率。


## 内存相关

### 1. 谈谈内存泄露，什么情况下内存会泄露？怎么定位排查内存泄漏问题？

**内存泄漏定义**

当在Go语言中发生内存泄漏时，通常是因为程序中的某些对象在不再需要时仍然被引用，导致无法被垃圾回收器回收。这可能是因为对象被意外地保留在某个地方，比如缓存中，或者因为某些资源没有被正确释放。

以下是一些可能导致内存泄漏的常见情况：

1. 循环引用：如果两个对象相互引用，而且没有其他对象引用它们，那么它们将无法被垃圾回收器回收。比如两个协程共同引用对象，没有释放。

2. 未关闭资源：比如文件、网络连接等资源在使用完毕后没有被正确关闭，导致资源泄漏。

3. 大对象缓存：如果程序中使用了大对象缓存，而且没有合理地控制缓存的大小或者没有及时清理缓存，就会导致内存泄漏。

4. Goroutine泄漏：如果创建了大量的goroutine，而且这些goroutine没有被正确管理和释放，就会导致内存泄漏。
    - 申请过多的goroutine：例如在for循环中申请过多的goroutine来不及释放导致内存泄漏。一个 goroutine 的最低栈大小为 2KB，在高并发的场景下，对内存的消耗也是非常恐怖的
    - goroutine阻塞： 协程阻塞导致内存不能正常进行释放。
    - I/O问题： I/O连接未设置超时时间，导致goroutine一直在等待，代码会一直阻塞。
    - 互斥锁未释放：goroutine无法获取到锁资源，导致goroutine阻塞
    - 死锁：当程序死锁时其他goroutine也会阻塞
    - waitgroup使用不当：waitgroup的Add、Done和wait数量不匹配会导致wait一直在等待。
    - time.Ticker 是每隔指定的时间就会向通道内写数据。作为循环触发器，必须调用 stop 方法才会停止，从而被 GC 掉，否则会一直占用内存空间。
    - 字符串的截取引发临时性的内存泄漏：
    - 切片截取引起子切片内存泄漏：
    - 函数数组传参引发内存泄漏：函数传参的时候用到了数组传参，且这个数组够大（我们假设数组大小为 100 万，64 位机上消耗的内存约为 800w 字节，即 8MB 内存），或者该函数短时间内被调用 N 次，那么可想而知，会消耗大量内存，对性能产生极大的影响，如果短时间内分配大量内存，而又来不及 GC，那么就会产生临时性的内存泄漏，对于高并发场景相当可怕


```go

// 字符串截取内存泄漏
func main() {
 var str0 = "12345678901234567890"
 str1 := str0[:10]
}

// 切片截取引起子切片内存泄漏
func main() {
   var s0 = []int{0,1,2,3,4,5,6,7,8,9}
   s1 := s0[:3]
}
```

**内存泄漏排查**
1. pprof排查：可以使用go 内置的pprof进行内存泄漏排查。重点检查高内存占用的路径。
2. bcc排查工具：使用中的`memleak` 工具进行内存泄漏分析
3. 使用valgrind：使用其中的`Memcheck` 工具，直接进行内存泄漏分析

___

- 参考：[Go程序内存泄露问题快速定位](https://www.hitzhangjie.pro/blog/2021-04-14-go%E7%A8%8B%E5%BA%8F%E5%86%85%E5%AD%98%E6%B3%84%E9%9C%B2%E9%97%AE%E9%A2%98%E5%BF%AB%E9%80%9F%E5%AE%9A%E4%BD%8D/);[一些可能的内存泄漏场景](https://gfw.go101.org/article/memory-leaking.html);[No.7 一篇文章讲清楚golang内存泄漏](https://developer.aliyun.com/article/1353024);[Valgrind内存泄漏分析](https://yuanfentiank789.github.io/2018/11/01/%E7%94%A8Valgrind%E6%A3%80%E6%B5%8B%E5%86%85%E5%AD%98%E6%B3%84%E6%BC%8F/);[GO常规问题](https://xie.infoq.cn/article/1bf88b039aad3c61af144154c)


### 2. 知道 golang 的内存逃逸吗？什么情况下会发生内存逃逸？

Go 语言中，堆内存是通过垃圾回收机制自动管理的，无需开发者指定。那么，Go 编译器怎么知道某个变量需要分配在栈上，还是堆上呢？编译器决定内存分配位置的方式，就称之为逃逸分析(escape analysis)。逃逸分析由编译器完成，作用于编译阶段。

发生内存逃逸的主要情况如下：
1. 指针逃逸：返回对象指针，导致函数内变量内存在堆上分配。
2. interface{} 动态类型逃逸：在 Go 语言中，空接口即 interface{} 可以表示任意的类型，如果函数参数为 interface{}，编译期间很难确定其参数的具体类型，也会发生逃逸。
3. 栈空间不足：栈空间较小，函数递归较深。容易导致栈溢出。超过一定大小的局部变量逃逸到堆上。
4. 闭包：闭包中一个内层函数中访问到其外层函数的作用域。访问共享的变量会发生逃逸。

___

- 参考：[Go 逃逸分析](https://geektutu.com/post/hpg-escape-analysis.html)

### 3. 请简述 Go 是如何分配内存的？

#### 3.1 内存管理单元
go 内存管理，主要是指 go runtime 中的堆和栈上的内存管理。本质上是自带回收机制的内存池。可以有效降低编码过程中的内存管理新智负担，提高资源利用率。主要通过4种主要内存管理单元实现。其主要内存结构如下：

![内存管理单元](https://img.draveness.me/2020-02-29-15829868066479-go-memory-layout.png)

- `mspan`(内存管理基本单元): 内存池单元抽象，包含多个大小为8KB内存块，和对应的Object对象大小，用于快速进行内存申请和分配。
![对象和page大小](http://wangpengcheng.github.io/img/2024-03-31_22-33-23.png)
- `mcache`(线程缓存):  是 Go 语言中的线程缓存，它会与线程上的处理器一一绑定，主要用来缓存用户程序申请的微小对象。包含堆对象和栈对象。无需进行锁操作
![线程花村](https://img.draveness.me/2020-02-29-15829868066512-mcache-and-mspans.png)
- `mcentral`(中心缓存): 内存分配器的中心缓存，与线程缓存不同，访问中心缓存中的内存管理单元需要使用互斥锁。同时内存获取时会[分为多级](https://draveness.me/golang/docs/part3-runtime/ch07-memory/golang-memory-allocator/#%e5%86%85%e5%ad%98%e7%ae%a1%e7%90%86%e5%8d%95%e5%85%83-1)内存查询。
- `mheap`(堆内存):内存分配的核心结构体，Go 语言程序会将其作为全局变量存储，而堆上初始化的所有对象都由该结构体统一管理，该结构体中包含两组非常重要的字段，其中一个是全局的中心缓存列表 central，另一个是管理堆区内存区域的 arenas 以及相关字段。
![runtime.heapArena](https://img.draveness.me/2020-02-29-15829868066531-mheap-and-memories.png)

#### 3.1 内存分配

go 根据对象占用内存，将其分为了对象、小对象和微对象。不同的对象由不同的层级进行内存分配。

- 微对象 (0, 16B) — 先使用微型分配器，再依次尝试线程缓存、中心缓存和堆分配内存；
- 小对象 [16B, 32KB] — 依次尝试使用线程缓存、中心缓存和堆分配内存；
- 大对象 (32KB, +∞) — 直接在堆上分配内存；

对于堆空间而言，主要分配策略如下：

- Go语言源代码中「栈内存」和「堆内存」的分配都是虚拟内存，最终CPU在执行指令过程中通过内部的MMU把虚拟内存转化为物理内存。
- Go语言编译期间会进行逃逸分析，判断并标记变量是否需要分配到堆上，比如创建Map、Slice时。
- 栈内存分配
    - 小于32KB的栈内存
        - 来源优先级1：线程缓存mcache
        - 来源优先级2：全局缓存stackpool
        - 来源优先级3：逻辑处理器结构p.pagecache
        - 来源优先级4：堆mheap
    - 大于等于32KB的栈内存
        - 来源优先级1：全局缓存stackLarge
        - 来源优先级2：逻辑处理器结构p.pagecache
        - 来源优先级3：堆mheap
- 堆内存分配
    - 微对象 0 < Micro Object < 16B
        - 来源优先级1：线程缓存mcache.tiny
        - 来源优先级2：线程缓存mcache.alloc
    - 小对象 16B =< Small Object <= 32KB
        - 来源优先级1：线程缓存mcache.alloc
        - 来源优先级2：中央缓存mcentral
        - 来源优先级3：逻辑处理器结构p.pagecache
        - 来源优先级4：堆mheap
    - 大对象 32KB < Large Object
        - 来源优先级1：逻辑处理器结构p.pagecache
        - 来源优先级2：堆mheap
- 「栈内存」也来源于堆mheap

![栈内存](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/08ec2fe4f9054b8295452e697a84eaa1~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp)

#### 3.2 内存回收策略

详见：[05 简述 Go 语言GC(垃圾回收)的工作原理](https://wangpengcheng.github.io/2023/04/17/go_interview/#05-%E7%AE%80%E8%BF%B0-go-%E8%AF%AD%E8%A8%80gc%E5%9E%83%E5%9C%BE%E5%9B%9E%E6%94%B6%E7%9A%84%E5%B7%A5%E4%BD%9C%E5%8E%9F%E7%90%86)

___

- 参考：[7.1 内存分配器](https://draveness.me/golang/docs/part3-runtime/ch07-memory/golang-memory-allocator/);[go内存分配](https://golang.design/under-the-hood/zh-cn/part2runtime/ch07alloc/);[一文彻底理解Go语言栈内存/堆内存](https://juejin.cn/post/7135670650353483783);[7.3 栈空间管理](https://draveness.me/golang/docs/part3-runtime/ch07-memory/golang-stack-management/);[go垃圾收集器](https://draveness.me/golang/docs/part3-runtime/ch07-memory/golang-garbage-collector/)


### 4. Channel 分配在栈上还是堆上？哪些对象分配在堆上，哪些对象分配在栈上？

- Channel内存分配： Channel 被设计用来实现协程间通信的组件，其作用域和生命周期不可能仅限于某个函数内部，所以 golang 直接将其分配在堆上。栈上会有对应指针，指向堆内存对象


- Golang 中的变量只要被引用就一直会存活，存储在堆上还是栈上由内部实现决定而和具体的语法没有关系。通常情况下：
    - 栈上：函数调用的参数、返回值以及小类型局部变量大都会被分配到栈上，这部分内存会由编译器进行管理。 无需 GC 的标记。
    - 堆上：大对象、逃逸的变量会被分配到堆上，分配到堆上的对象。Go 的运行时 GC 就会在 后台将对应的内存进行标记从而能够在垃圾回收的时候将对应的内存回收，进而增加了开销。
- 堆栈分配区分：默认都是在栈中进行内存分配。有两种情况会分配在堆上
    - 栈内存不足：当栈内存不足时，会直接在堆上进行内存分配
    - 发生逃逸：变量进过逃逸分析后发现，在栈上分配有空指针风险。需要在堆上分配
        - 1. 指针逃逸：返回对象指针，导致函数内变量内存在堆上分配。
        - 2. interface{} 动态类型逃逸：在 Go 语言中，空接口即 interface{} 可以表示任意的类型，如果函数参数为 interface{}，编译期间很难确定其参数的具体类型，也会发生逃逸。
        - 3. 栈空间不足：栈空间较小，函数递归较深。容易导致栈溢出。超过一定大小的局部变量逃逸到堆上。
        - 4. 闭包：闭包中一个内层函数中访问到其外层函数的作用域。访问共享的变量会发生逃逸。


___

- 参考：[如何判断golang变量是分配在栈（stack）上还是堆（heap）上？](https://zhuanlan.zhihu.com/p/523195006)

### 5. 介绍一下大对象小对象，为什么小对象多了会造成 gc 压力？

小对象： 小于等于 32k 的对象就是，
大对象： 大于32KB的对象

一般小对象通过 mspan 分配内存；大对象则直接由 mheap 分配内存。通常小对象过多会导致 GC 三色法消耗过多的 CPU。优化思路是，减少对象分配。


___

- 参考：[整理 Golang 面试第二篇干货 13 问](https://xie.infoq.cn/article/1bf88b039aad3c61af144154c)

## 算法相关

### 标准输入问题
- [go标准输入问题](https://www.nowcoder.com/feed/main/detail/c707c89102eb4fd4b54919546f1a26a4?sourceSSR=search)


go 算法题目中需要手动读取程序输入。如何进行正常的标准输入获取。

#### 输入获取方式：

1. fmt.Scan/fmt.Scanln:直接使用Scan 可以方便快速进行多个变量的获取与赋值，但是解析较慢

```go
package main

import "fmt"

func main() {

    var name string
    var age int

    /*
        使用"&"获取score变量的内存地址(即取变量内存地址的运算符)，通过键盘输入为score变量指向的内存地址赋初值。

        fmt.Scan是一个阻塞的函数，如果它获取不到数据就会一直阻塞哟。

        fmt.Scan可以接收多个参数，用户输入参数默认使用空格或者回车换行符分割输入设备传入的参数，直到接收所有的参数为止
    */
    fmt.Scan(&name, &age)
    fmt.Println(name, age)

    /*
        和fmt.Scan功能类似，fmt.Scanln也是一个阻塞的函数，如果它获取不到数据就会一直阻塞哟。

        fmt.Scanln也可以接收多个参数，用户输入参数默认使用空格分割输入设备传入的参数，遇到回车换行符就结束接收参数
    */
    fmt.Scanln(&name, &age)
    fmt.Println(name, age)

     /*
        和fmt.Scanln功能类似，fmt.Scanf也是一个阻塞的函数，如果它获取不到数据就会一直阻塞哟。

        其实fmt.Scanln和fmt.Scanf可都以接收多个参数，用户输入参数默认使用空格分割输入设备传入的参数，遇到回车换行符就结束接收参数

        唯一区别就是可以格式化用户输入的数据类型,如下所示:
            %s:
                表示接收的参数会被转换成一个字符串类型，赋值给变量
            %d:
                表示接收的参数会被转换成一个整形类型，赋值给变量

        生产环境中使用fmt.Scanln和fmt.Scanf的情况相对较少，一般使用fmt.Scan的情况较多~
    */
    fmt.Scanf("%s%d", &name, &age)
    fmt.Println(name, age)
}
```

2. bufio.NewScanner: 获取标准输入即可。但是其最大长度仅有`64 * 1024`字节，需要使用bufio.Reader

```go
package main

import (
    "bufio"
    "fmt"
    "os"
    "strconv"
    "strings"
)

input := bufio.NewScanner(os.Stdin)
for input.Scan() {
    data := input.Text()
}   
```

3.  bufio.NewReader: 使用其获取标准输入，但是输入比较原始，需要注意格式解析

```go
package main
import (
    "bufio"
    "fmt"
    "os"
)
var inputReader *bufio.Reader
var input string
var err error
func main() {
    inputReader = bufio.NewReader(os.Stdin)
    fmt.Println("Please enter some input: ")
    input, err = inputReader.ReadString('S') //func (b *Reader) ReadString(delim byte) (line string, err error) ,‘S’ 这个例子里使用S表示结束符，也可以用其它，如'\n'
    if err == nil {
        fmt.Printf("The input was: %s\n", input)
    }
}

// Please enter some input:
// abcd
// 
// abc
// S
// The input was: abcd
//
// abc
// S
```

#### 常见场景

1. 多行数据(不知道有几行)，每行固定个数，空格隔开

```go
package main

import (
    "fmt"
    "io"
)

func main() {
    var a, b int
    for {
        _, err := fmt.Scan(&a, &b)
        if err == io.EOF {
            break
        }
        fmt.Println(a + b)
    }
}
// input
// 1 2
// 1 2
// out 
// 3
// 3
```

2. 场景2：多行数据，第一行只有一个数字n，表示后面的行数，其他每行固定个数

```go
package main

import "fmt"

func main() {
    var n, a, b int
    fmt.Scan(&n)
    for i := 0; i < n; i++ {
        fmt.Scan(&a, &b)
        fmt.Println(a + b)
    }
}
```

3. 场景3：输入数据有多行，每行固定个数，读取到特殊数据(如0,0)时停止

```go
package main

import "fmt"

func main() {
    var a, b int
    for {
        fmt.Scan(&a, &b)
        if a == 0 && b == 0 {
            break
        }
        fmt.Println(a + b)
    }
}

```

4. 场景4：输入数据有多行，每行第一个数字指定当前行数据个数，读取到特殊数据时停止

```go

package main

import "fmt"

func main() {
    var n, a int
    for {
        fmt.Scan(&n)
        if n == 0 {
            break
        }
        sum := 0
        for i := 0; i < n; i++ {
            fmt.Scan(&a)
            sum += a
        }
        fmt.Println(sum)
    }
}

```
5. 多行不定数据输入

输入数据有多组, 每行表示一组输入数据。
每行的第一个整数为整数的个数n(1 <= n <= 100)。
接下来n个正整数。
不知道总共有多少行数据。

```go
package main

import (
    "fmt"
    "io"
)

func main() {
    var n, a int
    for {
        // 读取首行n
        _, err := fmt.Scan(&n)
        if err == io.EOF {
            break
        }
        sum := 0
        for i := 0; i < n; i++ {
            fmt.Scan(&a)
            sum += a
        }
        fmt.Println(sum)
    }
}
```

6. 行长度不定
输入数据有多组, 每行表示一组输入数据。

每行不定有n个整数，空格隔开。(1 <= n <= 100)。

```go
package main

import (
    "bufio"
    "fmt"
    "os"
    "strconv"
    "strings"
)

func main() {
    // 获取输入
    inputs := bufio.NewScanner(os.Stdin)
    // 循环直到读取到EOF
    for inputs.Scan() {  //每次读入一行
        data := strings.Split(inputs.Text(), " ")  //通过空格将他们分割，并存入一个字符串切片
        var sum int
        for i := range data {
            val, _ := strconv.Atoi(data[i])   //将字符串转换为int
            sum += val
        }
        fmt.Println(sum)
    }
}
```

7. 数据

输入有两行，第一行n

第二行是n个字符串，字符串之间用空格隔开

```go
package main

import(
    "fmt"
    "os"
    "bufio"
    "sort"
    "strings"
)
 
func main(){
    in := bufio.NewScanner(os.Stdin)
    in.Scan()
    for in.Scan(){
        str := in.Text()
        s := strings.Split(str, " ")
        sort.Strings(s)  //排序
        fmt.Println(strings.Join(s," "))  //将切片连接成字符串
    }
}

```


- 参考：[Golang的标准输入输出](https://www.cnblogs.com/yinzhengjie2020/p/12245290.html);[Go语言标准输入输出处理](https://zhuanlan.zhihu.com/p/551393704);[GoLang 的 bufio.NewScanner 按行读取文件的坑](https://juejin.cn/post/6844903954942263304)