---
layout:     post
title:      shell练习笔记
subtitle:   leetcode shell练习笔记
date:       2023-11-07
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - shell
    - LeetCode
    - 编程练习
---

# shell练习笔记
> 2023-11-07 14:35:58

shell 练习笔记，包含leetcode等shell 题目冲刺笔记

## 参考链接
- [leetcode-shell](https://leetcode.cn/problemset/shell/)
- [shell脚本快速入门之-----shell脚本练习100例](https://cloud.tencent.com/developer/article/1691052)
- [now-code 在线编程shell篇](https://www.nowcoder.com/exam/oj?page=1&tab=SHELL%E7%AF%87&topicId=195)

## leetcode题目

### [192.统计词频率](https://leetcode.cn/problems/word-frequency/description/)

- 提交解答

```bash
cat words.txt | xargs -n1 | sort | uniq -c | sort -nr | awk '{print $2, $1}'
```

- 优质解答

```bash
awk '{for (i = 1; i <= NF; i++) {print $i}}' words.txt | sort | uniq -c | sort -nr | awk '{print $2, $1}'
```

- 优质解答2

```bashe
cat words.txt | tr -s ' ' '\n' | sort | uniq -c | sort -nr | awk '{print $2, $1}'
```

- 原始解答

```bash
cat words.txt | awk '{ for(i=1;i<=NF;i++){count[$i]++} } END { for(k in count){print k" "count[k]} }' | sort -rnk 2
```

### [193.有效电话号码](https://leetcode.cn/problems/valid-phone-numbers/description/)
- [grep正则表达式](https://www.myfreax.com/regular-expressions-in-grep/)

- 通过解答

```bash
grep -P '^([0-9]{3}-|\([0-9]{3}\) )[0-9]{3}-[0-9]{4}$' file.txt
```

- 优质解答

```bash
# Read from the file file.txt and output all valid phone numbers to stdout.
sed -rn "/^((\([0-9]{3}\) )|[0-9]{3}-)[0-9]{3}-[0-9]{4}$/p" file.txt
```

- 官方题解

```bash
grep -P '^([0-9]{3}-|\([0-9]{3}\) )[0-9]{3}-[0-9]{4}$' file.txt

awk '/^([0-9]{3}-|\([0-9]{3}\) )[0-9]{3}-[0-9]{4}$/' file.txt

gawk '/^([0-9]{3}-|\([0-9]{3}\) )[0-9]{3}-[0-9]{4}$/' file.txt
```