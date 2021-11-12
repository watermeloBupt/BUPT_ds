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

struct City city[numberOfCity];				//节点数组
struct ArcNode tempCity[numberOfCity];		//用于计算最小风险值

void input_city()							//输入城市的名称
{
    int i = 0;
    QString risk;
    QFile file("D:/city.txt");
    file.open(QIODevice::ReadOnly);
    QString str;
    while(!file.atEnd())
    {
        str = file.readLine();
        str = str.simplified();
        QStringList strList = str.split(' ');
        city[i].name = strList[0];
        risk = strList[1];
        if (risk == "高风险")
            city[i].riskVal = highRiskCity;
        else if (risk == "中风险")
            city[i].riskVal = middleRiskCity;
        else
            city[i].riskVal = lowRiskCity;
        i++;
    }
    file.close();							//关闭文件
}

void set_zero(struct ArcNode* p)					//初始化info和spendTime
{
    int i = 0, j = 0;
    while (i < 24)
    {
        j = 0;
        while (j < 3)
        {
            p->info[i][j] = -1;
            p->spendTime[i][j] = -1;
            p->travelNum[j][i] = "-1";
            j++;
        }
        i++;
    }
}

