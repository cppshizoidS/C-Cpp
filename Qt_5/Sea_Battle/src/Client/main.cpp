#include <QApplication>
#include <QTextCodec>

#include "seabattle.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);


    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    SeaBattle w;
    w.show();

    return a.exec();
}
