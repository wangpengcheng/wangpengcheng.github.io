---
layout:     post
title:      图说设计模式 学习笔记 (二)
subtitle:   图说设计模式 学习笔记 (二) 结构型模式
date:       2019-11-10
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - C/C++
    - 设计模式
    - UML
---

> 2019-11-10 21:41:49

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

# 结构型模式

结构型模式(Structural Pattern)描述如何将类或者对 象结合在一起形成更大的结构，就像搭积木，可以通过 简单积木的组合形成复杂的、功能更为强大的结构。

结构型模式可以分为类结构型模式和对象结构型模式：

- 类结构型模式关心类的组合，由多个类可以组合成一个更大的系统，在类结构型模式中一般只存在继承关系和实现关系。 
- 对象结构型模式关心类与对象的组合，通过关联关系使得在一 个类中定义另一个类的实例对象，然后通过该对象调用其方法。 
  

根据“合成复用原则”，在系统中尽量使用关联关系来替代继 承关系，因此大部分结构型模式都是对象结构型模式。

结构型模式的重点在于外观模式和适配器模式，享元模式不怎么重要。

## 1. 适配器模式

_参考链接：_ [C++ 适配器模式](https://blog.csdn.net/liang19890820/article/details/66973296)

- 客户端可以通过目标类的结构访问它所提供的服务。但是接口不一定是客户端所期望的。
- 适配器模式，提供这样的接口来，实现用户期望的结果。
- 适配器模式定义一个包装类，包装不兼容接口的对象，给适配对象(客户端)；提供对应的接口。

### 1.2 模式定义

适配器模式(Adapter Pattern) ：将一个接口转换成客户希望的另一个接口，适配器模式使接口不兼容的那些类可以一起工作，其别名为包装器(Wrapper)。适配器模式既可以作为类结构型模式，也可以作为对象结构型模式。

**适配器模式（Adapter Pattern）是一种补救模式，将一个类的接口转换成客户希望的另外一个接口，从而使原本由于接口不兼容而不能一起工作的类可以一起工作。**

### 1.3 模式结构

适配器模式包含如下角色：

- Target：目标抽象类
- Adapter：适配器类
- Adaptee：适配者类
- Client：客户类

适配器模式有对象适配器和类适配器两种实现：

对象适配器：

![对象适配器](https://design-patterns.readthedocs.io/zh_CN/latest/_images/Adapter.jpg)

类适配器

![类适配器模式](https://design-patterns.readthedocs.io/zh_CN/latest/_images/Adapter_classModel.jpg)

![时序图](https://design-patterns.readthedocs.io/zh_CN/latest/_images/seq_Adapter.jpg)

### 1.5 代码分析

```c
/* main function */
#include <iostream>
#include "Adapter.h"
#include "Adaptee.h"
#include "Target.h"

using namespace std;

int main(int argc, char *argv[])
{
    //创建适配器
	Adaptee * adaptee  = new Adaptee();
	//获取目标
    Target * tar = new Adapter(adaptee);
	//执行方法
    tar->request();
	
	return 0;
}
/* 定义适配器 */
///////////////////////////////////////////////////////////
//  Adapter.h
//  Implementation of the Class Adapter
//  Created on:      03-十月-2014 17:32:00
//  Original author: colin
///////////////////////////////////////////////////////////

#ifndef ADAPTER_H
#define ADAPTER_H

#include "Target.h"
#include "Adaptee.h"

class Adapter : public Target
{

public:
	Adapter(Adaptee *adaptee);
	virtual ~Adapter();

	virtual void request();

private:
	Adaptee* m_pAdaptee;

};
#endif // ADAPTER_H
/* 适配器实现 */
///////////////////////////////////////////////////////////
//  Adapter.cpp
//  Implementation of the Class Adapter
//  Created on:      03-十月-2014 17:32:00
//  Original author: colin
///////////////////////////////////////////////////////////

#include "Adapter.h"

Adapter::Adapter(Adaptee * adaptee){
	m_pAdaptee =  adaptee;
}
Adapter::~Adapter(){

}
/* 实现请求方法 */
void Adapter::request(){
	m_pAdaptee->specificRequest();
}

/* 适配者方法 */

///////////////////////////////////////////////////////////
//  Adaptee.h
//  Implementation of the Class Adaptee
//  Created on:      03-十月-2014 17:32:00
//  Original author: colin
//////////////////////////////////////////////////////////D

#ifndef ADAPTEE_H
#define ADAPTEE_H
/* 关键类适配器 */
class Adaptee
{

public:
	Adaptee();
	virtual ~Adaptee();

	void specificRequest();

};
#endif // !ADAPTEE_H

```
### 1.8 优点

- 将目标类和适配者类解耦，通过引入一个适配器类来重用现有的适配者类，而无须修改原有代码。
- 增加了类的透明性和复用性，将具体的实现封装在适配者类中，对于客户端类来说是透明的，而且提高了适配者的复用性。
- 灵活性和扩展性都非常好，通过使用配置文件，可以很方便地更换适配器，也可以在不修改原有代码的基础上增加新的适配器类，完全符合“开闭原则”。
  

**类适配器模式还具有如下优点：**
由于适配器类是适配者类的子类，因此可以在适配器类中置换一些适配者的方法，使得适配器的灵活性更强。
**对象适配器模式还具有如下优点：**
一个对象适配器可以把多个不同的适配者适配到同一个目标，也就是说，同一个适配器可以把适配者类和它的子类都适配到目标接口。

### 1.9 缺点

- 类适配器：
  - 对于Java、C#等不支持多重继承的语言，一次最多只能适配一个适配者类，而且目标抽象类只能为抽象类，不能为具体类，其使用有一定的局限性，不能将一个适配者类和它的子类都适配到目标接口。
- 对象适配器：
  - 与类适配器模式相比，要想置换适配者类的方法就不容易。如果一定要置换掉适配者类的一个或多个方法，就只好先做一个适配者类的子类，将适配者类的方法置换掉，然后再把适配者类的子类当做真正的适配者进行适配，实现过程较为复杂。

### 1.10 使用环境

- 系统需要使用现有的类，而这些类的接口不符合系统的需要。
- 想要建立一个可以重复使用的类，用于与一些彼此之间没有太大关联的一些类，包括一些可能在将来引进的类一起工作。

### 1.11 模式应用

java的JDBC给出一个客户端通用的抽象接口，每个具体数据库引擎（如SQL Server、Oracle、MySQL等）的JDBC驱动软件都是一个介于JDBC接口和数据库引擎接口之间的适配器软件。

### 1.12 模式扩展

当不需要全部实现接口提供的方法时，可先设计一个抽象类实现接口，并为该接口中每个方法提供一个默认实现（空方法），那么该抽象类的子类可有选择地覆盖父类的某些方法来实现需求，它适用于一个接口不想使用其所有的方法的情况。因此也称为单接口适配器模式。

## 2 桥接模式

桥接模式将继承关系转换为关联关系，从而降低了类与类之间的耦合，减少了代码编写量。适用于对于多种属性组合的情况。

### 2.2 模式定义

桥接模式(Bridge Pattern)：将抽象部分与它的实现部分分离，使它们都可以独立地变化。它是一种对象结构型模式，又称为柄体(Handle and Body)模式或接口(Interface)模式。

### 2.3 模式结构

桥接模式包含如下角色：

- Abstraction：抽象类
- RefinedAbstraction：扩充抽象类
- Implementor：实现类接口
- ConcreteImplementor：具体实现类

![桥接模式](https://design-patterns.readthedocs.io/zh_CN/latest/_images/Bridge.jpg)

时序图

![时序图](https://design-patterns.readthedocs.io/zh_CN/latest/_images/seq_Bridge.jpg)

```c
/* main.c */
#include <iostream>
#include "ConcreteImplementorA.h"
#include "ConcreteImplementorB.h"
#include "RefinedAbstraction.h"
#include "Abstraction.h"

using namespace std;

int main(int argc, char *argv[])
{
	
	Implementor * pImp = new ConcreteImplementorA();
	Abstraction * pa = new RefinedAbstraction(pImp);
	//注意这里是对公共抽象接口的调用
	pa->operation();
	
	Abstraction * pb = new RefinedAbstraction(new ConcreteImplementorB());
	pb->operation();
	
	delete pa;
	delete pb;
	
	return 0;
}
/* RefinedAbstraction.h */

///////////////////////////////////////////////////////////
//  RefinedAbstraction.h
//  Implementation of the Class RefinedAbstraction
//  Created on:      03-十月-2014 18:12:43
//  Original author: colin
///////////////////////////////////////////////////////////

#if !defined(EA_4BA5BE7C_DED5_4236_8362_F2988921CFA7__INCLUDED_)
#define EA_4BA5BE7C_DED5_4236_8362_F2988921CFA7__INCLUDED_

#include "Abstraction.h"

class RefinedAbstraction : public Abstraction
{

public:
	RefinedAbstraction();
	RefinedAbstraction(Implementor* imp);
	virtual ~RefinedAbstraction();

	virtual void operation();

};

RefinedAbstraction::RefinedAbstraction(){

}

RefinedAbstraction::RefinedAbstraction(Implementor* imp)
	:Abstraction(imp)
{
}

RefinedAbstraction::~RefinedAbstraction(){

}

void RefinedAbstraction::operation(){
	cout << "do something else ,and then " << endl;
	m_pImp->operationImp();
}

 
#endif // !defined(EA_4BA5BE7C_DED5_4236_8362_F2988921CFA7__INCLUDED_)

///////////////////////////////////////////////////////////
//  ConcreteImplementorA.cpp
//  Implementation of the Class ConcreteImplementorA
//  Created on:      03-十月-2014 18:12:43
//  Original author: colin
///////////////////////////////////////////////////////////

#include "ConcreteImplementorA.h"
#include <iostream>
using namespace std;

ConcreteImplementorA::ConcreteImplementorA(){

}
ConcreteImplementorA::~ConcreteImplementorA(){

}
//A中定义的操作函数，与B基本相同
void ConcreteImplementorA::operationImp(){
	cout << "imp in ConcreteImplementorA style." << endl;
}

```

### 2.6 模式分析

桥接模式就是在适配器模式的基础上，更改为了多种可桥接的方法。**主要目的是将抽象部分与它的实现部分分离，使得他们都可以独立进行变化。抽象类只控制抽象部分，具体的实现部分由继承的实现类本身来完成。通过公共虚函数接口的实现或者重载，来对其进行控制。**

- 抽象化：抽象化就是忽略一些信息，把不同的实体当作同样的实体对待。在面向对象中，将对象的共同性质抽取出来形成类的过程即为抽象化的过程。
- 抽象化：抽象化就是忽略一些信息，把不同的实体当作同样的实体对待。在面向对象中，将对象的共同性质抽取出来形成类的过程即为抽象化的过程。
- 实现化：针对抽象化给出的具体实现，就是实现化，抽象化与实现化是一对互逆的概念，实现化产生的对象比抽象化更具体，是对抽象化事物的进一步具体化的产物。
- 脱耦：脱耦就是将抽象化和实现化之间的耦合解脱开，或者说是将它们之间的强关联改换成弱关联，将两个角色之间的继承关系改为关联关系。桥接模式中的所谓脱耦，就是指在一个软件系统的抽象化和实现化之间使用关联关系（组合或者聚合关系）而不是继承关系，从而使两者可以相对独立地变化，这就是桥接模式的用意。

### 2.7 实例

参考示例：[链接](https://blog.csdn.net/liang19890820/article/details/79501177)

### 2.8 优点

- 分离抽象接口及其实现部分。
- 桥接模式有时类似于多继承方案，但是多继承方案违背了类的单一职责原则（即一个类只有一个变化的原因），复用性比较差，而且多继承结构中类的个数非常庞大，桥接模式是比多继承方案更好的解决方法。
- 桥接模式提高了系统的可扩充性，在两个变化维度中任意扩展一个维度，都不需要修改原有系统。
- 实现细节对客户透明，可以对用户隐藏实现细节。

### 2.9 缺点

- 桥接模式的引入会增加系统的理解与设计难度，由于聚合关联关系建立在抽象层，要求开发者针对抽象进行设计与编程。 
- 桥接模式要求正确识别出系统中两个独立变化的维度，因此其使用范围具有一定的局限性。

### 2.10 适用环境

- 在两个具体的角色之间，建立静态的继承联系，通过桥接模式，将两个函数进行抽象化并建立相关的联系。
- 抽象化的角色之和实现化的角色可以以继承的方式独立扩展而互不影响。
- 一个类存在两个独立变化的维度，且这两个唯独都需要进行扩展。
- 使用继承来实现，抽象化角色和具体角色的独立
- 不希望使用继承。

## 3 装饰模式

对于类或者对象的行为增加，一般有两种方式：

- 继承机制:使用继承机制，直接进行函数的添加。
- 关联机制，即将一个类的对象嵌入另外一个对象中，由另外一个对象来决定是否调用嵌入对象的行为，以便扩展自己的行为。，我们称这个嵌入的对象为“装饰器”。

## 3.2 模式定义：

装饰模式(Decorator Pattern) ：动态地给一个对象增加一些额外的职责(Responsibility)，就增加对象功能来说，装饰模式比生成子类实现更为灵活。其别名也可以称为包装器(Wrapper)，与适配器模式的别名相同，但它们适用于不同的场合。根据翻译的不同，装饰模式也有人称之为“油漆工模式”，它是一种对象结构型模式。

### 3.3 模式结构

包含角色如下：

- Component: 抽象构件
- ConcreteComponent: 具体构件
- Decorator: 抽象装饰类
- ConcreteDecorator: 具体装饰类

![结构模式图](https://design-patterns.readthedocs.io/zh_CN/latest/_images/Decorator.jpg)

时序图:

![时序图](https://design-patterns.readthedocs.io/zh_CN/latest/_images/seq_Decorator.jpg)

**装饰模式中的装饰类一般拥有为抽象构建成员，通过具体构建类来进行添加操作，成为具体的装饰类，最终实现具体的类操作的扩展，装饰器一般也来自于基础的抽象构件**

### 3.7 实例

> 星巴克（Starbucks）

在星巴克购买咖啡时，可以要求在其中加入各种调味品（辅料）。调味品很多，有些不收费（例如：白砂糖、香草粉等），有些则需要额外收费（例如：奶油、摩卡、糖浆等），所以充分利用起来吧！倘若咖啡不带劲，我们想要添加奶油、摩卡和糖浆，这时，就可以利用装饰者模式思想来实现。

### 3.8 代码实现

```c
/* 所有饮料的基类实现，即抽象构建实现 */
// component.h
#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

using namespace std;

// 所有饮料的基类
class IBeverage
{
public:
    virtual string Name() = 0;  // 名称
    virtual double Cost() = 0;  // 价钱
};

#endif // COMPONENT_H

/* 由基类，创建具体构建 */
// concrete_component.h
#ifndef CONCRETE_COMPONENT_H
#define CONCRETE_COMPONENT_H

#include "component.h"

/********** 具体的饮料（咖啡）**********/

// 黑咖啡，属于混合咖啡
class HouseBlend : public IBeverage
{
public:
    string Name() {
        return "HouseBlend";
    }

    double Cost() {
        return 30.0;
    }
};

// 深度烘培咖啡豆
class DarkRoast : public IBeverage
{
public:
    string Name() {
        return "DarkRoast";
    }

    double Cost() {
        return 28.5;
    }
};

#endif // CONCRETE_COMPONENT_H

/* 以抽象构建为成员，创建装饰类 */
// decorator.h
#ifndef DECORATOR_H
#define DECORATOR_H

#include "component.h"

// 调味品
class CondimentDecorator : public IBeverage
{
public :
    CondimentDecorator(IBeverage *beverage) : m_pBeverage(beverage) {}

    string Name() {
        return m_pBeverage->Name();
    }

    double Cost() {
        return m_pBeverage->Cost();
    }

protected :
    IBeverage *m_pBeverage;
} ;

#endif // DECORATOR_H

/* 添加具体装饰 */
// concrete_decorator.h
#ifndef CONCRETE_DECORATOR_H
#define CONCRETE_DECORATOR_H

#include "decorator.h"

/********** 具体的饮料（调味品）**********/

// 奶油
class Cream : public CondimentDecorator
{
public:
    Cream(IBeverage *beverage) : CondimentDecorator(beverage) {}

    string Name() {
        return m_pBeverage->Name() + " Cream";
    }

    double Cost() {
        return m_pBeverage->Cost() + 3.5;
    }
};

// 摩卡
class Mocha : public CondimentDecorator
{
public:
    Mocha(IBeverage *beverage) : CondimentDecorator(beverage) {}

    string Name() {
        return m_pBeverage->Name() + " Mocha";
    }

    double Cost() {
        return m_pBeverage->Cost() + 2.0;
    }
};

// 糖浆
class Syrup : public CondimentDecorator
{
public:
    Syrup(IBeverage *beverage) : CondimentDecorator(beverage) {}

    string Name() {
        return m_pBeverage->Name() + " Syrup";
    }

    double Cost() {
        return m_pBeverage->Cost() + 3.0;
    }
};

#endif // CONCRETE_DECORATOR_H

/* 创建客户端并使用 */

// main.cpp
#include "concrete_component.h"
#include "concrete_decorator.h"
#include <iostream>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete(p); (p)=NULL;} }
#endif

int main()
{
    /********** 黑咖啡 **********/
    IBeverage *pHouseBlend = new HouseBlend();
    cout << pHouseBlend->Name() << " : " << pHouseBlend->Cost() << endl;

    // 黑咖啡 + 奶油
    CondimentDecorator *pCream = new Cream(pHouseBlend);
    cout << pCream->Name() << " : " << pCream->Cost() << endl;

    // 黑咖啡 + 摩卡
    CondimentDecorator *pMocha = new Mocha(pHouseBlend);
    cout << pMocha->Name() << " : " << pMocha->Cost() << endl;

    // 黑咖啡 + 糖浆
    CondimentDecorator *pSyrup = new Syrup(pHouseBlend);
    cout << pSyrup->Name() << " : " << pSyrup->Cost() << endl;

    /********** 深度烘培咖啡豆 **********/
    IBeverage *pDarkRoast = new DarkRoast();
    cout << pDarkRoast->Name() << " : " << pDarkRoast->Cost() << endl;

    // 深度烘培咖啡豆 + 奶油
    CondimentDecorator *pCreamDR = new Cream(pDarkRoast);
    cout << pCreamDR->Name() << " : " << pCreamDR->Cost() << endl;

    // 深度烘培咖啡豆 + 奶油 + 摩卡
    CondimentDecorator *pCreamMocha = new Mocha(pCreamDR);
    cout << pCreamMocha->Name() << " : " << pCreamMocha->Cost() << endl;

    // 深度烘培咖啡豆 + 奶油 + 摩卡 + 糖浆
    CondimentDecorator *pCreamMochaSyrup = new Syrup(pCreamMocha);
    cout << pCreamMochaSyrup->Name() << " : " << pCreamMochaSyrup->Cost() << endl;

    SAFE_DELETE(pSyrup);
    SAFE_DELETE(pMocha);
    SAFE_DELETE(pCream);
    SAFE_DELETE(pHouseBlend);

    SAFE_DELETE(pCreamMochaSyrup);
    SAFE_DELETE(pCreamMocha);
    SAFE_DELETE(pCreamDR);
    SAFE_DELETE(pDarkRoast);

    getchar();

    return 0;
}
/*
输出如下：
HouseBlend : 30 
HouseBlend Cream : 33.5 
HouseBlend Mocha : 32 
HouseBlend Syrup : 33 
DarkRoast : 28.5 
DarkRoast Cream : 32 
DarkRoast Cream Mocha : 34 
DarkRoast Cream Mocha Syrup : 37

*/
```
如上所示，构建，通过共同具体的装饰器，添加种种不同的功能。装饰器通过，添加基类的成员变量，实现模拟继承。

### 3.8 优点

- 装饰模式提供比继承更多的灵活度
- 可以通过配置文件在运行时，选择不同的装饰器，从而实现不同的行为。
- 可以通过不同具体装饰器的排列组合，创造出不同行为的组合。为具体的基础构件，创建更加强大的对象。
- 具体构件类与具体装饰类可以独立变化，用户可以根据需要增加新的具体构件类和具体装饰类，在使用时再对其进行组合，原有代码无须改变，符合“开闭原则”。
  
### 3.9 缺点

- 使用装饰模式时，因为层层递进关系，会产生很多无用的小对象；和具体的装饰类。
- 这种比继承更加灵活机动的特性，也同时意味着装饰模式比继承更加易于出错，排错也很困难，对于多次装饰的对象，调试时寻找错误可能需要逐级排查，较为烦琐。

## 4 外观模式

外观模式(Facade Pattern)：外部与一个子系统的通信必须通过一个统一的外观对象进行，为子系统中的一组接口提供一个一致的界面，外观模式定义了一个高层接口，这个接口使得这一子系统更加容易使用。外观模式又称为门面模式，它是一种对象结构型模式。

Façade 为子系统中的一组接口提供了一个统一的高层接口，该接口使得子系统更加容易使用。

适配器模式和装饰者模式的区别：

**适配器将一个对象包装起来以改变其接口；装饰者将一个对象包装起来以增强新的行为和责任；而外观将一群对象包装起来以简化其接口。要注意装饰者模式的两个抽象类，一个是Compent, 还有一个是Decorator。**

### 4.3 模式结构

外观模式包含角色如下：

- Facade: 外观角色;模式的核心，被 Client 调用，知晓相关子系统的功能和责任。在正常情况下，它将所有从 Client 发来的请求委派到相应的子系统去，让子系统处理.
- SubSystem:子系统角色;可以同时有一个或者多个子系统，子系统可以是一个单独的类或类的集合。每个子系统都可以被 Client 直接调用，或者被 Facade 调用，它处理由 Facade 传过来的请求。子系统并不知道 Facade 的存在，对于子系统而言，Facade 仅仅是另外一个 Client 而已。

![外观模式类结构](https://design-patterns.readthedocs.io/zh_CN/latest/_images/Facade.jpg)

时序图：

![外观模式时序图](https://design-patterns.readthedocs.io/zh_CN/latest/_images/seq_Facade.jpg)


### 4.5 案例分析

京东快递，下单过程，需要有订单团队，供应商和快递员的三方共同协作才能实现：

#### 4.5.1 创建子系统

子系统包含 3 个，订单团队（确认付款、联系供应商）、供应商（确认库存、包装、联系快递）、快递员（分配人员、派送包裹）。

```c
// sub_system.h
#ifndef SUB_SYSTEM_H
#define SUB_SYSTEM_H

#include <iostream>
#include <string>
#include <windows.h>

const std::string c_stateToStrCourier[] = { "收到", "验证可达性", "分配人员", "派送包裹", "获取交货确认", "完成" };
const std::string c_stateToStrVendor[] = { "收到", "确认库存", "从仓库得到物品", "包装", "联系快递员", "完成" };
const std::string c_stateToStrOrderTeam[] = { "收到", "确认付款", "联系供应商", "完成" };
const int c_nMsec = 300;  // 休眠时间（毫秒） - Sleep(c_nMsec) 处可以替换为一些有用的代码

// 订单团队
class OrderTeam
{
public:
    void submitRequest() {
        m_nState = 0;
    }

    // 检测状态
    bool checkStatus() {
        std::cout << "订单团队 - 当前状态：" << c_stateToStrOrderTeam[m_nState] << std::endl;

        Sleep(c_nMsec);
        m_nState++;

        return (m_nState == Complete);
    }

private:
    enum States {
        Received,  // 收到
        VerifyPayment,  // 确认付款
        ContactVendor,  // 联系供应商
        Complete  // 完成
    };
    int m_nState;
};

// 供应商
class Vendor
{
public:
    void submitRequest() {
        m_nState = 0;
    }

    // 检测状态
    bool checkStatus() {
        std::cout << "供应商 - 当前状态：" << c_stateToStrVendor[m_nState] << std::endl;

        Sleep(c_nMsec);
        m_nState++;

        return (m_nState == Complete);
    }

private:
    enum States {
        Received,  // 收到
        VerifyInventory,  // 确认库存
        GetItemFromWareHouse,  // 从仓库得到物品
        PackItem,  // 包装
        ContactCourier,  // 联系快递员
        Complete  // 完成
    };
    int m_nState;
};

// 快递员
class Courier
{
public:
    // 将请求转发给快递员
    void submitRequest() {
        m_nState = 0;
    }

    // 检测状态
    bool checkStatus() {
        std::cout << "快递员 - 当前状态：" << c_stateToStrCourier[m_nState] << std::endl;

        Sleep(c_nMsec);
        m_nState++;

        return (m_nState == Complete);
    }

private:
    enum States {
        Received,  // 收到
        VerifyReachbility,  // 验证可达性
        AssignPerson,  // 分配人员
        DispatchPackage,  // 派送包裹
        GetDeliveryConfirmation,  // 获取交货确认
        Complete  // 完成
    };
    int m_nState;
};

#endif // SUB_SYSTEM_H
```
#### 4.5.2 创建外观

```c
// facade.h
#ifndef FACADE_H
#define FACADE_H

#include "sub_system.h"

// 网购外观
class OnlineShoppingFacade
{
public:
    OnlineShoppingFacade() {
        m_nCount = 0;
    }

    // 返回跟踪次数
    int followupNum() {
        return m_nCount;
    }

    // 提交订单
    void submitRequest() {
        m_nState = 0;
    }

    // 跟踪订单
    bool checkStatus(){
        // 收到订单请求
        switch (m_nState) {
        case Received:
            m_nState++;
            // 将请求转发给订单团队
            m_order.submitRequest();
            std::cout << "********** 提交给订单团队，跟踪次数：" << m_nCount << " **********" << std::endl;
            break;
        case SubmittedToOrderTeam:
            // 如果订单团队完成验证，则向供应商发出请求
            if (m_order.checkStatus()) {
                m_nState++;
                m_vendor.submitRequest();
                std::cout << "********** 提交给供应商，跟踪次数：" << m_nCount << " **********" << std::endl;
            }
            break;
        case SubmittedToVendor:
            // 如果供应商已将包裹打包，将其转发给快递员
            if (m_vendor.checkStatus()) {
                m_nState++;
                m_courier.submitRequest();
                std::cout << "********** 提交给快递员，跟踪次数：" << m_nCount << " **********" << std::endl;
            }
            break;
        case SubmittedToCourier:
            // 如果包裹交付，订单完成
            if (m_courier.checkStatus())
                return true;
        default:
            break;
        }

        m_nCount++;

        // 订单未完成
        return false;
    }

private:
    enum States {
        Received,  // 收到
        SubmittedToOrderTeam,  // 提交给订单团队
        SubmittedToVendor,  // 提交给供应商
        SubmittedToCourier  // 提交给快递员
    };

    int m_nState;  // 订单状态
    int m_nCount;  // 跟踪次数

    OrderTeam m_order;
    Vendor m_vendor;
    Courier m_courier;
};

#endif // FACADE_H
```
#### 4.5.2 创建客户端

```c
// main.cpp
#include "facade.h"

int main()
{
    OnlineShoppingFacade facade;

    // 提交订单
    facade.submitRequest();

    // 跟踪订单，直到订单完成
    while (!facade.checkStatus());

    std::cout << "********** 订单完成，跟踪次数：" << facade.followupNum() << " **********" << std::endl;

    getchar();

    return 0;
}
```

输出：

```bash
********** 提交给订单团队，跟踪次数：0 ********** 
订单团队 - 当前状态：收到 
订单团队 - 当前状态：确认付款 
订单团队 - 当前状态：联系供应商 
********** 提交给供应商，跟踪次数：3 ********** 
供应商 - 当前状态：收到 
供应商 - 当前状态：确认库存 
供应商 - 当前状态：从仓库得到物品 
供应商 - 当前状态：包装 
供应商 - 当前状态：联系快递员 
********** 提交给快递员，跟踪次数：8 ********** 
快递员 - 当前状态：收到 
快递员 - 当前状态：验证可达性 
快递员 - 当前状态：分配人员 
快递员 - 当前状态：派送包裹 
快递员 - 当前状态：获取交货确认 
********** 订单完成，跟踪次数：13 **********
```

### 4.8 优缺点

优点：
-  Client 屏蔽子系统组件，减少了 Client 处理的对象数目，并使得子系统使用起来更加容易。通过引入外观模式，Client 的代码将变得很简单，与之关联的对象也很少。
-  实现了子系统与 Client 之间的松耦合关系，这使得子系统的组件变化不会影响到调用它的 Client，只需要调整 Facade 即可。
-  降低了大型软件系统中的编译依赖性，并简化了系统在不同平台之间的移植过程，因为编译一个子系统一般不需要编译所有其他的子系统。一个子系统的修改对其他子系统没有任何影响，而且子系统内部变化也不会影响到外观对象。
-  只是提供了一个访问子系统的统一入口，并不影响用户直接使用子系统类。
缺点：

- 不能很好地限制 Client 使用子系统类，如果对 Client 访问子系统类做太多的限制，则会减少可变性和灵活性。
- 在不引入抽象外观类的情况下，增加新的子系统可能需要修改 Facade 或 Client 的源代码，违背了“开闭原则”。

### 4.9 适用场景

- 当要为一个复杂子系统提供一个简单接口时。该接口可以满足大多数用户的需求，而且用户也可以越过外观类直接访问子系统。
- Client 与多个子系统之间存在很大的依赖性。引入外观类将子系统与 Client 以及其他子系统解耦，可以提高子系统的独立性和可移植性。
- 在层次化结构中，可以使用外观模式定义系统中每一层的入口。层与层之间不直接产生联系，而通过外观类建立联系，降低层之间的耦合度。

### 4.12 模式扩展

不要试图通过外观类为子系统增加新行为；外观应该是对复杂模式的重新组装，而不是添加新功能。添加新功能应该使用装饰模式。

## 5 享元模式

_参考链接：_ [享元模式之C++实现](https://blog.csdn.net/JXH_123/article/details/38039703)

### 5.1 模式动机

- 通过共享计数实现相同或者相似对象的重用。
- 可以共享的内容称为内部状态,不能共享的称为外部状态。
- 享元模式中通常会出现工厂模式，需要创建享元工厂来负责维护一个享元池(FlyweightPool)用于存储具有相同内部状态的享元对象。
- 因为内部状态能共享的有限性，享元对象一般都设计为较小的对象，它所包含的内部状态比较少，这种对象也称为细粒度对象。享元模式通过，使用共享计数，实现大量细粒度对象的复用(加锁的问题？？？)；

### 5.2 模式定义

享元模式(Flyweight Pattern)：运用共享技术有效地支持大量细粒度对象的复用。系统只使用少量的对象，而这些对象都很相似，状态变化很小，可以实现对象的多次复用。由于享元模式要求能够共享的对象必须是细粒度对象，因此它又称为轻量级模式，它是一种对象结构型模式。

### 5.3 结构模式

- Flyweight: 抽象享元类;通常是一个抽象类，在抽象享元类中声明了具体享元类公共的方法，这些方法可以向外界提供享元对象的内部数据（内部状态），同时也可以通过这些方法来设置外部数据（外部状态）。
- ConcreteFlyweight: 具体享元类;实现了抽象享元类，其实例称为享元对象；在具体享元类中为内部状态提供了存储空间。通常可以结合单例模式来设计具体享元类，为每一个具体享元类提供唯一的享元对象。
- UnsharedConcreteFlyweight: 非共享具体享元类;并不是所有抽象享元类的子类都需要被共享，不能被共享的子类可设计为非共享具体享元类，当需要一个非共享具体享元类的对象时可以直接通过实例化创建。
- FlyweightFactory: 享元工厂类;用于创建并管理享元对象，它针对抽象享元类编程，将各种类型的具体享元对象存储在一个享元池中，享元池一般设计为一个存储“键值对”的集合（也可以是其他类型的集合），可以结合工厂模式进行设计；当用户请求一个具体享元对象时，享元工厂提供一个存储在享元池中已创建的实例或者创建一个新的实例（如果不存在的话），返回新创建的实例并将其存储在享元池中。

![享元结构模式](https://design-patterns.readthedocs.io/zh_CN/latest/_images/Flyweight.jpg)

**时序图**

![享元模式时序图](https://design-patterns.readthedocs.io/zh_CN/latest/_images/seq_Flyweight.jpg)

### 5.5 实例和代码分析

> 《反恐精英》 - Counter Strike
> 
> CS 将玩家分为“恐怖份子”（Terrorists）与“反恐精英”（Counter Terrorists）两个阵营，每个队伍必须在地图上进行多回合的战斗。在“爆破模式”中，T 阵营的任务是在指定时间内装置 C4 并引爆，而 CT 阵营的任务是拆除被装置的 C4。当玩家请求武器时，系统会为其分配所需的武器。
> 
> 现在，有 n 个玩 CS 的玩家，如果创建 n 个对象（每个玩家一个），这势必会占用大量内存。为了解决这个问题，可以使用享元模式（减少玩家数量），只需要为恐怖分子和反恐精英创建两个对象，在需要时反复使用即可。
> 


#### 5.5.1 内部状态和外部状态

- 内部状态（Intrinsic State）：“任务”就是两种玩家的内部状态，不会随外部环境的变而变化。两个队伍的“任务”基本是永远不变的。
- 外部状态（Intrinsic State）：“武器”等随环境改变而改变的，享元对象的外部状态通常由客户端保存，需要时再传入享元

由于区分了内部状态和外部状态，因此可以将具有相同内部状态的对象存储在享元池中来实现共享。当需要时，将对象从享元池中取出以实现对象的复用。通过向取出的对象注入不同的外部状态，可以得到一系列相似的对象，而这些对象在内存中实际上只存储一份。

#### 5.5.2 代码实现

**创建抽象享元**

```c
// flyweight.h
#ifndef FLYWEIGHT_H
#define FLYWEIGHT_H

#include <iostream>
#include <string>

// 玩家 - 有武器和使命
class IPlayer
{
public:
    virtual ~IPlayer() {}

    // 分配武器
    virtual void assignWeapon(std::string weapon) = 0;

    // 使命
    virtual void mission() = 0;

protected:
    std::string m_task; // 内部状态
    std::string m_weapon; // 外部状态
};

#endif // FLYWEIGHT_H
```

**创建具体享元**
具体享元类有两个-Terrorist、CounterTerrorist

```c
// concrete_flyweight.h
#ifndef CONCRETE_FLYWEIGHT_H
#define CONCRETE_FLYWEIGHT_H

#include "flyweight.h"

// 恐怖分子
class Terrorist : public IPlayer
{
public:
    Terrorist() {
        m_task = "Plant a bomb";
    }

    virtual void assignWeapon(std::string weapon) override {
        m_weapon = weapon;
    }

    virtual void mission() override {
        std::cout << "Terrorist with weapon " + m_weapon + "," + " Task is " + m_task << std::endl;
    }
};

// 反恐精英
class CounterTerrorist : public IPlayer
{
public:
    CounterTerrorist() {
        m_task = "Diffuse bomb";
    }

    virtual void assignWeapon(std::string weapon) override {
        m_weapon = weapon;
    }

    virtual void mission() override {
        std::cout << "Counter Terrorist with weapon " + m_weapon + "," + " Task is " + m_task << std::endl;
    }
};

#endif // CONCRETE_FLYWEIGHT_H
```
**创建享元工厂**
享元工厂的作用在于提供一个用于存储享元对象的享元池，当需要对象时，首先从享元池中获取，如果不存在，则创建一个新的享元对象，将其保存至享元池中并返回：

```c
// flyweight_factory.h
#ifndef FLYWEIGHT_FACTORY_H
#define FLYWEIGHT_FACTORY_H

#include "concrete_flyweight.h"
#include <map>

// 用于获取玩家
class PlayerFactory
{
public:
    // 如果 T/CT 对象存在，则直接从享元池获取；否则，创建一个新对象并添加到享元池中，然后返回。
    static IPlayer* getPlayer(std::string type)
    {
        IPlayer *p = NULL;
        if (m_map.find(type) != m_map.end()) {
            p = m_map[type];
        }
        else {
            // 创建 T/CT 对象
            if (type == "T") {
                std::cout << "Terrorist Created" << std::endl;
                p = new Terrorist();
            }
            else if (type == "CT") {
                std::cout << "Counter Terrorist Created" << std::endl;
                p = new CounterTerrorist();
            }
            else {
                std::cout << "Unreachable code!" << std::endl;
            }
            // 一旦创建，将其插入到 map 中
            m_map.insert(std::make_pair(type, p));
        }
        return p;
    }

private:
    // 存储 T/CT 对象（享元池）
    static std::map<std::string, IPlayer*> m_map;
};

#endif // FLYWEIGHT_FACTORY_H
```
**创建客户端**

```c
// main.cpp
#include "concrete_flyweight.h"
#include "flyweight_factory.h"
#include <ctime>

std::map<std::string, IPlayer*> PlayerFactory::m_map = std::map<std::string, IPlayer*>();

// 玩家类型和武器
static std::string s_playerType[2] = { "T", "CT" };
static std::string s_weapons[4] = { "AK-47", "Maverick", "Gut Knife", "Desert Eagle" };

#define GET_ARRAY_LEN(array, len) {len = (sizeof(array) / sizeof(array[0]));}

int main()
{
    srand((unsigned)time(NULL));

    int playerLen;
    int weaponsLen;
    GET_ARRAY_LEN(s_playerType, playerLen);
    GET_ARRAY_LEN(s_weapons, weaponsLen);

    // 假设，游戏中有十位玩家
    for (int i = 0; i < 10; i++) {
        // 获取随机玩家和武器
        int typeIndex = rand() % playerLen;
        int weaponIndex = rand() % weaponsLen;
        std::string type = s_playerType[typeIndex];
        std::string weapon = s_weapons[weaponIndex];

        // 获取玩家
        IPlayer *p = PlayerFactory::getPlayer(type);

        // 从武器库中随机分配武器
        p->assignWeapon(weapon);

        // 派玩家去执行任务
        p->mission();
    }

    getchar();

    return 0;
}
```
输出如下：
```sh
Counter Terrorist Created 
Counter Terrorist with weapon AK-47, Task is Diffuse bomb 
Counter Terrorist with weapon Gut Knife, Task is Diffuse bomb 
Counter Terrorist with weapon Maverick, Task is Diffuse bomb 
Counter Terrorist with weapon Maverick, Task is Diffuse bomb 
Terrorist Created 
Terrorist with weapon Desert Eagle, Task is Plant a bomb 
Counter Terrorist with weapon Maverick, Task is Diffuse bomb 
Counter Terrorist with weapon AK-47, Task is Diffuse bomb 
Counter Terrorist with weapon Maverick, Task is Diffuse bomb 
Terrorist with weapon Gut Knife, Task is Plant a bomb 
Counter Terrorist with weapon AK-47, Task is Diffuse bomb
```

### 5.6 模式分析

享元模式是一个考虑系统性能的设计模式，通过使用享元模式可以节约内存空间，提高系统的性能。

其核心在于享元工厂类，提供一个用于存储元对象的享元池，用户需要对象时，首先从享元池中获取，如果不存在再创建并，保存在享元池中。

### 5.8 优缺点 

优点：

- 可以极大减少内存中对象的数量，使得相同或相似对象在内存中只保存一份，从而可以节约系统资源，提高系统性能。
- 享元模式的外部状态相对独立，而且不会影响其内部状态，从而使得享元对象可以在不同的环境中被共享。

缺点：

- 享元模式使得系统变得复杂，需要分离出内部状态和外部状态，这使得程序的逻辑复杂化。
- 为了使对象可以共享，享元模式需要将享元对象的部分状态外部化，而读取外部状态将使得运行时间变长。

### 5.10 适用环境

- 一个系统有大量相同或者相似的对象，由于这类对象的大量使用，造成内存的大量耗费。
- 对象的大部分状态都可以外部化，可以将这些外部状态传入对象中。
- 使用享元模式需要维护一个存储享元对象的享元池，而这需要耗费资源，因此，应当在多次重复使用享元对象时才值得使用享元模式。

### 5.12. 模式扩展

- 单纯享元模式：所有的享元对象都是可以共享的，所有的抽象享元的子类都可以共享，不存在非共享具体享元类
- 符合享元模式：将一些单纯享元使用组合模式加以组合，可以形成复合享元对象，这样的复合享元对象本身不能共享，但是它们可以分解成单纯享元对象，而后者则可以共享。

享元模式与其他模式的联用

- 在享元模式的享元工厂类中通常提供一个静态的工厂方法用于返回享元对象，使用简单工厂模式来生成享元对象
- 在一个系统中，通常只有唯一一个享元工厂，因此享元工厂类可以使用单例模式进行设计。
- 享元模式可以结合组合模式形成复合享元模式，统一对享元对象设置外部状态。

## 6. 代理模式

_参考链接：_ [C++设计模式9--代理模式--万能的中间层](https://blog.csdn.net/gatieme/article/details/18035751)

### 6.1 模式动机

使用组合代理，将客户端不能使用的类或者接口，进行一次封装。具体由代理类来实现。代理类起到一个中间桥梁的作用。

### 6.2 模式定义

代理模式(Proxy Pattern) ：给某一个对象提供一个代 理，并由代理对象控制对原对象的引用。代理模式的英 文叫做Proxy或Surrogate，它是一种对象结构型模式。

### 6.3 模式结构

代理模式包含角色如下：

- Subject:抽象主题角色；声明了 RealSubject 与 Proxy 的共同接口，定义了某个/些功能。
- RealSubject（真实主题）：通常执行具体的业务逻辑，Proxy 控制对它的访问。
- Proxy:持有一个 RealSubject 引用（指针），可以在需要时将请求转发给 RealSubject，以此起到代理的作用。
- Client（客户端）：通过 Proxy 间接地与 RealSubject 进行交互。

**注意：** Proxy 和 RealSubject 都实现了 Subject 的接口，这允许 Client 可以像处理 RealSubject 一样处理 Proxy。

![结构模式类图](https://design-patterns.readthedocs.io/zh_CN/latest/_images/Proxy.jpg)

时序图

![结构模式时序图](https://design-patterns.readthedocs.io/zh_CN/latest/_images/seq_Proxy.jpg)


### 6.5 代码分析

#### 6.5.1 案例分析

> 中国移动 - 代理商
> 中国移动代理商：移动公司把充值的职责托付给代理点，代理点代替移动公司充值，客户直接与代理点打交道，而非移动公司。

#### 6.5.2 代码实现

##### 6.5.2.1 抽象主题

定义一个电信运行商 ITelco，并为其提供一个接口 Recharge()，用于充值：

```c
// subject.h
#ifndef SUBJECT_H
#define SUBJECT_H

// 电信运营商
class ITelco
{
public:
    virtual ~ITelco(){}
    virtual void Recharge(int money) = 0;  // 充值
};

#endif // SUBJECT_H
```
##### 6.5.2.2 创建真实主题

```c
// real_subject.h
#ifndef REAL_SUBJECT_H
#define REAL_SUBJECT_H

#include "subject.h"
#include <iostream>

// 中国移动
class CMCC : public ITelco
{
public:
    void Recharge(int money) override {
        std::cout << "Recharge " << money;
    }
};

#endif // REAL_SUBJECT_H
```
##### 6.5.2.3 创建代理
```c
// proxy.h
#ifndef PROXY_H
#define PROXY_H

#include "subject.h"
#include "real_subject.h"
#include <iostream>

// 代理点
class Proxy : public ITelco
{
public:
    Proxy() : m_pCMCC(NULL) {}
    ~Proxy() { delete m_pCMCC; }

    // 低于 50 不充
    void Recharge(int money) override {
        if (money >= 50) {
            if (m_pCMCC == NULL)
                m_pCMCC = new CMCC();
            m_pCMCC->Recharge(money);
        } else {
            std::cout << "Sorry, too little money" << std::endl;
        }
    }

private:
    CMCC *m_pCMCC;//代理移动成员变量
};

#endif // PROXY_H
```
**注意：** 注意： 代理类也是抽象主题类的子类，调用时可以在原有业务方法的基础上附加一些新的方法，来对功能进行扩充或约束。

##### 6.5.2.4 创建客户端

```c
// main.cpp
#include "proxy.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete(p); (p)=NULL;} }
#endif

int main()
{
    Proxy* proxy = new Proxy();
    proxy->Recharge(20);
    proxy->Recharge(100);

    SAFE_DELETE(proxy);

    getchar();

    return 0;
}
```

输出结果：

```shell
Sorry, too little money 
Recharge 100
```

### 6.8 优缺点

优点:
- 代理模式能将代理对象与真正被调用的对象分离，在一定程度上降低了系统的耦合度。
- 在客户端和目标对象之间，代理起到一个中介作用，这样可以保护目标对象。在对目标对象调用之前，代理对象也可以进行其他操作。

缺点：
- 这种模式引入了另一个抽象层，这有时可能是一个问题。如果真实主题被某些客户端直接访问，并且其中一些客户端可能访问代理类，这可能会导致不同的行为。
- 由于在客户端和真实主题之间增加了代理对象，因此有些类型的代理模式可能会造成请求的处理速度变慢。
- 实现代理模式需要额外的工作，有些代理模式的实现非常复杂。

### 6.9 适用场景

- 远程代理（Remote Proxy）：为一个位于不同地址空间的对象提供一个本地代理，对代理的方法调用会导致对远程对象的方法调用。ATM 就是一个例子，ATM 可能会持有（存在于远程服务器中的）银行信息的一个代理对象。
- 虚拟代理（Virtual Proxy）：使用虚拟代理，代理可以作为一个（资源消耗较大的）对象的代表。虚拟代理经常延迟对象的创建，直到需要为止。在创建对象之前（及创建对象过程中），虚拟代理也可以作为对象的代理；之后，代理将请求直接委托给 RealSubject。
- 保护代理（Protection Proxy）:根据访问权限，可以使用保护代理来控制对资源的访问。例如，有一个员工对象，保护代理可以允许普通员工调用对象的某些方法，管理员调用其他方法。
- 缓冲代理（Cache Proxy）:为某一个目标操作的结果提供临时的存储空间，以便多个客户端可以共享这些结果。
- 智能引用代理（Smart Reference Proxy）:当一个对象被引用时，提供一些额外的操作（例如：将对象被调用的次数记录下来）。

## 7 组合模式
_参考链接：_ [C++ 组合模式](https://blog.csdn.net/liang19890820/article/details/71240662)

### 7.1 模式定义

组合模式（Composite Pattern）:组合多个对象形成树形结构以表示具有“整体-部分”关系的层次结构。组合模式对单个对象（即：叶子构件）和组合对象（即：容器构件）的使用具有一致性，组合模式又被称为“整体-部分”（Part-Whole）模式，属于对象结构型模式。

### 7.2 模式结构

成员说明：

- Component（抽象构件）：为叶子构件和容器构件对象定义接口，可以包含所有子类共有行为的声明和实现。在抽象构件中，声明了访问及管理子构件的接口（例如：Add()、Remove()、GetChild() 等）。
- Leaf（叶子构件）：叶子节点没有子节点。它实现了 Component 中定义的行为，对于访问及管理子构件的接口，可以通过异常等方式进行处理。
- Composite（容器构件）：容器节点包含子节点（可以是叶子构件，也可以是容器构件）。它提供了一个集合用于存储子节点，实现了 Component 中定义的行为，包括访问及管理子构件的接口，在其业务方法中可以递归调用其子节点的业务方法。

UML结构图（透明组合模式）：

![UML结构图（透明组合模式）](https://img-blog.csdn.net/20170505195903712?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlhbmcxOTg5MDgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

UML结构图（安全组合模式）：

![UML结构图（安全组合模式）](https://img-blog.csdn.net/20170505195913620?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlhbmcxOTg5MDgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 7.3 透明组合模式和安全组合模式

根据 Component 的定义形式，可将组合模式分为两种形式：

- 透明组合模式:
  - 在 Component 中定义了用于访问和管理子构建的接口，这样做的好处是确保所有的构件类都有相同的接口。
  - 在 Client 看来，Leaf 与 Composite 所提供的接口一致，Client 可以相同地对待所有的对象。
- 安全组合模式
  - 在 Component 中不定义任何用于访问和管理子构建的接口，而在 Composite 中声明并实现
  - 这种做法是安全的，因为不需要向 Leaf 提供这些管理成员对象的接口，对于 Leaf 来说，Client 不可能调用到这些接口。

透明组合模式的缺点是不够安全，因为 Leaf 和 Composite 在本质上是有区别的。Leaf 不可能有下一个层级，因此为其提供 Add()、Remove()、GetChild() 等接口没有意义。这在编译阶段不会出错，但在运行阶段如果调用这些接口可能会出错（如果没有提供相应的异常处理）。

安全组合模式的缺点是不够透明，因为 Leaf 和 Composite 具有不同的接口，且 Composite 中那些用于访问和管理子构建的接口没有在 Component 中定义，因此 Client 不能完全针对抽象编程，必须有区别地对待 Leaf 和 Composite。

PS： 透明组合模式是组合模式的标准形式，但在实际应用中，安全组合模式的使用频率也非常高。

### 7.4 优缺点

优点：

- 组合模式可以清楚地定义分层次的复杂对象，表示对象的全部或部分层次，它让 Client 忽略了层次的差异，方便对整个层次结构进行控制。
- Client 可以一致地使用一个组合结构或其中单个对象，不必关心处理的是单个对象还是整个组合结构，简化了 Client 的代码。
- 在组合模式中，增加新的叶子构件和容器构件很方便，无须对现有类进行任何修改，符合“开闭原则”。
- 为树形结构提供了一种灵活的解决方案，通过递归组合容器对象和叶子对象，可以形成复杂的树形结构，但对树形结构的控制却非常简单。

缺点：

- 使设计变得更加抽象，对象的业务规则如果很复杂，则实现组合模式具有很大挑战性，而且不是所有的方法都与叶子对象子类都有关联。

### 7.5 适用场景

- 表示对象的“整体-部分”层次结构（树形结构）
- 希望用户忽略组合对象与单个对象的不同，统一地使用组合结构中的所有对象。

### 7.6 案例

适用组合模式，进行公司部门和人员的管理

#### 7.6.1 透明组合模式

**创建抽象构件**
Component 需要定义访问及管理子构件的接口：

```c
// component.h
#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>
#include <string>

using namespace std;

class Component
{
public:
    Component(string name) : m_strName(name) {}
    virtual ~Component() {}
    virtual void Add(Component *cmpt) = 0;  // 添加构件
    virtual void Remove(Component *cmpt) = 0;  // 删除构件
    virtual Component* GetChild(int index) = 0;  // 获取构件
    virtual void Operation(int indent) = 0;  // 显示构件（以缩进的方式）

private:
    Component();  // 不允许

protected:
    string m_strName;
};

#endif // COMPONENT_H
/* 安全组合模式 */

// component.h
#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>
#include <string>

using namespace std;

class Component
{
public:
    Component(string name) : m_strName(name) {}
    virtual ~Component() {}
    virtual void Operation(int indent) = 0;  // 显示构件（以缩进的方式）

private:
    Component();  //注意这里将其构造函数设置为了私有函数，不允许在栈中分配内存，只能输入参数进行工作

protected:
    string m_strName;
};

#endif // COMPONENT_H

```

**创建叶子构建**

作为Component的子类，Leaf需要实现Component中定义的所有接口，但是Leaf不能包含子构件。因此，**在Leaf中实现访问和管理子构件的函数时，需要进行异常处理或错误提示。**当然，这无疑会给Leaf的实现带来麻烦。

```c
// leaf.h
#ifndef LEAF_H
#define LEAF_H

#include "component.h"

class Leaf : public Component
{
public:
    Leaf(string name) : Component(name){}
    virtual ~Leaf(){}
    void Add(Component *cmpt) {
        cout << "Can't add to a Leaf" << endl;
    }
    void Remove(Component *cmpt) {
        cout << "Can't remove from a Leaf" << endl;
    }
    Component* GetChild(int index) {
        cout << "Can't get child from a Leaf" << endl;
        return NULL;
    }
    void Operation(int indent) {
        string newStr(indent, '-');
        cout << newStr << " " << m_strName <<endl;
    }

private:
    Leaf();  // 不允许
};

#endif // LEAF_H
/* 安全模式接口 */

// leaf.h
#ifndef LEAF_H
#define LEAF_H

#include "component.h"

class Leaf : public Component
{
public:
    Leaf(string name) : Component(name){}
    virtual ~Leaf(){}
    void Operation(int indent) {
        string newStr(indent, '-');
        cout << newStr << " " << m_strName <<endl;
    }

private:
    Leaf();  // 不允许
};

#endif // LEAF_H
```
**注意**： 与透明模式不同，这里已经没有了访问及管理子构件的接口，所有的接口都在 Composite 中，不再赘述（同上）。

**创建容器构建**

由于容器构建中可以包含叶子节点，因此对容器构建进行处理时可以使用递归的方式。

```c
// composite.h
#ifndef COMPOSITE_H
#define COMPOSITE_H

#include <vector>
#include "component.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete(p); (p)=NULL;} }
#endif

class Composite : public Component
{
public:
    Composite (string name) : Component(name) {}
    virtual ~Composite() {
        while (!m_elements.empty()) {
            vector<Component*>::iterator it = m_elements.begin();
            SAFE_DELETE(*it);
            m_elements.erase(it);
        }
    }
    void Add(Component *cmpt) {
        m_elements.push_back(cmpt);
    }
    void Remove(Component *cmpt) {
        vector<Component*>::iterator it = m_elements.begin();
        while (it != m_elements.end())  {
            if (*it == cmpt) {
                SAFE_DELETE(cmpt);
                m_elements.erase(it);
                break;
            }
            ++it;
        }
    }
    Component* GetChild(int index) {
        if (index >= m_elements.size())
            return NULL;

        return m_elements[index];
    }
    // 递归显示
    void Operation(int indent) {
        string newStr(indent, '-');
        cout << newStr << "+ " << m_strName << endl;
        // 显示每个节点的孩子
        vector<Component*>::iterator it = m_elements.begin();
        while (it != m_elements.end()) {
            (*it)->Operation(indent + 2);
            ++it;
        }
    }

private:
    Composite();  // 不允许

private:
    vector<Component *> m_elements;
};

#endif // COMPOSITE_H
```
**创建客户端**
主程序的客户端创建如下：

```c
// main.cpp
#include "composite.h"
#include "leaf.h"

int main()
{
    // 创建一个树形结构
    // 创建根节点
    Component *pRoot = new Composite("江湖公司（任我行）");

    // 创建分支
    Component *pDepart1 = new Composite("日月神教（东方不败）");
    pDepart1->Add(new Leaf("光明左使（向问天）"));
    pDepart1->Add(new Leaf("光明右使（曲洋）"));
    pRoot->Add(pDepart1);

    Component *pDepart2 = new Composite("五岳剑派（左冷蝉）");
    pDepart2->Add(new Leaf("嵩山（左冷蝉）"));
    pDepart2->Add(new Leaf("衡山（莫大）"));
    pDepart2->Add(new Leaf("华山（岳不群）"));
    pDepart2->Add(new Leaf("泰山（天门道长）"));
    pDepart2->Add(new Leaf("恒山（定闲师太）"));
    pRoot->Add(pDepart2);

    // 添加和删除叶子
    pRoot->Add(new Leaf("少林（方证大师）"));
    pRoot->Add(new Leaf("武当（冲虚道长）"));
    Component *pLeaf = new Leaf("青城（余沧海）");
    pRoot->Add(pLeaf);

    // 小丑，直接裁掉
    pRoot->Remove(pLeaf);

    // 递归地显示组织架构
    pRoot->Operation(1);

    // 删除分配的内存
    SAFE_DELETE(pRoot);

    getchar();

    return 0;
}
```

输出如下：
```sh
-+ 江湖公司（任我行） 
—+ 日月神教（东方不败） 
—– 光明左使（向问天） 
—– 光明右使（曲洋） 
—+ 五岳剑派（左冷蝉） 
—– 嵩山（左冷蝉） 
—– 衡山（莫大） 
—– 华山（岳不群） 
—– 泰山（天门道长） 
—– 恒山（定闲师太） 
— 少林（方证大师） 
— 武当（冲虚道长）
```