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

### [176.第二高的薪水](https://leetcode.cn/problems/second-highest-salary/description/)

- 提交解答

```sql
# Write your MySQL query statement below
# notice: 这里必须使用SELECT 子查询才能让空行显示null
SELECT (
    SELECT DISTINCT salary FROM Employee ORDER BY salary DESC LIMIT 1,1
) AS SecondHighestSalary;


# 使用ISFULL 进行二次替换
SELECT (
    IFNULL((SELECT DISTINCT salary FROM Employee ORDER BY salary DESC LIMIT 1,1),NULL)
) AS SecondHighestSalary;

```

- 优质解答

```sql
# 使用offset加快了速度
SELECT (
    SELECT DISTINCT Salary AS 'SecondHighestSalary' FROM Employee ORDER BY Salary DESC LIMIT 1 OFFSET 1
) AS 'SecondHighestSalary';  
```

- 官方题解：

```sql
# 思路相同
SELECT
    IFNULL(
      (SELECT DISTINCT Salary
       FROM Employee
       ORDER BY Salary DESC
        LIMIT 1 OFFSET 1),
    NULL) AS SecondHighestSalary
```

### [177.第N高的薪水](https://leetcode.cn/problems/nth-highest-salary/description/)

- 参考链接：
    - [MySQL之自定义函数](https://zhuanlan.zhihu.com/p/128744140)
    - [MySQL全面瓦解18：自定义函数](https://www.cnblogs.com/wzh2010/p/13843095.html)

- 个人解答：

```sql
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
    declare a int;
   SET a = N-1;
  RETURN (
    # Write your MySQL query statement below.
   SELECT IFNULL((SELECT DISTINCT salary  FROM Employee ORDER BY salary DESC LIMIT 1 OFFSET a),NULL)
   
  );
END
```

- 优质解答：

```sql
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
Declare M INT;
set M=N-1;
  RETURN (
      # Write your MySQL query statement below.
select distinct Salary
      from Employee
      order by Salary desc
      limit M,1
  );
END
```

- 官方题解：

```sql
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
DECLARE M INT; 
    SET M = N-1; 
  RETURN (
      SELECT DISTINCT salary
      FROM Employee
      ORDER BY salary DESC
      LIMIT M, 1
  );
END

# 作者：力扣官方题解
# 链接：https://leetcode.cn/problems/nth-highest-salary/
# 来源：力扣（LeetCode）
# 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```