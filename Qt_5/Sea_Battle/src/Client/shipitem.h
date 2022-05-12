#ifndef SHIPITEM_H
#define SHIPITEM_H

#include <QGraphicsRectItem>
#include <QPainter>
#include "ship.h"
#include <QGraphicsScene>


class ShipItem : public QGraphicsItem
{

public:
    explicit ShipItem(QGraphicsScene *scene, int level, Ship &ship);
    QRectF boundingRect() const
    {
        return QRectF(m_x,m_y,m_w,m_h);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
    {
        painter->save();
        if(!rotate)
            painter->drawPixmap(m_x,m_y,m_w,m_h, QPixmap(QObject::tr(":images/ship%1.png").arg(m_level)));
        else
            painter->drawPixmap(m_x,m_y,m_w,m_h, QPixmap(QObject::tr(":images/ship%1v.png").arg(m_level)));
        painter->restore();
    }

    void setRect(double _x, double _y, double _w, double _h, int _rotate = 0)
    {
        m_x = _x;
        m_y = _y;
        m_w = _w;
        m_h = _h;
        rotate = _rotate;
    }

     QVariant itemChange(GraphicsItemChange change, const QVariant &value);
     Ship & m_ship;
     int rotate;

     void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
     {
         rotate =!rotate;

         //выправляем координаты палуб.
         if(m_ship.cells.count() > 1)
         {
             int x = m_ship.cells[0].x();
             int y = m_ship.cells[0].y();

             if(rotate) //вертикальный корабль
             {
                 for(int j=1; j< m_ship.cells.count(); ++j)
                 {
                     m_ship.cells[j].setX(x);
                     m_ship.cells[j].setY(++y);
                 }

             }
             else
                 for(int j=1; j< m_ship.cells.count(); ++j)
                 {
                     m_ship.cells[j].setX(x++);
                     m_ship.cells[j].setY(y);
                 }
             std::swap(m_w,m_h);
         }

         scene()->update();

     }

private:

    double m_x;
    double m_y;

    double m_w;
    double m_h;

    double height;
    double width;

    double step;

    int m_level;

};

#endif // SHIPITEM_H
