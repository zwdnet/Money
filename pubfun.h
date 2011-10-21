#ifndef PUBFUN_H
#define PUBFUN_H
/*
电子记帐软件1.0（命令行版）
公共函数模块
作者:zwdnet@163.com
日期:2011-10-17
许可协议：GPL3
*/

#include <iostream>
#include <string>

using namespace std;

//公共函数,照这里写的：http://blog.csdn.net/touzani/article/details/1623850
//将字符串转换为数字(整数)
template <typename T>
string num2str(T n)
{
	stringstream ss;
	ss<<n;
	return ss.str();
}

//将字符串转换为数字(整数)
int str2int(string s)
{
	int num;
	stringstream ss(s);
	ss>>num;
	return num;
}

//将字符串转换为数字(小数)
float str2float(string s)
{
	float num;
	stringstream ss(s);
	ss>>num;
	return num;
}

#endif
