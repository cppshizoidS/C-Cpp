#include "ship.h"
#include <math.h>
#include <stdio.h>


Singleton::Singleton()
{
    for(int i=0; i< size; ++i)
        imgs[i] = QImage(QObject::tr(":/images/ship%1.png").arg(i+1));

}

QImage & Singleton::getImageForShip(int level)
{
    static Singleton inst;
    return inst.imgs[level-1];
}

Ship::Ship ()
{
    cells.clear();
}

void Ship::setTime(int r)
{
    t = r;
}

void Ship::addCell(int x, int y)
{
    cells.push_back(Cell(x,y));
}

int Ship::level() const
{
    return cells.size();
}

bool Ship::isAlive() const
{
    for(int i =0; i< cells.size(); ++i)
        if(cells[i].isAlive())
            return true;
    return false;
}

void Ship::paint(QPainter * painter, double s)
{
    double s_x = s * cells[0].x();
    double s_y = s * cells[0].y();

    if(level() >= 1)
    {
        painter->save();
        painter->translate(s_x, s_y);
        if(vert)
        {
            painter->rotate(90);
            painter->drawImage(0, -s, Singleton::getImageForShip(level()));

        }
        else
        {
            painter->drawImage(0, 0, Singleton::getImageForShip(level()));
        }
        painter->restore();
    }
}


