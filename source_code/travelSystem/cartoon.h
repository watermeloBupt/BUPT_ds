#ifndef CARTOON_H
#define CARTOON_H

#include <QWidget>

namespace Ui {
class cartoon;
}

class cartoon : public QWidget
{
    Q_OBJECT

public:
    explicit cartoon(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent * event);           //动画实现
    ~cartoon();

private:
    Ui::cartoon *ui;
};

#endif // CARTOON_H
