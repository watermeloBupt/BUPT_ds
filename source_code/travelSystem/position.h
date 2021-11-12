#ifndef POSITION_H
#define POSITION_H

#include <QDialog>
#include <QLabel>

namespace Ui {
class Position;
}

class Position : public QDialog
{
    Q_OBJECT

public:
    explicit Position(QWidget *parent = nullptr);
    ~Position();
    void search(int day, int hour);     //同步时间函数
    Ui::Position *ui;

private:
};


#endif // POSITION_H
