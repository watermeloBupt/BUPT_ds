#ifndef CHANGE_ROUTE_H
#define CHANGE_ROUTE_H

#include <QDialog>

namespace Ui {
class change_route;
}

class change_route : public QDialog
{
    Q_OBJECT

public:
    explicit change_route(QWidget *parent = nullptr);
    ~change_route();
    void change_ok();
    void change_cancel();

private:
    Ui::change_route *ui;
};

#endif // CHANGE_ROUTE_H
