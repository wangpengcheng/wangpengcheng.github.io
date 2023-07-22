---
layout:     post
title:      jekyll-spcaeship相关功能测试
subtitle:   测试说明
date:       2021-08-14
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - Markdown
    - jekyll-spaceship
---

# jekyll-spcaeship功能测试

__参考链接__:
- [jekyll-spaceship插件说明](https://github.com/jeffreytse/jekyll-spaceship)
- [GitHub Action自动编译](https://github.com/jeffreytse/jekyll-deploy-action)
- [jekyll官方网站](https://jekyllrb.com/)
- [jekyll mac安装说明](https://jekyllrb.com/docs/installation/macos/)
- [标准状况地址](https://github.com/jeffreytse/jekyll-spaceship/edit/master/README.md)


### Table测试

|              Stage | Direct Products | ATP Yields |
| -----------------: | --------------: | ---------: |
|         Glycolysis |          2 ATP              ||
| ^^                 |          2 NADH |   3--5 ATP |
| Pyruvaye oxidation |          2 NADH |      5 ATP |
|  Citric acid cycle |          2 ATP              ||
| ^^                 |          6 NADH |     15 ATP |
| ^^                 |          2 FADH |      3 ATP |
|                               30--32 ATP        |||

#### Multiline

| :    Easy Multiline   : |||
| :----- | :----- | :------ |
| Apple  | Banana | Orange  \
| Apple  | Banana | Orange  \
| Apple  | Banana | Orange
| Apple  | Banana | Orange  \
| Apple  | Banana | Orange  |
| Apple  | Banana | Orange  |

#### Headerless
|--|--|--|--|--|--|--|--|
|♜| |♝|♛|♚|♝|♞|♜|
| |♟|♟|♟| |♟|♟|♟|
|♟| |♞| | | | | |
| |♗| | |♟| | | |
| | | | |♙| | | |
| | | | | |♘| | |
|♙|♙|♙|♙| |♙|♙|♙|
|♖|♘|♗|♕|♔| | |♖|

#### Cell Alignment



| :        Fruits \|\| Food       : |||
| :--------- | :-------- | :--------  |
| Apple      | : Apple : | Apple      \
| Banana     |   Banana  | Banana     \
| Orange     |   Orange  | Orange     |
| :   Rowspan is 4    : || How's it?  |
|^^    A. Peach         ||   1. Fine :|
|^^    B. Orange        ||^^ 2. Bad   |
|^^    C. Banana        ||  It's OK!  |

#### Cell Markdown

Sometimes we may need some abundant content (e.g., mathjax, image, video) in Markdown table
Therefore, here we also make markown syntax possible inside a cell.

| :                   MathJax \|\| Image                 : |||
| :------------ | :-------- | :----------------------------- |
| Apple         | : Apple : | Apple                          \
| Banana        | Banana    | Banana                         \
| Orange        | Orange    | Orange                         |
| :     Rowspan is 4     : || :        How's it?           : |
| ^^     A. Peach          ||    1. ![example][cell-image]   |
| ^^     B. Orange         || ^^ 2. $I = \int \rho R^{2} dV$ |
| ^^     C. Banana         || **It's OK!**                   |

[cell-image]: https://jekyllrb.com/img/octojekyll.png "An exemplary image"

#### Cell Inline Attributes

{:ref-name: #id .cls1 .cls2}
{:second: ref-name #id-of-other title="hallo you"}
{:other: ref-name second}


{: #id .cls1 .cls2} <!-- #id <=> id="id", .cls1 .cls2 <=> class="cls1 cls2" -->
{: ref-name title="hallo you"}
{: ref-name class='.cls3' .cls4}

#### 表格测试

{:color-style: style="background: black;"}
{:color-style: style="color: white;"}
{:text-style: style="font-weight: 800; text-decoration: underline;"}

|:             Here's an Inline Attribute Lists example                :||||
| ------- | ------------------ | -------------------- | ------------------ |
|:       :|:  <div style="color: red;"> &lt; Normal HTML Block > </div> :|||
| ^^      |   Red    {: .cls style="background: orange" }                |||
| ^^ IALs |   Green  {: #id style="background: green; color: white" }    |||
| ^^      |   Blue   {: style="background: blue; color: white" }         |||
| ^^      |   Black  {: color-style text-style }                         |||


### MathJax 测试


$ a * b = c ^ b $

$ 2^{\frac{n-1}{3}} $

$ \int\_a^b f(x)\,dx. $

### PlantUML测试

```plantuml!
Bob -> Alice : hello world
```
OR

@startuml
Bob -> Alice : hello
@enduml

### Mermaid Usage

```mermaid!
pie title Pets adopted by volunteers
  "Dogs" : 386
  "Cats" : 85
  "Rats" : 35
```

@startmermaid
pie title Pets adopted by volunteers
  "Dogs" : 386
  "Cats" : 85
  "Rats" : 35
@endmermaid



```mermaid!
sequenceDiagram
    participant Alice
    participant Bob
    Alice->>John: Hello John, how are you?
    loop Healthcheck
        John->>John: Fight against hypochondria
    end
    Note right of John: Rational thoughts <br/>prevail!
    John-->>Alice: Great!
    John->>Bob: How about you?
    Bob-->>John: Jolly good!
```

### Media Usage

![测试](http://open.spotify.com/track/4Dg5moVCTqxAb7Wr8Dq2T5)

![](//www.html5rocks.com/en/tutorials/video/basics/devstories.webm)

![](//techslides.com/demos/sample-videos/small.ogv?allow=autoplay)

![](//techslides.com/demos/sample-videos/small.mp4?width=400)


![](https://www.youtube.com/watch?v=Ptk_1Dc2iPY?width=800&height=500)

![](https://www.dailymotion.com/video/x7tfyq3?width=100%&height=400&autoplay=1)


### 8. Emoji Usage

I give this plugin two :+1:!