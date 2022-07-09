---
layout:     post
title:      字符串实现加法和乘法
subtitle:   字符串实现加法和乘法
date:       2019-09-03
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - C/C++
    - Linux
    - 数据结构
    - 程序设计
    - 基本计算操作
---

看到这篇文章比较有趣就转载了

[原文地址](https://blog.csdn.net/shanghairuoxiao/article/details/76577357)

# 字符串实现加法和乘法

## 通过字符串实现加法

```c++

#include <iostream>
#include <string>

using namespace std;

string add(string strLeft, string strRight)
{
    //将字符串转换为数字

    for(string::size_type i = 0; i < strLeft.size(); ++i)
    {
        strLeft[i] -= '0';
    }

    for(string::size_type i = 0; i < strRight.size(); ++i)
    {
        strRight[i] -= '0';
    }

    int carry(0);
    //结果可能的最大位数

    string::size_type maxBits = max(strLeft.size(), strRight.size()) + 1; 
    //创建结果字符串

    string strResult(maxBits, 0);
    int i = maxBits - 1;
    int j = strLeft.size() - 1;
    //先将左值拷贝到结果字符串中
    
    while(j >= 0)
    {
        strResult[i--] = strLeft[j--];
    }

    i = maxBits - 1;
    j = strRight.size() - 1;
    //循环计算，将加数的每一个添加上来

    for( ;j >= 0; --j)
    {
        //carry用来统计是否进位

        carry = (strResult[i] + strRight[j] + carry) / 10;
        //计算各个位数对应的值

        strResult[i] = (strResult[i] + strRight[j] + carry) % 10;
        //计算最后一位的下一位

        strResult[--i] += carry;
    }

    for(string::size_type i = 0; i < strResult.size(); ++i)
    {
        strResult[i] += '0';
    }

    if(strResult[0] == '0')
        return string(strResult.begin() + 1, strResult.end());
    else
        return strResult;
}

int main()
{
    string a, b;
    cout << "请输入两个数：" << endl;
    while(cin >> a >> b)
    {
        cout << "结果等于：";
        cout << add(a, b) << endl;
        cout << "请输入两个数：" << endl;
    }
}
```

## 乘法计算

```c++
/*
 * 对于大数，范围超过了最大整数的时候，通过字符串在实现数据的乘法
 */

#include <iostream>  
#include <string>  
#include <vector>  

using namespace std;  

//求两个大数的乘积（两数均为正数）  
string GetProductOfTwoBigNum( string strNumLeft, string strNumRight )  
{  
    if ( strNumRight.empty() && strNumRight.empty() )  
    {  
        return string("0");  
    }  

    //转换为数字  
    for( string::size_type i = 0; i < strNumLeft.size(); ++i )  
    {  
        strNumLeft[i] -= '0';  
    }  
    for( string::size_type i = 0; i < strNumRight.size(); ++i )  
    {  
        strNumRight[i] -= '0';  
    }  


    string::size_type nMaxBits = strNumLeft.size() + strNumRight.size() + 1;//最大位数，多增加一位，便于编码  
    string strProduct(nMaxBits, 0);
    char szTemp = 0;//每位乘积，辅助变量  
    char szCarrayTemp = 0;//进位信息  
    //双重循环进行遍历乘法

    for( int i = strNumRight.size() - 1; i >= 0; --i )  
    {  
        int k = strNumLeft.size() + i + 1;
        for( int j = strNumLeft.size() - 1; j >= 0; --j )  
        {  
            //位数相乘的临时值

            szTemp = ( strNumRight[i] * strNumLeft[j] + strProduct[k]) % 10;
            //临时进位

            szCarrayTemp = ( strNumRight[i] * strNumLeft[j] + strProduct[k] ) / 10;  
            //更改结果值
            
            strProduct[k] = szTemp;  
            //添加进位

            strProduct[--k] += szCarrayTemp;      
        }  
    }  

    //返回遍历strProduct，从而取出计算的结果  
    string strResult;  
    string::size_type k = 0;  
    while( k < strProduct.size() && strProduct[k] == 0 )  
    {  
        ++k;  
    }  
    for( ; k < strProduct.size(); ++k )  
    {  
        strResult.push_back( strProduct[k] + '0');//转换为字符  
    }  

    if ( strResult.empty() )  
    {  
        strResult.push_back( '0' );  
    }  

    return strResult;  
}  


int main()  
{  
    string strNumLeft;  
    string strNumRight;  


    cout << "输入两个乘数:" << endl;  
    while( cin >> strNumLeft >> strNumRight )  
    {  
        string strResult = GetProductOfTwoBigNum( strNumLeft, strNumRight );  
        cout << "两数之积：" << strResult << endl;  
        cout << "输入两个乘数：" << endl;  
    }  

    return 0;  
}
```