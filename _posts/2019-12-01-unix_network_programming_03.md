---
layout:     post
title:      UNIX网络编程 学习笔记 (三)
subtitle:   UNIX网络编程 学习笔记 (三) 
date:       2019-12-01
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
- [《Unix网络编程》卷1 中级](https://blog.csdn.net/zzxiaozhao/article/details/102662861)
- [《UNIX网络编程卷一》读书笔记](https://zevvez.github.io/2017/12/13/unp/#%E7%AC%AC%E4%B8%80%E7%AB%A0-%E7%AE%80%E4%BB%8B)

> 2019-12-01 22:10:53

## 第 9 章 基本SCTP套接字编程

略

## 第 10 章 SCTP客户/服务器程序例子

略

### 第 11 章 名字与地址转换(DNS)

域名系统(Domain Name System, DNS):主要用于主机名字和IP地址之间的映射。主要使用`gethostbyname`和`gethostbyaddr`在主句名字与IPv4地址之间进行转换。`getservbyname`和`getservbyport`在服务器名字和端口号之间进行转换。

DNS中的条目称为资源记录(resource record)；相关的资源记录方式如下：

- `A`记录: 把一个主机名映射为一个32位IPv4地址
- `AAAA`记录: 把一个主机名映射为一个128位IPv6地址
- `PTR`记录: 称为指针记录(pointer record)把IP地址映射为主机名
  - 对于IPv4,将32位地址的4个字节先反转顺序,每个字节都转为换各自的十进制ASCII值后,在添加in-addr.arpa,结果字符串用于PTR查询
  - 对于IPv6, 128位地址的32个四位组先反转顺序,每个四位组都转换成相应的十六进制的ASCII值后在添加ip6.arpa
- `MX`记录: 把一个主机指定作为给定主机的“邮件交换器”
- `CNAME`: 代表”canonical name”(规范名字),常见的用法是为常用的服务指派CNAME记录

#### 11.2.2 解析器(resolver)和名字服务器(name server)

- 应用程序通过调用解析器函数库中的函数(gethostbyname,gethostbyaddr)接触DNS服务器。
- 解析器函数库中的代码通过读取系统相关配置文件(/etc/resolv.conf)确定本地name server的地址。
- 解析器使用UDP向本地name server发出查询，如果本地name server不知道答案，解析器就会使用UDP向整个因特网查询其他的名字服务器；如果答案太长超过了UDP的承载能力，自动切换到TCP。
- resolver可以看成DNS的客户端；nameserver可以看成DNS的服务器。

![客户、解析其和名字服务器的典型关系](https://wangpengcheng.github.io/img/2019-12-02-10-03-17.png)

解析器使用UDP向本地名字服务器发出查询，如果本地查找不到，就会使用UDP在整个Internet网上查询其他域名服务器。如果答案太长，超出了UDP的消息的承载能力，本地名字服务器和解析器会自动切换到TCP

#### 11.2.3 DNS 替代方法

- 静态主机文件(/etc/hosts)
- 网络信息系统(NIS)
- 轻量级目录访问协议(LDAP)

### 11.3 gethostbyname 函数

这个函数只能返回IPv4地址，在新的程序中应该使用getaddrinfo；
- 函数定义: `struct hostent * gethostbyname(const char * hostname);`
- 返回：成功则返回非空指针, 失败则NULL且设置h_errno
- 注意此函数失败之后并不会设置errno变量,而是将全局整数变量h_errno设置为在头文件<netdb.h>中定义的常值之一:
  - `HOST_NOT_FOUND`
  - `TRY_AGAIN`
  - `NO_RECOVERY`
  - `NO_DATA`(等同于`NO_ADDRESS`)
    - 此错误表示所指向的名字有效,但是没有`A`记录

hostnet关键结构体如下：
```c
struct hostent{
    char    *h_name;        /* 主机的官方名称 */
    char    **h_aliases;    /* 主机别名的指针数组 */
    int     h_addrtype;     /* 主机IP地址的类型 */
    int     h_length;       /* IPv4的地址长度 */
    char**  h_addr_list;    /* 指向IPv4的地址指针 */
}
```
![hostnet包含的结构体信息](https://img-blog.csdnimg.cn/20191026141722883.png#pic_center)

例子：
```c++
#include "unp.h"

int main(int argc, char const *argv[]) {
    char * ptr, ** pptr;
    char str[INET_ADDRSTRLEN];
    /* 定义返回的结构体 */
    struct hostent * hptr;

    while(--argc>0){
        /* 获取输入参数 */
        ptr = * ++argv;
        /* 查找对应的ip地址 */
        if((hptr = gethostbyname(ptr)) == NULL) {
            err_msg("gethostbyname error for host :%s : %s", ptr, hstrerror(h_errno));
                continue;
        }
        printf("official hostname: %s\n", hptr->h_name);
        /* 遍历输出所有别名 */
        for(pptr = hptr->h_aliases; *pptr != NULL; pptr++){
            printf("\talias: %s\n", *pptr);
        }
        /* 选择地址类型并输出 */
        switch (hptr->h_addrtype) {
            case AF_INET:
                pptr = hptr->h_addr_list;
                for (; *pptr != NULL; pptr++){
                    printf("\taddress : %s\n",
                    Inet_ntop(hptr->h_addrtype, * pptr, str, sizeof(str)));
                    break;
                }
            default:
                err_ret("unknown address type");
                break;
        }
    }
    return 0;
}
/* RUN
./hostent music.163.com
official hostname: music.163.com
        address : 59.111.160.197
        address : 59.111.160.195
        address : 223.252.199.66
*/

```

### 11.4 gethostbyaddr函数

gethostbyaddr函数试图由一个二进制的IP地址找到相应的主机名，与 gethostbyname 的行为刚好相反。(**注意：在新的程序中应该使用`getnameinfo`**)
注：此处与原书中的 gethostbyaddr函数不同，原书中的函数在库<netdb.h>，且addr参数为 const char *
```c
#include <sys/socket.h>
struct hostent * gethostbyaddr(const void * addr,socklen_t len,int type);
//if success return not NULL pointer,if error return NULL and set h_errno
```

addr 参数是一个指向IPV4地址的某个in_addr结构的指针；len参数是这个结构的大小；family参数为`AF_INET`或`AF_INET6`

按照DNS的说法，`gethostbyaddr`在`in_addr.arpa`域中向一个名字服务器查询`PTR`记录。

### 11.5 getservbyname和getservbyport函数
#### 11.5.1 getservbyname

getservbyname函数用于根据给定的名字查找相应的服务。一般服务器对应的IP地址保存在`/etc/services`中;典型内容如下：

```sh
[breap@breap ~]$ grep -e ^ftp -e ^domain /etc/services
ftp-data        20/tcp
ftp-data        20/udp
ftp             21/tcp
ftp             21/udp          fsp fspd
domain          53/tcp                          # name-domain server
domain          53/udp
ftp-data        20/sctp                 # FTP
.......
```

函数的定义和声明如下：

```c
#include <netdb.h>
struct servent * getservbyname(const char * servname,const char * protoname);
//if success return not NULL pointer,if error return NULL
struct servent
{
    char * s_name; //Official services name
    char ** s_aliases; //Alias list
    int s_port; //Port number,Network byte order
    char * s_proto; //Protocol to use
}
```
参数说明：
- `servname`:服务器参数名称；必须指定
- `protoname`:协议名称，可选参数，非空时，必须有匹配的协议；有些因特网服务既用 TCP 也用 UDP 提供，其他英特网则仅仅支持单个协议。
    - 如果`protoname`未指定而`servname`指定服务支持多个协议，那么返回哪个端口号取决于实现。
        - 通常情况下这种选择无关紧要，因为支持多个协议的服务往往使用相同的`TCP`端口号和`UDP`端口号，不过这点并无保证。
    - `servent`结构中我们关心的主要字段是端口号(网络字节序)
        - 既然端口号是以网络字节序返回的，把它存放到套接字地址结构时绝对不能调用`htons`。

函数调用示例：
```c
struct servent *sptr;
sptr=getservbyname("domain","udp"); //DNS using UDP
sptr=getservbyname("ftp","tcp"); //FTP using TCP
sptr=getservbyname("ftp",NULL); //FTP using TCP
sptr=getservbyname("ftp","udp"); //this call will fail
```
#### 11.5.2 getservbyport

getservbyport 函数用于给定端口号和可选协议查询相应服务.函数的定义如下：

```c
#include <netdb.h>
struct servent * getservbyport(int port,const char * protoname);
//if success return not NULL pointer,if error return NULL
```
`port`参数的值必须为**网络字节序**。该函数的典型调用如下：
```c
struct servent *sptr;
sptr=getservbyport(htons(53),"udp"); //DNS using UDP
sptr=getservbyport(htons(21),"tcp"); //FTP using TCP
sptr=getservbyport(htons(21),NULL); // FTP using TCP
sptr=getservbyport(htons(21),"udp"); //this call will fail
```

注意：
- **某些端口号在TCP上是一种服务，在UDP上却用于完全不同的另一种服务。**
- **端口可以复用,只要协议不同**
例如：
```sh
[breap@breap ~]$ grep 514 /etc/services
shell           514/tcp         cmd             # no passwords used
syslog          514/udp
```


使用示例如下：
```c++
#include "unp.h"

int main(int argc, char const *argv[]) {
    int sockfd, n;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;
    struct in_addr ** pptr;
    struct in_addr * inetaddrp[2];
    struct in_addr inetaddr;
    struct hostent * hp;
    struct servent * sp;

    if(argc!=3){
        err_quit("usage: daytimetcpcli1 <hostname> <services>");
    }
    if((hp = gethostbyname(argv[1])) == NULL) {
        /* 确定是否为正确的ASCII地址格式 */
        if (inet_aton(argv[1], &inetaddr)==0){
            err_quit("hostname error for %s : %s", argv[1], hstrerror(h_errno));
        }else{
            inetaddrp[0] = &inetaddr;
            inetaddrp[1] = NULL;
            pptr = inetaddrp;
        }
    }else{
        pptr = (struct in_addr ** )hp->h_addr_list;
    }
    /* 获取服务 */
    if((sp = getservbyname(argv[2], "tcp")) == NULL){
        err_quit("getservbyname error for %s : %s", argv[2]);
    }
    for(; *pptr != NULL; pptr++) {
        /* 建立TCP连接 */
        sockfd = Socket(AF_INET, SOCK_STREAM, 0);
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = sp->s_port;
        memcpy(&servaddr.sin_addr, * pptr, sizeof(struct in_addr));
        printf("trying %s \n", Sock_ntop((SA *)&servaddr, sizeof(servaddr)));
        /* 进行连接 */
        if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) == 0){
            break;
        }
        err_ret("connect error");
        close(sockfd);
    }

    if (*pptr == NULL){
        err_quit("unable to connect");
    }
    /* 执行read */
    while((n = Read(sockfd, recvline, MAXLINE))>0){
        recvline[n] = 0; // null terminate
        Fputs(recvline, stdout);
    }
    return 0;
}
/*
[breap@breap name_test]$ ./daytimetcpcli1 127.0.0.1 daytime
trying 127.0.0.1:13
Tue Jan 16 20:06:35 2018
*/

```

### 11.6 getaddrinfo

这个函数能够处理名字到地址以及服务器到端口这两种转换。返回一个sockaddr结构而不是一个地址列表。该函数在POSIX规范中定义如下：
```c
#include <netdb.h>
int getaddrinfo(const char * hostname,const char * service,const struct addrinfo *hints,struct addrinfo **result);
//if success return 0,if error return not 0;
```
参数解析：
- hostname:主句名或者地址串(IPv4的点分十进制数串或IPv6的十六进制数串)
- service:服务名或者十进制端口号数串
- hints:期望返回的信息类型的暗示，可以为空指针，也可以指向某个addrinfo结构指针。
- result:返回结果；由result参数指向的变量已被填入一个指针，它指向的是由其中的ai_next成员串接起来的addrinfo结构链表(多个addrinfo结构返回时，这些结构的先后顺序没有保证 )。
  - 可导致返回多个`addrinfo`结构的情况有两个：
    - 如果与`hostname`参数关联的地址有多个，那么适用于所请求地址族(可通过hints结构的 ai_family成员设置)的每个地址都返回一个对应的结构
    - 如果`service`参数指定的服务支持多个套接字类型，那么每个套接字类型都可能返回一个对应的结构，具体取决与 `hints`结构的`ai_socktype`成员。 （注意，`getaddrinfo`的多数实现认为只能按照由`ai_socktype`成员请求的套接字类型端口号数串到端口的转换，如果没有指定这个成员，那就返回一个错误）
addrinfo的定义如下：

```c
struct addrinfo{
	int ai_flags; //AI_PASSIVE,AI_CANONNAME
	int ai_family; //AF_XXX
	int ai_socktype; //SOCK_XXX
	int ai_protocol; //0 or IPPROTO_XXX for IPV4 and IPV6
	socklen_t ai_addrlen; //length of ai_addr
	char * ai_canonname; //ptr to canonical name for host
	struct sockaddr * ai_addr; //ptr to socket address structure
	struct addrinfo * ai_next; //ptr to next structure in linked list
}
```
`addrinfo`结构中调用者(hints)可以设置的成员有, (如果hints参数是一个空指针，该函数就假设 ai_flags, ai_socktype, ai_protocol 的值均为0，ai_family的值为AF_UNSPEC。)
调用者hints可以设置的成员包括：

|调用者(`hints`)|可以设置的成员|
|:---|:---|
|`ai_flags`|零个或多个在一起的AI_XXX值.|
|`ai_family`|某个AF_XXX|
|`ai_socktype`|某个SOCK_XXX值|
|`ai_protocol`|某个写控制协议|


|ai_flags可能值|含义|
|:---|:---|
|`AI_PASSIVE`|套接字将用于被动打开。|
|`AI_CANONNAME`|告知 getaddrinfo 函数返回主机的规范名字。(canon,规范)|
|`AI_NUMERICHOST`|防止任何类型的名字到地址映射， hostname参数必须为一个地址串|
|`AI_NUMERICSERV`|防止任何类型的名字到服务的映射,service 参数必须是一个十进制端口号数串。|
|`AI_V4MAPPED`|如果同时指定 ai_family 成员的值为 AF_INET6,那么如果没有可用的AAAA记录，就返回与A记录对应的IPV4映射的IPV6地址。|
|`AI_ALL`|如果同时指定 AI_V4MAPPED 标志，那么除了返回与AAAA记录对应的IPV6地址外，还返回与A记录对应的IPV4映射的IPV6地址。|
|`AI_ADDRCONFIG`|按照所在主机的配置选择返回地址类型，也就是只查找与所在主机回馈接口以外的网络接口配置的IP地址版本一致的地址。|

一些常见的输入 (**如清楚知道自己需要的套接字类型，应该在`hints`中设置`ai_socktype`成员,避免返回多个结构，甚至出现错误的`ai_socktype`值**):

- 指定`hostname`和`service`。这是TCP或UDP客户进程调用getaddrinfo的常规输入。
- 对于`TCP`客户在一个循环中针对每个返回的IP地址，调用`socket`和`connect`，直到有一个连接成功，或者所有地址尝试完毕为止。
- 对于`UDP`客户由`getaddrinfo`填入的套接字地址结构用于` sendto` 或 `connect`。
- 只指定`service`而不指定`hostname`，同时在hints结构中指定 `AI_PASSIVE`标志。这是典型的服务器进程.
  - 返回的套接字地址结构中应含有一个值为 `INADDR_ANY(IPV4)`或`IN6ADDR_ANY_INIT(IPV6)`。
  - TCP 服务器随后调用 `socket`、`bind`、`listen`、`accept`。
  - UDP服务器将调用 `socket`、`bind`、`recvfrom`。

简单的使用示例如下：

```c
struct addrinfo hints,*res;
/* 重置结构 */
bzero(&hints,sizeof(hints));
hints.ai_flags=AI_CANONNAME;
hints.ai_family=AF_INET;
/* 获取host和server的ip地址 */
getaddinfo("freebad4","domain",&hints,&res);

```

上面主机freebsd4的规范名字是freebad4.unpbook.com；并且它在DNS中有2个IPv4地址；则返回结果如下：

![](https://wangpengcheng.github.io/img/2019-12-03-21-08-52.png)

### 11.7 gai_strerror函数

`getaddrinfo`返回非0值表示错误,而`gai_strerror`可以通过此非0值返回一个指向对应的出错信息串的指针。函数的基本使用如下：

```c
#include <netdb.h>
const char * gai_strerror(int error);
// 返回：指向错误描述消息字符串的指针
```
getaddrinfo返回的非0错误长值：

|常数|说明|
|:---|:---|
|`EAI_AGAIN`|名字解析中临时失败|
|`EAI_BADFLAGS`|ai_flags的值无效|
|`EAI_FAIL`|名字解析中不可恢复地失败|
|`EAI_FAMILY`|不支持 ai_family|
|`EAI_MEMORY`|内存分配失败|
|`EAI_NONAME`|hostname或service未提供，或者不可知|
|`EAI_OVERFLOW`|用户参数缓冲区溢出（仅限 getnameinfo 函数）|
|`EAI_SERVICE`|不支持 ai_socktype 类型的service|
|`EAI_SOCKTYPE`|不支持 ai_socktype|
|`EAI_SYSTEM`|在errno变量中有系统错误返回|

### 11.8 freeaddrinfo函数

getaddrinfo返回的所有存储空间都是动态获取的。需要使用freeaddrinfo进行调用返还给系统。函数定义如下：

```c
#include <netdb.h>
void freeaddrinfo(struct addrinfo *ai);
// ai 参数应指向由 getaddrinfo 返回的第一个 addrinfo 结构。
```

### 11.9 getaddrinfo函数IPv6

POSIX 规范定义了 getaddrinfo 函数以及该函数为IPV4或IPV6返回的信息。

注意以下几点：

- `getadrinfo`在处理两个不同的输入：
  - 一个是套接字地址结构类型，调用者期待返回的地址结构符合这个类型；
  - 另一个是资源记录类型，在DNS或者其它数据库中执行的查找符合这个类型.
- 由调用者在`hints`结构中提供的地址族指定调用者期待返回的套接字地址结构的类型
- `POSIX`声称如果调用者指定`AF_UNSPEC`,那么`getaddrinfo`函数返回的是适合于指定主机名和服务名且适合任意协议族的地址.
- `POSIX`的这个声明也意味着如果设置了`AI_PASSIVE`标志但是没有指定主机名，那么IPV6通配地址(IN6ADDR_ANY_INIT或0::0)应该作为`sockaddr_in6`结构返回，同样IPV4通配地址 (INADDR_ANY 或 0.0.0.0)应该作为sockaddr_in结构返回。
- 在`hints`结构的`ai_family`成员中指定的地址族以及在`ai_flag`s成员中指定的`AI_V4MAPPED`和`AI_ALL`等标志决定了在DNS中查找的资源记录类型。
- 主机名参数还可以是IPV6的十六进制数串或IPV4的点分十进制数串。这个数串的有效性取决于调用者指定的地址族

POSIX 规范定义了 getaddrinfo 函数以及该函数为IPV4或IPV6返回的信息。在下表汇总这些返回值之前，我们注意以下几点：

- getaddrinfo 在处理两个不同的输入：一个是套接字地址结构类型，调用者期待返回的地址结构符合这个类型；另一个是资源记录类型，在DNS或其他数据库中执行的查找符合这个类型。
- 由调用者在hints结构中提供的地址族指定调用者期待返回的套接字地址结构的类型
- POSIX声称如果调用者指定AF_UNSPEC,那么 getaddrinfo 函数返回的是适合于指定主机名和服务名且适合任意协议族的地址
- POSIX的这个声明也意味着如果设置了 `AI_PASSIVE` 标志但是没有指定主机名，那么 IPV6 通配地址（`IN6ADDR_ANY_INIT`或0::0）应该作为 `sockaddr_in6` 结构返回，同样 IPV4 通配地址 (`INADDR_ANY` 或 0.0.0.0)应该作为 `sockaddr_in` 结构返回
- 在`hints`结构的 `ai_family` 成员中指定的地址族以及在`ai_flags`成员中指定的 `AI_V4MAPPED` 和 `AI_ALL` 等标志决定了在 DNS中查找的资源记录类型
- 主机名参数还可以是IPV6的十六进制数串或IPV4的点分十进制数串。这个数串的有效性取决于调用者指定的地址族

----

<table>
<tbody>
<tr>
<td>&nbsp;调用者指定的主机名</td>
<td>&nbsp;调用者指定的地址族</td>
<td>&nbsp;主机名字符串包含</td>
<td>&nbsp;结果</td>
<td>行为&nbsp;</td>
</tr>
<tr>
<td rowspan="11">非空主机名字符串：主动或被动&nbsp;</td>
<td rowspan="3">AF_UNSPEC&nbsp;</td>
<td>&nbsp;主机名</td>
<td>以 sockaddr_in6{} 返回所有 AAAA 记录，以 sockaddr_in{} 返回所有A记录</td>
<td>AAAA记录搜索加上A记录搜索&nbsp;</td>
</tr>
<tr>
<td>&nbsp;十六进制数串</td>
<td>&nbsp;一个sockaddr_in6{}</td>
<td>&nbsp;inet_pton(AF_INET6)</td>
</tr>
<tr>
<td>&nbsp;点分十进制数串</td>
<td>&nbsp;一个sockaddr_in{}</td>
<td>&nbsp;inet_pton(AF_INET)&nbsp;</td>
</tr>
<tr>
<td rowspan="5">AF_INET6&nbsp;</td>
<td rowspan="3">&nbsp;主机名</td>
<td>以 sockaddr_in6{} 返回所有 AAAA 记录</td>
<td>&nbsp;AAAA记录搜索</td>
</tr>
<tr>
<td>&nbsp;在 ai_flags 含 AI_V4MAPPED前提下：若存在AAAA记录则以 sockaddr_in6{} 返回所有 AAAA 记录；否则以 sockaddr_in6{}作为IPV4映射的IPV6地址返回所有A记录</td>
<td>AAAA记录搜索，若无结果则A记录搜索&nbsp;&nbsp;</td>
</tr>
<tr>
<td>&nbsp;在 ai_flags 含&nbsp;AI_V4MAPPED和 AI_ALL前提下：以 sockaddr_in6{} 返回所有 AAAA 记录，并且以 sockaddr_in6{}作为IPV4映射的IPV6地址返回所有A记录</td>
<td>AAAA记录搜索加上A记录搜索&nbsp;&nbsp;</td>
</tr>
<tr>
<td>&nbsp;十六进制数串</td>
<td>&nbsp;一个sockaddr_in6{}</td>
<td>&nbsp;inet_pton(AF_INET6)</td>
</tr>
<tr>
<td>&nbsp;点分十进制数串</td>
<td>&nbsp;作为主机名查找</td>
<td>&nbsp;</td>
</tr>
<tr>
<td rowspan="3">&nbsp;AF_INET</td>
<td>&nbsp;主机名</td>
<td>&nbsp;以 sockaddr_in{} 返回所有A记录</td>
<td>&nbsp;A记录搜索</td>
</tr>
<tr>
<td>&nbsp;&nbsp;十六进制数串</td>
<td>&nbsp;&nbsp;作为主机名查找</td>
<td>&nbsp;</td>
</tr>
<tr>
<td>&nbsp;&nbsp;点分十进制数串</td>
<td>&nbsp;一个sockaddr_in{}</td>
<td>&nbsp;inet_pton(AF_INET)&nbsp;</td>
</tr>
<tr>
<td rowspan="3">空主机名字符串：被动&nbsp;</td>
<td>&nbsp;AF_UNSPEC</td>
<td>
<p>&nbsp;隐含0::0</p>
<p>隐含0.0.0.0</p>
</td>
<td>&nbsp;一个sockaddr_in6{}和 一个sockaddr_in{}</td>
<td>
<p>&nbsp;inet_pton(AF_INET6)</p>
<p>&nbsp;inet_pton(AF_INET)&nbsp;</p>
</td>
</tr>
<tr>
<td>&nbsp;AF_INET6</td>
<td>&nbsp;隐含0::0</td>
<td>&nbsp;&nbsp;一个sockaddr_in6{}</td>
<td>&nbsp;inet_pton(AF_INET6)</td>
</tr>
<tr>
<td>&nbsp;AF_INET</td>
<td>隐含0.0.0.0&nbsp;</td>
<td>&nbsp;&nbsp;一个sockaddr_in{}</td>
<td>&nbsp;inet_pton(AF_INET)</td>
</tr>
<tr>
<td rowspan="3">空主机名字符串：主动&nbsp;&nbsp;</td>
<td>&nbsp;AF_UNSPEC</td>
<td>
<p>隐含0::1</p>
<p>隐含127.0.0.1&nbsp;</p>
</td>
<td>一个sockaddr_in6{}和 一个sockaddr_in{}&nbsp;</td>
<td>
<p>&nbsp;inet_pton(AF_INET6)</p>
<p>&nbsp;inet_pton(AF_INET)&nbsp;</p>
</td>
</tr>
<tr>
<td>&nbsp;AF_INET6</td>
<td>&nbsp;隐含0::1</td>
<td>&nbsp;&nbsp;一个sockaddr_in6{}</td>
<td>&nbsp;inet_pton(AF_INET6)</td>
</tr>
<tr>
<td>&nbsp;AF_INET</td>
<td>&nbsp;隐含127.0.0.1</td>
<td>&nbsp;&nbsp;一个sockaddr_in{}</td>
<td>&nbsp;inet_pton(AF_INET)</td>
</tr>
</tbody>
</table>

----


### 11.10 getaddrinfo函数，例子

```c++
static void do_funccall(
                        const char *host,/* 主机名 */
                        const char *serv,/* 服务器名 */
                        int flags, /* 标志参数 */
                        int family,/* 套接字协议族 */
                        int sockettype, /* 套接字类型 */
                        int protocol, /* 控制协议 */
                        int exprc /* 控制协议族 */
                        )
{
    int rc;
    /* 定义地址结构体 */
    struct addrinfo hints, *res;
    /* 初始化结构体 */
    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_flags=flags;
    hints.ai_family=family;
    hints.ai_socktype = socktype;
	hints.ai_protocol = protocol;
    /* 执行函数 */
    rc=getaddrinfo(host,serv,&hints,&res);
    if(rc!=exprc){
        printf("expected return = %d (%s),\nactual return = %d (%s)\n",exprc, gai_strerror(exprc), rc, gai_strerror(rc));
        if(host!=NULL)
            printf(" host =%s\n",host);
        if(serv!=NULL)
            printf(" serv=%s \n",serv);
        printf("  flags = %d, family = %s, socktype = %s, protocol = %d\n",
				flags, str_fam(family), str_sock(socktype), protocol);
		exit(2);
    }
}
```
### 11.11 host_serv函数

访问getaddrinfo的第一个接口函数不要求调用者调用者分配并填写一个 hints 结构。该结构中我们感兴趣的两个字段(**地址族和套接字类型**)成为这个名为host_serv的接口函数的参数。

```c++
#include "unp.h"
//返回若成功则指向addrinfo结构的指针，若出错则为NULL

struct addrinfo *host_serv(const char *hostname,const char *service,int family,int socktype)
{
    int n;
    struct addrinfo hints,*res;
    bzero(&hints,sizeof(struct addrinfo));
    /* always return canonical name */
    hints.ai_flags=AI_CANONNAME;
    /* AF_UNSPEC,AF_INET,AF_INET6,etc */
    hints.ai_family=family;
    /* 0,SOCK_STREAM,SOCK_DGRAM,etc */
    hints.ai_socktype=socktype;
    /* 执行函数，进行检查 */
    if((n=getaddrinfo(host,serv,&hints,&res))!=0) return NULL;
    return res;
}
```
### 11.12 tcp_connect函数

tcp_connect函数执行客户的通常步骤：创建一个TCP套接字并连接到一个服务器：
```c++
#include "unp.h"

// 返回：若成功则为已连接的套接字描述符，若出错则不返回
int tcp_connect(const char *host, const char *service) {
    /* 定义socket套接字 */
    int sockfd, n;
    /* 定义地址信息结构体 */
    struct addrinfo hints, *res, *ressave;
    /* 初始化结构体 */
    bzero(&hints, sizeof(struct addrinfo));
    /* 进行初始化 */
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    /* 查询地址 */
    if ((n = getaddrinfo(host, service, &hints, &res)) != 0)
    {
        err_quit("tcp_connect error for %s , %s : %s", host, service,gai_strerror(n));
    }
    ressave = res;
    /* 遍历结果进行连接 */
    do{
        /* 创建和初始化套接字 */
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        /* 建立连接 */
        if (sockfd < 0) continue; // ignore this one
        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0) break; // success
        /* 关闭连接 */
        Close(sockfd); // ignore this one
    }while ((res = res->ai_next) != NULL);
    /* 检查结果 */
    if(res == NULL) // errno set from final connect()
        err_sys("tcp_connect error for %s ,%s", host, service);
    /* 释放内存 */
    freeaddrinfo(ressave);
    return sockfd;
}

```
例子：时间获取客户程序

```c++
#include "unp.h"
int main(int argc,char const *argv[])
{
    int socket,n;
    char recvline[MAXLINE+1];
    socklen_t len;
    struct sockaddr_storage ss;
    if(argc!=3) err_quit("usage : daytimetcpcli <hostname/IPaddress> <service/prot#>");
    /* 建立TCP连接 */
    sockfd=tcp_conect(argv[1],argv[2]);
    len=sizeof(ss);
    /* 获取对端(服务器)的协议地址 */
    Getpeername(sockfd,(SA *)&ss,&len);
    /* 输出解析地址 */
    printf("connected to %s \n", Sock_ntop_host((SA *)&ss, len));
    /* 处理套接字连接 */
    while(n=Read(sockd,recvline,MAXLINE)>0){
        recvline[n]=0;
        Fputs(recvline,stdout);
    }
    return 0;
}
```
### 11.13 tcp_listen函数

`tcp_listen`函数执行`TCP`服务器的通常步骤：创建一个`TCP`套接字，给它捆绑服务器的众所周知的端口，并允许接受外来的连接请求。

```c++
#include "unp.h"
/* 若成功则为套接字描述符，若出错则不返回 */
int tcp_listen(const char *host,const char *serv,socklen_t *addrlen)
{
    /* 初始化相关变量 */
    int listenfd,n;
    const int on=1;
    struct addrinfo hints,*res,*ressave;

    /* 初始化相关变量 */
    bzero(&hints,sizeof(struct addrinfo));
    hints.ai_flags=AI_PASSIVE;
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    /* 获取地址信息 */
    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0){
        err_quit("tcp_listen error for %s , %s : %s", host, serv, gai_strerror(n));
    }
    ressave = res;
    /* 遍历查询结果 */
    do{
        /* 创建socket */
        listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        //error next
        if(listenfd<0){
            continue;
        }
        /* 设置socket选项;防止端口已经有连接存在 */
        Setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
        /* 连接地址和端口 */
        if(bind(listenfd,res->ai_addr，res->ai_addrlen)==0) break;
        Close(listenfd);
    }while((res=res->ai_next)!=NULL);
    if(res==NULL){
        err_sys("tcp listen error for %s,%s ",host,serv);
    }
    Listen(listenfd,LISTENQ);
    /* 返回协议地址 */
    if(addrlenp){
        *addrlenp=res->ai_addrlen;
    }
    freeaddrinfo(ressave);
    return listendfd;
}
```

例子1：时间获取服务器程序

```c++
#include "unp.h"
#include <time.h>
int main()
{
    /* 预定义变量 */
    int listenfd,connfd;
    socklen_t len;
    char buff[MAXLINE];
    time_t ticks;
    /* 客户端地址 */
    struct sockaddr_storage cliaddr;
    if(argc!=2){
        err_quit("usage:datatime cpsrvl <service or support #>");
    }
    listenfd=tcp_listen(NULL,argv[1],NULL);
    while(1){
        len=sizeof(cliaddr);
        connfd=Accpet(listenfd,(SA*)&cliaddr,&len);
        printf("connect from %s\n",Sock_ntop_host((SA*)&cliaddr,len));
        ticks=time(NULL);
        snprintf(buff,sizeof(buff),"%.24s\r\n",ctime(&ticks));
        Write(connfd,buff,strlen(buff));
        Close(connfd);
    }
    return 0;
}
```

注意：上述程序中，`tcp_listen`的第一个参数是一个空指针，而且tcp_listen函数内部指定的地址组`AF_UNSPEC`，两个结合可能导致`getaddrinfo`返回非期望地址族的套接字结构--双栈主机上返回的第一个套接字地址结构将是IPv6的，但是我们期望该服务器仅仅处理IPv4.

例子2：可指定协议的时间获取服务器程序:

```c++
#include "unp.h"

#include <time.h>

int main(int argc, char const *argv[]) {
    int listenfd, connfd;
    socklen_t len, addrlen;
    char buff[MAXLINE];
    time_t ticks;
    struct sockaddr_storage cliaddr;

    if (argc == 2) listenfd = tcp_listen(NULL, argv[1], &addrlen);
    else if (argc == 3) listenfd = tcp_listen(argv[1], argv[2], &addrlen);
    else err_quit("usage : daytimetcpsrv2 [<host>] <sevice or port>");

    while (1) {
        len = sizeof(cliaddr);
        connfd = Accept(listenfd, (SA *)&cliaddr, &len);
        printf("connection from %s\n", Sock_ntop_host((SA *)&cliaddr, len));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buff, strlen(buff));

        Close(connfd);
    }
    return 0;
}

```

### 11.14 udp_client函数

udp_client： 用于创建未连接的 UDP 套接字。

```c++
#include "unp.h"

/* 若成功则为未连接套接字描述符，若出错则不返回 */

int udp_client(const char *host, const char *serv, SA **saptr, socklen_t *lenp) {
    int sockfd, n;
    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    /* 这里获取的起始是服务器的IP和端口,而次函数的功能为,返回服务器套接字结构地址和本地套接字*/
    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
        err_quit("udp_client erorr for %s, %s : %s", host, serv, gai_strerror(n));
    ressave = res;

    do {
        /* 对于UDP套接字是可以不绑定的端口的,在第一次调用sendto的时候由内核指定一个临时端口 */
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd >= 0) break;
    } while ((res = res->ai_next) != NULL);

    if (res == NULL){
        err_sys("udp_client error for %s , %s", host, serv);
    }
    /* 分配地址内存 */
    *saptr = Malloc(res->ai_addrlen);
    /* 将结果复制到res中 */
    memecpy(*saptr, res->ai_addr, res->ai_addrlen);
    /* 返回地址长度 */
    *lenp = res->ai_addrlen;
    freeaddrinfo(ressave);
    return (sockfd);
}
```
例子：协议无关时间获取客户程序

```c++
#include "unp.h"

int main(int argc, char const *argv[]) {
    int sockfd, n;
    char recvline[MAXLINE + 1];
    socklen_t salen;
    struct sockaddr * sa;

    if (argc != 3) err_quit("usage : daytimeudpcli1 <hostname/IPaddress> <service/port#>");

    sockfd = Udp_client(argv[1], argv[2], (void **)&sa, &salen);
    printf("sending to %s\n", Sock_ntop_host(sa, salen));
    /* send 1 byte datagram */
    Sendto(sockfd, "", 1, 0, sa, salen);
    n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
    recvline[n] = '\0';
    Fputs(recvline, stdout);
    return 0;
}
```

### 11.15 udp_connect

该函数创建一个已连接UDP套接字
```c++

#include "unp.h"

// 返回：若成功则为以连接套接字描述符，若出错则不返回
int udp_connect(const char *host, const char *serv) {
    int sockfd, n;
    struct addrinfo hints, * res, * ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if((n = getaddrinfo(host, serv, &hints, &res)) != 0){
        err_quit("udp_connect error for %s, %s : %s", host, serv, gai_strerror(n));
    }
    ressave = res;

    do { 
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0) continue;
        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0) break;
        Close(sockfd);
    } while ((res = res->ai_next) != NULL);

    if (res == NULL) err_sys("udp_connect error for %s, %s ", host, serv);
    freeaddrinfo(ressave);
    return sockfd;
}

```
### 11.16 udp_server

```c++
#include "unp.h"


/* 返回：若成功则为以连接套接字描述符，若出错则不返回 */
int udp_server(const char *host, const char *serv, socklen_t *addrlenp) {
    int sockfd, n;
    struct addrinfo hints, * res, * ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
        err_quit("udp_server error for %s, %s : %s", host, serv, gai_strerror(n));
    ressave = res;

    do {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0) continue;
        if (bind(sockfd, res->ai_addr, res->addrlen)) == 0) break;
        Close(sockfd);
    } while ((res = res->ai_next) != NULL);

    if (res == NULL) err_sys("udp_server error for %s, %s ", host, serv);
    if (addrlenp) *addrlenp = res->ai_addrlen;
    freeaddrinfo(ressave);
    return sockfd;
}

```
例子：协议无关时间获取服务器程序

```c++
#include "unp.h"

#include <time.h>

int main(int argc, char **argv){
    int				sockfd;
    ssize_t			n;
    char			buff[MAXLINE];
    time_t			ticks;
    socklen_t		len;
    struct sockaddr_storage	cliaddr;

    if (argc == 2)
        sockfd = udp_server(NULL, argv[1], NULL);
    else if (argc == 3)
        sockfd = udp_server(argv[1], argv[2], NULL);
    else
        err_quit("usage: daytimeudpsrv [ <host> ] <service or port>");

    for ( ; ; ) {
        len = sizeof(cliaddr);
        n = Recvfrom(sockfd, buff, MAXLINE, 0, (SA *)&cliaddr, &len);
        printf("datagram from %s\n", Sock_ntop((SA *)&cliaddr, len));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Sendto(sockfd, buff, strlen(buff), 0, (SA *)&cliaddr, len);
    }
}
```

### 11.17 getnameinfo

这个是getaddrinfo的补充含糊，使用一个套接字地址为参数，返回描述中的主句的一个字符串和描述其中的服务的另一个字符串。这个函数可以不关心地址协议的类型，由函数自身处理。函数定义如下：

```c
#include <netdb.h>
/* 返回：若成功则为0，若出错则为非0 */
int getnameinfo(const struct sockaddr *sockaddr,socklen_t addrlen,char *host,socklen_t hostlen,char * serv,socklen_t servlen,int flags);
```
参数解析:

- `sockaddr`和`addrlen`指定一个套接字地址结构，此套接字地址结构包含转换成直观可读的字符的协议地址
- `host`和`hostlen`指定主机字符串(由调用者预先分配存储空间)(调用者不想返回主机字符串；那就指定`hostlen`为0);
- `serv`和serlen指定服务字符串(由调用者预先分配存储空间)(调用者不想返回服务字符串，那就指定`servlen`为0)

sock_ntop 和 getnameinfo 的差别在于，前者不涉及DNS，只返回IP 地址和端口号的一个可显示版本,后者通常尝试获取主机和服务的名字

getnameinfo函数6个可指定的标志：

|**常值**|**说明**|
|:---|:---|
|`NI_DGRAM`|数据报服务|
|`NI_NAMEREQD`|若不能从地址解析出名字则返回错误|
|`NI_NOFQDN`|只返回`FQDN`的主机名部分,一般是主机名称如`aix`而不是`aix.unpbook.com`|
|`NI_NUMERICHOST`|以数串格式返回主机字符串，这样getnameinfo就不会调用DNS,而是直接返回IP地址|
|`NI_NUMERICSCOPE`|以数串格式返回范围标识字符串|
|`NI_NUMERICSERV`|以数串格式返回服务字符串|

当处理的是数据报套接字时，调用者应该设置`NI_DGRAM`标志。否则getnameinfo无法确定所用协议(TCP和UDP)。

### 11.18 可重入函数（安全的，可被中断而不出错的函数）

_参考链接：_ [浅谈可重入函数与不可重入函数](https://blog.csdn.net/u011123091/article/details/81748686)

查看本章讲解的名字和地址转换函数以及第4章中的inet_XXX函数，我们就重入函数提请注意以下几点:
- 重用相关函数
  - 因历史原因, `gethostbyname`, `gethostbyname`, `getservbyname`, `getservbyport` 这4个函数是不可重入的，因为他们都返回指向同一个静态结构的指针(可能在静态结构可能被回调函数修改了)
  - `inet_pton`,`inet_ntop`总是可重入的
  - 因历史原因，`inet_ntoa`是不可重入的，不过支持线程的一些实现提供了使用线程特定数据的可重入版本。
  - `getaddrinfo` 可重入的前提是它调用的函数都是可重入的，这就是说，他应该调用可重入版本的 `gethostbyname`和`getservbyname`。该函数返回的结果全部存放在动态分配内存空间的原因之一就是允许它可重入。
  - `getnameinfo`可重入的前提是它调用的函数都是可重入的，这就是说，他应该调用可重入版本的 `gethostbyaddr`和`getservbyport`
- `errono`变量存在类似的问题： 首先因该注意若没有任何错误发生则`errno`的值不会改变。因此，除非知道发生了一个错误，否则不应该查看`errno`的值。

#### 11.19 gethostbyname_r和gethostbyaddr_r函数

将gethostname之类的不可重入的函数更改为重入的函数。主要更改的思路和方法如下：

1. gethostname_r:主要将不可重入函数填写并返回静态结构的做法改为由调用者分配再由可重入函数填写结构。
2. getaddrinfo：由可重入函数调用malloc以动态分配内存空间。但是必须使用freeaddrinfo释放动态分配的内存空间。

两个函数的定义如下：

```c
/* 返回：若成功则为非空指针，若出错则为NULL */

int gethostbyname_r(const char *name,
               struct hostent *ret, char *buf, size_t buflen,
               struct hostent **result, int *h_errnop);
/* 返回：若成功则为非空指针，若出错则为NULL */

int gethostbyaddr_r(const void *addr, socklen_t len, int type,
               struct hostent *ret, char *buf, size_t buflen,
               struct hostent **result, int *h_errnop);
```
注意：buf参数是由调用者分配且大小为buflen的缓冲区。主要存放规范主机名，别名指针数组，各个别名字符串，地址之恩数组以及各个实际地址。

### 11.21 其它网络信息

我们在本章中一直关注`主机名`和`IP地址`以及`服务名`和`端口号`。然而我们的视野可以更广阔，应用程序可能想要查找四类与网络相关的信息：`主机`、`网络`、`协议`、`服务`。

- 大多数查找针对的是主机(`gethostbyname`和 `bethostbyaddr`),
- 一小部分查找针对的是服务(`getservbyname`和`getservbyport`),
- 更小一部分查找针对的是网络和协议

所有四类信息都可以存放在一个文件中，每类信息各定义有三个访问函数：
- 函数`getXXXent`读出文件中的下一个表项，必要的话首先打开文件。
- 函数`setXXXent`打开(如果尚未打开的话) 并回绕文件。
- 函数`endXXXent`关闭文件。

每类信息都定义了各自的结构，包括 `hostent`,`netent`,`servent`. 都由 <netdb.h> 提供。

除了顺序处理文件 `get`、`set`、`end`这三个函数外，每类信息还提供一些键值查找函数。这些函数顺序遍历整个文件(通过调用 `getXXXent`函数读出每一行)，并寻找与某个参数匹配的一个表项。这些键值查找函数具有形如 `getXXXbyYYY` 的名字。

四类网络相关信息：

|信息|数据文件|结构|键值查找函数|
|:---|:---|:---|:---|
|主机|`/etc/hosts`|`hostent`|`gethostbyaddr`,`gethostbyname`|
|网络|`/etc/networks`|`netent`|`getnetbyaddr`, `getnetbyname`|
|协议|`/etc/protocols`|`protoent`|`getprotobyname`, `getprotobynumber`|
|服务|`/etc/services`|`servent`|`getservbyname`, `getservbyport`|

在使用DNS的前提下如何应用这些函数呢？首先只有主机和网络信息可以通过DNS获取，协议和服务信息总是从相应的文件中读取。我们之前有提到过，不同的实现有不同的方法供系统管理员指定是使用DNS还是使用文件来查找主机和网络信息。

其次，如果使用DNS查找主机和网络信息，那么只有键值查找函数才有意义。举例来说，你不能使用gethostent并期待顺序遍历DNS中所有表项。如果调用gethostent,那么它仅仅读取/etc/hosts文件并避免访问DNS。

注：虽然网络信息可以做成通过DNS能够访问到，但是很少有人这么做。