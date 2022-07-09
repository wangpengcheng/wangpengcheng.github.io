---
layout:     post
title:      图说设计模式 学习笔记 (一)
subtitle:   图说设计模式 学习笔记 (一) 创建型模式
date:       2019-11-07
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - C/C++
    - 设计模式
    - UML
---

> 2019-11-07 10:58:49

# 图说设计模式阅读笔记

_参考链接：_ 

- [github地址](https://github.com/me115/design_patterns)
- [23种设计模式C++实现](https://blog.csdn.net/Bing_Lee/article/details/87640606)
- [图说设计模式](https://design-patterns.readthedocs.io/zh_CN/latest/)
- [设计模式C++实现笔记](https://www.jianshu.com/c/c3f6140b8315)
- [C++ 设计模式](https://blog.csdn.net/liang19890820/article/details/66974516)
- [C++设计模式代码地址](https://github.com/Waleon/DesignPatterns)
- [设计模式(可复用面向对象的软件的基础)](https://pan.baidu.com/disk/home#/all?path=%2FLearning_Note%2F%E8%AE%A1%E7%AE%97%E6%9C%BA%E5%9B%BE%E4%B9%A6%2F%E8%BD%AF%E4%BB%B6%E5%B7%A5%E7%A8%8B%E5%92%8C%E9%A1%B9%E7%9B%AE%E7%AE%A1%E7%90%86&vmode=list)
- [C++设计模式视频](https://www.bilibili.com/video/av52251106)

# 0 C++ 面向对象设计

- 封装：隐藏内部实现
- 继承：复用现有代码
- 多态：改写对象行为

设计模式关键在于分解和抽象;

设计模式的主要目的是**易于变化**

**面向对象设计原则--比设计模式更加重要**
违背了设计原则，设计模式是错误的。

- 依赖倒置原则(DIP)--实现隔离变化
  - 高层模块(稳定)不应该依赖于底层模块(变化)，二者都应该依赖于抽象(稳定)。
  - 抽象(稳定)不应该依赖于实现细节(变化)，细节应该依赖于抽象(稳定)。
  - 解释，将需要变动的部分，作为稳定公共抽象基类的子类，将公共方法写在基类中，关键方法作为虚函数交给子类实现。调用公共函数时，只用管理基类指针，使用基类指针调用公共虚函数，就可以调用对应的子函数。实现稳定与不稳定的隔离。
- 开放封闭原则(OCP):
  - 对扩展开放，对更改封闭
  - 类模块应该是可扩展的，但是不可修改。
  - 解释：增加中间抽象，避免过多的更改，保证可扩展性。
- 单一职责原则(SRP)
  - 一个类应该仅有一个引起它变化的原因。
  - 变化的方向隐含着类的责任
- Liskov替换原则(LSP)
  - 子类必须能够替换他们的基类(IS-A)
  - 继承表达抽象。
- 接口隔离(ISP)
  - 不应该强迫客户程序依赖它们不用的方法。
  - 接口应该小而完备。
- 优先使用对象组合，而不是类继承
  - 继承类通常为"白箱复用"，对象组合通常为"黑箱复用"。
  - 继承在某种程度上破坏了封装性，子类父类耦合度高。
  - 对象组合则只被要求被组合的对象具有良好定义的接口，耦合度低。
- 封装变化点
  - 使用封装来创建对象之间的分界层，让设计者可以在分界层中任意一侧进行修改，而不会对另外一侧产生不良的影响，从而实现层次间的松耦合。
- 针对接口编程，而不是针对实现编程
  - 不将变量类型声明为某个具体的类，而是声明为某个接口。
  - 客户端程序无需获知对象的具体类型，只需要知道对象所具有的接口。
  - 减少系统中各个部分的依赖关系，从而实现“高内聚”、“松耦合”的类型设计方案。

## 接口标准化

- 设计习语：描述与特定编程语言相关的低层模式，技巧，惯用法(例如，effictive C++等)
- 设计模式：描述“类与相互通信的对象之间的组织关系，包括它们的角色，职责，协作方式等方面”。
- 架构模式：描述系统中与基本结构组织关系密切的高层模式，包括子系统划分，职责，以及如果组织它们之间关系的规则。

# 0.1 C++ 模板化方法

COF-23设计模式分类

- 从目的来看
  - 创建型：将对象的部分创建工作延迟到子类，或者其它对象，从而应对需求变化为对象创建具体的类时，带来的冲击
  - 结构型：通过类的继承或者对象的组合获得更加灵活的结构，从而应对需求变化为对象的结构带来的冲击。
  - 行为型：通过类继承或者对象的组合，明确类与对象间的职责，从而应对需求变化为多个交互的对象的冲击。
- 从范围来看：
  - 类模式处理与子类的静态关系
  - 对象模式处理对象之间的动态关系。

## 从封装变化角度对模式分类

### 重构的关键技法

- 静态-> 动态
- 早绑定 -> 晚绑定：前调用后—>后调用前
- 继承 -> 组合
- 编译时依赖  -> 运行时依赖
- 紧耦合 -> 松耦合

### “组建协作”模式：

- “框架与应用”分离，“组件协作”模式通过晚期绑定,来实现框架与应用之间的松耦合，是二者之间协作时常用的模式。

#### 模板方法

定义一个操作中的算法的骨架(稳定)，而将一些操作(变化)延迟到子类(通常使用虚函数)。Temlpate Method使得子类可以(复用)一个算法的结构即可重定义(override重写)；某些特定的步奏。

**设计模式的核心，在于查找稳定部分非稳定部分，用不同的模式，来设计两者之间的关系**

![设计模式](https://img-blog.csdn.net/2018051615452165?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1dpS2lfU3U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 看懂UML类图和时序图

## 图说设计模式阅读笔记-创建型模式

## 简单工厂模式( Simple Factory Pattern )
========================================


### 1.1 模式动机
--------------------
考虑一个简单的软件应用场景，一个软件系统可以提供多个外观不同的按钮（如圆形按钮、矩形按钮、菱形按钮等），
这些按钮都源自同一个基类，不过在继承基类后不同的子类修改了部分属性从而使得它们可以呈现不同的外观，如果我们希望在使用这些按钮时，不需要知道这些具体按钮类的名字，只需要知道表示该按钮类的一个参数，并提供一个调用方便的方法，把该参数传入方法即可返回一个相应的按钮对象，此时，就可以使用简单工厂模式。 

### 1.2 模式定义
--------------------
简单工厂模式(Simple Factory Pattern)：又称为静态工厂方法(Static Factory Method)模式，它属于类创建型模式。在简单工厂模式中，可以根据参数的不同返回不同类的实例。简单工厂模式专门定义一个类来负责创建其他类的实例，被创建的实例通常都具有共同的父类。


### 1.3 模式结构
--------------------
简单工厂模式包含如下角色：

- Factory：工厂角色
    工厂角色负责实现创建所有实例的内部逻辑
- Product：抽象产品角色
    抽象产品角色是所创建的所有对象的父类，负责描述所有实例所共有的公共接口
- ConcreteProduct：具体产品角色
    具体产品角色是创建目标，所有创建的对象都充当这个角色的某个具体类的实例。

![_static/SimpleFactory.jpg](https://design-patterns.readthedocs.io/zh_CN/latest/_images/SimpleFactory.jpg)

### 1.4 时序图
--------------------

![时序图](https://design-patterns.readthedocs.io/zh_CN/latest/_images/seq_SimpleFactory.jpg)

### 1.5 代码分析
--------------------

```c++
///////////////////////////////////////////////////////////
//  Factory.cpp
//  Implementation of the Class Factory
//  Created on:      01-十月-2014 18:41:33
//  Original author: colin
///////////////////////////////////////////////////////////

#include "Factory.h"
#include "ConcreteProductA.h"
#include "ConcreteProductB.h"
Product* Factory::createProduct(string proname){
	if ( "A" == proname )
	{
		return new ConcreteProductA();
	}
	else if("B" == proname)
	{
		return new ConcreteProductB();
	}
	return  NULL;
}
```


### 1.6 模式分析
--------------------

- 将对象的创建和对象本身业务处理分离可以降低系统的耦合度，使得两者修改起来都相对容易。
- 在调用工厂类的工厂方法时，由于工厂方法是静态方法，使用起来很方便，可通过类名直接调用，而且只需要传入一个简单的参数即可，在实际开发中，还可以在调用时将所传入的参数保存在XML等格式的配置文件中，修改参数时无须修改任何源代码。
- 简单工厂模式最大的问题在于工厂类的职责相对过重，增加新的产品需要修改工厂类的判断逻辑，这一点与开闭原则是相违背的。
- 简单工厂模式的要点在于：当你需要什么，只需要传入一个正确的参数，就可以获取你所需要的对象，而无须知道其创建细节。

### 实例
--------------------

```c++
/* Product.h */
/* 定义产品 */

class Product
{

public:
	Product();
	virtual ~Product();
	
	virtual void Use() = 0;

};

/* 定义不同的产品 */
#include "Product.h"
#include <string>
using namespace std;

class ConcreteProductA : public Product
{

public:
	ConcreteProductA();
	virtual ~ConcreteProductA();
	virtual void Use();

};
void ConcreteProductA::Use()
{
	cout << "use productB" << endl;
}

class ConcreteProductB : public Product
{
public:
	ConcreteProductB();
	virtual ~ConcreteProductB();
	
	virtual void Use();

};
void ConcreteProductB::Use()
{
	cout << "use productB" << endl;	
}
/* 定义工厂类 */
class Factory
{

public:
	Factory();
	virtual ~Factory();

	static Product * createProduct(string proname);

};
/* 工厂类的实现函数 */
Product* Factory::createProduct(string proname){
	if ( "A" == proname )
	{
		return new ConcreteProductA();
	}
	else if("B" == proname)
	{
		return new ConcreteProductB();
	}
	return  NULL;
}

/* 使用实例 */

int main(int argc, char *argv[])
{
	Product * prod = Factory::createProduct("A");
	prod->Use();
	delete prod;
	return 0;
}

```

简单工厂模式的优点
--------------------

- 工厂类含有必要的判断逻辑，可以决定在什么时候创建哪一个产品类的实例，客户端可以免除直接创建产品对象的责任，而仅仅“消费”产品；简单工厂模式通过这种做法实现了对责任的分割，它提供了专门的工厂类用于创建对象。
- 客户端无须知道所创建的具体产品类的类名，只需要知道具体产品类所对应的参数即可，对于一些复杂的类名，通过简单工厂模式可以减少使用者的记忆量。
- 通过引入配置文件，可以在不修改任何客户端代码的情况下更换和增加新的具体产品类，在一定程度上提高了系统的灵活性。

简单工厂模式的缺点
--------------------

- 由于工厂类集中了所有产品创建逻辑，一旦不能正常工作，整个系统都要受到影响。
- 使用简单工厂模式将会增加系统中类的个数，在一定程序上增加了系统的复杂度和理解难度。
- 系统扩展困难，一旦添加新产品就不得不修改工厂逻辑，在产品类型较多时，有可能造成工厂逻辑过于复杂，不利于系统的扩展和维护。
- 简单工厂模式由于使用了静态工厂方法，造成工厂角色无法形成基于继承的等级结构。

适用环境
--------------------
在以下情况下可以使用简单工厂模式：

- 工厂类负责创建的对象比较少：由于创建的对象较少，不会造成工厂方法中的业务逻辑太过复杂。
- 客户端只知道传入工厂类的参数，对于如何创建对象不关心：客户端既不需要关心创建细节，甚至连类名都不需要记住，只需要知道类型所对应的参数。

模式应用
--------------------
1. JDK类库中广泛使用了简单工厂模式，如工具类java.text.DateFormat，它用于格式化一个本地日期或者时间。

```java

    public final static DateFormat getDateInstance();
    public final static DateFormat getDateInstance(int style);
    public final static DateFormat getDateInstance(int style,Locale 
    locale);
```
2. Java加密技术

获取不同加密算法的密钥生成器::
```java
    KeyGenerator keyGen=KeyGenerator.getInstance("DESede");
```
创建密码器::
```java
    Cipher cp=Cipher.getInstance("DESede");
```
### 1.12 总结
--------------------

- 创建型模式对类的实例化过程进行了抽象，能够将对象的创建与对象的使用过程分离。
- 简单工厂模式又称为静态工厂方法模式，它属于类创建型模式。在简单工厂模式中，可以根据参数的不同返回不同类的实例。简单工厂模式专门定义一个类来负责创建其他类的实例，被创建的实例通常都具有共同的父类。
- 简单工厂模式包含三个角色：工厂角色负责实现创建所有实例的内部逻辑；抽象产品角色是所创建的所有对象的父类，负责描述所有实例所共有的公共接口；具体产品角色是创建目标，所有创建的对象都充当这个角色的某个具体类的实例。
- 简单工厂模式的要点在于：当你需要什么，只需要传入一个正确的参数，就可以获取你所需要的对象，而无须知道其创建细节。
- 简单工厂模式最大的优点在于实现对象的创建和对象的使用分离，将对象的创建交给专门的工厂类负责，但是其最大的缺点在于工厂类不够灵活，增加新的具体产品需要修改工厂类的判断逻辑代码，而且产品较多时，工厂方法代码将会非常复杂。
- 简单工厂模式适用情况包括：工厂类负责创建的对象比较少；客户端只知道传入工厂类的参数，对于如何创建对象不关心。


## 2 工厂方法模式(Factory Method Pattern)

### 2.1 模式动机

不仅仅只是调用对应的创建函数，更多的是定义具体的相关方法和属性(根据输入请求)；现在对该系统进行修改，不再设计一个按钮工厂类来统一负责所有产品的创建，而是将具体按钮的创建过程交给专门的工厂子类去完成，我们先定义一个抽象的按钮工厂类，再定义具体的工厂类来生成圆形按钮、矩形按钮、菱形按钮等，它们实现在抽象按钮工厂类中定义的方法。这种抽象化的结果使这种结构可以在不修改具体工厂类的情况下引进新的产品，如果出现新的按钮类型，只需要为这种新类型的按钮创建一个具体的工厂类就可以获得该新按钮的实例，这一特点无疑使得工厂方法模式具有超越简单工厂模式的优越性，更加符合“开闭原则”。

### 2.2 模式定义

工厂方法模式(Factory Method Pattern)又称为工厂模式，也叫虚拟构造器(Virtual Constructor)模式或者多态工厂(Polymorphic Factory)模式，它属于类创建型模式。在工厂方法模式中，工厂父类负责定义创建产品对象的公共接口，而工厂子类则负责生成具体的产品对象，这样做的目的是将产品类的实例化操作延迟到工厂子类中完成，即通过工厂子类来确定究竟应该实例化哪一个具体产品类。

### 2.3 模式结构和时序图

- Product：抽象产品
- ConcreteProduct：具体产品
- Factory：抽象工厂
- ConcreteFactory：具体工厂

模式结构：

![模式结构](https://design-patterns.readthedocs.io/zh_CN/latest/_images/FactoryMethod.jpg)

时序图：

![时序图](https://design-patterns.readthedocs.io/zh_CN/latest/_images/seq_FactoryMethod.jpg)

### 2.4 代码分析

```c++
///////////////////////////////////////////////////////////
//  ConcreteFactory.cpp
//  Implementation of the Class ConcreteFactory
//  Created on:      02-十月-2014 10:18:58
//  Original author: colin
///////////////////////////////////////////////////////////

#include "Factory.h"
#include "ConcreteFactory.h"
#include "Product.h"
#include <iostream>
using namespace std;

Product* ConcreteFactory::factoryMethod(){

	return  new ConcreteProduct();
}

int main(int argc, char *argv[])
{
    //创建对应的工厂
	Factory * fc = new ConcreteFactory();
	//创建对应产品
    Product * prod = fc->factoryMethod();
	prod->use();
	
	delete fc;
	delete prod;
	
	return 0;
}
```

### 2.6 模式分析

工厂方法模式是简单工厂模式的进一步抽象和推广，将产品细分到各个子场中去。使用不同的厂来创建对应的产品。在工厂方法模式中，核心的工厂类不再负责所有产品的创建，而是将具体创建工作交给子类去做。这个核心类仅仅负责给出具体工厂必须实现的接口，而不负责哪一个产品类被实例化这种细节，这使得工厂方法模式可以允许系统在不修改工厂角色的情况下引进新产品。

### 2.7 实例

日志记录器

某系统日志记录器要求支持多种日志记录方式，如文件记录、数据库记录等，且用户可以根据要求动态选择日志记录方式， 现使用工厂方法模式设计该系统。

结构图：

![结构图](https://design-patterns.readthedocs.io/zh_CN/latest/_images/loger.jpg)

时序图：

![时序图](https://design-patterns.readthedocs.io/zh_CN/latest/_images/seq_loger.jpg)

### 2.8 工厂方法模式的优先

- 工厂无需关心细节，甚至无序知道具体产品的类名称。
- 工厂角色和茶农角色的多态性设计是工厂方法模式的关键。它能确定创建的产品对象；创建细节完全封装在具体工厂内部。所有的具体工厂类都具有同一抽象父类。
- 添加新产品时，只需要添加一个具体的工厂和具体产品就可以了，系统的扩展性较好，完全符合"开闭原则"。

### 2.9 工厂模式的缺点

- 添加新产品时，需要编写新的具体产品类，而且需要提供与之赌赢的工厂类，系统中类的个数将成对增加，会带来额外的编译开销。
- 由于考虑到系统的可扩展性，需要引入抽象层，在客户端代码中均使用抽象层进行定义，增加了系统的抽象性和理解难度，且在实现时可能需要用到DOM、反射等技术，增加了系统的实现难度。

### 2.10 适用环境

在以下情况下可以使用工厂方法模式：

- 一个类不知道它所需要的对象的类：在工厂方法模式中，客户端不需要知道具体产品类的类名，只需要知道所对应的工厂即可，具体的产品对象由具体工厂类创建；客户端需要知道创建具体产品的工厂类。
- 一个类通过其子类来指定创建哪个对象：在工厂方法模式中，对于抽象工厂类只需要提供一个创建产品的接口，而由其子类来确定具体要创建的对象，利用面向对象的多态性和里氏代换原则，在程序运行时，子类对象将覆盖父类对象，从而使得系统更容易扩展。
- 将创建对象的任务委托给多个工厂子类中的某一个，客户端在使用时可以无须关心是哪一个工厂子类创建产品子类，需要时再动态指定，可将具体工厂类的类名存储在配置文件或数据库中。

### 2.12 扩展模式

- 使用多个工厂方法：在抽象工厂角色中可以定义多个工厂方法，从而使具体工厂角色实现这些不同的工厂方法，这些方法可以包含不同的业务逻辑，以满足对不同的产品对象的需求。
- 产品对象的重复使用：工厂对象将已经创建过的产品保存到一个集合（如数组、List等）中，然后根据客户对产品的请求，对集合进行查询。如果有满足要求的产品对象，就直接将该产品返回客户端；如果集合中没有这样的产品对象，那么就创建一个新的满足要求的产品对象，然后将这个对象在增加到集合中，再返回给客户端。
- 多态性的丧失和模式的退化：如果工厂仅仅返回一个具体产品对象，便违背了工厂方法的用意，发生退化，此时就不再是工厂方法模式了。一般来说，工厂对象应当有一个抽象的父类型，如果工厂等级结构中只有一个具体工厂类的话，抽象工厂就可以省略，也将发生了退化。当只有一个具体工厂，在具体工厂中可以创建所有的产品对象，并且工厂方法设计为静态方法时，工厂方法模式就退化成简单工厂模式。

### 2.13 总结

- 工厂方法模式又称为工厂模式，它属于类创建型模式。产品类的是咯花操作延迟到工厂子类中完成，即通过工厂子类来确定具体实例化的类。
- 工厂方法模式包含四个角色：抽象产品是定义产品的接口，是工厂方法模式所创建对象的超类型，即产品对象的共同父类或接口；具体产品实现了抽象产品接口，某种类型的具体产品由专门的具体工厂创建，它们之间往往一一对应；抽象工厂中声明了工厂方法，用于返回一个产品，它是工厂方法模式的核心，任何在模式中创建对象的工厂类都必须实现该接口；具体工厂是抽象工厂类的子类，实现了抽象工厂中定义的工厂方法，并可由客户调用，返回一个具体产品类的实例。
- 系统可以在不修改工厂角色的情况下，引进新产品。
- 适合情况：
    - 一个类不需要直到它所需要的对象的类;
    - 一个类通过其子类来指定创建哪个对象；
    - 客户端产品在使用时无序关心是哪个工厂子类创建子产品，需要时再动态指定。


## 3 抽象工厂模式

### 3.1 模式动机

- 工厂模式中，需要一个模式能够提供多个产品对象。因此需要细化产品，通过产品等级结构(即产品的继承结构)、和产品族(同一个工厂生产的，位于不同产品等级结构的一组产品)
- 当系统所提供的工厂所需生产的具体产品并不是一个简单的对象，而是多个位于不同产品等级结构中属于不同类型的具体产品时需要使用抽象工厂模式。
- 抽象工厂模式是所有形式的工厂模式中最为抽象和最具一般性的一种形态。
- 抽象工厂模式与工厂方法模式最大的区别在于，工厂方法模式针对的是一个产品等级结构，而抽象工厂模式则需要面对多个产品等级结构，一个工厂等级结构可以负责多个不同产品等级结构中的产品对象的创建 。当一个工厂等级结构可以创建出分属于不同产品等级结构的一个产品族中的所有对象时，抽象工厂模式比工厂方法模式更为简单、有效率。
- 抽象工厂模式是所有形式的工厂模式中最为抽象和最具一般性的一种形态。
- 抽象工厂模式相对于工厂模式，更加抽象化。

### 3.2 模式定义

抽象工厂模式(Abstract Factory Pattern)：提供一个创建一系列相关或相互依赖对象的接口，而无须指定它们具体的类。抽象工厂模式又称为Kit模式，属于对象创建型模式。

### 3.3 模式结构和时序图

抽象工厂模式包含如下角色：

- AbstractFactory：抽象工厂
- ConcreteFactory：具体工厂
- AbstractProduct：抽象产品
- Product：具体产品

![抽象工厂模式](https://design-patterns.readthedocs.io/zh_CN/latest/_images/AbatractFactory.jpg)

时序图：

![时序图](https://design-patterns.readthedocs.io/zh_CN/latest/_images/seq_AbatractFactory.jpg)

### 3.5 代码分析

```c
#include <iostream>
#include "AbstractFactory.h"
#include "AbstractProductA.h"
#include "AbstractProductB.h"
#include "ConcreteFactory1.h"
#include "ConcreteFactory2.h"
using namespace std;

int main(int argc, char *argv[])
{
	AbstractFactory * fc = new ConcreteFactory1();
	AbstractProductA * pa =  fc->createProductA();
	AbstractProductB * pb = fc->createProductB();
	pa->use();
	pb->eat();
	
	AbstractFactory * fc2 = new ConcreteFactory2();
	AbstractProductA * pa2 =  fc2->createProductA();
	AbstractProductB * pb2 = fc2->createProductB();
	pa2->use();
	pb2->eat();
}
///////////////////////////////////////////////////////////
//  ConcreteFactory1.cpp
//  Implementation of the Class ConcreteFactory1
//  Created on:      02-十月-2014 15:04:11
//  Original author: colin
///////////////////////////////////////////////////////////

#include "ConcreteFactory1.h"
#include "ProductA1.h"
#include "ProductB1.h"
/* 定义工厂生产线 */
AbstractProductA * ConcreteFactory1::createProductA(){
	return new ProductA1();
}


AbstractProductB * ConcreteFactory1::createProductB(){
	return new ProductB1();
}
///////////////////////////////////////////////////////////
//  ProductA1.cpp
//  Implementation of the Class ProductA1
//  Created on:      02-十月-2014 15:04:17
//  Original author: colin
///////////////////////////////////////////////////////////

#include "ProductA1.h"
#include <iostream>
using namespace std;
/* 产品函数功能 */
void ProductA1::use(){
	cout << "use Product A1" << endl;
}
```

### 3.8 优点

- 隔离了具体类的生成，使得客户端并不需要直到什么被创建。变换具体工厂就更加容易。所有的工厂体都实现了抽象工厂定义中的公共接口。应用抽象工厂模式可以实现高内聚低耦合的设计目的，因此抽象工厂模式得到了广泛的应用。
- 当一个产品族中的多个对象被设计成一起工作时，它能够保证客户端始终只使用同一个产品族中的对象。这对一些需要根据当前环境来决定其行为的软件系统来说，是一种非常实用的设计模式。
- 增加新的具体工厂和产品族很方便，无须修改已有系统，符合“开闭原则”。

### 3.9 缺点

- 添加新的产品对象时，可能需要对抽象工厂角色进行更改。
- 开闭原则的倾斜性（增加新的工厂和产品族容易，增加新的产品等级结构麻烦）。

### 3.10 适用环境

- 一个系统不应当依赖于产品类实例如何被创建、组合和表达的细节，这对于所有类型的工厂模式都是重要的。
- 系统中有多于一个的产品族，而每次只使用其中某一产品族。
- 属于同一个产品族的产品将在一起使用，这一约束必须在系统的设计中体现出来。
- 系统提供一个产品类的库，所有的产品以同样的接口出现，从而使客户端不依赖于具体实现。

### 3.11 应用

在很多软件系统中需要更换界面主题，要求界面中的按钮、文本框、背景色等一起发生改变时，可以使用抽象工厂模式进行设计。调用接口相同，只需要切换style工厂，就能实现内容的更新和设计。

### 3.12 模式扩展

- **开闭原则**：对系统扩展开放，对修改关闭，通过扩展达到增强其功能的目的。
- 抽象工厂模式以一种倾斜的方式支持增加新的产品，为新产品族的增加提供方便，但不能为新的产品等级结构的增加提供这样的方便。
- **工厂模式的退化**:当每个工厂只创建一个产品时，其退化为工厂方法模式。当工厂方法模式中抽象工厂与具体工厂合并时，提供一个统一的工厂来创建产品对象，并将创建的对象的工厂方法设计为静态方法时，工厂方法退化为简单工厂模式。

## 4. 建造者模式

### 4.1 模式动机


复杂对象的构建逐步分离，例如各个成员变量的初始化函数，和对应的不同成员变量的功能函数。不同的建造者实现的函数功能呢不同，最终产生的产品也不同

### 4.2 模式定义

造者模式(Builder Pattern)：将一个复杂对象的构建与它的表示分离，使得同样的构建过程可以创建不同的表示。

建造者模式是一步一步创建一个复杂的对象，它允许用户只通过指定复杂对象的类型和内容就可以构建它们，用户不需要知道内部的具体构建细节。建造者模式属于对象创建型模式。根据中文翻译的不同，建造者模式又可以称为生成器模式。

### 4.3 模式结构

建造者模式包含如下角色：
- Builder：抽象建造者
- ConcreteBuilder：具体建造者
- Director：指挥者
- Product：产品角色

![建造者类图](https://design-patterns.readthedocs.io/zh_CN/latest/_images/Builder.jpg)

时序图

![建造者时序图](https://design-patterns.readthedocs.io/zh_CN/latest/_images/seq_Builder.jpg)

通过选择建造者，然后一步步实现产品的生产。

### 4.5 代码分析

```c

#include <iostream>
#include "ConcreteBuilder.h"
#include "Director.h"
#include "Builder.h"
#include "Product.h"

using namespace std;

int main(int argc, char *argv[])
{
	//获取建造者
	ConcreteBuilder * builder = new ConcreteBuilder();
	//操作者
	Director  director;
	//设置建造者
	director.setBuilder(builder);
	//创建产品
	Product * pd =  director.constuct();
	pd->show();
	
	delete builder;
	delete pd;
	return 0;
}
/* 建造者的定义 */

///////////////////////////////////////////////////////////
//  ConcreteBuilder.cpp
//  Implementation of the Class ConcreteBuilder
//  Created on:      02-十月-2014 15:57:03
//  Original author: colin
///////////////////////////////////////////////////////////

#include "ConcreteBuilder.h"


ConcreteBuilder::ConcreteBuilder(){

}

ConcreteBuilder::~ConcreteBuilder(){

}

void ConcreteBuilder::buildPartA(){
	m_prod->setA("A Style "); //不同的建造者，可以实现不同产品的建造  
}


void ConcreteBuilder::buildPartB(){
	m_prod->setB("B Style ");
}


void ConcreteBuilder::buildPartC(){
	m_prod->setC("C style ");
}
/* 定义建造函数 */
///////////////////////////////////////////////////////////
//  Director.cpp
//  Implementation of the Class Director
//  Created on:      02-十月-2014 15:57:01
//  Original author: colin
///////////////////////////////////////////////////////////

#include "Director.h"

Director::Director(){
}

Director::~Director(){
}

Product* Director::constuct(){
	m_pbuilder->buildPartA();
	m_pbuilder->buildPartB();
	m_pbuilder->buildPartC();
	
	return m_pbuilder->getResult();
}


void Director::setBuilder(Builder* buider){
	m_pbuilder = buider;
}


```

### 4.6 模式分析

建造者类中定义了产品的创建方法和返回方法，并使用了指挥者，进行隔离和指挥。它和工厂模式有所区别，工厂模式是将所有的产品添加到工厂中，含有全部的对象和方法，这个是可以有选择的创建工厂对象。避免了重用。**主要用于，本身对象建立十分复杂的类的构建**

在客户端代码中，无须关心产品对象的具体组装过程，只需确定具体建造者的类型即可，建造者模式将复杂对象的构建与对象的表现分离开来，这样使得同样的构建过程可以创建出不同的表现。

### 4.7 实例：

实例：KFC套餐

类图设计如下

![KFC类图](https://design-patterns.readthedocs.io/zh_CN/latest/_images/KFCBuilder.jpg)

### 4.8 优点

- 在建造者模式中， 客户端不必知道产品内部组成的细节，将产品本身与产品的创建过程解耦，使得相同的创建过程可以创建不同的产品对象。
- 建造者相对独立，用户使用不同的建造者创建不同的产品对象
- 可以更加精细的控制产品的创建过程
- 增加新的具体建造者无须修改原有类库的代码，指挥者类针对抽象者类编程，扩展系统方便。

### 4.9 缺点

- 产品相似，当需求产品间差距较大时，不适合。
- 如果内部变化复杂。可能会导致需要定义很多类，导致代码庞大。

### 4.10 适用环境

- 生成对象有复杂的内部结构，产品对象有多个成员属性
- 产品对象属性，相互依赖,需要指定其生成顺序。
- 对象的创建过程独立于创建该对象的类。在建造者模式中引入了指挥者类，将创建过程封装在指挥者类中，而不在建造者类中。
- 隔离复杂对象的创建和使用，并使得相同的创建过程可以创建不同的产品。

### 4.11 模式应用

在很多游戏软件中，地图包括天空、地面、背景等组成部分，人物角色包括人体、服装、装备等组成部分，可以使用建造者模式对其进行设计，通过不同的具体建造者创建不同类型的地图或人物。

### 4.12 模式扩展

建造者模式的简化:

- 省略抽象建造者角色：如果系统中只需要一个具体建造者的话，可以省略掉抽象建造者。
- 省略指挥者角色：在具体建造者只有一个的情况下，如果抽象建造者角色已经被省略掉，那么还可以省略指挥者角色，让Builder角色扮演指挥者与建造者双重角色。

建造者模式与抽象工厂模式的比较:

- **建造者模式返回一个组装好的完整产品 ，而抽象工厂模式返回一系列相关的产品，这些产品位于不同的产品等级结构，构成了一个产品族。**
- 抽象工厂模式中，客户端需要实例化工厂类，然后再进行创建产品对象，建造者模式中，客户端可以不直接调用建造者的相关方法，而是通过指挥者类来指导如何生成对象。**它侧重于一步步构造一个复杂对象，返回一个完整的对象。**
- 如果将抽象工厂模式看成 **汽车配件**生产工厂 ，生产一个产品族的产品，那么建造者模式就是一个 **汽车组装工厂**，通过对部件的组装可以返回一辆完整的汽车。--就是主要负责复杂组成的类的生成。

### 4.13 总结

- 建造者模式将一个复杂对象的构建与它的表示分离，使得同样的构建过程可以创建不同的表示。
- 建造者模式的主要优点在于客户端不必知道产品内部组成的细节，将**产品本身**与产品的**创建过程**解耦。
- 建造者模式适用于，需要生成的产品对象有复杂的内部结构的情况，成员复杂，并且相互间具有依赖性的情况。

## 5. 单例模式

### 5.1 模式动机

系统中的某些类，只能有一个实例，如时间统计函数，打印机等。一般会被实例化为全局静态函数。

### 5.2 模式定义

单例模式(Singleton Pattern)：单例模式确保某一个类只有一个实例，而且自行实例化并向整个系统提供这个实例，这个类称为单例类，它提供全局访问的方法。

单例模式的要点有三个：一是某个类只能有一个实例；二是它必须自行创建这个实例；三是它必须自行向整个系统提供这个实例。单例模式是一种对象创建型模式。单例模式又名单件模式或单态模式。

### 5.3 模式结构

单例模式值包含单例(Singleton)一个角色

![单例模式](https://design-patterns.readthedocs.io/zh_CN/latest/_images/Singleton.jpg)

![时序图](https://design-patterns.readthedocs.io/zh_CN/latest/_images/seq_Singleton.jpg)

### 5.5 代码分析

```c
#include <iostream>
#include "Singleton.h"
using namespace std;

int main(int argc, char *argv[])
{
	//获取对象指针
	Singleton * sg = Singleton::getInstance();
	//使用对象方法
	sg->singletonOperation();
	
	return 0;
}
///////////////////////////////////////////////////////////
//  Singleton.cpp
//  Implementation of the Class Singleton
//  Created on:      02-十月-2014 17:24:46
//  Original author: colin
///////////////////////////////////////////////////////////

#include "Singleton.h"
#include <iostream>
using namespace std;

//创建静态单例模式
Singleton * Singleton::instance = NULL;
Singleton::Singleton(){

}

Singleton::~Singleton(){
	delete instance;
}

Singleton* Singleton::getInstance(){
	if (instance == NULL)
	{
		instance = new Singleton();
	}
	
	return  instance;
}


void Singleton::singletonOperation(){
	cout << "singletonOperation" << endl;
}
```

### 5.6 模式分析

单例模式的目的是保证一个类仅有一个实例，并提供一个访问它的全局访问点。单例类拥有一个私有构造函数，确保用户无法通过new关键字直接实例化它。除此之外，该模式中包含一个静态私有成员变量与静态公有的工厂方法，该工厂方法负责检验实例的存在性并实例化自己，然后存储在静态成员变量中，以确保只有一个实例被创建。

在单例模式的实现过程中，需要注意如下三点：

- 单例类的构造函数为私有；
- 提供一个自身的静态私有成员变量；
- 提供一个公有的静态工厂方法。

### 5.8 优点

- 提供了对唯一实例的受控访问。因为单例类封装了它的唯一实例，所以它可以严格控制客户怎样以及何时访问它，并为设计及开发团队提供了共享的概念。
- 由于在系统内存中只存在一个对象，因此可以节约系统资源，对于一些需要频繁创建和销毁的对象，单例模式无疑可以提高系统的性能。
- 允许可变数目的实例。我们可以基于单例模式进行扩展，使用与单例控制相似的方法来获得指定个数的对象实例。

### 5.9 缺点

- 由于单例模式中没有抽象层，因此单例类的扩展有很大的困难。
- 单例类的职责过重，在一定程度上违背了“单一职责原则”。
- 滥用单例将带来一些负面问题，如为了节省资源将数据库连接池对象设计为单例类，可能会导致共享连接池对象的程序过多而出现连接池溢出；现在很多面向对象语言(如Java、C#)的运行环境都提供了自动垃圾回收的技术，因此，如果实例化的对象长时间不被利用，系统会认为它是垃圾，会自动销毁并回收资源，下次利用时又将重新实例化，这将导致对象状态的丢失。

### 5.10 适用环境

- 系统只需要一个实例对象，如系统要求提供一个唯一的序列号生成器，或者需要考虑资源消耗太大而只允许创建一个对象。
- 客户调用类的单个实例只允许使用一个公共访问点，除了该公共访问点，不能通过其他途径访问该实例。
- 在一个系统中要求一个类只有一个实例时才应当使用单例模式。反过来，如果一个类可以有几个实例共存，就需要对单例模式进行改进，使之成为多例模式。

### 5.11 模式应用

一个具有自动编号主键的表可以有多个用户同时使用，但数据库中只能有一个地方分配下一个主键编号，否则会出现主键重复，因此该主键编号生成器必须具备唯一性，可以通过单例模式来实现。



