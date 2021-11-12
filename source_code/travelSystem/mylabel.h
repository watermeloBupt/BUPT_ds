#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>

class mylabel : public QLabel
{
public:
    mylabel();
    void paintEvent(QPaintEvent * event);
};

#endif // MYLABEL_H
