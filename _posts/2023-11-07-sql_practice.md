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

### [178. 分数排名](https://leetcode.cn/problems/rank-scores/description/)

- 参考链接：
    - [mysql 计算排名，生成排行榜](https://segmentfault.com/a/1190000014386692)
    - [mysql窗口函数](https://dev.mysql.com/doc/refman/8.0/en/window-functions-usage.html)

- 提交解答：

```sql
SELECT
 a.score,
(SELECT COUNT(DISTINCT  score) FROM Scores AS b WHERE  b.score > a.score) + 1 AS 'rank'
FROM Scores AS a ORDER BY a.score DESC;
```

- 优质解答：

```sql
# select  * from  Scores id  where score order by desc;
# dense_rank()窗口函数进行排序
#对Scores这张表的S字段进行排序
#对score字段进行排序按照降序排序
#最后放到rank这个新的列中
select S.score, dense_rank() over(
    order by S.score desc
) As 'rank' from Scores S;
```

- 官方题解：

```sql
SELECT
  S.score,
  COUNT(DISTINCT T.score) AS 'rank'
FROM
  Scores S
  INNER JOIN Scores T ON S.score <= T.score
GROUP BY
  S.id,
  S.score
ORDER BY
  S.score DESC;

#作者：力扣官方题解
#链接：https://leetcode.cn/problems/rank-scores/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```

```sql
SELECT
  S.score,
  DENSE_RANK() OVER (
    ORDER BY
      S.score DESC
  ) AS 'rank'
FROM
  Scores S;

#作者：力扣官方题解
#链接：https://leetcode.cn/problems/rank-scores/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```