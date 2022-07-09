---
layout:     post
title:      前端学习笔记--Vue
subtitle:   前端学习笔记--Vue(一)
date:       2020-10-20
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - 前端
    - Vue
    - CSS
    - MVVM
---
> 2020-07-17 17:58:49 
# Vue学习笔记

_参考链接：_
- [vue官方网站](https://v3.cn.vuejs.org/)
- [ECMAScript 6 简介](https://es6.ruanyifeng.com/#docs/intro)
- [vue企业开发实战](https://www.zhihu.com/pub/reader/119630650/chapter/1179818801281019904)

## 0. 预备知识：JSES6

- ES6 新增了let命令，用来声明变量。它的用法类似于var，但是所声明的变量，只在let命令所在的代码块内有效。

```javascript
{
  let a = 10;
  var b = 1;
}

a // ReferenceError: a is not defined.
b // 1
```
- let只能在指定作用领中使用。注意在作用域中不能进行重复定义。
- `const` 只能声明常量，声明之后不能修改。也是在块级作用领域内作用
- `const`使用的指认，更多的是基于指针的对象，相当于C++中的顶部指针`*const a` 不能更改其指向的对象，但是可以改变具体对象的值
- `ES6`中允许对象的解构赋值与数组的解构赋值：
```javascript
var jsonData= {
  id: 123,
  name: "json_data",
  data: [123,456]
}
```

## 1. `Vue`相关指令
### 1. 条件渲染指令：
- `v-if`指令：vue-> view :值进行判断与相关操作。可以实现复杂功能，但是消耗较大。
- `v-else`指令：vue->view:值进行判断与相关操作。
- `v-show`指令：vue->view:当表达值为false时，元素会隐藏，使用的是HTML标签中的`display:none`相关方法。直接对属性进行控制，性能损耗较小。
- `v-for`指令：使用方法:`v-for = '(item,index) in items'`。遍历对象使用示例：
```html
<li v-for="(value,key,index) in person" >
{{index}}-{{key}}-{{value}}
</li>

<script>
export default{
  data() {
    return {
      person:{
        name: '小暖'，
        age: 20
      },
    };
  }
}
</script>
```
- `v-for` 数组遍历指令：
```html
<li v-for="(item,index) in lession">
  {{index}}-{{item.name}}-{{item.value}}
<li>
```
- `v-on`指令为HTML元素绑定监听事件，类似原生Javascript中的`onclick`在HTML元素上进行监听。可以直接进行方法的绑定。同时使用vue进行方法的相关设定。
  - 注意：事件在vue中需要统一使用`method`进行定义和管理。
  - 它后面还可以增加修饰符：
    - `@check.stop`:调用`event.stopPropagation()`:阻止事件冒泡到父元素，阻止任何父事件处理任务被执行。
    - `@check.prevent`:调用 `event.preventDefault()`;该方法将通知 Web 浏览器不要执行与事件关联的默认动作（如果存在这样的动作）
    - `@check.self`：当事件是从侦听器绑定的元素本身触发时才触发回调
    - `@check.{keycode}`: 只在指定键上触发回调。
- `v-model`:数据双向绑定，可以同时进行修改与显示。
  - 使用方法:`v-model=变量`
  - 方法只能使用在`input`、`select`、`textarea`等相关作用域上。
  - 修饰符：
    - `.lazy`:数据不是同步更新，而是失去焦点更新或者回车更新。
    - `.trim`:自动过滤输入的首尾空格。

## 2. vue组件

HTML相关元素与动作的统一封装管理与进一步抽象就是vue组件的原型。通过组件可以快速实现代码的复用。

### 2.1 props进行数据传递
_参考连接：_
- [props传递属性](https://zhuanlan.zhihu.com/p/38092554)
- [vue官方文档](https://cn.vuejs.org/v2/guide/components.html)

组件中的消息传递，一般使用`props`进行父组件与子组件之间的消息传递。使用示例如下：

```html
<script>
// 创建组件 
Vue.component('blog-post', {
  // 定义组件属性
  props: ['title'],
  // 定义组件模板
  template: '<h3>{{ title }}</h3>'
})
</script>

<!--使用组件-->
<blog-post title="test1"></blog-post>
<blog-post title="test2"></blog-post>
<blog-post title="test3"></blog-post>
```

### 2.2 组件通信
子组件向父组件进行通信，使用$.emit("change",this.val)进行香瓜消息的抛出；本质是通过事件监听的方式，将组件事件进行抛出；然后再由父组件进行监听；详见：[使用事件抛出一个值](https://cn.vuejs.org/v2/guide/components.html#%E4%BD%BF%E7%94%A8%E4%BA%8B%E4%BB%B6%E6%8A%9B%E5%87%BA%E4%B8%80%E4%B8%AA%E5%80%BC)

使用代码示例如下：

```html
<script>
// 定义组件
<div id="blog-posts-events-demo">
  <div :style="{ fontSize: postFontSize + 'em' }">
    <blog-post
      v-for="post in posts"
      v-bind:key="post.id"
      v-bind:post="post"
    ></blog-post>
  </div>
</div>



<blog-post
  ...
  // 注意这里的事件接收
  v-on:enlarge-text="postFontSize += 0.1"
></blog-post>

<button v-on:click="$emit('enlarge-text')">
  Enlarge text
</button>
</script>
```
进一步的相关操作：
```html
<!--抛出事件值-->
<button v-on:click="$emit('enlarge-text', 0.1)">
  Enlarge text
</button>
<!--监听事件值-->
<blog-post
  ...
  v-on:enlarge-text="postFontSize += $event"
></blog-post>

<!--事件监听-->
<blog-post
  ...
  v-on:enlarge-text="onEnlargeText"
></blog-post>
<script>
// 设置处理数据
methods: {
  onEnlargeText: function (enlargeAmount) {
    this.postFontSize += enlargeAmount
  }
}
</script>
```


兄弟节点之间的通信，主要使用子->父->子的相关方式进行更新。之间的主要通信方式。关键通信方式，类似于Qt的信号于槽操作。

### 2.3 slot内容分发
_参考连接:_
- [插槽组件](https://v3.cn.vuejs.org/guide/component-slots.html)
