---
layout:     post
title:      微服务API网关设计
subtitle:   分布式微服务接入网关设计
date:       2024-08-14
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - 分布式网关
    - 微服务
    - 接入层
---

> 2024-09-12 14:20:58

_参考链接_：

- [腾讯蓝鲸API网关](https://github.com/TencentBlueKing/blueking-apigateway)
- [微服务API网关Kong](https://github.com/Kong/kong)
- [南北向网关：apache API six ](https://github.com/apache/apisix)
- [spring-cloud-gateway](https://github.com/spring-cloud/spring-cloud-gateway)
- [开源API网关manba](https://github.com/fagongzi/manba)
- [云原生API网关higress](https://github.com/alibaba/higress)
- [k8s网关方案Envoy](https://github.com/envoyproxy/gateway)
- [lura API GATEWAY](https://github.com/luraproject/lura)
- [go-kratos-gateway](https://github.com/go-kratos/gateway)


## 背景

随着云原生微服务时代的到来，DDD驱动模式模式下，[微服务分层架构](https://xie.infoq.cn/article/9c9b39baed1c384d0aff60127)逐渐流行。其中接入层功能逐渐凸显，为了承接[东西向流量](https://www.ctyun.cn/developer/article/443274264797253)公共的API接入层需要独立为单独的微服务，完成流量转发、协议转换、审计、鉴权等功能。

![DDD分层架构](https://ucc.alicdn.com/pic/developer-ecology/6907f6f6de8046baba328c60a60c3ce5.png)

## 核心需求

- 流量分发：进行统一的接口代理与流量分发
- 协议转换：支持HTTP/rpc 协议版本相互转换
- 审计&鉴权：基础请求日志审计与认证鉴权功能
- 参数过滤&转换：参数的过滤与转换功能(非核心)

## 开源方案分析

### spring-cloud-gateway


### api six



## 方案梳理

## 存在问题


## 展望




