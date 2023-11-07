---
layout:     post
title:      SQL练习笔记
subtitle:   leetcode/nowcode SQL练习笔记
date:       2023-11-07
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - SQL
    - LeetCode
    - 编程练习
---

# shell练习笔记
> 2023-11-07 14:35:58

SQL 练习笔记，包含leetcode等SQL题目冲刺笔记

## 参考链接
- [leetcode-shell](https://leetcode.cn/problemset/database/)


## leetcode 题目


### [175. 组合两个表](https://leetcode.cn/problems/combine-two-tables/description/)


- 提交解答：
```sql
SELECT Person.FirstName as firstName, Person.LastName as lastName, Address.City as city, Address.State AS state  FROM Person LEFT JOIN  Address ON Person.PersonId = Address.PersonId;
```
- 优质解答：

```sql
# Write your MySQL query statement below
SELECT firstname, lastname, city, state FROM person LEFT JOIN address ON person.personid = address.personid
```

- 官方题解：

```sql
select FirstName, LastName, City, State
from Person left join Address
on Person.PersonId = Address.PersonId
;
```
>
> 作者：LeetCode
> 链接：https://leetcode.cn/problems/combine-two-tables/
> 来源：力扣（LeetCode）
> 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
>