---
layout:     post
title:      实验室服务器使用说明
subtitle:   实验室服务器使用说明
date:       2020-04-21
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - 服务器配置
---

# 实验室服务器使用说明

> 2020-04-21 15:30:29


## 1. 在线服务器参数列表

|名称|位置|局域网IP|CPU|显卡|基本环境|登录账号|登录密码|frp映射端口|共享目录|
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|node00-ubuntu|12F|192.168.58.194|`Intel(R) Core(TM) i7-9700 CPU @ 3.00GHz`|`RTX2080`|`NVIDIA-SMI 430.50;CUDA 10.1.243;cudnn 7.6.3;python3-pytorch1.3.1;horovod 0.18.2;openmpi 4.0.2`|node|root|6002|`/home/node/DataShare`|
|node01-ubuntu|12E|192.168.58.58|`Intel(R) Core(TM) i7-8700 CPU @ 3.20GHz`|`GTX1070`|`NVIDIA-SMI 430.50;CUDA 10.1.243;cudnn 7.6.3;python3-pytorch1.3.1;horovod 0.18.2;openmpi 4.0.2`|node|root|6003|`/home/node/DataShare`|
|node02-ubuntu|10E|192.168.58.220|`Intel(R) Core(TM) i7-8700 CPU @ 3.20GHz`|`GTX1070`|`NVIDIA-SMI 430.50;CUDA 10.1.243;cudnn 7.6.3;python3-pytorch1.3.1;horovod 0.18.2;openmpi 4.0.2`|node|root|6004|`/home/node/DataShare`|

![座位图片](https://wangpengcheng.github.io/img/20200421180747.jpg)

## 2. ssh 远程登录方式
使用ssh 公网服务器地址与对应frp端口号登录。公网服务器地址稍后公开;使用示例如下:
```ssh
ssh node@192.168.58.194 -p 6002 
```

## 3. 挂载共享文件夹

文件夹存放在node00节点中，其它节点可以使用如下的命令进行挂载，并直接访问node用户下的DataShare文件夹

```shell
sudo mount -t nfs node00-ubuntu:/home/node/DataShare /mnt/DataShare
```

## 4. horovod 分布式训练使用

## 4.1 horovod使用参考

horovod使用参考分布式训练环境已经搭建完毕；具体使用可以参考官方文档和介绍：
- [官方链接](https://github.com/horovod/horovod)

## 4.2 测试

- 以node用户登入node00节点
- 运行测试脚本
```shell
node@node00-ubuntu:~$ ~/DataShare/horovod_test.sh
```
- 输入如下结果，表示运行正确
```shell
Tue Apr 21 18:22:52 2020[0]<stdout>:cuda is available:1
Tue Apr 21 18:22:52 2020[0]<stdout>:args.cuda is available:1
Tue Apr 21 18:22:53 2020[2]<stdout>:cuda is available:1
Tue Apr 21 18:22:53 2020[2]<stdout>:args.cuda is available:1
Tue Apr 21 18:22:53 2020[1]<stdout>:cuda is available:1
Tue Apr 21 18:22:53 2020[1]<stdout>:args.cuda is available:1
Tue Apr 21 18:22:53 2020[1]<stdout>:is Use nccl:0
Tue Apr 21 18:22:53 2020[2]<stdout>:is Use nccl:0
Tue Apr 21 18:22:53 2020[0]<stdout>:is Use nccl:0
Tue Apr 21 18:22:57 2020[2]<stderr>:/home/node/DataShare/pytorch_mnist.py:88: UserWarning: Implicit dimension choice for log_softmax has been deprecated. Change the call to include dim=X as an argument.
Tue Apr 21 18:22:57 2020[2]<stderr>:  return F.log_softmax(x)
Tue Apr 21 18:22:57 2020[1]<stderr>:/home/node/DataShare/pytorch_mnist.py:88: UserWarning: Implicit dimension choice for log_softmax has been deprecated. Change the call to include dim=X as an argument.
Tue Apr 21 18:22:57 2020[1]<stderr>:  return F.log_softmax(x)
Tue Apr 21 18:22:57 2020[0]<stderr>:/home/node/DataShare/pytorch_mnist.py:88: UserWarning: Implicit dimension choice for log_softmax has been deprecated. Change the call to include dim=X as an argument.
Tue Apr 21 18:22:57 2020[0]<stderr>:  return F.log_softmax(x)
Tue Apr 21 18:22:57 2020[1]<stdout>:Train Epoch: 1 [0/20000 (0%)]       Loss: 2.315418
Tue Apr 21 18:22:57 2020[0]<stdout>:Train Epoch: 1 [0/20000 (0%)]       Loss: 2.344431
Tue Apr 21 18:22:57 2020[2]<stdout>:Train Epoch: 1 [0/20000 (0%)]       Loss: 2.331809
Tue Apr 21 18:22:57 2020[0]<stdout>:Train Epoch: 1 [640/20000 (3%)]     Loss: 2.322608
Tue Apr 21 18:22:57 2020[2]<stdout>:Train Epoch: 1 [640/20000 (3%)]     Loss: 2.287621
Tue Apr 21 18:22:57 2020[1]<stdout>:Train Epoch: 1 [640/20000 (3%)]     Loss: 2.296363
Tue Apr 21 18:22:57 2020[0]<stdout>:Train Epoch: 1 [1280/20000 (6%)]    Loss: 2.292358
Tue Apr 21 18:22:57 2020[2]<stdout>:Train Epoch: 1 [1280/20000 (6%)]    Loss: 2.301269
Tue Apr 21 18:22:57 2020[1]<stdout>:Train Epoch: 1 [1280/20000 (6%)]    Loss: 2.297354
Tue Apr 21 18:22:57 2020[1]<stdout>:Train Epoch: 1 [1920/20000 (10%)]   Loss: 2.271436
Tue Apr 21 18:22:57 2020[0]<stdout>:Train Epoch: 1 [1920/20000 (10%)]   Loss: 2.261778
Tue Apr 21 18:22:57 2020[2]<stdout>:Train Epoch: 1 [1920/20000 (10%)]   Loss: 2.236528
Tue Apr 21 18:22:57 2020[0]<stdout>:Train Epoch: 1 [2560/20000 (13%)]   Loss: 2.232875
Tue Apr 21 18:22:57 2020[2]<stdout>:Train Epoch: 1 [2560/20000 (13%)]   Loss: 2.186383
Tue Apr 21 18:22:57 2020[1]<stdout>:Train Epoch: 1 [2560/20000 (13%)]   Loss: 2.130816
Tue Apr 21 18:22:58 2020[1]<stdout>:Train Epoch: 1 [3200/20000 (16%)]   Loss: 2.072108
Tue Apr 21 18:22:58 2020[2]<stdout>:Train Epoch: 1 [3200/20000 (16%)]   Loss: 2.014250
Tue Apr 21 18:22:58 2020[0]<stdout>:Train Epoch: 1 [3200/20000 (16%)]   Loss: 1.965220
Tue Apr 21 18:22:58 2020[1]<stdout>:Train Epoch: 1 [3840/20000 (19%)]   Loss: 1.791345
Tue Apr 21 18:22:58 2020[0]<stdout>:Train Epoch: 1 [3840/20000 (19%)]   Loss: 1.755321
Tue Apr 21 18:22:58 2020[2]<stdout>:Train Epoch: 1 [3840/20000 (19%)]   Loss: 1.719203
Tue Apr 21 18:22:58 2020[1]<stdout>:Train Epoch: 1 [4480/20000 (22%)]   Loss: 1.493969
Tue Apr 21 18:22:58 2020[0]<stdout>:Train Epoch: 1 [4480/20000 (22%)]   Loss: 1.564214
Tue Apr 21 18:22:58 2020[2]<stdout>:Train Epoch: 1 [4480/20000 (22%)]   Loss: 1.565062
Tue Apr 21 18:22:58 2020[1]<stdout>:Train Epoch: 1 [5120/20000 (26%)]   Loss: 1.336752
Tue Apr 21 18:22:58 2020[0]<stdout>:Train Epoch: 1 [5120/20000 (26%)]   Loss: 1.180155
Tue Apr 21 18:22:58 2020[2]<stdout>:Train Epoch: 1 [5120/20000 (26%)]   Loss: 1.159705
Tue Apr 21 18:22:58 2020[2]<stdout>:Train Epoch: 1 [5760/20000 (29%)]   Loss: 1.084617
Tue Apr 21 18:22:58 2020[0]<stdout>:Train Epoch: 1 [5760/20000 (29%)]   Loss: 1.155716
Tue Apr 21 18:22:58 2020[1]<stdout>:Train Epoch: 1 [5760/20000 (29%)]   Loss: 0.975106
Tue Apr 21 18:22:58 2020[0]<stdout>:Train Epoch: 1 [6400/20000 (32%)]   Loss: 1.125397
Tue Apr 21 18:22:58 2020[2]<stdout>:Train Epoch: 1 [6400/20000 (32%)]   Loss: 0.980524
Tue Apr 21 18:22:58 2020[1]<stdout>:Train Epoch: 1 [6400/20000 (32%)]   Loss: 0.896509
Tue Apr 21 18:22:58 2020[2]<stdout>:Train Epoch: 1 [7040/20000 (35%)]   Loss: 0.889235
Tue Apr 21 18:22:58 2020[0]<stdout>:Train Epoch: 1 [7040/20000 (35%)]   Loss: 0.900998
Tue Apr 21 18:22:58 2020[1]<stdout>:Train Epoch: 1 [7040/20000 (35%)]   Loss: 1.070345
Tue Apr 21 18:22:58 2020[2]<stdout>:Train Epoch: 1 [7680/20000 (38%)]   Loss: 0.791290
Tue Apr 21 18:22:58 2020[0]<stdout>:Train Epoch: 1 [7680/20000 (38%)]   Loss: 0.765100
Tue Apr 21 18:22:58 2020[1]<stdout>:Train Epoch: 1 [7680/20000 (38%)]   Loss: 0.771025
Tue Apr 21 18:22:58 2020[2]<stdout>:Train Epoch: 1 [8320/20000 (42%)]   Loss: 0.607366
Tue Apr 21 18:22:58 2020[1]<stdout>:Train Epoch: 1 [8320/20000 (42%)]   Loss: 0.812178
Tue Apr 21 18:22:58 2020[0]<stdout>:Train Epoch: 1 [8320/20000 (42%)]   Loss: 0.910085
Tue Apr 21 18:22:58 2020[2]<stdout>:Train Epoch: 1 [8960/20000 (45%)]   Loss: 0.503765
Tue Apr 21 18:22:58 2020[0]<stdout>:Train Epoch: 1 [8960/20000 (45%)]   Loss: 0.662145
Tue Apr 21 18:22:58 2020[1]<stdout>:Train Epoch: 1 [8960/20000 (45%)]   Loss: 0.585488
Tue Apr 21 18:22:59 2020[1]<stdout>:Train Epoch: 1 [9600/20000 (48%)]   Loss: 0.877532
Tue Apr 21 18:22:59 2020[0]<stdout>:Train Epoch: 1 [9600/20000 (48%)]   Loss: 0.920247
Tue Apr 21 18:22:59 2020[2]<stdout>:Train Epoch: 1 [9600/20000 (48%)]   Loss: 0.769557
Tue Apr 21 18:22:59 2020[2]<stdout>:Train Epoch: 1 [10240/20000 (51%)]  Loss: 0.723089
Tue Apr 21 18:22:59 2020[1]<stdout>:Train Epoch: 1 [10240/20000 (51%)]  Loss: 0.557535
Tue Apr 21 18:22:59 2020[0]<stdout>:Train Epoch: 1 [10240/20000 (51%)]  Loss: 0.753275
Tue Apr 21 18:22:59 2020[2]<stdout>:Train Epoch: 1 [10880/20000 (54%)]  Loss: 0.616381
Tue Apr 21 18:22:59 2020[0]<stdout>:Train Epoch: 1 [10880/20000 (54%)]  Loss: 0.519525
Tue Apr 21 18:22:59 2020[1]<stdout>:Train Epoch: 1 [10880/20000 (54%)]  Loss: 0.488052
Tue Apr 21 18:22:59 2020[0]<stdout>:Train Epoch: 1 [11520/20000 (58%)]  Loss: 0.664815
Tue Apr 21 18:22:59 2020[2]<stdout>:Train Epoch: 1 [11520/20000 (58%)]  Loss: 0.539390
Tue Apr 21 18:22:59 2020[1]<stdout>:Train Epoch: 1 [11520/20000 (58%)]  Loss: 0.860400
Tue Apr 21 18:22:59 2020[0]<stdout>:Train Epoch: 1 [12160/20000 (61%)]  Loss: 0.939230
Tue Apr 21 18:22:59 2020[1]<stdout>:Train Epoch: 1 [12160/20000 (61%)]  Loss: 0.628644
Tue Apr 21 18:22:59 2020[2]<stdout>:Train Epoch: 1 [12160/20000 (61%)]  Loss: 0.589586
Tue Apr 21 18:22:59 2020[0]<stdout>:Train Epoch: 1 [12800/20000 (64%)]  Loss: 0.512697
Tue Apr 21 18:22:59 2020[2]<stdout>:Train Epoch: 1
```

## 5. jupyter远程操作
建设中