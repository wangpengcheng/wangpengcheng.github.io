---
layout:     post
title:      makrdown新语法学习
subtitle:   测试说明
date:       2021-08-15
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - Markdown
    - PlantUML
    - 
---

# jekyll-spcaeship功能测试

__参考链接__:
- [mermaid文档](https://mermaid-js.github.io/mermaid/#/n00b-gettingStarted)
- [mermaid在线编辑器]([e/jekyll-spaceship/edit/master/README.md](https://mermaid-js.github.io/mermaid-live-editor/edit/#eyJjb2RlIjoic2VxdWVuY2VEaWFncmFtXG4gICAgQWxpY2UtPj4rSm9objogSGVsbG8gSm9obiwgaG93IGFyZSB5b3U_XG4gICAgQWxpY2UtPj4rSm9objogSm9obiwgY2FuIHlvdSBoZWFyIG1lP1xuICAgIEpvaG4tLT4-LUFsaWNlOiBIaSBBbGljZSwgSSBjYW4gaGVhciB5b3UhXG4gICAgSm9obi0tPj4tQWxpY2U6IEkgZmVlbCBncmVhdCFcbiAgICAgICAgICAgICIsIm1lcm1haWQiOiJ7XG4gIFwidGhlbWVcIjogXCJkZWZhdWx0XCJcbn0iLCJ1cGRhdGVFZGl0b3IiOmZhbHNlLCJhdXRvU3luYyI6dHJ1ZSwidXBkYXRlRGlhZ3JhbSI6ZmFsc2V9))
- [plantuml学习笔记](https://wangpengcheng.github.io/2020/06/26/uml_learn_note/)
- [Emoji表情列表](https://www.webfx.com/tools/emoji-cheat-sheet/)
- [全表情列表](https://www.unicode.org/emoji/charts/full-emoji-list.html)
- [在 markdown 中使用表情符号](https://www.jianshu.com/p/31f075f8e118)
- [emoji中文查询](https://www.emojiall.com/zh-hans)


> 最新博客完成了升级，为了熟悉新对功能，进行新的语法知识的学习

## 1. MathJax语法
- [MathJax Documentation](http://docs.mathjax.org/en/latest/)
- [MathJax公式教程](https://blog.csdn.net/dabokele/article/details/79577072)
- [LaTeX 公式篇](https://zhuanlan.zhihu.com/p/110756681)
- [LATEX公式总结](https://www.jianshu.com/p/22117d964baf)

尝试使用公式：
$$\Gamma(z) = \int_0^\infty t^{z-1}e^{-t}dt\,$$


$$
\left \{ 
\begin{array}{c}
a_1x+b_1y+c_1z=d_1 \\
a_2x+b_2y+c_2z=d_2 \\
a_3x+b_3y+c_3z=d_3
\end{array}
\right
\tag{1.1}
.$$

$$
f(n)
\begin{cases}
\cfrac n2, &if\ n\ is\ even\\[5ex]
3n + 1, &if\  n\ is\ odd
\end{cases}
$$

## 2. PlantUML语法

## 3. Mermaid语法

## 4. Media语法


```
Inline-style:
![]({media-link})

OR

![][{reference}]

[{reference}]: {media-link}
```

对于常用视频网站可以直接进行添加

正常使用如下：

<iframe src="//player.bilibili.com/player.html?aid=462256092&bvid=BV1mL411E7Uk&cid=388230485&page=1" scrolling="no" border="0" frameborder="no" framespacing="0" allowfullscreen="true" style="width: 100%;height:480px"> </iframe>

