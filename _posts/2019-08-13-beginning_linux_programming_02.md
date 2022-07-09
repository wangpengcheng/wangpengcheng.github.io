---
layout:     post
title:      Linux程序设计 学习笔记 (二)
subtitle:   Linux程序设计 学习笔记 (二)
date:       2019-08-13
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - C/C++
    - Linux
    - 操作系统
    - 程序设计
---

## 参考链接：

- [Linux内核文档首页](https://www.kernel.org/doc/Documentation/)
- [Linux文档](https://linux.die.net/)
- [Linux c 开发手册](https://legacy.gitbook.com/book/wizardforcel/linux-c-api-ref/details)
- [Linux Kernel API](https://www.kernel.org/doc/htmldocs/kernel-api/index.html)
- [书中代码地址](http://www.wrox.com/WileyCDA/WroxTitle/Beginning-Linux-Programming-4th-Edition.productCd-0470147628,descCd-DOWNLOAD.html)

## 第三章 文件操作

### 3.1 linux文件结构
_参考链接：_　
- [每天进步一点点——Linux中的文件描述符与打开文件之间的关系](https://blog.csdn.net/cywosp/article/details/38965239)；
- [Linux文件读写机制及优化方式](https://blog.csdn.net/u014743697/article/details/52663975);
- [【Linux学习笔记】标准IO缓冲：行缓冲、全缓冲、无缓冲](https://blog.csdn.net/LYJwonderful/article/details/80646602)

linux中一切皆是文件。

文件除了本身的文件内容外，还有文件的附属管理信息，这些保存在`inode`节点中。目录是用于保存其他文件的节点号和节点名字的文件，目录文件中的每个数据项都是指向某个文件节点的链接，删除文件名就是删除与之对应的链接。
删除文件时，就是删除了文件的目录项，并且指向该文件的链接数目-1。文件中的数据仍然能够通过其它指向链接访问到。被链接数为0时，才会被正真删除。

引文linux中一切皆是文件的思想，其中硬件设备都是文件，可以使用，mount进行设备的挂载：

```
mount -t iso9660 /dev/hdc/ /mnt/cdrom
cd /mut/cdrom
```

重要的控制设备目录有三个：

- /dev/console:控制台设备
- /dev/tty: 控制终端
- /dev/null: 空设备，所有向这个设备中的输出都将被丢弃。

操作系统的内核是一组 **设备驱动程序**；内核提供了用于访问设备驱动程序的底层函数(系统调用)：open、read、write、close、ioctl等相关信息。

输入输出底层系统调用的效率非常低。
- 会影响系统性能：linux需要从用户态，切换到内核状态。因此应该每次读写大量的数据而不是每次仅仅读写一个字符。
- 硬件会限制对底层系统调用一次能读写的数据块的大小。

针对上述情况，linux提供了标准的函数接口`stdio.h`。

![文件读取调用](https://wangpengcheng.github.io/img/2019-08-24-14-17-46.png)

#### 3.4.1 write调用

格式：
```
#include <unistd.h>

size_t write(int fildes,const void *buf,size_t nbytes);
```

将缓冲区buf的前nbytes个字节写入文件描述符`fildes`关联的文件中返回实际的字节数，如果存在错误，返回的实际字节数可能小鱼`nbytes`。

#### 3.4.2 read系统调用

```c
#include <unistd.h>

size_t read(int fildes,void *buf, size_t nbytes);

```

和上面基本相同只是变成了读取

```c

#include <unistd.h>
#include <stdlib.h>

int main()
{
    char buffer[128];
    int nread;

    nread = read(0, buffer, 128);
    if (nread == -1)
        write(2, "A read error has occurred\n", 26);

    if ((write(1,buffer,nread)) != nread)
        write(2, "A write error has occurred\n",27);

    exit(0);
}

# input : hello there | ./simple_read
# output :hello there
```

#### 3.4.3 open 系统调用

```c
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int open(const char *path,int oflags);
int open(const char *path,int oflags,mode_t mode);

```
open建立了一条文件或者设备的访问方式。调用成功，返回一个可以被read、write和其它系统调用的文件描述符fildes。这个不会和其它正在运行中的进程共享。即便是同一文件，不同进程，也会产生不同的共享访问符号。因此文件同时读写的时候，会相互副高。一般使用文件锁来防止出现冲突。

![open的访问模式](https://wangpengcheng.github.io/img/2019-08-24-14-41-31.png)

POSIX规范还标准化了一个creat调用，相当于`oflags=O_CREAT|O_WONLY|O_TRUNC`

#### 3.4.4 访问权限设置



使用`O_CREAT`标志的open滴啊用来创建文件时，需要用3个参数格式的open调用。第三个参数mode是几个标志按位或后得到的。

- S_IRUSR:读权限，文件属主。
- S_IWUSR:写权限，文件属主。
- S_IXUSR:执行权限，文件属主。
- S_IRGRP:读权限，文件所属组。
- S_IWGRP:写权限，文件所属组。
- S_IXGRP:执行权限，文件所属组。
- S_IROTH:读权限，其它用户。
- S_IWOTH:写权限，其它用户。
- S_IXOTH:执行权限，其它用户。

示例：
```c

open("myfile",O_CREAT,S_IRUSR|S_IXOTH);

```
##### 3.4.4.1 umask

这里的权限设置，实际上海市发送权限请请求，是否被设置，取决于当时umask的值。

![umask取值的含义1](https://wangpengcheng.github.io/img/2019-08-24-14-56-18.png)

![umask取值的含义2](https://wangpengcheng.github.io/img/2019-08-24-14-57-20.png)

chmod操作也就是在直接更改umask的值。

##### 3.4.4.2 close系统调用

```c
#include <unistd.h>

int close(int fildes);

```
这里使用close主要是滴啊用终止文件描述符fildes与文件之间的关联。文件描述符被释放能够重新使用。返回正确为0，返回错误为-1

注意：对于网络文件系统，可能不会在关闭文件之前报告文件写操作中出现的错误。因为在执行写操作时，数据可能未被确认写入。

##### 3.4.4.3 ioctl系统调用

```c
#include <unistd.h>

int ioctl(int fildes,int cmd,...); 
```

提供了一个用于控制设备及其描述符行为和配置服务的借口。

#### 3.4.5 其它文件管理有关的系统调用

##### 3.4.5.1 lseek系统调用

对文件描述符`fildes`的读写指针进行设置，可以用来设置文件的下一个读写位置。位置可以是文件中的绝对位置，也可以使相对于当前位置或者文件尾部的某个而相对位置。

```c
#includ <unistd.h>

#includ <sys/types.h>

off_t lseek(int fildes,off_t offset,int whence);

```
whence是一个枚举值，用来定义该值便宜的用法：

- SEEK_SET: offset是一个绝对值
- SEEK_CUR: offset是相对于当前位置的一个相对位置。
- SEEK_END: offset是相对于文件尾的一个相对位置。

##### 3.4.5.2 fstat、stat、和lstat系统调用

这些是文件描述符相关的问价状态信息，该信息将会写到一个buf结构中，buf的地址以参数形式传递给fstat。其原型如下：

```
#include <unistd.h>

#include <sys/stat.h>

#include <sys/types.h>

int fstat(int fildes,struct stat *buf);
int stat(const char *path,struct stat *buf);
int lstat(const char *path,struct stat *buf);

```

stat和lstat返回的是通过文件名查找到的状态信息。但是当文件是符号链接时，lstat返回符号链接本身信息，stat返回 **符号链接指向的信息**。stat结构成员内容如下：

![结构体成员](https://wangpengcheng.github.io/img/2019-08-24-15-30-18.png)

st_mode关键字定义在头文件`sys/stat.h`中。这些宏包括对访问权限、问价类型标志以及一些用于帮助测试特定类型和权限的掩码的定义。

- S_IFBLK: 文件是一个特殊的块设备。
- S_IFDIR: 文件是一个目录
- S_IFCHR: 文件是一个特殊的字符设备
- S_IFIFO: 文件是一个FIFO(命名管道)
- S_IFREG: 文件是一个普通文件。
- S_FLNK: 文件是一个链接符号。

其它模式标志：

- S_ISUID:文件设置了SUID位。
- S_ISGID:文件设置了SGID位。

![其它文件类型](https://wangpengcheng.github.io/img/2019-08-24-15-36-41.png)

##### 3.4.5.3 dup和dup2系统调用

```c
#include <unistd.h>

int dup(int fildes);
int dup2(int fildes,int fildes2);
```

上面主要是复制文件描述符，可以通过更多的文件描述符访问同一个文件。

### 3.5 标准I/O库

_参考链接：_ [Linux下标准I/O缓冲机制](https://blog.csdn.net/gkzscs/article/details/83033337)

与c标准I/O相同，但是与C++中的输入和书而出流不同，c++中是单独的输出流模型，同时为了兼容C，与C的输入输出同步，会比较慢，因此可以使用`ios::sync_with_stdio(false);`,这样c++流会块很多。

其中的主要函数有：

- fopen:
- fclose:
- fread:
- fwrite:
- fflush:将文件流中的所有未写出的数据立刻写处理啊，确保程序继续执行前数据都已经被写到磁盘上了，flclose中隐含了一次fflush调用。
- fseek:与lseek对应的文件流函数。为下一次读写操作指定位置。

```c++
#include <stdio.h>

int fseek(FILE *stream,long int offset, int whence);

```

- fgetc,getc,getchar:fgetc和getc基本相同，都是从文件流中取出一个字符返回，当它达到文件文件末尾过着出现错误时返回EOF；getchar相当于get(stdin),从标准输入里读取下一个字符。

- fputc、putc和putchar和基本一致，不过多了输出字符参数`c`。
- fgets和gets：前者从文件流stream中读取一个字符串，遇到换行符或者结束文件末尾，将换行符传递到接收字符串中并添加`\n`表示字符串结尾，因此实际输出字符串数目为`n-1`。后者从标准输入中读取数据并丢弃遇到的换行符。在接收字符传的尾部加上一个null字符。

注意： gets对传输字符的个数并没有限制，所以可能会溢出自己的传输缓冲区。因此，你应该避免使用它，并用fgets来代替。([[Linux]标准IO全缓冲和行缓冲](https://www.cnblogs.com/yiyide266/p/10566160.html))


### 3.6 格式化输入和输出

- print、fprintf和sprintf:第一个将其送到标准输出.fprintf将自己输出输送到一个指定的文件流。sprintf将输出和一个结尾空字符写到作为参数传递过来的字符串s里。
![输出格式1](https://wangpengcheng.github.io/img/2019-08-24-19-17-41.png)
![输出格式2](https://wangpengcheng.github.io/img/2019-08-24-19-18-16.png)
- scanf、fscanf、sscanf：与printf基本相同。
![输入格式](https://wangpengcheng.github.io/img/2019-08-24-19-19-55.png)
- fgetpos:获取当前文件流读写的位置。
- fsetpos:设置文件流的当前读写位置
- ftell:返回文件流当前读写位置的偏移
- freopen:重新使用一个文件流。
- setvbuf:设置文件流的缓冲机制
- remove:相当于unlink函数，如果path是一个目录，则相当于rmdir函数。
- ferror:测试一个文件流的错误标识，如果该标识被设置就返回一个非零值，否则返回0
- feof:测试一个文件流的文件尾标识，被设置则返回非零值。否则返回0.
- clearerr:重新清除错误信息。
- fileno:获取文件流使用的底层文件描述符。失败返回-1
- fdopen:在已经打开的文件描述符上，创建一个新的文件流。实际所用是为一个已经打开的文件描述符提供stdio缓冲区。


### 3.7 文件和目录的维护

- chmod:改变文件或者目录的访问权限

```cpp
#include <sys/types.h>
#include <unistd.h>

int chown(const char *path,uid_t owner,git_t group);
```

- unlink、link和symlink:管理文件链接数目，具体的可以参考第二章中内容。


```cpp
#include <unistd.h>

int unlink(const char *path);
int link(const char *path1,const char *path2);
int symlink(cosnt char *path1,const char *path2);
```

- mkdir和rmdir:文件夹的创建与删除。
- chdir和getcwd:访问和文件夹路径，获取当前文件夹路径。
- opendir: 创建一个目录，并返回创建的目录流。如果成功，它返回一个指向DIR结构的指针，可以用于读取目录数据。
- readdir:返回一个指针，结构里，保存一个目录流dirp中下一个目录的资料，直到返回NULL;
- telldir:返回目录流中当前的位置。
- seekdir:设置目录流dirp的目录项指针。loc的值用来设置指针位置。

```c
#include <sys/types.h>
#include <dirent.h>

void seekdir(DIR *dirp,long int loc);
```

- closedir：关闭目录流。

一个目录扫描程序

```cpp
#include <unistd.h>

#include <stdio.h>

#include <dirent.h>

#include <string.h>

#include <sys/stat.h>

#include <stdlib.h>

void printdir(char *dir, int depth)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    if((dp = opendir(dir)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }
    chdir(dir);
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            /* Found a directory, but ignore . and .. */
            if(strcmp(".",entry->d_name) == 0 || 
                strcmp("..",entry->d_name) == 0)
                continue;
            printf("%*s%s/\n",depth,"",entry->d_name);
            /* Recurse at a new indent level */
            printdir(entry->d_name,depth+4);
        }
        else printf("%*s%s\n",depth,"",entry->d_name);
    }
    chdir("..");
    closedir(dp);
}

/*  Now we move onto the main function.  */

int main()
{
    printf("Directory scan of /home:\n");
    printdir("/home",0);
    printf("done.\n");

    exit(0);
}

```

### 3.9 错误处理

错误代码存在子啊头文件`errno.h`中，关键内容如下所示：

![错误处理图片](https://wangpengcheng.github.io/img/2019-08-24-20-03-29.png)

### 3.10 /proc文件处理系统

linux提供了一个特殊的文件系统procfs，通常以/proc目录的形式呈现。包含了许多特殊文件用来对驱动程序和内核信息进行更高层次的访问。只要有正确的访问权限，就可以通过读写这些文件来获取信息或者设置参数。

proc文件夹中包含以PID命名的文件夹和其它设备文件夹，设备的基本信息就放在其中。可以直接读取这些文件就可以获取其状态信息。例如：`cat /proc/cpuinfo`

输出信息如下：
```shell

processor   : 0
vendor_id   : GenuineIntel
cpu family  : 6
model       : 94
model name  : Intel(R) Core(TM) i5-6400 CPU @ 2.70GHz
stepping    : 3
microcode   : 0xcc
cpu MHz     : 1652.835
cache size  : 6144 KB
physical id : 0
siblings    : 4
core id     : 0
cpu cores   : 4
apicid      : 0
initial apicid  : 0
fpu     : yes
fpu_exception   : yes
cpuid level : 22
wp      : yes

......

```

可以使用`cat /proc/net/sockstat`获得网络套接字的使用统计。

### 3.11 高级主题 fcnt1和mmap

#### 3.11.1 fcnt1系统调用

- `fcnt1(fildes,F_DUPFD,newfd)`:返回一个新的文件描述符，其数值等于或者大于整数`newfd`;
- `fcnt1(fildes,F_GETFD)`：返回在fcntl.h头文件中定义的文件描述符标志。其中包括FD_CLOEXEX，它的作用是决定是否在成功调用了某个exec系列的系统调用之后关闭该文件描述符。
- `fcnt1(fildes,F_SETFD，flag)`:设置文件描述符标志
- `fcnt1(fildes,F_GETFL)`和`fcnt1(fildes,F_GETFL，flags)`:获取和设置文件庄涛标志和访问模式。

可以使使用fcntl实现建议性文件锁

#### 3.11.2 mmap函数(内存映射)

建立一段可被两个或更多个程序读写的内存。一个程序对它所做出的修改可以被其它程序看见。

mmap创建一个指向一段内存区域的指针，该内存区域与可以通过一个打开文件的描述符访问的内容相关。
```c++
#include <sys/mman.h>

void *mmap(void *addr,size_t len,int prot,int flags,int fildes,off_t off);
```
使用off参数来改变，经过共享内存段访问的文件中数据的起始偏移。打开的文件描述符有`fildes`参数给出。可以访问的数据量(内存段的成都)由len参数设置。
使用addr来请求地址，如果取值为0，结果指针将会自动分配。
prot参数用于设置内存段的访问权限，下面是其按位OR的结果

- `PROT_READ`:允许读该内存段。
- `PROT_WRITE`:允许写该内存段。
- `PROT_EXEC`:允许执行该内存段。
- `PROT_NONE`:该内存段不能被访问。
flags参数影响如下：

![选项图片](https://wangpengcheng.github.io/img/2019-08-30-14-14-16.png)

使用`msync`函数：把在该内存段的某个部分或整段中的修改写回到被映射的文件中(或者从被映射文件里读出)

```c++
#include <sys/mman.h>
int msync(void *addr,size_t len,int flags);
```
flags参数如下所示：

![flags相关参数](https://wangpengcheng.github.io/img/2019-08-30-14-24-19.png)

释放内存段

```c

/*  We start by defining a RECORD structure
    and then create NRECORDS versions each recording their number.
    These are appended to the file records.dat. 
*/

#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>

typedef struct {
    int integer;
    char string[24];
} RECORD;

#define NRECORDS (100)

int main(int argc,char* argv[])
{
    //使用record数据结构用来保存相关记录和编号。

    RECORD record, *mapped;
    int i, f;
    FILE *fp;

    fp = fopen("records.dat","w+");
    for(i=0; i<NRECORDS; i++) {
        record.integer = i;
        //输出相关信息

        sprintf(record.string,"RECORD-%d",i);
        //将信息写入到文件中

        fwrite(&record,sizeof(record),1,fp);
    }
    fclose(fp);

    fp = fopen("records.dat","r+");
    //重置seek中的记录为第43条

    fseek(fp,43*sizeof(record),SEEK_SET);
    //读取相关数据

    fread(&record,sizeof(record),1,fp);

    record.integer = 143;
    //输出相关的信息

    sprintf(record.string,"RECORD-%d",record.integer);

    fseek(fp,43*sizeof(record),SEEK_SET);
    fwrite(&record,sizeof(record),1,fp);
    fclose(fp);


    //将记录映射到内存中，访问第43条记录，把它的整数值改为243(同时更新该记录中的字符串)
    
    //打开文件

    f = open("records.dat",O_RDWR);
    //进行函数映射

    mapped = (RECORD *)mmap(0, NRECORDS*sizeof(record), 
                          PROT_READ|PROT_WRITE, MAP_SHARED, f, 0);

    //获取mmap中的第43个数据

    mapped[43].integer = 243;
    //将数据保存在mapped[43].string中

    sprintf(mapped[43].string,"RECORD-%d",mapped[43].integer);
    //将修改添加到文件中

    msync((void *)mapped, NRECORDS*sizeof(record), MS_ASYNC);
    //销毁映射的内存。

    munmap((void *)mapped, NRECORDS*sizeof(record));
    close(f);

    exit(0);
}
```

## 4 Linux环境

在linux命令行中，建议每一个命令行开关都应该以一个短横线开头，其后包含单个字母或数字。获取参数和检查的代码如下：

```c++
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int arg;
    for(arg = 0; arg < argc; arg++) {
        if(argv[arg][0] == '-')
            printf("option: %s\n", argv[arg]+1);
        else
            printf("argument %d: %s\n", arg, argv[arg]);
    }
    exit(0);
}
```

linux中提供了getopt函数，支持需要关联值和不需要关联值的选项

```c++
#include <unistd.h>

int getopt(int aegc,char *const argv[],const char *optstring);
extern char *optarg;
extern int optind,opterr,optopt;
```

使用示例如下：

```c++
#include <stdio.h>

#include <unistd.h>

#include <stdlib.h>

int main(int argc, char *argv[])
{
    int opt;

    while((opt = getopt(argc, argv, ":if:lr")) != -1) {
        switch(opt) {
        case 'i':
        case 'l':
        case 'r':
            printf("option: %c\n", opt);
            break;
        case 'f':
            printf("filename: %s\n", optarg);
            break;
        case ':':
            printf("option needs a value\n");
            break;
        case '?':
            printf("unknown option: %c\n", optopt);
            break;
        }
    }
    for(; optind < argc; optind++)
        printf("argument: %s\n", argv[optind]);
    exit(0);
}

//输入： ./argopt -i -lr 'hi there' -f fred.c -q

/*
输出：

option: i
option: l
option: r
filename: fred.c
unknown option: q
argument :hi there

*/

```

getopt_long:接受以双划线(--)开始的长参数。使用示例如下：

```c++
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <getopt.h>

int main(int argc, char *argv[])
{
    int opt;
    struct option longopts[] = {
	{"initialize", 0, NULL, 'i'},
	{"file", 1, NULL, 'f'},
	{"list", 0, NULL, 'l'},
	{"restart", 0, NULL, 'r'},
    //注意必须包含全为0的结构结尾

	{0,0,0,0}
    };

    while((opt = getopt_long(argc, argv, ":if:lr", longopts, NULL)) != -1) {
        switch(opt) {
        case 'i':
        case 'l':
        case 'r':
            printf("option: %c\n", opt);
            break;
        case 'f':
            printf("filename: %s\n", optarg);
            break;
        case ':':
            printf("option needs a value\n");
            break;
        case '?':
            printf("unknown option: %c\n", optopt);
            break;
        }
    }
    for(; optind < argc; optind++)
        printf("argument: %s\n", argv[optind]);
    exit(0);
}


```

![option结构体](https://wangpengcheng.github.io/img/2019-08-30-15-42-39.png)

### 4.2 环境变量

linux中使用`getenv(const char* env_name)`和`int putenv(const char *string)`来进行环境变量的读取和设置。

### 4.3 日期和时间

linux中可以使用time函数来得到底层的时间值，它返回的是从纪元开始至今的秒数，tloc不是一个空指针，time函数还会把返回值写入tloc指针指向的位置。

- difftime(time_t time1,time_t time2): 计算两个时间点之间的时间差。值作为浮点数返回。
- `struct tm *gmttime(const time_t timeval)`:将底层时间值分解为一个结构函数。

![tm结构的成员函数](https://wangpengcheng.github.io/img/2019-08-30-21-22-52.png)

```c++
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int i;
    time_t the_time;

    for(i = 1; i <= 10; i++) {
        the_time = time((time_t *)0);
        printf("The time is %ld\n", the_time);
        sleep(2);
    }
    exit(0);
}

```
下面是gmtime函数的使用示例：

```c++
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    struct tm *tm_ptr;
    time_t the_time;
    (void) time(&the_time);
    tm_ptr=gmtime(&the_time);

    printf("Raw time is %ld\n",the_time);
    printf("gmtime gives \n");
    printf("date: %02d/%02d/%02d\n", 
        tm_ptr->tm_year,tm_ptr->tm_mon+1,tm_ptr->tm_mday);
    printf("time: %02d:%02d:%02d\n",
        tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
    exit(0);

}
```

- `localtime(const time_t *timeval)`:会根据时区进行更正。
- `mktime(struct tm *timeptr)`:将timeptr转换为原始的mktime函数。
- `char *asctime(const struct tm *timeptr)`:返回时间字符串。表示实际的时间。
- `char *ctime(const time_t *timeval)`:相当于`asctime(localtime(timeval))`,转换为更简单的本地时间。
- `size_t strftime(char *s,size_t maxsize,const char *format, struct tm *timeptr)`:函数格式化timeptr指针指向的tm结构所表示的时间和日期，并将结果放在字符串s中。
- `size_t strptime(const char *buff,const char *format,struct tm *timeptr)`:函数格式化timeptr指针指向的tm结构所表示的时间和日期，并将结果放在字符串s中。

![strftime的一般格式](https://wangpengcheng.github.io/img/2019-08-30-21-44-41.png)

### 4.4 临时文件

linux中可以使用相关的函数，进行临时文件的操作。

- `char *tmpnam(char *s)`:生成一个唯一的文件名。但是注意这里可能会存在另外一个程序创建出的文件名同名的文件。
- `FILE *tmpfile(void)`:生成唯一的文件索引，可以避免重名的情况发生。返回一个文件流指针。该文件以读写的方式打开(通过w+方式的fopen),当对它的所有引用全部关闭时，该文件会被自动删除。
- `char *mktemp(char* template)/int mkstemp(char *template)`：指定函数模板返回创建的文件。template参数必须是一个以6个x字符结尾的字符串。

### 4.5 用户信息

在`sys/types.h`中存在类型`uid_t`。它通常是一个小整数，一般情况下UID的值都大于100。可以使用`getuid`函数返回程序关联的UID，它通常是启动程序的用户的UID。getlogin函数返回与当前用户关联的登录名称。

用户账号数据库。通常保存在系统文件`/etc/passwd`中。每行分别对应用户的：用户名、加密口令、用户标识符(UID)、组标识符(GID)、全名、home目录、默认shell.

由此可以实现用户名和密码的相关查找信息可以进行修改和访问,密码信息一般放在`/etc/shadow`文件中。用相关的密码访问接口。

```c++
#include <sys/types.h>

#include <pwd.h>

struct passwd *getpwuid(uid_t uid);
struct passwd *getpwnam(const char *name);
```

下面是passwd关键的数据结构

![passwd相关数据结构](https://wangpengcheng.github.io/img/2019-09-02-14-42-41.png)

从密码数据库中提取相关信息
```c++

#include <sys/types.h>
#include <pwd.h>

#include <stdio.h>

#include <unistd.h>

#include <stdlib.h>

int main()
{
    //定义uid和

    uid_t uid;
    gid_t gid;
    //定义结构体

    struct passwd *pw;
    uid=getuid();
    gid=getgit();

    printf("User is %s \n",getlogin());

    printf("User IDs: uid=%d,gid=%d\n",uid,gid);
    pw=getpwuid(uid);
    //输出对应的参数

    printf("UID passwd entry:\n name=%s, uid=%d,home=%s,shell=%s \n",pw->pw_name,pw->pw_uid,pw->pw_gid,pw->pw_dir,pw->pw_shell);
    //获取root用户的相关信息

    pw=getpwnam("root");
    printf("root passwd entry: \n");
    printf("UID passwd entry:\n name=%s, uid=%d,home=%s,shell=%s \n",pw->pw_name,pw->pw_uid,pw->pw_gid,pw->pw_dir,pw->pw_shell);
    exit(0);
}

```
### 4.6 主机信息

主机信息结构体`utsname`存储在`sys/utsname.h`文件中。可以使用`int gethostname(char* name,size_t namelen)`函数来进行查询。成功返回0，否则返回-1。

```c++
#include <sys/utsname.h>

int uname(struct utsname *name);

```
![uname相关参数](https://wangpengcheng.github.io/img/2019-09-02-14-59-20.png)

下面是简单的使用示例：

```c++

#include <sys/utsname.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char computer[256];
    struct utsname uts;

    //获取相关参数
    if(gethostname(computer, 255) != 0 || uname(&uts) < 0) {
        fprintf(stderr, "Could not get host information\n");
        exit(1);
    }

    printf("Computer host name is %s\n", computer);
    printf("System is %s on %s hardware\n", uts.sysname, uts.machine);
    printf("Nodename is %s\n", uts.nodename);
    printf("Version is %s, %s\n", uts.release, uts.version);
    exit(0);
}

```

### 4.7 使用日志

一般linux的系统日志文件保存在`/usr/adm`或者`/var/log`目录中。文件`/var/log/message`包含所有的系统信息。`/var/log/mail`包含来自邮件系统的其它日志信息，`/var/log/debug`可能包含调试信息。具体的可以根据`/etc/syslog.conf`文件或者`/etc/syslog-ng/sys-log-ng.conf`文件来检查系统的配置。我的Ubuntu 18.04 优麒麟存在于`/etc/rsyslog.conf`文件中。

可以使用如下的API进行操作

```c++

#include <syslog.h>
//输出日志函数

void syslog(int priority,const char* message,arguments...);
//关闭和打开日志函数

void closelog(void);
void openlog(const char *ident,int logopt,int facility);
//设置日志掩码,控制日志的优先级

int setlogmask(int maskpri);

```
这个函数向系统的日志发送一条日志信息。每条信息中的priority参数对应了日志的级别。

![LOG_USER相对值](https://wangpengcheng.github.io/img/2019-09-02-15-44-49.png)

![logopt参数选项](https://wangpengcheng.github.io/img/2019-09-02-15-48-42.png)

下面是简单是示例：

```c++
#include <syslog.h>

#include <stdio.h>

#include <unistd.h>

#include <stdlib.h>

int main()
{
    int logmask;

    openlog("logmask", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_INFO,"informative message, pid = %d", getpid());
    syslog(LOG_DEBUG,"debug message, should appear");
    logmask = setlogmask(LOG_UPTO(LOG_NOTICE));
    syslog(LOG_DEBUG,"debug message, should not appear");
    exit(0);
}
```

### 4.8 资源和限制

在`limit.h`中定义了许多代表操作系统方面限制的显示。相关常量如下所示：

![限制常量使用](https://wangpengcheng.github.io/img/2019-09-02-15-58-48.png)

相关的设置函数

```c++
#include <sys/resource.h>

int getpriority(int which,id_t who);
int getpriority(int which,id_t who,int priority);
int getrlimit(int resource,struct rlimit *r_limit);
int getrlimit(int resource,const struct rlimit *r_limit);
int getrusage(int who,struct rusage *r_usage);
```
cpu耗费时间参数

![确定cpu耗费时间参数](https://wangpengcheng.github.io/img/2019-09-02-16-03-03.png)

其中`timeval`结构定义在头文件`sys/time.h`中。

一个程序耗费的CPU时间可分为：

- 用户时间： 程序自身的指令所耗费等待时间。
- 系统时间： 操作系统为程序执行所需要花费的时间，即执行输入输出操作的系统调用或其它系统函数所花费的时间。

**注意：多线程中程序运行时间的统计不能使用`ctime.h`中的`clock()`函数；因为它是根据cpu总的cpu运转时钟来的，会比单个的时间长；使用`time`(最小单位为s)或`timeval`(纳秒和微秒)函数来通过系统时间对多线程耗费的时间进行统计计算。([多线程C++运行时间统计](https://blog.csdn.net/u012526003/article/details/80862801))**

可以使用getrusage函数将cpu时间信息写入参数`r_usage`指向的`rusage`结构中。参数`who`可以是一下常量：

![who常量选值](https://wangpengcheng.github.io/img/2019-09-02-16-24-09.png)

使用which参数确定优先级

![获取用户当前优先级](https://wangpengcheng.github.io/img/2019-09-02-16-25-17.png)

使用`getrlimit`和`setrlimit`来读取和设置系统资源方面的限制函数。

![软件限制参数](https://wangpengcheng.github.io/img/2019-09-02-16-45-10.png)

linux中一般的线程数量限制([Linux最大线程数限制](https://www.cnblogs.com/guojintao/articles/10389713.html);[Linux资源限制](https://blog.csdn.net/qq_36441027/article/details/81040229))为
- 系统：62228
- 用户最大进程数：1024
- 单个进程最大线程数：1024
- 线程栈的大小(一般限制因素)：8M([Linux最大线程数限制](https://blog.csdn.net/qq_37924084/article/details/78403764))

除了线程数和内存，linux中还有其它许多因素可以进行限制；它们一般由`rlimit`函数中的`resource`参数指定。在头文件`sys/resource.h`中定义

![相关限制参数](https://wangpengcheng.github.io/img/2019-09-02-16-56-19.png)

## 第五章 终端

终端输入和输出的模式：

1. 标准模式与非标准模式：
- 标准模式：用户按下回车键之后，程序接受终端的输入，允许退格和删除。
- 非标准模式：用户输入接收程序的设置。

linux会暂存用户输入的内容，直到用户按下回车键。linux终端的输出缓冲设置是行缓冲。因此会以行为单位进行输出和输入。

注意：linux/unix都是以换行符`\n`表示一行的结束，并不是换行符，其它操作系统(如MS-DOS)用回车符和换行符两个自读的结合来表示一行的结束。使用重定向输出(`>`或`>>`)和输入(`<`或`<<`)可以重新定义输入和输出。

使用`unistd.h`中的`int isatty(int fd)`可以判断出该描述符是否连接到了一个终端(连接返回1，否则返回0)。

linux中的`/dev/tty`特殊设备直接对终端进行读写，指向当前的终端或登录会话.

### 5.3 终端驱动程序和通用终端接口

linux中可以使用通用终端接口(GTI)来控制终端。

![终端模型结构图](https://wangpengcheng.github.io/img/2019-09-03-20-17-24.png)

主要的控制功能有：

- 行编辑:是否允许使用退格键进行编辑。
- 缓存：是立即读取字符，还是等待一段可配置的延迟之后再读取它们。
- 回显：允许控制字符的回显，例如读取密码时。
- 回车/换行(CR/LF):定义如何在输入/输出时映射回车/换行符，比如打印`\n`字符时如何处理。
- 线速:很少用于PC控制台，但对调制解调器或通过串行线连接的终端非常重要。

下面是uinux中的硬件模型

![设备模型](https://wangpengcheng.github.io/img/2019-09-03-20-31-02.png)

### 5.4 termios结构

termios数据结构和相关单数调用都定义在头文件`termios.h`中，当使用其中的函数时，需要添加动态连接库`ncurses`。

![最小termios的结构体定义](https://wangpengcheng.github.io/img/2019-09-03-20-39-54.png)

可以使用`int tcgetattr(int fd,struct termios *termios_p)`来进行结构体的初始化，再使用`int tcsetattr(int fd,int actions,const strcut termios *termios_p)`来进行对应值的修改。

action选项如下：
- TCSANON:立刻对值进行修改。
- TCSADRAIN:等当前的输出完成之后再对值进行修改。
- TCSAFLUSH:等当前的输出完成之后再对值进行修改,但丢弃还未从`read`调用返回的当前可用的任何输入。
设置的值，影响的功能按照不同的模式被分成一下几组：

- 输入模式：输入模式控制输入数据(终端驱动程序从串行口或者键盘接收到的字符)在被传递给程序之前的处理方式。设置`c_iflag`成员的标志进行控制，其可选值如下：

![c_iflags相关选项](https://wangpengcheng.github.io/img/2019-09-03-20-48-47.png)

- 输出模式：控制输出模式的字符串，通过设置`c_oflag`成员的标志对输出模式进行控制。选项如下所示：
![输出标志设置](https://wangpengcheng.github.io/img/2019-09-03-20-55-51.png)

注意：如果没有设置OPOST，则所有其它标志都被忽略。
- 控制模式：控制终端的硬件特性。`c_cflag`成员的标志对控制模式进行配置。可选参数如下：

![硬件相关设置](https://wangpengcheng.github.io/img/2019-09-03-20-58-38.png)

- 本地模式：控制终端的各种特性。更改`c_lflag`参数，成员的宏内容如下：

![c_lflag宏定义内容](https://wangpengcheng.github.io/img/2019-09-03-21-11-59.png)

这里最重要的两个标志是ECHO(抑制键入字符的回显)和ICANON(将终端在两个截然不同的接收字符处理模式间进行切换，设置了ICANON标志，就启用标准输入行进行处理模式，否则，就启用非标准处理模式)。

- 特殊控制字符：对于一些特殊的组合按键(如`ctrl+c`)进行处理。并且使用`c_cc`数组成员将各种特殊控制字符映射到对应的支持函数。每个字符的位置是由一个宏进行定义的，但并不限制这些字符必须是控制字符。根据模式的不同数组下标不同
    + 标准模式的数组下标：

![标准模式的数组下标](https://wangpengcheng.github.io/img/2019-09-03-21-33-43.png)
    + 非标准模式的数组下标：

![非标准模式下的数组下标](https://wangpengcheng.github.io/img/2019-09-03-21-35-32.png)

#### 5.4.2 特殊控制字符

![特殊控制字符](https://wangpengcheng.github.io/img/2019-09-03-21-36-41.png)

#### 5.4.3 TIME和MIN值

TIME和MIN的值只能用于非标准模式，两者结合起来共同控制对输入的读取。两者的结合分为四中情况：

- MIN=0&&TIME=0:read调用总是立刻返回，如果存在待处理的字符，就会返回；如果没有就会返回0,并且不读取任何字符。
- MIN=0&&TIME>0:字符可以处理或者是经过TIME个十分之一秒的时间间隔，read调用就返回。如果因为超时而未读到任何字符，read就返回0，否则返回读取的字符数目。
- MIN>0&&TIME=0:这种情况下，read将一直等待，直到有MIN个字符可以读取时才返回读取字符的数量。达到文件尾时返回0.
- MIN>0&&TIME>0:当read被调用时，会等待接收一个字符，在接收到一个字符以及后续的每个字符之后，一个字符间隔定时器被启动(或者重启)。当有MIN个字符可读或两个字符之间的时间间隔超过TIME*0.1s时，read调用返回。

#### 5.4.4 通过shell王文终端模式


```shell
#查看当前的termios.h设置情况

stty -a

#重新设置标识符的快捷键

stty sane
# 通过保存设置和重新读取来更改终端设置

#输出和保存设置

stty -g > save_stty
#读取设置

stty $(cat save_stty)

#直接操作设置

stty -icanon min 1 time0

#重新恢复启用回显功能

stty -echo

```

#### 5.4.5 终端速度

使用如下函数更改终端的刷新速度

```c++
#include <termios.h>
speed_t cfgetispeed(const struct termios *);
speed_t cfgetospeed(const struct termios *);
int cfsetispeed(struct termios *,speed_t speed);
int cfsetospeed(struct termios *,speed_t speed);
```
speed_t 相关参数：

- B0:挂起终端
- B1200：1200波特
- B2400：2400波特
- B9600：9600波特
- B19200：19200波特
- B38400：38400波特


#### 5.4.6 其它函数

```c++
#include <termios.h>

//让调用程序一直等待

int tcdrain(int fd);
//暂停或重新开始输出

int tcflow(int fd,int flowtypes);
//清空输入、输出或者两者都清空

int tcflush(int fd,int in_out_selector);

```

### 5.5 终端的输出

使用`curses.h`中的`terminfo()`函数可以实现相关信息的查询。

使用`echo $TERM`可以识别当前的终端类型.终端的相关信息保存在`/usr/lib/terminfo`或`/usr/share/terminfo`目录中。

查看终端输出设置

```shell
infocmp vt100 

#	Reconstructed via infocmp from file: /lib/terminfo/v/vt100
vt100|vt100-am|dec vt100 (w/advanced video),
    am, mc5i, msgr, xenl, xon,
    cols#80, it#8, lines#24, vt#3,
    acsc=``aaffggjjkkllmmnnooppqqrrssttuuvvwwxxyyzz{{||}}~~,
    bel=^G, blink=\E[5m$<2>, bold=\E[1m$<2>,
    clear=\E[H\E[J$<50>, cr=^M, csr=\E[%i%p1%d;%p2%dr,
    cub=\E[%p1%dD, cub1=^H, cud=\E[%p1%dB, cud1=^J,
    cuf=\E[%p1%dC, cuf1=\E[C$<2>,
    cup=\E[%i%p1%d;%p2%dH$<5>, cuu=\E[%p1%dA,
    cuu1=\E[A$<2>, ed=\E[J$<50>, el=\E[K$<3>, el1=\E[1K$<3>,
    enacs=\E(B\E)0, home=\E[H, ht=^I, hts=\EH, ind=^J, ka1=\EOq,
    ka3=\EOs, kb2=\EOr, kbs=^H, kc1=\EOp, kc3=\EOn, kcub1=\EOD,
    kcud1=\EOB, kcuf1=\EOC, kcuu1=\EOA, kent=\EOM, kf0=\EOy,
    kf1=\EOP, kf10=\EOx, kf2=\EOQ, kf3=\EOR, kf4=\EOS, kf5=\EOt,
    kf6=\EOu, kf7=\EOv, kf8=\EOl, kf9=\EOw, lf1=pf1, lf2=pf2,
    lf3=pf3, lf4=pf4, mc0=\E[0i, mc4=\E[4i, mc5=\E[5i, rc=\E8,
    rev=\E[7m$<2>, ri=\EM$<5>, rmacs=^O, rmam=\E[?7l,
    rmkx=\E[?1l\E>, rmso=\E[m$<2>, rmul=\E[m$<2>,
    rs2=\E>\E[?3l\E[?4l\E[?5l\E[?7h\E[?8h, sc=\E7,
    sgr=\E[0%?%p1%p6%|%t;1%;%?%p2%t;4%;%?%p1%p3%|%t;7%;%?%p4%t;5%;m%?%p9%t\016%e\017%;$<2>,
    sgr0=\E[m\017$<2>, smacs=^N, smam=\E[?7h, smkx=\E[?1h\E=,
    smso=\E[7m$<2>, smul=\E[4m$<2>, tbc=\E[3g,

```

可以使用`int setupterm(char *term,int fd,int *errret)`设置终端的输出功能。

### 5.6 检测击键动作

参考链接：[linux键盘驱动](https://www.cnblogs.com/lifexy/p/7553861.html)

使用一般方法进行相关的键盘检测。

### 5.7 虚拟控制台

linux中提供了虚拟控制台的功能，一组终端设备共享PC电脑的屏幕、键盘和鼠标。虚拟控制台可以通过字符设备文件/dev/ttyN进行使用。一般从数字1开始。




