#ifndef ADDTRAVEL_H
#define ADDTRAVEL_H
#include <QString>
#include <QDialog>
#include <QDebug>

namespace Ui {
class AddTravel;
}

class AddTravel : public QDialog
{
    Q_OBJECT

public:
    explicit AddTravel(QWidget *parent = nullptr);
    ~AddTravel();
    void getinput(int day,int hour);                //对话框点击确定添加乘客函数
    void no_getinput();                             //对话框点击取消不添加乘客函数
    Ui::AddTravel *ui;
private:

};

#endif // ADDTRAVEL_H
