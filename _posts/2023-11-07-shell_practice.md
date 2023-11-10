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

### [194. 转置文件](https://leetcode.cn/problems/transpose-file/description/)

- 参考链接：
    - [awk多维数组](https://blog.csdn.net/xiyangyang052/article/details/45584311)
    - [xargs命令](https://www.runoob.com/linux/linux-comm-xargs.html)

- 提交解答：

```bash
#!/bin/bash
awk '{
    nf=NF;
    for(i=1; i<=NF; i++){ 
        array1[NR,i]=$i; 
    }
} 
END{ 
    for(i=1;i<=nf;i++){
        for (j=1;j<=NR;j++) {
            if (j<NR) {
                printf "%s ",array1[j,i];
            }else{
                printf "%s",array1[j,i];
            }
            
        }
        print '\n'
    } 
}' file.txt
```

- 优质解答1:

```bash
# Read from the file file.txt and print its transposed content to stdout.
# 统计列数
line=$(cat file.txt | head -n 1 | wc -w)

# 遍历所有列
for i in $(seq 1 $line)

do
  # 输出指定列 | 将列转换为行
   awk '{print $'''$i'''}' file.txt | xargs

done
```

- 优质解答2：

```bash
# 获取列数
COLUMNS=$(head -n 1 file.txt| wc -w)

for i in $(seq 1 $COLUMNS); do
    # 获取第i列，然后用paste合并
    cut -d ' ' -f"$i" file.txt | paste -s -d' ' -
done
```

- 官方题解：

```bash

# 获取列数
COLUMNS=$(head -n 1 file.txt| wc -w)

for i in $(seq 1 $COLUMNS); do
    # 获取第i列，然后用paste合并
    cut -d ' ' -f"$i" file.txt | paste -s -d' ' -
done


#作者：Aronic
#链接：https://leetcode.cn/problems/transpose-file/
#来源：力扣（LeetCode）
#著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```