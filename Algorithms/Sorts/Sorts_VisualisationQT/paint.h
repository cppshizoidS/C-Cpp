#ifndef PAINT_H
#define PAINT_H

#include <QObject>
#include <QtWidgets>
#include <QPainter>
#include <QMediaPlayer>
#include <list>
#include <vector>

const QColor DEFAULT_LINE_COLOR = Qt::lightGray;
const QColor DEFAULT_BACKGROUND_COLOR = Qt::black;
const int MAX_PEN_WIDTH = 100;

class Paint : public QWidget {
    Q_OBJECT
public:
    explicit Paint(QWidget *parent = nullptr);
    void setAnimate(bool anim) { animate = anim;}
    void setLineColor(QColor color) { lineColor = color;}
    void setPaintData(int*, int*, int, int);
    void setPaintType(QString option) {paintType = option;}
    void setPenWidth(int width) { penWidth = width > MAX_PEN_WIDTH ? MAX_PEN_WIDTH : width;}
    void setSpacing(int space) { spacing = space;}
    QStringList getPaintTypes() {return paintTypes;}
    void resetLineColor() {lineColor = DEFAULT_LINE_COLOR;}

protected:
    void paintEvent(QPaintEvent *event) override; // this method is called when `update()` or `repaint()` is called

private:
    QPalette palette;
    QPen pen;
    QString paintType;
    QStringList paintTypes = {"Bar", "Star"};
    QColor lineColor {DEFAULT_LINE_COLOR};
    QColor backgroundColor {DEFAULT_BACKGROUND_COLOR};
    std::vector<QColor> colors = {Qt::red, Qt::green};
    QMediaPlayer* sortingsound;

    bool animate;
    int penWidth;
    int spacing;

    int* numbers;
    int* colorIndices;
    int size;
    int sizeColorIndices;

    void setPen(const QPen &pen){this->pen = pen;}
};

#endif // PAINT_H
