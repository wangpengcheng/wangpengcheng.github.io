---
layout:     post
title:      markdown 学习笔记
subtitle:   github markdown 详细学习笔记
date:       2019-4-10
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - github
    - markdown
    - linux
---


# github MarkDown 规则

*详细内容可以查看[markdown示例](https://github.com/wangpengcheng/wangpengcheng.github.io/blob/master/_posts/2019-4-10-markdown_example.md)*

---
## 横线
markdown 使用`***、---、___`可以显示横线效果
```
***
---
___

```
渲染效果如下：

***

---

___


## 基本标题
---
>  #、## 、### 分别代表一级标题、二级标题、三级标题等
>  例：# 一级标题 ## 二级标题 ### 三级标题

# 一级标题
## 二级标题
### 三级标题

## 文本
---
markdown语法中

加粗是'** **'或者'_ _'

斜体是 '* *'或者'_ _'

删除线 '~~ ~~'

引用文本关键字'>';

```
*This text will be italic*
_This will also be italic_

**This text will be bold**
__This will also be bold__

_You **can** combine them_

> 这是一段引用文本

```

分别表示内容如下：

This text will be italic*
_This will also be italic_

**This text will be bold**
__This will also be bold__

_You **can** combine them_
> 这是一段引用文本
这是一段`中间`引用文本


## list 列表
---
使用 " * "和" - "表示列表
>实际代码：

```
Sometimes you want numbered lists:

1. One
2. Two
3. Three

Sometimes you want bullet points:

* Start a line with a star
* Profit!

Alternatively,

- Dashes work just as well
- And if you have sub points, put two spaces before the dash or star:
  - Like this
  - And this
  
```

渲染效果：

Sometimes you want numbered lists:

1. One
2. Two
3. Three

Sometimes you want bullet points:

* Start a line with a star
* Profit!

Alternatively,

- Dashes work just as well
- And if you have sub points, put two spaces before the dash or star:
  - Like this
  - And this


## 引用代码
---
使用 " ``` "表示引用代码；
例如
```markdown
``` c++
int a = 1;
int b = 2;
int c = a+b;
```
```
显示状况如下：
``` c++
int a = 1;
int b = 2;
int c = a+b;
```
## 使用链接
---
> 可以利用`[]()`来使用链接，链接地址可以指向图片，文章锚点或者网页链接

```
[百度](https://www.baidu.com)
[图片](https://camo.githubusercontent.com/15675678891dead0d516b6ee7a57ed12101ce69a/687474703a2f2f7777772e62616964752e636f6d2f696d672f62646c6f676f2e676966)
```

效果：
[百度](https://www.baidu.com)
[图片](https://camo.githubusercontent.com/15675678891dead0d516b6ee7a57ed12101ce69a/687474703a2f2f7777772e62616964752e636f6d2f696d672f62646c6f676f2e676966)

## Extras
---
### 使用表格

markdown 使用如下方式来表示表格

```markdown
表头1  | 表头2|
--------- | --------|
表格单元  | 表格单元 |
表格单元  | 表格单元 |

| 表头1  | 表头2|
| ---------- | -----------|
| 表格单元   | 表格单元   |
| 表格单元   | 表格单元   |

### 对齐
表格可以指定对齐方式

| 左对齐 | 居中  | 右对齐 |
| :------------ |:---------------:| -----:|
| col 3 is      | some wordy text | $1600 |
| col 2 is      | centered        |   $12 |
| zebra stripes | are neat        |    $1 |

```
渲染效果：

------

表头1  | 表头2|
--------- | --------|
表格单元  | 表格单元 |
表格单元  | 表格单元 |

| 表头1  | 表头2|
| ---------- | -----------|
| 表格单元   | 表格单元   |
| 表格单元   | 表格单元   |

### 对齐
表格可以指定对齐方式

| 左对齐 | 居中  | 右对齐 |
| :------------ |:---------------:| -----:|
| col 3 is      | some wordy text | $1600 |
| col 2 is      | centered        |   $12 |
| zebra stripes | are neat        |    $1 |

---
*参考链接：*
 - [Mastering Markdown](https://guides.github.com/features/mastering-markdown/)
 - [Github上Markdown基本语法](https://www.cnblogs.com/yabin/p/6366151.html)
 - [GitHub上README写法暨GFM语法解读](https://blog.csdn.net/guodongxiaren/article/details/23690801)
