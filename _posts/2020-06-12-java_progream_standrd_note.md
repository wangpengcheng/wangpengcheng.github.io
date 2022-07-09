---
layout:     post
title:      Java开发规范学习笔记
subtitle:   Java开发规范华山版学习笔记
date:       2020-06-12
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - Java
    - 编译管理
    - 读书笔记
---


# Java开发规范学习笔记

_参考链接:_
- [阿里云官网课程链接](https://edu.aliyun.com/course/417?spm=5176.10731460.0.0.280639f4xpKm4n)
- [Git 官方手册](https://github.com/chjw8016/alibaba-java-style-guide)
- [在线书籍下载地址](https://edu.aliyun.com/course/417/material/24016/download?spm=5176.10731542.0.0.4a8c52bb9jM9Ji)
- [算法刷题笔记](https://labuladong.gitbook.io/algo/di-ling-zhang-bi-du-xi-lie)

## 1 编程规范

### 1.1 命名风格
1. 【强制】代码中的命名不能以下划线或者`$`符号开头或者结束
2. 代码中禁止使用拼音和英文混合方式
3. 类名强制使用驼峰风格；但是**DO/BO/DTO/VO/AO/PO/UID**等；([阿里巴巴Java开发手册中的DO、DTO、BO、AO、VO、POJO定义](https://www.cnblogs.com/EasonJim/p/7967999.html))
    - DO(Data Object):与数据库表结构一一对应，通过DAO层向上传输数据源对象。
    - DTO（ Data Transfer Object）：数据传输对象，Service或Manager向外传输的对象。
    - BO（ Business Object）：业务对象。 由Service层输出的封装业务逻辑的对象。
    - AO（ Application Object）：应用对象。 在Web层与Service层之间抽象的复用对象模型，极为贴近展示层，复用度不高。
    - VO（ View Object）：显示层对象，通常是Web向模板渲染引擎层传输的对象。
    - POJO（ Plain Ordinary Java Object）：在本手册中， POJO专指只有setter/getter/toString的简单类，包括DO/DTO/BO/VO等。
    - Query：数据查询对象，各层接收上层的查询请求。 注意超过2个参数的查询封装，禁止使用Map类来传输。
4. 方法名、参数名、成员变量、局部变量统一使用驼峰式命名方法
5. `static final`的常量命名全部大写，单词间用下划线隔开；力求语义表达完整清楚，不要嫌名字长；例如`MAX_COUNT`
6. 抽象类名使用Abstract或者Base开头；异常类命名使用Exception 结尾；测试以Test结尾
7. 数组中括号需要前置，`[]`与类型紧邻以定义类型。
8. POJO类中布尔类型变量都不要加is前缀。否则设置set/get函数的时候容易引起冲突。
9. 包名统一小写，自然语义之间使用`.`号进行隔开；类名统一首字母大写，可以使用复数形式；但是包名不能使用复数形式。
10. 避免在子类父类的成员变量之间或者不同的代码块的局部变量之间采用相同的命名，使可读性降低。
11. 在常量和变量的命名时，表示类型的名词放在词尾，以提升辨识度。
12. 如果模块、接口、类、方法使用了设计模式，在命名时需要体现；例如`OrderFactory`
13. 接口类中的方法和属性不要添加任何修饰符号。尽量不要在接口里面定义变量；接口中定义的变量必定与类相关管，为`static final`,并且是整个应用的基础常量。这样是为了防止在接口的继承过程中产生的变量的更改和转变。
14. 对于Service和DAO类，基于SOA的理念，暴露出来的服务一定是接口，内部的实现类使用Impl的后缀与接口区别
15. 枚举类名带上Enum后缀，枚举成员名需要全大写单词间使用下划线隔开
16. 各层命名约规
    - Service/DAO层方法命名规约 
        1. 获取单个对象的方法使用get做前缀
        2. 获取多个对象的方法使用list做前缀，复数形式如结尾，如：listObjects;
        3. 获取统计值的方法使用count做前缀
        4. 插入的方法用save/insert做前缀
        5. 删除的方法使用remove/delete做前缀
        6. 修改的方法使用update做前缀
    - 领域模型命名规约
        1. 数据对象：xxxDo,xxx为数据表名；如UserDO
        2. 数据传输对象:xxxDTO,xxx为业务领域相关的名称。如: userDTO
        3. 展示对象:xxxVO,xxx一般为网页名称
        4. POJO 时DO/DTO/BO/VO的统称，禁止命名使用

### 1.2 常量定义:
1. **不允许任何魔法值（即未经预先定义的常量）直接出现在代码中。**
2. **在long或者Long赋值时，数值后使用大写的L，不能是小写的l，小写容易跟数 字1混淆，造成误解。**
3. 常量复用的层次结构:常量的复用层次有5层:
    - 跨应用共享常量:放置在二方库中，通常在constant目录下
    - 应用内共享常量:放置在一方库中，通常是子模块中的constant目录下
    - 子工程内部共享常量：在当前子工程的constant目录下
    - 包内共享常量:在当前包下单独的constant目录下
    - 类共享常量:直接在类内部private static final 定义。
    - 如果变量值仅在一个固定范围内变化用enum类型来定义。 

### 1.3 代码格式

1. **如果是大括号内为空，则简洁地写成{}即可，大括号中间无需换行和空格；如果是非 空代码块则**： 
    1. 左大括号前不换行。  
    2. 左大括号后换行。
    3. 右大括号前换行。  
    4. 右大括号后还有 else 等代码则不换行；表示终止的右大括号后必须换行。
2. 左右括号和字符之间不出现空格。左大括号前需要空格；*******左括号左侧，右括号右侧****必须留一个空格。
3. 任何二目、三目运算符的左右两边都需要添加一个空格。
4. 强制使用4个空格缩进，禁止使用tab字符
5. 强制注释的双斜线与注释内容之间有且仅有一个空格
6. 在强制进行类型转换时，右括号与强制转换值之间不需要任何空格隔开。
8. 单行字符数限制不超过120个，超出需要换行；换行时遵循如下原则：
    - 第二行相对第一行缩进4个空格，从第三行开始，不再继续缩进
    - 运算符与下文一起换行
    - 方法调用的点符号与下文一起换行
    - 方法调用中的多个参数需要换行时，在逗号后进行
    - 在括号前不要环行。
9. 方法参数在定义和传入时，多个参数逗号后面必须加空格；例如：`method(args1, args2, args3)`
10. IDE中的编码方式设置为UTF-8;IDE中的文件的换行符使用Unix格式("\n")
11. 单个方法的总行数不超过80行；行数过多需要进行解耦。
12. 不同逻辑、不同语义、不同业务的代码之间插入一个空行分隔开来以提升可读性。

### 1.4 OOP约规

1. **强制**;对于类的对象引用访问，直接使用类引用静态变量或者静态方法，增加编译器的解析
2. **强制**所有的复写方法必须使用@override关键字；可以使用idea的快捷方式进行代码的生成和检查
3. **强制**可变参数，一般使用在相同参数类型，相同含义时使用。避免使用Object；可变参数必须放置在参数列表的最后。
4. **强制**外部正在调用或者二方库依赖的接口，不允许修改方法签名，避免对接口调用方产生影响。接口过时必须加@Deprecated注解
5. 不能使用果实的类或者方法
6. Object的equals方法容易抛空指针异常，应该使用常量或者确定有值的对象来调用equals
    - 注意：这里的equals主要是两者的内容是否相等；而`==`更多的是比较两者的内存地址是否相同；Object类的equals方法只是简单的比较指针指向的内存地址是否相同。所以一般类的方法都会进行，地址判断和类型的双重判断进行比较。([Object 的 equals 方法容易抛出空指](https://blog.csdn.net/teavamc/article/details/90702512))
    - 应该使用"test".equals(object)而不是`object.equals("test")`。
7. **强制**所有包装类型对象值之间的比较，全部使用equals方法比较；
    - 对于Integer var=? 在-128~127范围内的赋值，会在IntegerCache.cache产生，会复用已有对象，这个区间内的Integer值可以直接使用==进行判断，但是这个区间之外的所有数据都会在堆上产生，并不会复用已有对象。但是对于使用new关键字的主动内存创建和分配使用==时比较的是其内存地址，虽然指向的是同一块地址，但是还是会返回false, **推荐使用equals方法进行判断**
8. **任何货币金额，均以最小货币单位且整型类型来进行存储**，比如人民币就是以分为最小单位。
9. **强制**浮点数之间的等值判断，基本数据类型不能使用==进行判断，包装数据类型不能使用equals进行判断。浮点数需要使用"尾数+阶码"的编码方式，进行判断。例如`if(Math.abs(1.0f-0.9f)<(1e-6f))`
    - 或者使用BigDecimal来定义值，再进行浮点数的运算；例如:`BigDecimal a=new BigDecimal("1.0"),b= new BigDecimal("0.9"); if(a.equals(b))`
10. **强制**定义数据对象DO类时，属性类型要与数据库字段类型相匹配。匹配相关的数据表内容如下:
    - [MyBatis与MySQL对应数据类型](https://blog.csdn.net/a515557595_xzb/article/details/81587510)
    - [mybatis常用jdbcType数据类型以及对应的JavaType](https://www.cnblogs.com/yucongblog/p/7388648.html)

a. 常用数据类型映射表: 

|MySQL|JDBCType|JavaType|备注|
|:---:|:---:|:---:|:---:|
|`char`|`CHAR`|`String`|定长字符|
|`varchar`|`VARCHAR`|`String`|变长字符串|
|`tinyint`|`TINYINT`|`byte`|1字节|
|`smallint`|`SMALLINT`|`short`|2字节|
|`int`|`INTEGER`|`int`|4字节|
|`float`|`FLOAT`|`float`|4字节|
|`bigint`|`BIGINT`|`long`|8字节|
|`double`|`DOUBLE`|`double`|8字节|
|`bit`|`BOOLEAN`|`boolean`|布尔类型|

b. 日期时间和大对象映射表:

|MySQL|JDBCType|JavaType|备注|
|:---:|:---:|:---:|:---:|
|`date`|`Date`|`util.Date`,`sql.Date`|`YYYY-MM-DD`|
|`time`|`TIME`|`util.Date`、`sql.Date`|`HH:MM:SS`|
|`timestamp`|`TIMESTAMP`|`util.Date`、`sql.Date`|`YYYY-MM-DD HH:MM:SS`|
|`text`|`VARCHAR`|`String`|文本类型【2^16-1字节】|
|`longtext`|`LONGVARCHAR`|`String`|长文本类型【2^32-1字节】|

11. **强制**为了防止精度损失，禁止使用构造方法BigDecimal(double)方式将double值转化为BigDecimal对象。推荐使用转String的方式进行构造。或者直接使用`BigDecimal.valueOf()`方法
12. 关于基本数据类型和包装类数据类型的使用标准如下:
    1. **强制** 所有的POJO类属性必须使用包装数据类型
    2. **强制** RPC方法的返回值和参数必须使用包装数据类型
    3. 所有的局部数据变量都使用基本数据类型
    - 原因:POJO类属性没有初始值时提醒使用者，在使用是必须自己显式地进行赋值，任何NPE问题，或者入库检查，都由使用者来保证。
    - 例如:数据库的拆线呢结果可能式null,因为自动拆箱，用基本数据类型接收有[NPE空值异常](https://www.zhihu.com/question/55650388/answer/145790742)风险。
13. **强制** 定义DO/DTO/VO等POJO类时，不要设定任何默认属性值。因为可能存在时间节点等问题的更新。导致值的差异化。
14. **强制** 序列化类新增属性时，请不要修改serialVersionUID字段，避免反序列失败；如果完全不兼容升级，避免反序列化混乱，那么请修改serialVersionUID值([理解serialVersionUID是什么？有什么用？如何生成？](https://www.cnblogs.com/xuxinstyle/p/11394358.html);[Intellij IDEA怎么自动生成serialVersionUID](https://jingyan.baidu.com/article/f79b7cb3d19cc49144023ea6.html))。
15. **强制** 构造方法中禁止加入任何业务逻辑，如果有初始化逻辑，请放在init方法中。
16. 禁止在POJO类中，同时存在对应属性xxx的isXxx()和getXxx()方法。
17. **强制** 禁止在POJO类中必须写toString方法。使用IDE工具中的source>generate toString时，如果继承了另外一个POJO类，注意在前面加一个super.toString();
18. 使用索引访问用String 的split方法得到的数组时，需要做最后一个分割符后有无内容的检查，否则会有抛出`IndexOutOfBoundException`的风险。
19. 类的多个构造方法，或者多个同名方法，这些方法应该按照顺序防止在一起；同时类内方法定义的顺序依次是:
    - 公有方法或者保护方法
    - 私有方法
    - getter/setter方法。
20. setter 方法中，参数名称与类变量名称一致，this.成员名 = 参数名。在getter和setter中，不要增加业务逻辑，增加排查问题的难度。
21. 循环体内部，字符串的连接方式，使用StringBuilder的append方法进行扩展。
22. final主要声明类，成员变量、方法、以及本地变量；下列情况使用final关键字:不被允许继承的类，不能修改的引用的域或者对象、不允许被覆写的放啊，如POJO的setter方法、不允许运行过程中重新赋值的局部变量
23. 使用Object的clone方法时，建议重载成深拷贝再进行。
24. 成员变量域方法访问控制从严:
    - 禁止外部创建，构造方法必须是private,并提供创建类
    - 工具类不允许有public和default方法
    - 注意protect、public、private的成员变量修饰的方法类。
    - static成员变量，一般都考虑是否为final

### 1.5 日期时间

1. **日期格式化时，传入pattern 中表示年份统一使用小写的y。**
    - 说明: 日期格式化时，yyyy表示当天所在的年，而大写的YYYY表示当天所在周属于的年份，，一周从周日开始，周六结束，只要本周跨年，返回的 YYYY 就是下一年。 
    - 正例:`new SimpleDateFormat("yyyy-MM-dd-HH:mm:ss")`
2. **在日期格式中分清楚M/m、H/h**
    - 说明:日期格式中的这两对字母表意如下:
        1. 表示月份是大写的M;
        2. 表示分钟的是小写的m;
        3. 24小时制写的是大写的H;
        4. 12小时制的是小写的h;
3. **获取当前的毫秒数要使用`System.currentTimeMills()`;而不是`new Data().getTIme()`**
    - 说明：如果想获取更加精确的纳秒级时间值，使用 System.nanoTime 的方式。在 JDK8 中，针对统计时间 等场景，推荐使用 Instant([Java8 Instant 时间戳](https://blog.csdn.net/chunzhilianxue/article/details/80974202);[Java8中的LocalDateTime、Instant、DateTimeFormatter](https://www.jianshu.com/p/9828d79a2528)) 类。 
4. **不允许在程序任何地方中使用：1）java.sql.Date 2）java.sql.Time 3） java.sql.Timestamp。**
    - 说明：第1 个不记录时间，getHours()抛出异常；第2个不记录日期，getYear()抛出异常；第3 个在构造 方法 super((time/1000)*1000)，fastTime 和nanos分开存储秒和纳秒信息。
    - 反例： java.util.Date.after(Date)进行时间比较时，当入参是 java.sql.Timestamp 时，会触发 JDK BUG(JDK9 已修复)，可能导致比较时的意外结果。
5. **不要在程序中写死一年为365天，避免在公历闰年时出现日期转换错误或程序逻辑 错误。**
    ```java
    //正例
    // 获取今年的天数 
    int daysOfThisYear = LocalDate.now().lengthOfYear();
    //获取指定某天的天数
    LocalDate.of(2011, 1, 1).lengthOfYear();

    //反例
    
    // 第二种情况：一年有效期的会员制，今年1月26日注册，硬编码365返回的却是1月25日 
    Calendar calendar = Calendar.getInstance(); calendar.set(2020, 1, 26);
    calendar.add(Calendar.DATE, 365);
    ```
6. 避免公历闰年2月问题。闰年的2月份有29天，一年后的那一天不可能是2月29 日。
7. 使用枚举值来指代月份。如果使用数字，注意Date，Calendar等日期相关类的月份 month 取值在0-11之间。
    - 说明：参考JDK 原生注释，Month value is 0-based. e.g., 0 for January.
    - 正例： Calendar.JANUARY，Calendar.FEBRUARY，Calendar.MARCH 等来指代相应月份来进行传参或 比较。

### 1.6 集合处理

1. **强制**关于hashCode和equal的处理，遵循如下规则:
    1. 只要覆写equals,就必须覆写hashCode;
    2. 因为Set存储的是不重复的对象，根据hashCode和equals进行判断，所以存储Set的对象必须覆写这两个方法。
    3. 如果自定义对象作为Map的键，那么必须覆写hashCode和equals
2. **判断所有集合内部的元素是否为空，使用isEmpty()方法，而不是size()==0的方式。**
    - 说明：前者的时间复杂度为 O(1)，而且可读性更好。
    - 对于不同的类型推荐使用ConllectionUtil、MapUtil、String.util等进行判断；一般是先判断是否为空指针，然后才是判断是否为空。
3. **在使用java.util.stream.Collectors类的 toMap()方法转为Map集合时，一定要使 用含有参数类型为BinaryOperator，参数名为mergeFunction 的方法，否则当出现相同key 值时会抛出IllegalStateException 异常。**
    - 说明：参数mergeFunction 的作用是当出现 key重复时，自定义对 value 的处理策略。 
    - 正例： 
    ```java
    List<Pair<String, Double>> pairArrayList = new ArrayList<>(3); 
    pairArrayList.add(new Pair<>("version", 6.19)); 
    pairArrayList.add(new Pair<>("version", 10.24)); 
    pairArrayList.add(new Pair<>("version", 13.14)); 
    Map<String, Double> map = pairArrayList.stream().collect( 
            // 生成的map集合中只有一个键值对：{version=13.14},当存在重复的key,保留v2去除v1;
            Collectors.toMap(Pair::getKey, Pair::getValue, (v1, v2) -> v2)
        );
    ```
    - 反例:
    ```java
    String[] departments = new String[] {"iERP", "iERP", "EIBU"}; 
    // 抛出IllegalStateException异常 
    Map<Integer, String> map = Arrays.stream(departments)
                                        .collect(Collectors.toMap(String::hashCode, str -> str)); 
    ```
    - 参考链接:[java8 快速实现List转map 、分组、过滤等操作](https://blog.csdn.net/lu930124/article/details/77595585);[关于Java Lambda表达式看这一篇就够了](https://objcoding.com/2019/03/04/lambda/)
    - 注意:**Lambda表达式实际是生成了一个类的内部方法，再进行调用，因此Lambda方法中的this指针是类指针，而且类内的全局变量可以共同被访问。**
4. **在使用java.util.stream.Collectors类的 toMap()方法转为Map集合时，一定要注 意当value为null时会抛NPE异常。**
    - 说明：在java.util.HashMap 的merge 方法里会进行如下的判断： 
    ```java
        if (value == null || remappingFunction == null) throw new NullPointerException(); 
    ```
    - 反例:
    ```java
        List<Pair<String, Double>> pairArrayList = new ArrayList<>(2); 
        pairArrayList.add(new Pair<>("version1", 4.22));
        //注意这里的null值
        pairArrayList.add(new Pair<>("version2", null)); 
        Map<String, Double> map = pairArrayList.stream().collect( 
            // 抛出NullPointerException异常
             Collectors.toMap(Pair::getKey, Pair::getValue, (v1, v2) -> v2)); 
    ```

5. 强制ArrayList的subList不能转换为ArrayList,**否则会抛出ClassCastException异常**；因为其返回的是ArrayList的内部类SubList,是ArrayList的一个视图，对于SubList子列表的所有操作最终会反映到原列表上。
6. **使用Map的方法keySet()/values()/entrySet()**返回集合对象时，不可以对其进行添加元素操作，否则会抛出`UnsupportedOperationException`异常。
7. **Collections**类返回的对象，如：`emptyList()/singletoList()`等都是`immutablelist`,不可对其进行添加或者删除元素的操作。空链表不支持修改等操作。
8. 再subList场景中，因为其返回的是是父集合的一部分视图，因此原来集合的修改，均会使其发生变化。建议开辟一个新的集合去接收对象。
9. **使用集合转数组的方法，必须使用集合的toArray(T[] array)，传入的是类型完全一致，长度为0的数组，这样避免内存的浪费和重新分配**
    - [深入理解List的toArray()方法和toArray(T[] a)方法](https://wangpengcheng.github.io/img/2020-06-21-01-29-alicodestanrd.jpg)
10. **在使用Collection接口任何实现类的addAll()方法时，都要对输入的集合参数进行NPE判断**--在ArrayList.addAll方法的第一行代码即Object[] a = c.toArray();其中c为输入集合参数，如果为null，则直接抛出异常。
11. **使用工具类Arrays.asList()把数组转换成集合时，不能使用其修改集合相关的方法，它的add/remove/clear方法会抛出UnsupportedOperationException异常。**
说明：asList的返回对象是一个Arrays内部类，并没有实现集合的修改方法。Arrays.asList体现的是适配器模式，只是转换接口，后台的数据仍是数组。
    ```java
    String[] str = new String[] { "you", "wu" };
    Listlist = Arrays.asList(str);
    ```
    - 第一种情况：list.add("yangguanbao");运行时异常。
    - 第二种情况：str[0]="gujin";那么list.get(0)也会随之修改。

12. 泛型通配符`<? extends T>`来接收返回的数据，此写法的泛型集合不能使用add方法，而<? super T>不能使用get方法，作为接口调用赋值时易出错。
    - 因为<?extendsT>主要存储类和其子类；你不能往Plate<? extends T>中插入任何类型的对象,因为你不能保证列表实际指向的类型是什么。只能保证读取的是T及其子类；并且初始化后不能再添加，因此不能使用add方法。而`<?super T>`刚好和其相反相当于C++中的虚基类，不过这里统一指示成为Object。
    - 存储多时使用<?superT>;访问多时使用<?extendsT>
    - [<? extends T> 及<? super T> 重温](https://www.cnblogs.com/zero2max/p/11398537.html)
13. 在无泛型限制定义的集合赋值给泛型集合时，在使用元素集合时，需要进行instanceof判断，避免抛出`ClassCastException`异常
。不能直接进行赋值。
    ```java
    List<String> generics = null;
    List notGenerics = new ArrayList(10);
    notGenerics.add(new Object());
    notGenerics.add(new Integer(1));
    generics = notGenerics;
    // 此处抛出 ClassCastException 异常

    String string = generics.get(0);
    ```

14. **不要在 foreach 循环里进行元素的 remove/add 操作。remove 元素请使用Iterator方式，如果并发操作，需要对 Iterator 对象加锁。**
    ```java
    //正例

    List<String> list= new ArrayList<>();list.add("1");
    list.add("2");
    Iterator<String> iterator = list.iterator();
    while(iterator.hasNext()){
        Stringitem=iterator.next();
        if(删除元素的条件){
            iterator.remove();
        }
    }
    //反例

    for (String item : list) {
        if ("1".equals(item)) {
            list.remove(item);
        }
    }
    // 主要还是和C++类似的迭代器失效的问题。
    ```

15. **在 JDK7 版本及以上，Comparator 实现类要满足如下三个条件，不然 Arrays.sort，Collections.sort 会抛 IllegalArgumentException 异常。**
    -  x，y 的比较结果和 y，x 的比较结果相反。
    -  x>y，y>z，则 x>z。
    - x=y，则 x，z 比较结果和 y，z 比较结果相同。
    ```java
    //反例：下例中没有处理相等的情况，交换两个对象判断结果并不互反，不符合第一个条件，在实际使用中可能会出现异常。
    new Comparator<Student>() {
        @Override
        public int compare(Student o1, Student o2) {
            return o1.getId() > o2.getId() ? 1 : -1;
        }
    };
    ```
16. 集合泛型定义时，在JDK7及以上，使用diamond语法或全省略。
    - 说明：菱形泛型，即 diamond，直接使用<>来指代前边已经指定的类型。 
    ```java
    //正例:
    // diamond方式，即<> 
    HashMap<String, String> userCache = new HashMap<>(16); 
    // 全省略方式 
    ArrayList<User> users = new ArrayList(10); 
    ```
17. 集合初始化时，指定集合初始值大小([java集合超详解](https://blog.csdn.net/feiyanaffection/article/details/81394745);[java集合框架（深入）](https://www.cnblogs.com/TestMa/p/10641367.html))。
18. 使用entrySet遍历Map类集合KV，而不是keySet方式进行遍历。
    - keySet其实是遍历了2次，一次是转为Iterator对象，另一次是从hashMap中取出key所对应的value。而entrySet只是遍历了一次就把key和value都放到了entry中，效率更高。如果是JDK8，使用Map.foreach方法。正例：values()返回的是V值集合，是一个list集合对象；keySet()返回的是K值集合，是一个Set集合对象；entrySet()返回的是K-V值组合集合。
    - [java中Map遍历的四种方式](https://www.cnblogs.com/damoblog/p/9124937.html)
19. 高度注意Map类集合K/V能不能存储null值的情况，如下表格:

|集合类|Key|Value|Super|说明|
|:--|:---:|:---:|:---:|:---:|
|`Hashtable`|不允许为null|不允许为null|Dictionary|线程安全|
|`ConcurrentHashMap`|不允许为null|不允许为null|`AbstractMap`|锁分段技术(JDK8:CAS)|
|`TreeMap`|不允许为null|允许为null|`AbstractMap`|线程不安全|
|`HashMap`|许为null|允许为null|`AbstractMap`|线程不安全|

- 注意:`ConcurrentHashMap`的默认初始表大小是16。并且是线程安全的类。


20. 合理利用好集合的有序性(sort)和稳定性(order)，避免集合的无序性(unsort)和不稳定性(unorder)带来的负面影响。
    - 说明：有序性是指遍历的结果是按某种比较规则依次排列的。稳定性指集合每次遍历的元素次序是一定的。如：ArrayList是order/unsort；HashMap是unorder/unsort；TreeSet是order/sort。
21. 利用Set元素的唯一性，可以快速对一个集合进行去重操作，避免使用List的contains方法进行遍历、对比、去重操作。

### 1.7 并发处理

1. **获取单例对象需要保障线程安全，其中的方法也要保证线程安全**。
    - [Java-单例设计模式（懒汉与饿汉）](https://www.cnblogs.com/llstudy/p/9345686.html)
2. **创建线程或线程池时请指定有意义的线程名称，方便出错时回溯**
    - 正例：自定义线程工厂，并且根据外部特征进行分组，比如，来自同一机房的调用，把机房编号赋值给`whatFeaturOfGroup`
    ```java
        public class UserThreadFactory implements ThreadFactory {     
            private final String namePrefix;     
            private final AtomicInteger nextId = new AtomicInteger(1); 
            // 定义线程组名称，在jstack 问题排查时，非常有帮助     
            UserThreadFactory(String whatFeaturOfGroup) {         
                namePrefix = "From UserThreadFactory's " + whatFeaturOfGroup + "-Worker-";     
            }
 
            @Override     
            public Thread newThread(Runnable task) {        
                    String name = namePrefix + nextId.getAndIncrement();
                    Thread thread = new Thread(null, task, name, 0, false);
                    System.out.println(thread.getName());
                    return thread;
                } 
        } 
    ```
3. 线程资源必须通过线程池提供，不允许在应用中自行显式创建线程。避免创建线程的不必要开销。
    - [Java基础篇-线程与线程池](https://zhuanlan.zhihu.com/p/106803927?utm_source=wechat_session)
    - [Java中线程池ThreadPoolExecutor原理探究](http://ifeve.com/java%E4%B8%AD%E7%BA%BF%E7%A8%8B%E6%B1%A0threadpoolexecutor%E5%8E%9F%E7%90%86%E6%8E%A2%E7%A9%B6/)
    - [Java线程池详解及常用方法](https://baijiahao.baidu.com/s?id=1662039191028815781&wfr=spider&for=pc)
    - [Java线程池 面试点](https://zhuanlan.zhihu.com/p/71938772)
4. **线程池不允许使用Executors去创建，而是通过ThreadPoolExecutor的方式，这样的处理方式让写的同学更加明确线程池的运行规则，规避资源耗尽的风险**
    - 主要弊端如下:
        - FixedThreadPool和SingleThreadPool:允许的请求队列长度为Integer.MAX_VALUE，可能会堆积大量的请求，从而导致OOM。
        - CachedThreadPool和ScheduledThreadPool:允许的创建线程数量为Integer.MAX_VALUE，可能会创建大量的线程，从而导致OOM。
5. **SimpleDateFormat是线程不安全的类，一般不要定义为static变量，如果定义为static，必须加锁，或者使用DateUtils工具类。**

    ```java
    //正例：注意线程安全，使用DateUtils。亦推荐如下处理：
    private static final ThreadLocal<DateFormat> df = new ThreadLocal<DateFormat>() {
        @Override
        protected DateFormat initialValue() {
            return new SimpleDateFormat("yyyy-MM-dd");
        }
    };
    ```
    - 说明：如果是JDK8的应用，可以使用Instant代替Date，LocalDateTime代替Calendar，DateTimeFormatter代替SimpleDateFormat，官方给出的解释：simplebeautifulstrongimmutablethread-safe。

6. 必须回收自定义的ThreadLocal变量。尤其是在线程池场景下，线程经常会被复用，如果不清理自定义的ThreadLocal变量，可能会影响后续业务逻辑和造成内存泄漏等问题；尽量在代理中使用`try-finally`快进行回收。

7. **高并发时，同步调用应该去考量锁的性能损耗。能用无锁数据结构，就不要用锁；能锁区块，就不要锁整个方法体；能用对象锁，就不要用类锁。**
    - 可能使加锁的代码块工作量尽可能的小，避免在锁代码块中调用RPC方法。
    - [Java锁详解](https://blog.csdn.net/zcl_love_wx/article/details/93977947)
    - [java中的各种锁详细介绍](https://www.cnblogs.com/jyroy/p/11365935.html)
    - [Java高并发之无锁与Atomic源码分析](https://www.cnblogs.com/xdecode/p/9022525.html)

8. **对多个资源、数据库表、对象同时加锁时，需要保持一致的加锁顺序，否则会造成死锁。**
    - 说明：线程一需要对表 A、B、C依次全部加锁后才可以进行更新操作，那么线程二的加锁顺序也必须是 A、 B、C，否则可能出现死锁。 

9. **在使用阻塞等待获取锁的方式中，必须在 try 代码块之外，并且在加锁方法与 try 代码块之间没有任何可能抛出异常的方法调用，避免加锁成功后，在 finally 中无法解锁。** 
    - 说明一：如果在 lock 方法与 try**代码块之间的方法调用抛出异常**，那么**无法解锁**，造成其它线程无法成功获取锁。 
    - 说明二：如果 lock 方法在 try 代码块之内，可能由于其它方法抛出异常，导致在 finally 代码块中，unlock 对未加锁的对象解锁，它会调用 AQS 的 tryRelease 方法（取决于具体实现类），抛出IllegalMonitorStateException 异常。
    - 说明三：在 Lock 对象的 lock 方法实现中可能抛出 unchecked 异常，产生的后果与说明二相同。
    ```java
    Lock  lock  =  new  Xxx Lock(); 
    //  ...
    lock.lock();
    try{
        doSomething();
        doOthers();
    } finally {
        lock.unlock();
    }

    // 反例:
    Lock lock = new XxxLock(); 
    try {     
        // 如果此处抛出异常，则直接执行finally代码块     
        doSomething();     
        // 无论加锁是否成功，finally代码块都会执行     
        lock.lock();     
        doOthers(); 
    } finally {     
        lock.unlock(); 
    }
    ```
10. **在使用尝试机制来获取锁的方式中，进入业务代码块之前，必须先判断当前线程是否持有锁。锁的释放规则与锁的阻塞等待方式相同。**
    - 说明：Lock 对象的 unlock 方法在执行时，它会调用 AQS 的 tryRelease 方法（取决于具体实现类），如果当前线程不持有锁，则抛出 IllegalMonitorStateException 异常。
    ```java
    Lock  lock  =  new  Xxx Lock(); 
    //  ...
    boolean  isLocked  =  lock.tryLock(); 
    if  (isLocked) {
        try{
            doSomething();
            doOthers();
        } finally {
            lock.unlock();
        }
    }
    ```
11. **并发修改同一记录时，避免更新丢失，需要加锁。要么在应用层加锁，要么在缓存加锁，要么在数据库层使用乐观锁，使用 version 作为更新依据。**
    - 说明：如果每次访问冲突概率小于 20%，推荐使用乐观锁，否则使用悲观锁。乐观锁的重试次数不得小于3 次。
12. **多线程并行处理定时任务时，Timer 运行多个 TimeTask 时，只要其中之一没有捕获抛出的异常，其它任务便会自动终止运行(多线程异常会抛出错误)，如果在处理定时任务时使用`ScheduledExecutorService`则没有这个问题**
    - 说明：乐观锁在获得锁的同时已经完成了更新操作，校验逻辑容易出现漏洞，另外，乐观锁对冲突的解决策 略有较复杂的要求，处理不当容易造成系统压力或数据异常，所以资金相关的金融敏感信息不建议使用乐观 锁更新。 
    - 正例：悲观锁遵循一锁二判三更新四释放的原则。
    - 参考链接: [java定时器之Timer使用与原理分析](https://blog.csdn.net/fuyuwei2015/article/details/83825851);[JAVA：定时器的三种方法（详细注解）](https://blog.csdn.net/qq_36537546/article/details/83044977?utm_medium=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase)

13. 资金相关的金融敏感信息，使用悲观锁策略。
14. 使用 `CountDownLatch` 进行异步转同步操作，每个线程退出前必须调用 `countDown` 方法，线程执行代码注意 catch 异常，确保 `countDown` 方法被执行到，避免主线程无法执行至 `await` 方法，直到超时才返回结果。
    - 注意，子线程抛出异常堆栈，不能在主线程try-catch到。 
    - [CountDownLatch详解](https://www.jianshu.com/p/128476015902)

15. 避免 Random 实例被多线程使用，虽然共享该实例是线程安全的，但会因竞争同一seed  导致的性能下降。
    - Random 实例包括 java.util.Random  的实例或者  Math.random()的方式。 
    - 在 JDK7 之后，可以直接使用 API ThreadLocalRandom，而在  JDK7 之前，需要编码保证每个线程持有一个实例。 
16. 在并发场景下，可以使用双重检查锁实现延迟初始化的优化问题隐患，推荐解决方案中较为简单的一种，将目标属性声明为`volatile`类型。
17. volatile解决多线程内存不可见问题，对于一写多读可以解决变量同步问题，但是如果时多写，同样无法解决线程安全问题；建议使用`AtomicInteger`等原子操作。如果是JDK8,推荐使用LongAdder。
    - 主要是volatile是JVM虚拟机中的内存栅栏。对于多个java线程之间可以实现内存可见。和C++中类似，都是直接在共享内存中读取，而不是自己的线程内部内存读取，这个样子可以解决不可见问题，但是并不能解决安全问题;因为竞争的本质还是存在。
    - [volatile关键字 C++与Java的区别](https://blog.csdn.net/LU_ZHAO/article/details/104802876)

18. HashMap 在容量不够进行 resize 时由于高并发可能出现死链，导致 CPU 飙升，在开发过程中可以使用其它数据结构或加锁来规避此风险。
    - HashMap在进行扩容的时候，会主动拷贝节点的链表。对于链表的拷贝是从头节点开始依次进行拷贝的。但是在多线程情况下，可能存在多个节点的先后问题。造成链表中出现环路。
    - 这个问题在JDK8中做出了改进；JDK 8 中采用的是位桶 + 链表/红黑树的方式，当某个位桶的链表的长度超过 8 的时候；这个链表就将转换成红黑树。避免了复杂链表。同时使用了head 和 tail 来保证链表的顺序和之前一样(JDK7会造成逆序反转)；但是还会有数据丢失等弊端（并发本身的问题）(主要是并发时，相同key的覆盖问题)。因此多线程情况下还是建议使用 ConcurrentHashMap。
19. ThreadLocal 对象使用 static 修饰，ThreadLocal 无法解决共享对象的更新问题。 
    - 说明：这个变量是针对一个线程内所有操作共享的，所以设置为静态变量，所有此类实例共享此静态变量，也就是说在类第一次被使用时装载，只分配一块存储空间，所有此类的对象(只要是这个线程内定义的)都可以操控这个变量。 
    - [将ThreadLocal变量设置为private static的好处是啥？](https://www.zhihu.com/question/35250439)
    - [彻底理解ThreadLocal](https://www.cnblogs.com/xzwblog/p/7227509.html)

### 1.8 控制语句

1. **在一个switch快内，每个case要么通过continue/break/return等来终止，要么注释说明程序将继续执行到哪一个case为止；在一个switch块内，都必须包含一个defualt语句并且放在最后，即使它什么代码也没有**
    - 注意 break 是退出 switch 语句块，而 return 是退出方法体。 
2. **当 switch 括号内的变量类型为 String 并且此变量为外部参数时，必须先进行 null判断。**
    ```java
        public  class  SwitchString  {      
            public  static  void  main(String[]  args){
                method(null);
            }       
            public  static  void  method(String  param)  {
                switch  (param)  {              
                //  肯定不是进入这里              
                case  "sth":                  
                    System.out.println("it's  sth");
                    break;              //  也不是进入这里
                case  "null":
                    System.out.println("it's  null");
                    break;              //  也不是进入这里
                default:
                    System.out.println("default");
                    }
            }
            //传入空指针时，会直接报空指针异常。
        } 
    ```
3. **在 if/else/for/while/do 语句中必须使用大括号。即使只有一行if**
    - 说明：即使只有一行代码，禁止不采用大括号的编码方式：if (condition) statements; 
4. **三目运算符condition? 表达式1 : 表达式2中，高度注意表达式1和2在类型对齐时，对于自动拆箱和装箱可能抛出因自动拆箱导致的NPE异常。**
    - 说明：以下两种场景会触发类型对齐的拆箱操作： 
        - 表达式1 或表达式2 的值只要有一个是原始类型。 
        - 表达式1 或表达式2 的值的类型不一致，会强制拆箱升级成表示范围更大的那个类型。 
    ```java
    Integer a = 1; 
    Integer b = 2; 
    Integer c = null; 
    Boolean flag = false; 
    // a*b的结果是int类型，那么c会强制拆箱成int类型，抛出 NPE异常 
    Integer result=(flag? a*b : c); 
    ```
5. **在高并发场景中，避免使用”等于”判断作为中断或退出的条件。** 
    - 如果并发控制没有处理好，容易产生等值判断被“击穿”的情况，使用大于或小于的区间判断条件来代替。 
    - 例：判断剩余奖品数量等于 0 时，终止发放奖品，但因为并发处理错误导致奖品数量瞬间变成了负数，这样的话，活动无法终止。 
6. **当某个方法的代码行数超过10行时，return / throw 等中断逻辑的右大括号后加一个空行。**
    - 说明:这样可以使得代码的逻辑更加清晰可靠。
7. 表达异常的分支时，少用 if-else 方式，**if else 请勿超过 3 层。**
    - 超过 3 层的  if-else  的逻辑判断代码可以使用卫语句、策略模式、状态模式等来实现，其中卫语句即代码逻辑先考虑失败、异常、中断、退出等直接返回的情况，以方法多个出口的方式，解决代码中判断分支嵌套的问题，这是逆向思维的体现。 
8. 除常用方法（如 getXxx/isXxx）等外，不要在条件判断中执行其它复杂的语句，将复杂逻辑判断的结果赋值给一个有意义的布尔变量名，以提高可读性。 
9. 不要在其它表达式（尤其是条件表达式）中，插入赋值语句--**容易造成死循环**
10. 循环体中的语句要考量性能，以下操作尽量移至循环体外处理，如定义对象、变量、获取数据库连接，进行不必要的 try-catch 操作（这个 try-catch 是否可以移至循环体外）。 
11. 避免采用取反逻辑运算符。
    - 例:使用 if (x < 628)  来表达  x  小于 628。而不是:使用 if (!(x >= 628))  来表达  x  小于 628。 
12. 接口入参保护，这种场景常见的是用作批量操作的接口。防止参数过大，造成性能爆炸。 
11. 下列情形，需要进行参数校验：
    - 下列情形，需要进行参数校验：
        - 调用频次低的方法。 
        - 执行时间开销很大的方法。此情形中，参数校验时间几乎可以忽略不计，但如果因为参数错误导致     中间执行回退，或者错误，那得不偿失。
        - 需要极高稳定性和可用性的方法。 
        - 对外提供的开放接口，不管是 RPC/API/HTTP 接口。 
        - 敏感权限入口。
12. 下列情形，不需要进行参数校验： 
    - 极有可能被循环调用的方法。但在方法说明里必须注明外部参数检查要求。 
    - 底层调用频度比较高的方法。毕竟是像纯净水过滤的最后一道，参数错误不太可能到底层才会暴露问题。**一般 DAO 层与 Service 层都在同一个应用中**，部署在同一台服务器中，**所以 DAO 的参数校验，可以省略**。 
    - 被声明成 private 只会被自己代码所调用的方法，如果能够确定调用方法的代码传入参数已经做过检查或者肯定不会有问题，此时可以不校验参数。 

### 1.9 注释规约 
1. **类、类属性、类方法的注释必须使用 Javadoc 规范，使用`/**内容*/`格式，不得使用`// xxx `方式。**
    - 在 IDE 编辑窗口中，Javadoc 方式会提示相关注释，生成 Javadoc 可以正确输出相应注释；在 IDE中，工程调用方法时，不进入方法即可悬浮提示方法、参数、返回值的意义，提高阅读效率。 
2. **所有的抽象方法（包括接口中的方法）必须要用 Javadoc 注释、除了返回值、参数、异常说明外，还必须指出该方法做什么事情，实现什么功能。**
    - 对子类的实现要求，或者调用注意事项，请一并说明。 
3. **所有的类都必须添加创建者和创建日期。**
4. **方法内部单行注释，在被注释语句上方另起一行，使用//注释。方法内部多行注释使用/* */注释，注意与代码对齐。**
5. **所有的枚举类型字段必须要有注释，说明每个数据项的用途。**
6. 与其“半吊子”英文来注释，不如用中文注释把问题说清楚。专有名词与关键字保持英文原文即可。 
7. 代码修改的同时，注释也要进行相应的修改，尤其是参数、返回值、异常、核心逻辑等的修改。
8. 谨慎注释掉代码。在上方详细说明，而不是简单地注释掉。如果无用，则删除。 
    - 后续会恢复此段代码逻辑。
    - 永久不用。前者如果没有备注信息，难以知晓注释动机。后者建议直接删掉（代码仓库已然保存了历史代码）。
9. 对于注释的要求：
    - 能够准确反映设计思想和代码逻辑
    - 能够描述业务含义，使别的程序员能够迅速了解到代码背后的信息
    - 完全没有注释的大段代码对于阅读者形同天书，注释是给自己看的，即使隔很长时间，也能清晰理解当时的思路；
    - 注释也是给继任者看的，使其能够快速接替自己的工作。 
10. 好的命名、代码结构是自解释的，注释力求精简准确、表达到位。避免出现注释的一个极端：过多过滥的注释，代码的逻辑一旦修改，修改注释是相当大的负担。 
11. 特殊注释标记，请注明标记人与标记时间。注意及时处理这些标记，通过标记扫描，经常清理此类标记。线上故障有时候就是来源于这些标记处的代码。 
    - 待办事宜（TODO）:（`标记人，标记时间，[预计处理时间]`） 
        - 表示需要实现，但目前还未实现的功能。这实际上是一个 Javadoc 的标签，目前的 Javadoc 还没   有实现，但已经被广泛使用。只能应用于类，接口和方法（因为它是一个 Javadoc 标签）。
    - 错误，不能工作（FIXME）:（`标记人，标记时间，[预计处理时间]`） 
        - 在注释中用 FIXME 标记某代码是错误的，而且不能工作，需要及时纠正的情况。 

### 1.10 其它

1. **在使用正则表达式时，利用好其预编译功能，可以有效加快正则匹配速度。**
    - 不要在方法体内定义：Pattern pattern = Pattern.compile(“规则”);
    - [是否进行预编译的正则表达式时间效率对比](https://blog.csdn.net/qq_20735197/article/details/103082691)
2. **避免用Apache Beanutils进行属性的copy**
    - 说明：Apache BeanUtils性能较差，可以使用其他方案比如 Spring BeanUtils, Cglib BeanCopier，注意 均是浅拷贝。 
3. **velocity 调用 POJO 类的属性时，直接使用属性名取值即可，模板引擎会自动按规范调用 POJO 的 getXxx()，如果是 boolean 基本数据类型变量（boolean 命名不需要加 is 前缀），会自动调用 is Xxx()方法。**
    - 注意如果是 Boolean 包装类对象，优先调用 getXxx()的方法。 
    - [Velocity教程](https://www.jianshu.com/p/5913903324ff)
4. **后台输送给页面的变量必须加$!{var}——中间的感叹号。**
    - 如果 var 等于 null 或者不存在，那么${var}会直接显示在页面上。 
5. **注意  Math.random()  这个方法返回是 double 类型，注意取值的范围  0≤x<1（能够取到零值，注意除零异常），如果想获取整数类型的随机数，不要将 x 放大 10 的若干倍然后取整，直接使用 Random 对象的 nextInt 或者 nextLong 方法。**
6. **获取当前毫秒数 System.currentTimeMillis();  而不是 new Date().getTime();**
    - 如果想获取更加精确的纳秒级时间值，使用 System.nanoTime()的方式。在 JDK8 中，针对统计时间等场景，推荐使用 Instant 类。
7. **日期格式化时，传入 pattern 中表示年份统一使用小写的 y**。 
    - 日期格式化时，yyyy 表示当天所在的年，而大写的 YYYY 代表是 week in which year（JDK7 之后引入的概念），意思是当天所在的周属于的年份，一周从周日开始，周六结束，只要本周跨年，返回的 YYYY 就是下一年。另外需要注意：
        - 表示月份是大写的 M 
        - 表示分钟则是小写的 m 
        - 24 小时制的是大写的 H 
        - 12 小时制的则是小写的 h 
        - 正确的日期格式:`new SimpleDateFormat("yyyy-MM-dd HH:mm:ss"); `
8. 不要在视图模板中加入任何复杂的逻辑。 
9. 任何数据结构的构造或初始化，都应指定大小，避免数据结构无限增长吃光内存。 
10. 及时清理不再使用的代码段或配置信息。
    - 对于垃圾代码或过时配置，坚决清理干净，避免程序过度臃肿，代码冗余。 
    - 对于暂时被注释掉，后续可能恢复使用的代码片断，在注释代码上方，统一规定使用三个斜杠(///)来说明注释掉代码的理由。

## 2 异常日志 
### 2.1 错误码 
1. **错误码的制定原则：快速溯源、简单易记、沟通标准化。**
    - 说明： 错误码想得过于完美和复杂，就像康熙字典中的生僻字一样，用词似乎精准，但是字典不容易随身 携带并且简单易懂。 
    - 正例：错误码回答的问题是谁的错？错在哪？
        1. 错误码必须能够快速知晓错误来源，可快速判断是谁的问 题。
        2. 错误码易于记忆和比对（代码中容易 equals）。
        3. 错误码能够脱离文档和系统平台达到线下轻量 化地自由沟通的目的。 
2. **错误码不体现版本号和错误等级信息。**
    - 说明：错误码以不断追加的方式进行兼容。错误等级由日志和错误码本身的释义来决定。
3. **全部正常，但不得不填充错误码时返回五个零：00000。**
4. **错误码为字符串类型，共5位，分成两个部分：错误产生来源+四位数字编号**
    - 说明：错误产生来源分为 A/B/C，A 表示错误来源于用户，比如参数错误，用户安装版本过低，用户支付 超时等问题；B表示错误来源于当前系统，往往是业务逻辑出错，或程序健壮性差等问题；C表示错误来源 于第三方服务，比如 CDN服务出错，消息投递超时等问题；四位数字编号从 0001到9999，大类之间的 步长间距预留 100，参考文末附表3。 
5. **编号不与公司业务架构，更不与组织架构挂钩，一切与平台先到先申请的原则进行， 审批生效，编号即被永久固定。**
6. **错误码使用者避免随意定义新的错误码。**
    - 说明：尽可能在原有错误码附表中找到语义相同或者相近的错误码在代码中使用即可。 
7. **错误码不能直接输出给用户作为提示信息使用。**
    - 说明：堆栈（stack_trace）、错误信息(error_message)、错误码（error_code）、提示信息（user_tip） 是一个有效关联并互相转义的和谐整体，但是请勿互相越俎代庖。 
8. 错误码之外的业务独特信息由error_message来承载，而不是让错误码本身涵盖过 多具体业务属性。
9. 在获取第三方服务错误码时，向上抛出允许本系统转义，由C转为B，并且在错误信 息上带上原有的第三方错误码。 
10. 错误码分为一级宏观错误码、二级宏观错误码、三级宏观错误码。 
    - 说明：在无法更加具体确定的错误场景中，可以直接使用一级宏观错误码，分别是：A0001（用户端错误）、B0001（系统执行出错）、C0001（调用第三方服务出错）。 
    - 正例：调用第三方服务出错是一级，中间件错误是二级，消息服务出错是三级。 
11. **错误码的后三位编号与HTTP状态码没有任何关系。**
12. **错误码尽量有利于不同文化背景的开发者进行交流与代码协作。**
    - 说明：英文单词形式的错误码不利于非英语母语国家（如阿拉伯语、希伯来语、俄罗斯语等）之间的开发 者互相协作。 
13. 错误码即人性，感性认知+口口相传，使用纯数字来进行错误码编排不利于感性记忆 和分类
    - 说明：数字是一个整体，每位数字的地位和含义是相同的。
    -  反例：一个五位数字 12345，第 1 位是错误等级，第2位是错误来源，345 是编号，人的大脑不会主动地 分辨每位数字的不同含义。 


### 2.2 异常规约

1. **Java类库中定义的可以通过预检查的方式规避RuntimeException的异常不应该通过catch的方式类处理，比如:`NullPointerException`、`IndexOutOfBoundsException`**
2. **异常不要用来做流程控制，条件控制。**
    - 异常设计的初衷是解决程序运行中的各种意外情况，且异常的处理效率比条件判断方式要低很多。 
3. **catch 时请分清稳定代码和非稳定代码，稳定代码指的是无论如何不会出错的代码。对于非稳定代码的 catch 尽可能进行区分异常类型，再做对应的异常处理。**
4. **捕获异常是为了处理它，不要捕获了却什么都不处理而抛弃之，如果不想处理它，请将该异常抛给它的调用者。最外层的业务使用者，必须处理异常，将其转化为用户可以理解的内容。**
5. **有 try 块放到了事务代码中，catch 异常后，如果需要回滚事务，一定要注意手动回滚事务。**
6. **finally 块必须对资源对象、流对象进行关闭，有异常也要做 try-catch。**
7. **不要在 finally 块中使用 return。**
    - try 块中的 return 语句执行成功后，并不马上返回，而是继续执行 finally 块中的语句，如果此处存在 return 语句，则在此直接返回，无情丢弃掉 try 块中的返回点。
8. **捕获异常与抛异常，必须是完全匹配，或者捕获异常是抛异常的父类。**
9. **在调用 RPC、二方包、或动态生成类的相关方法时，捕捉异常必须使用 Throwable类来进行拦截。**
    - 通过反射机制来调用方法，如果找不到方法，抛出 NoSuchMethodException。什么情况会抛出NoSuchMethodError 呢？二方包([一方包、二方包、三方包是什么？](https://www.cnblogs.com/aspirant/p/12141653.html))在类冲突时，仲裁机制可能导致引入非预期的版本使类的方法签名不匹配，或者在字节码修改框架（比如：ASM）动态创建或修改类时，修改了相应的方法签名。这些情况，即使代码编译期是正确的，但在代码运行期时，会抛出 NoSuchMethodError。
10. 方法的返回值可以为 null，不强制返回空集合，或者空对象等，必须添加注释充分说明什么情况下会返回 null 值。 
    - 本手册明确防止 NPE 是调用者的责任。即使被调用方法返回空集合或者空对象，对调用者来说，也并非高枕无忧，必须考虑到远程调用失败、序列化失败、运行时异常等场景返回 null 的情况。 
11. 防止 NPE，是程序员的基本修养，注意 NPE 产生的场景：
    - 返回类型为基本数据类型，return 包装数据类型的对象时，自动拆箱有可能产生 NPE。 
    - 数据库的查询结果可能为 null。
    - 集合里的元素即使 is NotEmpty，取出的数据元素也可能为 null。
    - 远程调用返回对象时，一律要求进行空指针判断，防止 NPE。
    - 对于 Session 中获取的数据，建议进行 NPE 检查，避免空指针。 
    - 级联调用 obj.getA().getB().getC()；一连串调用，易产生 NPE。 
        - 正例：使用 JDK8 的 Optional 类来防止 NPE 问题。
12. 定义时区分 unchecked / checked  异常，避免直接抛出 new RuntimeException()，更不允许抛出 Exception 或者 Throwable，应使用有业务含义的自定义异常。推荐业界已定义过的自定义异常，如：DAOException / ServiceException 等。 
    - [Java异常：选择Checked Exception还是Unchecked Exception?](https://blog.csdn.net/kingzone_2008/article/details/8535287)
13. 对于公司外的 http/api 开放接口必须使用“错误码”；而应用内部推荐异常抛出；跨应用间 RPC 调用优先考虑使用 Result 方式，封装 isSuccess()方法、“错误码”、“错误简短信息”。 
    - 使用抛异常返回方式，调用方如果没有捕获到就会产生运行时错误。
    - 如果不加栈信息，只是 new 自定义异常，加入自己的理解的 error message，对于调用端解决问题的帮助不会太多。如果加了栈信息，在频繁调用出错的情况下，数据序列化和传输的性能损耗也是问题。 
14. 避免出现重复的代码（Don't Repeat Yourself），即 DRY 原则([DRY原则和Shy原则](https://www.cnblogs.com/happyPawpaw/p/4125657.html))

### 2.3 日志规约

1. **应用中不可直接使用日志系统（Log4j、Logback）中的 API，而应依赖使用日志框架  SLF4J 中的 API，使用门面模式的日志框架，有利于维护和各个类的日志处理方式统一。**
    ```java
    //slf4j
    import  org.slf4j.Logger;  
    import  org.slf4j.LoggerFactory;   private  static  final  Logger  logger  =  LoggerFactory.getLogger(Test.class);  
    //jcl
    import org.apache.commons.logging.Log; 
    import org.apache.commons.logging.LogFactory; private static final Log log = LogFactory.getLog(Test.class); 

    ```
    - [slf4j的使用](https://www.jianshu.com/p/805a754053cf)
    - [slf4j官方文档](http://www.slf4j.org/)
2. **所有日志文件至少保存 15 天，因为有些异常具备以“周”为频次发生的特点。网络运行状态、安全相关信息、系统监测、管理后台操作、用户敏感操作需要留存相关的网络日志不少于 6 个月。**
3. **应用中的扩展日志（如打点、临时监控、访问日志等）命名方式：**
    - appName_log,Type_log,Name.log。logType:日志类型，如 stats/monitor/access 等；logName:日志描述。这种命名的好处：通过文件名就可知道日志文件属于什么应用，什么类型，什么目的，也有利于归类查找。
    - 推荐对日志进行分类，如将错误日志和业务日志分开存放，便于开发人员查看，也便于通过日志对系统进行及时监控。
    - 正例：mppserver 应用中单独监控时区转换异常，如：mppserver_monitor_timeZoneConvert.log  
4. 在日志输出时，字符串变量之间的拼接使用占位符的方式。
    - 因为 String 字符串的拼接会使用 StringBuilder 的 append()方式，有一定的性能损耗。使用占位符仅是替换动作，可以有效提升性能。
    - 例: `logger.debug("Processing trade with id: {} and symbol: {}", id, symbol);`
    - [String+，StringBuilder，String.format运行效率比较](https://blog.csdn.net/imi00/article/details/80951856)
    - [Java魔法堂：String.format详解](https://www.cnblogs.com/fsjohnhuang/p/4094777.html)
5. **对于 trace/debug/info 级别的日志输出，必须进行日志级别的开关判断。**
    - 虽然在 debug(参数)的方法体内第一行代码 isDisabled(Level.DEBUG_INT)为真时（Slf4j 的常见实现 Log4j 和 Logback），就直接 return，但是参数可能会进行字符串拼接运算。此外，如果debug(getName())这种参数内有 getName()方法调用，无谓浪费方法调用的开销。
    ```java
    // 如果判断为真，那么可以输出 trace 和 debug 级别的日志
    if (logger.isDebugEnabled()) {
        logger.debug("Current ID is: {} and name is: {}", id, getName());
    }
    ```
6. **避免重复打印日志，浪费磁盘空间，务必在 log4j.xml 中设置 additivity=false。**
    - 正：`<logger name="com.taobao.dubbo.config" additivity="false">`
7. **生产环境禁止直接使用System.out 或System.err 输出日志或使用 e.printStackTrace()打印异常堆栈。**
    - 说明：标准日志输出与标准错误输出文件每次 Jboss重启时才滚动，如果大量输出送往这两个文件，容易 造成文件大小超过操作系统大小限制。
8. **异常信息应该包括两类信息：案发现场信息和异常堆栈信息。如果不处理，那么通过关键字 throws 往上抛出。**
    - 正：`logger.error(各类参数或者对象 toString() + "_" + e.getMessage(), e);`
9. **日志打印时禁止直接用JSON工具将对象转换成String。**
    - 说明：如果对象里某些 get 方法被重写，存在抛出异常的情况，则可能会因为打印日志而影响正常业务流 程的执行。 
    - 正例：打印日志时仅打印出业务相关属性值或者调用其对象的 toString()方法。 

8. 谨慎地记录日志。生产环境禁止输出 debug 日志；有选择地输出 info 日志；如果使用 warn 来记录刚上线时的业务行为信息，一定要注意日志输出量的问题，避免把服务器磁盘撑爆，并记得及时删除这些观察日志。 
9. 可以使用 warn 日志级别来记录用户输入参数错误的情况，避免用户投诉时，无所适从。如非必要，请不要在此场景打出 error 级别，避免频繁报警。
10. 尽量用英文来描述日志错误信息，如果日志中的错误信息用英文描述不清楚的话使用中文描述即可，否则容易产生歧义。**国际化团队或海外部署的服务器由于字符集问题，使用全英文来注释和描述日志错误信息。**

## 3 单元测试
1. **好的单元测试必须遵守AIR(空气)原则**
    - A:Automatic(自动化)
    - I:Independent(独立性)
    - R:Repeatable(可重复)
2. **单元测试应该是完全自定执行的，并非互交式的。测试用例通常式被定期执行的，执行过程中必须完全自动化才有意义。输出结果需要让检查的测试不是一个好的单元测试，单元测试中不准使用System.out来进行人肉验证；必须使用assert来验证**
3. **保持单元测试的独立性。为了保证单元测试稳定可靠且便于维护，单元测试用例之间决不能互相调用，也不能依赖执行的先后次序。**
    - 说明:不能存在method2 需要依赖method1 的执行，将执行结果作为method2 的输入。 这种情况
4. **单元测试是可以重复执行的，不能受到外界环境的影响**
    - 说明：单元测试通常会被放到持续集成中，每次有代码 check in 时单元测试都会被执行。如果单测对外部 环境（网络、服务、中间件等）有依赖，容易导致持续集成机制的不可用。 
    - 正例：为了不受外界环境影响，要求设计代码时就把 SUT 的依赖改成注入，在测试时用 spring 这样的DI 框架注入一个本地（内存）实现或者 Mock 实现。 
5. **对于单元测试，要保证测试粒度足够小，有助于精确定位问题。单测粒度至多是类级别，一般是方法级别。**
    - 说明：只有测试粒度小才能在出错时尽快定位到出错位置。单测不负责检查跨类或者跨系统的交互逻辑， 那是集成测试的领域。 
6. **核心业务、核心应用、核心模块的增量代码确保单元测试通过**
    - 说明：新增代码及时补充单元测试，如果新增代码影响了原有单元测试，请及时修正。 
7. **单元测试代码必须写在如下工程目录:`src/test/java`;不允许写在业务代码目录下。**
    - 源码编译时会跳过此目录，而单元测试框架默认是扫描此目录。
8. 单元测试的基本目标:语句覆盖率达到70%；核心模块的语句覆盖率和分支覆盖率都要达到100%。
    - 说明：在工程规约的应用分层中提到的DAO层，Manager 层，可重用度高的 Service，都应该进行单元测试。
    - [白盒测试的几种覆盖方法：语句覆盖、判定覆盖、条件覆盖、判定/条件覆盖、组合覆盖和路径覆盖详解](https://blog.csdn.net/LOVEYSUXIN/article/details/82592588?utm_medium=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase)
9. 编写单元测试遵循BCDE原则，以保证被测试模块的交付质量
    - B：Border，边界值测试，包括循环边界、特殊取值、特殊时间点、数据顺序等。
    - C：Correct，正确的输入，并得到预期的结果。
    - D：Design，与设计文档相结合，来编写单元测试。 
    - E：Error，强制错误信息输入（如：非法数据、异常流程、业务允许外等），并得到预期的结果。
10. 对于数据库相关的查询，更新，删除等操作，不能假设数据库里的数据是存在的，或者直接操作数据库把数据插入进去，请使用程序插入或者导入数据的方式来准备数据。
    - 反例：删除某一行数据的单元测试，在数据库中，先直接手动增加一行作为删除目标，但是这一行新增数 据并不符合业务插入规则，导致测试结果异常。 
11. 和数据库相关的单元测试，可以设定自动回滚机制，不给数据库造成脏数据。或者对单元测试产生的数据有明确的前后缀标识。
    - 正例：在阿里巴巴企业智能事业部的内部单元测试中，使用
ENTERPRISE_INTELLIGENCE _ UNIT_ TEST_的前缀来标识单元测试相关代码。 
12. 对于不可测的代码在适当的时机做必要的重构，使代码变得可测，避免为了达到测试要求而书写不规范测试代码。
13. 在设计评审阶段，开发人员需要和测试人员一起确定单元测试范围，单元测试最好覆盖所有测试用例(UC)。 
14. 单元测试作为一种质量保障手段，在项目提测前完成单元测试，不建议项目发布后补充单元测试用例。 
15. 为了更方便地进行单元测试，业务代码应避免以下情况：
    - 构造方法中做的事情过多。 
    - 存在过多的全局变量和静态方法。
    - 存在过多的外部依赖。 
    - 存在过多的条件语句。
    - 说明：多层条件语句建议使用卫语句、策略模式、状态模式等方式重构。 
16. 不要对单元测试存在如下误解：。
    - 那是测试同学干的事情。本文是开发手册，凡是本文内容都是与开发同学强相关的。
    - 单元测试代码是多余的。系统的整体功能与各单元部件的测试正常与否是强相关的。 
    - 单元测试代码不需要维护。一年半载后，那么单元测试几乎处于废弃状态。 
    - 单元测试与线上故障没有辩证关系。好的单元测试能够最大限度地规避线上故障。 

## 4. 安全约规

1. **隶属于用户个人的页面或者功能必须进行权限控制校验。**
2. **用户敏感数据禁止直接展示，必须对展示数据进行脱敏。**
    - 例如: 中国大陆个人手机号码显示为:137****0969，隐藏中间 4 位，防止隐私泄露。 
3. **用户输入的 SQL 参数严格使用参数绑定或者 METADATA 字段值限定，防止 SQL 注入，禁止字符串拼接 SQL 访问数据库。**
4. **用户请求传入的任何参数必须做有效性验证。**
    - 忽略参数校验可能导致:
        - page size 过大导致内存溢出、
        - 恶意order by导致数据库慢查询、
        - 任意重定向、
        - SQL注入、
        - 反序列化注入、
        - 正则输入源串拒绝服务ReDos;
    - java客户端输入可以使用特殊的构造字符串来验证，可能导致死循环。
5. **禁止向HTML页面输出未经安全过滤或正确转义的用户数据**
6. **表单、Ajax提交必须执行CSRF安全验证**
    - CSRF(Cross-site request forgery)跨站请求伪造是一类常见编程漏洞。对于存在 CSRF 漏洞的应用/网站，攻击者可以事先构造好 URL，只要受害者用户一访问，后台便在用户不知情的情况下对数据库中用户参数进行相应修改。 
7. **URL外部重定向传入的目标地址必须执行白名单过滤。**
8. **在使用平台资源，譬如短信、邮件、电话、下单、支付，必须实现正确的防重放的机制，如数量限制、疲劳度控制、验证码校验，避免被滥刷而导致资损。**
    - 说明：如注册时发送验证码到手机，如果没有限制次数和频率，那么可以利用此功能骚扰到其它用户，并造成短信平台资源浪费。
9. 发贴、评论、发送即时消息等用户生成内容的场景必须实现防刷、文本内容违禁词过滤等风控策略。 

## 5. MySQL 数据库 


### 5.1 建表约规

1. **表达是与否概念的字段，必须使用 is_xxx 的方式命名，数据类型是 unsigned tinyint（1 表示是，0 表示否）。**
    - 任何字段如果为非负数，必须是 unsigned。 
    - 正例：表达逻辑删除的字段名 is_deleted，1 表示删除，0 表示未删除。 
2. **表名、字段名必须使用小写字母或数字，禁止出现数字开头，禁止两个下划线中间只出现数字。数据库字段名的修改代价很大，因为无法进行预发布，所以字段名称需要慎重考虑。**
    - MySQL 在 Windows 下不区分大小写，但在 Linux 下默认是区分大小写。因此，数据库名、表名、字段名，都不允许出现任何大写字母，避免节外生枝。
3. **表名不使用复数名词。**
    - 说明：表名应该仅仅表示表里面的实体内容，不应该表示实体数量，对应于 DO 类名也是单数形式，符合表达习惯。 
4. **禁用保留字，如 desc、range、match、delayed 等，请参考 MySQL 官方保留字([MySQL 关键字及保留字](https://www.cnblogs.com/zhuyeshen/p/11496545.html);[官网文档](https://dev.mysql.com/doc/refman/5.7/en/keywords.html))。**
5. **主键索引名为 pk_字段名；唯一索引名为 uk_字段名；普通索引名则为 idx_字段名。**
    - 说明：pk_  即 primary key；uk_  即  unique key；idx_  即 index 的简称。 
6. **小数类型为 decimal，禁止使用 float 和 double。**
    - 在存储的时候，float  和  double  都存在精度损失的问题，很可能在比较值的时候，得到不正确的结果。如果存储的数据范围超过  decimal  的范围，建议将数据拆成整数和小数并分开存储。
7. **如果存储的字符串长度几乎相等，使用 char 定长字符串类型。**
8. **varchar 是可变长字符串，不预先分配存储空间，长度不要超过 5000，如果存储长度大于此值，定义字段类型为 text，独立出来一张表，用主键来对应，避免影响其它字段索引效率。**
9. **表必备三字段：id, create_time, update_time。**
    - 说明：其中 id 必为主键，类型为 bigint unsigned、单表时自增、步长为 1。create_time, update_time的类型均为 datetime 类型。
10. **表的命名最好是遵循“业务名称_表的作用”。**；例如: `alipay_task / force_project / trade_config `
11. 库名与应用名称尽量一致。 
12. 如果修改字段含义或对字段表示的状态追加时，需要及时更新字段注释。 
13. 字段允许适当冗余，以提高查询性能，但必须考虑数据一致。冗余字段应遵循：
    - 不是频繁修改的字段。
    - 不是 varchar 超长字段，更不能是 text 字段。
    - 不是唯一索引的字段。 
    - 正例：商品类目名称使用频率高，字段长度短，名称基本一不变，可在相关联的表中冗余存储类目名   称，避免关联查询。 
14. 单表行数超过 500 万行或者单表容量超过 2GB，才推荐进行分库分表。 请不要在一开始就分库分表。
15. 合适的字符存储长度，不但节约数据库表空间、节约索引存储，更重要的是提升检索速度。 
如下表，其中无符号值可以避免误存负数，且扩大了表示范围。

|对象|年龄区间|类型|字节|表示范围|
|:---:|:---:|:---:|:---:|:---:|
|人|150岁之内|`tinyint unsigned`| 1 |无符号值：0到255| 
|龟|数百岁|`smallint unsigned`|2 |无符号值：0到65535 |
|恐龙化石|数千万年|`int unsigned`|4 |无符号值：0到约42.9亿| 
|太阳|约50亿年|`bigint unsigned`|8 |无符号值：0到约10的19次方|

### 5.2 索引规约

1. **业务上具有唯一性的字段，即使是多个多个字段的组合也必须建成唯一索引**
    - 说明:这样可以大大提高插叙速度，虽然插入速度会有所影响但是影响不大。另外没有唯一索引，容易产生脏数据。
2. **超过三个表不能`JOIN`,需要JOIN的字段类型必须一致，多表关联查询被关联的字段需要有索引。**
    - 说明:即使是双表，也需要注意表索引和SQL性能。
3. **在varchar上面建立索引时，需要考虑字符串的长度。没有必要设置为全字段索引；根据实际文本区分度决定索引长度即可**
    - 对于一般的字符串类型的数据，长度可以设置为20左右，区分度在90%以上。可以使用`count(distinct left(列名，索引长度))/count()`的区分度来确定
4. **因为索引的最左匹配原则，因此页面搜索禁止左模糊或者全模糊，如果需要请走搜索引擎来决定**
5. 为了利用索引的有序性，order by最后的字段是组合索引的一部分，并且放在索引组合顺序的最后，避免出现file_sort的情况
    - 正例: where a =? and b = ? order by c;索引:a_b_c;
    - 反例: 索引如果存在查询范围，索引的有序性无法利用，如: WHERE a>10 ORDER BY b;
6. 利用覆盖索引来进行查询，避免回表操作。
7. 利用延迟关联或者子查询优化超多分页场景。
    - MySQL 并不是跳过offset行，二十取offset+N行，然后返回放弃前offset行，返回N行，因此要么控制返回的总页数，要么对超过特定阈值的页数进行SQL改写。
8. SQL性能优化的目标：至少要达到range级别，要求是ref级别；如果可以consts最好。
    - 说明：
        - coonsts表单中最多只有一个匹配行(主键或者唯一索引)，在优化阶段即可读取到数据。
        - ref指的是使用普通的索引(normal index)
        - range 对索引进行范围检索
    - 反例：explain表的结果，type=index，索引物理文件全扫描，速度非常慢，这个index级别比较range还低，与全表扫描是小巫见大巫。
    - 访问类型, 显示查询使用了何种类型, 从最好到最差依次是 :`system>const>eq_ref>ref>range>index>ALL`
    - [MySQL自带查询优化器(MySQL Query Optimizer)](https://zhuanlan.zhihu.com/p/56529089)
    - [MySql范围查找时，索引不生效问题原因](https://blog.csdn.net/qq_25188255/article/details/81316498)
9. 因为最左匹配原则；建组合索引的时候，区分度最高的在最左边。
    - 正例：如果where a=? and b=?，a列的几乎接近于唯一值，那么只需要单建 idx_a索引即可。
    - 说明：存在非等号和等号混合判断条件时，在建索引时，请把等号条件的列前置。如：where c>? and d=? 那么即使 c的区分度更高，也必须把d 放在索引的最前列，即建立组合索引idx_d_c。 
10. 防止因字段类型不同造成的隐式转换，导致索引失效。
11. 创建索引时避免有如下极端误解
    - 经常查询的才需要创建一个索引。
    - 索引会消耗空间，严重拖慢记录更新以及行的新增速度。
    - 抵制唯一索引。认为业务的唯一性一律要在应用层通过"先查后插"方式解决。

### 5.3 SQL语句

1. **不要使用count(列名)或者count(常量)来代替count(*),count(*)时SQL中的统计方法；与数据库无关，与NULL和非NULL无关**
2. **`count(distinct col)`计算该列除NULL之外的所有不重复行数，注意count(distinct col1, col2)如果其中一列全为NULL,那么另外一列有不同的值，也返回为0**
3. **当某一列的值全是NULL时，count(col)的返回结果为0，但sum(col)的返回结果为NULL，因此使用sum()时需注意NPE问题。**
    - 正例:使用如下方式来避免sum的NPE问题：SELECT IFNULL(SUM(column), 0) FROM table;
4. **使用ISNULL()来判断是否为NULL值。**
    - 说明：NULL与任何值的直接比较都为NULL。如`<>`、`=`、`<>`等都是NULL
5. **代码中写分页查询逻辑时，若count为0应直接返回，避免执行后面的分页语句。**
6. **不得使用外键与级联，一切外键概念必须在应用层解决。**
    - 外键与级联更新适用于单机低并发，不适合分布式、高并发集群；级联更新是强阻塞，存在数据库更新风暴的风险；外键影响数据库的插入速度。
7. **禁止使用存储过程([MySQL 存储过程](https://www.runoob.com/w3cnote/mysql-stored-procedure.html))，存储过程难以调试和扩展，更没有移植性。**
8. **数据订正（特别是删除、修改记录操作）时，要先select，避免出现误删除，确认无误才能执行更新语句。**
9. **对于数据库中表记录的查询和变更，只要涉及多个表，都需要在列名前加表的别名（或 表名）进行限定。**
    - 说明：对多表进行查询记录、更新记录、删除记录时，如果对操作列没有限定表的别名（或表名），并且 操作列在多个表中存在时，就会抛异常。 
    - 正例：`select t1.name from table_first as t1 , table_second as t2 where t1.id=t2.id`; 
    - 反例：在某业务中，由于多表关联查询语句没有加表的别名（或表名）的限制，正常运行两年后，最近在 某个表中增加一个同名字段，在预发布环境做数据库变更后，线上查询语句出现出 1052 异常：Column 'name' in field list is ambiguous。
10. SQL语句中表的别名前加as，并且以t1、t2、t3、...的顺序依次命名。 
    - 说明：1）别名可以是表的简称，或者是根据表出现的顺序，以 t1、t2、t3 的方式命名。2）别名前加 as 使别名更容易识别。
    - 正例：`select t1.name from table_first as t1, table_second as t2 where t1.id=t2.id`; 
11. in操作能避免则避免，若实在避免不了，需要仔细评估in后边的集合元素数量，控制在1000个之内。

12. 如果有国际化需要，所有的字符存储与表示，均以utf-8编码，注意字符统计函数的区别。
    - 如果需要存储表情，那么选择utf8mb4来进行存储，注意它与utf-8编码的区别。
11. TRUNCATE TABLE 比 DELETE 速度快，且使用的系统和事务日志资源少，但TRUNCATE无事务且不触发trigger，有可能造成事故，故不建议在开发代码中使用此语句。
    - TRUNCATE TABLE 在功能上与不带 WHERE 子句的 DELETE 语句相同。

### 5.4 ORM 映射

1. **在表查询中，一律不要使用 * 作为查询的字段列表，需要哪些字段必须明确写明。** 原因如下：
    - 增加查询分析器解析成本
    - 增加字段容易月resultMap配置不一致
    - 无用字段增加网络消耗，尤其是text类型的字段
2. **POJO类的布尔属性不能加is,但是数据库字段必须加`is_`,要求在resultMap中进行字段与属性之间的映射**
3. **不要用resultClass当返回参数，即使所有类属性名与数据库字段一一对应，也需要定义；反过来，每一个表也必然有一个POJO类与之对应。**
    - 说明: 配置映射关系，使字段与DO类解耦，方便维护。
4. **sql.xml配置参数使用：#{}，#param# 不要使用${} 此种方式容易出现SQL注入。**
    - 说明:因为${}是替换，可能不检查变量安全性，造成问题。
5. **iBATIS自带的queryForList(String statementName,int start,int size)不推荐使用。**
    - 说明：其实现方式是在数据库取到statementName对应的SQL语句的所有记录，再通过subList取start,size的子集合。
6. **不允许直接拿HashMap与Hashtable作为查询结果集的输出。**
    - 说明: resultClass="Hashtable"，会置入字段名和属性值，但是值的类型不可控。([查询结果为HashMap类型](https://blog.csdn.net/xushiyu1996818/article/details/89076657))
7. **更新数据表记录时，必须同时更新记录对应的gmt_modified字段值为当前时间。**
8. 不要写一个大而全的数据更新接口。传入为POJO类，不管是不是自己的目标更新字段，都进行update table set c1=value1,c2=value2,c3=value3; 这是不对的。执行SQL时，不要更新无改动的字段，一是易出错；二是效率低；三是增加binlog存储。
9. @Transactional事务不要滥用。事务会影响数据库的QPS，另外使用事务的地方需要考虑各方面的回滚方案，包括缓存回滚、搜索引擎回滚、消息补偿、统计修正等。
10. `<isEqual>`中的compareValue是与属性值对比的常量，一般是数字，表示相等时带上此条件；`<isNotEmpty>`表示不为空且不为null时执行；`<isNotNull>`表示不为null值时执行。

## 6 工程结构

### 6.1 应用分层

1. 图中默认上层依赖于下层，箭头关系表示可直接依赖，如：开放接口层可以依赖于Web层，也可以直接依赖于Service层，依此类推：
    - 开放接口层：可直接封装Service方法暴露成RPC接口；通过Web封装成http接口；进行网关安全控制、流量控制等。
    - 终端显示层：各个端的模板渲染并执行显示的层。当前主要是velocity渲染，JS渲染，JSP渲染，移动端展示等。
    - Web层：主要是对访问控制进行转发，各类基本参数校验，或者不复用的业务简单处理等。
    - Service层：相对具体的业务逻辑服务层。
    - Manager层：通用业务处理层，它有如下特征：
        - 对第三方平台封装的层，预处理返回结果及转化异常信息。
        - 对Service层通用能力的下沉，如缓存方案、中间件通用处理。
        - 与DAO层交互，对多个DAO的组合复用。
    - DAO层：数据访问层，与底层MySQL、Oracle、Hbase等进行数据交互。
    - 外部接口或第三方平台：包括其它部门RPC开放接口，基础平台，其它公司的HTTP接口。
![数据层级](https://wangpengcheng.github.io/img/2020-06-16-21-11-58.jpg)

2. 关于异常和日志:
    - DAO层异常较多，无法用细粒度的异常进行catch，使用catch(Exception e)方式，并throw new DAOException(e)，不需要打印日志
    - 在Manager/Service 层使用日志，并捕获DAO层的错误并记录；避免性能浪费
    - 在Service层出现异常时，必须记录出错日志到磁盘，尽可能带上参数信息，相当于保护案发现场
    - 如果Manager层与Service同机部署，日志方式与DAO层处理一致，如果是单独部署，则采用与Service一致的处理方式。
    - web层绝对不能继续向上抛出异常；因为处于顶层；可能导致页面无法正常渲染。应该直接跳到友好错误页面
    - 开放接口层要将异常处理成错误码和错误信息方式返回。
3. 分层领域模型规约：
    - DO（Data Object）：此对象与数据库表结构一一对应，通过DAO层向上传输数据源对象。
    - DTO（Data Transfer Object）：数据传输对象，Service或Manager向外传输的对象。
    - BO（Business Object）：业务对象，由Service层输出的封装业务逻辑的对象。
    - AO（Application Object）：应用对象，在Web层与Service层之间抽象的复用对象模型，极为贴近展示层，复用度不高。
    - VO（View Object）：显示层对象，通常是Web向模板渲染引擎层传输的对象。
    - Query：数据查询对象，各层接收上层的查询请求。注意超过2个参数的查询封装，禁止使用Map类来传输。

### 6.2 二方库依赖

1. **定义GAV遵从以下规则：**
    - GroupID格式：com.{公司/BU }.业务线 [.子业务线]，最多4级。
        - 说明：{公司/BU} 例如：alibaba/taobao/tmall/aliexpress等BU一级；子业务线可选。
        - 正例：com.taobao.jstorm 或 com.alibaba.dubbo.register
    - ArtifactID格式：产品线名-模块名。语义不重复不遗漏，先到中央仓库去查证一下。
        - 正例：dubbo-client / fastjson-api / jstorm-tool
    - Version：详细规定参考下方
2. **二方库版本号命名方式：主版本号.次版本号.修订号**
    - 主版本号：产品方向改变，或者大规模API不兼容，或者架构不兼容升级。
    - 次版本号：保持相对兼容性，增加主要功能特性，影响范围极小的API不兼容修改。
    - 修订号：保持完全兼容性，修复BUG、新增次要功能特性等。
    - 注意起始版本号必须为：1.0.0，而不是0.0.1，正式发布的类库必须先去中央仓库进行查证，使版本号有延续性，正式版本号不允许覆盖升级。如当前版本：1.3.3，那么下一个合理的版本号：1.3.4 或 1.4.0 或 2.0.0
3. **线上应用不要依赖SNAPSHOT版本（安全包除外）**
    - 说明：不依赖SNAPSHOT版本是保证应用发布的幂等性。另外，也可以加快编译时的打包构建。
4. **二方库的新增或升级，保持除功能点之外的其它jar包仲裁结果不变。如果有改变，必须明确评估和验证。**
    - 说明：在升级时，进行dependency:resolve前后信息比对，如果仲裁结果完全不一致，那么通过dependency:tree命令，找出差异点，进行<exclude>排除jar包。
5. **二方库里可以定义枚举类型，参数可以使用枚举类型，但是接口返回值不允许使用枚举类型或者包含枚举类型的POJO对象。**
    - 因为这样枚举类型容易添加，但是对使用者来说是多变的。([枚举类的特殊使用规范](https://blog.csdn.net/zzu007/article/details/83342998))
6. **依赖于一个二方库群时，必须定义一个统一的版本变量，避免版本号不一致。**
    - 说明：依赖springframework-core,-context,-beans，它们都是同一个版本，可以定义一个变量来保存版本：${spring.version}，定义依赖的时候，引用该版本。
7. **禁止在子项目的pom依赖中出现相同的GroupId，相同的ArtifactId，但是不同的Version。**
    - 说明: 在本地调试时会使用各子项目指定的版本号，但是合并成一个war，只能有一个版本号出现在最后的lib目录中。可能出现线下调试是正确的，发布到线上却出故障的问题。
8. 底层基础技术框架、核心数据管理平台、或近硬件端系统谨慎引入第三方实现,因为硬件的非统一性，容易出问题。
9. 所有pom文件中的依赖声明放在<dependencies>语句块中，所有版本仲裁放在<dependencyManagement>语句块中。
10. 说明：<dependencyManagement>里只是声明版本，并不实现引入，因此子项目需要显式的声明依赖，version和scope都读取自父pom。而<dependencies>所有声明在主pom的<dependencies>里的依赖都会自动引入，并默认被所有的子项目继承。
11. 二方库不要有配置项，最低限度不要再增加配置项。
12. 为避免应用二方库的依赖冲突问题，二方库发布者应当遵循以下原则
    - **精简可控原则**：移除一切不必要的API和依赖，只包含 Service API、必要的领域模型对象、Utils类、常量、枚举等。如果依赖其它二方库，尽量是provided引入，让二方库使用者去依赖具体版本号；无log具体实现，只依赖日志框架。
    - **稳定可追溯原则**:每个版本的变化应该被记录，二方库由谁维护，源码在哪里，都需要能方便查到。除非用户主动升级版本，否则公共二方库的行为不应该发生变化。

### 6.3 服务器

1. 高并发服务器默认的TCPtime_wait时间是240s可以在`/etc/sysctl.conf`设置一个较小的值;增加TCP连接符号的的释放。
2. 调大服务器所支持的最大文件句柄数（File Descriptor，简写为fd）。
    - Linux进程最大支持的文件句柄数目是1024个，可以进行更改适配。
3. 给JVM环境参数设置-XX:+HeapDumpOnOutOfMemoryError参数，让JVM碰到OOM场景时输出dump信息。
4. 在线上生产环境，JVM的Xms和Xmx设置一样大小的内存容量，避免在GC 后调整堆大小带来的压力。
5. 服务器内部重定向使用forward；外部重定向地址使用URL拼装工具类来生成，否则会带来URL维护不一致的问题和潜在的安全风险。

## 7 设计规约

1. **存储方案和底层数据结构的设计获得评审一致通过，并沉淀成为文档。**
2. **在需求分析阶段，如果与系统交互的User超过一类并且相关的User Case超过5个，使用用例图来表达更加清晰的结构化需求。**
3. **如果某个业务对象的状态超过3个，使用状态图来表达并且明确状态变化的各个触发条件。**
    - 说明：状态图的核心是对象状态，首先明确对象有多少种状态，然后明确两两状态之间是否存在直接转换关系，再明确触发状态转换的条件是什么。 
    - 正例：淘宝订单状态有已下单、待付款、已付款、待发货、已发货、已收货等。比如已下单与已收货这两种状态之间是不可能有直接转换关系的。

4. **如果系统中某个功能的调用链路上的涉及对象超过3个，使用时序图来表达并且明确各调用环节的输入与输出。**
    - 说明：时序图反映了一系列对象间的交互与协作关系，清晰立体地反映系统的调用纵深链路。
5. **如果系统中模型类超过5个，并且存在复杂的依赖关系，使用类图来表达并且明确类之间的关系。**
    - 说明：类图像建筑领域的施工图，如果搭平房，可能不需要，但如果建造蚂蚁Z空间大楼，肯定需要详细的施工图。
6. **如果系统中超过2个对象之间存在协作关系，并且需要表示复杂的处理流程，使用活动图来表示。**
    - 说明：活动图是流程图的扩展，增加了能够体现协作关系的对象泳道，支持表示并发等。

7. 系统架构设计时明确以下目标： 
    - 确定系统边界。确定系统在技术层面上的做与不做。 
    - 确定系统内模块之间的关系。确定模块之间的依赖关系及模块的宏观输入与输出。
    - 确定指导后续设计与演化的原则。使后续的子系统或模块设计在一个既定的框架内和技术方向上继 续演化。 
    - 确定非功能性需求。非功能性需求是指安全性、可用性、可扩展性等。 
8. 需求分析与系统设计在考虑主干功能的同时，需要充分评估异常流程与业务边界。
    - 反例：用户在淘宝付款过程中，银行扣款成功，发送给用户扣款成功短信，但是支付宝入款时由于断网演 练产生异常，淘宝订单页面依然显示未付款，导致用户投诉。
9. 类在设计与实现时要符合单一原则。
10. 谨慎使用继承的方式来进行扩展，优先使用聚合/组合的方式来实现。
    - 说明：不得已使用继承的话，必须符合里氏代换原则，此原则说父类能够出现的地方子类一定能够出现。
11. 系统设计时，根据依赖倒置原则，尽量依赖抽象类与接口，有利于扩展与维护。
    - 低层次模块依赖于高层次模块的抽象，方便系统间的解耦。
12. 系统设计时，注意对扩展开放，对修改闭合。
    - 说明：极端情况下，交付线上生产环境的代码都是不可修改的，同一业务域内的需求变化，通过模块或类的扩展来实现。
13. 系统设计阶段，共性业务或公共行为抽取出来公共模块、公共配置、公共类、公共方法等，避免出现重复代码或重复配置的情况。
14. 避免如下误解：敏捷开发 = 讲故事 + 编码 + 发布。
15. 系统设计主要目的是明确需求、理顺逻辑、后期维护，次要目的用于指导编码。
16. 可扩展性的本质是找到系统的变化点，并隔离变化点。 
    - 说明：世间众多设计模式其实就是一种设计模式即隔离变化点的模式。 
    - 正例：极致扩展性的标志，就是需求的新增，不会在原有代码交付物上进行任何形式的修改。 
17. 设计的本质就是识别和表达系统难点，找到系统的变化点，并隔离变化点。
    - 说明：世间众多设计模式目的是相同的，即隔离系统变化点。
18. 代码即文档的观点是错误的，清晰的代码只是文档的某个片断，而不是全部。
    - 说明：代码的深度调用，模块层面上的依赖关系网，业务场景逻辑，非功能性需求等问题是需要相应的文 档来完整地呈现的。 
19. 系统架构设计的目的：
    - 确定系统边界。确定系统在技术层面上的做与不做。 
    - 确定系统内模块之间的关系。确定模块之间的依赖关系及模块的宏观输入与输出。
    - 确定指导后续设计与演化的原则。使后续的子系统或模块设计在规定的框架内继续演化。
    - 确定非功能性需求。非功能性需求是指安全性、可用性、可扩展性等。
20. 在做无障碍产品设计时，需要考虑到：
    - 所有可交互的控件元素必须能被tab键聚焦，并且焦点顺序需符合自然操作逻辑。
    - 用于登陆校验和请求拦截的验证码均需提供图形验证以外的其它方式。
    - 自定义的控件类型需明确交互方式。


## 最终考试成绩证书


![本地证书](https://wangpengcheng.github.io/img/2020-06-21-01-29-alicodestanrd.jpg)

