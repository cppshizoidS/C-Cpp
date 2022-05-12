#ifndef SHIP_H
#define SHIP_H

#include <QVector>
#include <QPainter>
#include <QImage>

#include "cell.h"


class Ship
{

public:
    Ship ();

    void addCell(int x, int y);
    int level() const;

    bool isAlive() const;
    void paint(QPainter * painter, double s);

    QVector<Cell> cells;
    bool vert;

    void setTime(int);

private:

    int t;

};


class Singleton
{
    private:
        Singleton();
        static const int size = 4;
        QImage imgs[size];
    public:
        static QImage & getImageForShip(int level);
};

#endif // SHIP_H
