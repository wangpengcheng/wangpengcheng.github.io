---
layout:     post
title:      UNIX网络编程 学习笔记 (一)
subtitle:   UNIX网络编程 学习笔记 (一) 
date:       2019-11-10
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - C/C++
    - UNIX
    - 网络编程
---

# UNIX网络编程 学习笔记
_参考链接：_

- [《Unix网络编程》卷1 初级](https://blog.csdn.net/zzxiaozhao/article/details/102637708)

# UNIX网络编程 学习笔记 (一)--简介和TCP/IP

_参考链接：_



## 第 0 章 网络基础知识

## 第1章 简介

网络应用系统主要构成有两部分：客户端(client)和服务器(server)。

举例来说：web服务器程序时一个长时间运行的守护程序，web客户与服务器之间使用TCP通信，TCP转而使用IP通信，IP通过以太网驱动程序的数据链路层通信。

![客户端与服务器使用TCP在同一个以太网中通信](https://img-blog.csdnimg.cn/20191019134039155.png#pic_enter)

一般是使用的IPv4(32位，4字节)或者IPv6(128位,16字节)；

**客户端和服务器通常是用户进程，而TCP和IP协议通常是内核中"协议栈"的一部分。**

网络分为：
- LAN：局域网(内网)
- WAN：广域网(外网)

路由器是广域网的架构设备。当下最大的广域网是因特网internet。

![不同局域网的客户机和主机](https://img-blog.csdnimg.cn/20191019134242600.png#pic_center)

首先设置UNP相关的头文件如下：
```c++
/* unp.h */
#ifndef __UNP_H__
#define __UNP_H__
 
#include <sys/types.h>	    /* basic system data types */
#include <sys/socket.h>	    /* basic socket definitions */
#include <sys/time.h>	    /* timeval{} for select() */
#include <time.h>		    /* timespec{} for pselect() */
#include <netinet/in.h>	    /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h>	    /* inet(3) functions */
#include <errno.h>
#include <fcntl.h>		    /* for nonblocking */
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>	    /* for S_xxx file mode constants */
#include <sys/uio.h>		/* for iovec{} and readv/writev */
#include <unistd.h>
#include <sys/wait.h>
#include <sys/un.h>		    /* for Unix domain sockets */
 
#endif //__UNP_H__
```

下面是一个简单的服务器客户端程序
```c++
/* server.h */
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE 4096

#define LISTENQ 1024

//#define SA struct sockaddr
typedef struct sockaddr SA;
int main(int argc, char *argv[]){
    /*定义监听文件符和链接文件符*/
    int listenfd,connfd;
    /*定义网络地址结构体*/
    struct sockaddr_in servaddr;
    /*定义缓冲buffer*/
    char buff[MAXLINE];
    //使用计时时钟
    time_t ticks;
    //创建一个TCP的IPv4网络链接;SOCK_DGRAM表示UDP
    listenfd=socket(AF_INET,SOCK_STREAM,0);
    //初始化网络地址结构体
    bzero(&servaddr,sizeof(servaddr));
    //初始化参数
    servaddr.sin_family=AF_INET;/* 设置网络协议 */
    /* 设置IP地址，需要进行网络序的转换，INADDR_ANY表示127.0.0.1 本地地址 */
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    /*设置端口号1300,注意字节序转换*/
    servaddr.sin_port=htons(1300);
    /* bind将socket和servaddr链接起来，表示监听端口 */
    bind(listenfd,(SA*)&servaddr,sizeof(servaddr));
    /* 调用listen 函数将套接字转化为监听套接字 */
    listen(listenfd,LISTENQ);
    // 接受服务器链接,发送应答
     //针对客户端接受的链接套接字,注意下面的代码到accept之后才能执行
	connfd = accept(listenfd, (SA *) NULL, NULL);
	for ( ; ; ) {
        //获取当前时间
        ticks = time(NULL);
        //将当前时间写入buff
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        //将buff写入到文件连接符中
        write(connfd, buff, strlen(buff));
        sleep(1);//睡眠1s
	}
    //关闭连接符
	close(connfd);
    return 0;
}
```
客户端相关代码

```c++
/* client */
#include "unp.h"
#define MAXLINE 1024
//#define SA struct sockaddr
typedef struct sockaddr SA;
 
int main(int argc, char **argv)
{
    /* 创建套接字文件描述符 */
    int sockfd, n;
    /* 定义缓冲区 */
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;
    if (argc != 2)
        printf ("usage: a.out <IPaddress>\n");
    /*
    使用socket创建一个网际(AF_INET)字节流(SOCK_STEREAM)套接字,
    返回类型为整数类型描述符, 后面的函数调用(如 connect, read等)就使用该描述符来标识此套接字
    */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        printf ("socket error\n");
    /*
     把IP和Port填入一个网际套接字地址结构(名为 servaddr和sockaddr_in的结构变量)
    */
    /* 1. 使用bzero将结构体清空 */ 
    bzero(&servaddr, sizeof(servaddr));
    /* 2. 置地址族为 AF_INET */
    servaddr.sin_family = AF_INET;  
    /* 3. 置位端口 */
    servaddr.sin_port   = htons(1300);
    /* 4. 置位IP */
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        printf ("inet_pton error for %s\n", argv[1]);
    /* 建立和服务器的链接 */
    if(connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
        printf ("connect error\n");
    /* 读入并输出服务器应答 */
    /* 使用read函数读取服务器应答,使用标准输出 fputs输出结构 */
    while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
        /* null terminate */
        recvline[n] = 0;
        if(fputs(recvline, stdout) == EOF)
            printf ("fputs error\n");
    }
    if (n < 0)
        printf ("read error\n");

    //关闭socket连接
    close(sockfd);
    return 0;
}
```
注意：

- **调用sprintf无法检查目的缓冲区是否溢出，相反，snprintf要求其第二个参数指定目的缓冲区的大小，因此可以确保该缓冲区不溢出。**
- 许多网络入侵是由黑客通过发送数据，导致服务器对sprintf的调用使其缓冲区溢出而发生的，**必须小心使用的函数还有gets/strcat/strcpy**，通常应分别改为调用fgets/strncat/strncpy，更好的替代函数还有strlcat/strlcpy可以确保结果是正确终止的字符串。

OSI模型 open systems interconnection(全称：计算机通信开放系统互连模型。)
- 分层：
    - 物理层/数据链路层：主要是设备驱动和网络硬件，通常我们不必关心。
    - 网络层：由IPv4和IPv6这两个协议处理。详细在附录A中。
    - 传输层：即本书所讲的套接字编程接口，从应用层(上3层)进入传输层的接口。
    - 应用层/会话层/表示层：统称为应用层，如web客户端(浏览器)、telnet客户端、web服务器、FTP服务器等。
- 结构图

![结构图](https://img-blog.csdnimg.cn/20191019140405326.png#pic_center)

### 1.2 网络相关调试命令

#### 1.2.1 网络细节的三个基本命令：`netstar/ifconfig/ping`

- netstat
    - netstat -ni // 提供网络接口信息，-n输出数值地址而不是反向解析为名字
    - $ netstat -ni

```sh
    Kernel Interface table
Iface   MTU Met   RX-OK RX-ERR RX-DRP RX-OVR    TX-OK TX-ERR TX-DRP TX-OVR Flg
eth0       1500 0     15459      0      0 0         10444      0      0      0 BMRU
lo        16436 0       138      0      0 0           138      0      0      0 LRU
// lo 环回接口
// eth0 以太网接口
```
-  netstat -nr // 展示路由表信息，另一种确定接口的方法

```sh
内核 IP 路由表
Destination     Gateway         Genmask         Flags   MSS Window  irtt Iface
0.0.0.0         192.168.31.1    0.0.0.0         UG        0 0          0 eth0
169.254.0.0     0.0.0.0         255.255.0.0     U         0 0          0 eth0
192.168.31.0    0.0.0.0         255.255.255.0   U         0 0          0 eth0

```


- ifconfig
    - ifconfig eth0 // 获得eth0以太网接口的详细信


```sh
ifconfig eth0

eth0      Link encap:以太网  硬件地址 00:0c:29:55:a0:99
          inet 地址:192.168.31.205  广播:192.168.31.255  掩码:255.255.255.0
          inet6 地址: fe80::20c:29ff:fe55:a099/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  跃点数:1
          接收数据包:15624 错误:0 丢弃:0 过载:0 帧数:0
          发送数据包:10571 错误:0 丢弃:0 过载:0 载波:0
          碰撞:0 发送队列长度:1000
          接收字节:1468669 (1.4 MB)  发送字节:1070042 (1.0 MB)
          中断:19 基本地址:0x2000
// MULTICAST 标志通常指明该接口所在主机支持多播。
```

- ping // 测试ip地址是否联通当前以太网络

```sh
$ ping -b 192.168.31.255

PING 192.168.31.255 (192.168.31.255) 56(84) bytes of data.
64 bytes from 192.168.31.255: icmp_req=1 ttl=64 time=0.253 ms
64 bytes from 192.168.31.255: icmp_req=2 ttl=64 time=0.022 ms
64 bytes from 192.168.31.255: icmp_req=3 ttl=64 time=0.029 ms
```

注意：64位体系结构的趋势原因之一是：在每个进程内部可以由此使用更长的编址长度(即64位指针)，从而可以寻址更大的内存空间(超过2^32字节)。



## 传输层：TCP、UDP和SCTP

下面是TCP/IP协议族的概况

![TCP/IP协议族的概况](https://wangpengcheng.github.io/img/2019-11-21-16-02-21.png)

相关协议:
- TCP：传输控制协议，面相连接，全双工字节流。流套接字。关心：确认、超时、重传等细节。
- UDP：用户数据报协议，无连接协议。数据报套接字。不保证最终达到目的地。
- ICMP：网际控制消息协议，处理路由器和主机之间流通的错误和控制消息。
- ARP：地址解析协议，把IPv4地址映射成一个硬件地址(数据链路层)。
- RARP：反地址解析协议，把一个硬件地址映射成一个IPv4地址(数据链路层)。
- SCTP：流控制传输协议，提供可靠的全双工关联的面向链接的协议。
- ICMP:网际控制消息协议。主要处理路由器和主机之间的流通的错误和控制消息。
- IGMP:网际组管理协议。主要用于多播。


### 2.6 TCP的建立和终止

#### 2.6.1 三次握手

1. 服务器准备好接受外来的连接--socket、bind和listen这三个函数来完成，即被动打开。
2. 客户端调用connect发起主动打开(active open).客户端发送SYN(同步)分节。SYN通常不携带数据，其所在IP数据报只含有一个IP首部、一个TCP首部以及可能有的TCP选项
3. 服务器确认(ACK)客户端的SYN,同时自己发送一个SYN分节，含有服务器将在同一连接中发送的数据的初始序列好。服务器在单个分节中发送SYN和对客户端SYN的ACK(确认)
4. 客户端必须确认服务器的SYN

![TCP三次握手](https://wangpengcheng.github.io/img/2019-11-20-20-11-32.png)

下面是TCP的四次挥手

![TCP四次挥手](https://wangpengcheng.github.io/img/2019-11-21-22-07-01.png)

TCP链接相关状态图如下;

![TCP状态转换图](https://wangpengcheng.github.io/img/2019-11-22-17-06-18.png)

**TCP的分组交换**

![TCP的分组交换](https://wangpengcheng.github.io/img/2019-11-22-17-08-08.png)

### 2.8 SCTP关联的建立和终止

SCTP因为是非双全工通信，因此不需要像TCP一样，需要进行四次挥手(没两次挥手，关闭一边的通道)。但是，采用了独特的校验机制，需要在开始链接时，进行四次握手，互相交换确认号。关联过程如下:

1. 服务器准备好接受外来的关联
2. 客户通过connect或者发送一个隐式打开关联的消息进行主动打开。发送一个INIT消息(初始化)，告诉客户端的IP地址清单，初始序列好和相关分组的起始标记、客户请求的外出流的数目以及客户能够支持的外来流的数目。
3. 服务器发送INIT ACK消息来确认客户端的INIT消息，其中还有服务器的IP地址清单、初始序列号、起始标记。同时会激励一个状态的cookie，用于确信本关联有效所需要的所有状态。
4. 客户端结构服务器消息，并以一个COOKIE ECHO消息回射服务器的状态cookie.同时在分组中捆绑了用户数据
5. 服务器以一个COOKIE ACK消息确认客户回射的cookie是正确的，本关联于是建立。该消息也可能在同一个分组中还捆绑了用户数据。

![SCTP四路握手](https://wangpengcheng.github.io/img/2019-11-22-20-10-05.png)

注意：**SCTP使用四路握手，是为了避免拒绝服务共计**。SCTP中cookie状态提供了一个任意长度的字段，并且要求实施基于加密的安全性以防护攻击，TCP中cookie编码只有32位长的初始序列号中。

#### 2.8.2 关联终止

因为SCTP没有类似与TCP的TIME_WAIT状态，而是使用了验证标记，所有后续块都在捆绑它们的SCTP分组的公共首部标记了初始的INIT块和INIT ACK块中作为起始标记交换的研究表示，由来自旧链接的块通过所在SCTP分组的公共首部间接携带的验证标记对于新连接来说是不正确的。因此，避免了使用TIME_WAIT状态来保持整个连接的做法。

其状态转换图如下：

![SCTP状态转换图](https://wangpengcheng.github.io/img/2019-11-22-20-17-44.png)

分组连接状态如下：

![SCTP分组连接](https://wangpengcheng.github.io/img/2019-11-22-20-19-13.png)

当多个客户端连接服务器的相同端口时，会产生多个套接字

![多个客户端的连接](https://wangpengcheng.github.io/img/2019-11-22-20-22-37.png)

### 2.11 缓冲区大小及限制

- IPv4:最大大小是65535字节，包括IPv4首部；因为总长度字段，只有16位。
![IPv4首部](https://wangpengcheng.github.io/img/2019-11-22-20-25-58.png)
- IPv6:是65575字节，包括40字节的IPv6首部。净荷长度占据16位(不包括IPv6首部)。特大净荷可以将净荷字段扩展到32位，但是需要MTU(最大传输单元)超过65535的数据链路提供支持。
![IPv6首部](https://wangpengcheng.github.io/img/2019-11-22-20-26-42.png)
- MTU:网络硬件规定，以太网的MTU是1500字节。IPv4最小为68字节，IPv6最小为1280字节。
- 最小重组缓冲区：IPv4或IPv6的任何实现都必须保证支持的最小数据报大小；IPv4-576;IPv6-1500字节。
- MSS(最大分节大小):TCP中通告对端在每个分节中能发送的最大TCP数据量；从而避免分片；其经常设置为MTU减去IP和TCP首部的固定长度。MSS值是一个16位的字段，限定 其最大值为65535.但是在IPv6中MSS基本无效。
- SCTP到对端所有地址发现的最小路径MTU保持一个分片点。这个最小MTU大小用于把较大的用户消息分割成较小的能够以按个IP数据报发送的若干片段。

TCP输出步奏如下：

![TCP套接字步奏](https://wangpengcheng.github.io/img/2019-11-22-21-01-47.png)

UDP输出步奏如下：

![UDP输出步奏](https://wangpengcheng.github.io/img/2019-11-22-21-02-50.png)

SCTP输出如下：

![SCTP输出](https://wangpengcheng.github.io/img/2019-11-22-21-03-31.png)

### 2.13 常见的因特网应用的协议使用

![常见网络协议](https://wangpengcheng.github.io/img/2019-11-22-21-05-11.png)

# 第二部分 基本套接字

## 第三章 套接字编程简介

几乎每一个例子都用到了套接字地址结构. 这些结构可以在两个方向上传递: 从 进程到内核 和 内核到进程 .

地址转换函数在**地址的文本表达** 和他们存放在**套接字地址结构中的二进制之间**进行转换
- inet_addr/inet_ntoa 适用于IPv4
- inet_npton/inet_ntop 适用于IPv4/IPv6

### 3.2 套接字的地址结构

套接字地址结构在给定主机上使用:**虽然其定义了某些字段用于不同主机间进行通信,但是结构体本身并不在主机间进行传递**

#### 3.2.1 IPv4套接字地址结构

其地址结构在`<netinet/in.h>`中定义如下：
```c
// IPv4套接字地址结构：sockaddr_in
#include <netinet/in.h>

struct in_addr {
    in_addr_t       s_addr;         /* 32bit IPv4 address. */
};

struct sockaddr_in {
    uint8_t         sin_len;        /* length of structure (16) */
    sa_family_t     sin_family;     /* AF_INET */
    in_port_t       sin_port;       /* 16bit TCP/UDP port number */
    struct in_addr  sin_addr;       /* 32bit IPv4 address */
    char            sin_zero[8];    /* unused */ 
};

```
**注意：in_zero 未使用,我们应该把它置位0,按照惯例我们应该先把结构体置位0,而不是单单把sin_zero置位0**

![POSIX规范](https://wangpengcheng.github.io/img/2019-11-22-21-17-20.png)

#### 3.2.2 Pv6套接字地址结构: `sockaddr_in6`

```c
//IPv6套接字地址结构：sockaddr_in6
#include <netinet/in.h>

struct in6_addr {
    uint8_t         s6_addr[16];    /* 128bit IPv6 address */
};

#define SIN6_LEN
struct sockaddr_in6 {
    uint8_t         sin6_len;       /* length of structure (28) */
    sa_family_t     sin6_family;    /* AF_INET6 */
    in_port_t       sin6_port;      /* transport layer port */
    uint32_t        sin6_flowinfo;  /* flow information, undefined */
    struct in6_addr sin6_addr;      /* IPv6 address */
    uint32_t        sin6_scope_id;  /* set of interfaces for a scope */
};

```
- sin6_flowinfo字段分为:
    - 低序20位是流标(flow label)
    - 高序12位保留

#### 3.2.3 通用套接字地址结构`sockaddr`

使用通用结构，降低IPv4和IPv6操作方法的耦合。套接字函数被定义为以指向某个**通用套接字地址结构**的指针作为其参数之一,例如bind函数：`int bind(int, struct sockaddr*, socklen_t)`(注意第二个参数为通用套接字地址结构)

因此对这些函数的调用都必须把指向特定协议的地址结构进行强制转换。如：
```c
bind(sockfd, (struct sockaddr*) &serv, sizeif(serv));
```
通用结构体内容如下：
```c
// 通用套接字地址结构：sockaddr
#include <sys/socket.h>
struct sockaddr {
	uint8_t         sa_len;         /* length of structure */
	sa_family_t     sa_family;      /* address family: AF_XXXX value */
	char            sa_data[14];    /* protocol-specific address */
};
```
- 新的通用套接字地址结构: `sockaddr_storage`
	- 特点:
		- 所过系统支持得任何套接字地址结构有对齐需求,那`sockaddr_storage`满足最严格得对齐需求
		- `sockaddr_storage`足够大,注意容纳系统支持得任何套接字地址结构
		- 出了`ss_familly`和`ss_len`外`sockaddr_storage`结构中得其他字段对用户都是透明得

其地址结构如下
```c
#include <netinet/in.h>
struct sockaddr_strorage {
    uint8_t         ss_len;         /* length of structure */
    sa_family_t     ss_family;      /* address family: AF_XXXX value */
};
```
相关套接字比较参数如下：
![套接字结构比较](https://img-blog.csdnimg.cn/20191019144530405.png#pic_center)

### 3.3 值结果参数

内核和进程之间的相互复制

- 从进程到内核传递套接字地址结构的函数有3个:bind,connect, sendto,这些函数的参数是指向某个套接字地址结构的指针,另一个参数是该结构的整数大小,既然指针和指针所指的内容的大小都传递给了内核,于是内核知道需要从进程复制多少数据
- 从内核到进程传递套接字地址结构的函数有4个:`accept`,`recvfrom`,`getsockname`,`getpeername`,这四个函数的其中两个参数是指向某套接字地址结构的指针,和指向该套接字地址结构的内容的大小(**是两个指针,分别指向套接字地址结构和套接字地址结构的大小**)

当函数被调用时,结构大小是一个值(此值告诉内核该结构的大小,使内核在写此结构时不至于越界),当函数返回时,结构大小又是一个结果(它告诉进程内核在此结构中确切存储了多少信息),这种参数类型叫值结果参数。

![内核到进程的套接字地址传递](https://img-blog.csdnimg.cn/20191021134248765.png#pic_center)

### 3.4 字节排序函数

关于大小端：

- 大端：高字节放低地址，如0x0102,内存中放的是0x0102
- 小端：高字节放高地址，低字节放低地址如0x0102,内存中放的是0x0201

![大端字节序](https://wangpengcheng.github.io/img/2019-11-22-21-33-36.png)

**网际协议使用大端字节序传送多字节整数**；网络协议中的字节序函数如下：

```c
#include <netinet/in h>
uint16_t htons (uint16t host16bitvalue): //h:host  主机字节序
uint32_t htonl (uint32t host32bitvalue); //n:network 网络字节序
uint16_t ntohs (uint16t net16bitvalue);  //s:short
uint32_t ntohl (uint32t net32bitvalue);  //l:long
```
### 3.5 字节操纵函数

操作函数有下列两种：

- str开头：处理C字符串(即以\0结尾)
- b开头：起源与4.2BSD,这里给出源自Berkeley的函数：
    - `void bzero(void *dest, size_t nbytes)`：把目标字节串中指定数目的字节置为0
    - `void bcopy(const void *src, void *dest, size_t nbytes)`：将指定数目的字节从源字节串移到目标字节串
    - `int bcmp(const void *ptrl, const void *ptr2, size_t nbytes)`：比较两个任意的字节串，若相同返回0，不同返回非0
```c
#include <strings.h>
void bzero(void *dest, size_t nbytes);
void bcopy(const void *src, void *dest, size_t nbytes);
int bcmp(const void *ptrl, const void *ptr2, size_t nbytes);
```
- m系列：以mem开头，是ANSI C标准中的字节操作函数，c中都使用：
    - `void *memset(void *dest, int c, size_t len)`:把目标字符串指定数目的字节置为c
    - `void *memcpy(void *dest, const void *src, size_t nbytes)`:类似bcopy，不过两个指针参数的顺序相反。**当源字节串与目标字节串重叠时，bcopy能够正确处理，但是memcpy的操作结果却不可知**。这种情形下必须该用ANSI C的memmove函数。
    - `int memcmp(const void *ptr1, const void *ptr2, size_t nbytes)`：比较两个任意的字节串，若相同返回0，否则返回一个非0值。
```c
#include <string.h>
void *memset(void *dest, int c, size_t len);
void *memcpy(void *dest, const void *src, size_t nbytes);
int memcmp(const void *ptr1, const void *ptr2, size_t nbytes);
```

### 3.6 inet_aton和inet_addr和inet_ntoa函数
他们都是将十进制数串("206.168.112.96")IPv4地址转换为32位网络字节序二进制
```c
#include <arpa/inet.h>
/* 有效返回1，否则为0 */
int inet_aton(const char *strptr,struct in_addr *addrptr);
/* 返回有效地址，否则返回 INADDR_NONE--已废弃 */
in_addr_t inet_addr(const char *strptr);
/* 将二进制地址转化为10进制字符串 */
char *inet_ntoa(struct in_addr inaddr);
```
### 3.7 inet_pton和inet_ntop函数(推荐使用)

这个两个函数的作用是实现ip地址点分十进制格式和二进制格式的相互转换。`p`指表达式, `n`指数值,地址的表达格式通常是ASCII字符串,而在套接字地址结构中这是二进制值。

- `int inet_pton(int family, const char *strptr, void *addrptr)`: 尝试转换由strptr指针所指的字符串，并通过addrptr指针存放二进制结果。
    - `family:AF_INET或AF_INET6`
    - 返回：1（成功） 0（不是有效格式） -1（出错）
- `onst char *inet_ntop(int family, const void *addrptr, char *strptr, size_t len)`：**inet_ntop进行相反的转换，从数值格式（addrptr）转换到表达格式（strptr）。len参数是目标存储单元的大小，以免该函数溢出其调用者的缓冲区。**
    - `family:AF_INET或AF_INET6`
    - 指定`size_t`防止溢出：
        - `#define INET_ADDRSTRLEN 16`
        - `#define INET6_ADDRSTRLEN 46`
    - 返回：结果指针（成功） NULL（失败）
```c
#include <arpa/inet.h>
int inet_pton(int family, const char *strptr, void *addrptr);
//返回：1（成功） 0（不是有效格式） -1（出错）
const char *inet_ntop(int family, const void *addrptr, char *strptr, size_t len);
//返回：结果指针（成功） NULL（失败）
```

![相关地址转换](https://wangpengcheng.github.io/img/2019-11-22-21-54-12.png)

### 3.8 sock_notop和相关函数

```c
// IPv4
struct sockaddr_in addr;
inet_ntop(AF_INET, &addr.sin_addr, str, sizeof(str));
// IPv6
struct sockaddr_in6 addr6;
inet_ntop(AF_INET6, &addr6.sin_addr, str, sizeof(str));
```
inet_ntop需要知道地址族, 地址结构中的二进制地址指针;以上过程还是比较繁琐的,为了简便起见,我们定义一个`char *sock_ntop(const struct sockaddr *sockaddr, socklen_t addrlen)`函数, 它的参数为:
- **套接字地址结构指针**
- **套接字地址结构的长度**

关键使用如下：
```c
/* include sock_ntop */
char *sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
    char        portstr[8];
    static char str[128];       /* Unix domain is largest */

    switch (sa->sa_family) {
    case AF_INET: {
        //将指针转换为IPv4地址结构
        struct sockaddr_in  *sin = (struct sockaddr_in *) sa;
        //地址转换：成功则返回c字符串形式的IP地址，str指定转换格式
        if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
            return(NULL);
        //字节排序：网络转换为主机的字节序
        if (ntohs(sin->sin_port) != 0) {
            snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
            strcat(str, portstr);
        }
        return(portstr);
}

```

### 3.9 readn、writen和readline 函数

字节流套接字(如TCP套接字)上的read和write函数所表现的行为不同于通常的文件I/O.字节流套接字上调用的read和write输入和输出的字节数可能比请求，的数量少,这并不是出错. **原因是内核中用于套接字的缓冲区可能已经达到极限了.**此时需要调用者再次调用read和write函数，对剩余的进行操作。
为了以防万一, 不让实现返回不足的字节,使用writen和readn来改进函数。

- `ssize_t readn(int filedes, void *buff, size_t nbytes)`:返回: 读入字节数, 出错返回1
- `ssize_t written(int filedes, void *buff, size_t nbytes)`:返回: 写入字节数, 出错返回1
- `ssize_t readline(int filedes, void *buff, size_t maxlen)`:反回,读入字节数, 出错返回1。**注意：`readline`非常低效,其每读一个字节 就调用依次系统read函数**

readn的基本实现如下：

```c
ssize_t                     /* Read "n" bytes from a descriptor. */
readn(int fd, void *vptr, size_t n)
{
    size_t  nleft;
    ssize_t nread;
    char    *ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        //如果读取失败
        if ( (nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR) /* 查找EINTR错误，表示系统被一个捕获信号中断 */
                nread = 0;      /* and call read() again */
            else
                return(-1);
        //如果读成功了    
        } else if (nread == 0)
            break;              /* EOF */
        //计算漏读的字节数，再读文件
        nleft -= nread;
        ptr   += nread;
    }
    return(n - nleft);      /* return >= 0 */
}
/* end readn */
```
stdio缓冲状态是不可见的，不能被用于代替readline.基于文本行的网络协议(如SMTP,HTTP,FTP控制链接协议)；淫荡尽量依照缓冲区而不是文本行的要求来考虑编程。下面定义一个简单的readline函数改进;主要是使用

```c++
#include "unp.h"
/* 定义读取次数 */
static int read_cnt;
static char *read_ptr;
/* 定义读取buf */
static char read_buf[MAXLINE];
/* 读取函数 */
static ssize_t my_read(int fd,char *ptr)
{
    if(read_cnt<=0){
        again:
            if((read_cnt==read(fd,read_buf,sizeof(read_buf)))<0){/* 进行一次读取 */
                if(errno==EINTER)
                    goto again;
                return (-1);
            }else if(read_cnt==0){
                return 0；
            }
            read_ptr=read_buf;
    }
    read_cnt--;
    *ptr=*read+ptr++;/* 重置空指针 */
    return (1);
}

ssize_t readline(int fd,void *vptr,size_t maxlen)
{
    ssize_t n,rc;
    char c,*ptr;
    ptr=vptr;
    for(n=1;n<maxlen;n++){
        if((rc=my_read(fd,&c)==1)){
            *ptr++=c;/*将字符指针指向c*/
            if(c=='\n') break;/* 检测到新行 */
        }else if(rc==0){
            *ptr=0;
            return (-1); /* 读取到EOF结束符，直接返回 */
        }else{
            return (-1); /* 读取错误 */
        }
    }
    /*重置指针*/
    *ptr=0;
    /* 返回大小 */
    return (n);
}
/* 从缓冲取中读取数据 */
ssize_t readlinebuf(void **vptrptr)
{
    if(read_cnt)
        *vptrptr=read_ptr;
    return (read_cnt);
}
/* 简单来说就是将读取一个字符，转变为了读取多个字符 */
```
### 第 4 章 基本TCP套接字编程

TCP连接过程中基本使用如下：

![TCP服务其套接字过程](https://wangpengcheng.github.io/img/2019-11-23-15-32-46.png)

![套接字类型](https://wangpengcheng.github.io/img/2019-11-23-20-59-19.png)

![type值](https://wangpengcheng.github.io/img/2019-11-23-21-00-02.png)

![组合值](https://wangpengcheng.github.io/img/2019-11-23-21-00-39.png)

### 4.2 socket

`int socket(int framily, int type, int protocal)`:
- `framily`参数表明协议族(协议域),
- `type`参数表示套接字类型
- `protocal`表示协议类型(或则设置为0)
- 并不是所有的`framily`和`type`的组合都是有效的
- `AF_`前缀表示地址族,`PF_`前缀表示协议族
- socket函数的返回值为一个非负整数(套接字描述符, sockfd),**套接字描述符知识制定了协议族和套接字类型,并没有指定本地协议或则远程协议**


### 4.3 connect 函数

`int connect(int sockfd, const struct sockaddr* servaddr, socklen_t addrlen);`
- `sockfd`:套接字描述符
- 第二三个参数表示一个套接字地址结构(内部有服务器IP+Port)
- 客户端在调用connect前不一定需要调用bind函数，因为如果需要的话，内核会确定源IP地址，并选择一个临时端口作为源端口
- **出错的情况:**
  - TCP客户没有收到SYN分节的响应，返回`ETIMEOUT`错误，如往本地子网上一个不存在的IP发送SYN
  - 硬错误：收到RST(表示复位)，该服务器主机在指定的端口上，没有进程在等待与之链接(服务器没有运行)。客户端收到RST立刻返回`ECONNREFUSED`错误。产生RST的可能条件如下：
    - 目的地为某端口的SYN到达，然而端口上没有正在监听的服务器；
    - TCP想取消一个已有连接；
    - TCP接收到一个根本不存在的连接上的分节.
  - 软错误: 发送SYN分节引发路由器“destination unreachable”(目的地不可达)；ICMP错误。

###  4.4 bind函数

本地洗衣地址赋予一个套接字，对于网际协议来说，是将IP地址与TCP或UDP的端口号进行组合。

```c
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr *myaddr, socklen_t addrlen);
//成功返回0，出错返回-1
```

- 服务器在启动时，若未绑定端口;内核会选择一个临时的端口(客户端)。服务器一般不会。但是在RPC服务器中会监听端口，创建自己的临时端口。
- TCP客户端一般不会绑定端口，因为这样会限定该套接字只能从指定端口发送。一般内核会根据目睹IP地址和端口，选择源IP地址。
- 常见错误“address already in use”
![指定端口关系](https://wangpengcheng.github.io/img/2019-11-23-21-58-41.png)

### 4.5 listen 函数

socket被创建时，默认为一个主动套接字，主要使用connect发起连接的客户端套接字。listen函数把一个未连接的套接字转换为一个被动套接字。
```c
#include <sys/socket.h>
int listen(int sockfd, int backlog);
//成功返回0，出错返回-1

```

函数应该在socket和bind之后，accept函数之前。backlog表示正在连接状态和完全连接状态的队列的最大数目值。
监听套接字维护两个队列：未完成连接队列（SYN_RCVD)和已完成连接队列(ESTABLISHED)；backlog要求这两个队列之和不超过它。

**当一个客户SYN到达时，若这些队列是满的，TCP就忽略该分节，不发送RST**
![监听队列](https://wangpengcheng.github.io/img/2019-11-23-22-08-35.png)

![关系](https://wangpengcheng.github.io/img/2019-11-23-22-11-04.png)

### 4.6 accept函数

accept拥有两个值-结果参数，cliaddr和addrlen可以返回peer端信息，如果不关心，可以置NULL。

accept主要是从已完成连接队列头返回下一个已完成链接。如果以完成为空，则进行睡眠。

accept成功时会返回一个自动生成的全新描述符，代表客户之间的TCP连接，输入为监听套接字。返回为已连接套接字

```c
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr * cliaddr, socklen_t *addrlen);
//成功返回非负描述符号，出错返回-1
```

### 4.7 fork和exec函数

fork函数调用一次返回两次，一次返回新派生进程(子进程)的进程ID号，子进程又返回一次，返回值为0，，告知当前进程为子进程。

父进程中调用fork之前打开的所有描述符在fork返回之后由子进程分享。网络服务器利用了这个特性。accept之后调用fork。已连接的套接字就在两个进程之间共享；通常情况，父进程会关闭这个已连接套接字。子进程则继续进行。

还可以使用exec函数进行额外的操作，但是这个操作会直接将当前进程的工作内容切换，并不会有返回值。

![获取相关操作](https://wangpengcheng.github.io/img/2019-11-25-15-27-26.png)

### 4.8 并发服务器

并发服务器中使用fork操作，来进行多个客户端的并发处理。同时也要求，在使用close时不仅父进程要关闭，子进程也要关闭socket，对应的文件描述符引用为0，才能真正结束网络链接。

![](https://wangpengcheng.github.io/img/2019-11-25-15-36-57.png)

![](https://wangpengcheng.github.io/img/2019-11-25-15-37-28.png)

所以需要并发服务器的相关操作伪代码如下：
```c
/* 伪代码 */
pid_t pid;
int   listenfd, connfd;
listenfd = socket (...);
bind (listenfd, ...);
listen (listenfd, LISTENQ);
for (; ; ) {
    connfd = accept (listenfd, ...);
    if ((pid = fork()) == 0) {
        close (listenfd); /* child closes listening socket */
        /* do something */
        close (connfd);   /* done with this client */
        exit (0);
    }
    close (connfd);       /* parent closes connected socket */
}
```

### 4.9 close()函数

- `int close(sockfd);`：可以用来关闭套接字，并终止TCP连接
- 确实想终止连接可以用`shutdown()`函数。

### 4.10 getsocketname和getpeername函数

相关函数操作如下：
```c
#include <sys/socket.h>
int getsockname (int sockfd, struct sockaddr *localaddr, socklen_t *addrlen);
int getpeername (int sockfd, struct sockaddr *peeraddr, socklen_t *addrlen);
```
使用解释：

- 在一个没有调用`bind`的TCP客户端上，`connect`成功返回后，`getsockname`用于返回由内核赋予该连接的本地IP地址和本地端口号；
- 在以端口号0调用`bind`后，`getsockname`用于返回由内核赋予的本地端口号；
- `getsockname`可用于获取某个套接字的地址族。
- 当一个服务器是由调用过accept的某个进程通过调用`exec`执行程序时，它能够获取客户身份的唯一途径便是调用`getpeername`。
- 大多数TCP服务器是并发的，大多数UDP服务器是迭代的(UDP无链接，每个应用只需要保持一个线程)。

使用示例：

```c
/* 代码演示：获取套接字的地址族 */
int sockfd_to_family(int sockfd)
{
	struct sockaddr_storage ss;
	socklen_t	len;
 
	len = sizeof(ss);
	if (getsockname(sockfd, (SA *) &ss, &len) < 0)
		return(-1);
	return(ss.ss_family);
}
```

### 第 5 章 TCP客户端/服务器程序示例
_参考链接：_

- [《UNIX网络编程卷1》读书笔记–第五章TCP客户/服务实例](https://blog.csdn.net/mashuiping/article/details/65628979)
- [UNIX网络编程卷一 第五章 TCP客户/服务器程序示例](https://blog.csdn.net/haoyuedangkong_fei/article/details/65448137)


本章开始编写一个完整的TCP客户/服务器程序实例。
- (1) 客户冲标准输入读入一行文本，并写给服务器
- (2）服务器从网络输入读入这行文本，并回射给客户
- (3）客户从网络读入这行回射文本，并显示在标准输出上。

![服务器相关操作](https://img-blog.csdnimg.cn/20191023150943597.png)

一个简单的client和sever
```c++
/* client */
#include "unp.h"

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_inservaddr;
	
	if (argc != 2)
	err_quit("usage: tcpcli <IPaddress>");
	
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	
	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
	str_cli(stdin, sockfd);/* do it all */
	exit(0);
}
void str_cli(FILE *fp, int sockfd)
{
	char sendline[MAXLINE], recvline[MAXLINE];
	while (Fgets(sendline, MAXLINE, fp) != NULL) {
		Writen(sockfd, sendline, strlen(sendline) );
		if (Readline(sockfd, recvline, MAXLINE) == 0)
		err_quit("str_cli: server terminated prematurely");
		Fputs(recvline, stdout);
	}
}
```
**Sever**
```c++
#include "unp.h"
int main(int argc, char **argv) {
	int listenfd, connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_incliaddr, servaddr;
	
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);
	
	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
	Listen(listenfd, LISTENQ);
	
	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
		if ( (childpid = Fork()) == 0) {/* child process */
			Close(listenfd);/* close listening socket */
			str_echo(connfd);/* process the request */
			exit(0);
		}
		Close(connfd);/* parent closes connected socket */
	}
}
void str_echo(int sockfd) {
	ssize_t n;
	char buf[MAXLINE];
	
again:
	while ( (n = read(sockfd, buf, MAXLINE)) > 0)
	Writen(sockfd, buf, n);
	
	if (n < 0 && errno == EINTR)
	goto again;
	else if (n < 0)
	err_sys("str_echo: read error");
}

```
**工作流程**

- 服务端先在后台运行
  - 连接阶段:
    - `socket`创建套接字
    - 调用`bind`设置服务的端口号为9877，任意一个网卡的IP
    - 调用`listen`,将套接字改为被动连接套接字，
    - 维护队列，这一步完成后就可以接收客户的connect了，
    - 调用`accept`，初次调用时并没有已连接的套接字，进入睡眠.
  - 工作阶段:
    - 创建子进程:
      - 将`accept`放在一个无限循环中，
      - `accept`返回成功，就`fork`一个子进程
      - 在子进程中处理已建立连接的任务，父进程就继续等待下一个连接。
    - 子进程工作:
      - 在**子进程中需要关闭socket创建的描述符，父进程中关闭connect返回的描述符**
        - 因为fork创建进程时这两个描述符都会复制到子进程中，如果不关闭，在子进程退出时由于父进程还打开了connect描述符，**将不会发送FIN字节**，而且每一个连接都**会消耗一个描述符资源永远不会释放**。
      - 在`str_echo`中，服务器从套接字中读取内容，若没有内容就阻塞，然后直接写回套接字。
- 客户端：
  - 链接阶段：
    - 创建套接字
    - 设置服务器IP和端口号
    - 调用connect发起连接
      - 调用connect后会发送SYN字节
      - 在收到服务端的ACK后
      - connect就返回，进入established状态
  - 工作阶段
    - 从标准输入中读取一行文本
    - 将它写到套接字中
    - 从套接字中读一行文本
    - 写到标准输出

### 5.6 服务器正常启动

服务器和客户端阻塞后：

- 客户端正常是阻塞在`fgets`，等待用户输入；在用户输入`EOF`后，`fgets`返回`NULL`，`str_cli`退出。
- 客户端程序调用`exit`结束程序，详细流程如下:
  - `exit`首先会先关闭打开的套接字描述符，(客户单套接字`close`)
  - 引发`FIN`发送到套接字中，进入`FIN_WAIT_1`状态，(客户端发送`FIN`)
  - 收到服务器的`ACK`后进入`FIN_WAIT_2`状态，(服务器发送回复:`ACK`)
  - 再收到`FIN`后发送`ACK`然后进入`TIME_WAIT`状态(服务器发送:`FIN`, 客户端回复:`ACK`)
  - 等待`2MSL`
- 客户端程序运行时查看套接字状态
```sh
$ netstat -a |grep 9877
tcp        0      0 *:9877                  *:*                     LISTEN     
tcp        0      0 localhost:36368         localhost:9877          ESTABLISHED
tcp        0      0 localhost:9877          localhost:36368         ESTABLISHED
```
- 客户端程序终止运行后查看套接字状态
```sh
$ netstat -a |grep 9877
tcp        0      0 *:9877                  *:*                     LISTEN     
tcp        0      0 localhost:36368         localhost:9877          TIME_WAIT
```
注意：服务器的意外崩溃，或者主进程的主动结束，可能存在子进程的僵尸进程。

### 5.8 问题分析

#### 5.8.1 僵尸进程

当主动关闭服务器后，使用ps查看进程状态发现存在僵尸进程

```shell
$ ps -o pid,ppid,stat,args
  PID  PPID STAT COMMAND
30143 30142 Ss   -bash
34810 30143 S    ./tcpserv01
34812 34810 Z    [tcpserv01] <defunct>
34813 30143 R+   ps -o pid,ppid,stat,args
```

为了避免产生僵尸进程，应该使用`wait`或者`waitpid`等待子进程结束后，再使用主进程终结。

- 父进程如果设置了信号处理函数那么就可以在信号处理函数中调用wait或waitpid.
- 如果创建的子进程不止一个：
  - 需要在一个循环中调用`waitpid`来处理，并且设置`WNOHANG`参数。
  - 因为一个`wait/waitpid`只处理一个僵尸进程，而且调用`wait`时会挂起，这在信号处理函数中是不妥的。
  - 如果父进程不设置信号处理函数，那么就可以再父进程退出时调用`wait`,或`waitpid`，通常这种情况下父进程都是很快就退出，不然还是会产生僵尸进程。

**让init进程处理僵尸进程**

- 这种情况下存在于：
  - 父进程没有处理`SIGCHLD`信号，或在信号处理函数中没有`waitpid`
  - **且**父进程已经结束后才存在的情况
- 这时init就会成为僵尸进程的父进程，我们就不用管了。**其实这中情况多半是由于父进程忘记处理了**。这里就可以不处理`SIGCHLD`信号，因为这个信号并不会导致程序结束，只要在父进程中close然后调用`wait/waitpid`就好了。

#### 5.8.2 处理被中断的系统调用


为了说明这个问题，我们引入信号处理函数，其实信号处理就相当于一个软件中断，中断随时都可能发生，因此我们编写代码过程中需要考虑中断的情况。

```c
int main(int argc, char **argv){
	int listenfd, connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_incliaddr, servaddr;
	void sig_chld(int signo);
	Sigfunc * Signal(int signo, Sigfunc *func);
	
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
	Listen(listenfd, LISTENQ);
	Signal(SIGCHLD, sig_chld);

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
		
		if ( (childpid = Fork()) == 0) {/* child process */
			Close(listenfd);/* close listening socket */
			str_echo(connfd);/* process the request */
			exit(0);
		}
		Close(connfd);/* parent closes connected socket */
	}
}

void sig_chld(int signo){
	pid_t pid;
	int stat;
	printf("enter sig_chld\n");
	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
	//while( (pid = wait(NULL)) > 0)
	printf("child %d terminated\n", pid);
	printf("quit sig_chld\n");
	return;
}

```

上面例子中中断处理函数中调用printf是不太合适的，因为printf是不可重入函数，在程序规模比较大，进程多时可能出现奇怪错误，这里只为了查看程序状态。

`Signal`是一个书中作者写的一个包裹函数，采用signation函数实现，实现代码中可以设置是否设置SA_RESTART, 这个配置就表示当系统调用被中断以后是否自动重新启动。

因为不同的UNIX系统实现可能不一样，有些系统默认重启有些则默认不重启，因此我们自己配置就可以更好控制，当然也为了不用直接配置`signation`，才将其包装起来。

对于`accept`、`read`、`write`、`select`等慢系统调用通常我们都希望他们被中断之后能继续返回中断前的状态继续执行，因为并不会产生错误，而对于`connect`在中断之后我们则不能重启，因为在中断之后其连接肯定会失败。

### 5.8.3 wait和waitpid

wait接收到任意一个信号之后，就会执行断开操作。但是会留下n-1个僵尸进程。因为所有的信号都在信号处理函数执行之前产生，信号处理函数只执行一次，因为UNIX信号一般是不排队的，正确的解决办法是使用`waitpid`代替wait

```c
void sig_chld(int signo){
	pid_t pid;
	int stat;
	printf("enter sig_chld\n");
	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
	//while( (pid = wait(NULL)) > 0)
	printf("child %d terminated\n", pid);
	printf("quit sig_chld\n");
	return;
}
```

指定WNOHANG参数，使得pid在有尚未终止的子进程运行时不要阻塞。

### 5.11 accept返回前连接终止

三路握手完成建立连接后，客户端TCP却发送了一个RST(复位)。服务器中，该连接已经在TCP队列中。等待accept时，RST到达。相当于，服务器开启socket、bind、listen后让accept睡眠一段时间，在此期间启动客户端，一旦连接就发送RST。

![accpt](https://wangpengcheng.github.io/img/2019-11-25-22-20-57.png)

当服务器在`accept`阻塞时，假如进程突然崩溃
- 此时子进程退出时:
  - 发FIN字节发送到套接字，客户端收到后回应以一个ACK
  - 同时内核向父进程发送一个`SIGCHLD`信号，父进程调用`sig_chil`处理，处理完成后返回`accept`调用
  - 注意：**如果没有配置自动重启标识，`accept`调用将出错，并将`errno `设为`EINTR`**
- 解决方案:
  - 在配置信号处理函数时，设置`act.sa_flags |= SA_RESTART`;这样当accept被中断返回后，能继续 阻塞。
  - 修改accept的判断条件: 当accept返回错误时，我们可以判断一下是否errno为EINTR，如果是我们就手动重启accept。
```c
connfd = accept(listenfd, (SA *) &cliaddr, &clilen);
if(connfd < 0){
	if (errno == EINTR){
		continue;
	} else {
		err_sys("accept error");
	}
}
```
**Code: 注意这里我们调用的时accept 而不是 包裹函数Accept**

### 5.12 服务器进程意外终止

这个问题也可以用上面的情形测试，我们通过kill掉服务器子进程来模拟。

对于客户端:

1. 当服务器终止后会发送的FIN字节(表明服务端不在发送内容)，客户端自动以ACK回应
2. 然后服务器被**强行毙掉**
3. 但客户端并不知道服务器进程已经被毙掉了(它只收到了FIN,并不能说明它被毙掉了)，因为客户端此时是阻塞于fgets的，并不会发送FIN字节给服务器，此时客户端认为链接并没有关闭，因此一直等待用户从标准输入输入字符
4. 如果用户一直不输入那么程序永远不知道服务器已经挂了。
5. 当用户输入一些字符的时候，服务器就会回应一个`RST`，客户才知道服务器已经挂了
6. 如果客户继续发送内容将引发`SIGPIPE`信号（这种情况很可能发生，因为客户发给服务端的内容可能是分几次发送的，第一次发的时候就回收到`RST`，在收到`RST`期间还可能发送很多内容）。

如何解决：

这个问题的根本原因在于客户端，它不能仅仅阻塞于fgets,它应该同时关注stdin 和 socket ,任意一个退出都应该及时知道。因此可以使用select来管理这2个描述符。

### 5.13 发送数据格式有限制

当发送字符串时一般没什么问题，只要不同主机都支持同一中字符编码，但是如果发送的是二进制就有很多问题，比如不同主机字节序可能不同、CPU位数不同，各种数据类型占用空间以及对齐格式可能不同，这其实也是二进制文件的兼容性问题，因此兼容难度非常大。

### 5.14 服务器崩溃 或者网络中断

TCP有重传机制，当网络不通时，客户端将不停地重传未收到确认的分组，直到放弃。。。这里可能需要很久的时间，我们当然希望能尽快知道服务器崩溃的消息了，利用SO_KEEPALIVE套接字选项就可以解决这个问题。
