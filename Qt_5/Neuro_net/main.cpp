#include "mainwindow.h"
#include "PaintScene.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainwindow w;
    //w.show();
    //PaintScene w;
    w.show();
    return a.exec();
}
