---
layout:     post
title:      Apache Kafka面经
subtitle:   Apache Kafka面经&源码阅读笔记
date:       2024-02-21
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - 架构
    - 中间件
    - 消息中间件
---


- [Kafka 实现延迟队列、死信队列、重试队列](https://www.cnblogs.com/zhengzhaoxiang/p/13973263.html)
- [18道kafka高频面试题哪些你还不会？（含答案和思维导图）](https://developer.aliyun.com/article/740170)
- [kafka教程](https://dunwu.github.io/bigdata-tutorial/kafka/#%F0%9F%93%96-%E5%86%85%E5%AE%B9)
- [40 道精选 Kafka 面试题](https://javabetter.cn/interview/kafka-40.html)

## Kafka 死信说一下

死信即为消费端无法正常消费处理，为了防止其长时间滞留在分片队列中，**将其投入到重试队列中，防止其被丢弃**。当超过一定重试次数后，将其投入到**死信队列中**，这里的死信就可以看作消费者不能处理的消息。再比如超过既定的重试次数之后将消息投入死信队列，这里就可以将死信看作不符合处理要求的消息。
消息进入死信队列后，需要进一步处理， (转回、导出和丢弃)

kafka中可以使用 `enableConsumeRetry(true)` 开启消费重试，重试多次后，再进入死信队列

```java
// https://kafka.apache.org/documentation/#consumerconfigs

Properties props = new Properties();
props.put("bootstrap.servers", "localhost:9092");
props.put("group.id", "test-group");
props.put("enable.auto.commit", "false");
props.put("key.deserializer", "org.apache.kafka.common.serialization.StringDeserializer");
props.put("value.deserializer", "org.apache.kafka.common.serialization.StringDeserializer");

KafkaConsumer<String, String> consumer = new KafkaConsumer<>(props);
consumer.subscribe(Collections.singletonList("test-topic"));

// 开启消费者重试功能
consumer.enableConsumeRetry(true);

while (true) {
    ConsumerRecords<String, String> records = consumer.poll(Duration.ofMillis(100));
    
    for (ConsumerRecord<String, String> record : records) {
        // 处理消息的逻辑
        try {
            processRecord(record);
        } catch (Exception e) {
            // 消息处理发生错误，将自动重试
        }
    }
}

consumer.close();


```

___

- 参考：[Kafka 实现延迟队列、死信队列、重试队列](https://www.cnblogs.com/zhengzhaoxiang/p/13973263.html)

## Kafka 分片(Partition)说一下

Partition 是kafka topic 数据发送与缓存的一种分片机制，相当于多个缓冲队列，支持kafka进行水平扩容。

1. 定义：Topic 是一个逻辑概念，Partition 是最小的存储单元，掌握着一个 Topic 的部分数据。每个 Partition 都是一个单独的 log 文件，每条记录都以追加的形式写入。
2. 消息顺序：topic存在多个分片时，分片内消息有序，但是因为消息发送至分片是默认随机的，因此整体无序

![Partition ](https://pic4.zhimg.com/80/v2-2d4cffb01a27fae3bf480db36e6e6317_720w.webp)
3. 消费分组：kafka使用分组的方式进行消费组分组，需要广播消费，可以创建一个只有一个分区的Topic，并让每个消费者组中的所有消费者都订阅该Topic，这样每个消费者都可以独立地接收该分区中的所有消息，从而实现消息的广播。
4. 优点：
    - 提供扩展能力：把 Partition 分散开之后，Topic 就可以水平扩展。同时支持topic被多个Consumer并行消费。提升消费能力
    - 提供数据冗余：kafka  Partition 生成多个副本，并且把它们分散在不同的 Broker，如果一个 Broker 故障了，Consumer 可以在其他 Broker 上找到 Partition 的副本，继续获取消息。
5. 缺点：
    - 增加了消息写入的复杂性与消息读取的复杂性
6. 广播消费：kafka 支持广播消费模式，使用广播消费模式，将每个消费者的Group ID设置为相同的值，并将属性 enable.auto.commit 设置为 false，这样每个消费者都可以独立地接收到所有的消息。
___

- 参考：[细说 Kafka Partition 分区](https://zhuanlan.zhihu.com/p/371886710);[Kafka设置广播消费](https://cloud.tencent.com/developer/article/1863356)


## 消费堆积处理方式

消费堆积常用处理方式：
1. 消费者扩容：增加处理消费者的数量，加快消费速度
2. 实例消费者线程数增加：增加消费者线程，开启多线程消费
3. 适当增加topic分片数：如果topic分片数过少，导致消费者数量>=topic分片数。可以适当扩展增加topic分片数
4. 消费异常排查：是否存在异常处理，导致消息不能被正常处理


## 确认分片被消费

如果您想知道某个特定消息是否已被消费，您可以使用消息的偏移量来确定它的位置，然后与消费者组的当前偏移量进行比较。如果消息的偏移量小于消费者组的当前偏移量，则表示消息已被消费。

总的来说，Kafka 中的消息偏移量是您查看消息是否被消费的关键工具。通过使用偏移量，您可以轻松地监视消息处理情况，并确保您的消息得到了适当的处理。


## 如何获取topic主题的列表

可以通过如下的方式获取主题的列表

```bash
bin/kafka-topics.sh --list --zookeeper localhost:2181
```

## 生产者和消费者的命令行是什么？

- 生产者在主题上发布消息：
```bash
bin/kafka-console-producer.sh --broker-list 192.168.43.49:9092 --topicHello-Kafka
```
注意这里的 IP 是 server.properties 中的 listeners 的配置。接下来每个新行就是输入一条新消息。

- 消费者接受消息：
```bash
bin/kafka-console-consumer.sh --zookeeper localhost:2181 --topicHello-Kafka --from-beginning
```

## consumer是推还是拉？

Kafka 遵循了一种大部分消息系统共同的传统的设计：producer 将消息推送到 broker，consumer 从broker 拉取消息。
- 主要的好处如下：
    - 防止broker push过多导致consumer 崩溃：使用push方式进行消费时，当 broker 推送的速率远大于 consumer 消费的速率时，consumer 恐怕就要崩溃了。最终 Kafka 还是选取了传统的 pull 模式。
    - consumer 可以自主决定是否批量的从 broker 拉取数据 ：Pull 模式下，consumer 就可以根据自己的消费能力去决定这些策略。减轻了broker消息推送的负担
- 缺点：
    - 消费阻塞：broker中无消息时，会导致consumer不断在循环中轮询，直到新消息到达。kafka 通过 设置consumer 阻塞直到消息到达。


## 讲讲 kafka 维护消费状态跟踪的方法

Topic 被分成了若干分区，每个分区在同一时间只被一个 consumer 消费。这意味着每个分区被消费的消息在日志中的位置仅仅是一个简单的整数：offset。这样就很容易标记每个分区消费状态就很容易了，仅仅需要一个整数而已。这样消费状态的跟踪就很简单了。
这带来了另外一个好处：consumer 可以把 offset 调成一个较老的值，去重新消费老的消息。这对传统的消息系统来说看起来有些不可思议，但确实是非常有用的，谁规定了一条消息只能被消费一次呢？消费端进行消费后会通过`commit` 方法确认其已经消费对应消息。


____

- 参考：[Kafka消费循环](http://www.linkedkeeper.com/1653.html)

## kafka消费后消息还存在吗

在Kafka中消息是以分区(Partition)的形式被保存，消息在分区中是有序且不可变的。消费者进行消费消息时，实际上只是从分区中读取消息。消费者消费后Kafka不会立即将消息从分区中移除，而是重新设置消费组的偏移量(Offset)。同组其它消费者消费时获取最新数据。**消费者可以通过重置偏移量的方式，进行重复消费**

## Kafka消息存储时间

首先，Kafka的配置决定了消息是否会被永久保存。在Kafka的配置中，有一个参数叫做log.retention.ms，指定了消息在Kafka中的保留时间。默认情况下，这个值为7天，即Kafka中的消息会在7天后被删除。如果需要更长的消息保存时间，可以将这个值设置得更大。

其次，消费者消费消息的方式也会影响消息的删除。在Kafka中，有两种消费者：

- 消费者组：多个消费者可以共同消费主题中的消息。当消费者组中的某个消费者消费一条消息后，这条消息并不会立刻被删除。只有当所有消费者都成功消费这条消息后，这条消息才会被删除。

- 单个消费者：一个消费者独立地消费一个或多个主题中的消息。当单个消费者消费一条消息后，这条消息会立刻被删除。

## 消息被消费后，没有删除，导致Kafka存储空间占满

消息被消费后，并不会被删除，只有超过老化时间，才会被删除，可以通过减少老化时间或者扩容存储空间进行解决


## 说一下主从同步

Kafka允许topic的分区拥有若干副本，这个数量是可以配置的，你可以为每个topci配置副本的数量。Kafka会自动在每个个副本上备份数据，所以当一个节点down掉时数据依然是可用的。
Kafka的副本功能不是必须的，你可以配置只有一个副本，这样其实就相当于只有一份数据。

## 为什么需要消息系统，mysql 不能满足需求吗？

1. 系统解耦：允许你独立的扩展或修改两边的处理过程，只要确保它们遵守同样的接口约束。
2. 冗余容灾：消息队列把数据进行持久化直到它们已经被完全处理，通过这一方式规避了数据丢失风险。许多消息队列所采用的”插入-获取-删除”范式中，在把一个消息从队列中删除之前，需要你的处理系统明确的指出该消息已经被处理完毕，从而确保你的数据被安全的保存直到你使用完毕。
3. 扩展性：解耦处理过程，方便进行扩容
4. 异步&削峰：可以通过MQ的方式有效解决请求流量高峰的问题，将请求暂存，缓慢进行处理。避免突发请求导致后端崩溃。支持异步通信处理
5. 可恢复性：消息队列降低了进程间的耦合度，进行了消息暂存，支持回放和重复消费
6. 顺序保证：保证数据处理的顺序性
7. 缓冲：控制和优化数据生产以及数据处理速度，解决生产消息和消费消息的处理速度不一致的情况。

## Zookeeper 对于 Kafka 的作用是什么？

在 Kafka 中，它被用于提交偏移量，因此如果节点在任何情况下都失败了，它都可以从之前提交的偏移量中获取除此之外，它还执行其他活动，如: leader 检测、分布式同步、配置管理、识别新节点何时离开或连接、集群、节点实时状态等等。

## 数据传输的事务定义有哪三种？

和 MQTT 的事务定义一样都是 3 种。
1. 最多一次: 消息不会被重复发送，最多被传输一次，但也有可能一次不传输
2. 最少一次: 消息不会被漏发送，最少被传输一次，但也有可能被重复传输.
3. 精确的一次（Exactly once）: 不会漏传输也不会重复传输,每个消息都传输被一次而且仅仅被传输一次，这是大家所期望的

## Kafka 判断一个节点是否还活着有那两个条件？

1. 节点必须可以维护和 ZooKeeper 的连接，Zookeeper 通过心跳机制检查每个节点的连接
2. 如果节点是个 follower,他必须能及时的同步 leader 的写操作，延时不能太久

## Kafka 与传统 MQ 消息系统之间有三个关键区别

1. 日志持久化：kafka支持持久化日志，这些日志可以被重复读取和无限期保留
2. 扩容灵活：Kafka 是一个分布式系统：它以集群的方式运行，可以灵活伸缩，在内部通过复制数据提升容错能力和高可用性
3. 顺序处理：kafka有着强大的顺序处理能力，支持实时的流式处理


## kafka 的 ack 的三种机制

equest.required.acks 有三个值 0 1 -1(all)
- 0:生产者不会等待 broker 的 ack，这个延迟最低但是存储的保证最弱当 server 挂掉的时候就会丢数据。
- 1：服务端会等待 ack 值 leader 副本确认接收到消息后发送 ack 但是如果 leader挂掉后他不确保是否复制完成新 leader 也会导致数据丢失。
- -1(all)：服务端会等所有的 follower 的副本受到数据后才会受到 leader 发出的ack，这样数据不会丢失

## 消费者如何不自动提交偏移量，由应用提交？


将 auto.commit.offset 设为 false，然后在处理一批消息后 commitSync() 或者异步提交 commitAsync()

```java

ConsumerRecords<> records = consumer.poll();
for (ConsumerRecord<> record : records){
    。。。
    tyr{
        consumer.commitSync()
    }
    。。。
}

```

## 消费者故障，出现活锁问题如何解决？

- 活锁定义：消费者持续发送心跳但是没有处理消息的情况。

为了预防消费者在这种情况下一直持有分区，我们使用 max.poll.interval.ms 活跃检测机制，来进行判断消息是否被消费。当poll评率大于最大时间间隔，被判定为非活动成员，客户端将主动离开组，同时发生offset提交失败，最终表现为无法进行消息处理。**根因在于处理时间大于活跃检测时间**

- 解决方法：主要还是从消费端处理时间入手，增加
    - max.poll.interval.ms：增大 poll 的间隔，可以为消费者提供更多的时间去处理返回的消息（调用 poll(long)返回的消息，通常返回的消息都是一批）。缺点是此值越大将会延迟组重新平衡。
    - max.poll.records：此设置限制每次调用 poll 返回的消息数，这样可以更容易的预测每次 poll 间隔要处理的最大值。通过调整此值，可以减少 poll 间隔，减少重新平衡分组的
    - 开启多线程处理：主线程仅仅进行消息读取，处理交给子线程。

## 如何控制消费的位置

kafka 使用 `seek(TopicPartition, long)`指定新的消费位置。用于查找服务器保留的最早和最新的 offset 的特殊的方法也可用（`seekToBeginning(Collection)` 和`seekToEnd(Collection)`）

## kafka 分布式（不是单机）的情况下，如何保证消息的顺序消费?

kakfa 分布式的单位是 partition，同一个 partition 用一个 write ahead log 组织，所以可以保证 FIFO 的顺序。不同partition不能保证顺序性。
- 生产端写入顺序性：
    - 用户可以通过 message key 来定义,因为同一个 key 的 message 可以保证只发送到同一个 partition。只要保证message key相同，就可以生产端的顺序性。
    - partition 指定：直接指定写入的partition
- 消费端消费顺序性：一个partition 中可以绑定多个消费组，统一消费组中消费者轮流进行消费。只要保证一个消费组中当前partition只有一个消费者就可以保证消费顺序性

## kafka 的高可用机制是什么


## kafka 如何减少数据丢失

### kafka数据丢失
从 Kafka 整体架构图我们可以得出有三次消息传递的过程：

1）Producer 端发送消息给 Kafka Broker 端。

2）Kafka Broker 将消息进行同步并持久化数据。

3）Consumer 端从 Kafka Broker 将消息拉取并进行消费。

![kafka消息传递过程](https://s8.51cto.com/oss/202204/20/a89d7813053ad2d83fd086bd9b7709395dd50a.png)

三次传递过程中均有可能造成消息丢失，主要场景如下：

- Producer 端消息发送失败：
    - 网络原因：由于网络抖动导致数据根本就没发送到 Broker 端。
    - 数据原因：消息体太大超出 Broker 承受范围而导致 Broker 拒收消息
    - ack机制原因：ack默认级别为1(leader接收成功即可)，Leader Partition 异常 Crash，Follower Partition 未进行同步数据 ACK，导致数据丢失。
![ack机制](https://s6.51cto.com/oss/202204/20/252af52153170a5bd54558518be0754e880519.png)

- Broker端丢消息丢失：
    - 持久化原因：broker使用异步磁盘刷入的方式，数据刷盘过程中 Broker 宕机会导致数据丢失，且选举了一个落后 Leader Partition 很多的 Follower Partition 成为新的 Leader Partition，那么落后的消息数据就会丢失。
        - 由于 Kafka 中并没有提供「同步刷盘」的方式，所以说从单个 Broker 来看还是很有可能丢失数据的。
        - kafka 通过「多 Partition （分区）多 Replica（副本）机制」已经可以最大限度的保证数据不丢失，如果数据已经写入 PageCache 中但是还没来得及刷写到磁盘，此时如果所在 Broker 突然宕机挂掉或者停电，极端情况还是会造成数据丢失。

![持久化](https://s3.51cto.com/oss/202204/20/e584308437ff38bc09764442028f82332ab58a.png)

-  Consumer数据丢失：Consumer 提交 Offset异常导致数据丢失
    - 自动提交：轮询提交，可以有效解决重复消费的问题，但是也会导致部分消息消费异常后被提交 ([kafka消费者自动提交是如何工作的？](https://segmentfault.com/q/1010000042942095))
    - 拉取消息后「先提交 Offset，后处理消息」: 处理异常，导致消息丢失
    - 拉取消息后「先处理消息，在进行提交 Offset」：提交前异常，会导致业务重复消费


![消费流程](https://s4.51cto.com/oss/202204/20/125dee467e054202def595b57ea4ccd4db1d99.png)

### 解决方案

- 生产端：
    - 异常回调：设置异常回调函数，捕获发送异常。
        - 网络丢失：直接重试发送
        - 消息大小不合格：重新调整大小再发送
    - ACK 确认机制：设置`request.required.acks` 为-1保证每个副本都接收到消息。同时为了防止follwer 未全部同步，Leader异常挂掉，导致的发送失败，可以设置`replication.factor >= 2` 与 `min.insync.replicas > 1`
        - ![](https://s5.51cto.com/oss/202204/20/e9c7e8a24a69cccaac65035cc6649a8ed134d8.png)
    - 设置重试：设置重试相关参数保证消息稳定
        - 重试次数：设置`retries` 为大于0，允许其反复重试
        - 重试时间间隔：增大`retry.backoff.ms`(默认为100ms)，比如设置为300ms。
- broker端：通过「多分区多副本」的方式来最大限度的保证数据不丢失。
    - 禁止落后数据被选为leader：设置`unclean.leader.election.enable` 为false,数据太多的follower被选举为leader
    - 增大分区副本：设置`replication.factor>=3`增大分区副本，保证follower副本继续提供服务
    - 增大最少写入副本数：增大`min.insync.replicas` 提升消息持久性，保证数据不丢失。(保证`replication.factor > min.insync.replicas`, 如果相等，只要有一个副本异常 Crash 掉，整个分区就无法正常工作了，因此推荐设置成： replication.factor = min.insync.replicas +1, 最大限度保证系统可用性。)
- Consumer端：确保正常消费后提交offset
    - 强业务逻辑：保证拉取数据、业务逻辑处理、提交消费 Offset 位移信息。顺序执行
    - 关闭自动提交：设置参数 `enable.auto.commit = false`，关闭自动提交，采用手动提交位移的方式。([Kafka学习笔记: 自动提交&手动提交.](https://blog.csdn.net/zhanglong_4444/article/details/103713757))


总结不丢消息的配置为：
- topic 副本数 >= 3
- topic [MinISR](https://kafka.apache.org/documentation/#brokerconfigs_min.insync.replicas) >= 2
- 生产者 [acks](https://kafka.apache.org/documentation/#producerconfigs_acks) = -1 (或all)
- 服务端关闭[脏选](https://kafka.apache.org/documentation/#brokerconfigs_unclean.leader.election.enable)
- 消费者 [auto.reset.offset](https://kafka.apache.org/documentation/#consumerconfigs_auto.offset.reset) = nearest

___


- 参考：[刨根问底: Kafka 到底会不会丢数据？](https://www.51cto.com/article/707006.html);[Kafka提交offset机制](https://www.cnblogs.com/FG123/p/10091599.html)

## kafka 如何避免重复消费

1. 业务自行保证（容错）：消息中设置业务唯一键，遇到重复消息时直接丢弃
2. 手动提交（强校验）：使用手动提交方式，保证每个消息都被正确消费。
3. 消费控制：控制消费者提交消费偏移量的时机和方式，避免重复提交或漏提交。

___

- [kafka避免重复消费问题](https://juejin.cn/post/7224677855245910075)