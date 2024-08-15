---
layout:     post
title:      SR-IOV调研学习
subtitle:   SR-IOV调研学习笔记(一)
date:       2024-07-25
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - kubernetes
    - 技术调研
    - SR-IOV
---

> 2024-07-26 14:41:58

_参考链接：_

- [ConnectX-5网卡](https://www.nvidia.cn/networking/ethernet/connectx-5/)
- [SR-IOV是什么？性能能好到什么程度？](https://zhuanlan.zhihu.com/p/91197211)
- [SR-IOV指南](https://feisky.gitbooks.io/sdn/content/linux/sr-iov.html)
- [red-hat#配置SR-IOV网络](https://docs.redhat.com/zh_hans/documentation/red_hat_enterprise_linux_openstack_platform/7/html/networking_guide/sec-sr-iov#create_virtual_functions_on_the_compute_node)
- [SR-IOV设置](https://blog.csdn.net/leoufung/article/details/121046338)
- [sr-iov相关文档配置](https://docs.nvidia.com/networking/display/public/sol/qsg+for+high+availability+with+nvidia+enhanced+sr-iov+with+bonding+support+(vf-lag))
- [Mellanox CX6 vdpa 硬件卸载 ovs-kernel 方式](https://wangzheng422.github.io/docker_env/notes/2021/2021.10.cx6dx.vdpa.offload.html)
- [vdpa-deployment](https://github.com/redhat-nfvpe/vdpa-deployment/blob/master/README.md)
- [vdpa配置](https://www.redhat.com/en/blog/hands-vdpa-what-do-you-do-when-you-aint-got-hardware-part-1)
- [NVIDIA ConnectX-5 Ethernet Adapter Cards for OCP Spec 2.0 User Manual](https://docs.nvidia.com/networking/display/connectx5enocp2)
- [PERFORMANCE TUNING FOR MELLANOX ADAPTERS](https://enterprise-support.nvidia.com/s/article/performance-tuning-for-mellanox-adapters#jive_content_id_Getting_started)
- [nvidia MLNX_OFED 驱动文档](https://docs.nvidia.com/networking/display/mlnxofedv23103220lts/introduction)
- [nvidia SR-IOV](https://docs.nvidia.com/networking/display/mlnxofedv531001/single+root+io+virtualization+(sr-iov))
- [Upstream how to use SF kernel vdpa device](https://github.com/Mellanox/scalablefunctions/wiki/Upstream-how-to-use-SF-kernel-vdpa-device)
- [OVS Offload Using ASAP² Direct](https://docs.nvidia.com/networking/display/mlnxofedv531001/ovs+offload+using+asap%C2%B2+direct)
- [Configure SR-IOV Network Virtual Functions in Linux* KVM*](https://www.intel.com/content/www/us/en/developer/articles/technical/configure-sr-iov-network-virtual-functions-in-linux-kvm.html)
- [深入高可用架构原理与实践--内核网络](https://www.thebyte.com.cn/network/linux-kernel-networking.html)
- [linux 内核网络](https://docs.kernel.org/networking/switchdev.html)

# SR-IOV


# vDPA