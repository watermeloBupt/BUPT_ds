#ifndef INQUIRE_H
#define INQUIRE_H

#include <QDialog>

namespace Ui {
class Inquire;
}

class Inquire : public QDialog
{
    Q_OBJECT

public:
    explicit Inquire(QWidget *parent = nullptr);
    ~Inquire();
    void search();                      //对话框点击确定根据id查询乘客状态函数
    void no_search();                   //对话框点击取消

private:
    Ui::Inquire *ui;
};

#endif // INQUIRE_H
