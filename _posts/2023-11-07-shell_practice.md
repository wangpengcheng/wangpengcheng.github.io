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

### [195. 第十行](https://leetcode.cn/problems/tenth-line/description/)

- 提交解答：

```bash
# Read from the file file.txt and output the tenth line to stdout.
tail -n +10 file.txt | head -n 1;
```

- 优质解答：

```bash
# 直接使用sed 进行过滤
sed -n '10p' file.txt
```

- 官方题解：

```bash
# awk，当处理到第10行时，打印该行。NR表示awk处理的当前行号。
awk 'NR == 10' file.txt

# tail+head，使用tail命令输出文件的第10行及之后的所有行，然后将结果通过管道传递给head命令。head命令只保留第一行并将其作为输出。
tail -n +10 file.txt | head -1

# sed，-n选项取消sed默认的输出，'10p'指定只打印第10行。
sed -n '10p' file.txt
```


## nowcode 题目

### [SHELL1 统计文件的行数](https://www.nowcoder.com/practice/205ccba30b264ae697a78f425f276779?tpId=195&tqId=36211&rp=1&ru=/exam/oj&qru=/exam/oj&sourceUrl=%2Fexam%2Foj&difficulty=undefined&judgeStatus=undefined&tags=&title=)

- 提交解答1
```bash
#!/bin/bash
# 结合cat
cat nowcoder.txt | wc -l 

# 直接使用wc,但是会输出文件名，需要用awk 进行切分
wc -l nowcoder.txt | awk '{print $1}'


# 直接awk打印行号
awk 'END{print NR}' ./nowcoder.txt
```

- 提交解答2

```bash
#!/bin/bash

read -a arr
while [ ${#arr[@]} -eq 2 ]
    do
        sum=$((${arr[0]} + ${arr[1]}))
        echo $sum
        read -a arr
    done
exit 0

```

- 优质解答：
```bash
# 直接使用read 进行解析
line=0
while read p
do
    ((line++))
done < ./nowcoder.txt
echo $line
```

- 官方题解：

```bash

# 直接使用wc
wc -l ./nowcoder.txt | awk '{print $1}'

# awk 直接输出行号
awk 'END{print NR}' ./nowcoder.txt

# grep 统计
grep -c "" ./nowcoder.txt 

# grep 统计
grep -n "" ./nowcoder.txt  | awk -F ":" '{print $1 }' | tail -n 1

# sed 统计
sed -n '$=' ./nowcoder.txt
```


## shell 日常脚本收集

### 打印日志

```bash
#!/bin/bash

log_model="日志测试模块"

function main_log() {
    # 打印格式： [时间] [pid] [modename] [args]
    # 0. 获取原始字符串
    data_str="[`date +"%Y-%m-%d %H:%M:%S.%N"`] [$$] [$log_model] $*"
    # 1. 输出到控制台
    echo $data_str
    # 2. 输出到文件
    echo $data_str >> log.txt 
}

function main() {
    main_log "$@"
}

main "$@"
```
