---
layout:     post
title:      ali中间件学习笔记
subtitle:   阿里中间件学习笔记
date:       2020-08-25
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - Java
    - 实习笔记
    - 中间件
---

# 阿里中间件学习笔记

- [tengine](https://github.com/alibaba/tengine)
- [arthas](https://github.com/alibaba/arthas)
- [diamond](https://github.com/takeseem/diamond)
- [Sentinel](https://github.com/alibaba/Sentinel)

```java
@Test
    public void testbreakpointStorePlay(){
        SopStoreBreakpointRequest request = new SopStoreBreakpointRequest();
        request.setSessionId("sid");
        request.setResumeParamMaps(Maps.newHashMap());
        Boolean res = Boolean.TRUE;
        doReturn(res).when(sopEngineManager).breakpointStorePlay(anyObject());
        Result<Void> result = sopRuntimeService.breakpointStorePlay(request,null);
        Assert.assertTrue(result.getSuccess());
        res = Boolean.FALSE;
        Result<Void> failRes = sopRuntimeService.breakpointStorePlay(request,null);
        Assert.assertFalse(failRes.getSuccess());

    }

```