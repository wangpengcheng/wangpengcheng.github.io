---
layout:     post
title:      C++ Encode 学习笔记
subtitle:   C++ Encode 详细学习笔记
date:       2019-4-11
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - C++
    - encode
    - utf-8 
---

# C++ byte流和字符串文件编解码

------
_参考链接[C++ url编解码类](https://blog.csdn.net/sunflover454/article/details/53885419)_

> 2019-4-11 :00:10:12 ;在刷leetcode的535题时联想到了C++ byte编码和file string的字符编码问题，因此开这篇文章希望能尽量解决，

下面是简单的C++ url UTF-8和GKB的编解码实现

`UrlConvert.h`头文件定义：
```C++
//UrlConvert.h
#ifndef __URLCONVERT_H__
#define __URLCONVERT_H__
#pragma once
/*
使用方法：
CUrlConvert url;
//Encode test
string str1 = url.UrlEncode_GBK("谷姐");
cout << "UrlEncode_GBK " << str1.c_str() <<endl;
string str2 = url.UrlEncode_UTF8("谷姐");
cout << "UrlEncode_UTF8 " << str2.c_str() <<endl;
//Decode test
string str3 = url.UrlDecode_GBK(str1);
cout << "UrlDecode_GBK " << str3.c_str() <<endl;
string str4 = url.UrlDecode_UTF8(str2);
cout << "UrlDecode_UTF8 " << str4.c_str() <<endl;
*/
#include <Windows.h>
#include <string>
 
class CUrlConvert
{
public:
	CUrlConvert(void);
	~CUrlConvert(void);
 
	// URL编码，编码为GBK
	std::string UrlEncode_GBK(std::string strOrg);
	// URL编码，编码为UTF-8
	std::string UrlEncode_UTF8(std::string strOrg);
 
	// URL解码，解码为GBK
	std::string UrlDecode_GBK(std::string strOrg);
	// URL解码，解码为UTF-8
	std::string UrlDecode_UTF8(std::string strOrg);
 
private:
	// URL解码，解码为GBK
	std::string URLDecode(std::string& strOrg);
	// URL编码，编码为GBK
	std::string URLEncode(std::string& strOrg);
 
	std::string GBKToUTF8(const std::string &strGBK);
	std::string UTF8ToGBK(const std::string &strUTF8);
};
 
#endif //__URLCONVERT_H__
```
------

`UrlConvert.cpp`实现文件如下：
```C++
//UrlConvert.cpp
#include "UrlConvert.h"
 
byte toHex(const byte &x)  
{  
	return x > 9 ? x -10 + 'A': x + '0';  
}  
 
byte fromHex(const byte &x)  
{  
	return isdigit(x) ? x-'0' : x-'A'+10;  
} 
 
CUrlConvert::CUrlConvert(void)
{
}
 
 
CUrlConvert::~CUrlConvert(void)
{
}
 
// URL解码，解码为GBK
std::string CUrlConvert::URLDecode(std::string& strOrg)
{
	std::string sOut;  
	for( size_t ix = 0; ix < strOrg.size(); ix++ )  
	{  
		byte ch = 0;  
		if(strOrg[ix]=='%')  
		{  
			ch = (fromHex(strOrg[ix+1])<<4);  
			ch |= fromHex(strOrg[ix+2]);  
			ix += 2;  
		}  
		else if(strOrg[ix] == '+')  
		{  
			ch = ' ';  
		}  
		else  
		{  
			ch = strOrg[ix];  
		}  
		sOut += (char)ch;  
	}  
	return sOut;
}
 
// URL编码，编码为GBK
std::string CUrlConvert::URLEncode(std::string& strOrg)
{
	std::string sOut;  
	for( size_t ix = 0; ix < strOrg.size(); ix++ )  
	{        
		byte buf[4];  
		memset( buf, 0, 4 );  
		if( isalnum( (byte)strOrg[ix] ) )  
		{        
			buf[0] = strOrg[ix];  
		}  
		//else if ( isspace( (byte)strOrg[ix] ) ) //貌似把空格编码成%20或者+都可以  
		//{  
		//    buf[0] = '+';  
		//}  
		else  
		{  
			buf[0] = '%';  
			buf[1] = toHex( (byte)strOrg[ix] >> 4 );  
			buf[2] = toHex( (byte)strOrg[ix] % 16);  
		}  
		sOut += (char *)buf;  
	}  
	return sOut;
}
 
std::string CUrlConvert::GBKToUTF8(const std::string &strGBK)
{
	std::string strOutUTF8 = "";
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(),  - 1, NULL, 0);
	wchar_t *str1 = new wchar_t[n];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(),  - 1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1,  - 1, NULL, 0, NULL, NULL);
	char *str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1,  - 1, str2, n, NULL, NULL);
	strOutUTF8 = str2;
	delete [] str1;
	delete [] str2;
	return strOutUTF8;
}
 
std::string CUrlConvert::UTF8ToGBK(const std::string &strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(),  - 1, NULL, 0);
	wchar_t *wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, (len+1)*sizeof(WCHAR));
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUTF8.c_str(),  - 1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK,  - 1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK,  - 1, szGBK, len, NULL, NULL);
	//strUTF8 = szGBK;
	std::string strTemp(szGBK);
	delete [] szGBK;
	delete [] wszGBK;
	return strTemp;
}
 
// URL编码，编码为GBK
std::string CUrlConvert::UrlEncode_GBK(std::string strOrg)
{
	return URLEncode(strOrg);
}
 
// URL编码，编码为UTF-8
std::string CUrlConvert::UrlEncode_UTF8(std::string strOrg)
{
	return URLEncode(GBKToUTF8(strOrg));
}
 
// URL解码，解码为GBK
std::string CUrlConvert::UrlDecode_GBK(std::string strOrg)
{
	return URLDecode(strOrg);
}
 
// URL解码，解码为UTF-8
std::string CUrlConvert::UrlDecode_UTF8(std::string strOrg)
{
	return UTF8ToGBK(URLDecode(strOrg));
}
```
