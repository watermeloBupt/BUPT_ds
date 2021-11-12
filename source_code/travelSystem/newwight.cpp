#include "newwight.h"
#include "ui_newwight.h"
#include "mainwindow.h"
#include <QPainter>
#include <QString>
#include "passenger.h"
#include "arrange.h"
#include "timetable.h"

extern struct Passenger passenger[maxPassenger];	//乘客信息的结构体数组
extern struct Timetable timetable[maxTimetable];	//时刻表信息的结构体数组
extern int passengerNumber;
extern int DAY;
extern int HOUR;
extern int SECOND;
extern QString id;

newWight::newWight(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::newWight)
{
    ui->setupUi(this);
}

newWight::~newWight()
{
    delete ui;
}

void newWight::paintEvent(QPaintEvent *)//动画实现
{
    QPixmap pixmap_map(":/new/prefix1/image/map1111.png");
    QPixmap pixmap = pixmap_map.scaled(791, 721);
    QPainter llpainter(&pixmap);
    llpainter.setPen(QPen(Qt::red, 3, Qt::SolidLine));

    int i = 0, j = 0, flag = 0;
    int x_1 = 0, y_1 = 0, x_2 = 0, y_2 = 0;
    float travel_x, travel_y;

    while( i < passengerNumber )//找到该乘客
    {
        if( passenger[i].Number == id )
        {
            j = i;
            flag = 1;
        }
        i++;
    }


    if((DAY == passenger[j].startTimeDay && HOUR >= passenger[j].startTimeHour) || (DAY > passenger[j].startTimeDay))
    {
        x_1 = get_X(passenger[j].Route[passenger[j].visitedPlace]);
        y_1 = get_Y(passenger[j].Route[passenger[j].visitedPlace]);
        x_2 = get_X(passenger[j].Route[passenger[j].visitedPlace + 1]);
        y_2 = get_Y(passenger[j].Route[passenger[j].visitedPlace + 1]);
    }

    if( ((DAY == passenger[j].startTimeDay && HOUR >= passenger[j].startTimeHour) || (DAY > passenger[j].startTimeDay)) && passenger[j].visitedPlace != passenger[j].cityNumber && flag == 1)
        llpainter.drawLine(x_1, y_1, x_2, y_2);//画线
    llpainter.end();
    QPainter painter(this);
    painter.drawPixmap(0, 0, pixmap);

    //交通工具
    QPixmap pixmap_plane(":/new/prefix1/image/plane.gif");
    QPixmap pixmap_train(":/new/prefix1/image/train.gif");
    QPixmap pixmap_bus(":/new/prefix1/image/bus.gif");
    QPixmap pixmap_position(":/new/prefix1/image/position.png");
    if( passenger[j].typeTravel[passenger[j].visitedPlace] == 0 && passenger[j].flag == 1 )
    {
        pixmap = pixmap_plane.scaled(20, 20);
    }
    else if( passenger[j].typeTravel[passenger[j].visitedPlace] == 1 && passenger[j].flag == 1)
    {
        pixmap = pixmap_train.scaled(20, 20);
    }
    else if( passenger[j].typeTravel[passenger[j].visitedPlace] == 2 && passenger[j].flag == 1 )
        pixmap = pixmap_bus.scaled(20, 20);
    else
        pixmap = pixmap_position.scaled(20, 20);

    if( passenger[j].flag == 0 )
    {
        travel_x = x_1;
        travel_y = y_1;
    }
    else
    {
        float total_time = timetable[transform(passenger[j].travelNum[passenger[j].visitedPlace])].finishTime - timetable[transform(passenger[j].travelNum[passenger[j].visitedPlace])].startTime;
        float spend_time = HOUR - timetable[transform(passenger[j].travelNum[passenger[j].visitedPlace])].startTime;
        float rate = spend_time / total_time + SECOND / (20 * total_time) ;
        int total_x = x_2 - x_1;
        int total_y = y_2 - y_1;
        travel_x = x_1 + total_x * rate ;
        travel_y = y_1 + total_y * rate;
    }

    if( x_1 != 0 && y_1 != 0 && flag == 1)
        painter.drawPixmap(travel_x - 10, travel_y - 10, pixmap);

}

int get_X(QString city)     //各地的x坐标
{
    if( city == "湖北" )
        return 550;
    else if( city == "广东" )
        return 550;
    else if( city == "福建" )
        return 620;
    else if( city == "四川" )
        return 370;
    else if( city == "青海" )
        return 270;
    else if( city == "西藏" )
        return 170;
    else if( city == "河南" )
        return 550;
    else if( city == "江苏" )
        return 640;
    else if( city == "山西" )
        return 520;
    else if( city == "黑龙江")
        return 700;
    else if( city == "北京" )
        return 580;
    else if( city == "河北" )
        return 570;
    else
        return -1;
}

int get_Y(QString city)     //各地的y坐标
{
    if( city == "湖北" )
        return 450;
    else if( city == "广东" )
        return 570;
    else if( city == "福建" )
        return 530;
    else if( city == "四川" )
        return 450;
    else if( city == "青海" )
        return 320;
    else if( city == "西藏" )
        return 390;
    else if( city == "河南" )
        return 370;
    else if( city == "江苏" )
        return 390;
    else if( city == "山西" )
        return 310;
    else if( city == "黑龙江")
        return 100;
    else if( city == "北京" )
        return 270;
    else if( city == "河北" )
        return 290;
    else
        return -1;
}
