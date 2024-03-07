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
SELECT DISTINCT
    l1.Num AS ConsecutiveNums
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


### [570. 至少有5名直接下属的经理](https://leetcode.cn/problems/managers-with-at-least-5-direct-reports/description/)

- 提交解答：

```sql
# Write your MySQL query statement below

# 直接使用子查询
SELECT name FROM Employee WHERE id IN (
    SELECT managerId FROM Employee GROUP BY managerId HAVING COUNT(*) >= 5
);

# 使用连表查询
SELECT e1.name FROM 
Employee e1 LEFT JOIN  Employee e2 ON  e1.Id = e2.managerId GROUP BY e1.id HAVING COUNT(*) >=5; 

```

- 优质解答：

```sql
# Write your MySQL query statement below
select
    m.name as name
from 
    employee e 
join 
    employee m 
on e.managerId = m.id
group by
    e.managerId
having count(*)>=5
```

- 官方题解：

```sql
select Employee.Name as Name
from (
  select ManagerId as Id
  from Employee
  group by ManagerId
  having count(Id) >= 5
) as Manager join Employee
on Manager.Id = Employee.Id

#作者：力扣官方题解
#链接：https://leetcode.cn/problems/managers-with-at-least-5-direct-reports/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```

### [584. 寻找用户推荐人](https://leetcode.cn/problems/find-customer-referee/description/)

_参考链接：_
- [SQL - where条件里的!=会过滤值为null的数据](https://blog.csdn.net/lewky_liu/article/details/103059943)


- 提交解答：

```sql
# Write your MySQL query statement below
SELECT name FROM Customer  WHERE referee_id <> 2 OR referee_id is null;
```

- 优质解答：

```sql
# Write your MySQL query statement below
# 进行过滤筛选
select t.name from Customer t where (t.referee_id!=2 or t.referee_id is null)
# 进行对应的过滤筛选
select name from Customer where referee_id != 2 or referee_id is null;
```

- 官方题解：

```sql
SELECT name FROM customer WHERE referee_id != 2 OR referee_id IS NULL;
```


### [统计用户每天登陆时长](https://zhuanlan.zhihu.com/p/358887348)


### [577.员工奖金](https://leetcode.cn/problems/employee-bonus/description/)

- 提交解答：

```sql
# Write your MySQL query statement below
# 直接反向取数即可
SELECT Employee.name, Bonus.bonus FROM Employee LEFT JOIN  Bonus ON Employee.empId = Bonus.empId WHERE Employee.empId NOT IN (
    SELECT empId FROM Bonus WHERE bonus >=1000
);
```

- 优质解答(官方题解)：

```sql
# Write your MySQL query statement below
# 直接使用null过滤
select Employee.name, Bonus.bonus from Employee left join Bonus on Employee.empId=Bonus.empId where Bonus.bonus <1000 or bonus  is null;
```

### [585. 2016年的投资](https://leetcode.cn/problems/investments-in-2016/)

- 提交解答：

```sql
# Write your MySQL query statement below


# 保留两位小数
SELECT ROUND(SUM(tiv_2016),2 ) AS tiv_2016 FROM Insurance 
WHERE pid IN (
    # 条件1
    SELECT  a.pid FROM Insurance a, Insurance b WHERE a.pid != b.pid  AND a.tiv_2015 = b.tiv_2015) 
AND pid NOT IN (
    # 条件2
    SELECT  a.pid FROM Insurance a, Insurance b WHERE a.pid != b.pid AND a.lat = b.lat AND a.lon = b.lon
);



# 保留两位小数
SELECT ROUND(SUM(tiv_2016),2 ) AS tiv_2016 
FROM Insurance 
WHERE  tiv_2015  IN (
    # 至少一相同
    SELECT tiv_2015  FROM Insurance  GROUP BY tiv_2015 HAVING COUNT(*) >1 ) 
AND (lat, lon) IN (
    SELECT lat,lon FROM Insurance GROUP BY lat,lon HAVING COUNT(*) = 1
);
```

- 优质解答：

```sql
# 使用partition 直接统计数量
select round( sum(tiv_2016) ,2)  tiv_2016
from(
    select *,
    count(*) over(partition by tiv_2015) c1,
    count(*) over(partition by lat,lon) c2
    from Insurance
) t1
where t1.c1 > 1 and t1.c2 = 1 
```

- 官方题解：

```sql

SELECT
    SUM(insurance.TIV_2016) AS TIV_2016
FROM
    insurance
WHERE
    insurance.TIV_2015 IN
    (
      SELECT
        TIV_2015
      FROM
        insurance
      GROUP BY TIV_2015
      HAVING COUNT(*) > 1
    )
    AND CONCAT(LAT, LON) IN
    (
      SELECT
        CONCAT(LAT, LON)
      FROM
        insurance
      GROUP BY LAT , LON
      HAVING COUNT(*) = 1
    )
;

#作者：LeetCode
##来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```

### [586. 订单最多的客户](https://leetcode.cn/problems/customer-placing-the-largest-number-of-orders/description/)

- 提交解答：

```sql

SELECT customer_number FROM Orders GROUP bY customer_number ORDER BY  COUNT(*) DESC LIMIT 1;
```

- 优质解答：

```sql
# 不使用COUNT(*) 直接指明字段，提升了速度
select  customer_number
from Orders
group by customer_number
order by count(customer_number) desc
limit 1


##  使用子查询
select cn customer_number 
from (
    select count(customer_number) cc, customer_number cn
    from Orders
    group by customer_number
) T
order by T.cc desc
limit 0,1
```

- 官方题解：

```sql
SELECT
    customer_number
FROM
    orders
GROUP BY customer_number
ORDER BY COUNT(*) DESC
LIMIT 1
;

# 作者：力扣官方题解
# 链接：https://leetcode.cn/problems/customer-placing-the-largest-number-of-orders/solutions/2366301/ding-dan-zui-duo-de-ke-hu-by-leetcode-so-bywe/
# 来源：力扣（LeetCode）
# 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```

### [595. 大的国家](https://leetcode.cn/problems/big-countries/description/)


World 表：

```
+-------------+---------+
| Column Name | Type    |
+-------------+---------+
| name        | varchar |
| continent   | varchar |
| area        | int     |
| population  | int     |
| gdp         | bigint  |
+-------------+---------+
```

name 是该表的主键（具有唯一值的列）。
这张表的每一行提供：国家名称、所属大陆、面积、人口和 GDP 值。
 

如果一个国家满足下述两个条件之一，则认为该国是 大国 ：

面积至少为 300 万平方公里（即，3000000 km2），或者
人口至少为 2500 万（即 25000000）
编写解决方案找出 大国 的国家名称、人口和面积。

按 任意顺序 返回结果表。


- 提交解答：

```sql
# Write your MySQL query statement below
SELECT name, population, area FROM World WHERE population >= 25000000 OR area >= 3000000;
```

- 优质解答：

```sql 
# Write your MySQL query statement below
select name, population, area
from World
where area >= 3000000 or population >=25000000
```

- 官方题解：

```sql
SELECT
    name, population, area
FROM
    world
WHERE
    area >= 3000000 OR population >= 25000000
;

#作者：力扣官方题解
#链接：https://leetcode.cn/problems/big-countries/solutions/2366084/big-countries-by-leetcode-solution-8k13/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```

### [596. 超过5名学生的课](https://leetcode.cn/problems/classes-more-than-5-students/description/)

表: Courses

```
+-------------+---------+
| Column Name | Type    |
+-------------+---------+
| student     | varchar |
| class       | varchar |
+-------------+---------+
```

在 SQL 中，(student, class)是该表的主键列。
该表的每一行表示学生的名字和他们注册的班级。
 

查询 至少有5个学生 的所有班级。

以 任意顺序 返回结果表。

查询结果格式如下所示。

 

示例 1:

输入: 

```
Courses table:
+---------+----------+
| student | class    |
+---------+----------+
| A       | Math     |
| B       | English  |
| C       | Math     |
| D       | Biology  |
| E       | Math     |
| F       | Computer |
| G       | Math     |
| H       | Math     |
| I       | Math     |
+---------+----------+
```

输出:

``` 
+---------+ 
| class   | 
+---------+ 
| Math    | 
+---------+
```

解释: 
-数学课有6个学生，所以我们包括它。
-英语课有1名学生，所以我们不包括它。
-生物课有1名学生，所以我们不包括它。
-计算机课有1个学生，所以我们不包括它。

- 提交解答：

```sql
# Write your MySQL query statement below
# 主要思路直接使用GROUP 和having进行解决
SELECT class FROM Courses GROUP BY class HAVING COUNT() >=5;
```

- 优质解答：

```sql
# Write your MySQL query statement below
select
    class
From
    Courses
group by 
    class
having count(student)>=5;
```

- 官方题解：

```sql

## 使用子查询对每个班级的学生进行统计
## 统计完成后执行过滤
SELECT
    class
FROM
    (SELECT
        class, COUNT(student) AS num
    FROM
        courses
    GROUP BY class) AS temp_table
WHERE
    num >= 5
;

#作者：力扣官方题解
#链接：https://leetcode.cn/problems/classes-more-than-5-students/solutions/2366294/chao-guo-5ming-xue-sheng-de-ke-by-leetco-l4es/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```

### [602. 好友申请 II ：谁有最多的好友](https://leetcode.cn/problems/friend-reqz    

RequestAccepted 表：

```
+----------------+---------+
| Column Name    | Type    |
+----------------+---------+
| requester_id   | int     |
| accepter_id    | int     |
| accept_date    | date    |
+----------------+---------+
```

(requester_id, accepter_id) 是这张表的主键(具有唯一值的列的组合)。
这张表包含发送好友请求的人的 ID ，接收好友请求的人的 ID ，以及好友请求通过的日期。
 

编写解决方案，找出拥有最多的好友的人和他拥有的好友数目。

生成的测试用例保证拥有最多好友数目的只有 1 个人。

查询结果格式如下例所示。

 

示例 1：

输入：
RequestAccepted 表：

```
+--------------+-------------+-------------+
| requester_id | accepter_id | accept_date |
+--------------+-------------+-------------+
| 1            | 2           | 2016/06/03  |
| 1            | 3           | 2016/06/08  |
| 2            | 3           | 2016/06/08  |
| 3            | 4           | 2016/06/09  |
+--------------+-------------+-------------+
```

输出：

```
+----+-----+
| id | num |
+----+-----+
| 3  | 3   |
+----+-----+
```

解释：
编号为 3 的人是编号为 1 ，2 和 4 的人的好友，所以他总共有 3 个好友，比其他人都多。
 

进阶：在真实世界里，可能会有多个人拥有好友数相同且最多，你能找到所有这些人吗？


- 提交解答：

```sql
## 主要思路
# 先分别统计作为发送者和接收者的子表，再进行合并统计
# 注意这里使用 UNION ALL 进行两张表的合并
SELECT user AS id , SUM(count)  AS num   FROM 
(
    SELECT accepter_id AS user, COUNT(*) AS count FROM RequestAccepted GROUP BY accepter_id
    union 
    SELECT  requester_id AS user, COUNT(*) AS count FROM RequestAccepted GROUP BY requester_id
) c GROUP BY id ORDER BY num DESC LIMIT 0,1

```

- 优质解答：

```sql
# Write your MySQL query statement below
# 使用union 与组合group， 进行了统一去重
select id, count(*) as num from (
    select r.requester_id as id, accepter_id as sid, accept_date from requestaccepted r group by id, sid
    union 
    (   
        # 这个子查询巧妙的将列进行了转换 
        select t.accepter_id as id, requester_id as sid, accept_date from requestaccepted t group by id, sid)
) tmp group by id order by count(*) desc limit 1

# 思路相同
select id, num from
(select pid as id, sum(cnt) as num from
    (select accepter_id as pid, count(1) as cnt from request_accepted group by accepter_id
    union all
    select requester_id as pid, count(1) as cnt from request_accepted group by requester_id) t1
group by pid) t2
order by num desc
limit 1;

# 思路相同，代码更加简洁
select t1.ids as id,count(*) as num
from(
        select requester_id as ids from RequestAccepted 
        union all
        select accepter_id as ids from RequestAccepted
) as t1
group by id
order by num desc
limit 1;
```

- 官方题解：

```sql
select ids as id, cnt as num
from
(
select ids, count(*) as cnt
   from
   (
        select requester_id as ids from request_accepted
        union all
        select accepter_id from request_accepted
    ) as tbl1
   group by ids
   ) as tbl2
order by cnt desc
limit 1
;

#作者：LeetCode
#链接：https://leetcode.cn/problems/friend-requests-ii-who-has-the-most-friends/solutions/23104/hao-you-shen-qing-ii-shui-you-zui-duo-de-hao-you-b/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```

### [607. 销售员](https://leetcode.cn/problems/sales-person/description/)

表: SalesPerson

```
+-----------------+---------+
| Column Name     | Type    |
+-----------------+---------+
| sales_id        | int     |
| name            | varchar |
| salary          | int     |
| commission_rate | int     |
| hire_date       | date    |
+-----------------+---------+
```

sales_id 是该表的主键列(具有唯一值的列)。
该表的每一行都显示了销售人员的姓名和 ID ，以及他们的工资、佣金率和雇佣日期。
 

表: Company

```
+-------------+---------+
| Column Name | Type    |
+-------------+---------+
| com_id      | int     |
| name        | varchar |
| city        | varchar |
+-------------+---------+
```

com_id 是该表的主键列(具有唯一值的列)。
该表的每一行都表示公司的名称和 ID ，以及公司所在的城市。
 

表: Orders

```
+-------------+------+
| Column Name | Type |
+-------------+------+
| order_id    | int  |
| order_date  | date |
| com_id      | int  |
| sales_id    | int  |
| amount      | int  |
+-------------+------+
```

order_id 是该表的主键列(具有唯一值的列)。
com_id 是 Company 表中 com_id 的外键（reference 列）。
sales_id 是来自销售员表 sales_id 的外键（reference 列）。
该表的每一行包含一个订单的信息。这包括公司的 ID 、销售人员的 ID 、订单日期和支付的金额。
 

编写解决方案，找出没有任何与名为 “RED” 的公司相关的订单的所有销售人员的姓名。

以 任意顺序 返回结果表。

返回结果格式如下所示。

示例 1：

输入：
SalesPerson 表:
```
+----------+------+--------+-----------------+------------+
| sales_id | name | salary | commission_rate | hire_date  |
+----------+------+--------+-----------------+------------+
| 1        | John | 100000 | 6               | 4/1/2006   |
| 2        | Amy  | 12000  | 5               | 5/1/2010   |
| 3        | Mark | 65000  | 12              | 12/25/2008 |
| 4        | Pam  | 25000  | 25              | 1/1/2005   |
| 5        | Alex | 5000   | 10              | 2/3/2007   |
+----------+------+--------+-----------------+------------+
```

Company 表:
```
+--------+--------+----------+
| com_id | name   | city     |
+--------+--------+----------+
| 1      | RED    | Boston   |
| 2      | ORANGE | New York |
| 3      | YELLOW | Boston   |
| 4      | GREEN  | Austin   |
+--------+--------+----------+
```

Orders 表:
```
+----------+------------+--------+----------+--------+
| order_id | order_date | com_id | sales_id | amount |
+----------+------------+--------+----------+--------+
| 1        | 1/1/2014   | 3      | 4        | 10000  |
| 2        | 2/1/2014   | 4      | 5        | 5000   |
| 3        | 3/1/2014   | 1      | 1        | 50000  |
| 4        | 4/1/2014   | 1      | 4        | 25000  |
+----------+------------+--------+----------+--------+
```

输出：

```
+------+
| name |
+------+
| Amy  |
| Mark |
| Alex |
+------+
```

解释：
根据表 orders 中的订单 '3' 和 '4' ，容易看出只有 'John' 和 'Pam' 两个销售员曾经向公司 'RED' 销售过。
所以我们需要输出表 salesperson 中所有其他人的名字。

- 提交解答:

```sql
# Write your MySQL query statement below
# 按照公司和订单找出相关的销售员ID
# 最后使用not in 排除即可

SELECT name FROM SalesPerson WHERE sales_id NOT IN (
SELECT DISTINCT(sales_id) AS sales_id 
    FROM  Orders 
    LEFT JOIN Company ON Orders.com_id =  Company.com_id 
WHERE Company.name = "RED"
)
```

- 优质解答：

```sql

# 代码更加简洁，取消了左连接
SELECT
    name
FROM SalesPerson 
WHERE sales_id NOT IN (
    SELECT sales_id
    FROM Orders
    WHERE com_id = (
        SELECT com_id 
        FROM Company 
        WHERE name = 'RED'
    )
)
```

- 官方题解：

```sql
SELECT
    s.name
FROM
    salesperson s
WHERE
    s.sales_id NOT IN (SELECT
            o.sales_id
        FROM
            orders o
                LEFT JOIN
            company c ON o.com_id = c.com_id
        WHERE
            c.name = 'RED')
;

#作者：力扣官方题解
#链接：https://leetcode.cn/problems/sales-person/solutions/2366337/xiao-shou-yuan-by-leetcode-solution-wg7i/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```

### [601.体育馆的人流量](https://leetcode.cn/problems/human-traffic-of-stadium/description/)

表：Stadium
```
+---------------+---------+
| Column Name   | Type    |
+---------------+---------+
| id            | int     |
| visit_date    | date    |
| people        | int     |
+---------------+---------+
```
visit_date 是该表中具有唯一值的列。
每日人流量信息被记录在这三列信息中：序号 (id)、日期 (visit_date)、 人流量 (people)
每天只有一行记录，日期随着 id 的增加而增加
 

编写解决方案找出每行的人数大于或等于 100 且 id 连续的三行或更多行记录。

返回按 visit_date 升序排列 的结果表。

查询结果格式如下所示。

 

示例 1:

输入：
Stadium 表:
```
+------+------------+-----------+
| id   | visit_date | people    |
+------+------------+-----------+
| 1    | 2017-01-01 | 10        |
| 2    | 2017-01-02 | 109       |
| 3    | 2017-01-03 | 150       |
| 4    | 2017-01-04 | 99        |
| 5    | 2017-01-05 | 145       |
| 6    | 2017-01-06 | 1455      |
| 7    | 2017-01-07 | 199       |
| 8    | 2017-01-09 | 188       |
+------+------------+-----------+
```
输出：
```
+------+------------+-----------+
| id   | visit_date | people    |
+------+------------+-----------+
| 5    | 2017-01-05 | 145       |
| 6    | 2017-01-06 | 1455      |
| 7    | 2017-01-07 | 199       |
| 8    | 2017-01-09 | 188       |
+------+------------+-----------+
```
解释：
id 为 5、6、7、8 的四行 id 连续，并且每行都有 >= 100 的人数记录。
请注意，即使第 7 行和第 8 行的 visit_date 不是连续的，输出也应当包含第 8 行，因为我们只需要考虑 id 连续的记录。
不输出 id 为 2 和 3 的行，因为至少需要三条 id 连续的记录。

- 提交解答：

```sql
# 直接使用连表查询
# Write your MySQL query statement below
SELECT DISTINCT l1.*
FROM
    Stadium l1,
    Stadium l2,
    Stadium l3
WHERE
    l1.people >= 100
    AND l2.people >= 100
    AND l3.people >= 100
    AND (
        ( l1.id+1 = l2.id AND l3.id = l1.id + 2 ) 
        OR ( l1.id+1 = l2.id AND l3.id = l1.id - 1 ) 
        OR ( l1.id-1 = l2.id AND l3.id = l1.id - 2 )
    )
ORDER BY visit_date;

```

- 优质解答：

```sql
# Write your MySQL query statement below
# 使用了group by 直接进行了去重
select s1.*
from stadium s1, stadium s2, stadium s3
where(
    (s1.id + 1 = s2.id and s1.id + 2 = s3.id) or
    (s1.id - 1 = s2.id and s1.id + 1 = s3.id) or
    (s1.id - 1 = s2.id and s1.id - 2 = s3.id)) and 
    s1.people >= 100 and s2.people >= 100 and s3.people >= 100
group by s1.id
```

- 官方题解：

```sql
select distinct t1.*
from stadium t1, stadium t2, stadium t3
where t1.people >= 100 and t2.people >= 100 and t3.people >= 100
and
(
	  (t1.id - t2.id = 1 and t1.id - t3.id = 2 and t2.id - t3.id =1)  -- t1, t2, t3
    or
    (t2.id - t1.id = 1 and t2.id - t3.id = 2 and t1.id - t3.id =1) -- t2, t1, t3
    or
    (t3.id - t2.id = 1 and t2.id - t1.id =1 and t3.id - t1.id = 2) -- t3, t2, t1
)
order by t1.id
;

#作者：LeetCode
#链接：https://leetcode.cn/problems/human-traffic-of-stadium/solutions/50428/ti-yu-guan-de-ren-liu-liang-by-leetcode/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```

### [608. 树节点](https://leetcode.cn/problems/tree-node/description/)

- [MySQL 的CASE WHEN 语句使用说明](https://blog.csdn.net/helloxiaozhe/article/details/78124138)

表：Tree
```
+-------------+------+
| Column Name | Type |
+-------------+------+
| id          | int  |
| p_id        | int  |
+-------------+------+
```

id 是该表中具有唯一值的列。
该表的每行包含树中节点的 id 及其父节点的 id 信息。
给定的结构总是一个有效的树。
 

树中的每个节点可以是以下三种类型之一：

"Leaf"：节点是叶子节点。
"Root"：节点是树的根节点。
"lnner"：节点既不是叶子节点也不是根节点。
编写一个解决方案来报告树中每个节点的类型。

以 任意顺序 返回结果表。

结果格式如下所示。

 

示例 1：


输入：

```
Tree table:
+----+------+
| id | p_id |
+----+------+
| 1  | null |
| 2  | 1    |
| 3  | 1    |
| 4  | 2    |
| 5  | 2    |
+----+------+
```

输出：

```
+----+-------+
| id | type  |
+----+-------+
| 1  | Root  |
| 2  | Inner |
| 3  | Leaf  |
| 4  | Leaf  |
| 5  | Leaf  |
+----+-------+
```

解释：
节点 1 是根节点，因为它的父节点为空，并且它有子节点 2 和 3。
节点 2 是一个内部节点，因为它有父节点 1 和子节点 4 和 5。
节点 3、4 和 5 是叶子节点，因为它们有父节点而没有子节点。
示例 2：


输入：

```
Tree table:
+----+------+
| id | p_id |
+----+------+
| 1  | null |
+----+------+
```

输出：

```
+----+-------+
| id | type  |
+----+-------+
| 1  | Root  |
+----+-------+
```

解释：如果树中只有一个节点，则只需要输出其根属性。

- 提交解答

```sql
# 主要思路
# 统计节点所有的子节点数目，与父节点数目
SELECT A.id, IF(A.p_count=0, "Root",IF(s_count > 0,"Inner","Leaf") ) AS  type  
FROM 
(
 SELECT id, COUNT(p_id) AS p_count  FROM TREE GROUP BY id
) A LEFT JOIN 
(
    SELECT p_id  AS id, COUNT(id) AS s_count FROM TREE WHERE p_id >=0 GROUP BY p_id
) B ON A.id = B.id;

```

- 优质解答：

```sql
select id,case when p_id is null then 'Root' 
when id not in (select distinct p_id from Tree where p_id is not null) then 'Leaf'
else 'Inner' end type
from Tree 


```

- 官方题解：

```sql
SELECT
    atree.id,
    IF(ISNULL(atree.p_id),
        'Root',
        IF(atree.id IN (SELECT p_id FROM tree), 'Inner','Leaf')) Type
FROM
    tree atree
ORDER BY atree.id

#作者：LeetCode
#链接：https://leetcode.cn/problems/tree-node/solutions/23160/shu-jie-dian-by-leetcode/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```