#include "cell.h"
#include "ship.h"

#include <QPainter>

Cell::Cell()
{

}

Cell::Cell(int x, int y): m_x(x), m_y(y), alive(true)
{
    rot = qrand() % 360;
}

bool Cell::isAlive() const
{
    return alive;
}

void Cell::setKilled()
{
    alive = false;
}

int Cell::x()
{
    return m_x;
}

int Cell::y()
{
    return m_y;
}

void Cell::paint(QPainter * painter, const Ship *ship, double s)
{

    return ;

    const double of = 5;
    painter->save();

    double s_x = x() * s + 0.5*s;
    double s_y = y() * s + 0.5*s;

    QPointF p(s_x, s_y);

    painter->setBrush(QColor("#721b1b"));
    painter->drawEllipse(p, 10,10);

    painter->setBrush(QBrush(QColor("#caa6e3"), Qt::Dense5Pattern));
    painter->drawEllipse(p, 8,8);

    painter->translate(s_x, s_y);
    painter->rotate(rot);
    painter->drawRect(-3, -3, 25,6);

    painter->restore();

}
