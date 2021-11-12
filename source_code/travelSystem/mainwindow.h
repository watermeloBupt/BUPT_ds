#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include "addtravel.h"
#include "inquire.h"
#include "position.h"
#include "cartoon.h"
#include "change_route.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int day = 0, hour = 0;                      //时间的日期和小时
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void outputExe(int Seq, int type);          //添加乘客输出函数
    void outputFile(int hour,int day, int type);//日志文件的输出函数
    AddTravel * addtravel;                      //添加乘客的子窗口
    Inquire * inquire;                          //查询乘客状态的子窗口
    Position * position;                        //显示乘客状态的子窗口
    cartoon * cartoon_1;                        //显示动画的子窗口
    change_route * changeRoute;                 //更改乘客路径的子窗口

private:
    Ui::MainWindow *ui;
};

QString travel_type(int type);
#endif // MAINWINDOW_H
