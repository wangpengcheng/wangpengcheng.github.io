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
- [Go安全指南](https://github.com/Tencent/secguide/blob/main/Go%E5%AE%89%E5%85%A8%E6%8C%87%E5%8D%97.md)
- [Go 编码规范建议](https://cloud.tencent.com/developer/article/1911268)
- [Go语言高性能编程](https://geektutu.com/post/hpg-escape-analysis.html)

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
### 变量与常量

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


### 作用域

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


### defer延迟调用

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

### 3. 关闭的 channel. 有数据的 channel，再进行读. 写. 关闭会怎么样？（各类变种题型）

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
