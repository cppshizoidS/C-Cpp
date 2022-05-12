#ifndef CONNECTTOSERVER_H
#define CONNECTTOSERVER_H

#include <QDialog>

namespace Ui {
    class ConnectToServer;
}

class AutoLocation;

struct SeaServer {
    QString ip;               //адрес сервера
    QString login;            //логин
    AutoLocation * location;  //тактика размещения кораблей для игры с компьютером
    QString sloc;             //название последней тактики
    int port;                 //порт для соединения с сервером

};

class AutoLocation;

class ConnectToServer : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectToServer(QWidget *parent = 0);
    ~ConnectToServer();

    const struct SeaServer * getServ();

private:
    Ui::ConnectToServer *ui;
    void loadSettings();

    struct SeaServer serv;
    AutoLocation * getAutoLocation(int ind);
private slots:

    void saveSettings(const struct SeaServer &);
    void ok();
    void cancel();

    void computer(bool);
    void server(bool);

    void check(const QString &);

};

#endif // CONNECTTOSERVER_H
