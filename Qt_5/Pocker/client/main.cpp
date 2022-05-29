#include <QApplication>
#include <QObject>
#include <QFile>
#include <QMessageBox>

#include "countdowntimer.h"
#include "client.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    Client client;

    if(!client.hasValidHost()){
        QMessageBox::information(0, "Failed to Start", "Make sure host.config exists in the program directory and is in the format of x.x.x.x:port");
        return 0;
    }

    if(!client.connectTcp()){
        QMessageBox::information(0, "Cannot connect to Server", "Unable to establish a connection with the server in host.config");
        return 0;
    }

    client.startClient();
    return a.exec();
}
