#ifndef DATABASE_H
#define DATABASE_H
/*
电子记帐软件1.0（命令行版）
数据库处理模块，用于封装用sqlite3进行的数据库增删查改及输入指定的查询语句的操作
作者:zwdnet@163.com
日期:2011-10-16
许可协议：GPL3
*/
#include <sqlite3.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;
//保存数据库查询结果的结构
typedef struct queryresult
{
	int row;        //查询结果行数
	int col;        //查询结果列数
	char ** result; //查询结果的二维数组
	void clear()
	{
		if (result != NULL)
		{
			sqlite3_free_table(result);
			row = 0;
			col = 0;
		}
	}
	queryresult()   //结构的构造函数
	{
		row = 0;
		col = 0;
		result = NULL;
	}
	~queryresult() //析构函数，如果存入了结果，释放内存
	{
		clear();
	}
} QueryResult;

//封装数据库操作的类
class DataBase
{
public:
	DataBase();
	~DataBase();
	bool initDataBase();          //初始化数据库
	bool openDataBase();          //打开数据库
	bool closeDataBase();         //关闭数据库
	bool modifyDataBase(const string & querySQL);//修改数据库，插入删除
	bool runSQL(const string & querySQL,
		    QueryResult & res); //运行查询语句querySQL，结果存放到res中
	char * getErrorMessage(void); //获得错误信息
	void clearResult(QueryResult & res); //清除查询结果结构，以供新的查询结果存入

private:
	void rmDataBase();     //内部函数，用于在初始化的时侯删除sqlite3数据库文件
	bool createDataBase(); //创建数据库及相关的表

private:
	sqlite3 * db;  //数据库指针
	char * errMsg; //数据库操作的错误信息
	bool bOpen;    //标志数据库是否打开的变量
	QueryResult result; //查询数据库的结果
	char * databaseName; //数据库名称
};
#endif
