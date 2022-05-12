#ifndef SEABATTLE_H
#define SEABATTLE_H

#include <QMainWindow>
#include <QSystemTrayIcon>

#include <QAbstractSocket>
#include <stdio.h>

namespace Ui {
    class SeaBattle;
}

class QMenu;
class QLabel;
class QTimer;
class Navy;
class QModelIndex;
class Solver;
class QTcpSocket;
class QAction;

class SeaBattle : public QMainWindow
{
    Q_OBJECT

public:
    explicit SeaBattle(QWidget *parent = 0);
    ~SeaBattle();

private:

    enum cState {ANY, START, CONNECTED, GAME, LOCAL};

    Ui::SeaBattle *ui;

    QSystemTrayIcon * tray;
    QMenu * menu;

    QAction * act_users;

    QLabel * lblStatus;
    QLabel * lblCurTime;
    QLabel * lblGameTime;
    QLabel * lblUser;
    QLabel * lblEnemy;
    QLabel * lblState;

    QTimer * timer;

    Navy * my;
    Navy * enemy;

    Solver * solver;

    bool curState;
    bool use_sound;
    QTcpSocket * socket;

    bool game;
    QString m_data;
    QStringList m_users;

    QString curPlayer;
    QString player;

    cState state;

    int m_x, m_y;
    quint32 m_blockSize;

    FILE * fp;
    QString apath;

    void loadSettings();
    void saveSettings();

    void switch_cursor(bool my = true);
    void response(const QString &);

    bool writeMessage(const QString & mes);
    void changeState(cState);

    void writeTraining();

protected:
    void closeEvent(QCloseEvent *);

private slots:

    void tray_exit();
    void tray_show();
    void tray_about();

    void tray_activated(QSystemTrayIcon::ActivationReason);

    void timeout();
    void connectToServer();

    void enemy_clicked(int ,int );
    void my_clicked(int, int );

    void readyRead();

    void listUsers();
    void sendMes();

    void stateChanged(QAbstractSocket::SocketState);
    void show_chat();
    void show_ships();

    void SetScene();
    void start_game();

};

#endif // SEABATTLE_H
