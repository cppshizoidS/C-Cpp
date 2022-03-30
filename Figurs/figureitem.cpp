#include "figureitem.h"

FigureItem::FigureItem(QPoint position, QColor color)
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    this->color = color;
    this->position = position;
    setPos(position);
}

void FigureItem::updateInfo()
{
   QString InfoCoor = QString("\nКоординаты: %1, %2").arg(position.x()).arg((position.y()));
   QString InfoFigure = QString("Площадь: %1\nПериметр: %2").arg(figure->calcArea()).arg(figure->calcPerimeter());
   setToolTip(InfoFigure + InfoCoor);
}

void FigureItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(color);
    painter->drawPath(path);
    if(isSelected() == true)
    {
        painter->restore();
        painter->setPen(QPen(Qt::DashLine));
        painter->drawRect(bounding);
    }
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF FigureItem::boundingRect() const
{
    return QRectF(bounding);
}

FigureItem::~FigureItem(){}

void FigureItem::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    point = mouseEvent->pos();
    if (mouseEvent->button() == Qt::RightButton)
    {
        setSelected(!isSelected());
    }
    else
    {
        this->setCursor(QCursor(Qt::ClosedHandCursor));
    }
}

void FigureItem::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    setPos(QPoint(mouseEvent->scenePos().x()- point.x(),mouseEvent->scenePos().y()-point.y()));
    position = mouseEvent->scenePos();
}

void FigureItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    this->setCursor(QCursor(Qt::ArrowCursor));
    updateInfo();
    Q_UNUSED(mouseEvent);
}

CircleItem::CircleItem(Circle circle, QPoint position, QColor color) : FigureItem(position, color)
{
    figure = new Circle(circle);
    setPaint();
    updateInfo();
}

void CircleItem::setPaint()
{
    path = QPainterPath();
    int r = ((Circle*)figure)->getR();
    path.addEllipse(0, 0, r, r);
    bounding = QRectF(QGraphicsEllipseItem(0, 0, r, r).boundingRect());
}

CircleItem::~CircleItem()
{
    delete (Circle*)figure;
}

TriangleItem::TriangleItem(Triangle triangle, QPoint position, QColor color) : FigureItem(position, color)
{
    figure = new Triangle(triangle);
    setPaint();
    updateInfo();
}

void TriangleItem::setPaint()
{
    path = QPainterPath();
    int x1, y1, x2, y2, x3, y3;
    ((Triangle*)figure)->getCoor(x1, y1, x2, y2, x3, y3);
    path.addPolygon(QPolygon(QList<QPoint>{QPoint(x1, y1), QPoint(x2, y2), QPoint(x3, y3)}));
    bounding = QRectF(QPolygon(QList<QPoint>{QPoint(x1, y1), QPoint(x2, y2), QPoint(x3, y3)}).boundingRect());
}

TriangleItem::~TriangleItem()
{
    delete (Triangle*)figure;
}

RingItem::RingItem(Ring ring, QPoint position, QColor color) : FigureItem(position, color)
{
    figure = new Ring(ring);
    setPaint();
    updateInfo();
}

void RingItem::setPaint()
{
    path = QPainterPath();
    int r = ((Ring*)figure)->getr();
    int R = ((Ring*)figure)->getR();
    path.addEllipse(0, 0, R, R);
    path.addEllipse((R - r)/2, (R - r)/2, r, r);
    bounding = QRectF(QGraphicsEllipseItem(0, 0, R, R).boundingRect());
}

RingItem::~RingItem()
{
    delete (Ring*)figure;
}
