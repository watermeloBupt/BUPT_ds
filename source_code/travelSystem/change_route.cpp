#include "change_route.h"
#include "ui_change_route.h"
#include "arrange.h"
#include "city.h"
#include "passenger.h"
#include "timetable.h"
#include "mainwindow.h"
#include <QErrorMessage>

extern struct Passenger passenger[maxPassenger];	//乘客信息的结构体数组
extern int passengerNumber;                         //乘客的数量
extern int numPlace;                                //记录乘客路径上途径的地点数（DFS用）
extern float minimumRisk;                           //乘客从出发地到目的地的最低风险值
extern struct Timetable timetable[maxTimetable];	//时刻表信息的结构体数组
extern int DAY;
extern int HOUR;
extern int change_seq;

change_route::change_route(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::change_route)
{
    ui->setupUi(this);
}

change_route::~change_route()
{
    delete ui;
}

void change_route::change_ok()
{
    int i = 0;
    while( i < passengerNumber )
    {
        if( passenger[i].Number == ui->lineEdit->text() )
        {
            if( passenger[i].flag == 0)
            {
                change_seq = i;
                passenger[i].startPoint = passenger[i].Route[passenger[i].visitedPlace];
                passenger[i].visitedPlace = 0;
                passenger[i].finishPoint = ui->comboBox_2->currentText();
                if( ui->comboBox_3->currentText() == "限时最少风险策略" )
                    passenger[i].strategyType = ui->lineEdit_2->text().toInt();
                else if( ui->comboBox_3->currentText() == "最少风险策略" )
                    passenger[i].strategyType = -1;
                else if( ui->comboBox_3->currentText() == "最短时间到达策略" )
                    passenger[i].strategyType = -2;
                passenger[i].minRisk = 99999;
                passenger[i].cityNumber = 0;
                passenger[i].startTimeDay = DAY;
                passenger[i].startTimeHour = HOUR;

                //计算乘客的最小风险值路径
                int startSeq = find_city(passenger[i].startPoint);
                int finishSeq = find_city(passenger[i].finishPoint);
                numPlace = 0;
                minimumRisk = 0;
                if( passenger[i].strategyType > 0 )
                    DFS_travel(startSeq, finishSeq, i);
                else if( passenger[i].strategyType == -1 )
                    Dijkstra_risk(i);
                else if( passenger[i].strategyType == -2 )
                    Dijkstra_time(i);
            }
            else
            {
                change_seq = -1;
                QErrorMessage *dialog = new QErrorMessage(this);
                dialog->setWindowTitle("错误信息对话框");
                dialog->showMessage("请在乘客乘坐该交通工具到达城市后再更改路线！");
                dialog->exec();
            }
        }
        i++;
    }
    ui->lineEdit->setText("");
    ui->comboBox_2->setCurrentText("湖北");
    ui->lineEdit_2->setText("");
    ui->comboBox_3->setCurrentText("最少风险策略");
}

void change_route::change_cancel()
{
    ui->lineEdit->setText("");
    ui->comboBox_2->setCurrentText("湖北");
    ui->lineEdit_2->setText("");
    ui->comboBox_3->setCurrentText("最少风险策略");
}
