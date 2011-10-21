#include "datadeal.h"
#include "pubfun.h"

/********************************************************/
//收入支出计算开始
Income::Income()
{
	data.openDataBase();
}

Income::~Income()
{
	data.closeDataBase();
}

//获取出错信息
string Income::getErrorMessage()
{
	return errMsg;
}

//将数据插入数据库
bool Income::setValue(Table & IncomeTable)
{
	string sql = "INSERT INTO INCOME VALUES ( ";
	if (IncomeTable.ID == 0)
	{
		sql += "NULL";
	}
	else
	{
		sql += num2str(IncomeTable.ID);
	}
	sql += " , ";
	sql += num2str(IncomeTable.time);
	sql += " , \"";
	sql += IncomeTable.name;
	sql += "\" , ";
	sql += num2str(IncomeTable.amount);
	sql += " , ";
	sql += num2str(IncomeTable.typeID);
	sql += " , ";
	if (IncomeTable.remark == "")
	{
		sql += "NULL";
	}
	else
	{
		sql += "\"";
		sql += IncomeTable.remark;
		sql += "\"";
	}
	sql += ");";
	if (!data.modifyDataBase(sql))
	{
		errMsg = data.getErrorMessage();
		return false;
	}
	return true;
}

//根据ID值删除某一项
bool Income::removeValue(const int & ID)
{
	string sql = "DELETE FROM INCOME WHERE ID = ";
	sql += num2str(ID);
	sql += ";";
	if (!data.modifyDataBase(sql))
	{
		errMsg = data.getErrorMessage();
		return false;
	}
	return true;
}

//根据项目类型名称获得项目类型代码
int Income::getTypeID(const string & typeName) 
{
	QueryResult res;
	string sql = "SELECT TypeID FROM IncomeType WHERE TypeName = \"";
	sql += typeName;
	sql += "\";";
	if (!data.runSQL(sql, res))
	{
		errMsg = data.getErrorMessage();
		return -1;
	}
	else if (res.row == 0 && res.col == 0)
	{
		errMsg = "There isn't any result";
		return -1;
	}
	int ID = str2int(res.result[1]);
	return ID;
}

//根据类型ID取得类型名称
string Income::getTypeName(const int & ID)
{
	QueryResult res;
	string sql = "SELECT TypeName FROM IncomeType WHERE TypeID = ";
	sql += num2str(ID);
	sql += ";";
	if (!data.runSQL(sql, res))
	{
		errMsg = data.getErrorMessage();
		return "NULL";
	}
	else if (res.row == 0 && res.col == 0)
	{
		errMsg = "There isn't any result";
		return "NULL";
	}
	string TypeName = res.result[1];
	return TypeName;
}

//将项目类型插入类型数据库
bool Income::setType(Type & IncomeType)
{
	string sql = "INSERT INTO IncomeType VALUES( ";
	if (IncomeType.typeID != 0)
	{
		sql += num2str(IncomeType.typeID);
	}
	else
	{
		sql += "NULL";
	}
	sql += " , \"";
	sql += IncomeType.typeName;
	sql += "\");";
	if (!data.modifyDataBase(sql))
	{
		errMsg = data.getErrorMessage();
		return false;
	}
	return true;
}

//获取数据库中所有收入支出的总和，看余额多少
float Income::Sum(const string & sql)
{
	QueryResult res;
	if (!data.runSQL(sql, res))
	{
		errMsg = data.getErrorMessage();
		return -10000000; //返回负一亿，表示错误
	}
	else if (res.row == 0 && res.col == 0)
	{
		errMsg = "There isn't any data";
		return -10000000;
	}
	float sum = 0.0;
	for (int i = 1; i < res.row+1; i++)
	{
		sum += str2float(res.result[i]);
	}
	return sum;
}

//获取数据库中所有收入支出的总和，看余额多少
float Income::getSum()
{
	string sql = "SELECT Amount FROM Income;";
	return Sum(sql);
}

//计算从beginTime到endTime时段的收入支出总额
float Income::getSum(const int & beginTime, const int endTime) 
{
	string sql = "SELECT Amount FROM Income Where Time >= ";
	sql += num2str(beginTime);
	sql += " and Time <= ";
	sql += num2str(endTime);
	sql += ";";
	return Sum(sql);
}

//计算数据库内所有收入的总和
float Income::getSumIncome(void)
{
	string sql = "SELECT Amount FROM Income WHERE Amount > 0;";
	return Sum(sql);
}

//计算从beginTime到endTime时段的收入总额
float Income::getSumIncome(const int & beginTime, const int endTime)
{
	string sql = "SELECT Amount FROM Income Where Time >= ";
	sql += num2str(beginTime);
	sql += " and Time <= ";
	sql += num2str(endTime);
	sql += " and ";
	sql += "Amount > 0;";
	return Sum(sql);
}

//计算数据库内所有支出的总和
float Income::getSumExpense(void)
{
	string sql = "SELECT Amount FROM Income WHERE Amount < 0;";
	return Sum(sql);
}

//计算从beginTime到endTime时段的收入总额
float Income::getSumExpense(const int & beginTime, const int endTime)
{
	string sql = "SELECT Amount FROM Income Where Time >= ";
	sql += num2str(beginTime);
	sql += " and Time <= ";
	sql += num2str(endTime);
	sql += " and ";
	sql += "Amount < 0;";
	return Sum(sql);
}

//计算数据库内特定项目的总和
float Income::getSumByType(const int & TypeID)
{
	string sql = "SELECT Amount FROM Income WHERE TypeID = ";
	sql += num2str(TypeID);
	sql += ";";
	return Sum(sql);
}

//计算从beginTime到endTime时段的特定项目的收入支出总额
float Income::getSumByType(const int & beginTime, const int endTime,
		            const int & TypeID)
{
	string sql = "SELECT Amount FROM Income Where Time >= ";
	sql += num2str(beginTime);
	sql += " and Time <= ";
	sql += num2str(endTime);
	sql += " and ";
	sql += "TypeID = ";
	sql += num2str(TypeID);
	sql += ";";
	return Sum(sql);
}

/********************************************/
//资产负债数据计算类开始

Investment::Investment()
{
	data.openDataBase();
}

Investment::~Investment()
{
	data.closeDataBase();
}

//获取出错信息
string Investment::getErrorMessage()
{
	return errMsg;
}

//将数据插入数据库
bool Investment::setValue(Table & InvestmentTable)
{
	string sql = "INSERT INTO Investment VALUES ( ";
	if (InvestmentTable.ID == 0)
	{
		sql += "NULL";
	}
	else
	{
		sql += num2str(InvestmentTable.ID);
	}
	sql += " , ";
	sql += num2str(InvestmentTable.time);
	sql += " , \"";
	sql += InvestmentTable.name;
	sql += "\" , ";
	sql += num2str(InvestmentTable.amount);
	sql += " , ";
	sql += num2str(InvestmentTable.typeID);
	sql += " , ";
	if (InvestmentTable.remark == "")
	{
		sql += "NULL";
	}
	else
	{
		sql += "\"";
		sql += InvestmentTable.remark;
		sql += "\"";
	}
	sql += ");";
	if (!data.modifyDataBase(sql))
	{
		errMsg = data.getErrorMessage();
		return false;
	}
	return true;
}

//根据ID值删除某一项
bool Investment::removeValue(const int & ID)
{
	string sql = "DELETE FROM Investment WHERE ID = ";
	sql += num2str(ID);
	sql += ";";
	if (!data.modifyDataBase(sql))
	{
		errMsg = data.getErrorMessage();
		return false;
	}
	return true;
}

//根据项目类型名称获得项目类型代码
int Investment::getTypeID(const string & typeName) 
{
	QueryResult res;
	string sql = "SELECT TypeID FROM InvestmentType WHERE TypeName = \"";
	sql += typeName;
	sql += "\";";
	if (!data.runSQL(sql, res))
	{
		errMsg = data.getErrorMessage();
		return -1;
	}
	else if (res.row == 0 && res.col == 0)
	{
		errMsg = "There isn't any result";
		return -1;
	}
	int ID = str2int(res.result[1]);
	return ID;
}

//根据类型ID取得类型名称
string Investment::getTypeName(const int & ID)
{
	QueryResult res;
	string sql = "SELECT TypeName FROM InvestmentType WHERE TypeID = ";
	sql += num2str(ID);
	sql += ";";
	if (!data.runSQL(sql, res))
	{
		errMsg = data.getErrorMessage();
		return "NULL";
	}
	else if (res.row == 0 && res.col == 0)
	{
		errMsg = "There isn't any result";
		return "NULL";
	}
	string TypeName = res.result[1];
	return TypeName;
}

//将项目类型插入类型数据库
bool Investment::setType(Type & InvestmentType)
{
	string sql = "INSERT INTO InvestmentType VALUES( ";
	if (InvestmentType.typeID != 0)
	{
		sql += num2str(InvestmentType.typeID);
	}
	else
	{
		sql += "NULL";
	}
	sql += " , \"";
	sql += InvestmentType.typeName;
	sql += "\");";
	if (!data.modifyDataBase(sql))
	{
		errMsg = data.getErrorMessage();
		return false;
	}
	return true;
}

//获取数据库中所有资产负债总和，看净资产多少
float Investment::Sum(const string & sql)
{
	QueryResult res;
	if (!data.runSQL(sql, res))
	{
		errMsg = data.getErrorMessage();
		return 0; //返回0，表示错误
	}
	else if (res.row == 0 && res.col == 0)
	{
		errMsg = "There isn't any data";
		return 0;
	}
	float sum = 0.0;
	for (int i = 1; i < res.row+1; i++)
	{
		sum += str2float(res.result[i]);
	}
	return sum;
}

//获取数据库中所有资产负债的总和，看净资产多少
float Investment::getSum()
{
	string sql = "SELECT Amount FROM Investment;";
	return Sum(sql);
}

//计算从beginTime到endTime时段的资产负债总额
float Investment::getSum(const int & beginTime, const int endTime) 
{
	string sql = "SELECT Amount FROM Investment Where Time >= ";
	sql += num2str(beginTime);
	sql += " and Time <= ";
	sql += num2str(endTime);
	sql += ";";
	return Sum(sql);
}

//计算数据库内所有资产的总和
float Investment::getSumInvestment(void)
{
	string sql = "SELECT Amount FROM Investment WHERE Amount > 0;";
	return Sum(sql);
}

//计算从beginTime到endTime时段的资产总额
float Investment::getSumInvestment(const int & beginTime, const int endTime)
{
	string sql = "SELECT Amount FROM Investment Where Time >= ";
	sql += num2str(beginTime);
	sql += " and Time <= ";
	sql += num2str(endTime);
	sql += " and ";
	sql += "Amount > 0;";
	return Sum(sql);
}

//计算数据库内所有负债的总和
float Investment::getSumDebt(void)
{
	string sql = "SELECT Amount FROM Investment WHERE Amount < 0;";
	return Sum(sql);
}

//计算从beginTime到endTime时段的负债总额
float Investment::getSumDebt(const int & beginTime, const int endTime)
{
	string sql = "SELECT Amount FROM Investment Where Time >= ";
	sql += num2str(beginTime);
	sql += " and Time <= ";
	sql += num2str(endTime);
	sql += " and ";
	sql += "Amount < 0;";
	return Sum(sql);
}

//计算数据库内特定项目的资产负债总和
float Investment::getSumByType(const int & TypeID)
{
	string sql = "SELECT Amount FROM Investment WHERE TypeID = ";
	sql += num2str(TypeID);
	sql += ";";
	return Sum(sql);
}

//计算从beginTime到endTime时段的特定项目的资产负债总额
float Investment::getSumByType(const int & beginTime, const int endTime,
		            const int & TypeID)
{
	string sql = "SELECT Amount FROM Investment Where Time >= ";
	sql += num2str(beginTime);
	sql += " and Time <= ";
	sql += num2str(endTime);
	sql += " and ";
	sql += "TypeID = ";
	sql += num2str(TypeID);
	sql += ";";
	return Sum(sql);
}
