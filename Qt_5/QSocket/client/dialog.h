#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QHostAddress>
#include "/home/vlad/programs/Lab2_server/myclient.h"
#include <QMessageBox>
class MyClient;

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void onSokConnected();
    void onSokDisconnected();
    void onSokReadyRead();
    void onSokDisplayError(QAbstractSocket::SocketError socketError);

    void on_pbConnect_clicked();
    void on_pbDisconnect_clicked();
    void on_cbToAll_clicked();
    void on_pbSend_clicked();

private:
    Ui::Dialog *ui;
    QTcpSocket *_sok;
    quint16 _blockSize;
    QString _name;
    void AddToLog(QString text, QColor color = Qt::black);

};

#endif // DIALOG_H
