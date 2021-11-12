#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arrange.h"
#include "city.h"
#include "passenger.h"
#include "timetable.h"
#include "ui_position.h"
#include "ui_addtravel.h"
#include <QDebug>
#include <QDialog>
#include <QPushButton>
#include <QFont>
#include <QLabel>
#include <QTimer>
#include <QString>
#include <QFile>
#include <QObject>
#include <QErrorMessage>

extern int passengerNumber;
extern struct Passenger passenger[maxPassenger];	//乘客信息的结构体数组
extern struct Timetable timetable[maxTimetable];	//时刻表信息的结构体数组

QString id;
int change_seq;
int DAY = 0;
int HOUR = 0;
int SECOND = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int i = 0, j = 0;
    input_city();							//通过文件输入城市信息并用数组存储
    passengerNumber = input_passenger();	//输入乘客信息
    input_timetable();						//输入时刻表信息，并完成城市的邻接表
    arrange_route();						//为每个乘客找到最小风险的路径

    while (j < passengerNumber)
    {
        qDebug()<< passenger[j].cityNumber <<endl;
        i = 0;
        while (i < passenger[j].cityNumber)
        {
            qDebug()<< passenger[j].Route[i] <<endl;
            qDebug()<< passenger[j].typeTravel[i] <<endl;
            qDebug()<< passenger[j].travelNum[i] <<endl;
            i++;
        }
        qDebug()<< passenger[j].Route[i] <<endl;
        qDebug()<< passenger[j].minRisk <<endl;
        qDebug()<< "----------------------------------" <<endl;
        j++;
    }

    //按退出程序按钮退出程序
    connect(ui->pushButton_3,&QPushButton::clicked,[=](){
        this->close();
    });

    //创建五个子窗口
    addtravel = new AddTravel;
    inquire = new Inquire;
    position = new Position;
    cartoon_1 = new cartoon;
    changeRoute = new change_route;

    //计时器
    QTimer * timer = new QTimer(this);
    ui->time->setText("0");
    ui->day->setText("0");
    outputFile(0, 0, 0);
    timer->start(100);
    connect(timer,&QTimer::timeout,[=]()mutable{
       static int num = 0;
       static int num1 = 0;
       SECOND++;
       if( SECOND == 20 )
       {
           num++;
           if(num == 24)
           {
               num = 0;
               num1++;
           }
           hour = num;
           day = num1;
           HOUR = hour;
           DAY = day;
           outputFile(num , num1, 0);
           SECOND = 0;
       }
       //outputFile(num , num1, 0);
       ui->time->setText( QString::number(num) );
       ui->day->setText( QString::number(num1) );
       cartoon_1->update();
       position->search(DAY, HOUR);
    });


    //将按钮1与添加乘客绑定
    connect(ui->pushButton,&QPushButton::clicked,[=]()mutable{
        timer->stop();
        if( addtravel->exec() == AddTravel::Accepted )
        {
            if(addtravel->ui->comboBox->currentText() == addtravel->ui->comboBox_2->currentText())
            {
                QErrorMessage *dialog = new QErrorMessage(this);
                dialog->setWindowTitle("错误信息对话框");
                dialog->showMessage("乘客起点和终点不能为同一个！！！");
                dialog->exec();
            }
            else if(addtravel->ui->lineEdit->text() == "")
            {
                QErrorMessage *dialog = new QErrorMessage(this);
                dialog->setWindowTitle("错误信息对话框");
                dialog->showMessage("乘客的ID不能为空！！！");
                dialog->exec();
            }
            else if(addtravel->ui->comboBox_3->currentText() == "限时最少风险策略" && addtravel->ui->lineEdit_2->text() == "")
            {
                QErrorMessage *dialog = new QErrorMessage(this);
                dialog->setWindowTitle("错误信息对话框");
                dialog->showMessage("乘客的限时不能为空！！！");
                dialog->exec();
            }
            else
            {
                int num_temp = 0, flag_temp = 0;
                while( num_temp < passengerNumber )
                {
                    if( passenger[num_temp].Number == addtravel->ui->lineEdit->text())
                    {
                        flag_temp = 1;
                        QErrorMessage *dialog = new QErrorMessage(this);
                        dialog->setWindowTitle("错误信息对话框");
                        dialog->showMessage("该乘客ID已存在！！！");
                        dialog->exec();
                    }
                    num_temp++;
                }
                if( flag_temp == 0 )
                {
                    addtravel->getinput(day,hour);
                    outputExe( passengerNumber - 1, 0);
                }
            }
        }
        else
        {
            addtravel->no_getinput();
        }
        timer->start(100);
    });

    //将按钮2与查询乘客状态绑定
    connect(ui->pushButton_2,&QPushButton::clicked,[=]()mutable{
        QString output;
        timer->stop();
        if( inquire->exec() == Inquire::Accepted )
        {
            inquire->search();
            timer->start(100);
            position->ui->textEdit->setText("");
            output += tr("查询的乘客编号为： %1 \n").arg(id);
            position->ui->textEdit->append(output);
            outputFile(hour , day, 1);
            position->show();
        }
        else
        {
            inquire->no_search();
            timer->start(100);
        }
    });
    
    //将按钮4与显示动画绑定
    connect(ui->pushButton_4,&QPushButton::clicked,[=]()mutable{
        cartoon_1->show();
    });

    //将按钮5与更改路线绑定
    connect(ui->pushButton_5,&QPushButton::clicked,[=]()mutable{
        timer->stop();
        if( changeRoute->exec() == change_route::Accepted )
        {
            changeRoute->change_ok();
            if(change_seq > 0)
                outputExe(change_seq, 1);
            timer->start(100);
        }
        else
        {
            changeRoute->change_cancel();
            timer->start(100);
        }
    });

}

QString travel_type(int type)
{
    if(type == 0)
        return "飞机";
    else if(type == 1)
        return "火车";
    else
        return "汽车";
}

void MainWindow::outputExe(int Seq, int type)//窗口输出
{
    QString output;
    int i = 0, day_start, hour_start, day_arrive, hour_arrive;
    hour_start = timetable[transform(passenger[Seq].travelNum[0])].startTime;
    if( hour_start >= passenger[Seq].startTimeHour )
        day_start = day;
    else
        day_start = day + 1;
    hour_arrive = hour_start + passenger[Seq].spendTime;
    day_arrive = day_start;
    while( hour_arrive >= 24 )
    {
        day_arrive++;
        hour_arrive = hour_arrive - 24;
    }
    i = 0;
    while( i < passenger[Seq].cityNumber )
    {
        Timetable a;
        a = timetable[transform(passenger[Seq].travelNum[i])];
        if( hour_start > a.startTime && i != 0 )
        {
            day_start++;
        }
        if( i == 0 )
        {
            ui->textEdit->setAlignment(Qt::AlignLeft|Qt::AlignTop);
            if( type == 0 )
                ui->textEdit->append("乘客添加成功!\n");
            else
                ui->textEdit->append("乘客更改路径成功！\n");
            ui->textEdit->append("==========乘客信息 <img src=:/new/prefix1/image/pas_mes1> ==========");
            output = "";
            output += tr("乘客编号： %1\n出发地： %2     目的地： %3\n乘客在整个行程中的风险值为： %4\n").arg(passenger[Seq].Number).arg(passenger[Seq].startPoint).arg(passenger[Seq].finishPoint).arg(passenger[Seq].minRisk);
            ui->textEdit->append(output);
            ui->textEdit->append("==========时间详情 <img src=:/new/prefix1/image/clock> ==========");
            output = "";
            output += tr("预计出发时间： %1日 %2时\n").arg(day_start).arg(hour_start);
            output += tr( "预计抵达时间： %1日 %2时\n本此行程总共耗时： %3小时\n" ).arg(day_arrive).arg(hour_arrive).arg(passenger[Seq].spendTime);
            ui->textEdit->append(output);
            output = "";
            ui->textEdit->append("==========日程安排 <img src=:/new/prefix1/image/arrange> ==========");
            output += tr("#%3: %1 -> %2         ").arg(a.startPoint).arg(a.finishPoint).arg(i + 1);
            ui->textEdit->append(output);
            output = "";
            if( a.travelType == "飞机" )
                ui->textEdit->append("交通工具：飞机 <img src=:/new/prefix1/image/plane>\n");
            else if( a.travelType == "火车" )
                ui->textEdit->append("交通工具：火车 <img src=:/new/prefix1/image/train>\n");
            else
                ui->textEdit->append("交通工具：汽车 <img src=:/new/prefix1/image/bus>\n");
            output += tr("班次： %1\n出发时间： %2日 %3时\n抵达时间： %4日 %5时\n").arg(a.travelNum).arg(day_start).arg(a.startTime).arg(day_start).arg(a.finishTime);
            ui->textEdit->append(output);
            output = "";
        }
        else
        {
            output += tr("#%3: %1 -> %2         ").arg(a.startPoint).arg(a.finishPoint).arg(i + 1);
            ui->textEdit->append(output);
            output = "";
            if( a.travelType == "飞机" )
                ui->textEdit->append("交通工具：飞机 <img src=:/new/prefix1/image/plane>\n");
            else if( a.travelType == "火车" )
                ui->textEdit->append("交通工具：火车 <img src=:/new/prefix1/image/train>\n");
            else
                ui->textEdit->append("交通工具：汽车 <img src=:/new/prefix1/image/bus>\n");
            output += tr("班次： %1\n出发时间： %2日 %3时\n抵达时间： %4日 %5时\n").arg(a.travelNum).arg(day_start).arg(a.startTime).arg(day_start).arg(a.finishTime);
            ui->textEdit->append(output);
            output = "";
        }
        hour_start = a.finishTime;
        i++;
    }
    ui->textEdit->append("-------------------------------------------");
}

void MainWindow::outputFile(int hour,int day,int type)//日志文件输出（type=0表示文件输出1表示在窗口输出）
{
    int i = 0, j = 0;
    int flag = 0;
    QString output;
    QFile *myFile;
    QTextStream *outfile;
    QString filename = ("D:/ds_output.txt");
    myFile = new QFile(filename);
    myFile->open(QIODevice::WriteOnly|QIODevice::Append);
    outfile = new QTextStream(myFile);
    outfile->setCodec(QTextCodec::codecForName("GBK"));//GBK编码格式
    if( type == 0 )
    {
        *outfile << QObject::tr("时间为：") << day << QObject::tr("日") << hour << QObject::tr("时") <<"\n";
    }

    position->ui->textEdit->setText("");
    output += tr("查询的乘客编号为：%1\n").arg(id);
    position->ui->textEdit->append(output);

    while( i < passengerNumber )
    {
        if( id == passenger[i].Number )
        {
            if(((DAY == passenger[i].startTimeDay && HOUR >= passenger[i].startTimeHour) || (DAY > passenger[i].startTimeDay)) && passenger[i].visitedPlace != passenger[i].cityNumber )
            {
                output = "";
                output = tr("该乘客起点为：%1\n终点为：%2\n").arg(passenger[i].startPoint).arg(passenger[i].finishPoint);
                position->ui->textEdit->append(output);
                if( passenger[i].strategyType == -1 )
                {
                    output = "";
                    output = tr("使用的策略是不限时最小风险\n");
                    position->ui->textEdit->append(output);
                }
                else if( passenger[i].strategyType > 0 )
                {
                    output = "";
                    output = tr("使用的策略是限时最小风险 限时：%1小时\n").arg(passenger[i].strategyType);
                    position->ui->textEdit->append(output);
                }
                else if( passenger[i].strategyType == -2 )
                {
                    output = "";
                    output = tr("使用的策略是最短时间到达\n");
                    position->ui->textEdit->append(output);
                }
                output = "";
                output = tr("经过计算该乘客的路程如下\n");
                position->ui->textEdit->append(output);
                int temp_i = 0;
                while( temp_i <= passenger[i].cityNumber)
                {
                    output = "";
                    if( temp_i ==0 )
                        output = tr("%1").arg(passenger[i].Route[temp_i]);
                    else
                        output = tr("->%1").arg(passenger[i].Route[temp_i]);
                    position->ui->textEdit->moveCursor(QTextCursor::End);
                    position->ui->textEdit->insertPlainText(output);
                    temp_i++;
                }
                position->ui->textEdit->append("");
            }
            else if( passenger[i].visitedPlace == passenger[i].cityNumber )
            {
                output = "";
                output = tr("该乘客已到达目的地！\n");
                position->ui->textEdit->append(output);
            }
            flag = 1;
        }

        j = passenger[i].visitedPlace;
        if((passenger[i].visitedPlace < passenger[i].cityNumber) && (passenger[i].startTimeDay < day || (passenger[i].startTimeHour <= hour && passenger[i].startTimeDay == day)))
        {

            if(hour != timetable[transform(passenger[i].travelNum[j])].startTime)
            {
                if( passenger[i].flag == 0 )
                {
                    if( type == 0 )
                    *outfile << QObject::tr("乘客编号为：") << passenger[i].Number << QObject::tr(" ,正在等待从 ") << timetable[transform(passenger[i].travelNum[j])].startPoint << QObject::tr(" 到 ")
                             << timetable[transform(passenger[i].travelNum[j])].finishPoint << QObject::tr(" 的") << travel_type(passenger[i].typeTravel[j]) << QObject::tr(", 该交通工具的编号为：")
                             << timetable[transform(passenger[i].travelNum[j])].travelNum << QObject::tr(" 出发时间为：") << timetable[transform(passenger[i].travelNum[j])].startTime << QObject::tr("时。")
                             << QObject::tr(" 到达时间为：") << timetable[transform(passenger[i].travelNum[j])].finishTime << QObject::tr("时。") << "\n";
                    if( flag == 1 )
                    {
                        output = "";
                        output = tr("乘客编号为： %1\n正在等待从 %2 到 %3 的%4\n该交通工具的编号为：%5\n出发时间为：%6时\n到达时间为：%7时\n").arg(id).arg(timetable[transform(passenger[i].travelNum[j])].startPoint).arg(timetable[transform(passenger[i].travelNum[j])].finishPoint)
                                .arg(travel_type(passenger[i].typeTravel[j])).arg(timetable[transform(passenger[i].travelNum[j])].travelNum).arg(timetable[transform(passenger[i].travelNum[j])].startTime).arg(timetable[transform(passenger[i].travelNum[j])].finishTime);
                        position->ui->textEdit->append(output);
                    }
                }
                else if( passenger[i].flag == 1 && hour >= timetable[transform(passenger[i].travelNum[j])].startTime && hour < timetable[transform(passenger[i].travelNum[j])].finishTime )
                {
                    if( type == 0 )
                    *outfile << QObject::tr("乘客编号为：") << passenger[i].Number << QObject::tr(" ,正在搭乘从 ") << timetable[transform(passenger[i].travelNum[j])].startPoint << QObject::tr(" 到 ")
                             << timetable[transform(passenger[i].travelNum[j])].finishPoint << QObject::tr(" 的") << travel_type(passenger[i].typeTravel[j]) << QObject::tr(", 该交通工具的编号为：")
                             << timetable[transform(passenger[i].travelNum[j])].travelNum << QObject::tr(" 出发时间为：") << timetable[transform(passenger[i].travelNum[j])].startTime << QObject::tr("时。")
                             << QObject::tr(" 到达时间为：") << timetable[transform(passenger[i].travelNum[j])].finishTime << QObject::tr("时。") << "\n";
                    if( flag == 1 )
                    {
                        output = "";
                        output = tr("乘客编号为：%1\n正在搭乘从 %2 到 %3 的%4\n该交通工具的编号为：%5\n出发时间为：%6时\n到达时间为：%7时\n").arg(id).arg(timetable[transform(passenger[i].travelNum[j])].startPoint).arg(timetable[transform(passenger[i].travelNum[j])].finishPoint)
                                .arg(travel_type(passenger[i].typeTravel[j])).arg(timetable[transform(passenger[i].travelNum[j])].travelNum).arg(timetable[transform(passenger[i].travelNum[j])].startTime).arg(timetable[transform(passenger[i].travelNum[j])].finishTime);
                        position->ui->textEdit->append(output);
                    }
                }
                else if( passenger[i].flag == 1 && hour == timetable[transform(passenger[i].travelNum[j])].finishTime )
                {
                    if( type == 0 )
                    *outfile << QObject::tr("乘客编号为：") << passenger[i].Number << QObject::tr(" ,已经到达 ") << timetable[transform(passenger[i].travelNum[j])].finishPoint ;
                    if( flag == 1 )
                    {
                        output = "";
                        output = tr("乘客编号为：%1 \n已经到达 %2 !\n ").arg(passenger[i].Number).arg(timetable[transform(passenger[i].travelNum[j])].finishPoint);
                        position->ui->textEdit->append(output);
                    }
                    j++;
                    passenger[i].visitedPlace++;
                    passenger[i].flag = 0;
                    if( passenger[i].visitedPlace >= passenger[i].cityNumber )
                    {
                        if( type == 0 )
                        *outfile << QObject::tr(" ! 该乘客已到达目的地， 旅程结束！\n");
                        if( flag == 1 )
                        {
                            output = "";
                            output = tr("该乘客已到达目的地， 旅程结束！\n");
                            position->ui->textEdit->append(output);
                        }
                    }

                    if( passenger[i].visitedPlace < passenger[i].cityNumber && hour != timetable[transform(passenger[i].travelNum[j])].startTime )
                    {
                        if( type == 0 )
                        *outfile << QObject::tr(" ! 目前正等待从 ") << timetable[transform(passenger[i].travelNum[j])].startPoint << QObject::tr(" 到 ")
                                 << timetable[transform(passenger[i].travelNum[j])].finishPoint << QObject::tr(" 的") << travel_type(passenger[i].typeTravel[j]) << QObject::tr(", 该交通工具的编号为：")
                                 << timetable[transform(passenger[i].travelNum[j])].travelNum << QObject::tr(" 出发时间为：") << timetable[transform(passenger[i].travelNum[j])].startTime << QObject::tr("时。")
                                 << QObject::tr(" 到达时间为：") << timetable[transform(passenger[i].travelNum[j])].finishTime << QObject::tr("时。") << "\n";
                        if( flag == 1 )
                        {
                            output = "";
                            output = tr("目前正等待从 %1 到 %2 的%3\n该交通工具的编号为：%4\n 出发时间为：%5时\n 到达时间为：%6时\n").arg(timetable[transform(passenger[i].travelNum[j])].startPoint).arg(timetable[transform(passenger[i].travelNum[j])].finishPoint)
                                    .arg(travel_type(passenger[i].typeTravel[j])).arg(timetable[transform(passenger[i].travelNum[j])].travelNum).arg(timetable[transform(passenger[i].travelNum[j])].startTime).arg(timetable[transform(passenger[i].travelNum[j])].finishTime);
                            position->ui->textEdit->append(output);
                        }
                    }
                    else if( passenger[i].visitedPlace < passenger[i].cityNumber && hour == timetable[transform(passenger[i].travelNum[j])].startTime )
                    {
                        passenger[i].flag = 1;
                        if( type == 0 )
                        *outfile << QObject::tr(" !正好搭乘从 ") << timetable[transform(passenger[i].travelNum[j])].startPoint << QObject::tr(" 到 ")
                                 << timetable[transform(passenger[i].travelNum[j])].finishPoint << QObject::tr(" 的") << travel_type(passenger[i].typeTravel[j]) << QObject::tr(", 该交通工具的编号为：")
                                 << timetable[transform(passenger[i].travelNum[j])].travelNum << QObject::tr(" 出发时间为：") << timetable[transform(passenger[i].travelNum[j])].startTime << QObject::tr("时。")
                                 << QObject::tr(" 到达时间为：") << timetable[transform(passenger[i].travelNum[j])].finishTime << QObject::tr("时。") << "\n";
                        if( flag == 1 )
                        {
                            output = "";
                            output = tr("正好搭乘从 %1 到 %2 的%3\n该交通工具的编号为：%4\n出发时间为：%5时\n到达时间为：%6时\n").arg(timetable[transform(passenger[i].travelNum[j])].startPoint).arg(timetable[transform(passenger[i].travelNum[j])].finishPoint)
                                    .arg(travel_type(passenger[i].typeTravel[j])).arg(timetable[transform(passenger[i].travelNum[j])].travelNum).arg(timetable[transform(passenger[i].travelNum[j])].startTime).arg(timetable[transform(passenger[i].travelNum[j])].finishTime);
                            position->ui->textEdit->append(output);
                        }
                    }
                }

            }
            else
            {
                *outfile << QObject::tr("乘客编号为：") << passenger[i].Number << QObject::tr(" ,开始搭乘从 ") << timetable[transform(passenger[i].travelNum[j])].startPoint << QObject::tr(" 到 ")
                         << timetable[transform(passenger[i].travelNum[j])].finishPoint << QObject::tr(" 的") << travel_type(passenger[i].typeTravel[j]) << QObject::tr(", 该交通工具的编号为：")
                         << timetable[transform(passenger[i].travelNum[j])].travelNum << QObject::tr(" 出发时间为：") << timetable[transform(passenger[i].travelNum[j])].startTime << QObject::tr("时。")
                         << QObject::tr(" 到达时间为：") << timetable[transform(passenger[i].travelNum[j])].finishTime << QObject::tr("时。") << "\n";
                if( flag == 1 )
                {
                    output = "";
                    output = tr("乘客编号为：%1\n开始搭乘从 %2 到 %3 的%4\n该交通工具的编号为：%5\n出发时间为：%6时\n到达时间为：%7时\n").arg(id).arg(timetable[transform(passenger[i].travelNum[j])].startPoint).arg(timetable[transform(passenger[i].travelNum[j])].finishPoint)
                            .arg(travel_type(passenger[i].typeTravel[j])).arg(timetable[transform(passenger[i].travelNum[j])].travelNum).arg(timetable[transform(passenger[i].travelNum[j])].startTime).arg(timetable[transform(passenger[i].travelNum[j])].finishTime);
                    position->ui->textEdit->append(output);
                }
                passenger[i].flag = 1;
            }
        }


        i++;
        flag = 0;
    }

    myFile->close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

