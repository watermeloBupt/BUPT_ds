#include "position.h"
#include "ui_position.h"
#include <QString>

Position::Position(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Position)
{
    ui->setupUi(this);
}

Position::~Position()
{
    delete ui;
}

void Position::search(int day, int hour)
{
    QString a, b;
    a = QString::number(day);
    b = QString::number(hour);
    ui->time->setText(b);
    ui->day->setText(a);
    update();
}
