---
layout:     post
title:      virtio学习笔记
subtitle:   virtio 基础学习笔记
date:       2024-07-14
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - 虚拟化
    - virtio
    - 学习笔记
---

_参考链接：_

- [virtio简介（一）—— 框架分析](https://www.cnblogs.com/edver/p/14684104.html)
- [VIRTIO官网](http://docs.oasis-open.org/virtio/virtio/v1.0/virtio-v1.0.html)
- [vdpa优化](https://vdpa-dev.gitlab.io/)
- [linux-virtio](https://docs.kernel.org/driver-api/virtio/virtio.html)
- [libvirt-virtio](https://wiki.libvirt.org/Virtio.html)
- [详解：VirtIO Networking 虚拟网络设备实现架构](https://www.sdnlab.com/26199.html)
- [Virtio-Net 技术分析](https://www.openeuler.org/zh/blog/xinleguo/2020-11-23-Virtio_Net_Technology.html)
- [Virtio Networking: virtio-net 和 vhost-net](https://nxw.name/2022/virtio-networking-virtio-net-vhost-net)
- [vdpa设备驱动代码分析](https://cloud.tencent.com/developer/article/2436757)
- [linux设备驱动(4)bus详解](https://www.cnblogs.com/xinghuo123/p/12872026.html)
- [Virtio-Balloon超详细分析](https://ssdxiao.github.io/linux/2017/03/20/Virtio-Balloon.html)
- [有关virtio_blk的介绍及FPGA的实现一](https://www.ctyun.cn/developer/article/551950058917957)
- [devconf 19′: virtio 硬件加速](http://tech.mytrix.me/2019/05/devconf-19-virtio-%E7%A1%AC%E4%BB%B6%E5%8A%A0%E9%80%9F/)
- [【中国Linux内核大会：arch专场】05-Virtio-GPU视频硬件加速](https://www.bilibili.com/video/BV1Te411k7JN/)
- [virtio-gpu](https://www.qemu.org/docs/master/system/devices/virtio-gpu.html)

# virtio 框架介绍

# virtio源码分析(以vhost-net为例)

- [vhost-net--动手使用vhost-net](https://blog.csdn.net/Phoenix_zxk/article/details/132819619)
- [vhost-user--亲身体验 vhost-user：热烈欢迎 DPDK](https://blog.csdn.net/Phoenix_zxk/article/details/132839437)
- [OVS+DPDK的VIRTIO后端进行网络端口管理，Libvirt+Qemu+Kvm进行虚拟机管理](https://movetobe.github.io/computer_system/computer_networking/cloud_networking/virtio/virtio_env.html)


## libvirt创建vhost-net网卡

## qemu vhost网卡创建

## gust 驱动源码分析

## host 驱动源码分析


# virtio 驱动实战

## virtio net 网络驱动


在 QEMU 中创建一个自定义的 virtio-net 设备涉及到对 VirtIO 规范的实现，以及与 QEMU 的设备模型适配。以下是实现自定义 virtio-net 设备的基本步骤。

- 1. 环境准备
确保你拥有 QEMU 的开发环境，并获取 QEMU 源代码。
```bash
git clone https://git.qemu.org/git/qemu.git
cd qemu
```  

- 2. 创建自定义设备文件

在 hw/virtio/ 目录下创建一个新的文件，假设命名为 virtio_net_custom.c。可以参考 virtio-net.c 的实现。

```cpp
# include "qemu/osdep.h"  
# include "hw/virtio/virtio.h"  
# include "net/net.h"  
# include "hw/virtio/virtio-net.h"  

typedef struct VirtioNetCustom {  
    VirtIOContainer virtio;  
    NetClientState *nc; // 网络客户端  
} VirtioNetCustom;  

// 设备的重置逻辑  
static void virtio_net_custom_reset(VirtioNetCustom *dev) {  
    // 适当重置设备状态  
}  

// 实现 VirtIO 设备的 Realize 方法  
static void virtio_net_custom_realize(VirtIODevice *vdev, Error **errp) {  
    VirtioNetCustom*dev = VIRTIO_NET_CUSTOM(vdev);  

    // ... 初始化网络接口，设置相关参数等 ...  
    
    dev->nc = net_client_init("custom-net", ...); // 初始化网络客户端  
}  

// 在 virtio_net_custom_realize 函数中  
// 这里设置转发给本机网卡
static void virtio_net_custom_realize(VirtIODevice *vdev, Error **errp) {  
    VirtioNetCustom *dev = VIRTIO_NET_CUSTOM(vdev);  
    
    // 创建虚拟网络客户端，指向物理网卡  
    dev->nc = net_client_init("tap", /* 你的 tap 设备名称或其他参数 */);  
    
    // 如果需要，设置网络参数，例如 IP 地址等  
    // net_client_set_ip(dev->nc, "192.168.0.2");  
    
    // 设置回调以处理进出的网络流量  
    net_client_set_receive_func(dev->nc, receive_packet, dev);  
    net_client_set_send_func(dev->nc, send_packet, dev);  
}  

// 处理接收到的网络包  
static void receive_packet(NetClientState *nc, const uint8_t *buf, size_t len) {  
    VirtioNetCustom *dev = container_of(nc, VirtioNetCustom, nc);  
    
    // 将接收到的数据包发送到 VirtIO 设备  
    virtio_net_send(dev->virtio, buf, len);  
}  

// 发送网络包的实现  
static void send_packet(NetClientState *nc, const uint8_t *buf, size_t len) {  
    // 直接从你的 VirtIO 设备转发包到物理网卡或将其发送出去  
    // 实际发送包的逻辑取决于如何配置与物理网卡的连接  
}  

// 处理 VirtIO 请求  
static void virtio_net_custom_handle(VirtIODevice *vdev) {  
    VirtioNetCustom*dev = VIRTIO_NET_CUSTOM(vdev);  
    // 处理网络请求  
}  

// 定义 VirtIO 设备的属性  
static Property virtio_net_custom_properties[] = {  
    // 添加设备属性  
    { /*sentinel*/ }  
};  

// 自动化注册设备  
static const VirtIODeviceInfo virtio_net_custom_info = {  
    .base = {  
        .init = virtio_net_custom_realize,  
        .reset = virtio_net_custom_reset,  
    },  
    .props = virtio_net_custom_properties  
};  

// 设备类别初始化  
static void virtio_net_custom_class_init(ObjectClass *oc, void*data) {  
    VirtIODeviceClass *k = VIRTIO_DEVICE_CLASS(oc);  
    k->handle = virtio_net_custom_handle;  
}  

// 注册设备类型  
static TypeInfo virtio_net_custom_type = {  
    .name = "virtio-net-custom",  
    .parent = TYPE_VIRTIO_DEVICE,  
    .instance_size = sizeof(VirtioNetCustom),  
    .class_init = virtio_net_custom_class_init,  
};  

// 设备初始化  
static void virtio_net_custom_register(void) {  
    type_register_static(&virtio_net_custom_type);  
}  

type_init(virtio_net_custom_register);  
```
- 3. 更新构建系统
确保你的新设备文件被包含在编译中：
在 hw/virtio/Makefile.objs 中添加：

obj-y += virtio_net_custom.o  
- 4. 编译 QEMU
在 QEMU 根目录下编译：
```
./configure  
make
```  

- 5. 添加你的为你的自定义设备添加后端TAP设备

```bash
sudo ip tuntap add dev tap0 mode tap  
sudo ip link set dev tap0 up  
sudo dhclient tap0  # 获取 IP 地址（可选） 
```

- 6. 运行 QEMU 使用自定义设备
使用以上命令启动你的自定义设备：
```bash
qemu-system-x86_64 -device virtio-net-custom,netdev=net0 -netdev tap,id=net0,packets=on  
```
- 7. 调试和测试
使用调试工具（如 GDB）检查你的设备逻辑，确保功能符合预期。你可以在实现代码中添加日志来帮助诊断问题。

小结
上面的步骤提供了创建一个自定义 virtio-net 设备的基本框架。具体实现细节会依据你的需求而有所不同，可以参考 QEMU 现有的 virtio-net 设备实现来获取更多信息和示例。此外，确保了解 VirtIO 规范，以便正确处理网络协议和设备虚拟化的相关内容。
