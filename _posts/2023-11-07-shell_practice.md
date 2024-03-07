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
### [SHELL2 打印文件的最后5行](https://www.nowcoder.com/practice/ff6f36d357d24ce5a0eb817a0ef85ee2?tpId=195&tqId=36212&rp=1&ru=/exam/oj&qru=/exam/oj&sourceUrl=%2Fexam%2Foj%3Fpage%3D1%26tab%3DSHELL%25E7%25AF%2587%26topicId%3D195&difficulty=undefined&judgeStatus=undefined&tags=&title=)

- 提交解答：

```bash
tail -n 5 nowcoder.txt
```

- 优质解答：

```bash
# 使用awk统计所有行数据，再进行打印
awk 'BEGIN {x=1} { arg[x]=$0;x++} END{ for (i=NR-4;i<=NR;i++) print arg[i]}'

# 使用head 负数进行打印
head -5 filename

# 使用sed命令进行添加
sed -n '5,20p' filename

```

### [SHELL3 输出 0 到 500 中 7 的倍数](https://www.nowcoder.com/practice/8b85768394304511b0eb887244e51872?tpId=195&tqId=36213&rp=1&ru=/exam/oj&qru=/exam/oj&sourceUrl=%2Fexam%2Foj%3Fpage%3D1%26tab%3DSHELL%25E7%25AF%2587%26topicId%3D195&difficulty=undefined&judgeStatus=undefined&tags=&title=)

- 官方题解：
    - [shell算术运算(加减乘除求余)](https://blog.csdn.net/qq_41419761/article/details/84033620)
    - [seq命令的用法](https://www.jianshu.com/p/7dff1a106ab5)

- 提交解答：

```bash
#!/bin/bash
for ((i=0;i<=500;i++));
do 
    res=$(expr $i % 7)
    if [[ $res == 0 ]];then
        echo $i;
    fi
done 
```

- 优质题解：

```bash
#!/bin/bash
# 直接使用间隔的方式进行输出即可
for num in {0..500..7}; do 
  echo "${num}"
done

# 使用sed 直接进行输出
seq 0 7 500
```

- 官方题解：

```bash
#!/bin/bash
for i in {0..500};
do
    if [[ i%7 -eq 0 ]];then
        echo $i
    fi
done
```

```bash
#!/bin/bash
for var in {0..500}
do
    if ((var%7==0))
    then
        echo $var
    else
        continue
    fi
done
```

### [SHELL4 输出第5行的内容](https://www.nowcoder.com/practice/1d5978c6136d4252904757b4fa0c9296?tpId=195&tqId=36214&rp=1&ru=/exam/oj&qru=/exam/oj&sourceUrl=%2Fexam%2Foj%3Fpage%3D1%26tab%3DSHELL%25E7%25AF%2587%26topicId%3D195&difficulty=undefined&judgeStatus=undefined&tags=&title=)

- 提交解答：

```bash
#!/bin/bash
awk 'NR == 5' nowcoder.txt
```

- 优质解答：

```bash
#!/bin/bash
head -n 5 nowcoder.txt | tail -n 1

# 使用sed  
sed -n 5p nowcoder.txt


# 使用read
line=1
while read value
do 
    if [ $line -eq 5 ]
    then echo $value
    fi
    ((line++))
done < nowcoder.txt
```

### [SHELL5 打印空行的行号](https://www.nowcoder.com/practice/030fc368e42e44b8b1f8985a8d6ad255?tpId=195&tqId=36215&rp=1&ru=/exam/oj&qru=/exam/oj&sourceUrl=%2Fexam%2Foj&difficulty=undefined&judgeStatus=undefined&tags=&title=)

- 提交解答

```bash
#!/bin/bash
# 直接使用awk 

awk '{
    if ( length($0) <= 0) {
        print NR
    }
}' nowcoder.txt
```

- 优质解答

```bash
#!/bin/bash
# https://www.gl.sh.cn/2018/08/15/shell_zhong_pan_duan_zi_fu_chuan_wei_kong_de_ji_zhong_fang_fa.html
line=1
while read value
do  
    # if [ ! "$value" ]; then
    # if [ -z "$value" ]; then
    if [ X$value = "X" ]; then
        echo $line $value
    fi
    ((line++))
done < nowcoder.txt
```

### [SHELL6 去掉空行](https://www.nowcoder.com/practice/0372acd5725d40669640fd25e9fb7b0f?tpId=195&tqId=36216&rp=1&ru=/exam/oj&qru=/exam/oj&sourceUrl=%2Fexam%2Foj%3Fpage%3D1%26tab%3DSHELL%25E7%25AF%2587%26topicId%3D195&difficulty=undefined&judgeStatus=undefined&tags=&title=)

- 提交解答：

```bash
#!/bin/bash

# 打印数据
awk '{ 
    if (length($0) > 0 ) {
        print $0
    }
}' nowcoder.txt


# 使用read

while  read  value
do
    if [[ X$value != "X" ]]; then
        echo $value
    fi
done < nowcoder.txt
```

### [SHELL7 打印字母数小于8的单词](https://www.nowcoder.com/practice/bd5b5d4b93a04226a81afbabf0be797d?tpId=195&tqId=36217&rp=1&ru=/exam/oj&qru=/exam/oj&sourceUrl=%2Fexam%2Foj%3Fpage%3D1%26tab%3DSHELL%25E7%25AF%2587%26topicId%3D195&difficulty=undefined&judgeStatus=undefined&tags=&title=)

- 提交解答：

```bash
#!/bin/bash
awk '{ 
    for (i=1;i<=NF;i++) { 
        if (length($i)<8) {
            printf "%s\n",$i
        }
    } 
}' nowcoder.txt
```

- 优质解答1：

```bash
### 获取字符串长度的方式 ele-字符串变量
# awk 的length()函数
# awk的NF变量 echo "${ele}" | awk -F"" '{print NF}'
# wc -c 字节个数,一个英文一个字节,因此可用,但是换行符也会被算上, 中文编码方式不同结果也不同
# wc -m 字符个数
# wc -L line的长度
# ${#ele} [ele是字符串变量名]
# expr length ${ele}
awk '{for(i=1;i<=NF;i++) if(length($i)<8) print $i}' nowcoder.txt
cat nowcoder.txt | awk '{for(i=1;i<=NF;i++) if(length($i)<8) print $i}'

### 已下都用方法/函数定义
function test0() {
    for ele in `cat nowcoder.txt`; do 
        if [ ${#ele} -lt 8 ]; then
            echo ${ele}
        fi
    done
}

# tem=`cat now/coder.txt`
# arr=(${tem})
function test1() {
    for ele in ${arr[@]}; do 
        if [ ${#ele} -lt 8 ]; then
            echo ${ele}
        fi
    done
}

function test11() {
    for ele in ${arr[@]}; do
        #temp=$(echo -n "${ele}" | wc -c)
        # -n参数:去除"\n"换行符,不去除的话,默认带换行符,字符个数就肉眼看到的多一个
        temp=`echo -n "${ele}" | wc -c`
        if [ ${temp} -lt 8 ]; then
            echo ${ele}
        fi
    done
}

function test111() {
    for ele in ${arr[@]}; do
        temp=`expr length ${ele}`
        if [ ${temp} -lt 8 ]; then
            echo ${ele}
        fi
    done
}

function test1111() {
    for ele in ${arr[@]}; do
        temp=`echo "${ele}" | wc -L`
        if [ ${temp} -lt 8 ]; then
            echo ${ele}
        fi
    done
}

function test2() {
    local ele=""
    for (( i=0; i<${#arr[@]}; i++ )); do
        ele=${arr[${i}]}
        if [ ${#ele} -lt 8 ]; then
            echo ${ele}
        fi
    done
}
test0

```

### [SHELL8 统计所有进程占用内存百分比的和](https://www.nowcoder.com/practice/fb24140bac154e5b99e44e0cee45dcaf?tpId=195&tqId=36218&rp=1&ru=/exam/oj&qru=/exam/oj&sourceUrl=%2Fexam%2Foj%3Fpage%3D1%26tab%3DSHELL%25E7%25AF%2587%26topicId%3D195&difficulty=undefined&judgeStatus=undefined&tags=&title=)

- 参考链接：
    - [题解 | #统计所有进程占用内存大小的和#](https://blog.nowcoder.net/n/82e5732e08ae4abe83bb07e75860dc9e)


- 提交解答：

```bash
#!/bin/bash
# 直接使用awk,注意awk相关语法
# 建议在awk中引用shell变量，使用格式：'"$var"'
awk  '
BEGIN{sum=0}
{
    if(NR>1){ 
        sum+=$4
    }
} 
END {print sum}
' nowcoder.txt 
```

- 优质解答：

```bash
#!/bin/bash
# 直接使用计算
sum=0
for i in `awk '{print $6}' nowcoder.txt`
do
    ((sum+=$i))
    done
echo $sum


#  性能最佳
sum=0;
while read p
do
    arr=($p)
    ((sum+=arr[5]))
done <nowcoder.txt
echo $sum
```


### [SHELL9 统计每个单词出现的个数](https://www.nowcoder.com/practice/ad921ccc0ba041ea93e9fb40bb0f2786?tpId=195&tqId=36219&rp=1&ru=/exam/oj&qru=/exam/oj&sourceUrl=%2Fexam%2Foj%3Fpage%3D1%26tab%3DSHELL%25E7%25AF%2587%26topicId%3D195&difficulty=undefined&judgeStatus=undefined&tags=&title=)

- 提交解答：

```bash
#!/bin/bash

# 主要思路使用awk 加 进行统计，使用sort进行输出
awk '
{
    for(i=1;i<=NF;i++) {
        arr[$i]++
    }
}
END {
    for(key in arr) {
        printf "%s %d\n",key,arr[key]
    }
}
' nowcoder.txt | sort -k 2


```

- 优质解答：

```bash
# 使用cat 进行文本提取
# 使用xargs 转换为行
# 使用sort 进行排序
# 使用uniq 进行去重统计
# 使用sort 按照数值进行排序
# 使用awk 列输出顺序交换
cat nowcoder.txt | xargs -n1 | sort | uniq -c | sort -n | awk '{print $2, $1}'

```

- 优质解答：

```bash
declare -A map

while read line
do
    # 进行行读取
    arr=$line
    # 遍历行中每一个单次
    for i in ${arr[@]}
    # 进行数量统计
    do
        if [ -z ${map[$i]} ]
        then
            map[$i]=1
        else
            ((map[$i]++))
        fi
    done
 done < nowcoder.txt
 
 mm=()
 
for value in ${map[@]}
do
    mm[${#mm[@]}]=${value}
done
# 在这里进行手动排序
len=${#mm[@]}
for ((i=0;i<len;i++))
do
    for ((j=i+1;j<len;j++))
    do
        if [ ${mm[$i]} -gt ${mm[$j]} ]
        then
            tmp=${mm[$i]}
            mm[$i]=${mm[$j]}
            mm[$j]=$tmp
         fi
     done
 done
 
for ((k=0;k<${#mm[*]};k++))
do
    for key in ${!map[@]}
    do
        if [ ${map[$key]} -eq ${mm[$k]} ]
        then
            echo $key ${map[$key]}
        fi
    done
done
```

### [SHELL10 第二列是否有重复](https://www.nowcoder.com/practice/61b79ffe88964c7ab7b98ae16dd76492?tpId=195&tqId=36220&rp=1&ru=/exam/oj&qru=/exam/oj&sourceUrl=%2Fexam%2Foj%3Fpage%3D1%26tab%3DSHELL%25E7%25AF%2587%26topicId%3D195&difficulty=undefined&judgeStatus=undefined&tags=&title=)


- 提交解答：

```bash
# 主要思路还是使用awk 进行数据过滤
awk '{print $2}' nowcoder.txt | sort | uniq -c | sort -n | awk '{if($1>1) {printf "%d %s\n",$1,$2}}'
# 使用awk进行统计

awk '{ arr[$2]++} END{ for (key in arr) { if (arr[key] >1) { printf "%d %s\n",arr[key],key } } }' nowcoder.txt | sort -n 

```

- 优质解答：

```bash
# 知己使用read line 进行分割

declare -A arr
while read line
do
    temparr=($line)
    arr[${temparr[1]}]=$((${arr[${temparr[1]}]}+1))
done
for key in ${!arr[@]}
do
    if [[ ${arr[$key]} -gt 1 ]]
    then
        echo ${arr[$key]}" "$key
    fi
done
```


- 优质解答2：

```bash
#!/bin/bash
# 用map存储
unset map
declare -A map
# 将每一行及重复数以键值对形式存储
while read line
do
  j=(${line[@]})     #  将每行的数据以数组方式存储,()初始化
  i=${j[1]}     #  将第二列的值赋值给j
  if (( -z ${map[$i]} ))    # 这数据第一次出现
  then
     map[$i]=1
  else
    ((map[$i]++))   # 这数据不是第一次出现，value值加一
  fi
done < nowcoder.txt

for key in ${!map[@]}
do
  if (( ${map[$key]} > 1 ))
  then
    echo ${map[$key]} $key
  fi
done
```

- 官方题解：

```bash
cat nowcoder.txt | awk '{print $2}' | sort | uniq -c | grep -v '1' | sort -n

#先用awk提取文本的第二列信息并对之进行排序，排序后才能用uniq进行去重统计（先去重统计在排序会造成统计不准确），再用grep把没有重复的取反求出（没有重复数值就是1），最后按题目要求再次按数值排序！！！

```

### [SHELL11 转置文件的内容](https://www.nowcoder.com/practice/2240cd809c8f4d80b3479d7c95bb1e2e?tpId=195&tqId=36221&rp=1&ru=/exam/oj&qru=/exam/oj&sourceUrl=%2Fexam%2Foj%3Fpage%3D1%26tab%3DSHELL%25E7%25AF%2587%26topicId%3D195&difficulty=undefined&judgeStatus=undefined&tags=&title=)

- 提交解答：

```bash
#!/bin/bash
# 1. 先统计行列
# 使用awk 输出指定列，使用xargs转换为 行

# 统计列数
line=$(cat nowcoder.txt | head -n 1 |  wc -w)
for i in $(seq 1 $line)
do 
    awk '{print $'''$i'''}' nowcoder.txt | xargs
done 
```

- 优质解答：

```bash
#/bin/bash
 
# 进行统计
lines=()
while read line
do
    lines[${#lines[@]}]=$line

done<nowcoder.txt

# 获取当前列长度
rows=${#lines[@]}
t=(${lines[0]})
columns=${#t[@]}

# 遍历进行数据打印
for ((i=0;i<$columns;i++))
do
    for ((j=0;j<$rows;j++))
    do
        tmp=(${lines[$j]})
        echo "${tmp[$i]} "
    done
done
```

- 优质解答2：

```bash


#awk '{a[NR]=$1} END{print "'${a[1]}',a[2],a[3],a[4],a[5]\nb[1],b[2],b[3],b[4],b[5]"}' nowcoder.txt
#awk '{a[NF]=$1} {print a[NF]}' nowcoder.txt
#
#! /bin/bash
declare -a arr1
declare -a arr2
 
while read line
do
    arr=($line)
    arr1[${#arr1[@]}]=${arr[0]}
    arr2[${#arr2[@]}]=${arr[1]}
done < nowcoder.txt
 
echo ${arr1[@]}
echo ${arr2[@]}
#
```


# [SHELL12 打印每一行出现的数字个数](https://www.nowcoder.com/practice/2d2a124f98054292aef71b453e705ca9?tpId=195&tqId=36222&rp=1&ru=/exam/oj&qru=/exam/oj&sourceUrl=%2Fexam%2Foj%3Fpage%3D1%26tab%3DSHELL%25E7%25AF%2587%26topicId%3D195&difficulty=undefined&judgeStatus=undefined&tags=&title=)

- 提交解答：

```bash
#!/bin/bash

# 1. 注意shell中的计算方式： let / $(()) / `expr $a + $b` 
# 2. 字符串/数组提取： ${line:i:1}
lineNum=0
sumNum=0
while read line
do
    lineCount=0
    # 统计行长度
    num=${#line}
    ((lineNum++))
    for i in $(seq 0 ${num})
    do 
        ch=${line:i:1}
        case $ch in 
            [1-5])
                ((lineCount++))
                #((sumNum++))
                ;;
        esac
    done
    echo "line$lineNum number: $lineCount"
    sumNum=$((sumNum + lineCount))
    #  sumNum=`expr $sumNum + $lineCount`
    #echo "dd $sumNum"
done < nowcoder.txt
echo "sum is $sumNum"

````

- 优质解答1：

```bash
#!/bin/bash
row=0
sum=0
while read line
do
        num=0
        len=${#line}
        for((i=0;i<len;i++))
        do
                tmp=${line:i:1}
                if [[ ${tmp} -eq 1 || ${tmp} -eq 2 || ${tmp} -eq 3 || ${tmp} -eq 4 || ${tmp} -eq 5 ]]
                then
                        ((num++))
                fi
        done
        ((row++))
        ((sum+=num))
        echo "line${row} number:${num}"
done < nowcoder.txt
echo "sum is ${sum}"
```

- 优质解答2：

```bash
# 使用awk -F 进行字符串分割，然后按照行进行统计
#
awk -F "" 'BEGIN{sum=0} {count=0,for(i=1;i<=NF;i++){if($i ~ /[1-5]/){count++}} print "line"NR+1" number:"count,sum += count} END{print "sum is "sum}' nowcoder.txt


# 这里巧妙的使用了字符串分割，检查分割后的字符串列数-1即可
awk -F "[1,2,3,4,5]" 'BEGIN{sum=0}{print "line"NR" number:"(NF-1);sum+=(NF-1)}END{print "sum is "sum}' nowcoder.txt
```


### [SHELL13 去掉所有包含this的句子](https://www.nowcoder.com/practice/2c5a46ef755a4f099368f7588361a8af?tpId=195&tqId=36223&rp=1&ru=/exam/oj&qru=/exam/oj&sourceUrl=%2Fexam%2Foj&difficulty=undefined&judgeStatus=undefined&tags=&title=)

- 提交解答：

```bash
#!/bin/bash
# 使用grep -v 进行基础的操作
cat nowcoder.txt | grep -v 'this'
```

- 优质解答：

```bash
while read line
do
    # 直接使用 =~进行判断
    if [[ $line =~ "this" ]];;then
        continue
    else
        echo $line
    fi
done < nowcoder.txt
```

- 优质解答2：

```bash
# 使用sed 直接进行删除
sed '/this/d'

# awk
awk '$0!~/this/ {print $0}'
```

### [SHELL14 求平均值](https://www.nowcoder.com/practice/c44b98aeaf9942d3a61548bff306a7de?tpId=195&tqId=36224&rp=1&ru=/exam/oj&qru=/exam/oj&sourceUrl=%2Fexam%2Foj%3Fpage%3D1%26tab%3DSHELL%25E7%25AF%2587%26topicId%3D195&difficulty=undefined&judgeStatus=undefined&tags=&title=)

- [shell中的浮点计算](https://www.cnblogs.com/chengmo/archive/2010/09/30/1839556.html)
- [bc命令计算浮点数](https://www.runoob.com/linux/linux-comm-bc.html)

- 提交解答

```bash
#!/bin/bash

# 使用wak 进行统计和输出

awk ' 
BEGIN{
    sum=0
    numberCount=0
}
{
    if(NR==1) {
        numberCount=$1
    } else {
        sum+=$1
    }
}
END{
    res=sum/numberCount*1.000
    printf "%.3f\n",res
}

'  nowcoder.txt 
```

- 优质解答1:

```bash
#!/bin/bash
awk '{if(NR>1) {sum+=$0}} END {printf("%.3f", sum/(NR-1))}' nowcoder.txt
```

- 优质解答2：

```bash
#!/usr/bin/env bash

function solution_1() {
    read count
    local sum=0
    local loop=${count}
    while (( ${loop} > 0)); do
        read m
        sum=$((${sum}+${m}))
        loop=$((${loop}-1))
    done
    # 这里使用scale 指定了浮点数小数位长度
    echo "scale=3; ${sum}/${count}" | bc 
}

function solution_2() {
    read count
    local sum=0
    local loop=1
    while (( ${loop} <= ${count})); do
        read m
        ((sum+=m))
        ((loop++))
    done
    echo "scale=3; ${sum}/${count}" | bc 
}

function solution_3()  {
    awk 'NR==1{all=$0} NR>1{total+=$0} END{printf "%.3f" ,total/all}'
}

function solution_4()  {
    read -p '请输入数组长度：' len
    i=1
    while [ $i -le $len ]
    do
        read -p '请输入数组数字：' num[$i]
        let i++
    done
    for i in ${num[*]}
    do
        sum=$((sum+i))
    done
    echo "scale=3; ${sum}/${len}" | bc 
    #awk -va=$sum -vb=$len 'BEGIN{printf "%.3f\n",a / b }'
}

function solution_999() {
    read n
    sum=0
    read m
    arr=($m)
    for ele in ${arr{@}}; do 
        sum=$((${sum}+${ele}))
    done
    echo "scale=3; ${sum}/${n}" | bc 
}

solution_1
```


### [SHELL15 去掉不需要的单词](https://www.nowcoder.com/practice/838a3acde92c4805a22ac73ca04e503b?tpId=195&rp=1&ru=%2Fexam%2Foj&qru=%2Fexam%2Foj&sourceUrl=%2Fexam%2Foj%3Fpage%3D1%26tab%3DSHELL%25E7%25AF%2587%26topicId%3D195&difficulty=&judgeStatus=&tags=&title=&gioEnter=menu)


- 提交解答：

```bash
#!/bin/bash
# 直接使用grep的反向选择
cat nowcoder.txt | grep  -v -i 'B'
# 如下选择
grep  -E -v "[bB]"
```

- 优质解答：

```bash
#!/user/bin/bash
# 直接读取检查是否含有b
function solution_1() {
    local arr=""
    while read line; do
        arr=(${line})
        for ele in ${arr[@]}; do
            # 含有对应字母，直接跳过
            if [[ "${ele}" =~ B || "${ele}" =~ b ]]; then
                continue
            fi
            echo "${ele} "
        done
    done < nowcoder.txt
}
 
function solution_2() {
    local arr=""
    while read line; do
        arr=(${line})
        for ele in ${arr[@]}; do
            if [[ "${ele}" =~ B|b ]]; then
                continue
            fi
            echo "${ele} "
        done
    done < nowcoder.txt
}
# 直接使用sed进行替换
function solution_4() {
    sed '/B\|b/d' nowcoder.txt
}
# 使用awk
function solution_5() {
    #awk '{ for(i=0; i<NF; i++) if($i ~! /b/) print $i }' nowcoder.txt
    #awk '/\+[^b\+]\+/{print $1}' nowcoder.txt
    :
}
 
function solution_999() {
    while read line; do
        my_array=("${my_array[@]}" $line)
    done
    declare -a pattern=(${my_array[@]/*[B|b]*/})
    echo ${pattern[@]}
}
 
solution_1
```

### [shell16 判断输入的是否为IP地址](https://www.nowcoder.com/practice/ad7b6dbfab2a4267a9991110c57aa64f?tpId=195&tqId=39425&rp=1&ru=/exam/company&qru=/exam/company&sourceUrl=%2Fexam%2Fcompany&difficulty=undefined&judgeStatus=undefined&tags=&title=)


- 提交解答：

```bash
#!/bin/bash
# 主要思路：1. 先用grep 查找是否规范 2. 用awk 判断数字范围是否合法
function is_vailable_ip() {
    local originStr filterStr
    originStr=$1
    filterStr=$(echo $originStr | grep -E "^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$")
    # 1. 检查输入是否合法
    if [[ "$originStr" = "$filterStr" ]]; then
        #2. 检查数字是否合法
        local awkRes
        awkRes=$(echo $originStr | awk -F . '$1<=255&&
            $2<=255&&
            $3<=255&&
            $4<=255 {
                print "yes"
            }')
        if [[ "$awkRes" = "yes"  ]]; then
            echo "yes"
        else
            echo "no"
        fi
    else 
        echo "error"
    fi
}

while read line; do
    is_vailable_ip $line 
done < nowcoder.txt
```

- 优质解答：

```bash
#!/bin/bash
while read line
do 
    # 先进行字符串分割
    arr=(${line//./ })
    if [ ${#arr[*]} -ne 4 ];then
        printf "error\n"
    else
        # 遍历子字符串
        for((i=0;i<${#arr[*]};i++))
            do
                # 检查是否有超过255的数
                [ ${arr[${i}]} -gt 255 ] && printf "no\n" && break
            done
        # 符合条件输出yes
        [ $i == 4 ] && printf "yes\n"
    fi
done < nowcoder.txt

```

- 优质解答2：

```bash
#!/bin/bash
ip_re='^((25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\.){3}(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])$'
while read line;do
    if [[ $line =~ ^([0-9]+\.){3}([0-9]+)$ ]];then
        if [[ $line =~ $ip_re ]];then
            echo 'yes'
        else
            echo 'no'
        fi
    else
        echo 'error'
    fi
done < ./nowcoder.txt
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


### 检查yum包是否安装

```bash
#!/bin/bash
function pkg_install_check() {
    local pkg_name
    pkg_name=$1
    if ! command -v $pkg_name >/dev/null 2>&1 ; then
        yum install -y $pkg_name
    fi
    # 再次进行校验
    if ! command -v $pkg_name >/dev/null 2>&1 ; then
        error_abort 1 "$pkg_name not found"
    fi
}

# 确认jq 包已经安装
pkg_install_check "jq"
```