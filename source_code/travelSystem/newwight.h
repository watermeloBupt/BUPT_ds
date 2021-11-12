#ifndef NEWWIGHT_H
#define NEWWIGHT_H

#include <QWidget>

namespace Ui {
class newWight;
}

class newWight : public QWidget
{
    Q_OBJECT

public:
    explicit newWight(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent * event);           //乘客查询动画实现
    ~newWight();

private:
    Ui::newWight *ui;
};

int get_X(QString city);        //各地的x坐标
int get_Y(QString city);        //各地的y坐标

#endif // NEWWIGHT_H
