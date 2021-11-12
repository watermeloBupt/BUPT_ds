#pragma once
#include <QString>
#include <QFile>
#include <QDebug>
#define maxPassenger 10000  				//乘客最大数量
#define maxTimetable 1000   				//一天交通工具的最大次数（包括汽车、火车和飞机）
#define numberOfCity 12						//城市的数量
#define lowRiskCity 0.2                     //低风险城市的风险值
#define middleRiskCity 0.5                  //中风险城市的风险值
#define highRiskCity 0.9                    //高风险城市的风险值
#define planeRisk 9                         //乘坐飞机的风险值
#define trainRisk 5                         //乘坐火车的风险值
#define busRisk 2                           //乘坐汽车的风险值

struct City									//定义城市结构体
{
    QString name;			                //城市的名称
    float riskVal;							//城市的风险值
    struct ArcNode* first_ptr;				//第一个表结点的地址,指向第一条依附该顶点的弧的指针
};

struct ArcNode
{
    int adjvex;					  			//数组对应的位置
    struct ArcNode* nextarc;	  			//指向下一条弧的指针
    float info[24][3];			  			//权值(24表示24小时不同的权值，3表示用三种交通工具不同的权值:0飞机1火车2汽车）
    int spendTime[24][3];					//乘坐三种交通工具所花费的时间（0飞机1火车2汽车）
    QString travelNum[3][24];				//交通工具的编号
};

void input_city();							//输入城市的名称
void set_zero(struct ArcNode* p);			//初始化info和spendTime
