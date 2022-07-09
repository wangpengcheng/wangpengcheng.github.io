---
layout:     post
title:      高性能MySQL读书笔记(一)
subtitle:   高性能MySQL读书笔记(一)
date:       2019-12-27
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - 数据库
    - 后台开发
    - MySql
---

# 高性能MySQL(第3版)

# 第 1 章 MySQL架构与历史

![](https://wangpengcheng.github.io/img/2019-12-28-13-27-37.png)

### 1.2.2 锁的粒度

- 表锁：MySQL中最基本的锁策略，是开销最小的锁；在`ALTER TABLE`之类的语句中会使用表锁
- 行级锁：最大程度支持并发处理(同时也带来了最大的锁开销)

事物的ACID:
- A(atomicity):原子性
- C(consistency):一致性;
- I(isolation):隔离性
- D(durability):持久性；

### 1.3.1 隔离级别

![](https://wangpengcheng.github.io/img/2019-12-28-14-13-07.png)

InnoDB的处理死锁的方法是，将持有最少行级排他锁的事物进行回滚。

### 1.3.4 MySQL中的事物

MySQL中每个语句就是一个事物，会自动提交;可以通过`AUTOCOMMIT`变量来启用或者禁用自动提交模式

MYSQL 中可以通过执行`SET TRANSACTION ISOLATION LEVEL`命令来设置隔离级别。新的隔离级别会在下一个事物开始的时候生效。

事物主要是由下层的存储引擎实现的。所以在同一个事物中使用多种存储引擎是不可靠的。

InnoDB中存在两阶段锁定协议。在事物执行过程中执行锁定，锁只有在执行commit或者ROLLBACK的时候才会释放，并且所有的锁都是在同一时刻被释放。前面描述的锁定都是隐式的锁定。InnoDB会根据隔离级别在需要的时候自动加锁。

## 1.4 多版本并发控制(MVCC)

MVCC是行级锁的一个变种，在很多情况下避免了加锁操作。因此开销更低。主要是同保存数据在某个时间点的快照来实现的。

InnoDB通过在每行记录后面保存两个隐藏的列来保存行创建的时间和删除时间(系统版本号)。用来和查询到的每个版本号进行比较

SELECT 检查条件如下：
- 只查找早于当前事物版本的数据行(也就是，行的系统版本号小于或者等于事物的系统版本号)，确保事物的读取的行，要么是在事物开始前就已经存在的，要么是事物自身插入或者修改过的
- 行的删除版本要么未定义，要么大于当前事物版本号，保证事物读取到的行，在事物开始前未被删除

INSERT:
- InniDB为新插入的每一行保存当前系统版本号作为行版本号

DELECT
- InnoDB为删除的每一行保存当前系统版本号作为删除标识。

UPDATE
- InnoDB为插入一行新记录，保存当前系统版本好作为行版本号，同时保存当前系统版本号到原来的行作为行删除标识

## 1.5 MySQL的存储引擎

_参考链接:_ [mysql存储引擎](https://blog.csdn.net/yjclsx/article/details/81911027?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-4.edu_weight&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-4.edu_weight)

文件系统中，MySQL将每个数据库保存为数据目录下的一个子目录。创建表时，会创建.frm保存表的定义和数据

InnoDB是默认事物型引擎，设计用来处理大量的短期(short-liived)事物，短期事物大部分正常提交，很少会被回滚。它支持排序创建索引、删除或者增加索引时不需要复制全表数据、新的支持压缩的存储格式。

InnoDB可以通过[间隙锁](https://www.cnblogs.com/crazylqy/p/7821481.html)(检索条件必须有索引（没有索引的话，mysql会全表扫描，那样会锁定整张表所有的记录，包括不存在的记录，此时其他事务不能修改不能删除不能添加）)策略防止幻读的出现。

InnoDB能够自动在内存中创建hash索引，以加速读操作的自适应哈希索引;加速插入操作的插入缓冲区(insert buffer)等

### 1.5.2 MyISAM存储引擎

5.1之前的默认引擎但是，崩溃后无法恢复。

其将表存储在：
数据文件(.MYD):
索引文件(.MYI):

MyISAM可以对表进行压缩。压缩表是不能进行修改的(除非先将表解除压缩)；可以极大的减少磁盘空间占用，因此也可以减少磁盘I/O，从而提升查询性能。
### 1.5.3 MEMORY存储引擎

Memory存储引擎使用存在于内存中的内容来创建表。每个memory表只实际对应一个磁盘文件，格式是.frm。memory类型的表访问非常的快，因为它的数据是放在内存中的，并且默认使用HASH索引，但是一旦服务关闭，表中的数据就会丢失掉。
MEMORY存储引擎的表可以选择使用BTREE索引或者HASH索引，两种不同类型的索引有其不同的使用范围

Hash索引优点：
Hash 索引结构的特殊性，其检索效率非常高，索引的检索可以一次定位，不像B-Tree 索引需要从根节点到枝节点，最后才能访问到页节点这样多次的IO访问，所以 Hash 索引的查询效率要远高于 B-Tree 索引。
Hash索引缺点： 那么不精确查找呢，也很明显，因为hash算法是基于等值计算的，所以对于“like”等范围查找hash索引无效，不支持；

Memory类型的存储引擎主要用于哪些内容变化不频繁的代码表，或者作为统计操作的中间结果表，便于高效地对中间结果进行分析并得到最终的统计结果，。对存储引擎为memory的表进行更新操作要谨慎，因为数据并没有实际写入到磁盘中，所以一定要对下次重新启动服务后如何获得这些修改后的数据有所考虑。

### 1.5.4 MERGE存储引擎

Merge存储引擎是一组MyISAM表的组合，这些MyISAM表必须结构完全相同，merge表本身并没有数据，对merge类型的表可以进行查询，更新，删除操作，这些操作实际上是对内部的MyISAM表进行的

### 1.5.5 选择合适的引擎

全文索引，建议优先考虑InnoDB加上Sphinx组合；MyISAM和ARchive存储引擎对开销低，插入速度非常快

对于更换引擎可以使用`ALTER TABLE`语句更换数据引擎。如下：

```sql
ALTER TABLE mytable ENGINE=InnoDB;
```

使用mysqldump工具将数据导出到文件

使用CREATE创建新表，再将旧表的数据导入新表之中

```sql
CREATE TABLE innodb_table LIKE myisam_table;
ALTER TABLE innodb_table ENGINE=InnoDB;
INSERT INTO innodb_table SELECT *FROM myisam_table;
```

## 1.7 MySQL的开发模式

# 第 2 章 MySQL基准测试

测试的指标：
- 吞吐量：单位时间内的事物处理数。
- 响应时间或者延迟：测试任务所需的整体时间。
- 并发性：同时工作中的线程数或者连接数。
- 可扩展性：增加资源，获取成倍的吞吐量

# 第 3 章 服务器性能剖析

## 3.1 性能优化简介

性能优化：在一定的工作负载下，尽可能地降低响应时间

应该尽量，搞清楚程序花费的时间在哪里

### 3.1.1 通过性能剖析进行优化

测量任务花费的时间，然后对结果进行统计和排序。将重要的任务排到前面。

测试脚本如下：

```sh
#!/bin/sh
INTERVAL=5
PREFIX=$INTERVAL-sec-status
RUNFILE=/home/benchmarks/runing
mysql -e 'SHOW GLOBAL VARIABLES' >> mysql-variables
while test -e $RUNFILE; do
    file=$(date+%F_%I)
    sleep=$(data+%s.%N awk "{print $INTERVAL - (\$1 % INTERVAL)}")
    sleep $sleep
    ts="$(data+"TS %s.%N %F %T")"
    loadavg="$(uptime)"
    echo "$ts $loadavg" >> $PREFIX-${file}-status
    mysql -e 'SHOW GLOBAL STATUS' >> $PREFIX-${file}-status &
    echo "$ts $loadavg" >> $PREFIX-${file}-innodbstatus
    mysql -e 'SHOW ENGINE INNODB STATUS \G' >> $PREFIX-${file}-status &
    echo "$ts $loadavg" >> $PREFIX-${file}-processlist
    mysql -e 'SHOW FULL PROCESSLIST \G' >> $PREFIX-${file}-processlist &
    echo $ts;
done
echo Exiting because $RUNFILE does not exist.
```

## 3.3 剖析MySQL查询

慢查询日志开销最低，精度最高的测量查询时间的工具--`pt-query-digest`([MySQL慢查询分析工具pt-query-digest详解](https://blog.csdn.net/xiaoweite1/article/details/80299754))。

### 3.3.2 剖析单条查询

1. 使用`SET profiling=1`:来开启单条时间统计查询功能。
2. 执行mysql相关语句。
3. 使用`SHOW PROFILE`:一条插叙提交给服务器式，会记录剖析信息到一张临时表，并给查询赋予一个从1开始的整数标识符。
4. `SHOW PROFILE FOR QUERY 1;`:显示比较小的查询时间

使用`SHOW STATUS`命令返回计数器，返回各个操作的相关执行次数.`SHOW STATUS`本身也会也会创建一个临时表，而且也会通过句柄操作访问此临时表。会影响到`SHOW STATUS`结果中对应的数字。

### 3.3.3 使用性能剖析

使用`SHOW PROCESSLIST`不停捕获和输出

# 第 4 章 Schema与数据类型优化

Schema：数据库中的组织结构，schemas andschemata都可以作为复数形式。模式中包含了schema对象，可以是表(table)、列(column)、数据类型(data type)、视图(view)、存储过程(stored procedures)、关系(relationships)、主键(primary key)、外键(foreign key)等。数据库模式可以用一个可视化的图来表示，它显示了数据库对象及其相互之间的关系。([数据库中的Schema是什么?](https://blog.csdn.net/u010429286/article/details/79022484))

## 4.1 选择优化的数据类型

有几个原则如下：

- 更小的通常更好:尽可能使用较小的数据结构,因为其占用更少的磁盘，内存和CPU
- 简单就好
- 尽量避免NULL


# 第5章 创建高性能的索引

索引(键--key):是存储引擎用于快速找到记录的一种数据结构。是索引的基本功能。

索引优化是对查询性能优化的最有效手段。索引能够轻易将查询新年提高几个数量级

## 5.1 索引基础

使用中如果在查询条件上建立了索引，mysql将使用索引找到其对应的行--先在索引上按照值及您修改查找，然后返回所有包含该值的数据行。避免了全表扫描

对于多个列的索引，顺序十分重要，MySQL只能高效地使用索引的最左前缀。

### 5.1.1 索引类型

不同的存储引擎的索引的工作方式不同，MySQL支持的索引类型如下：

- B-Tree 索引：它是大多是引擎支持的索引。NDB集群存储引擎内部实际使用了T-Tree结构存储(BTREE);InnoDB则使用的是B+Tree.
    - B-Tree([B-Tree和B+Tree](https://blog.csdn.net/qq_36204764/article/details/99291423);[BTree和B+Tree详解](https://blog.csdn.net/Hubery_James/article/details/80565137)) MyISAM使用前缀压缩计数使得索引更小；Innodb按照原数据格式进行存储。MyISAM索引通过数据的物理位置引用被索引的行，而InnoDB则根据主键引用被索引的行。
    下面是Innodb中的索引工作原理和方式：
    ![Innodb索引](https://wangpengcheng.github.io/img/2020-02-09-14-02-20.png)
    - 假设有如下数据表：

```sql
CREATE TABLE(
    last_name VARCHAR(50) NOT NULL,
    first_name VARCHAR(50) NOT NULL,
    dob     DATE NOT NULL,
    gender  enum('m','f') NOT NULL,
    KEY(last_name,first_name,dob)
);
```

其索引结构如下(注意这里是以第一列为优先，前面相同再比较后面):
![索引结构](https://wangpengcheng.github.io/img/2020-02-09-14-08-20.png)

B-Tree索引适用于全键值、键值范围或者键前缀查找，其中键前缀查找只适用于根据最左前缀的查找。对下列类型的查询有效：

1. 全值匹配
2. 匹配左前缀
3. 匹配列前缀
4. 匹配范围值
5. 精确匹配某一列范围匹配另外一列
6. ORDER的有序排序

B-Tree的限制：
1. 不是按照最左列开始查找，则无法使用索引。
2. 不能跳过索引中的列，中间有缺失，则只使用索引的第一列
3. 查询中有某个列的查询范围，则其右边所有列都无法使用索引优化查找。例如`WHERE last_name='Smith' AND frist_name LIKE 'J%' AND dob='1976-12-23'`,这个查询只能使用索引的前两列，因为这里的LIKE是一个范围条件。

- 哈希(Hash)索引:基于hash表实现，只有精确匹配**所有列的查询才有效**。MySQL中只有Memory引擎显式支持hash索引(也支持非唯一hash索引)。可以使用`KEY USING HASH(fname)`来使用hash索引。hash索引查找的速度非常快。但是其限制如下：
    1. 只包含hash值和指针，而不存储字段值，不能使用索引中的值来避免读取行。
    2. hash值并不是按照索引值顺序存储的，所以也无法用于排序。
    3. 不支持部分索引匹配列查找。
    4. 只支持等值比较查询，包括=、IN(),不支持任何的范围查询。
    5. 访问hash的数据非常快，除非有很多hash冲突
    6. hash冲突较多时，一些索引维护操作的代价也会非常的高。

可以在InnoDB引擎中对关键字段，使用hash映射--自适应哈希索引(adaptive hash index)，相当于添加了一个另类的索引，这样对于长文本的检索速度会有提升。可以自定义hash函数和创建触发器([Mysql中的触发器](https://www.cnblogs.com/fps2tao/p/10400936.html))

- 空间数据索引(R-Tree): MyISAM表支持空间索引，可以用作地理数据存储，建议使用PostgreSQL来进行空间三维坐标的存储。
- 全文索引:查找文本中的关键词，而不是直接比较索引中的值。
- 其他索引：TokuDB使用分形树索引([TokuDB的索引结构–分形树的实现](https://www.cnblogs.com/chaosheng/p/5250037.html))

## 5.2 索引的优点

索引的优点内容如下:
1. 索引大大减小了服务器需要扫描的数据量
2. 索引可以帮助服务器避免排序和临时表
3. 索引可以将随机I/O变为顺序I/O

但是索引不是万能的，可以使用Inforight来实现TB级别的数据，使用块级别元数据计数来代替索引。

## 5.3 高性能的索引策略

### 5.3.1 独立的列

指索引列不能是表达式的一本分，也不能是函数的参数；例如下面的查询无法使用`actor_id`的索引

```sql
SELECT actor_id FROM sakila.actor WHERE actor_id+1=5;
```
### 5.3.2 前缀索引和索引选择性

对于长字符串，可以设置索引的部分字符，这样可以大大节约索引空间，从而提高索引效率。

NySQL中使用LEFT进行前缀的提取和获取。通常需要计算完整列的选择性，并使前缀的选择性接近于完整列的选择性。计算过程如下：

```sql
# 计算一般情况下的命中率
SELECT COUNT(DISTINCT city)/COUNT(*) FROM sakila.city_demo;
# 计算不同前缀时的命中率

SELECT COUNT(DISTINCT LEFT(city,3))/COUNT(*) AS sel3,
COUNT(DISTINCT LEFT(city,4))/COUNT(*) AS sel3,
COUNT(DISTINCT LEFT(city,5))/COUNT(*) AS sel3,
COUNT(DISTINCT LEFT(city,6))/COUNT(*) AS sel3,
COUNT(DISTINCT LEFT(city,7))/COUNT(*) AS sel3,
FROM sakila.city_demo;
```

有时也需要考虑平均情况下的合适前缀索引长度；

接下来就按照指定长度创建前缀索引

```sql
ALTER TABLE sakila.city_demo ADD KEY (city(7));
```

它能够使索引更小更快，但是没有办法使用ORDER BY和GROUP BY。

### 5.3.3 多列索引

- 当出现服务器对多个索引做相交操作时(多个AND条件),一个包含所有相关列的多列索引，而不是多个独立的单列索引
- 当服务器需要对多个索引做联合操作时(多个OR条件)，通常需要耗费大量CPU和内存资源在算法发缓存、排序和合并操作上。
- 优化器一般不会把这些纳入到“查询成本”中，优化器只关心随机页面读取。让查询成本被低估，导致该执行计划还不如直接进行全表扫描。

### 5.3.4 选择合适的索引列顺序

在一个多列B-Tree中，索引列的顺序意味着索引首先按照最左列进行排序，其次时第二列。

一般：将选择性最高的列放到索引最前列(不考虑分组和排序)。
对于两个选择条件时，应该尽量使用条件值的基数比较高的值。但是应该注意各种选择的引用条件和选择。

### 5.3.5 聚簇索引

_参考链接：_ [聚簇索引与非聚簇索引（也叫二级索引）](https://www.jianshu.com/p/fa8192853184)

聚簇索引并不是一种单独的索引类型，而是一种数据存储的方式。InnoDB的聚簇索引实际上在同一个结构中保存了B-Tree索引和数据行

当表有聚簇索引时，它的数据行实际上存放在索引的叶子页，数据行和相邻的键值紧凑地存储在一起。一个表只能有一个聚簇索引(覆盖索引可以模拟多个聚簇索引的情况)。其存放如下图：

![显示图片](https://wangpengcheng.github.io/img/2020-02-10-14-45-02.png)

InnoDB使用主键作为聚簇索引，没有就选择唯一的非空索引代替，聚集的数据优点如下：

- 可以将数据保存在一起
- 数据访问更快。
- 使用覆盖索引扫描的查询可以直接使用页节点中的主键值

缺点如下：
- 其最大限度的提高了I/O密集型应用的性能，如果全部放在内存中，则访问顺序没有那么重要。聚簇索引没有优势了
- 插入速度严重依赖于插入顺序。
- 跟新聚簇索引列的代价很高。
- 基于聚簇索引的表在插入新行，或者主键被更新导致需要移动的时候，可能面临“页分裂”。
- 其可能导致全表扫描变慢，尤其是行稀疏或者页存储不连续的时候。
- 二级索引较大。包含引用行的主键列
- 二级索引访问需要两次索引查找，而不是一次--二级索引中保存的不是只想行的物理位置的指针，而是行的主键值。

对于二级索引不同搜索引擎的索引结构和分布如下:

![搜索分布](https://upload-images.jianshu.io/upload_images/10154499-5772dddedb909374.PNG?imageMogr2/auto-orient/strip/imageView2/2/format/webp)

MyISAM基本是完全独立的两个索引，InnoDB是先到1再到2。

InnoDB的内部主键分布如下:

![](https://wangpengcheng.github.io/img/2020-02-11-10-49-58.png)

聚簇索引的每一个叶子节点都包含了主键、事务ID、用于事务和MVCC的回滚指针以及所有的剩余列。二级索引中存储的是key值而不是行数据指针。

当使用自增的主键作为索引时，因为其顺序性，能够保证记录填充时的有序，记录按照是顺序进行填充，不必考虑随机时的空白问题。

如果时UUID乱序执行，会有如下问题：

- 先要进行磁盘数据的加载，造成大量的随机I/O
- 写入时乱序的，InnoDB分页和数据移动会频繁很多。
- 频繁的分页，页会变得稀疏并被不规则地填充，所以最终数据会有碎片

使用InnoDB时应该尽可能地按照主键顺序插入数据，并尽可能地使用单调增加的聚簇键的值来插入新行

**注意:** 顺序主键在高并发时，顺序插入会造成明显的争用。主键的上界会成为热点。会造成频繁的锁机制。

### 5.3.6 覆盖索引

_参考链接:_ [mysql覆盖索引详解](https://www.cnblogs.com/pyng/p/9599977.html);[mysql覆盖索引与回表](https://www.jianshu.com/p/8991cbca3854)

可以在索引中包含已有的数据，这样就可以直接进行数据的读取，而不用使用行。这个我们就称其为"覆盖索引"

优点如下：
- 索引条目远小于数据行的大小，只需要读取索引，极大减少数据访问量。减轻缓存和I/O操作
- 索引顺序存储，对于I/O密集型的查询范围会比随机从磁盘读取每一行数据的I/O少得多。
- 对于InnoDB的二级索引保存行的主键值，如果其能够覆盖查询，则可以避免对主键索引的二次查询。

索引覆盖必须要存储索引列的值，而哈希索引、空间索引和全文索引等都不存储索引列的值，MySQL只能使用B-Tree索引做覆盖索引。

对于模糊查询或者LIKE匹配，可能存在索引无法覆盖的情况，这时需要对查询语句进行优化，增加索引列，来实现覆盖索引；例如：

```sql
# 使用store_id和film_id做覆盖索引
EXPLAIN SELECT store)id,film_id FROM sakila.inventory \G
# 模糊查询无法进行索引覆盖;不能根据索引结果建立覆盖索引，MySQL不能再索引中执行LIKE操作
EXPLAIN　SELECT　*　FROM products WHERE actor='SEAN CARREY' AND title like '%APOLLO%' \G
# 将数据列列出，巧妙的设查询索引，可以先将索引覆盖至三个数据列，然后进行查询，避免回表
EXPLAIN SELECT * FROM products JOIN (
    SELECT prod_id
    FROM products
    WHERE actor='SEAN CARREY' AND title LIKE '%APOLLO%'
) AS t1 ON (t1.prod_id=products.prod_id) \G
```

这种方式称为关联延迟

### 5.3.7 使用索引扫描来做排序

生成有序结果的方式：
1. 排序操作:
2. 索引顺序扫描:(explain为index)

因为在做索引扫描时，没有实现覆盖索引，对每一次查询到的索引都需要进行回表查询对应的行的其它数据。**因此按照索引顺序读取数据的速度通常比顺序地全表扫描要慢，尤其在与I/O密集型的工作负载时**

只有当索引的列顺序和ORDER BY句子的顺序完全一致，并且所有列的排序方向(倒序或正序)都一样时，MySQL才能够使用索引对结果进行排序。

关联查询多张表时，只有ORDER BY 子句引用的字段全部为第一个表时，才能使用索引做排序。ORDER BY子句和查找型查询的限制是一样的--满足索引的左前缀的要求。

因为索引的本质还是一个B-Tree因此，当查询条件的第一个是一个常量时，可以使用索引中的第二列索引作为排序条件。因为这样B-Tree只查询一次。例如：

```sql
EXPLAIN SELECT rental_id,staff_id FROM sakila.rental WHERE rental_date='2005-05-25' ORDER BY inventory_id,customer_id \G 
```

### 5.3.5 压缩(前缀压缩)索引

MyISAM中使用前缀压缩的方式，进行索引压缩，当第一个值时'perform'第二个是'performance' 则其压缩后存储的是类似'7,ance'这样的形势。也可以使用这样的前缀压缩的方式。

这样会有额外的操作性能的损失。但是对于排序鄙视很友好。在块中查找某一行的操作平均都需要扫描半个索引块。适合CPU密集型应用。

### 5.3.9 冗余和重复索引

MySQL允许相同列的重复索引；优化器在优化时也需要逐个地进行考虑，这会影响性能(KEY(col)和FULLTEXT KEY(col)这种不同类型的索引，并不计算在其中)。

如果创建了索引(A,B)；再创建了索引(A);则A为冗余索引。

对于多个列的查询，可以适当的进行索引的扩展，用来提升查询的性能；例如：

```sql
# 原始查询只统计行数，为覆盖索引，性能较高
SELECT COUNT(*) FROM userinfo WHERE state_id=5;
# 查询了其他列，存在了回表查询；性能下降
SELECT state_id,city,address FROM userinfo WHERE state_id=5;
# 扩展索引后，执行查询语句如下：
ALRER TABLE userinfo DROP KEY state_id, ADD KEY state_id_2 (state_id,city,address);
```

注意：索引的增多，会造成INSERT、UPDATE和DELETE速度的降低。可以使用pt-upgrade 等工具，来进行检查计划中的索引变更。

### 53.10 未使用的索引

使用Percona Server或者MariaDB中打开userstate服务器变量，和pt-index-usage等进行索引的查询和包好，删除未使用的索引。

### 5.3.11 索引和锁

索引可以减少行的访问，简洁减少锁的使用。**但是有的时候，会将关键的索引数据行锁定，造成其它事务(事务是指是程序中一系列严密的逻辑操作，而且所有操作必须全部成功完成，否则在每个操作中所作的所有更改都会被撤消。)无法进行，需要注意**

**InnoDB**在二级索引上使用共享(读)锁，但访问主键索引需要排他(写)锁。消除了使用覆盖索引的可能性，使得SELECT FOR UODATE 比LOCK IN SHARE MODE 或非锁定查询要慢很多。

## 5.4 索引案例学习

一个在线网站的用户信息列表有很多列：国家、地区、城市、性别、眼睛颜色等。

### 5.4.1 支持多种过滤条件

使用sex和country作为前缀;一般在选择性低的列上创建索引。使用sex可以使用`AND SEX IN('m','f')`来让MySQL选择该索引。这样不会过滤任何行。

为了支持多种过滤条件，选择索引：
- (SEX,country,age):年龄的覆盖索引
- (sex,country,regin,city,age):尽量多的搜索条件

### 5.4.2 避免多个范围条件

假设使用如下的查询语句:

```sql
WHERE eye_color IN('brown','blue','hazel')
AND hair_color IN('black','red','blonde','brown')
AND sex IN('M','F')
AND last_online > DATE_SUB(NOW(),INTERVAL 7 DAY)
AND age BETWEEN 18 AND 25
```

将多个列设置为索引将其转换为条件的索引。

### 5.4.3 优化排序

## 5.5 维护索引和表

使CHECK TABLE 和REPAIR TABLE来进行损坏表的检测和修复。

表的数据存储碎片类型
1. 行碎片:数据行被存储为多个地方的多个片段中。即使时索引也会导致性能下降
2. 行间碎片:行在磁盘上的非顺序性。其对全表扫描和聚簇索引扫描影响较大。
3. 剩余空间碎片:数据页中有大量的空余空间。导致服务器读取大量不需要的数据，造成浪费。

MyISAM表，三类碎片化都可能发生，但是InnoDB不会出现短小的行碎片；InnoDB会移动短小的行并重写到一个片段中

# 第 6 章 查询性能优化

## 6.1 为什么查询速度会慢

原因很多

## 6.2 慢查询基础：优化数据访问

查询的最基本的原因是 访问的数据太多。可以通过减少访问的数据两进行优化；具体步骤如下:

1. 确认应用程序是否检索大量超过需要是数据--访问行列太多
    - 查询不需要的记录:会返回所有值再筛选
    - 总是取出全部列
    - 重复查询相同的数据：例如再用户评论中--不断地重复执行相同的查询
    - 多表关联时返回全部列表:例如

```sql
# 返回三个表的全部数据列
SELECT * FROM sakila.actor 
INNER JOIN sakila.film_actor USING(actor_id)
INNER JOIN sakila.film USING(film_id)
WHERE sakila.film.title= 'ACademy Dinosaur';
# 改进后的写法如下

SELECT sakila.actor.* FROM sakila.actor
```

2. MySQL服务器层是否在分析大量超过需要的数据行


衡量查询开销的三个指标如下:
- 响应时间:服务时间(查询时间)+排队时间(资源中断的时间)
- 扫描的行数:
- 返回的行数
- 访问类型

一般MySQL能够使用如下三种方式引用WHERE条件，从好到坏依次为:
- 使用WHERE条件来过滤不匹配的记录再，存储引擎层完成
- 使用覆盖扫描(Using index),服务器层完成
- 从数据表中返回数据(Using Where),MySQL服务器层完成，先读出数据然后进行过滤。

一般的优化方法:
- 使用覆盖扫描
- 改变库表结构
- 重写复杂查询

## 6.3 重构查询的方式

- 使用多个简单查询代替一个复杂查询
- 切分查询:将大查询切分成小查询，例如再进行DELETE时，使用多个小查询，避免一次锁住过多的数据、占满整个事务日志。例如

```sql
# 原始查询
DELETE FROM messages WHERE created < DATE_SUB(NOW(),INTERVAL 3 MONTH)
# 循环优化为:
rows_affected=0
do {
    rows_affected=do_query(
        "DELETE FROM messages WHERE created <DATE_SUB(NOW(),INTERVAL 3 MONTH)"
    )
}while rows_affected > 0

```

- 分解关联查询:对每一个表进行一次单表查询，然后将结果再应用程序中进行关联。例如：

```sql
# 原始语句
SELECT * FROM tag
JOIN tag_post ON tag_post.tag_id=tag.id
JOIN post ON tag_post.tag_id=post.id
WHERE tag.tag='mysql';
# 分解成如下查询语句
SELECT * FROM tag WHERE tag='mysql';
SELECT * FROM tag_post WHERE tag_id=1234;
SELECT * FROM post WHERE post.id IN (123,456,9098,8904);
```

优点如下：
1. 让缓存的效率更高。已经缓存的内容，就不必再继续读取，可以直接跳过
2. 查询分解后，执行单个查询可以减少锁的竞争
3. 在应用层做关联，可以更容易对数据库进行拆分，共容易做到高性能和可扩展
4. 查询本身效率也可能会有所提升。使用IN()代替关联查询，可以让MySQL按照ID顺序进行查询，可能比随机的关联要更加高效，
5. 可以减少冗余记录的查询。避免数据库层的重复数据访问。
6. 在应用层中实现了哈希关联，而不是使用MySQL的陶杰循环关联。某些场景哈希关联的效率要高很多。

## 6.4 查询执行的基础

MySQL的一般查询流程如下：

![](https://wangpengcheng.github.io/img/2020-02-11-22-49-58.png)

### 6.4.1 MySQL 客户端/服务器通信协议

MySQL客户但和服务器中间的通信协议时"半双工"的。因此当查询语句较长时参数"max_allowed_packet"很重要。

都是从数据库中的缓存获取数据。可以通过对MySQL的接口，进行设置取消缓冲。

MySQL的查询状态(SHOW FULL PROCESSLIST进行查询)
1. Sleep:线程正在等待客户端发送新的请求
2. Query:线程正在执行查询或者正在将结果发送给客户端
3. Locked:该线程正在等待表锁，InnoDB的行锁，并不会体现在线程状态中。
4. Analyzing and statistics :线程正在收集存储引擎的统计信息，并生成查询的执行计划
5. Cpoy to tmp table [on disk]:线程正在执行查询，并将其结果集都复制到一个临时表中，一般是GROUP BY操作或者UNION 操作
6. Sorting result:线程正在对结果集进行排序
7. Sending data:线程可能在多个状态之间传递数据，或者在生成结果集，或者在想客户端返回数据

### 6.4.2 查询缓存

MySQL的缓存查询时一个对大小写敏感的哈希查找实现的。即使有一个字节不同，也不会匹配缓存结果。

### 6.4.3 查询优化处理

- 语法解析器和预处理:通过关键字将SQL语句进行解析，并生成一颗对应的"解析树"，再使用语法规则验证和解析查询。检查语句是否合法
- 查询优化器:尝试预测执行的成本，并选择其中成本最小的一个。最小单位时随机读取一个4K数据页的成本。可以通过查询当前会话的'Last_query_cost'计算查询成本;例如:

```sql
SELECT SQL_NO_CACHE COUNT(*) FROM sakila.film_actor;
SHOW STATUS LIKE 'last_query_cost'
```

输出：1040.599000;表示需要做1040个数据页的随机查找才能完成上述操作。

由于统计信息不准确、基于成本模型而非时间最优、不考虑其他的并发执行、不会考虑模型外的操作成本、无法估算所有成本等原因；其选择错误。

主要优化策略分为两种：
- 静态优化:直接对解析树进行分析,与查询值无关是编译时优化
- 动态优化:和查询的上下文等因素有关；时运行时优化

MYSQL能处理的优化类型有:
- 重新定义关联表的顺序
- 将外连接转化为内连接
- 使用等价变换规则
- 优化COUNT()、MIN()和MAX()(最大最小是索引的最后一条)
- 预估并转化为常数表达式:优先进行条件筛选再进行内联
- 覆盖索引扫描:
- 子查询优化:有时将子查询转换为一种效率更高的形式，从而减少多个查询对数据进行访问。
- 提前终止查询:查询为空提前终止
- 等值传播:两个列最大值通过等式关联，MySQL能够把其中一个列的WHERE条件传递到另外一个列上，两个表使用同一个条件查询
- 列表IN()的比较:先进行排序，二分查找确定列表中的值是否满足条件。(O(lg(n))复杂度)，或者转化为多个OR语句(O(n)复杂度)

#### 6.4.3.1 MySQL如何执行关联查询

任何一次查询都可能是关联;MySQL使用循环嵌套关联，先查一个，获取内容；再按行进行关键迭代查询；直到结束。

![嵌套样式](https://wangpengcheng.github.io/img/2020-02-11-23-49-58.png)

#### 6.4.3.2 执行计划

MySQL使用指令树，而不是生成查询字节码来执行查询。

![](https://wangpengcheng.github.io/img/2020-02-11-23-44-58.png)

#### 6.4.3.3 关联查询优化器

最重要部分，决定了多个表关联时的顺序。可以使用STRAIGHT_JOIN关键字重写查询，让优化器按照最优的关联顺序执行。

当不同关联的表数，超过optimizer_search_depth时，使用贪婪搜索模式，查找最优的关联顺序。

#### 6.4.3.4 排序优化

排序成本相当高，应该尽可能避免排序或者避免大量数据进行排序。

MySQL使用**单次传输排序**；先读取查询所需要的所有列;再根据给定列进行排序。相对于两次传输排序(先找数据再排序，第二次读取对应行数据)；减少了I/O;但是返回的列会非常多，占用额外占用大量的空间。

查询所需列的总长度不超过参数max_length_for_sort_data时，使用单次传输。

当使用LIMIT时，先进行结果筛选再排序。

### 6.4.4 查询执行引擎

MySQL重复执行计划中的各个操作，直到完成所有的数据查询。

### 6.4.5 返回结果给客户端

一定会返回结果，即使为空。返回过程是一个增量、逐步返回的过程。一旦服务器处理完最后一个关联表。开始生成第一条结果时，MySQL就可以开始向客户端逐步返回结果集了。

结果集中的每一行都会以一个满足MySQL客户端/服务器通信协议的封包发送，再通过TCP协议进行传输。

## 6.5 MySQL查询优化器的局限性

### 6.5.1 关联子查询

关联子查询中的WHERE条件中的IN()子查询语句；一般会被改写成为，外部嵌套查询；建议使用内联优化。进行改进

建议不要使用主观猜测，应该通过测试来验证猜想

### 6.5.2 UNION的限制

MySQL无法将限制条件从外层"下推"到内层。使得限制条件无法应用到内层查询的优化上面。

使用UNION时，应该尽量先对部分结果进行筛选，最后再进行结果集合的合并。

例如：

```sql
# 这条查询，会将两个集合查找20条记录进行，添加到临时表中；再筛选20条
(SELECT first_name,last_name FROM sakila.actor ORDER BY last_name) UNION ALL (SELECT first_name,last_name FROM sakila.customer ORDER BY last_name) LIMIT 20;
# 应当修改如下:

(SELECT first_name,last_name FROM sakila.actor ORDER BY last_name LIMIT 20) UNION ALL (SELECT first_name,last_name FROM sakila.customer ORDER BY last_name LIMIT 20) LIMIT 20;
```

### 6.5.3 索引合并优化

使用多个索引合并和交叉过滤的方式来定位需要查找的行。

### 6.5.4 等值传递

### 6.5.5 并行执行

MySQL无法利用多核特性执行并行查询。

### 6.5.6 哈希关联

MySQL所有的关联都是嵌套循环关联。但是可以通过建立哈希索引实现哈希关联

### 6.5.7 松散索引扫描

_参考连接：_ [mysql 松散索引与紧凑索引扫描（引入数据结构）](https://www.cnblogs.com/novice-dxx/p/11955920.html)

EXPLAIN中显示"Using index for group-by",表示这里将使用松散索引扫描.其与全表扫描相对(紧凑索引)

`select * from xxx where B = xxx group by A;` 添加 group by 字段后，会先根据 A 索引分组后，会在每个 A 的范围内使用索引进行快速查询定位所需要的 B 列，这就叫做松散索引扫描，比新建一个索引的效率会慢 A 的 distinct 倍，但省去了新索引的消耗。

### 6.5.8 最大值和最小值优化

因此索引的元婴，对于最大和最小值；可以使用LIMIT来进行查找。例如:

```sql
# 原始语句需要，扫描整张表
SELECT MIN(actor_id) FROM sakila.actor WHERE first_name='PENLOPE';
# 显式使用索引，降低MySQL扫描数量
SELECT actor_id FROM sakila.actor USE INDEX(PRIMARY)
WHERE firts_name='PENLOPE' LIMIT 1;
```

### 6.5.9 再同一个表上查询和更新

MySQL不允许对同一张表同时进行查询和更新。但是可以通过使用临时表，来绕过上面的限制；例如:

```sql
# 下面的语句会出现错误
UPDATE tbl AS outer_tbl
    SET cnt=(
        SELECT count(*) FROM tbl AS inner_tbl
        WHERE inner_tbl.type=outer_tbl.type
    );
# 生成临时表并内联
UPDATE tbl
    INNER JOIN(
        SELECT type,count(*) AS cnt
        FROM tbl
        GROUP BY type
    ) AS der USING(type)
SET tbl.cnt=der.cnt;
```

## 6.6 查询优化器的提示

_参考连接:_ [mysql查询优化之三：查询优化器提示（hint）](https://www.cnblogs.com/jpfss/p/11738122.html)


## 6.7 优化特定类型的查询

### 6.7.1 优化COUNT()查询

COUNT(*) 忽略所有列，直接统计所有行数。当没有条件时；可以通过MySQL的存储引擎直接获取对应的值。

COUNT一般都需要扫描大量的行，即大量的数据访问。比较难优化，**善用补集进行替代，减少扫描数量**

### 6.7.2 优化关联查询

- 确保ON或者USING子句中的列上有索引；
- 确保任何的GROUP BY 和ORDER BY中的表达式只涉及到一个表中的列，这样MySQL才有可能使用索引来优化这个过程。
- 升级MySQL时需要注意关联语法的变化。

### 6.7.3 优化子查询

尽量使用关联查询代替

### 6.7.4 优化GROUP BY 和DISTINCT

### 6.7.5 优化LIMIT分页

尽量使用偏移+数量，尽可能的使用索引覆盖扫描，而不是查询所有的列。然后根据需要做一次关联操作再返回需要的列。例如:

```sql
SELECT film_id,description FROM sakila.film ORDER BY title LIMIT 50,5;
# 改写成下面的样子,使用索引再进行筛选
SELECT film.film_id,film.description
FROM saklia.film INNER JOIN(
    SELECT film_id FROM sakila.film
    ORDER BY title LIMIT 50,5
) AS lim USING(film_id);

```

### 6.7.6 优化SQL_CALC_FOUND_ROWS

这个使用再LIMIT之后会额昂MySQL扫描满足条件的所有行，再进行筛选。而不是数量足够之后就停止了。

### 6.7.7 优化UNION 查询

MySQL总是通过创建临时表的方式来执行UNION查询；没有需要消除重复行是，一定使用UNION ALL;这样MySQL就不会给临时表加上DISTINCT选项(对临时表做唯一性检查)

### 6.7.9 使用用户自定义变量

例如:
```sql
SET @last_week:=CURRENT_DATE-INTERVAL 1 WEEK;
SELECT ... WHERE col <= @last_week;
```

![](https://wangpengcheng.github.io/img/2020-02-12-23-49-58.png)
![](https://wangpengcheng.github.io/img/2020-02-12-22-49-58.png)


优化都是:不做，少做，快速地做

# 第 7 章 MySQL高级特性



# MySQL 常用函数

## 字符串处理函数

![](https://wangpengcheng.github.io/img/2020-03-07-21-15-35.png)
![](https://wangpengcheng.github.io/img/2020-03-07-21-17-35.png)

## 日期处理函数

![](https://wangpengcheng.github.io/img/2020-03-07-21-18-35.png)

## 数值处理函数

![](https://wangpengcheng.github.io/img/2020-03-07-21-20-35.png)

## 聚集函数(运行在组上的函数)

![](https://wangpengcheng.github.io/img/2020-03-0721-21-42.png)

聚集函数除去重复值时，使用`DISTINCT`

## mysql的相关执行顺序问题

_参考连接：_ [连接（join）group by、order by、where的执行顺序](https://blog.csdn.net/mine_song/article/details/70185744)

```sql

FROM
ON
JOIN
WHERE
GROUP BY
WITH CUBE 或 WITH ROLLUP
HAVING
SELECT
DISTINCT
ORDER BY
TOP 
```

## GROUP BY 使用注意事项

![](https://wangpengcheng.github.io/img/2020-03-07-21-26-34.png)

## SELECT 子句顺序

![](https://wangpengcheng.github.io/img/2020-03-07-21-31-34.png)
![](https://wangpengcheng.github.io/img/2020-03-07-21-32-34.png)

## 