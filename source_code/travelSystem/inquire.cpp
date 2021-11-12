#include "inquire.h"
#include "ui_inquire.h"
#include <QString>
#include "mainwindow.h"

extern QString id;

Inquire::Inquire(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Inquire)
{
    ui->setupUi(this);
}

Inquire::~Inquire()
{
    delete ui;
}

void Inquire::search()
{
    id = ui->lineEdit->text();
    ui->lineEdit->setText("");
}

void Inquire::no_search()
{
    ui->lineEdit->setText("");
}
