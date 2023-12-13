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

# SQL练习笔记
> 2023-11-07 14:35:58

SQL 练习笔记，包含leetcode等SQL题目冲刺笔记

## 参考链接
- [leetcode-shell](https://leetcode.cn/problemset/database/)


## leetcode 题目


### [175. 组合两个表](https://leetcode.cn/problems/combine-two-tables/description/)

- 提交解答：

```sql
# 直接正常查询即可 
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
on Person.PersonId = Address.PersonId;
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

### [180. 连续出现的数字](https://leetcode.cn/problems/consecutive-numbers/description/)

- 参考链接：
    - [mysql in 多个字段的用法](https://blog.csdn.net/Guanjs2016/article/details/80237490)

- 提交解答：

```sql

# 使用in语句,保证id进行同匹配即可
SELECT DISTINCT(num) AS ConsecutiveNums   FROM Logs
WHERE (id+1, num) IN (SELECT id,num FROM Logs)
AND (id+2, num) IN (SELECT id,num FROM Logs)
```

- 优质解答：

```sql
# 
# Write your MySQL query statement below
# 查询num 别名
select distinct Num as ConsecutiveNums
from (
  select Num, 
    # 筛选CNT
    case 
      when @prev = Num then @count := @count + 1
      when (@prev := Num) is not null then @count := 1
    end as CNT
  from Logs, (select @prev := null,@count := null) as t
) as temp
where temp.CNT >= 3
```

- 官方题解：

```sql

## 直接使用别名进行简单计算即可
SELECT *
FROM
    Logs l1,
    Logs l2,
    Logs l3
WHERE
    l1.Id = l2.Id - 1
    AND l2.Id = l3.Id - 1
    AND l1.Num = l2.Num
    AND l2.Num = l3.Num
;

# 作者：LeetCode
# 链接：https://leetcode.cn/problems/consecutive-numbers/
# 来源：力扣（LeetCode）
# 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```

### [181. 超过经理收入的员工](https://leetcode.cn/problems/employees-earning-more-than-their-managers/description/)

- 提交解答：

```sql
# Write your MySQL query statement below
# 直接使用子查询别名进行筛选
SELECT name as Employee FROM Employee JOIN (
 SELECT id, salary FROM Employee
) AS new_table ON Employee.managerId = new_table.id WHERE  Employee.salary > new_table.salary;
```

- 优质解答：

```sql
# Write your MySQL query statement below
# 使用inner join 进行快速查询
select a.name as Employee
from  Employee as a  inner join Employee as b
on a.managerId=b.id and a.salary>b.salary;
```

- 官方题解：

```sql

# 直接别名进行联表查询
SELECT
    *
FROM
    Employee AS a,
    Employee AS b
WHERE
    a.ManagerId = b.Id
        AND a.Salary > b.Salary;

#作者：LeetCode
#链接：https://leetcode.cn/problems/employees-earning-more-than-their-managers/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

# 使用join 进行查询
SELECT
     a.NAME AS Employee
FROM Employee AS a JOIN Employee AS b
     ON a.ManagerId = b.Id
     AND a.Salary > b.Salary
;

#作者：LeetCode
#链接：https://leetcode.cn/problems/employees-earning-more-than-their-managers/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```

### [182. 查找重复的电子邮箱](https://leetcode.cn/problems/duplicate-emails/description/)

- 提交解答

```sql
# Write your MySQL query statement below

SELECT email AS Email FROM Person GROUP BY email HAVING COUNT(*) > 1;
```

- 优质解答：

```sql
# Write your MySQL query statement below
SELECT DISTINCT Email FROM PERSON GROUP BY EMAIL HAVING COUNT(EMAIL) > 1;
```

- 官方题解：

```sql
## 1. 直接使用group by 和临时表
select Email from
(
  select Email, count(Email) as num
  from Person
  group by Email
) as statistic
where num > 1;

#作者：LeetCode
#链接：https://leetcode.cn/problems/duplicate-emails/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

## 2. group by 和having

select Email from Person group by Email having count(Email) > 1;

```

### [183. 从不订购的客户](https://leetcode.cn/problems/customers-who-never-order/description/)

- 提交解答

```sql
# Write your MySQL query statement below

SELECT name AS Customers FROM Customers WHERE Id NOT IN ( 
    SELECT DISTINCT(customerId) AS customerId FROM Orders
); 
```

- 优质解答

```sql
# Write your MySQL query statement below
# 思路相同，没有使用DISTINCT 节省了时间
SELECT
    name as Customers
FROM
    Customers
WHERE
    id NOT IN (SELECT customerId as id FROM Orders)

# 使用NOT EXISTS 函数节省了时间
SELECT Name as Customers FROM Customers as c1
WHERE NOT EXISTS(
    SELECT Id FROM Orders as c2
    WHERE c2.CustomerId = c1.Id
)
```

- 官方题解

```sql

# 使用 not in 进行排除
select customers.name as 'Customers'
from customers
where customers.id not in
(
    select customerid from orders
);

#作者：力扣官方题解
#链接：https://leetcode.cn/problems/customers-who-never-order/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

# 使用左连接
SELECT Customers.name AS Customers 
FROM Customers
LEFT JOIN Orders ON Customers.Id = Orders.CustomerId WHERE customerId is NULL;

#作者：力扣官方题解
#链接：https://leetcode.cn/problems/customers-who-never-order/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

```

### [184. 部门工资最高的员工](https://leetcode.cn/problems/department-highest-salary/description/)

- 提交解答:

```sql
# Write your MySQL query statement below
# 使用子查询，找出每个部门的最大值，然后匹配即可

SELECT b.department_name AS Department, Employee.name AS Employee, Employee.Salary AS Salary
FROM Employee, (
		SELECT MAX(salary) AS max_salary, Department.id AS department_id, Department.name AS department_name
		FROM Employee
			LEFT JOIN Department ON Department.id = Employee.departmentId
		GROUP BY departmentId
	) b
WHERE Employee.departmentId = b.department_id
	AND Employee.Salary = b.max_salary;
```

- 优质解答1：

```sql
# Write your MySQL query statement below
# 使用rank 统计最大值
SELECT Department, name AS Employee, salary AS Salary 
FROM
(
    SELECT a.*, b.name AS Department, 
           RANK() OVER(PARTITION BY a.departmentId ORDER BY salary DESC) AS rk
    FROM Employee a LEFT JOIN Department b ON a.departmentId =b.id
) t
WHERE rk = 1
```

- 优质解答2：

```sql
# Write your MySQL query statement below
# 使用连接后，直接使用IN 查询
SELECT Department.Name AS 'Department', Employee.Name AS 'Employee', Salary
FROM Employee
JOIN Department 
ON Employee.DepartmentId = Department.Id
WHERE (Employee.DepartmentId, Salary) 
IN (SELECT DepartmentId, MAX(Salary)
   FROM Employee
   GROUP BY DepartmentId);
```

- 官方题解：

```sql
SELECT
    Department.name AS 'Department',
    Employee.name AS '

Employee',
    Salary
FROM
    Employee
        JOIN
    Department ON Employee.DepartmentId = Department.Id
WHERE
    (Employee.DepartmentId , Salary) IN
    (   SELECT
            DepartmentId, MAX(Salary)
        FROM
            Employee
        GROUP BY DepartmentId
    )
;

#作者：力扣官方题解
#链接：https://leetcode.cn/problems/department-highest-salary/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

```

### [185. 部门工资前三高的所有员工](https://leetcode.cn/problems/department-top-three-salaries/description/)

- 提交解答：

```sql

# 子查询排序后，使用 ROW_NUMBER 窗口函数统计行数
SELECT  Department.name AS Department, Employee.name AS Employee , Employee.salary AS Salary  
FROM Employee LEFT JOIN (
    SELECT departmentId, MIN(salary) as min_salary  FROM (
    SELECT departmentId,salary,
        ROW_NUMBER()  OVER(partition by departmentId  order by salary  desc) as num
    FROM 
    (
        select departmentId,salary FROM Employee GROUP BY departmentId,salary ) b
    ) c
    WHERE num <= 3 GROUP BY departmentId
) d ON  Employee.departmentId = d.departmentId
LEFT JOIN Department  ON Employee.departmentId = Department.id 
WHERE Employee.salary >= d.min_salary ;
```

- 优质解答1：

```sql

## 直接左连接相同表，找到比自己小的表，最终统计小的数目即可
SELECT  d.name Department, e.name Employee, e.salary Salary
FROM Employee e
LEFT JOIN Employee ee
ON e.departmentId = ee.departmentId AND e.salary < ee.salary
JOIN Department d
ON d.id = e.departmentId
GROUP BY e.id
HAVING count(distinct(ee.salary)) < 3;
```

- 优质解答2：

```sql
# 直接使用 DENSE_RANK 窗口函数，统计去重后的行数
SELECT Department, name AS Employee, salary AS Salary 
FROM
(
    SELECT a.*, b.name AS Department, 
           DENSE_RANK() OVER(PARTITION BY a.departmentId ORDER BY salary DESC) AS rk
    FROM Employee a LEFT JOIN Department b ON a.departmentId =b.id
) t
WHERE rk <= 3
```

- 官方题解：

```sql
SELECT
    d.Name AS 'Department', e1.Name AS 'Employee', e1.Salary
FROM
    Employee e1
        JOIN
    Department d ON e1.DepartmentId = d.Id
WHERE
    3 > (SELECT
            COUNT(DISTINCT e2.Salary)
        FROM
            Employee e2
        WHERE
            e2.Salary > e1.Salary
                AND e1.DepartmentId = e2.DepartmentId
        )
;

#作者：LeetCode
#链接：https://leetcode.cn/problems/department-top-three-salaries/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```

### [196. 删除重复的电子邮箱](https://leetcode.cn/problems/delete-duplicate-emails/description/)

- 提交解答

```sql
# Write your MySQL query statement below
# 使用子查询找出最小的id即可
DELETE FROM  Person WHERE id NOT IN (
    SELECT a.min_id FROM
        (select email, MIN(id) AS min_id FROM Person GROUP BY email) a
);

```

- 优质解答

```sql
delete from Person
where id in(
    select id from(
        select *,row_number() over(partition by email order by id ) as row_num
        from Person
    ) a
    where row_num >1)

```

- 官方题解

```sql
# 使用where语句更加简洁
DELETE p1 FROM
    Person p1,
    Person p2
WHERE
    p1.Email = p2.Email AND p1.Id > p2.Id

#作者：力扣官方题解
#链接：https://leetcode.cn/problems/delete-duplicate-emails/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

```

### [511. 游戏玩法分析 I](https://leetcode.cn/problems/game-play-analysis-i/description/)

- 提交解答：

```sql

SELECT player_id, MIN(event_date) AS first_login  FROM Activity GROUP BY player_id; 
```

- 优质解答：

```sql
elect a1.player_id, min(a1.event_date) as first_login
from Activity as a1
group by a1.player_id;
```

- 官方题解：

```sql
# 使用窗口函数进行解决
SELECT DISTINCT
  A.player_id,
  FIRST_VALUE(A.event_date) OVER (
    PARTITION BY
      A.player_id
    ORDER BY
      A.event_date
  ) AS first_login
FROM
  Activity A;

#作者：力扣官方题解
#链接：https://leetcode.cn/problems/game-play-analysis-i/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```

### [550. 游戏玩法分析 IV](https://leetcode.cn/problems/game-play-analysis-iv/description/)

- 提交解答：

```sql
# Write your MySQL query statement below
SELECT  IFNULL(
    ROUND(
        COUNT(a.player_id) / c.all_user, 
        2
    ), 
    0
    ) AS fraction FROM 
Activity a,
# 首次登陆时间
(SELECT player_id, MIN(event_date) AS frist_date FROM Activity GROUP BY player_id) b,
(SELECT COUNT(DISTINCT(player_id))  AS all_user FROM  Activity)  c
WHERE a.player_id = b.player_id AND a.event_date = date_add(b.frist_date, interval 1 day);

```

- 优质解答：

```sql

# Write your MySQL query statement below

# 使用datediff直接进行判断
select round(avg(if(a.event_date is null, 0, 1)), 2) fraction
from 
    (select player_id, min(event_date) as login
    from activity
    group by player_id) p 
left join activity a 
on p.player_id=a.player_id and datediff(a.event_date, p.login)=1

```

- 优质解答2：

```sql
# 思路基本相同
SELECT 
    IFNULL(ROUND(COUNT(DISTINCT(result.player_id)) / COUNT(DISTINCT(Activity.player_id)),2),0) 
    AS fraction
FROM
(SELECT Activity.player_id as player_id
FROM 
(SELECT 
    player_id,
    DATE_ADD(MIN(event_date),INTERVAL 1 DAY) AS second_date
FROM
    Activity
GROUP BY player_id
) AS excepted,Activity
WHERE Activity.event_date = excepted.second_date and Activity.player_id = excepted.player_id
) AS result,Activity
```

- 官方题解：

```sql
select IFNULL(round(count(distinct(Result.player_id)) / count(distinct(Activity.player_id)), 2), 0) as fraction
from (
  select Activity.player_id as player_id
  from (
    select player_id, DATE_ADD(MIN(event_date), INTERVAL 1 DAY) as second_date
    from Activity
    group by player_id
  ) as Expected, Activity
  where Activity.event_date = Expected.second_date and Activity.player_id = Expected.player_id
) as Result, Activity

# 作者：力扣官方题解
# 链接：https://leetcode.cn/problems/game-play-analysis-iv/
# 来源：力扣（LeetCode）
# 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```

### [577. 员工奖金](https://leetcode.cn/problems/employee-bonus/description/)

- 提交解答：

```sql
SELECT Employee.name, Bonus.bonus FROM Employee 
LEFT JOIN  Bonus ON 
    Employee.empId = Bonus.empId 
WHERE Employee.empId NOT IN (
    SELECT empId FROM Bonus WHERE bonus >=1000
);
```


- 优质解答：

```sql
# Write your MySQL query statement below
select Employee.name, Bonus.bonus from Employee left join Bonus on Employee.empId=Bonus.empId where Bonus.bonus <1000 or bonus  is null;
```

- 官方题解：

```sql

select name, bonus
from Employee left join Bonus
on Employee.EmpId = Bonus.EmpId
where bonus is null or bonus < 1000

#作者：力扣官方题解
#链接：https://leetcode.cn/problems/employee-bonus/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```

### [262. 行程和用户](https://leetcode.cn/problems/trips-and-users/description/)

- 提交解答：

```sql

# 1. 先做子查询，查询出非禁止用户的所有订单
# 2. 子查询统计，每天订单总数/取消订单数，天数
# 3. round计算对应值

SELECT b.request_at AS Day, IFNULL(ROUND(c.completed_count /b.completed_count, 2), 0.00) AS "Cancellation Rate" FROM (
    SELECT  request_at , COUNT(*) AS completed_count FROM (
        SELECT status, request_at   FROM Trips WHERE client_id NOT IN (
            SELECT users_id FROM Users WHERE banned = "Yes"
        ) AND driver_id NOT IN (
            SELECT users_id FROM Users WHERE banned = "Yes"
        ) ) a WHERE request_at between  "2013-10-01" and "2013-10-03" GROUP BY request_at 
    ) b  
LEFT JOIN 
    (
    SELECT  request_at , COUNT(*) AS completed_count FROM (
        SELECT status, request_at   FROM Trips WHERE client_id NOT IN (
            SELECT users_id FROM Users WHERE banned = "Yes"
        ) AND driver_id NOT IN (
            SELECT users_id FROM Users WHERE banned = "Yes"
        ) ) b WHERE status  != "completed" AND  request_at between  "2013-10-01" and "2013-10-03"  GROUP BY request_at 
    ) c  
ON b.request_at = c.request_at 
```

- 优质解答1：

```sql

# 直接使用AVG函数，统计特殊状态的值
SELECT 
    request_at Day,ROUND(AVG(status != 'completed'),2) 'Cancellation Rate' 
  FROM 
    Trips 
  WHERE 
      client_id IN (SELECT users_id FROM Users WHERE banned = 'No') 
    AND 
      driver_id in (SELECT users_id FROM Users WHERE banned = 'No') 
    AND 
      request_at BETWEEN '2013-10-01' AND '2013-10-03' GROUP BY request_at;
```

- 优质解答2：

```sql
# Write your MySQL query statement below
# 使用了IF SUM 运算更快
SELECT T.request_at AS `Day`, 
ROUND(SUM(IF(T.STATUS = 'completed',0,1))/ COUNT(1),2) AS `Cancellation Rate`
FROM Trips AS T
JOIN Users AS U1 ON (T.client_id = U1.users_id AND U1.banned ='No')
JOIN Users AS U2 ON (T.driver_id = U2.users_id AND U2.banned ='No')
WHERE T.request_at BETWEEN '2013-10-01' AND '2013-10-03'
GROUP BY T.request_at;
```


- 官方题解：
无
