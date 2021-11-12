#include "cartoon.h"
#include "ui_cartoon.h"
#include <QPainter>
#include <QString>
#include "passenger.h"
#include "arrange.h"
#include "timetable.h"
#include "newwight.h"
#include "mainwindow.h"

extern struct Passenger passenger[maxPassenger];	//乘客信息的结构体数组
extern struct Timetable timetable[maxTimetable];	//时刻表信息的结构体数组
extern int passengerNumber;
extern int DAY;
extern int HOUR;
extern int SECOND;

cartoon::cartoon(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cartoon)
{
    ui->setupUi(this);
}

cartoon::~cartoon()
{
    delete ui;
}

void cartoon::paintEvent(QPaintEvent *)     //动画的实现
{
    int j = 0;
    int x_1 = 0, y_1 = 0, x_2 = 0, y_2 = 0;
    float travel_x, travel_y;
    QPixmap pixmap_map(":/new/prefix1/image/map1111.png");
    QPixmap pixmap = pixmap_map.scaled(791, 721);   //背景地图
    QPainter llpainter(&pixmap);
    llpainter.setPen(QPen(Qt::red, 3, Qt::SolidLine));
    while( j < passengerNumber )                    //添加红线
    {
        if((DAY == passenger[j].startTimeDay && HOUR >= passenger[j].startTimeHour) || (DAY > passenger[j].startTimeDay))
        {
            x_1 = get_X(passenger[j].Route[passenger[j].visitedPlace]);
            y_1 = get_Y(passenger[j].Route[passenger[j].visitedPlace]);
            x_2 = get_X(passenger[j].Route[passenger[j].visitedPlace + 1]);
            y_2 = get_Y(passenger[j].Route[passenger[j].visitedPlace + 1]);
        }
        if( ((DAY == passenger[j].startTimeDay && HOUR >= passenger[j].startTimeHour) || (DAY > passenger[j].startTimeDay)) && passenger[j].visitedPlace != passenger[j].cityNumber )
            llpainter.drawLine(x_1, y_1, x_2, y_2);
        QPainter painter(this);

        j++;
    }
    llpainter.end();
    QPainter painter(this);
    painter.drawPixmap(0, 0, pixmap);

    j = 0;
    QPixmap pixmap_type;
    while( j < passengerNumber )                //添加交通工具
    {

        if((DAY == passenger[j].startTimeDay && HOUR >= passenger[j].startTimeHour) || (DAY > passenger[j].startTimeDay))
        {
            x_1 = get_X(passenger[j].Route[passenger[j].visitedPlace]);
            y_1 = get_Y(passenger[j].Route[passenger[j].visitedPlace]);
            x_2 = get_X(passenger[j].Route[passenger[j].visitedPlace + 1]);
            y_2 = get_Y(passenger[j].Route[passenger[j].visitedPlace + 1]);
        }
        else
        {
            x_1 = 0;
            y_1 = 0;
            x_2 = 0;
            y_2 = 0;
        }
        QPixmap pixmap_plane(":/new/prefix1/image/plane.gif");
        QPixmap pixmap_train(":/new/prefix1/image/train.gif");
        QPixmap pixmap_bus(":/new/prefix1/image/bus.gif");
        QPixmap pixmap_position(":/new/prefix1/image/position.png");
        if( passenger[j].typeTravel[passenger[j].visitedPlace] == 0 && passenger[j].flag == 1 )
        {
            pixmap_type = pixmap_plane.scaled(20, 20);
        }
        else if( passenger[j].typeTravel[passenger[j].visitedPlace] == 1 && passenger[j].flag == 1)
        {
            pixmap_type = pixmap_train.scaled(20, 20);
        }
        else if( passenger[j].typeTravel[passenger[j].visitedPlace] == 2 && passenger[j].flag == 1 )
            pixmap_type = pixmap_bus.scaled(20, 20);
        else
            pixmap_type = pixmap_position.scaled(20, 20);

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

        if( x_1 != 0 && y_1 != 0 && passenger[j].visitedPlace != passenger[j].cityNumber)
            painter.drawPixmap(travel_x - 10, travel_y - 10, pixmap_type);

        j++;
    }
}
