#include "shipitem.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include <stdio.h>

ShipItem::ShipItem(QGraphicsScene *scene, int level, Ship & ship) :
    QGraphicsItem(NULL), m_ship(ship)
{
    scene->addItem(this);
    width = scene->width();
    height = scene->height();

    m_level = level;


    step = width/20;

    setFlag(ItemSendsGeometryChanges, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);


}

QVariant ShipItem::itemChange(GraphicsItemChange change, const QVariant &value)
{

    if (change == ItemPositionChange)
    {
        // value это новое положение.
        QPointF newPos = value.toPointF();
        QRectF rect = scene()->sceneRect();

        rect.setX(24);
        rect.setY(28);
        rect.setWidth(245);
        rect.setHeight(245);

        if (!rect.contains(newPos)) {
            // Сохраняем элемент внутри прямоугольника сцены.
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            return newPos;
        }

    }
    return QGraphicsItem::itemChange(change, value);
}
