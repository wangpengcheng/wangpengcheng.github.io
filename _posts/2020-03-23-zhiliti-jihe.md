---
layout:     post
title:      面试中的智力题集合
subtitle:   面试中的智力题集合
date:       2020-03-23
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - 后台开发
    - 智力题
    - 面试
---





# 参考

- [六道腾讯、百度、美团常爱问的面试智力题和答案](https://blog.csdn.net/abcdefg90876/article/details/102752131?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task)

## 1. 赛马, 25匹马, 5个赛道, 知道每场第一的具体成绩, 至少跑多少场可以找到最快的三匹

- [面试题：赛马问题](https://blog.csdn.net/jiutianhe/article/details/40744023)
- [腾讯QQ面试题：赛马问题](https://blog.csdn.net/zjhzyzc/article/details/4743585?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task)
- [一个很有意思的赛马问题](https://blog.csdn.net/Amy_mm/article/details/88733460?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task)

类似***\*K路归并排序\****, 分为***\*5组\****进行比赛, 选出5个第一名, 然后进行比赛, 确定一个, 并把第一名取走, 再从第一名的队列中, 选择第二名, 放入其中, 进行比赛.依次类推, 选择***\*最优的3匹马\****.共***\*需要8次比较\****, 5次初选, 3次选择最优.



![](https://img-blog.csdnimg.cn/20190322095144928.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FteV9tbQ==,size_16,color_FFFFFF,t_70)

最少10场（剩9匹的时候，选第一组的后3，第二组的前3，第三组的前2，如果第三组的第一这轮名次大于等于3，那就已经分出前四了，否则还要再多一场），最多11场。



## 2. 秒杀系统的设计
- [经典面试题，如何设计一个秒杀系统](https://blog.csdn.net/weixin_34246551/article/details/91369704)
- [秒杀系统设计思路](https://www.jianshu.com/p/60319f5f4167)
- [如何设计一个秒杀系统](https://github.com/gzc426/Java-Interview/blob/master/项目推荐/秒杀.md)
- 秒杀系统的整体架构可以概括为“稳、准、快”。
	- 稳:整个系统架构要满足高可用，流量符合预期时肯定要稳定，超出预期时也同样不能掉链子，你要保证秒杀活动顺利完成，即秒杀商品顺利地卖出去，这个是最基本的前提。
	- 准:你的业务需求是秒杀10台iPhone XS，那就只能成交10台，多一台少一台都不行。一旦库存不对，那平台就要承担损失。
	- 快:就是说系统的性能要足够高，否则你怎么支撑这么大的流量呢？不光是服务端要做极致的性能优化，而且在整个请求链路上都要做协同的优化，每个地方快一点，整个系统就完美了。
- 设计思路:将请求拦截在系统上游，降低下游压力。在一个并发量大，实际需求小的系统中，应当尽量在前端拦截无效流量，降低下游服务器和数据库的压力，不然很可能造成数据库读写锁冲突，甚至导致死锁，最终请求超时。
	- 限流：前端直接限流，允许少部分流量流向后端。
	- 削峰：瞬时大流量峰值容易压垮系统，解决这个问题是重中之重。常用的消峰方法有异步处理、缓存和消息中间件等技术。
	- 异步处理：秒杀系统是一个高并发系统，采用异步处理模式可以极大地提高系统并发量，其实异步处理就是削峰的一种实现方式。
	- 内存缓存：秒杀系统最大的瓶颈一般都是数据库读写，由于数据库读写属于磁盘IO，性能很低，如果能够把部分数据或业务逻辑转移到内存缓存，效率会有极大地提升。
	- 消息队列：消息队列可以削峰，将拦截大量并发请求，这也是一个异步处理过程，后台业务根据自己的处理能力，从消息队列中主动的拉取请求消息进行业务处理。
	- 可拓展：当然如果我们想支持更多用户，更大的并发，最好就将系统设计成弹性可拓展的，如果流量来了，拓展机器就好了，像淘宝、京东等双十一活动时会临时增加大量机器应对交易高峰。
- 主要手段:
	- 限流与降级:
		- 客户端限流:按钮置灰;js控制每秒只能发送一个请求
		- 站点层限流:
			- Nginx限流:限制IP的连接和并发分别由两个模块:
				- limit_req_zone： 用来限制单位时间内的请求数，即速率限制,采用的漏桶算法。
				- limit_req_conn： 用来限制同一时间连接数，即并发限制。
			- 站点层限流：防止脚本刷单；单个部署实例的每秒最大请求数，每个用户每秒的最大请求或者通过Redis记录和限制单个用户只能请求一次。
				- 写流量:根据uuid限制每个用户每秒只能一个请求。
				- 读流量：页面缓存和页面数据缓存。页面缓存可以是进程缓存，页面数据缓存一般是分布式缓存，保持各节点的数据一致性，如库存数量可以放到分布式缓存中。
			- **降级**
				- 如果流量太大，导致站点层限流后还是出现问题挂了或者站点层没问题，队列出问题了，则需要采取降级策略。
				- 对于站点层出问题，则可以在客户端直接提示“服务器繁忙，稍后再试”。
				- 对于队列挂了或者Redis挂了无法读取到库存信息，则可以在站点层降级处理，直接返回和提示“抢购人数太多，请稍后尝试”。
- **队列削峰**:通过第一步限流后，将合法流量放到一个队列中，实现流量削峰，达到流量可控和异步处理。
	- **入队条件**
		- 先将库存数量放到分布式缓存中，如Redis；先检查库存是否还有，有则扣减库存，这里尽量使用单线程，扣除成功将该请求放到队列中。
		- 否则库存不足，直接返回抢购完毕，或者可以优化一下说“抢购完毕，如果有小伙伴放弃，可以继续抢购”来避免队列消息处理失败导致还有没卖完。
	- **请求响应**
		- 入队成功或者失败都可以将该请求直接返回了，不过页面可以显示等待中或者提示抢购结果稍后通知，如现在很多抢票都是这样的。
- **服务层异步处理**
	- 服务层消费队列的数据，由于此时速度是可控的，故可以起一个后台服务节点即可，消费队列的数据，进行下单操作，递减数据库库存。
	- 如果消费队列的某个数据失败，可以采用fail-fast的原则，直接提示失败，不需要进行重试之类的复杂操作。
- **抢购结果通知**:由于使用了队列来异步处理，即入队后或者库存不足无法入队，该次抢购请求是直接返回了的，故对于抢购结果是需要进行额外通知的。
	- **客户端轮询**:
		- 可以通过客户端定时请求服务端，如每秒发送一个请求，如果成功，则提示抢购成功；失败，则返回失败。例如，客户端可以在没有轮询到处理结果时提示“抢购中，请耐心等待”，如果轮询到结果则提示成功或失败。
	- **消息推送**:
		- 另外一种方式可以是直接通过消息推送的方式来通知用户抢购结果。

![](https://upload-images.jianshu.io/upload_images/17825765-eb79eafb42ea2c19.png?imageMogr2/auto-orient/strip|imageView2/2/format/webp)



## 3.  逻辑题，有一副没有大小王的扑克牌，第一张翻过来，第二张不翻直接放到最后，依次这样，全部翻完，最后得到A到K，问最开始的顺序是什么样的，怎么想的 ;如果第一张不翻，第二张翻过来放到最后，这样最开始的顺序又是什么样的


- [用python解华为“13张扑克牌抽取题目”](https://blog.csdn.net/vinrex/article/details/37813899?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task)
- [扑克牌面试问题:从牌顶拿出一张牌放到桌子上,再从牌顶拿一张牌放在手上牌的底部，重复第一步、第二步的操作](https://blog.csdn.net/qinfeng9988/article/details/83868644?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task)
- [扑克牌的顺序问题](https://blog.csdn.net/qq_25002995/article/details/103115750?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task)
- 答:将操作逆序操作，先从桌面上拿起一张牌，然后看手中是否为空，不为空，则将底部的移动到上部。重复操作，直到桌面上没有牌。由顶到低*{13,12,11,10,9,8,7,6,5,4,3,2,1}*，开始顺序为:*{1,12,2,8,3,11,4,9,5,13,6,10,7}*  
- 同理，先移动到顶部，再桌面上的牌发送过来，顺序应该没有变化。

## 4. 4片A药片和4片B药片，每天必须一片A和一片B；现在AB混合在一起了怎么办？

答：本质上还是均等分的问题，将每个药片均分为4份，每一份组合成一份药片，刚好一片A和一片B。或者直接将所有的药片研磨成粉末混合均匀。每次取4分之一服用就好。

## 5. 屋里面右三盏灯，屋外右三个开关，如何只进一次屋确定，开关和灯的对应次序。

答：因为有三盏灯，所以必须要要至少2次操作，才能确定真正的灯。因此操作数必须为2或者以上；但是只能进屋中一次，所以需要两次操作的其它观察状态来确定。因此需要除了亮灯之外的其它操作。所以需要温度状态作为参考。因此先将其中一个开关打开，一段时间后，再打开另外一个开关，进屋后，温度最高的就是最先开启的那个，其次亮灯的是另外开启的那个。

## 6. A升杯子和B升的杯子如何倒出C升的水?

- [A升杯子和B升的杯子如何倒出C升的水?（某公司校园招聘笔试试题，某另外公司校园招聘笔试试题）](https://blog.csdn.net/stpeace/article/details/8106804)

- [终结：容量为a、b的两个杯子倒出容量c的水](https://blog.csdn.net/henuboy/article/details/11287183?depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1&utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1)
- 先来简化一下这个问题：如果c>a+b，那么我们总可以通过倒入n次a和m次b使得c-(na+mb)<a+b，(n、m为非负整数)，所以我们只需要研究c<a+b的情况。设x为a、b两数的最大公约数，那么能够量出c的数学条件是c能整除x，即c%x==0.解释一下原因。
- 因为x是最大公约数，因此根据欧几里得算法必存在两个整数p和q使得x=pa+qb；而且p和q一正一负，不妨设p正q负，那么要想量出x容量的水只需倒满p次a，倒空q次b即可，举例说明设a=25，b=15那么x=5；2b-a=x；也就是说把b倒满两次，每次倒满后倒入a中a满后将a倒空，最后两个杯子中剩余的就是x了。再设a=7，b=5那么x=1；3a-4b=x，换言之就是把a倒满，每次倒满后都倒入b，如果b满了就将b倒空（若a倒满b后a非空，倒空b后把a剩余的倒入b中然后再倒满a），如此倒满3次a，倒空4次b后两容器剩余的就是x了。因此C必定是x的整数倍。
- 下面证明若c不能整除x则不能用a，b容器量出，因为c不能整除x，那么必不存在整数p和q使得c=pa+qb,现在只需证明任何对a，b的操作都可以表示成au+bv，我们看对a和b的操作无非就是倒满a，倒空a，从a导向b，倒满b，倒空b，从b导向a。两个都是满的状态和两个都是空的状态都没有意义；一个满一个空，能进行操作就是从一个倒向另一个，假设a倒像b，那么结果要么a空要么b满，假设b满a非空，那么下一步若把a倒掉，没有意义（因为这又回到了一满一空的状态），所以需要把b倒掉，那么再下一步呢？把a倒满吗？这也没啥意义（因为这还是回到了一空一满的状态）。
  通过归纳发现有意义的操作都是把空的倒满，或把满的倒空。而把半空的灌满或者倒空都没意义（任何一个时刻a和b不可能都半空）。也就是说上说6种操作的一个序列，可以加上条件。也就是倒满a的时候要求是a是空的，倒空a的时候要求a是满的。这样任何一个操作过程最终就可以表示为a*u+b*v。