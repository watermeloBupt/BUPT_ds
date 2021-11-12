#pragma once
#include <QString>
#include <QFile>
#include <QDebug>
#define maxPassenger 10000  				//乘客最大数量
#define numberOfCity 12						//城市的数量
struct Passenger							//定义乘客结构体
{
    QString Number;							//乘客编号
    QString startPoint;                     //乘客的起点
    QString finishPoint;					//乘客的终点
    int startTimeDay;						//乘客的开始要求出发的日期
    int startTimeHour;						//乘客的开始要求出发的时间
    QString travelNum[numberOfCity];		//乘客乘坐的交通工具编号
    int strategyType;						//乘客选择策略类型   -1表示最少风险策略  -2表示最短时间到达策略  正数表示限时最少风险策略的时间
    QString Route[numberOfCity];			//乘客从起点到终点要走的路线
    int typeTravel[numberOfCity];			//乘客的旅行方式
    int cityNumber;							//乘客要走的城市数目
    float minRisk;							//乘客的最小风险值
    int spendTime;                          //乘客整条线路花费的时间
    int visitedPlace;                       //乘客已经到的城市的数量（用于输出）
    int flag;                               //乘客的搭乘状态（0表示等待 1表示搭上交通工具）
};

int input_passenger();						//输入乘客的信息
