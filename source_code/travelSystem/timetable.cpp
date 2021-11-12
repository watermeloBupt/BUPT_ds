#include "arrange.h"
#include "city.h"
#include "passenger.h"
#include "timetable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QString>

#define maxPassenger 10000  				//乘客最大数量
#define maxTimetable 1000   				//一天交通工具的最大次数（包括汽车、火车和飞机）
#define numberOfCity 12						//城市的数量

struct Timetable timetable[maxTimetable];	//时刻表信息的结构体数组
extern struct City city[numberOfCity];		//节点数组

void input_timetable()						//输入各个城市之间的交通工具,并用邻接表存储城市图
{
    QString travelNum, startPlace, finishPlace, typeTravel;
    int startTime, finishTime, startSeq, finishSeq, travelSeq, i = 0;
    QFile file("D:/timetable.txt");
    file.open(QIODevice::ReadOnly);
    QString str;
    while(!file.atEnd())
    {
        str = file.readLine();
        str = str.simplified();
        QStringList strList = str.split(' ');
        travelNum = strList[0];
        startPlace = strList[1];
        finishPlace = strList[2];
        typeTravel = strList[3];
        startTime = strList[4].toInt();
        finishTime = strList[5].toInt();

        //将时刻表存储于timetable数组中
        timetable[i].startPoint = startPlace;
        timetable[i].finishPoint = finishPlace;
        timetable[i].travelNum = travelNum;
        timetable[i].startTime = startTime;
        timetable[i].finishTime = finishTime;
        timetable[i].travelType = typeTravel;
        i++;

        //建立邻接表
        startSeq = find_city(startPlace);
        finishSeq = find_city(finishPlace);
        travelSeq = find_typeTravel(typeTravel);
        if (find_cityArr(startSeq, finishSeq) == 1)
        {
            updata_info(travelNum, startSeq, finishSeq, travelSeq, startTime, finishTime);
        }
        else
        {
            struct ArcNode* p = NULL;
            struct ArcNode* p1 = NULL;
            p = new struct ArcNode;
            //p = (ArcNode*)malloc(sizeof(struct ArcNode));
            set_zero(p);
            p->adjvex = finishSeq;
            p->nextarc = NULL;
            if (city[startSeq].first_ptr == NULL)
            {
                city[startSeq].first_ptr = p;
            }
            else
            {
                p1 = city[startSeq].first_ptr;
                while (p1->nextarc != NULL)
                {
                    p1 = p1->nextarc;
                }
                p1->nextarc = p;
            }
            updata_info(travelNum, startSeq, finishSeq, travelSeq, startTime, finishTime);
        }
    }
    file.close();
}

int transform(QString tran_a)								//通过时间表找到时刻表
{
    int i = 0, flag = 1, num = 0;
    while( i < maxTimetable && flag == 1 )
    {
        if( tran_a == timetable[i].travelNum )
        {
            num = i;
            flag = 0;
        }
        i++;
    }
    return num;
}

int find_cityArr(int startSeq, int finishSeq)//在邻接表中寻找，若之前有连线返回1，否则返回0
{
    int flag = 0;
    struct ArcNode* p1 = NULL;
    if (city[startSeq].first_ptr == NULL)
        return 0;
    else
    {
        p1 = city[startSeq].first_ptr;
        while (p1 != NULL)
        {
            if (p1->adjvex == finishSeq)
                flag = 1;
            p1 = p1->nextarc;
        }
        return flag;
    }
}

int find_city(QString cityName)						//寻找城市所对应的数组序号
{
    int i = 0, cityNum = -1;
    while (i < numberOfCity)
    {
        if (city[i].name == cityName)
            cityNum = i;
        i++;
    }
    return cityNum;
}

void updata_info(QString travelNum, int startSeq, int finishSeq, int travelSeq, int startTime, int finishTime)//更新权值
{
    int flag = 0, i = 0, spendTime = 0, travelTypeRisk = 0;
    float riskValue = 0;
    struct ArcNode* p1 = NULL;
    p1 = city[startSeq].first_ptr;
    if (p1->adjvex == finishSeq)
        flag++;
    while (p1 != NULL && flag == 0)					//找到出发点和终点的邻接表
    {
        p1 = p1->nextarc;
        if (p1->adjvex == finishSeq)
            flag++;
    }
    while (i < 24)									//根据时间表对其中的风险值以及花费时间进行计算
    {
        if (travelSeq == 0)
            travelTypeRisk = planeRisk;
        else if(travelSeq == 1)
            travelTypeRisk = trainRisk;
        else
            travelTypeRisk = busRisk;
        spendTime = 0;
        if (i <= startTime)
        {
            spendTime = spendTime + (startTime - i);
            riskValue = (startTime - i) * city[startSeq].riskVal + travelTypeRisk * (finishTime - startTime) * city[startSeq].riskVal;
            spendTime = spendTime + (finishTime - startTime);
        }
        else
        {
            spendTime = spendTime + (24 - (i - startTime));
            riskValue = (24 - (i - startTime)) * city[startSeq].riskVal + travelTypeRisk * (finishTime - startTime) * city[startSeq].riskVal;
            spendTime = spendTime + (finishTime - startTime);
        }
        //若找到比邻接表中更小的风险值则代替
        if (p1 && (p1->info[i][travelSeq] == -1 || p1->info[i][travelSeq] > riskValue))
        {
            p1->info[i][travelSeq] = riskValue;
            p1->travelNum[travelSeq][i] = travelNum;
        }
        //若找到比邻接表中更短的时间则代替
        if (p1 && (p1->spendTime[i][travelSeq] == -1 || p1->spendTime[i][travelSeq] > spendTime))
            p1->spendTime[i][travelSeq] = spendTime;

        i++;
    }
}
