#ifndef INTERFACE_H
#define INTERFACE_H

#include "colonie.h"

using namespace std;

class Colony;


class SetThings : public QObject
{
    Q_OBJECT
public:
    Colony * c;
    QWidget * window;
public slots:
    void setting();
    void pause();
    void stop();
    void quit_window();
};



bool ask_main_parameters(int& K,int& n,int& DX,int& DY);



#endif // INTERFACE_H
