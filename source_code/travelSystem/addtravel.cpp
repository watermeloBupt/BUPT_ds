#include "addtravel.h"
#include "ui_addtravel.h"
#include "arrange.h"
#include "city.h"
#include "passenger.h"
#include "timetable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QString>
#include <QErrorMessage>

extern struct Passenger passenger[maxPassenger];	//乘客信息的结构体数组
extern int passengerNumber;                         //乘客的数量
extern int numPlace;                                //记录乘客路径上途径的地点数（DFS用）
extern float minimumRisk;                           //乘客从出发地到目的地的最低风险值

AddTravel::AddTravel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTravel)
{
    ui->setupUi(this);
}

AddTravel::~AddTravel()
{
    delete ui;
}

void AddTravel::getinput(int day,int hour)
{

    //默认从添加乘客的时间小时加1后才能出发
    //例如 在0时5小时添加乘客
    //只能在0时6小时后出发
    if( hour == 23 )
    {
        day++;
        hour = 0;
    }
    else
    {
        hour++;
    }
    //乘客信息初始化
    passenger[passengerNumber].Number = ui->lineEdit->text();
    passenger[passengerNumber].startPoint = ui->comboBox->currentText();
    passenger[passengerNumber].finishPoint = ui->comboBox_2->currentText();
    if( ui->comboBox_3->currentText() == "限时最少风险策略" )
        passenger[passengerNumber].strategyType = ui->lineEdit_2->text().toInt();
    else if( ui->comboBox_3->currentText() == "最少风险策略" )
        passenger[passengerNumber].strategyType = -1;
    else if( ui->comboBox_3->currentText() == "最短时间到达策略" )
        passenger[passengerNumber].strategyType = -2;
    passenger[passengerNumber].minRisk = 99999;
    passenger[passengerNumber].cityNumber = 0;
    passenger[passengerNumber].startTimeDay = day;
    passenger[passengerNumber].startTimeHour = hour;

    qDebug()<< passenger[passengerNumber].startTimeDay <<endl;
    qDebug()<< passenger[passengerNumber].startTimeHour <<endl;

    //计算乘客的最小风险值路径
    int startSeq = find_city(passenger[passengerNumber].startPoint);
    int finishSeq = find_city(passenger[passengerNumber].finishPoint);
    numPlace = 0;
    minimumRisk = 0;
    if( passenger[passengerNumber].strategyType > 0 )
        DFS_travel(startSeq, finishSeq, passengerNumber);
    else if( passenger[passengerNumber].strategyType == -1 )
        Dijkstra_risk(passengerNumber);
    else if( passenger[passengerNumber].strategyType == -2 )
        Dijkstra_time(passengerNumber);

    qDebug()<< passenger[passengerNumber].cityNumber <<endl;
    int i = 0;
    while (i < passenger[passengerNumber].cityNumber)
    {
        qDebug()<< passenger[passengerNumber].Route[i] <<endl;
        qDebug()<< passenger[passengerNumber].typeTravel[i] <<endl;
        qDebug()<< passenger[passengerNumber].travelNum[i] <<endl;
        i++;
    }
    qDebug()<< passenger[passengerNumber].Route[i] <<endl;
    qDebug()<< passenger[passengerNumber].minRisk <<endl;
    qDebug()<< "----------------------------------" <<endl;

    passengerNumber++;

    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    ui->comboBox->setCurrentText("湖北");
    ui->comboBox_2->setCurrentText("湖北");
    ui->comboBox_3->setCurrentText("最少风险策略");
}

void AddTravel::no_getinput()
{
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    ui->comboBox->setCurrentText("湖北");
    ui->comboBox_2->setCurrentText("湖北");
    ui->comboBox_3->setCurrentText("最少风险策略");
}
