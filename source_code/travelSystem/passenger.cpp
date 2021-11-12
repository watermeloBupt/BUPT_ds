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

struct Passenger passenger[maxPassenger];	//乘客信息的结构体数组

int input_passenger()						//输入乘客的信息
{
    int i = 0;
    QFile file("D:/passenger.txt");
    file.open(QIODevice::ReadOnly);
    QString str;
    while(!file.atEnd())
    {
        str = file.readLine();
        str = str.simplified();
        QStringList strList = str.split(' ');
        passenger[i].Number = strList[0];
        passenger[i].startPoint = strList[1];
        passenger[i].finishPoint = strList[2];
        passenger[i].startTimeDay = strList[3].toInt();
        passenger[i].startTimeHour = strList[4].toInt();
        passenger[i].strategyType = strList[5].toInt();
        passenger[i].minRisk = 99999;
        passenger[i].cityNumber = 0;
        passenger[i].visitedPlace = 0;
        passenger[i].flag = 0;
        i++;
    }
    file.close();							//关闭文件
    return i;
}
