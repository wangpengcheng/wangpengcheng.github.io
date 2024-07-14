---
layout:     post
title:      虚拟化/KVM面经
subtitle:   虚拟化KVM面经
date:       2023-04-01
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - KVM
    - QEMU
    - 虚拟化
    - 面试
---

# 参考链接
- [虚拟化工程师之路](https://wangpengcheng.github.io/2023/04/11/virtualization_engineer_router/)
- [KVM虚拟化常见面试题 | 常用命令整理](https://blog.csdn.net/weixin_43313333/article/details/128904292)
- [常用虚拟化命令](https://developer.aliyun.com/article/1072584)
- [kvm面试题](https://engineeringinterviewquestions.com/kvm-interview-questions-answers-pdf/)
- [20 Linux Virtualization Interview Questions and Answers](https://www.linuxtechi.com/linux-virtualization-interview-questions/)
- [腾讯云csig虚拟化部门一面面经](https://www.nowcoder.com/feed/main/detail/2e58841b38774b048cf98a0a6d670027?sourceSSR=search)
- [30 个 Openstack 经典面试问题和解答](https://zhuanlan.zhihu.com/p/51984152)


# 1. kvm 常用操作

## 1.1 如何修改虚拟机密码
用户密码重设通常使用libguestfs-tools 工具集内的virt-customize命令重设

```bash
#0. 安装libguestfs-tools
yum -y install libguestfs-tools
#1. 确认实例已关机
virsh  list --all | grep ${instance_name}

#2.  修改其系统盘密码
virt-customize -a /opt/${instance_name}.img --root-password password:test-123456

#3. 重启登录
virsh start ${instance_name}
virsh console ${instance_name}
# ctrl+']' 退出登录

```

___

- 参考：[常用虚拟化命令](https://developer.aliyun.com/article/1072584);[常用kvm命令](https://www.cnblogs.com/mrwuzs/p/11600018.html)



## 1.2 如何制作镜像


使用官网标准镜像创建虚拟机，并进行安装。以rock-linux为例
1. 创建数据盘
```bash
# 拉取最新的 Rocky 镜像
wget -c https://download.rockylinux.org/pub/rocky/9/isos/x86_64/Rocky-9.3-x86_64-minimal.iso /tmp/
# 创建系统盘
qemu-img create -f qcow2  /tmp/rocky-test-disk1.qcow2 50G
```

2. 创建虚拟机

```bash
#!/bin/bash
virt-install \
    --name rocky-test \ # 设置名称
    --graphics vnc,listen='0.0.0.0',port=5920 \ # 设置vnc
    --memory=8192,maxmemory=16384 \  # 设置内存8~16GB
    --vcpus=2,maxvcpus=4 \ # 设置cpu
    --cdrom=/tmp/Rocky-9.3-x86_64-minimal.iso \  # 设置启动镜像
    --disk /tmp/rocky-test-disk1.qcow2,size=50,format=qcow2 \ # 添加数据磁盘
    --network bridge=bridge0,model=virtio \ # 设置网络为bridge0 模式
    --hvm \
    --virt-type kvm \ #设置虚拟化方式
    --autostart \
    --os-type=linux \
    --os-variant=rocky9.3 // 这行如果报错的话可以去掉
```

3. vnc 登录安装
前往[vnc官网](https://www.realvnc.com/en/connect/download/viewer/)，下载VNC-viewer。并使用`${宿主机地址}:5920` 进行VNC登录。成功后进行如下设置
- 设置Time & Date (一般都是Shanghai)
- 设置ROOT 登录密码(RootPassword)
- 选择软件最小安装（Software Selection）
- 设置网络和主机名
- 设置设置安装位置并对系统盘进行分区
- 安装完后重启

4. 关闭防火墙和selinux

```bash
# 关闭防火墙
systemctl disable firewalld.service --now
# 关闭selinux
vim  /etc/selinux/config
# 设置SELINUX=disabled
# 设置强制
setenforce 0
# 查询信息
getenforce
```

5. 清除个性化设置并压缩镜像

```bash
# 清除主机个性化设置
virt-sysprep -d rocky-test
# 压缩镜像
virt-sparsify --compress rocky-test.qcow2 rocky-test-02.qcow2
```

##  1.3 virsh 常用命令

1. `virsh`：直接进交互模式
2. `virsh nodeinfo`：查看KVM节点（服务器）信息
3. `virsh list`：列出正在运行的虚拟机
4. `virsh list --all`：列出所有虚拟机（包括未启动的）
5. `virsh dominfo` 虚拟机名称：查看指定虚拟机的信息
6. `virsh start` 虚拟机名称：将指定的虚拟机开机
7. `virsh reboot` 虚拟机名称：将指定的虚拟机重启
8. `virsh shutdown` 虚拟机名称：将指定的虚拟机正常关机
9. `virsh destroy` 虚拟机名称：将指定的虚拟机强制关机（相当于拔电源）
10. `virsh autostart` 虚拟机名称：将指定的虚拟机设置随KVM自动开机
11. `virsh autostart --disable` 虚拟机名称：禁止自动开机
12. `virsh dumpxml centos7-2 > /tmp/vm.xml`: 导出虚拟机配置
13. `virsh console ins-xx`:进入虚拟机控制台
14. `virt-clone -o 【原虚拟机】 -n 【新虚拟机】 -f 【新虚拟机镜像名（含路径）】`: 复制虚拟机
15. `qemu-img info vm.img`: 查看虚拟机镜像
16. `virsh snapshot-create vm`: 创建快照(快照包含内存信息)
17. `virsh snapshot-list vm`: 查询虚拟机快照
18. `virsh snapshot-create-as 【虚拟机名】 【快照名】`: 根据快照创建虚拟机
19. `virsh snapshot-current vm`: 查询虚拟机当前快照
20. `virsh snapshot-revert 【虚拟机名】 【快照名】`: 将虚拟机恢复至快照状态
21. `virsh snapshot-delet vm snapshot-vm`: 删除快照
22. `virsh pool-define-as vmdisk --type dir --target /data/vmfs`: 定义存储池与目录
23. `virsh pool-build vmdisk`: 创建已定义的存储池([kvm存储池和存储卷](https://www.cnblogs.com/mo-xiao-tong/p/12838733.html),[KVM存储池](https://blog.51cto.com/manual/2467163),[kvm-存储池基础介绍、创建](https://www.cnblogs.com/ygbh/p/17418666.html))
24. `virsh pool-start vmdisk`: 激活已经定义的存储池
25. `virsh pool-autostart vmdisk`: 启动已定义存储池
26. `virsh pool-list --all`: 查询所有存储池
27. `virsh vol-create-as vmdisk test.qcow2 3G --format qcow2`: 在存储池中创建虚拟机存储卷

## 1.4 实例创建和恢复快照

```bash
# 1. 创建快照
virsh snapshot-create-as linux_mini new-snapshot
# 2. 查询快照
virsh snapshot-list linux_mini
# 3. 使用快照恢复虚拟机
 virsh snapshot-revert   linux_mini new-snapshot
# 4. 查询虚拟机当前快照
virsh snapshot-current linux_mini
# 5. 删除快照--存在未删除快照，无法undefine实例
virsh snapshot-delete linux_mini linux_min-snap
```

注意：存在hostdev 设备的不支持闪照

```bash
error: Operation not supported: cannot migrate a domain with <hostdev mode='subsystem' type='pci'
```
针对这种情况需要弃盘迁移。不能支持快照和冷迁移。腾讯云也是这么玩的

## 1.5 实例热迁移

```bash
# 迁移到目标地址
virsh migrate <虚拟机名称> qemu+ssh://<目标主机IP地址>/system
# 迁移并指定参数
virsh migrate --live <虚拟机名称> qemu+ssh://<目标主机IP地址>/system
# 查询迁移进度
virsh domjobinfo <虚拟机名称>
# 取消迁移
virsh migrate-cancel <虚拟机名称>

```

___

- 参考：[实例热迁移](https://www.cnblogs.com/sammyliu/p/4572287.html);[KVM实例热迁移](https://blog.csdn.net/Tony_stark_L/article/details/132668909)

## 1.6 实例重装镜像

```bash
# 0. 关闭对应实例
virsh shutdown ins-xxx
# 1. 删除实例原有系统盘镜像
mv /data/instance/ins-xxx/ldisk-xxx.qcow2 /data/instance/ins-xxx/ldisk-xxx-bak.qcow2 
# 2. 创建新的实例系统盘
wget -c xxxx.qcow2 /data/instance/ins-xxx/ldisk-xxx.qcow2
# 3. 设置网卡基本信息
sed -i '/^IPADDR=/c'IPADDR=$instanceIP ifcfg-eth0
sed -i '/^GATEWAY=/c'GATEWAY=$gateway ifcfg-eth0
sed -i '/^NETMASK=/c'NETMASK=$netmask ifcfg-eth0
# 4. 将网卡配置文件拷贝到目标镜像中
## linux
virt-copy-in -a /data/instance/ins-xxx/ldisk-xxx.qcow2 ifcfg-eth0 /etc/sysconfig/network-scripts/
## windows
yum install libguestfs-winsupport.x86_64 dos2unix -y
unix2dos ifcfg-eth0
virt-copy-in -a "$imgPath/$diskID".qcow2 ifcfg-eth0 /"Windows"/

# 5. 重新拉起实例 
virsh define 
```
注意：虚拟机vnc登录，使用端口号加`:`的形式进行登录
使用如下命令查询，实例对应的vnc端口
```bash
virsh vncdisplay ins-xxxx
# :0 表示5900
# :1 表示5901
```

## 1.6 [加载usb设备](https://www.cnblogs.com/dewan/p/16788525.html)

```bash
# 1. 查询主机对应usb
sudo yum -y install usbutils pciutils
lsusb 
# Bus 001 Device 004: ID 287f:00f8
# 2. 生成usb.xml
sudo yum install -y virt-install
virt-xml --build-xml --hostdev 287f:00f8,type=usb   > add-usb.xml
#<hostdev mode="subsystem" type="usb" managed="yes">
#  <source>
#    <vendor id="0x287f"/>
#    <product id="0x00f8"/>
#  </source>
# </hostdev>


# 3. 进行挂载
virsh attach-device amzn2 add-usb.xml --live
#OPTIONS
#    [--domain] <string>  domain name, id or uuid
#    [--file] <string>  XML file
#    --persistent     make live change persistent
#    --config         affect next boot
#    --live           affect running domain
#    --current        affect current domain

# 4. 进行移除
virsh detach-device amzn2 add-usb.xml

# 快速步骤
# 添加
virt-xml amzn2 --update --add-device  --hostdev 21c4:0cd1,type=usb
# 删除
virt-xml amzn2 --update --remove-device --hostdev 21c4:0cd1,type=usb
```



# 2. [腾讯云csig虚拟化部门一面面经](https://www.nowcoder.com/feed/main/detail/2e58841b38774b048cf98a0a6d670027?sourceSSR=search)

## 2.1 启动一个vm的流程，libvirt,qemu,kvm分别都做了哪些工作

在启动一个虚拟机的过程中，libvirt会通过调用QEMU和KVM的API来创建并启动虚拟机。具体的流程包括以下步骤：

1. libvirt加载虚拟机的定义和配置信息。
2. libvirt调用QEMU创建虚拟机的运行环境，并加载虚拟机的镜像文件。
3. QEMU启动虚拟机，并模拟虚拟机的硬件设备。
4. KVM管理虚拟机的内存和CPU资源，并将虚拟机的指令转换为物理主机的指令。

- 参考：[libvirt官方文档](https://libvirt.org/)


## 2.2 虚拟内存槽memslot

虚拟内存槽主要是指 kvm_memslot 是虚拟机用来组织物理内存结构的关键数据结构。
维护了GPA到HVA的映射关系

也是虚拟机内存管理的核心关键。其主要操作流程如下所示：

![memslot](https://pic2.zhimg.com/80/v2-9afeb71bd8159893b268a704b0b27c99_720w.webp)


- [一文分析Linux虚拟化KVM-Qemu分析之内存虚拟化](https://zhuanlan.zhihu.com/p/596776561)

## 2.3 热迁移了解多少，迁移流程


## 2.4 脏页迭代流程，脏页位图，脏环，pml


## 2.5 gva到hpa转换过程


## 2.6 ebpf简要介绍一下，介绍ebpf项目中的vm exit ，mmu page fault，halt polling


## 2.7 使用ebpf技术对kvm进行观测，会对虚拟机产生性能影响吗


## 2.8 KVM内存虚拟化是怎么是怎么实现的？
追问：EPT页表如何实现
追问：Guest通过EPT页表访问要经过多少次访存
追问：SPT和EPT的优缺点，什么场合适合使用SPT或者EPT

## 2.9 KVM中CPU虚拟化是如何实现的

## 2.10 问了一下KVM和OpenStack/XEN的区别

## 2.11 Docker的命名空间有哪些

## 2.12 Docker的四种网络类型


## 2.13 IOMMU 介绍一下

IOMMU 是一种硬件功能，于将输入/输出设备的内存访问隔离，以防止设备对系统内存的非法访问。这对于虚拟化环境特别有用，因为它允许虚拟机对硬件设备进行独占访问，并且不会影响到其他虚拟机。

如果您正在使用虚拟化软件，并且需要为虚拟机提供独占访问硬件设备，那么您应该开启 IOMMU 功能。但是，如果您不使用虚拟化软件，或者不需要独占访问硬件设备，那么您可以不开启 IOMMU 功能。

- 参考： [IOMMU](https://zhuanlan.zhihu.com/p/365408539);[IOMMU](https://blog.51cto.com/u_12891/7741285)


# 3. [字节二面面经](https://www.nowcoder.com/discuss/353159400808456192?sourceSSR=search)



## 3.1 qemu-kvm的虚拟化过程讲一讲？

## 3.2 虚拟机上硬件设备如何模拟的？

## 3.3 vruntime和进程的优先级有什么关系？(查阅之后：虚拟运行时间 vruntime += 实际运行时间 delta_exec * NICE_0_LOAD/ 权重)

## 3.4 virtio中的VM Exit状态了解吗？什么时候会出现这个状态？

## 3.5 调度系统中的sched_entity知道吗？

## 3.6 操作系统中的虚拟地址转换物理地址的全过程？


## 3.7 了解内存虚拟化的影子页表，EPT技术吗？

## 4. [百度一面](https://www.nowcoder.com/discuss/353154553833005056?sourceSSR=search)

## 1 Linux kvm，GPU 直通，SRIOV

## 2. CPU架构，NUAM，SMP


## 3. Guest OS发个网络请求，到Host OS，再到硬件的过程


## 4. CPU ***结构，是否共享

# 5. [腾讯os内核虚拟化面经](https://www.nowcoder.com/discuss/538358005318930432?sourceSSR=search)

# 6. [KVM 常见面试题](https://blog.csdn.net/weixin_43313333/article/details/128904292)

## 1. 简单介绍一下KVM

KVM是Kernel-based Virtual Machine的简称，一个开源的系统虚拟化模块，使用Linux自身的调度器进行管理，KVM的虚拟化需要硬件支持（如Intel VT技术或者AMD V技术)。是基于硬件的完全虚拟化。

## 2. KVM的三个组件及作用

1. KVM：内核虚拟化模块，负责CPU虚拟化与内存虚拟化
2. qemu: 负责设备虚拟化，包含IO设备、usb设备，串口设备等
3. libvirt: 虚拟化综合管理工具，封装kvm/qemu接口，用于快速操作虚拟机

## 3. 磁盘镜像格式raw和qcow2的区别

虚拟机常见的磁盘格式有如下几种:
- qcow2(cow/qcow/qcow3): qemu 主要镜像格式。支持写复制、动态扩容、压缩、AES加密，但是读写性能相对raw较差。
- raw: 原始数据格式，可以直接由dd命令生成。较为原始。创建时占用全部容量，不支持动态扩容，不支持快照，性能好
- vmdk: VMware的格式，使用较少
- 格式转换：可以使用如下命令进行格式转换

```bash
# qcow2 -> raw
qemu-img convert -f qcow2 -O raw /var/lib/libvirt/images/centos7.0.qcow2 /var/lib/libvirt/images/centos7.0.raw

# 使用StarWind V2V Image Converter 工具进行镜像格式转换

```

___

- 参考：[虚拟磁盘镜像的存储格式](https://access.redhat.com/documentation/zh-cn/red_hat_virtualization/4.0/html/technical_reference/qcow2);[虚拟机镜像格式对比](https://www.cnblogs.com/diantong/p/11490152.html);[各种虚拟化镜像格式](https://blog.csdn.net/qq_33932782/article/details/54943854)


## 4. 虚拟机文件中配置文件和硬盘文件分别在哪些路径下

- 虚拟机配置文件，XML文件，位置 ：/etc/libvirt/qemu/
- 虚拟机硬盘文件，位置：/var/lib/libvirt/images

## 5. kvm虚拟机的网络配置有哪两种模式？默认使用哪一种？

- NAT模式：也是用户模式，数据包由NAT方式通过主机的接口进行传送，可以访问公网，但是无法从外部访问虚拟机网络，kvm默认用的这种网络。
- Bridge：也就是桥接模式，这种模式允许虚拟机像一个独立的主机一样拥有网络，外部的机器可以直接访问到虚拟机内部，但需要网卡支持，一般有线网卡都支持。

## 6. KVM三种工作模式 

1. 客户模式：执行非I/O的客户代码，虚拟机运行在这个模式下
2. 用户模式：用户执行I/O代码，QEMU运行在这个模式下
3. 内核模式：CPU调度和内存管理相关，KVM内核模块运行在该模式下

## 7. 什么是虚拟化技术

虚拟化技术是一种资源管理技术，是将计算机的各种实体资源（CPU、内存、磁盘空间、网络适配器等），予以抽象、转换后呈现出来并可供分割、组合为一个或多个电脑配置环境。

## 8. kvm支持哪些虚拟磁盘格式?

kvm从qemu继承了丰富的磁盘格式, 包括裸映象(raw images), 原始qemu格式(qcow), VMware格式和更多

## 9. Libvirt 包含 哪3 个组件？

- libvirtd: 宿主机daemon程序，用于接收和处理 API 请求；
- virsh： 常用KVM命令管理工具
- api库：常用的libvirt开发高级工具，如  virt-manager


## 10. 说一下KVM虚拟化架构

KVM 主要虚拟化架构如下：

![kvm虚拟化架构](https://img-blog.csdnimg.cn/2f5e91fdbeb74ae7bf8a441a0c0ded14.png)



## 11. [virsh shutdown与destory的区别](https://blog.csdn.net/wcs_sdu/article/details/99674181)



## 12. [添加USB设备](https://access.redhat.com/documentation/zh-cn/red_hat_enterprise_linux/6/html/virtualization_administration_guide/sect-managing_guest_virtual_machines_with_virsh-attaching_and_updating_a_device_with_virsh)




## 13. 腾讯云设备对比
- [高IO型IT5](https://cloud.tencent.com/document/product/213/11518#IT5)

```
2.5GHz Intel® Xeon® Cascade Lake 处理器，计算性能稳定。
搭配最新一代六通道 DDR4 内存。
最高可支持23Gbps内网带宽，满足极高的内网传输需求。
采用 NVMe SSD 的实例存储，提供低延迟、超高的 IOPS。
单盘随机读性能高达65万 IOPS（4KB块大小），顺序读吞吐能力高达2.8GB/s（128KB块大小）。
整机随机读性能高达205万 IOPS（4KB块大小），顺序读吞吐能力均高达11GB/s（128KB块大小）。
支持关闭或开启超线程配置。
```

内部高IO

```

```

一个机架4个机位100台