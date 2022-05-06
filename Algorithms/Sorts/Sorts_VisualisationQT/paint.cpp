#include "paint.h"

Paint::Paint(QWidget *parent) : QWidget(parent) {
    palette.setColor(QPalette::Background, backgroundColor);
    setPalette(palette);
    setAutoFillBackground(true);
    animate = false;
    setPen(QPen(lineColor, penWidth, Qt::PenStyle::SolidLine, Qt::PenCapStyle::SquareCap, Qt::PenJoinStyle::BevelJoin));
    sortingsound = new QMediaPlayer;
    sortingsound->setMedia(QUrl("qrc:/sounds/sorting.mp3")); // load the binary data of sortingsound from Resources folder
}

void Paint::setPaintData(int* n, int* idx, int size, int sizeIndices) {
    this->numbers = n;
    this->colorIndices = idx;
    this->size = size;
    this->sizeColorIndices = sizeIndices;
}

void Paint::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    int colorIdx{0};
    int space = (this->width()-(penWidth*(size-1)))/2;
    for (int i{0}; i < size; ++i) {
        pen.setColor(lineColor);
        pen.setWidth(penWidth); // set the appropriate penWidth according to the array size
        painter.setPen(pen);

        bool color{false};
        for (int j{0}; j < sizeColorIndices; ++j) { // color the element if i is included in color indices array
            if (colorIndices[j] == i) {color = true;}
        }

        if (animate && color) { // set color for the bar of specific index
            pen.setColor(colors[colorIdx]);
            painter.setPen(pen);
            ++colorIdx; // increment color index so that next colored bar can be colored differently
        }

        if (paintType == "Bar") {
            painter.drawLine((i)*penWidth+space, this->height(), // use `space` to center elements
                             (i)*penWidth+space, this->height() - numbers[i]);
        }
        else if (paintType == "Star") {
            painter.drawPoint((i)*penWidth+space, this->height() - numbers[i]);
        }
        else { // use `Bar` as default
            painter.drawLine((i)*penWidth+space, this->height()-1,
                             (i)*penWidth+space, this->height() - numbers[i]-1);
        }


        if (animate) { // avoid playing sound when `shuffle` is pressed
            if (sortingsound->state() == QMediaPlayer::PlayingState) {
                sortingsound->setPosition(0); // set postion to 0 if playing
            } else if (sortingsound->state() == QMediaPlayer::StoppedState) {
                sortingsound->play(); // simply play the sound if no sound
            }
        }
    }
}
