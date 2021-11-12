#include "arrange.h"
#include "city.h"
#include "passenger.h"
#include "timetable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QString>

int passengerNumber;						//乘客的数量
QString passengerRoute[numberOfCity];		//乘客的路径（DFS用）
int numPlace;								//记录乘客路径上途径的地点数（DFS用）
int spendTime;								//乘客从出发地到目的地的时间
float minimumRisk;							//乘客从出发地到目的地的最低风险值
int visitFlag[numberOfCity];				//乘客的旅行城市标记，避免成环0不在1在
QString tempTravelNum[numberOfCity];		//交通工具的编号

extern struct City city[numberOfCity];				//节点数组
extern struct ArcNode tempCity[numberOfCity];		//用于计算最小风险值
extern struct Passenger passenger[maxPassenger];	//乘客信息的结构体数组
extern struct Timetable timetable[maxTimetable];	//时刻表信息的结构体数组

int find_typeTravel(QString typeTravel)				//用数字表示交通方式:0飞机1火车2汽车
{
    if (typeTravel == "飞机")
        return 0;
    else if (typeTravel == "火车")
        return 1;
    else
        return 2;
}

void DFS_travel(int startSeq, int finishSeq, int Seq) //DFS深搜
{
    struct ArcNode* p = city[startSeq].first_ptr;
    visitFlag[startSeq] = 1;						//对城市进行标记，防止成环
    passengerRoute[numPlace] = city[startSeq].name;
    //如果起点和终点符合旅客的条件则计算风险值（numPlace <= 4是为了优化程序，超过4个则默认不是最优）
    if (city[finishSeq].name == passengerRoute[numPlace] && numPlace <= 4)
    {
        count_risk(Seq);
    }
    numPlace++;
    while (p)
    {
        int j;
        j = p->adjvex;
        if (visitFlag[j] == 0)
        {
            DFS_travel(j, finishSeq, Seq);			//递归
        }
        p = p->nextarc;
    }
    visitFlag[startSeq] = 0;
    numPlace--;
}

void arrange_route()						//对每一个乘客安排路径
{
    int i = 0, startSeq, finishSeq;

    while (i < passengerNumber)				//循环每一位乘客
    {
        startSeq = find_city(passenger[i].startPoint);
        finishSeq = find_city(passenger[i].finishPoint);
        numPlace = 0;
        minimumRisk = 0;
        if( passenger[i].strategyType > 0 )
            DFS_travel(startSeq, finishSeq, i);
        else if( passenger[i].strategyType == -1 )
            Dijkstra_risk(i);
        else if( passenger[i].strategyType == -2 )
            Dijkstra_time(i);
        i++;
    }
}

void count_risk(int Seq)					//计算风险值并找到最小风险值的路径，存到旅客的结构体数组中
{
    int startSeq, finishSeq, count = 0, edge = 1, time = 0, type = 0, timeCheck = 0;
    int i = 0, flag = 0, count_temp = 0, flag1 = 0, j1 = 0, temp = 0, flag2 = 0, travelTypeRisk = 0;
    struct Timetable temp_timetable;
    struct ArcNode* p;
    while (i < numPlace)					//将乘客的旅行路径存在tempCity数组中
    {
        startSeq = find_city(passengerRoute[i]);
        finishSeq = find_city(passengerRoute[i + 1]);
        p = city[startSeq].first_ptr;
        while (p && p->adjvex != finishSeq)
        {
            p = p->nextarc;
        }
        if (p != NULL)
        {
            tempCity[i] = *p;
        }
        i++;
    }

    i = numPlace;							//设置edge边界
    while (i > 1)
    {
        edge = edge * 10;
        i--;
    }

    i = 0;
    while (type < 3)						//第一次的出行方式
    {
        if (type == 0)
            travelTypeRisk = planeRisk;
        else if (type == 1)
            travelTypeRisk = trainRisk;
        else
            travelTypeRisk = busRisk;
        i = 0;
        flag = 0;
        timeCheck = -1;
        while (i < 24 && flag == 0)			//第一次的出行时间
        {
            time = i;
            temp = tempCity[0].spendTime[time][type];
            if (temp == -1)
            {
                flag = 1;
            }
            time = (time + tempCity[0].spendTime[i][type]) % 24;
            tempTravelNum[0] = tempCity[0].travelNum[type][i];
            count = 0;
            count_temp = transform_ternary(count);
            flag2 = 0;
            if (timeCheck == time)
                flag2 = 1;
            //对剩下的城市的旅行方式进行遍历，方法是利用三进制的加法，例如从000加到222，对4个城市之间的三种交通方式进行遍历，找到风险值最小的那个
            while (count_temp < edge && flag == 0 && flag2 == 0)
            {
                j1 = 1;
                time = (temp + i) % 24;
                QString str = tempCity[0].travelNum[type][i];
                temp_timetable = timetable[transform(str)];
                spendTime = temp_timetable.finishTime - temp_timetable.startTime;
                minimumRisk = travelTypeRisk * spendTime * city[find_city(passengerRoute[0])].riskVal;
                flag1 = 0;
                while (j1 < numPlace && flag1 == 0)
                {
                    tempTravelNum[j1] = tempCity[j1].travelNum[((count_temp) % 10) % 3][time];
                    minimumRisk = minimumRisk + tempCity[j1].info[time][((count_temp) % 10) % 3];
                    if (tempCity[j1].info[time][((count_temp) % 10) % 3] == -1)
                    {
                        minimumRisk = 999999;
                        flag1 = 1;
                    }
                    spendTime = spendTime + tempCity[j1].spendTime[time][((count_temp) % 10) % 3];
                    time = (time + tempCity[j1].spendTime[time][((count_temp) % 10) % 3]) % 24;
                    count_temp = count_temp / 10;
                    j1++;
                }
                //每次遍历完，判断是否小于存储的风险值，若小于则代替
                if (minimumRisk < passenger[Seq].minRisk && (passenger[Seq].strategyType == -1 || passenger[Seq].strategyType >= spendTime))
                {
                    passenger[Seq].minRisk = minimumRisk;
                    passenger[Seq].cityNumber = numPlace;
                    passenger[Seq].spendTime = spendTime;
                    int k1 = 0;
                    count_temp = transform_ternary(count);
                    while (k1 < numPlace)
                    {
                        passenger[Seq].Route[k1] = passengerRoute[k1];
                        passenger[Seq].travelNum[k1] = tempTravelNum[k1];
                        if (k1 == 0)
                        {
                            passenger[Seq].typeTravel[k1] = type;
                        }
                        else
                        {
                            passenger[Seq].typeTravel[k1] = count_temp % 10;
                            count_temp = count_temp / 10;
                        }
                        k1++;
                    }
                    passenger[Seq].Route[k1] = passengerRoute[k1];
                }
                count++;
                count_temp = transform_ternary(count);
            }
            timeCheck = (i + tempCity[0].spendTime[i][type]) % 24;
            i++;
        }
        type++;
    }
}

int transform_ternary(int num)						//转换成三进制
{
    int i = 1, ternary = 0;
    while (i <= num)
        i = i * 3;
    i = i / 3;
    while (i != 0)
    {
        ternary = ternary * 10 + num / i;
        num = num - (num / i) * i;
        i = i / 3;
    }
    return ternary;
}

void set_visit_flag()								//重置visitFlag
{
    int i = 0;
    while (i < numberOfCity)
    {
        visitFlag[i] = 0;
        i++;
    }
}

void Dijkstra_risk(int Seq)
{
    int i, j, k, type[numberOfCity][numberOfCity], hour[numberOfCity], spend_time[numberOfCity];
    int flag[numberOfCity], prev[numberOfCity];
    float dist[numberOfCity], min;
    struct ArcNode* p;
    QString num_travel[numberOfCity][numberOfCity];
    // flag[i]=1表示"顶点vs"到"顶点i"的最短路径已成功获取。
    //prev -- 前驱顶点数组。即，prev[i]的值是"顶点vs"到"顶点i"的最短路径所经历的全部顶点中，位于"顶点i"之前的那个顶点。
    //dist -- 长度数组。即，dist[i]是"顶点vs"到"顶点i"的最短路径的长度。
    //type -- 存储A到B的交通方式 0飞机 1火车 2汽车
    //hour -- 存储从起点到该城市后的时间（小时）
    //spend_time -- 存储从起点到该城市花费的时间
    //num_travel -- 存储班号

    // 初始化
    for (i = 0; i < numberOfCity; i++)
    {
        flag[i] = 0;              // 顶点i的最短路径还没获取到。
        prev[i] = 0;              // 顶点i的前驱顶点为0。
        dist[i] = -1;
        hour[i] = -1;
        spend_time[i] = 0;
        for (j = 0; j < numberOfCity; j++)
        {
            type[i][j] = -1;
        }
    }

    p = city[find_city(passenger[Seq].startPoint)].first_ptr;
    while (p)
    {
        float temp = 100000;
        for (i = 0; i < 24; i++)
        {
            for (j = 0; j < 3; j++)
            {
                if (p->info[i][j] < temp && p->info[i][j] > 0)
                {
                    temp = p->info[i][j];
                    spend_time[p->adjvex] = p->spendTime[i][j];
                    type[find_city(passenger[Seq].startPoint)][p->adjvex] = j;
                    num_travel[find_city(passenger[Seq].startPoint)][p->adjvex] = p->travelNum[j][i];
                    hour[p->adjvex] = (i + p->spendTime[i][j]) % 24;
                }
            }
        }
        dist[p->adjvex] = temp;
        prev[p->adjvex] = find_city(passenger[Seq].startPoint);
        p = p->nextarc;
    }

    // 对"顶点vs"自身进行初始化
    flag[find_city(passenger[Seq].startPoint)] = 1;
    dist[find_city(passenger[Seq].startPoint)] = 0;

    // 遍历 numberOfCity - 1次；每次找出一个顶点的最短路径。
    for (i = 1; i < numberOfCity; i++)
    {
        // 寻找当前最小的路径；
        // 即，在未获取最短路径的顶点中，找到离vs最近的顶点(k)。
        min = 100000;
        for (j = 0; j < numberOfCity; j++)
        {
            if (flag[j] == 0 && dist[j] < min && dist[j] > 0)
            {
                min = dist[j];
                k = j;
            }
        }
        // 标记"顶点k"为已经获取到最短路径
        flag[k] = 1;

        // 修正当前最短路径和前驱顶点
        // 即，当已经"顶点k的最短路径"之后，更新"未获取最短路径的顶点的最短路径和前驱顶点"。
        p = city[k].first_ptr;
        while (p)
        {
            for (j = 0; j < 3; j++)
            {
                if (p->info[hour[k]][j] > 0 && (dist[p->adjvex] < 0 || dist[p->adjvex] > (dist[k] + p->info[hour[k]][j])))
                {
                    dist[p->adjvex] = p->info[hour[k]][j] + dist[k];
                    spend_time[p->adjvex] = p->spendTime[hour[k]][j] + spend_time[k];
                    hour[p->adjvex] = (hour[k] + p->spendTime[hour[k]][j]) % 24;
                    prev[p->adjvex] = k;
                    type[k][p->adjvex] = j;
                    num_travel[k][p->adjvex] = p->travelNum[j][hour[k]];
                }
            }
            p = p->nextarc;
        }
    }

    //存储在乘客的结构体数组当中
    int route[numberOfCity];
    int num_route = 0, temp = find_city(passenger[Seq].finishPoint);
    route[0] = find_city(passenger[Seq].finishPoint);
    passenger[Seq].minRisk = dist[route[0]];
    passenger[Seq].spendTime = spend_time[route[0]];
    while (temp != find_city(passenger[Seq].startPoint))
    {
        temp = prev[temp];
        num_route++;
        route[num_route] = temp;
    }


    passenger[Seq].cityNumber = num_route;
    for (i = 0; i < num_route + 1; i++)
    {
        passenger[Seq].Route[num_route - i] = city[route[i]].name;
        if (i != 0)
        {
            passenger[Seq].travelNum[num_route - i] = num_travel[route[i]][route[i - 1]];
            passenger[Seq].typeTravel[num_route - i] = type[route[i]][route[i - 1]];
        }
    }
}

void Dijkstra_time(int Seq)
{
    int i, j, k, type[numberOfCity][numberOfCity], hour[numberOfCity], spend_time[numberOfCity];
    int flag[numberOfCity], prev[numberOfCity], min;
    float dist[numberOfCity];
    struct ArcNode* p;
    QString num_travel[numberOfCity][numberOfCity];
    // flag[i]=1表示"顶点vs"到"顶点i"的最短路径已成功获取。
    //prev -- 前驱顶点数组。即，prev[i]的值是"顶点vs"到"顶点i"的最短路径所经历的全部顶点中，位于"顶点i"之前的那个顶点。
    //dist -- 长度数组。即，dist[i]是"顶点vs"到"顶点i"的最短路径的长度。
    //type -- 存储A到B的交通方式 0飞机 1火车 2汽车
    //hour -- 存储从起点到该城市后的时间（小时）
    //spend_time -- 存储从起点到该城市花费的时间
    //num_travel -- 存储班号

    // 初始化
    for (i = 0; i < numberOfCity; i++)
    {
        flag[i] = 0;              // 顶点i的最短路径还没获取到。
        prev[i] = 0;              // 顶点i的前驱顶点为0。
        dist[i] = -1;			  // 顶点i的最短路径为"顶点vs"到"顶点i"的权。
        hour[i] = -1;
        spend_time[i] = 99999;
        for (j = 0; j < numberOfCity; j++)
        {
            type[i][j] = -1;
        }
    }

    p = city[find_city(passenger[Seq].startPoint)].first_ptr;
    while (p)
    {
        float temp = 100000;
        for (i = 0; i < 24; i++)
        {
            for (j = 0; j < 3; j++)
            {
                if (p->spendTime[i][j] < spend_time[p->adjvex] && p->spendTime[i][j] > 0)
                {
                    temp = p->info[i][j];
                    spend_time[p->adjvex] = p->spendTime[i][j];
                    type[find_city(passenger[Seq].startPoint)][p->adjvex] = j;
                    num_travel[find_city(passenger[Seq].startPoint)][p->adjvex] = p->travelNum[j][i];
                    hour[p->adjvex] = (i + p->spendTime[i][j]) % 24;
                }
            }
        }
        dist[p->adjvex] = temp;
        prev[p->adjvex] = find_city(passenger[Seq].startPoint);
        p = p->nextarc;
    }

    // 对"顶点vs"自身进行初始化
    flag[find_city(passenger[Seq].startPoint)] = 1;
    dist[find_city(passenger[Seq].startPoint)] = 0;
    spend_time[find_city(passenger[Seq].startPoint)] = 0;

    // 遍历 numberOfCity - 1次；每次找出一个顶点的最短路径。
    for (i = 1; i < numberOfCity; i++)
    {
        // 寻找当前最小的路径；
        // 即，在未获取最短路径的顶点中，找到离vs最近的顶点(k)。
        min = 100000;
        for (j = 0; j < numberOfCity; j++)
        {
            if (flag[j] == 0 && spend_time[j] < min)
            {
                min = spend_time[j];
                k = j;
            }
        }
        // 标记"顶点k"为已经获取到最短路径
        flag[k] = 1;

        // 修正当前最短路径和前驱顶点
        // 即，当已经"顶点k的最短路径"之后，更新"未获取最短路径的顶点的最短路径和前驱顶点"。
        p = city[k].first_ptr;
        while (p)
        {
            for (j = 0; j < 3; j++)
            {
                if (p->spendTime[hour[k]][j] > 0 && (spend_time[p->adjvex] > (spend_time[k] + p->spendTime[hour[k]][j])))
                {
                    dist[p->adjvex] = p->info[hour[k]][j] + dist[k];
                    spend_time[p->adjvex] = p->spendTime[hour[k]][j] + spend_time[k];
                    hour[p->adjvex] = (hour[k] + p->spendTime[hour[k]][j]) % 24;
                    prev[p->adjvex] = k;
                    type[k][p->adjvex] = j;
                    num_travel[k][p->adjvex] = p->travelNum[j][hour[k]];
                }
            }
            p = p->nextarc;
        }
    }

    //存储在乘客的结构体数组当中
    int route[numberOfCity];
    int num_route = 0, temp = find_city(passenger[Seq].finishPoint);
    route[0] = find_city(passenger[Seq].finishPoint);
    passenger[Seq].minRisk = dist[route[0]];
    passenger[Seq].spendTime = spend_time[route[0]];
    while (temp != find_city(passenger[Seq].startPoint))
    {
        temp = prev[temp];
        num_route++;
        route[num_route] = temp;
    }


    passenger[Seq].cityNumber = num_route;
    for (i = 0; i < num_route + 1; i++)
    {
        passenger[Seq].Route[num_route - i] = city[route[i]].name;
        if (i != 0)
        {
            passenger[Seq].travelNum[num_route - i] = num_travel[route[i]][route[i - 1]];
            passenger[Seq].typeTravel[num_route - i] = type[route[i]][route[i - 1]];
        }
    }
}
