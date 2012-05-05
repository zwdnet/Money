#ifndef PUBFUN_H
#define PUBFUN_H
/*
电子记帐软件1.0（命令行版）
命令行版要用的工具函数模块
作者:zwdnet@163.com
日期:2011-10-19
许可协议：GPL3
*/
#include <iostream>
#include <string>
#include <limits>
#include <cmath>
#include "pubfun.h"
#include "database.h"
#include "datadeal.h"

using namespace std;

//公共函数的声明，不加的话下面使用时报错，说函数未定义，原因不知
template <typename T>
string num2str(T n);
int str2int(string s);
float str2float(string s);
//声明结束

//获取用户输入
int getInput(void)
{
	int input = 0;
	int t = 0;
	do
	{
		system("clear");
		if (t > 0)
		{
			cout<<"输入错误，请重新输入!"<<endl;
		}
		t++;
		cout<<"1.初始化数据库（将删除所有现存数据，慎用！)......"<<endl
		    <<"2.备份/恢复数据库................................"<<endl
		    <<"3.增加一个收入/开支记录.........................."<<endl
		    <<"4.删除一个收入/开支记录.........................."<<endl
		    <<"5.用项目名称检索收入/开支记录...................."<<endl
		    <<"6.用日期范围检索收入/开支记录...................."<<endl
		    <<"7.增加一个投资/负债记录.........................."<<endl
		    <<"8.删除一个投资/负债记录.........................."<<endl
	            <<"9.用项目名称检索资产/负债记录...................."<<endl
		    <<"10.用日期范围检索资产/负债记录..................."<<endl
		    <<"11.更改一个社保缴费记录.........................."<<endl
		    <<"12.输出目前的社保状况............................"<<endl
		    <<"13.输出指定日期范围的现金流量表.................."<<endl
		    <<"14.输出指定日期范围的资产负债表.................."<<endl
		    <<"15.分析指定日期范围内的财务状况.................."<<endl
		    <<"16.输出当前数据库中存储的记录的总数.............."<<endl
		    <<"17.退出.........................................."<<endl;
		cout<<"欢迎使用!请按提示输入选择:";
		cin>>input;
		if (input >= 1 && input <= 17)
		{
			break;
		}
		else
		{
		    input = 0;
		    /*以下两行用于出错的时侯清除输入流的缓冲区，不然出错的字符仍在
		    缓冲区内，变成死循环。
		    参考:http://hi.baidu.com/our514/blog/item/47183ce936b2512ab80e2db5.html
		    */
		    cin.clear();
		    cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}while (1);
	return input;
}

//初始化数据库
void initDataBase(void)
{
	system("clear");
	cout<<"程序即将开始初始化数据库，这将删除所有记录。您应该只在开始使用本软件或者想清除所有数据的时侯使用本功能，并建议您在使用前先备份数据库!!!"<<endl
	    <<"确实要执行吗？(yes/no)";
	string answer;
	cin>>answer;
	if (answer == "no")
	{
		return;
	}
	else if (answer == "yes")
	{
		DataBase data;
		data.initDataBase();
		cout<<"数据库初始化完毕！"<<endl;
		cin.get();
		cout<<"按任意键继续......";
		cin.get();
	}
}

//备份/恢复数据库，程序使用了linux的命令，若要移植到其它系统应该修改（具体就是system语句)。
void backupDataBase(void)
{
	int choose = 0;
	do
	{
		system("clear");
		cout<<"1.备份数据库。2.恢复数据库"<<endl;
		cout<<"请输入选择:";
		cin>>choose;
	}while (choose != 1 && choose != 2);
	if (choose == 1) //备份数据库
	{
		cout<<"输入备份文件的完整路径及文件名:";
		string file;
		cin>>file;
		string command = "cp ./money.db ";
		command += file;
		system(command.c_str());
	}
	else if (choose == 2)  //恢复数据库
	{
		cout<<"输入备份文件的完整路径及文件名:";
		string file;
		cin>>file;
		string command = "cp ";
		command += file;
		command += " ./money.db";
		system(command.c_str());
	}
	cout<<"操作完成！按任意键继续......";
	cin.get();
	cin.get();
}

//判断输入的日期是否合法有效
bool judgeTime(int time)
{
	//合法的时间范围从1980年1月1日到2099年12月31日
	if (time < 19800101 || time > 21000101)
	{
		return false;
	}
	//日期转换，找出月份和天数
	int MonthDay = time % (time / 10000);
	int Month = MonthDay / 100;
	int Day = MonthDay % 100;
	if (Month == 0 || Month > 12) //检查月数对不对
	{
		return false;
	}
	if (Day == 0) //天数不能等于0
	{
		return false;
	}
	//检查大月份的天数
	if (Month == 1 || Month == 3 || Month == 5 || Month == 7 || Month == 8 ||
	    Month == 10 || Month == 12)
	{
		if (Day > 31)
		{
			return false;
		}
	}
	//检查2月
	else if (Month == 2)
	{
		//普通年闰年：前2个条件
		//世纪年闰年：后2个条件
		int year = time / 10000;
		if ((year % 4 == 0 && year % 100 != 0) ||
		    (year % 100 == 0 && year % 400 == 0))
		{
			if (Day > 29)
			{
				return false;
			}
		}
		//非闰年
		else
		{
			if (Day > 28)
			{
				return false;
			}
		}
	}
	//检查小月
	else
	{
		if (Day > 30)
		{
			return false;
		}
	}
	//以上检查都通过，说明日期是合法的
	return true;
}

//输入收入支出数据
bool inputIncomeData(Table & data)
{
	Income IncomeData;
	data.ID = NULL;
	cout<<"输入项目发生时间(格式:YYYYMMDD):";
	cin>>data.time;
	if (!judgeTime(data.time))
	{
		cout<<"输入的时间错误，必须在19800101到20991231之间，且为合法日期，按任意键继续";
		cin.get();
		cin.get();
		return false;
	}
	string name;
	cout<<"输入项目名称:";
	cin>>data.name;
	cout<<"输入项目金额，支出为负，收入为正:";
	cin>>data.amount;
	string typeName;
	//输出现有的所有项目类型，供用户选择
	string sql = "SELECT TypeID from IncomeType";
	QueryResult res;
	DataBase database;
	database.runSQL(sql, res);
	int i = 0;
	for (i = 1; i < res.row+1; i++)
	{
		//取得类型名称
		string TypeName = IncomeData.getTypeName(str2int(res.result[i]));
		cout<<i<<"."<<TypeName<<endl;
	}
	int option; //选择
	cout<<"请输入项目类型代码，如果要创建新的类型，请输入0:";
	cin>>option;
	if (option < 0 || option > res.row+1)
	{
		cout<<"输入错误!"<<endl;
		cin.get();
		return false;
	}
	if (option == 0)
	{
		cout<<"输入项目分类:";
		cin>>typeName;
	}
	else
	{
		typeName = IncomeData.getTypeName(str2int(res.result[option]));
	}
	cout<<"输入项目备注(无请输\"无\"):";
	cin>>data.remark;
	int typeID = IncomeData.getTypeID(typeName);
	if (typeID == -1)
	{
		Type IncomeType;
		IncomeType.typeName = typeName;
		IncomeType.typeID = NULL;
		IncomeData.setType(IncomeType);
		typeID = IncomeData.getTypeID(typeName);
	}
	data.typeID = typeID;
	return true;
}

//增加一项收入支出项目
void addIncome(void)
{
	system("clear");
	cout<<"增加一项收入支出项目"<<endl;
	Table IncomeTable;
	if (!inputIncomeData(IncomeTable))
	{
		return;
	}
	Income IncomeData;
	IncomeData.setValue(IncomeTable);
	cout<<"增加收入支出记录成功，按任意键继续......";
	cin.get();
	cin.get();
}

//删除一项收入支出
void removeIncome(void)
{
	system("clear");
	cout<<"删除一项收入支出项目"<<endl;
	cout<<"请输入要删除的项目ID(若不知道，请用软件的其它功能查询):";
	int ID = 0;
	cin>>ID;
	Income IncomeData;
	if (!IncomeData.removeValue(ID))
	{
		cout<<"该项目不存在，删除失败!按任意键继续......";
		cin.get();
		cin.get();
		return;
	}
	cout<<"删除成功，按任意键继续......";
	cin.get();
	cin.get();
	return;
}

//工具函数，输出记录的查询结果,which=1为收入支出记录，which=2为资产负债记录
void outputResult(const string & sql, const int which)
{
	DataBase incomeData;
	QueryResult res;
	if (!incomeData.runSQL(sql, res))
	{
		cout<<"查询失败!按任意键继续......";
		cin.get();
		cin.get();
		return;
	}
	cout<<"查询结果为:"<<endl;
	int t = 0;
	int index = 0;
	for (int i = res.col; i < (res.row+1)*res.col; i++)
	{
		t++;
		if (t == 1)
		{
			index++;
			cout<<"第"<<index<<"项:";
			cout<<"项目ID=";
		}
		else if (t == 2)
		{
			cout<<"项目日期=";
		}
		else if (t == 3)
		{
			cout<<"项目名称=";
		}
		else if (t == 4)
		{
			cout<<"项目金额=";
		}
		else if (t == 5)
		{
			//根据类型ID查找类型名称
			QueryResult TypeName;
			string querysql;
			if (which == 1)
			{
				querysql = "SELECT TypeName FROM IncomeType WHERE TypeID = ";
			}
			else if (which == 2)
			{
				querysql = "SELECT TypeName FROM InvestmentType WHERE TypeID = ";
			}
			querysql += res.result[i];
			querysql += ";";
			incomeData.runSQL(querysql, TypeName);
			cout<<"项目类型="<<TypeName.result[1]<<";";
		}
		else if (t == 6)
		{
			cout<<"备注=";
		}
		if (t != 5)
		{
			cout<<res.result[i]<<";";
		}
		if (t == 6)
		{
			t = 0;
			cout<<endl;
		}
	}
	cout<<"共有"<<index<<"项结果"<<endl;
	cout<<"查询结束，按任意键继续......";
	cin.get();
	cin.get();
}

//用项目名称检索收入支出记录
void searchIncomeByName(void)
{
	system("clear");
	string name;
	cout<<"用项目名称检索收入支出记录"<<endl;
	cout<<"请输入要检索的项目名称:";
	cin>>name;
	string sql = "SELECT * FROM Income where Name like \"%";
	sql += name;
	sql += "%\";";
	outputResult(sql, 1);
}

//用项目日期范围检索收入支出记录
void searchIncomeByTime(void)
{
	system("clear");
	int beginTime, endTime;
	cout<<"用项目日期检索收入支出记录"<<endl;
	cout<<"请输入要检索的项目的日期范围"<<endl;
	cout<<"起始时间:";
	cin>>beginTime;
	cout<<"结束时间:";
	cin>>endTime;
	if (!judgeTime(beginTime) || !judgeTime(endTime))
	{
		cout<<"输入的时间错误，必须在19800101到20991231之间，且为合法日期，按任意键继续";
		cin.get();
		cin.get();
		return;
	}
	if (beginTime > endTime)
	{
		cout<<"输入的时间错误，结束日期必须不小于开始时间，按任意键继续";
		cin.get();
		cin.get();
		return;
	}
	string sql = "SELECT * FROM Income where Time >= ";
	sql += num2str(beginTime);
	sql += " and Time <= ";
	sql += num2str(endTime);
	outputResult(sql, 1);
}

//输入资产负债数据
bool inputInvestmentData(Table & data)
{
	Investment InvestmentData;
	data.ID = NULL;
	cout<<"输入项目发生时间(格式:YYYYMMDD):";
	cin>>data.time;
	if (!judgeTime(data.time))
	{
		cout<<"输入的时间错误，必须在19800101到20991231之间，且为合法日期，按任意键继续";
		cin.get();
		cin.get();
		return false;
	}
	string name;
	cout<<"输入项目名称:";
	cin>>data.name;
	cout<<"输入项目金额，负债为负，资产为正:";
	cin>>data.amount;
	string typeName;
	//输出现有的所有项目类型，供用户选择
	string sql = "SELECT TypeID from InvestmentType";
	QueryResult res;
	DataBase database;
	database.runSQL(sql, res);
	int i = 0;
	for (i = 1; i < res.row+1; i++)
	{
		//取得类型名称
		string TypeName = InvestmentData.getTypeName(str2int(res.result[i]));
		cout<<i<<"."<<TypeName<<endl;
	}
	int option; //选择
	cout<<"请输入项目类型代码，如果要创建新的类型，请输入0:";
	cin>>option;
	if (option < 0 || option > res.row+1)
	{
		cout<<"输入错误!"<<endl;
		cin.get();
		return false;
	}
	if (option == 0)
	{
		cout<<"输入项目分类:";
		cin>>typeName;
	}
	else
	{
		typeName = InvestmentData.getTypeName(str2int(res.result[option]));
	}
	cout<<"输入项目备注(无请输\"无\"):";
	cin>>data.remark;
	int typeID = InvestmentData.getTypeID(typeName);
	if (typeID == -1)
	{
		Type InvestmentType;
		InvestmentType.typeName = typeName;
		InvestmentType.typeID = NULL;
		InvestmentData.setType(InvestmentType);
		typeID = InvestmentData.getTypeID(typeName);
	}
	data.typeID = typeID;
	return true;
}

//增加一项资产负债项目
void addInvestment(void)
{
	system("clear");
	cout<<"增加一项资产负债项目"<<endl;
	Table InvestmentTable;
	if (!inputInvestmentData(InvestmentTable))
	{
		return;
	}
	Investment InvestmentData;
	InvestmentData.setValue(InvestmentTable);
	cout<<"增加资产负债记录成功，按任意键继续......";
	cin.get();
	cin.get();
}

//删除一项资产负债
void removeInvestment(void)
{
	system("clear");
	cout<<"删除一项资产负债项目"<<endl;
	cout<<"请输入要删除的项目ID(若不知道，请用软件的其它功能查询):";
	int ID = 0;
	cin>>ID;
	Investment InvestmentData;
	if (!InvestmentData.removeValue(ID))
	{
		cout<<"该项目不存在，删除失败!按任意键继续......";
		cin.get();
		cin.get();
		return;
	}
	cout<<"删除成功，按任意键继续......";
	cin.get();
	cin.get();
	return;
}

//用项目名称检索资产负债记录
void searchInvestmentByName(void)
{
	system("clear");
	string name;
	cout<<"用项目名称检索资产负债记录"<<endl;
	cout<<"请输入要检索的项目名称:";
	cin>>name;
	string sql = "SELECT * FROM Investment where Name like \"%";
	sql += name;
	sql += "%\";";
	outputResult(sql, 2);
}

//用项目日期范围检索资产负债记录
void searchInvestmentByTime(void)
{
	system("clear");
	int beginTime, endTime;
	cout<<"用项目日期检索资产负债记录"<<endl;
	cout<<"请输入要检索的项目的日期范围"<<endl;
	cout<<"起始时间:";
	cin>>beginTime;
	cout<<"结束时间:";
	cin>>endTime;
	if (!judgeTime(beginTime) || !judgeTime(endTime))
	{
		cout<<"输入的时间错误，必须在19800101到20991231之间，且为合法日期，按任意键继续";
		cin.get();
		cin.get();
		return;
	}
	if (beginTime > endTime)
	{
		cout<<"输入的时间错误，结束日期必须不小于开始时间，按任意键继续";
		cin.get();
		cin.get();
		return;
	}
	string sql = "SELECT * FROM Investment where Time >= ";
	sql += num2str(beginTime);
	sql += " and Time <= ";
	sql += num2str(endTime);
	outputResult(sql, 2);
}

//更改社保个人缴费记录
void changeSocietyInsurance()
{
	DataBase data;
	data.openDataBase();
	system("clear");
	cout<<"增加一项社保个人缴费记录"<<endl;
	cout<<"输入项目发生时间(格式:YYYYMMDD):";
	int time;
	cin>>time;
	if (!judgeTime(time))
	{
		cout<<"输入的时间错误，必须在19800101到20991231之间，且为合法日期，按任意键继续";
		cin.get();
		cin.get();
		return;
	}

	QueryResult res;
	float old, med, job, house; //养老、医疗、失业、公积金的当前值
	string sql = "SELECT * FROM SocietyInsurance;";
	if (!data.runSQL(sql, res) || (res.row == 0 && res.col == 0))
	{
		old = 0.0;
		med = 0.0;
		job = 0.0;
		house = 0.0;
	}
	else
	{
		old = str2float(res.result[6]);
		med = str2float(res.result[7]);
		job = str2float(res.result[8]);
		house = str2float(res.result[9]);
	}
	float newOld, newMed, newJob, newHouse;  //输入的社保缴费值
	cout<<"输入社保各个项目的变更值：(正值为缴费，负值为消耗，如用医保看病)"
	    <<endl;
	cout<<"养老保险:";
	cin>>newOld;
	cout<<"医疗保险:";
	cin>>newMed;
	cout<<"失业保险:";
	cin>>newJob;
	cout<<"住房公积金:";
	cin>>newHouse;
	old += newOld;
	med += newMed;
	job += newJob;
	house += newHouse;
	sql = "UPDATE SocietyInsurance Set Time = ";
	sql += num2str(time);
	sql += ", Old = ";
	sql += num2str(old);
	sql += ", Med = ";
	sql += num2str(med);
	sql += ", Job = ";
	sql += num2str(job);
	sql += ", House = ";
	sql += num2str(house);
	sql += ";";
	if (!data.modifyDataBase(sql))
	{
		cout<<"修改社保缴费记录失败!"<<endl;
	}
	else
	{
		cout<<"修改成功！"<<endl;
	}
	data.closeDataBase();
	cout<<"按任意键继续"<<endl;
	cin.get();
	cin.get();
}

//输出当前的社保个人缴费情况
void outputSocientyInsuranceState()
{
	DataBase data;
	data.openDataBase();
	system("clear");
	cout<<"当前您的社保个人缴费总数为:"<<endl;
	QueryResult res;
	float old, med, job, house; //养老、医疗、失业、公积金的当前值
	string sql = "SELECT * FROM SocietyInsurance;";
	if (!data.runSQL(sql, res) || (res.col == 0 && res.row == 0))
	{
		cout<<"没有您的缴费记录"<<endl;
		cout<<"按任意键继续......"<<endl;
		cin.get();
		cin.get();
		data.closeDataBase();
		return;
	}
	old = str2float(res.result[6]);
	med = str2float(res.result[7]);
	job = str2float(res.result[8]);
	house = str2float(res.result[9]);
	cout<<"最后缴费时间:"<<res.result[5]<<endl;
	cout<<"养老保险个人缴费总额:"<<old<<endl;
	cout<<"医疗保险个人缴费总额:"<<med<<endl;
	cout<<"失业保险个人缴费总额:"<<job<<endl;
	cout<<"住房公积金个人缴费总额:"<<house<<endl;
	cout<<"按任意键继续......"<<endl;
	cin.get();
	cin.get();
	data.closeDataBase();
}

//输出指定日期范围的现金流量表
void outputCashFlow(void)
{
	system("clear");
	int beginTime, endTime;
	cout<<"输出指定日期范围的现金流量表"<<endl;
	cout<<"请输入要检索的项目的日期范围"<<endl;
	cout<<"起始时间:";
	cin>>beginTime;
	cout<<"结束时间:";
	cin>>endTime;
	if (!judgeTime(beginTime) || !judgeTime(endTime))
	{
		cout<<"输入的时间错误，必须在19800101到20991231之间，且为合法日期，按任意键继续";
		cin.get();
		cin.get();
		return;
	}
	Income IncomeData;
	//计算相关数据
	float TotalIncome = 0.0, TotalExpense = 0.0, Total = 0.0;
	TotalIncome = IncomeData.getSumIncome(beginTime, endTime);
	TotalExpense = IncomeData.getSumExpense(beginTime, endTime);
	Total = IncomeData.getSum(beginTime, endTime);

	//输出
	string space = "        ";//用于输出时空格
	string line = "----------------------------------------------"; //分割线
	system("clear");
	cout<<"从"<<beginTime<<"到"<<endTime<<"的现金流量表"<<endl;
	cout<<line<<endl;
	cout<<"总收入="<<TotalIncome<<"元,其中"<<endl;
	string sql = "SELECT TypeID from IncomeType";
	QueryResult res;
	DataBase database;
	database.runSQL(sql, res);
	for (int i = 1; i < res.row+1; i++)
	{
		//取得类型名称
		string TypeName = IncomeData.getTypeName(str2int(res.result[i]));
		if (TypeName == "NULL" || IncomeData.getSumByType(beginTime,
					endTime, str2int(res.result[i])) <= 0)
		{
			continue;
		}
		cout<<space;
		cout<<TypeName<<" = "<<IncomeData.getSumByType(beginTime,
				endTime, str2int(res.result[i]))<<endl;
	}
	cout<<line<<endl;
	cout<<"总支出="<<fabs(TotalExpense)<<"元,其中"<<endl;
	for (int i = 1; i < res.row+1; i++)
	{
		//取得类型名称
		string TypeName = IncomeData.getTypeName(str2int(res.result[i]));
		if (TypeName == "NULL" || IncomeData.getSumByType(beginTime,
					endTime, str2int(res.result[i])) >= 0)
		{
			continue;
		}
		cout<<space;
		cout<<TypeName<<" = "<<-IncomeData.getSumByType(beginTime,
				endTime, str2int(res.result[i]))<<endl;
	}
	cout<<line<<endl;
	cout<<"收入-支出="<<Total<<endl;
	cout<<line<<endl;
	cout<<"输出完毕，按任意键继续......";
	cin.get();
	cin.get();
}

//输出指定日期范围的资产负债表
void outputBalanceSheet(void)
{
	system("clear");
	int beginTime, endTime;
	cout<<"输出指定日期范围的资产负债表"<<endl;
	cout<<"请输入要检索的项目的日期范围"<<endl;
	cout<<"起始时间:";
	cin>>beginTime;
	cout<<"结束时间:";
	cin>>endTime;
	if (!judgeTime(beginTime) || !judgeTime(endTime))
	{
		cout<<"输入的时间错误，必须在19800101到20991231之间，且为合法日期，按任意键继续";
		cin.get();
		cin.get();
		return;
	}
	Investment InvestmentData;
	//计算相关数据
	float TotalInvestment = 0.0, TotalDebt = 0.0, Total = 0.0;
	TotalInvestment = InvestmentData.getSumInvestment(beginTime, endTime);
	TotalDebt = InvestmentData.getSumDebt(beginTime, endTime);
	Total = InvestmentData.getSum(beginTime, endTime);

	//输出
	string space = "        ";//用于输出时空格
	string line = "----------------------------------------------"; //分割线
	system("clear");
	cout<<"从"<<beginTime<<"到"<<endTime<<"的资产负债表"<<endl;
	cout<<line<<endl;
	cout<<"总资产="<<TotalInvestment<<"元,其中"<<endl;
	string sql = "SELECT TypeID from InvestmentType";
	QueryResult res;
	DataBase database;
	database.runSQL(sql, res);
	for (int i = 1; i < res.row+1; i++)
	{
		//取得类型名称
		string TypeName = InvestmentData.getTypeName(str2int(res.result[i]));
		if (TypeName == "NULL" || InvestmentData.getSumByType(beginTime,
					 endTime, str2int(res.result[i])) <= 0)
		{
			continue;
		}
		cout<<space;
		cout<<TypeName<<" = "<<InvestmentData.getSumByType(beginTime,
				endTime, str2int(res.result[i]))<<endl;
	}
	cout<<line<<endl;
	cout<<"总负债="<<fabs(TotalDebt)<<"元,其中"<<endl;
	for (int i = 1; i < res.row+1; i++)
	{
		//取得类型名称
		string TypeName = InvestmentData.getTypeName(str2int(res.result[i]));
		if (TypeName == "NULL" || InvestmentData.getSumByType(beginTime,
					endTime, str2int(res.result[i])) >= 0)
		{
			continue;
		}
		cout<<space;
		cout<<TypeName<<" = "<<-InvestmentData.getSumByType(beginTime,
				endTime, str2int(res.result[i]))<<endl;
	}
	cout<<line<<endl;
	cout<<"净资产="<<Total<<endl;
	cout<<line<<endl;
	cout<<"输出完毕，按任意键继续......";
	cin.get();
	cin.get();
}

/*分析指定日期范围的财务状况
  所有数据的计算方法和数据解释均来自于《个人理财教程》，张颖著，2007年3月北京第一版，对外经济贸易大学出版社*/
void analysisState(void)
{
	system("clear");
	int beginTime, endTime;
	cout<<"分析指定日期范围的财务状况"<<endl;
	cout<<"请输入要检索的项目的日期范围"<<endl;
	cout<<"起始时间:";
	cin>>beginTime;
	cout<<"结束时间:";
	cin>>endTime;
	if (!judgeTime(beginTime) || !judgeTime(endTime))
	{
		cout<<"输入的时间错误，必须在19800101到20991231之间，且为合法日期，按任意键继续";
		cin.get();
		cin.get();
		return;
	}
	string sql;
	DataBase database;
	Income IncomeData;
	Investment InvestmentData;
	string space = "        ";//用于输出时空格
	string line = "----------------------------------------------"; //分割线
	system("clear");
	cout<<"从"<<beginTime<<"到"<<endTime<<"的财务状况"<<endl;
	cout<<line<<endl;
	//分析收入支出表
	//盈余/赤字
	float RestDeficit = IncomeData.getSum(beginTime, endTime);
	cout<<"收入-支出="<<RestDeficit<<"元"<<endl;
	if (RestDeficit > 0)
	{
		cout<<"您在本时间段内的收入支出有盈余，状态正常"<<endl;
	}
	else if (RestDeficit == 0)
	{
		cout<<"您在本时间段内的收入支出刚好平衡，没有盈余，应该增收节支啦"<<endl;
	}
	else
	{
		cout<<"您在本时间段内的收入支出出现赤字啦，请增收节支，以改善您的财务状况"<<endl;
	}
	//计算用户本时间段的恩格尔系数，资料来自维基百科、百度百科等
	float SumExpense = IncomeData.getSumExpense(beginTime, endTime); //获取时间段内总支出
	int foodID = IncomeData.getTypeID("饮食"); //获取食品支出的TypeID
	float SumFoodExpense = IncomeData.getSumByType(beginTime, endTime, foodID); //获得时间段内的食品支出总数
	float Engel = SumFoodExpense/SumExpense; //计算恩格尔系数
	cout<<"您在本时间段内的恩格尔系数为:"<<Engel<<",";
	//输出恩格尔系数分析结果
	if (Engel > 0.6)
	{
		cout<<"您目前的生活水平处于贫穷状态。"<<endl;
	}
	else if (Engel <= 0.6 && Engel > 0.5)
	{
		cout<<"您目前的生活水平处于温饱状态。"<<endl;
	}
	else if (Engel <= 0.5 && Engel > 0.4)
	{
		cout<<"您目前的生活水平处于小康状态。"<<endl;
	}
	else if (Engel <= 0.4 && Engel > 0.3)
	{
		cout<<"您目前的生活水平处于相对富裕状态。"<<endl;
	}
	else if (Engel <= 0.3 && Engel > 0.2)
	{
		cout<<"您目前的生活水平处于富裕状态。"<<endl;
	}
	else 
	{
		cout<<"您目前的生活水平处于极其富裕状态。"<<endl;
	}
	cout<<line<<endl;
	//资产负债分析
	float TotalInvestment, TotalDebt;
	TotalInvestment = InvestmentData.getSumInvestment(beginTime, endTime);
	TotalDebt = InvestmentData.getSumDebt(beginTime, endTime);
	float TotalSub = TotalInvestment - TotalDebt;
	cout<<"您本期内的总资产="<<TotalInvestment<<"元"<<endl;
	cout<<"总负债="<<TotalDebt<<"元"<<endl;
	cout<<"净资产="<<TotalSub<<"元"<<endl;
	//计算偿付比例
	float RateOfPay = fabs(TotalSub/TotalInvestment);
	cout<<"您的偿付比例="<<RateOfPay<<",";
	if (RateOfPay > 0.8)
	{
		cout<<"您的偿付比例过高，没有充分利用自己的信用额度，建议增加借款优化财务结构"<<endl;
	}
	else if (RateOfPay <= 0.8 && RateOfPay > 0.5)
	{
		cout<<"您的偿付比例适当，建议维持现在的负债水平。"<<endl;
	}
	else
	{
		cout<<"您的偿付比例过低，意味您现在的生活主要靠借债来维持，一旦债务到期或经济不景气，可能资不抵债。建议降低债务水平。"<<endl;
	}
	cout<<line<<endl;
	//计算负债资产比例
	float RateOfDebtInvestment = fabs(TotalDebt/TotalInvestment);
	cout<<"您本期的负债资产比例="<<RateOfDebtInvestment<<",";
	if (RateOfDebtInvestment >= 1)
	{
		cout<<"您的负债大于资产，理论上您已破产!"<<endl;
	}
	else if (RateOfDebtInvestment < 1 && RateOfDebtInvestment >= 0.5)
	{
		cout<<"您的负债太多。"<<endl;
	}
	else
	{
		cout<<"您的负债适度。"<<endl;
	}
	cout<<line<<endl;
	//计算负债收入比例
	float TotalIncome = IncomeData.getSumIncome(beginTime, endTime);
	float RateOfDebtIncome = fabs(TotalDebt/TotalIncome);
	cout<<"您本期的负债收入比例="<<RateOfDebtIncome<<",";
	if (RateOfDebtIncome > 0.4)
	{
		cout<<"您本期内的负债与您的收入相比过高了，请减少债务。"<<endl;
	}
	else if (RateOfDebtIncome <= 0.4 && RateOfDebtIncome >= 0.36)
	{
		cout<<"您本期内的负债与您的收入的比例适宜，请继续保持或减少债务。"<<endl;
	}
	else
	{
		cout<<"您本期内的负债与您的收入的比例偏低，可适当增加债务。"<<endl;
	}
	cout<<line<<endl;
	//流动性比例
	int TypeID = InvestmentData.getTypeID("流动性资产");
	float FlowRate, FlowInvestment;
	float TotalExpense = IncomeData.getSumExpense(beginTime, endTime);
	if (TypeID == -1)
	{
		FlowRate = 0;
	}
	else
	{
		FlowInvestment = InvestmentData.getSumByType(TypeID);
		FlowRate = fabs(FlowInvestment/TotalExpense);
	}
	cout<<"您本期的流动性比例="<<FlowRate<<",";
	if (FlowRate > 6)
	{
		cout<<"您的流动性比例太高了，有点浪费，建议增加投资。"<<endl;
	}
	else if (FlowRate <= 6 && FlowRate >= 3)
	{
		cout<<"您的流动性比例适宜，建议保持。"<<endl;
	}
	else
	{
		cout<<"您的流动性比例太低，建议增加储蓄等流动性资产，已应急。"<<endl;
	}
	cout<<line<<endl;
	cout<<"输出完毕，按任意键继续......";
	cin.get();
	cin.get();
}

//输出当前数据库的状态
void outputDatabaseState(void)
{
	system("clear");
	DataBase database;
	QueryResult res;
	string querystr;
	int incomeNum = 0, investmentNum = 0; //分别记录收入支出项目和投资负债项目记录总数
	//查询收入支出记录总数
	querystr = "select count(*) from Income;";
	if (!database.runSQL(querystr, res))
	{
		cout<<"查询数据库失败！"<<endl;
		return;
	}
	else
	{
		incomeNum = str2int(res.result[1]);
	}
	res.clear();
	//查询投资负债记录总数
	querystr = "select count(*) from Investment;";
	if (!database.runSQL(querystr, res))
	{
		cout<<"查询数据库失败！"<<endl;
		return;
	}
	else
	{
		investmentNum = str2int(res.result[1]);
	}
	cout<<"当前数据库中的记录总数为:"<<endl;
	cout<<"收入支出记录总数:"<<incomeNum<<"项"<<endl;
	cout<<"投资负债记录总数:"<<investmentNum<<"项"<<endl;
	cout<<"查询完毕，按任意键继续......"<<endl;
	cin.get();
	cin.get();
}
#endif
