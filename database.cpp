/*
电子记帐软件1.0（命令行版）
数据库处理模块的实现
作者:zwdnet@163.com
日期:2011-10-16
许可协议：GPL3
*/
#include "database.h"

DataBase::DataBase()
{
	databaseName = "money.db"; //指定数据库文件名
	db = NULL;
	errMsg = "";
	bOpen = false; //开始时数据库关闭
}

DataBase::~DataBase()
{
	//如果打开了数据库，关闭
	if (db != NULL)
	{
		closeDataBase();
	}
	//如果分配了结果空间，收回
	if (result.result != NULL)
	{
		sqlite3_free_table(result.result);
		result.row = 0;
		result.col = 0;
	}
}

//删除数据库文件，等于删除所有数据，重新初始化
void DataBase::rmDataBase(void)
{
	//如果该文件存在，删除之。linux里有效，其它系统应该要修改
	if (access(databaseName, F_OK) != -1)
	{
		system("rm money.db");
	}
}

//打开数据库
bool DataBase::openDataBase(void)
{
	int rc = sqlite3_open(databaseName, &db);
	if (rc)
	{
		errMsg = const_cast<char*>(sqlite3_errmsg(db));
		return false;
	}
	bOpen = true;
	return true;
}

//关闭数据库
bool DataBase::closeDataBase(void)
{
	if (bOpen)
	{
		sqlite3_close(db);
		bOpen = false;
		return true;
	}
	//若数据库是打开的但程序还是到这里，说明关闭有误。
	if (bOpen)
	{
		return false;
	}
	//到这说明数据库本来就是没打开的。
	return true;
}

//创建数据库及相关表，数据库建模及结构请看文档
bool DataBase::createDataBase(void)
{
	//如果数据库未打开，先打开数据库（如果没有该数据库则自动创建）
	if (!bOpen)
	{
		if (!openDataBase())
		{
			return false;
		}
	}

	//创建收入支出表
	string sql = "CREATE TABLE Income( \
		      ID INTEGER PRIMARY KEY, \
		      Time DATE, \
		      Name NVARCHAR, \
		      Amount DECIMAL(7,2), \
		      TypeID INTEGER,  \
		      Remark NVARCHAR);";
	sqlite3_stmt * statement;
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, NULL) != SQLITE_OK)
	{
		errMsg = "插入收入支出表错误!";
		return false;
	}
	int success = sqlite3_step(statement);
	sqlite3_finalize(statement);
	if (success != SQLITE_DONE)
	{
		errMsg = "插入收入支出表错误!";
		return false;
	}

	//创建收入支出分类表
	sql = "CREATE TABLE IncomeType( \
	       TypeID INTEGER PRIMARY KEY, \
	       TypeName NVARCHAR);";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, NULL) != SQLITE_OK)
	{
		errMsg = "插入收入支出分类表错误!";
		return false;
	}
	success = sqlite3_step(statement);
	sqlite3_finalize(statement);
	if (success != SQLITE_DONE)
	{
		errMsg = "插入收入支出分类表错误!";
		return false;
	}

	//创建资产负债表
	sql = "CREATE TABLE Investment( \
		      ID INTEGER PRIMARY KEY, \
		      Time DATE, \
		      Name NVARCHAR, \
		      Amount DECIMAL(7,2), \
		      TypeID INTEGER,  \
		      Remark NVARCHAR);";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, NULL) != SQLITE_OK)
	{
		errMsg = "插入资产负债表错误!";
		return false;
	}
	success = sqlite3_step(statement);
	sqlite3_finalize(statement);
	if (success != SQLITE_DONE)
	{
		errMsg = "插入资产负债表错误!";
		return false;
	}

	//创建资产负债分类表
	sql = "CREATE TABLE InvestmentType( \
	       TypeID INTEGER PRIMARY KEY, \
	       TypeName NVARCHAR);";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, NULL) != SQLITE_OK)
	{
		errMsg = "插入资产负债分类表错误!";
		return false;
	}
	success = sqlite3_step(statement);
	sqlite3_finalize(statement);
	if (success != SQLITE_DONE)
	{
		errMsg = "插入资产负债分类表错误!";
		return false;
	}

	//创建社保表，只保存到最近日期的个人扣除值总额
	sql = "CREATE TABLE SocietyInsurance( \
	       Time DATE, \
	       Old DECIMAL(7,2),\
	       Med DECIMAL(7,2),\
	       Job DECIMAL(7,2),\
	       House DECIMAL(7,2));";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, NULL) != SQLITE_OK)
	{
		errMsg = "插入社保个人缴费表错误!";
		return false;
	}
	success = sqlite3_step(statement);
	sqlite3_finalize(statement);
	if (success != SQLITE_DONE)
	{
		errMsg = "插入社保个人缴费表错误!";
		return false;
	}
	//初始化的时侯先存入一个空数据，否则程序出错
	sql = "insert into SocietyInsurance values(19800101, 0.0, 0.0, 0.0, 0.0);";
	if (!modifyDataBase(sql))
	{
		errMsg = "插入社保个人缴费表错误!";
		return false;
	}

	closeDataBase();
	return true;
}

//获取错误信息
char * DataBase::getErrorMessage(void)
{
	return errMsg;
}

//初始化数据库
bool DataBase::initDataBase(void)
{
	rmDataBase();
	createDataBase();
}

//修改数据库（插入、删除）
bool DataBase::modifyDataBase(const string & querySQL)
{
	if (!bOpen)
	{
		openDataBase();
	}
	sqlite3_stmt * statement;
	if (sqlite3_prepare_v2(db, querySQL.c_str(), -1, &statement, NULL) != SQLITE_OK)
	{
		errMsg = "修改数据库失败!";
		return false;
	}
	int success = sqlite3_step(statement);
	sqlite3_finalize(statement);
	if (success != SQLITE_DONE)
	{
		errMsg = "修改数据库失败!";
		return false;
	}
	closeDataBase();
	return true;
}

//执行SQL查询语句
bool DataBase::runSQL(const string & querySQL,
	    QueryResult & res)
{
	//如果还没打开就查询，会报错。
	if (!bOpen)
	{
		if (!openDataBase())
		{
			return false;
		}
	}
	int rc = sqlite3_get_table(db, querySQL.c_str(), &res.result, &res.row, 
			 &res.col, &errMsg);
	if (rc != 0)
	{
		return false;
	}

	if (!closeDataBase())
	{
		return false;
	}
	return true;
}

/*清除查询结果结构，释放内存，供新的结果存入。因为使用
类的过程中会多次运行查询，多次获得结果，不能等程序
结束后自动调用结构的析构函数来释放内存。
*/
void DataBase::clearResult(QueryResult & res)
{
	if (res.row == 0 || res.col == 0 || res.result != NULL)
	{
		sqlite3_free_table(res.result);
		res.row = 0;
		res.col = 0;
	}
}
