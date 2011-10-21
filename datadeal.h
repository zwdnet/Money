#ifndef DATADEAL_H
#define DATADEAL_H
/*
电子记帐软件1.0（命令行版）
数据处理模块，用于封装将财务数据插入数据库中以及从数据库中取出数据的操作
作者:zwdnet@163.com
日期:2011-10-17
许可协议：GPL3
*/
#include <iostream>
#include <sstream>
#include <string>
#include <sqlite3.h>
#include "database.h"

using namespace std;

//收入支出/资产负债数据结构
typedef struct table
{
	int ID;  //项目ID
	int time; //项目发生的时间
	string name; //项目名称
	float amount; //项目的金额，如果是支出，为负值
	int typeID; //项目的类型ID
	string remark; //备注
}Table;

//项目类型数据结构
typedef struct type
{
	int typeID; //项目类型的ID
	string typeName; //项目类型的名称
}Type;


//收入支出的类
class Income
{
public:
	Income();
	~Income();
	string getErrorMessage();   //获得出错信息
	bool setValue(Table & IncomeTable); //将某一项收入支出表插入数据库
	bool removeValue(const int & ID);   //根据ID值删除某一项
	int getTypeID(const string & typeName); //根据项目的类型名称获得项目类型ID
	string getTypeName(const int & ID); //根据类型ID取得类型名称
	bool setType(Type & IncomeType); //将项目类型插入类型数据库
	float getSum(); //获取数据库所有收入支出的总和
	//获取从beginTime到endTime时段内数据库所有收入支出的总和
	float getSum(const int & beginTime, const int endTime); 
	float getSumIncome(); //获取数据库内所有收入的总和
	//获取从beginTime到endTime时段内数据库所有收入的总和
	float getSumIncome(const int & beginTime, const int endTime);
	float getSumExpense(); //获取数据库内所有支出的总和
	//获取从beginTime到endTime时段内数据库所有支出的总和
	float getSumExpense(const int & beginTime, const int endTime);
	//获得特定项目的金额总和
	float getSumByType(const int & typeID);
	//获取从beginTime到endTime时段内数据库特定类型项目的金额总和
	float getSumByType(const int & beginTime, const int endTime,
			   const int & typeID);

private:
	float Sum(const string & sql); //查询总数的函数,供各个成员函数调用，其它get*函数
	                              //只要加工sql查询语句就行了，避免重复。

private:
	Table IncomeTable; //收入支出表的数据结构
	DataBase data; //数据库处理类
	string errMsg; //记录错误信息
};

//资产负债的类
class Investment
{
public:
	Investment();
	~Investment();
	string getErrorMessage();   //获得出错信息
	bool setValue(Table & InvestmentTable); //将某一项资产负债表插入数据库
	bool removeValue(const int & ID);   //根据ID值删除某一项
	int getTypeID(const string & typeName); //根据项目的类型名称获得项目类型ID
	string getTypeName(const int & ID); //根据类型ID取得类型名称
	bool setType(Type & IncomeType); //将项目类型插入类型数据库
	float getSum(); //获取数据库所有资产负债的总和
	//获取从beginTime到endTime时段内数据库所有资产负债的总和
	float getSum(const int & beginTime, const int endTime); 
	float getSumInvestment(); //获取数据库内所有资产的总和
	//获取从beginTime到endTime时段内数据库所有资产的总和
	float getSumInvestment(const int & beginTime, const int endTime);
	float getSumDebt(); //获取数据库内所有负债的总和
	//获取从beginTime到endTime时段内数据库所有负债的总和
	float getSumDebt(const int & beginTime, const int endTime);
	//获得特定项目的金额总和
	float getSumByType(const int & typeID);
	//获取从beginTime到endTime时段内数据库特定类型项目的金额总和
	float getSumByType(const int & beginTime, const int endTime,
			   const int & typeID);

private:
	float Sum(const string & sql); //查询总数的函数,供各个成员函数调用，其它get*函数
	                              //只要加工sql查询语句就行了，避免重复。

private:
	Table InvestmentTable; //资产负债表的数据结构
	DataBase data; //数据库处理类
	string errMsg; //记录错误信息
};
#endif
