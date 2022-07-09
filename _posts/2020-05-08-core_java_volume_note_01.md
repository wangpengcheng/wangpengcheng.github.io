---
layout:     post
title:      Java核心编程 卷I 笔记(一)
subtitle:   Java核心编程 卷I 笔记(一)
date:       2020-05-08
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - Java
    - 基础编程
    - 读书笔记
---

> 2020-05-08 19:40:58

_参考链接：_
- [Java中文手册](https://www.matools.com/api/java8)

# 第一章 Java程序设计概述

## 1.2 Java 白皮书的关键术语
1. 简单性
2. 面向对象
3. 分布式
4. 健壮性
5. 安全性
6. 体系结构中立
7. 可移植性
8. 解释型
9. 高性能
10. 多线程
11. 动态性

# 第二章 Java程序设计环境

## 2.1 安装Java开发包工具

Java开发包工具可以到[Orcale官网](https://www.oracle.com/java/technologies/javase-downloads.html)进行下载；但是因为Orcale的下载机制；因此需要注册用户。可以使用该[网站](http://bugmenot.com/view/oracle.com)获取账户和密码进行下载。

注意：
- 安装JDK之后，需要设置PATH添加JDK/bin路径和CLASSPATH路径；方便进行相关环境变量的设置
- CLASSPATH中需要添加.路径。否则JAVA无法查找当前文件下的.class文件。

### 2.1.3 SRC源码

可以在JDK中找到标准库的源码；是src.zip的一个压缩文件。进行解压缩之后就可以获得源码。

### 2.1.4 下载本书源码示例
可以从如下链接查找和下载本书的源代码

- [源码地址](http://horstmann.com/corejava/)

## 2.2 使用命令行工具简单的编译源码

创建如下源代码Welcome.java：
```java
/**
* This file is a simple example for java 
* @version 1.30 2020-05-11
* @author WangPengCheng
*/
public class Welcome
{
    public static void main (String [] args)
    {
        //创建字符串常量
        String greeting ="Welcome to Core Java";
        System.out.println(greeting);
        for(int i=0;i<greeting.length();++i)
        {
            System.out.print("=");
        }
        System.out.println();
    }

}
```
执行下面的命令可以编译和运行；并产生结果

```shell
javac Welcome.java
java Welcome
Welcome to Core Java!
=====================
```
注意:
- Java的大小写敏感，java的文件名必须和public中的class名相同
- java运行时直接使用.class文件名。不需要带.class后缀
- 对于`错误: 找不到或无法加载主类 Welcome.class`；需要检查设置的路径


## 2.3 使用集成开发环境

本书推荐eclipse作为集成开发工具。但是因为不能翻墙下载一些jar；使用IntelliJ IDEA作为替代。解决下载问题之后可以使用eclipse

## 2.4 运行图形化应用程序
图形化应用程序的主要代码如下:
```java
/* 导入相关的包和库 */
import java.awt.*;
import java.io.*;
import javax.swing.*;
/**
* A program for viewing images
* @version 1.30 2020-05-11 18:25:62
* @author WangPengCheng
*/

public class ImageViewer
{
    public static void main(String[] args)
    {
	//lambel表达式添加函数
        EventQueue.invokeLater(()->{
            JFrame frame=new ImageViewerFrame();
            frame.setTitle("ImageViewer");
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.setVisible(true);
        });
    }
}
/*
 显示图片帧
*/
class ImageViewerFrame extends JFrame
{
    private JLabel label;
    private JFileChooser chooser;
    private static final int DEFAULT_WIDTH=300;
    private static final int DEFAULT_HEIGHT=400;
    //定义构造函数
    public ImageViewerFrame()
    {
        setSize(DEFAULT_WIDTH,DEFAULT_HEIGHT);
        //使用显示图片
        label=new JLabel();
        add(label);
        //设置文件夹当前路径
        chooser=new JFileChooser();
        chooser.setCurrentDirectory(new File("."));

        //设置文件选择框
        JMenuBar menuBar=new JMenuBar();
        setJMenuBar(menuBar);
        JMenu menu=new JMenu("File");
        menuBar.add(menu);
        //设置选框按钮
        JMenuItem openItem = new JMenuItem("Open");;
        menu.add(openItem);
        openItem.addActionListener(event->{
            //显示 选择弹窗
            int result=chooser.showOpenDialog(null);
            //根据选择设置icon
            if(result==JFileChooser.APPROVE_OPTION)
            {
                String name=chooser.getSelectedFile().getPath();
                label.setIcon(new ImageIcon(name));
            }
        });
        //设置关闭弹窗
        JMenuItem exitItem=new JMenuItem("Exit");
        menu.add(exitItem);
        exitItem.addActionListener(even->System.exit(0));
    }
}
```

## 2.5 构建并运行applet
详见 RoadApplet示例

# 第三章  Java的基本程序设计结构

## 3.1 一个简单的Java应用程序

注意：
- Java大小写敏感
- Java中类必须以字母开头；但是不能使用保留字段；标准规范为以大写字母开头。单次的每一个首字母都应该大写
- 源代码的文件名必须与公共类的名字相同，并使用Java作为扩展名。

## 3.2 注释
_参考链接：_ 

java中的注释有三种`//`,`/**/`和`/** */` ；最后一种是文档注释；可以用来生成文档。
文档注释的关键字；如下:

|标签|描述|示例|
|:---|:---|:---|
|`@author`|标识一个类的作者|`@author description`|
|`@deprecated`|指名一个过期的类或成员|@deprecated description|
|`{@docRoot}`|指明当前文档根目录的路径|Directory Path|
|`@exception`|标志一个类抛出的异常|@exception exception-name explanation|
|`{@inheritDoc}`|从直接父类继承的注释|Inherits a comment from the immediate surperclass.|
|`{@link}`|插入一个到另一个主题的链接|{@link name text}|
|`{@linkplain}`|插入一个到另一个主题的链接，但是该链接显示纯文本字体|Inserts an in-line link to another topic.|
|`@param`|说明一个方法的参数|@param parameter-name explanation|
|`@return`|说明返回值类型|@return explanation|
|`@see`|指定一个到另一个主题的链接|@see anchor|
|`@serial`|说明一个序列化属性|@serial description|
|`@serialData`|说明通过writeObject( ) 和 writeExternal( )方法写的数据|@serialData description|
|`@serialField`|说明一个ObjectStreamField组件|@serialField name type description|
|`@since`|标记当引入一个特定的变化时|@since release|
|`@throws`|和 @exception标签一样.	|The @throws tag has the same meaning as the @exception tag.|
|`{@value}`|显示常量的值，该常量必须是static属性。|Displays the value of a constant, which must be a static field.|
|@version|指定类的版本|@version info|

## 3.3 数据类型

_参考链接：_ [Java基本数据类型](https://www.runoob.com/java/java-basic-datatypes.html)

Java中有两大数据类型:内置数据类型和引用数据类型
- 内置数据类型:

|数据类型|位数大小|最小值|最大值|默认值|
|:---|:---|:---|:---|:---|
|boolean|1|false|true|false|
|byte|8|127（2^7-1）|-128（-2^7）|0|
|char|16|`\u0000(0)`|`\uffff(65535)`|无|
|short|16|-32768（-2^15）|32767（2^15 - 1）|0|
|int|32|-2,147,483,648（-2^31）|2,147,483,647（2^31 - 1）|0|
|float|32|无|无|0.0f|
|long|64|-9,223,372,036,854,775,808（-2^63）|9,223,372,036,854,775,807（2^63 -1）|0L|
|double|64|无|无|0.0d|

注意：
- **声明变量可以。但进行使用时，必须初始化之后才能使用。**
- Java中不区分变量的声明和定义。
- 自动类型转换:**整型、实型（常量）、字符型数据可以混合运算。运算中，不同类型的数据先转化为同一类型，然后进行运算。**

> 低  ------------------------------------>  高
> 
> byte,short,char—> int —> long—> float —> double 

- 注意:
    1. 不能对boolean类型进行类型转换。
    2. 不能把对象类型转换成不相关类的对象。
    3. 在把容量大的类型转换为容量小的类型时必须使用强制类型转换。
    4. 转换过程中可能导致溢出或损失精度。

- 引用类型
    - 在Java中，引用类型的变量非常类似于C/C++中的指针。引用类型指向一个对象，指向对象的变量是引用变量；对象和数组都是引用数据类型。
    - 所有引用类型的默认值都是null
    - 一个引用变量可以用来引用任何与之兼容的类型
    
注意:
- 对象只有在实例化之后才能被使用，而实例化对象的关键字就是new。
- 对象的实例化过程如下:
![](https://img-blog.csdnimg.cn/20181203210928597.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQzNTU1MzIz,size_16,color_FFFFFF,t_70)

- 常量:Java中利用关键字final指示常量。

## 3.5 运算符

Java中的使用`+,-,*,/`表示对应运算
注意:**整数被0除将会产生一个异常，而浮点数被0除将会得到无穷大结果(Infinity)**

### 3.5.1 数学函数与常量

Java中包含了各种各样的数学函数，一般使用Math开头。一般Math中的方法都是静态方法；Math中没有幂函数，需要借助pow方法，中的两个输入对象进行计算。

注意:java中的double类型不能用于金钱的运算，因为会存在误差。
当不想使用前缀时可以采取如下的办法:

```java
//将所有方法作为static类型使用
import static java.lang.Math.*;
System.out.println("The square root of \uo3co is"+sqrt(PI));
```
感想:Java基本上是按照C++的设计思路在进行改进。C++中使用namespace 实现方法的重复冲突解决。但是Java直接使用文件夹和package的方式来作为namespace；本质上是一样的，但是因为区分了static方法和变量。C++中的全局方法实际是static方法。

注意:
- 这里的`import static`。一般引入需要使用 ClassName.method(); 的方式去调用类中的静态方法；而静态引入后，直接使用 method(); 即可使用静态方法。
- Math类中，为了达到最快的性能，所有的方法都使用计算机浮点单元中的例程。如果想得到准确的结果，因该使用StrictMath类。

### 3.5.2 数值类型之间的转换

转换方式如图所示，一般是低精度向高精度转换。但是注意和C++不同，Java中的boolean类型，不能向其它类型进行转换。

### 3.5.7 位运算符

Java中的逻辑运算符和位运算符基本和C++相同，但是，为了避免右移运算时，C++使用最高符号位填充新高位。这里统一使用`>>>`使用0填充高位。消除了不确定性。

### 3.5.8 java中的运算符及其级别

![](https://wangpengcheng.github.io/img/2020-05-20-11-06-58.jpg)
![](https://wangpengcheng.github.io/img/2020-05-20-11-08-58.png)

### 3.6.3 不可变字符串

Java中的String类没有提供修改字符串的方法。每次的字符串修改，实际上都是新拷贝了一个字符串。原来的字符串仍旧存在于内存中。在jdk的实现中，string本质上是一个char数组；这里的char数组是被final修饰的。

### 3.6.4 检测字符串是否相同

因为Java不像C++可以重载==操作符，因此判断是否相同，需要使用equal函数或者compareTo()(结果为0时相同)函数。

- null是指String对象没有初始化，相当于char* temp=null;空串是其等于"";

### 3.6.6 码点与代码单元

_参考链接：_ [字符集和字符编码](https://blog.csdn.net/u012268339/article/details/54694310?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.nonecase)

因为Java中使用Unicode字符集，编码方式通常为UTF-8或者UTF-16等不定长编码方式，而Java中的char类型时定长，长度为16位；因此当存在特殊字符其编码长度大于16时；需要进行修改。因此Java中使用代码单元(CodePoints)，保证每个字符都能被正确获取。字符的遍历应该使用如下操作

```java
int cp=sentence.codePointAt(i);
if(Character.isSupplementCodePoint(cp)){
    i+=2;
}else{
    ++i;
}
```
### 3.6.7 String API

详见[参考手册](https://www.matools.com/api/java8):

### 3.6.9 StringBuilder

JDK5.0中引入StringBuilder类。非线程安全，实现高效的字符串处理；常用方法如下:
![](https://wangpengcheng.github.io/img/2020-05-20-17-08-58.png)


## 3.7 输入和输出

Java中的输入和输出一般有Scanner和Console两种方式，一般头通过System.in进行初始化。并对输入进行获取。这两个都是包含在java.util包中。主要函数和方法如下:

![](https://wangpengcheng.github.io/img/2020-05-20-19-08-58.png)

### 3.7.2 格式化输入和输出



一般使用print进行格式化的输入和输出;相关参数和标准如下:

![](https://wangpengcheng.github.io/img/2020-05-20-19-09-58.png)

相关格式标志如下:

![](https://wangpengcheng.github.io/img/2020-05-20-19-10-58.png)

print格式
![](https://wangpengcheng.github.io/img/2020-05-20-19-11-58.png)


### 3.7.3 文件输入与输出

可以使用Scaner并以文件路径作为参数进行文件的获取，可以指定字符串的编码格式。也可以使用`java.io.PrintWriter`构造一个将数据写入文件的PrintWriter。文件名由参数指定。

```java
Scanner in=new Scanner(Path.get("myfile.txt"),"UTF-8");
```

### 3.9 大数值

`java.util.BigInteger`和`java.util.BigDecimal`大数值可以处理任意长度数字序列的数字。使用`.valueOf`方法将普通的数值转换为大数值。但是运算操作需要使用特殊的函数；主要函数列表如下:

![](https://wangpengcheng.github.io/img/2020-05-20-19-20-58.png)

![](https://wangpengcheng.github.io/img/2020-05-20-19-20-55.png)

## 3.10 数组

Java中的数组实际上的内存分分配是在栈中(int[] 和new int[] 相同)。因此多维数组都是指针数组。每个元素都是指针，存放的是地址值。因此不必长度相同。但是数组除了已开始初始化外，之后只能借助第三方变量进行初始化。也可以使用new配合匿名数组进行初始化。也可以直接初始化

```java
int[] smallPrimes=new int[]{17,19,23,29};
//等同如下:
int[] anonymous={17,19,23,29};
smallPrimes=anonymous;
```

使用`Arrays.copyOf()`和`Arrays.sort()`进行深拷贝和排序。
注意Java中`[]`运算符被预定义为检查数组边界，没有指针预算。不能通过+1操纵得到数组的下一个元素。


`java.util.Arrays`中的常用方法:
- `static String toString(type[] a)`:将其转化为字符串，并使用,进行分割。
- `static type copyOf(type[] a,int length)`:拷贝函数
- `static type copyOfRange(type[] a,int start,int end)`: 失败时返回0或者false;
- `static void sort(type[] a)`:快速排序
- `static int binarySearch(type[] a,type v)`:二分搜索，其必须有序,返回对应值的下标。
- `static int binarySearch(type[] a,int start,int end,type v)`:二分搜索，限定范围。
- `static void fill(type[] a,type v)`:将所有的数据元素设置为v;
- `static type binarySearch(type[] a,type v)`:二分搜索，其必须有序
- `static boolean equals(type[] a,type[] b)`: 比较是否完全相同。

Java中的Arrays相当于C++中的vector。

# 第 4 章 对象与类

注意类中的更改器方法一般都没有更改原来的数据，而是产生了一个新的数据。简单来说就是写时复制。

### 4.3.2 Java多个源文件的使用

当使用Java编译.java文件时，如果程序使用了xxx类，则javac会自动查找名为xxx.class的文件，没有则查找xxx.java然后对其进行编译。如果xxx.java版本比xxx.class版本更新，则自动地重新编译这个文件。**Java编译器内置了make功能**

注意:
- Java构造器的工作方式与C++相同。但是所有的Java对象都是在堆中构造的，因此必须使用new操作符。
- 避免在构造函数中使用与全局变量同名的局部变量。
- Java中和C++相同使用this作为隐式参数进行函数方法和变量的调用和声明。
- Java中的所有方法都必须在类的内部定义
- 注意不要编写返回引用可变对象的访问器方法。因为Java中的对象都是指针，返回值拷贝也是指针值的拷贝。因此会存在可变对象，破坏了类的封装性。可以使用clone()方法，对对象进行深拷贝。
- 使用final，相当于C++中的const，定义必须初始化。
- Java中的静态方法与c++中定义相同，但是是直接通过类名.方法名使用。而不是解引用方法使用。
- Java中的所有方法都是按值进行参数传递的。


## 4.5 方法参数
Java为了实现C++中的引用功能--改变传入对象，将所有的基本类型都做了类对象的封装。并通过开箱与装箱实现两者之间的快速转换。

## 4.6 对象构造

- **重载**:Java允许重载任何方法，而不只是构造器方法。
- **默认域初始化**:在构造器中没有显式地给域赋予初值，那么就会被自动地赋值为默认值。各个基本类型的默认值详见3.3
- **默认构造器**:当类中没有任何构造器的时候，编译器才会提供无参构造器。存在任何一个构造器，其无参构造器都会消失。
- **显示初始化**:建议在定义时，显示初始化默认参数。
- **构造方法输入参数命名**:可以和成员变量相同，但是需要使用this指针区别两者。
- **块初始化**:可以使用块初始化代码，让部分代码在构造函数之前进行运行。或者使用static关键字构造相同功能的代码块。为了避免循环定义，应该将初始化代码块放在变量定义域之后。

### 4.6.2 调用构造器的具体步骤

1. 所有数据域被初始化为默认值
2. 按照在类声明中出现的次序，一次执行所有初始化语句和初始化块
3. 构造器第一行调用了第二个构造器，则执行第二个构造器主体。
4. 执行这个构造器的主体。

### 4.6.8 对象析构与finalize方法：
Java中不支持析构函数，但是可以给任何一个类添加finalize方法回收任何短缺的资源。

## 4.7 包
可以使用完整的路径名进行类的导入，也可以使用import。避免重复的方法。当相同包中存在重复，需要再import一次明确相同类的来源。

- 使用`package xxx.xx.xx;`方法将类导入到包中。
- 包的路径，一般都是文件夹路径。因此可以直接再对应的package路径中找到对应的.class文件。
- 非public类，同一个包中可以访问。包外不行。

## 4.8 类路径

类存储在文件系统的子目录中，类的路径必须与包名匹配。
为了类能够被多个程序共享，需要做到一下几点:
- 类放到一个目录中。包树状结构的基目录。文件路径必须和包名匹配。
- 将JAR文件放在一个目录中
- 设置类路径(class path)。包含所有类文件的路径的集合。
- 可以在运行时，使用`-classpath`参数指定类的路径。类似于C++的LIB_PATH。

注意:javac编译器总是在当前目录中查找文件。但是java虚拟机仅在类中有`·`时才查看当前目录。如果没有设置类路径问题不会有/如果设置了但是没有包含'.';则会运行错误。

## 4.9 文档注释

详细语法：[Java 文档注释](https://www.runoob.com/java/java-documentation.html)

### 4.9.7 注释的抽取
假设存放在docDirectory下，执行如下步骤:
1. 切换至源文件目录或者根目录。
2. 对于包执行如下命令: `javadoc -d docDirectory nameOfPackage`
    - 多个包执行如下命令: `javadoc -d docDirectory nameOfPackage1,nameOfPackage2,nameOfPackage3...`
    - 默认包运行:`javadoc -d docDirectory *.java`

## 4.10 类的设计技巧
1. 保证数据私有
2. 一定要对数据初始化
3. 不要在类中使用过多的基本类型。
4. 不是所有的域都需要独立的访问器或者修改器。
5. 将职责过多的类进行分解
6. 类名和方法名要能够体现他们的职责。
7. 优先使用不可变的类。


# 第 5 章 继承

Java中对于普通类的继承直接使用extend 代替C++中的`:`进行继承操作。而且Java中为了避免菱形继承问题，使用了interface。作为抽象基类接口。对于extend只能单继承，interface可以使用implenment来实现多个接口类的继承，符合面向对象设计原则中的面向接口编程。同时提供了abstract 关键字修饰抽象类。相当于C++中的虚基类。

注意:
- Java中的所有继承都是公有继承。
- Java中使用super关键字区分父类方法和子类方法；方便进行方法的重载；同时使用super显示进行父类构造函数的调用
- Java中的对象变量是多态的(毕竟指针)；因此可以实现向下类型的安全转变。但是要注意类型转变时的方法和名称关系。向上的类型转换一般都是不安全的要注意方法调用。
- 如果时private、static、final修饰的方法或者构造器这些都是静态绑定。编译器可以准确地直到调用的方法。此外的方法。编译器会为每个类创建一个方法表，列出所有的方法签名和实际调用的方法。在运行时由虚拟机进行方法查找。指明类的方法和实际对应的函数代码段。
- 在覆盖一个方法的时候，子类方法不能低于超类方法的可见性。特别是，如果超类方法是public则子类方法一定要声明为public。
- 使用final关键字可以禁止类的继承和方法的覆盖。
- 在进行向下的类型转换之前，都应该使用instanceof关键字检查类型是否能够被强制转换成功。
- Java类型转换失败时会产生一个异常。
- abstract修饰的抽象类不能够被实例化。但是可以创建一个具体的子类，用来指向抽象类--(ps:猜想抽象类没有构建方法)。
- public:对对所有类可见，protected--对本包和所有子类可见；private--本类可见；默认-对本包可见。

## 5.2 Object 所有类的超类

Java中的所有类都是由Object类扩展而来的。因此可以将任何类型的变量向下转换为Object类型的变量。

Java语言规范要求equals方法具有下面的特性:
- 1. 自反性:能够识别自身。
- 2. 对称性:对于任何引用x,对于x.equal(y)结果应该和y.equal(x)相同。
- 3. 传递性:a=c,c=b,则有a=c;
- 4. 一致性:x，y引用对象没有发生变化，反复调用应该返回同样的结果。
- 5. 对任意非空引用x,x.equals(null)应该返回false。

编写一个完美的equals方法的建议:

![](https://wangpengcheng.github.io/img/2020-05-21-7-57-58.png)

### 5.2.3 hashCode()方法

每个对象都有一个默认的散列码，其值为对象的存储地址。如果重新定义equals方法，就必须重新定义hashCode方法，以便将对象插入到散列表中。
### 5.2.4 toString方法

返回对象值的字符串。应该是`"classname ["+"valName="+val+"]"`;这样对象可以与一个字符串通过"+"连接起来。
### 5.3 泛型数组列表

Java中可以使用`ArrayList<ClassName>`的方式创建动态的泛型列表。不用定长。可以动态增长。相当于C++中的Vetcor。
**注意: `new ArrayList<>(100)`只是拥有100个容量的潜力；其中并没有任何元素--内存分配了但是没有内容**

## 5.4 对象的装箱与开箱

详见参考链接

## 5.5 参数数量可变的方法

```java
public class PrintStream
{
    public PrintStream printf(String fmt,Object... args){
        return format(fmt,args);
    }
    //注意：这里的Object实际上是一个object数组。
}
```

## 5.7 反射

能够分析类能力的程序称为反射。反射机制的功能及其强大。
- 运行时分析类的能力
- 运行时查看对象，例如通过一个toString方法供所有类使用
- 实现通过的数组操作代码
- 利用Method对象，这个对象很像C++中的函数指针。

### 5.7.1 Class类

Class类实际上是一个泛型类。例如Employee.class 的类型是Class<Employee>。虚拟机为每个类型管理一个Class对象。因此可以使用==运算符是心啊两个类对象的比较操作。

![反射类图](https://wangpengcheng.github.io/img/2020-05-21-18-38-58.png)

### 5.7.3 利用反射分析类的能力；

**java.lang.reflect相关方法:**
- [java.lang.reflect](https://www.matools.com/file/manual/jdk_api_1.8_google/java/lang/reflect/package-frame.html)

Java数组会记录每个元素的类型。即创建数组时new表达式中使用的元素类型。因此使用反射创建数组时，应该使用Array.newInstance(componentType,newLength);创建实际的数组和元素。下面时一个优秀的拷贝构造函数的代码:
```java
public static Object goodCopyOf(Object a,int newLength)
{
    Class cl=a.getClass();
    if(!cl.isArray()) return null;
    Class componenType=cl.getComponentType();
    int length=Array.getLength(a);
    Object newArray=Array.newInstance(componentType,newLength);
    System.arraycopy(a,0,newArray,0,Math.min(length,newLength));
    return newArray;
}
```

![](https://wangpengcheng.github.io/img/2020-05-21-17-38-58.png)

### 5.7.6 调用任意方法

Java中可以使用class类对应的getMethod方法查找字符串对应的方法。返回一个Method class类。然后使用Method的invoke方法就可以执行对应的函数了。

```java
Method m1=Employee.class.getMethod("getName");
//执行函数

String n=(String)m1.invoke(harry);
Method squre=MethodTableTest.class.getMethod("square",double.class);
double y=(Double)squre.invoke(null,10);
```

## 5.8 继承的设计技巧

1. 将公共操作和域放在超类
2. 不要使用受保护的域:protected 域对于子类无限制，很容易破坏其封装性。其用改用于只是那些不提供一般用途而应该在子类中重新定义的方法。
3. 使用继承实现"is-a"关系
4. 除非所有继承的方法都有意义。否则不要使用继承
5. 在覆盖方法时，不要改变预期的行为。
6. 使用多态，而非类型信息；对于相同类，应该统一接口。
7. 不要过多的使用反射:反射很脆弱，编译器很难帮助人们发现程序中的错误，只有在运行时才发现错误并导致异常。

# 第 6 章 接口、lambda表达式

## 6.1 接口(interface)

**在接口中不能包含实例域或静态方法，但可以包含常量；接口中的所有方法都被自动地设置为public，接口中的域将被自动地设置为public static final常量**

### 6.1.3 接口与抽象类

接口的出现主要是为了防止多重继承问题下的多继承解决方案。

### 6.1.4 静态方法和默认方法

在interface中使用static关键字和default对方法进行修饰；获取对应的方法。默认方法可以提前实现接口方法。注意接口中如果方法没有default则必须将其实现。

### 6.1.6 解决默认方法冲突

Java解决多个方法的默认冲突方法如下:
1. 超类优先。如果超类提供了一个具体方法，同名且有相同参数类型的默认方法会被忽略。
2. 接口冲突:如果一个超类提供了一个默认方法，另外一个接口提供了一个同名且参数类型相同的方法。必须覆盖这个方法来解决冲突。
3. 继承的类优先原则:继承超类和接口冲突时，优先考虑类方法--"类优先"原则。


## 6.2 接口示例

### 6.2.2 Comparator接口
 类似于C++中的compartor function 类。是对左右两个数进行比较。

 ```java
 class LengthComparator implements Comparator<String>
 {
     public int compare(String first,String second){
         return first.length()-second.length();
     }
 }

 Comparator<String> comp=new LengthComparator();
 Arrays.sort(friends,new LengthComparator);

 ```

### 6.2.3 对象克隆

因为Java中的所有类变量都是指针。因此会存在浅拷贝的问题。虽然Object存在一个protected clone()方法。但是它只能针对基本数据类型；对于类类型不会进行递归的拷贝。因此需要每个类重载或者定义自己的clone函数。为了实现这个功能；提供了Cloneable接口。来重写clone方法。进行深拷贝。

注意:
- 所有数组类型都有一个public的clone方法，而不是protected。

下面是一个可克隆的类：
```java
package clone;

import java.util.Date;
import java.util.GregorianCalendar;

public class Employee implements Cloneable
{
   private String name;
   private double salary;
   private Date hireDay;

   public Employee(String name, double salary)
   {
      this.name = name;
      this.salary = salary;
      hireDay = new Date();
   }

   public Employee clone() throws CloneNotSupportedException
   {
      // call Object.clone()
      Employee cloned = (Employee) super.clone();

      // clone mutable fields
      cloned.hireDay = (Date) hireDay.clone();

      return cloned;
   }

   /**
    * Set the hire day to a given date. 
    * @param year the year of the hire day
    * @param month the month of the hire day
    * @param day the day of the hire day
    */
   public void setHireDay(int year, int month, int day)
   {
      Date newHireDay = new GregorianCalendar(year, month - 1, day).getTime();
      
      // Example of instance field mutation
      hireDay.setTime(newHireDay.getTime());
   }

   public void raiseSalary(double byPercent)
   {
      double raise = salary * byPercent / 100;
      salary += raise;
   }

   public String toString()
   {
      return "Employee[name=" + name + ",salary=" + salary + ",hireDay=" + hireDay + "]";
   }
}
```

## 6.3 lambda表达式

存在的意义和C++中的lambda表达式一样。主要语法参数如下：
- `(Args...)->{ code ...;return ...}`(注意：不需要定义输入类型)

主要还是编译器辅助构造一个function class。并在必要时实例化。

注意lambda表达式返回值，和C++相同，无论任何分支都必须要有。否则就会错误。下面是一个简单的测试代码：

```java

package lambda;

import java.util.*;

import javax.swing.*;
import javax.swing.Timer;

/**
 * This program demonstrates the use of lambda expressions.
 * @version 1.0 2015-05-12
 * @author Cay Horstmann
 */
public class LambdaTest
{
   public static void main(String[] args)
   {
      String[] planets = new String[] { "Mercury", "Venus", "Earth", "Mars", 
            "Jupiter", "Saturn", "Uranus", "Neptune" };
      System.out.println(Arrays.toString(planets));
      System.out.println("Sorted in dictionary order:");
      Arrays.sort(planets);
      System.out.println(Arrays.toString(planets));
      System.out.println("Sorted by length:");
      //注意：没有定义输入类型
      Arrays.sort(planets, (first, second) -> first.length() - second.length());
      System.out.println(Arrays.toString(planets));
            
      Timer t = new Timer(1000, event ->
         System.out.println("The time is " + new Date()));
      t.start();   
         
      // keep program running until user selects "Ok"
      JOptionPane.showMessageDialog(null, "Quit program?");
      System.exit(0);         
   }
}

```
lambda 表达式主要还是将其转换为函数式接口。

## 6.3.4 方法引用

和C++类似，Java允许方法引用，直接传递已经存在的函数进行使用。也可以直接使用方法引用；例如：
```Java
Timer t=new Timer(1000,event->System.out.println(event));
//等价于

Timer t=new Timer(1000,System.out::println());

```
System.out::println()等价于x->System.out.println(x);

使用::进行分割，主要有三种情况：
- `object::instanceMethod`:相当于提供方法参数的lambda表达式
- `Class::staticMethod`:相当于提供方法参数的lambda表达式
- `Class::instanceMethod`:第一个参数会成为方法的目标。例如`String::compareToIgnoreCase`等同于`(x,y)->x.compareToIgnoreCase(y)`。

### 6.3.5 构造器引用

可以直接使用类的构造器引用的方式，传入函数参数；快速构造代码。
```java
//传入参数，构造Person数组
Person[] people=stream.toArray(Person[]::new);
```
### 6.3.6 变量作用域

lambda表达式可以捕获外围作用域中变量的值，唯一需要注意的是基础类型，很可能在执行前已经消失。**因此在lambda表达式中只能引用值不会改变的变量**

注意:
- **lamdba表达式的体域嵌套块有相同的作用域。因此需要注意命名冲突和遮蔽的有关规则**

### 6.3.7 处理lambda表达式

lambda本质上还是转换为一个函数接口。下面是一些常用的函数式接口。方便lambda进行转换。

![](https://wangpengcheng.github.io/img/2020-05-22-08-58-56.png)
![](https://wangpengcheng.github.io/img/2020-05-22-09-02-56.png)

## 6.4 内部类

内部类，可以访问自身的数据域，也可以访问创建它的外围类对象的数据域。
- 在内部类中可以使用`class.this.value`的方式明确访问外部变量。
- 内部类中声明的所有静态域都必须是final。
- 编译器获奖内部类翻译成用$分隔外部类名与内部类名的常规类文件，而虚拟机对此一无所知。

### 6.4.5 由外部方法访问变量

局部类不仅可以访问包含它们的外部类，还可以访问局部变量。不过局部变量事实上必须为final--不会改变。这是因为，为了防止局部变量生命周期提前结束，内部类在编译构造时会自动产生一个final的类内部变量；拷贝一次之后继续使用，防止发生错误。

注意：
- **Integer时不可变类，进入一个方法后，在里面的值的改变不会影响方法外的引用([Integer面试题：如何在调用方法内改变Integer参数的值?](https://blog.csdn.net/Synlla1119/article/details/94434401?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase))**
- 对于需要传入更新的变量来说，最好的方法时是使用一个长度为1的数组来代替它。这样因为在堆上分配的内存，因此可以进行修改。

### 6.4.6 匿名内部类

Java中可以只创建类的对象，并在后面重载关键方法；这样的方式创建匿名类。常用语法格式如下:
```java
new SuperType(construction parameters)
{
    inner class methods and data;
}
```
注意: 匿名类没有类名，所以不能有构造器。使用参数传递给超类构造器。尤其在内部类实现接口的时候，不能有任何构造函数。

匿名类使用如下:
```java
package anonymousInnerClass;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.Timer;

/**
 * This program demonstrates anonymous inner classes.
 * @version 1.11 2015-05-12
 * @author Cay Horstmann
 */
public class AnonymousInnerClassTest
{
   public static void main(String[] args)
   {
      TalkingClock clock = new TalkingClock();
      clock.start(1000, true);

      // keep program running until user selects "Ok"
      JOptionPane.showMessageDialog(null, "Quit program?");
      System.exit(0);
   }
}

/**
 * A clock that prints the time in regular intervals.
 */
class TalkingClock
{
   /**
    * Starts the clock.
    * @param interval the interval between messages (in milliseconds)
    * @param beep true if the clock should beep
    */
   public void start(int interval, boolean beep)
   {
       //创建匿名类
      ActionListener listener = new ActionListener()
         {
             //实现对应的函数
            public void actionPerformed(ActionEvent event)
            {
               System.out.println("At the tone, the time is " + new Date());
               if (beep) Toolkit.getDefaultToolkit().beep();
            }
         };
      Timer t = new Timer(interval, listener);
      t.start();
   }
}

```

### 6.7 静态内部类

当对象中不需要引用任何其他对象时，可以将这个内部类声明为static；**只有内部类可以声明为static；内部类不需要访问外围对象的时候，应该使用静态内部类；声明在接口中的内部类，自动成为static和public类**

```java

package staticInnerClass;

/**
 * This program demonstrates the use of static inner classes.
 * @version 1.02 2015-05-12
 * @author Cay Horstmann
 */
public class StaticInnerClassTest
{
   public static void main(String[] args)
   {
      double[] d = new double[20];
      for (int i = 0; i < d.length; i++)
         d[i] = 100 * Math.random();
      ArrayAlg.Pair p = ArrayAlg.minmax(d);
      System.out.println("min = " + p.getFirst());
      System.out.println("max = " + p.getSecond());
   }
}

class ArrayAlg
{
   /**
    * A pair of floating-point numbers
    */
   public static class Pair
   {
      private double first;
      private double second;

      /**
       * Constructs a pair from two floating-point numbers
       * @param f the first number
       * @param s the second number
       */
      public Pair(double f, double s)
      {
         first = f;
         second = s;
      }

      /**
       * Returns the first number of the pair
       * @return the first number
       */
      public double getFirst()
      {
         return first;
      }

      /**
       * Returns the second number of the pair
       * @return the second number
       */
      public double getSecond()
      {
         return second;
      }
   }

   /**
    * Computes both the minimum and the maximum of an array
    * @param values an array of floating-point numbers
    * @return a pair whose first element is the minimum and whose second element
    * is the maximum
    */
   public static Pair minmax(double[] values)
   {
      double min = Double.POSITIVE_INFINITY;
      double max = Double.NEGATIVE_INFINITY;
      for (double v : values)
      {
         if (min > v) min = v;
         if (max < v) max = v;
      }
      return new Pair(min, max);
   }
}

```

## 6.5 代理

在运行时创建一个实现了一组给定接口的新类。在运行时确定类型，并使用方法；相当于装饰器模式的更新。测试代码如下

```java
package proxy;

import java.lang.reflect.*;
import java.util.*;

/**
 * This program demonstrates the use of proxies.
 * @version 1.00 2000-04-13
 * @author Cay Horstmann
 */
public class ProxyTest
{
   public static void main(String[] args)
   {
       //创建对象数组
      Object[] elements = new Object[1000];

      // fill elements with proxies for the integers 1 ... 1000
      //初始化数组
      for (int i = 0; i < elements.length; i++)
      {
         Integer value = i + 1;
         //创建代理处理句柄
         InvocationHandler handler = new TraceHandler(value);
         //创建代理对象，
         Object proxy = Proxy.newProxyInstance(null, /* 类加载器*/
         new Class[] { Comparable.class } , /* class对象数组 */
         handler/* 每个元素需要实现的接口 */
         );
         elements[i] = proxy;
      }

      // construct a random integer
      //生成一个随机数
      Integer key = new Random().nextInt(elements.length) + 1;

      // search for the key
      //执行二分搜索
      int result = Arrays.binarySearch(elements, key);

      // print match if found
      //输出找到结果
      if (result >= 0) System.out.println(elements[result]);
   }
}

/**
 * An invocation handler that prints out the method name and parameters, then
 * invokes the original method
 */
class TraceHandler implements InvocationHandler
{
   private Object target;

   /**
    * Constructs a TraceHandler
    * @param t the implicit parameter of the method call
    */
   public TraceHandler(Object t)
   {
      target = t;
   }
    //打印调用参数
   public Object invoke(Object proxy, Method m, Object[] args) throws Throwable
   {
      // print implicit argument
      System.out.print(target);
      // print method name
      System.out.print("." + m.getName() + "(");
      // print explicit arguments
      //输出参数
      if (args != null)
      {
         for (int i = 0; i < args.length; i++)
         {
            System.out.print(args[i]);
            if (i < args.length - 1) System.out.print(", ");
         }
      }
      System.out.println(")");

      // invoke actual method
      //绑定目标和参数
      return m.invoke(target, args);
   }
}

```

