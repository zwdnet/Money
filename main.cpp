/*
电子记帐软件1.0（命令行版）
主程序
作者:zwdnet@163.com
日期:2011-10-16
许可协议：GPL3
*/
#include <iostream>
#include "database.h"
#include "datadeal.h"
#include "tools.h"

using namespace std;

int main()
{
	DataBase money; //数据库类
	int input = 0;
	while (input != 16)
	{
		input = getInput();
		switch (input)
		{
		case 1:
			initDataBase(); //初始化数据库
			break;
		case 2:
			backupDataBase(); //备份/恢复数据库
			break;
		case 3:
			addIncome();     //增加一项收入支出
			break;
		case 4:
			removeIncome();  //删除一项收入支出
			break;
		case 5:
			searchIncomeByName(); //用项目名称检索收入支出记录
			break;
		case 6:
			searchIncomeByTime(); //用项目日期检索收入支出记录
			break;
		case 7:
			addInvestment(); //增加一项资产负债
			break;
		case 8:
			removeInvestment();  //删除一项资产负债
			break;
		case 9:
			searchInvestmentByName(); //用项目名称检索资产负债记录
			break;
		case 10:
			searchInvestmentByTime(); //用项目日期检索资产负债记录
			break;
		case 11:
			changeSocietyInsurance();  //增加社保记录
			break;
		case 12:
			outputSocientyInsuranceState(); //输出社保状态
			break;
		case 13:
			outputCashFlow();   //输出指定日期范围的现金流量表
			break;
		case 14:
			outputBalanceSheet(); //输出指定日期范围的资产负债表
			break;
		case 15:
			analysisState(); //分析财务状况
			break;
		default:
			break;
		}
	}
	cout<<"感谢使用，再见！"<<endl;
	return 0;
}
