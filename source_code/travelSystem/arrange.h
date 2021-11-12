#pragma once
#include <QString>
#define maxPassenger 10000  				//乘客最大数量
#define maxTimetable 1000   				//一天交通工具的最大次数（包括汽车、火车和飞机）
#define numberOfCity 12						//城市的数量


int find_typeTravel(QString typeTravel);     //用数字表示交通方式:0飞机1火车2汽车
void DFS_travel(int startSeq, int finishSeq, int Seq); //DFS深搜算法求旅行路径
void arrange_route();						//对每一个乘客安排路径
void count_risk(int Seq);					//计算风险值并找到最小风险值的路径，存到旅客的结构体数组中
int transform_ternary(int num);				//转换成三进制
void set_visit_flag();						//重置visitFlag
void Dijkstra_risk(int Seq);                //Dijkstra算法求旅行路径
void Dijkstra_time(int Seq);                //Dijkstra算法求旅行路径
