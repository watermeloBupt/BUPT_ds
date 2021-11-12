#pragma once
#include <QString>
#include <QFile>
#include <QDebug>

struct Timetable							//定义时刻表结构体
{
    QString travelNum;						//交通工具的编号
    QString travelType;                     //交通工具的方式
    QString startPoint;                     //交通工具的起点
    QString finishPoint;					//交通工具的终点
    int startTime;  						//交通工具的起始时间
    int finishTime;							//交通工具的终止时间
};

void input_timetable();						//输入各个城市之间的交通工具,并用邻接表存储城市图
int transform(QString tran_a);				//通过时间表找到时刻表
int find_cityArr(int startSeq, int finishSeq);//在邻接表中寻找，若之前有连线返回1，否则返回0
int find_city(QString cityName);				//寻找城市所对应的数组序号
void updata_info(QString travelNum, int startSeq, int finishSeq, int travelSeq, int startTime, int finishTime);//更新权值
