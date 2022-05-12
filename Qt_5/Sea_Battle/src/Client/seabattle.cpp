#include "seabattle.h"


#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QSettings>
#include <QDesktopWidget>
#include <QToolButton>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <QTcpSocket>
#include <QDataStream>
#include <QSound>



#include <stdio.h>

SeaBattle::SeaBattle(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SeaBattle)
{
    ui->setupUi(this);

    game = false;
    use_sound = true;

    tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon(":/images/navy.png"));

    QAction * act_restore = new QAction(QIcon(":/images/anchor.png"),tr("Восстановить"), this);
    act_restore->setIconVisibleInMenu(true);

    QAction * act_exit = new QAction(QIcon(":/images/close.png"),tr("Выход"), this);
    act_exit->setIconVisibleInMenu(true);
    connect(act_exit, SIGNAL(triggered()), this, SLOT(tray_exit()));

    QAction * act_about = new QAction(QIcon(":/images/navy.png"),tr("О программе"), this);
    act_about->setIconVisibleInMenu(true);
    connect(act_about, SIGNAL(triggered()), this, SLOT(tray_about()));


    connect(ui->mact_start, SIGNAL(triggered()), this, SLOT(start_game()));

    menu = new QMenu(this);

    menu->addAction(act_restore);
    menu->addAction(act_about);

    menu->addSeparator();
    menu->addAction(act_exit);

    tray->setContextMenu(menu);
    tray->setToolTip(tr("Игра Морской Бой"));

    QAction *act_connect = new QAction(QIcon(":/images/connect.png"), tr("Соединиться с игровым сервером"), this);
    act_connect->setStatusTip(tr("Соединиться с игровым сервером для начала игры в Морской Бой."));

    connect (act_connect, SIGNAL(triggered()), this, SLOT(connectToServer()));

    ui->mainToolBar->addAction(act_connect);

    act_users = new QAction(QIcon(":/images/users.png"), tr("Свободные игроки"), this);
    act_users->setStatusTip(tr("Отобразить список свободных для игры игроков"));
    connect (act_users, SIGNAL(triggered()), this, SLOT(listUsers()));

    ui->mainToolBar->addAction(act_users);

    QAction *act_chat = new QAction(QIcon(":/images/chat.png"), tr("Окно чата"), this);
    act_chat->setStatusTip(tr("Показать окно обмена сообщениями"));
    connect(act_chat, SIGNAL(triggered()), this, SLOT(show_chat()));

    ui->mainToolBar->addAction(act_chat);

    QAction *act_ship = new QAction(QIcon(":/images/navy.png"), tr("Расстановка кораблей"), this);
    act_ship->setStatusTip(tr("Расставить корабли"));
    connect(act_ship, SIGNAL(triggered()), this, SLOT(show_ships()));

    //ui->mainToolBar->addAction(act_ship);

    QAction *act_start = new QAction(QIcon(":/images/accept.png"), tr("Начать игру"), this);
    act_start->setStatusTip(tr("Начать игру"));
    connect(act_start, SIGNAL(triggered()), this, SLOT(start_game()));

    ui->mainToolBar->addAction(act_start);


    lblStatus = new QLabel(tr("Нет соединения с сервером"), this);
    lblCurTime =  new QLabel(tr("--:--:--"), this);
    lblGameTime = new QLabel(tr(""), this);
    lblUser = new QLabel("", this);
    lblEnemy = new QLabel("", this);
    lblState = new QLabel(tr("Статус: %1").arg(START), this);

    statusBar()->addWidget(lblStatus,1);
    statusBar()->addWidget(lblState);
    statusBar()->addWidget(lblUser);
    statusBar()->addWidget(new QLabel(tr("против"), this));
    statusBar()->addWidget(lblEnemy);

    statusBar()->addWidget(lblGameTime);
    statusBar()->addWidget(lblCurTime);

    timer = new QTimer(this);
    timer->setInterval(1000);

    timeout();

    connect(ui->mact_exit, SIGNAL(triggered()), this, SLOT(tray_exit()));
    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(tray_activated(QSystemTrayIcon::ActivationReason)));

    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->start();

    loadSettings();

    QSettings settings;
    int bgcode = settings.value("bgcode", 0).toInt();

    my = ui->wdg;

    NavyPainter * navy = dynamic_cast<NavyPainter *> (my);
    if(navy)
        navy->changed_bg(bgcode);

    tray->show();


    my->m_location = new RandomLocation();

    my->locate();

    enemy = ui->wdg->enemy;
    enemy->m_location = new RandMaxLocation();

    enemy->locate();

    enemy->setEnemy(true);

    connect(ui->wdg, SIGNAL(enemy_clicked(int,int)), this, SLOT(enemy_clicked(int,int)));
    connect(ui->wdg, SIGNAL(my_clicked(int,int)), this, SLOT(my_clicked(int,int)));

    switch_cursor();

    solver = new Solver();
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));

    m_data = "";

    connect(ui->txtMes, SIGNAL(editingFinished()), this, SLOT(sendMes()));
    connect(ui->btnSend, SIGNAL(clicked()), this, SLOT(sendMes()));

    connect(ui->mact_about, SIGNAL(triggered()), this, SLOT(tray_about()));
    connect(ui->mact_listusers, SIGNAL(triggered()), this, SLOT(listUsers()));

    curState = START;

    ui->mainToolBar->addAction(act_exit);

    changeState(LOCAL);

    connect(ui->actScene1, SIGNAL(triggered()), this, SLOT(SetScene()));
    connect(ui->actScene2, SIGNAL(triggered()), this, SLOT(SetScene()));
    connect(ui->actScene3, SIGNAL(triggered()), this, SLOT(SetScene()));
    connect(act_restore, SIGNAL(triggered()), this, SLOT(tray_show()));

}

void SeaBattle::start_game()
{
    if(game)
    {
        QMessageBox::warning(this,tr("Ошибка"), tr("Нельзя расставлять корабли во время игры."));
        return;
    }

    //if(state == LOCAL)
    {
        my->locate();
        show_ships();

        enemy->locate();
        enemy->setEnemy(true);
        solver->clear();

        changeState(state);
        my->switch_cursor(state == LOCAL);

        solver->clear();

    }
}

void SeaBattle::SetScene()
{
    const QObject * obj = sender();
    const QAction * act = dynamic_cast<const QAction *>(obj);
    if(act)
    {
        QString title = act->text();
        QStringList n = title.split(tr("№"));
        if(n.size()>1)
        {
            int q = n[1].toInt();
            NavyPainter * navy = dynamic_cast<NavyPainter *> (my);
            if(navy)
            {
                navy->changed_bg(q-1);                
            }
        }
    }
}

void SeaBattle::show_ships()
{
    ShipSettle * settle = new ShipSettle(this, my);
    if(settle->exec())
    {

    }
    delete settle;
}

void SeaBattle::stateChanged(QAbstractSocket::SocketState st)
{
    switch(st)
    {
    case QAbstractSocket::ConnectedState:
        changeState(CONNECTED);
        break;
    case QAbstractSocket::UnconnectedState:
        changeState(START);
        break;
    }
}

void SeaBattle::show_chat()
{
    ui->tabWidget->setCurrentIndex(1);
}

void SeaBattle::changeState(cState st)
{
    lblState->setText(tr("Статус: %1").arg(st));
    state = st;
}

bool SeaBattle::writeMessage(const QString & mes)
{

    QByteArray da;
    QDataStream ds(&da, QIODevice::WriteOnly);

    ds << quint32(0)  << mes.toUtf8();
    ds.device()->seek(0);

    ds << quint32(da.size() - sizeof(quint32));

    socket->write(da);
    socket->flush();

}

void SeaBattle::sendMes()
{
    if(!socket->isOpen())
        return;

    QString text = ui->txtMes->text().trimmed();
    if(text.isEmpty())
        return;

    text.push_front(QString("SEND ") + curPlayer + " ");
    writeMessage(text);

    ui->txtMes->clear();
    ui->txtMes->setFocus();

    int d = text.length();
    ui->txt->appendPlainText(">>" + text.right(d- 5).left(d - 5));
}

void SeaBattle::switch_cursor(bool my)
{
    /*if(my)
        ui->tblEnemy->setCursor(QCursor(QPixmap(":/images/cursor.png")));
    else
        ui->tblEnemy->setCursor(QCursor(Qt::ArrowCursor));
    */
    curState = my;
}

void SeaBattle::response(const QString & mes)
{
    if(mes.startsWith(tr("FIRE")))
    {   // по нам стреляли

        QStringList coords = mes.split(" ");
        int x = coords[1].toInt();
        int y = coords[2].toInt();

        Navy::lState s = my->getState(x,y);
        if(s == Navy::EMPTY || s == Navy::KILLED || s == Navy::SHOT)
        {
            writeMessage(tr("SEND %1 resu %2\n").arg(curPlayer).arg(QString::number(Navy::S_ERROR)).toUtf8());
            return;
        }

        ui->wdg->shoot_my(x+1,y+1);

        if(s == Navy::SHIP) s = Navy::SHOT;
        else if(s == Navy::FREE) s = Navy::EMPTY;

        s = my->getState(x,y);
        writeMessage(tr("SEND %1 resu %2\n").arg(curPlayer).arg(QString::number(s)).toUtf8());

        my->setState(x, y, s);

        ui->wdg->update();

        if(!my->isAlive())
        {
            my->locate();
            enemy->clear();
            act_users->setEnabled(true);
        }

        if(s == Navy::EMPTY || s == Navy::FREE)
        {
            my->switch_cursor(true);
            return;
        }

        my->switch_cursor(false);

    }
    else if(mes.startsWith(tr("resu")))
    {
        QStringList coords = mes.split(" ");
        Navy::lState s = (Navy::lState)coords[1].toInt();

        if(s == Navy::S_ERROR)
        {
            QMessageBox::warning(this, tr("Ошибка"), tr("По этому полю вы уже стреляли."));
            my->switch_cursor(true);
            return;
        }

        enemy->setState(m_x, m_y, s);        

        if(s == Navy::SHOT || s == Navy::KILLED || s == Navy::SHIP)
            my->switch_cursor(true);
        else if(s == Navy::EMPTY || s == Navy::FREE)
            my->switch_cursor(false);

        if(s == Navy::SHIP) s = Navy::SHOT;
        else if(s == Navy::FREE) s = Navy::EMPTY;

        enemy->setState(m_x, m_y, s);
        //анализируем поле

        if(s == Navy::SHOT || s == Navy::KILLED)
        {
            //крест
            enemy->setState(m_x -1, m_y - 1, Navy::EMPTY);
            enemy->setState(m_x -1, m_y + 1, Navy::EMPTY);
            enemy->setState(m_x +1, m_y - 1, Navy::EMPTY);
            enemy->setState(m_x +1, m_y + 1, Navy::EMPTY);

            //кончики
            if(s == Navy::KILLED)
            {
                int x = m_x ;
                int y = m_y ;

                enemy->setState(x,y, Navy::KILLED);

                int i = 1;
                int l,r,t,b;

                Navy::lState ss = enemy->getState(x - 1, y);
                while(x - i >= 0 && (ss == Navy::SHOT || ss == Navy::KILLED))
                {
                    enemy->setState(x-i, y, Navy::KILLED);
                    i++;
                    ss = enemy->getState(x - i, y);
                }
                l=i;

                i = 1;
                ss = enemy->getState(x + 1, y);
                while(x + i < 10 && (ss == Navy::SHOT || ss == Navy::KILLED))
                {
                    enemy->setState(x+i, y, Navy::KILLED);
                    i++;
                    ss = enemy->getState(x + i, y);

                }
                r=i;

                i = 1;
                ss = enemy->getState(x, y - 1);
                while(y -i >= 0 && (ss == Navy::SHOT || ss == Navy::KILLED))
                {

                    enemy->setState(x, y - i, Navy::KILLED);
                    i++;
                    ss = enemy->getState(x, y - i);
                }
                t=i;
                i = 1;
                ss = enemy->getState(x, y + 1);
                while(y + i >= 0 && (ss == Navy::SHOT || ss == Navy::KILLED))
                {
                    enemy->setState(x, y + i, Navy::KILLED);
                    i++;
                    ss = enemy->getState(x, y + i);

                }
                b=i;

                enemy->setState(x-l, y, Navy::EMPTY);
                enemy->setState(x+r, y, Navy::EMPTY);
                enemy->setState(x, y-t, Navy::EMPTY);
                enemy->setState(x, y+b, Navy::EMPTY);

                b=0;

                for(int i=0; i< 10; ++i)
                    for(int j=0; j< 10; ++j)
                        if(enemy->field[i][j] == Navy::KILLED)
                            b++;

                if(b == 20)
                {
                    QMessageBox::warning(this, tr("Поздравляем"), tr("Вы победили!"));
                    my->switch_cursor(false);

                    my->locate();
                    enemy->clear();
                    act_users->setEnabled(true);

                    game = false;

                    return;
                }
            }
        }

        //анализируем поле
        //solver->setState(m_x, m_y, enemy->getState(m_x, m_y));

        ui->wdg->update();

    }
    else if(mes.startsWith("LIST"))
    {
        QStringList users = mes.split("\n");

        ChooseUser * choose = new ChooseUser(this, &users);
        if(choose->exec())
        {
            curPlayer = choose->getPlayer();
            lblEnemy->setText(curPlayer);
            writeMessage(tr("WELCOME %1 ").arg(curPlayer));
        }
        delete choose;
    }
    else if(mes.startsWith("WELCOME"))
    {
        QString pl = mes.trimmed().right(mes.length() - 7).trimmed();
        int ret = QMessageBox::question(this, tr("Приглашение на игру"),
                                        tr("Игрок %1 "
                                           "предлагает вам поиграть в морской бой!").arg(pl),
                                        QMessageBox::Ok | QMessageBox::Cancel);
        if(ret == QMessageBox::Ok)
        {
            writeMessage(tr("ACCEPT %1 ").arg(pl));

            curPlayer = pl;
            lblEnemy->setText(curPlayer);
            act_users->setEnabled(false);
            changeState(GAME);

            my->switch_cursor(false);
        }
        else
        {
            writeMessage(tr("REJECT %1 ").arg(pl));
            lblEnemy->setText("");
            act_users->setEnabled(true);
            my->switch_cursor(false);
        }

    }
    else if(mes.startsWith("ACCEPT"))
    {
        QString pl = mes.right(mes.length() - 6);
        QMessageBox::information(this, tr("Успешно"), tr("Канал связи с пользователем %1 установлен успешно.").arg(pl));
        curPlayer = pl.trimmed();
        lblEnemy->setText(curPlayer);
        act_users->setEnabled(false);
        changeState(GAME);
        my->switch_cursor(true);
    }
    else if(mes.startsWith("REJECT"))
    {
        QString pl = mes.right(mes.length() - 6);
        QMessageBox::critical(this, tr("Отказ"), tr("Пользователь %1 отказался от игры с вами.").arg(pl));
        curPlayer = "";
        lblEnemy->setText(curPlayer);
        act_users->setEnabled(true);
        my->switch_cursor(false);
    }
    else
    {
        ui->txt->appendPlainText(mes);
        tray->showMessage(tr("Сообщение"), mes,  QSystemTrayIcon::Information, 2000);
    }
}

void SeaBattle::my_clicked(int x, int y)
{
    game = true;
    Navy::lState s= my->getState(x,y);
    if(s == Navy::SHIP)
    {
        my->setState(x,y, Navy::SHOT);
        solver->setState(x,y, Navy::SHOT);

        solver->setState(x-1, y-1, Navy::EMPTY);
        solver->setState(x-1, y+1, Navy::EMPTY);
        solver->setState(x+1, y-1, Navy::EMPTY);
        solver->setState(x+1, y+1, Navy::EMPTY);

        if(!my->isShipAlive(x,y))
        {
            solver->setState(x,y, Navy::KILLED);

            int i = 1;
            int l,r,t,b;

            Navy::lState ss = solver->getState(x - 1, y);
            while(x - i >= 0 && (ss == Navy::SHOT || ss == Navy::KILLED))
            {
                solver->setState(x-i, y, Navy::KILLED);
                i++;
                ss = solver->getState(x - i, y);
            }
            l=i;

            i = 1;
            ss = solver->getState(x + 1, y);
            while(x + i < 10 && (ss == Navy::SHOT || ss == Navy::KILLED))
            {
                solver->setState(x+i, y, Navy::KILLED);
                i++;
                ss = solver->getState(x + i, y);

            }
            r=i;

            i = 1;
            ss = solver->getState(x, y - 1);
            while(y -i >= 0 && (ss == Navy::SHOT || ss == Navy::KILLED))
            {

                solver->setState(x, y - i, Navy::KILLED);
                i++;
                ss = solver->getState(x, y - i);
            }
            t=i;
            i = 1;
            ss = solver->getState(x, y + 1);
            while(y + i >= 0 && (ss == Navy::SHOT || ss == Navy::KILLED))
            {
                solver->setState(x, y + i, Navy::KILLED);
                i++;
                ss = solver->getState(x, y + i);

            }
            b=i;

            solver->setState(x-l, y, Navy::EMPTY);
            solver->setState(x+r, y, Navy::EMPTY);
            solver->setState(x, y-t, Navy::EMPTY);
            solver->setState(x, y+b, Navy::EMPTY);

            b=0;

            for(int i=0; i< 10; ++i)
                for(int j=0; j< 10; ++j)
                    if(solver->field[i][j] == Navy::KILLED)
                        b++;

            if(b == 20)
            {
                QMessageBox::warning(this, tr("Сожалеем"), tr("Но эту партию вы проиграли!"));
                my->switch_cursor(false);
                game = false;
                return;
            }

        }
    }
    else
    {
        my->setState(x,y, Navy::EMPTY);
        solver->setState(x,y, Navy::EMPTY);
    }
    update();
    if( state == LOCAL && s == Navy::SHIP && my->isAlive())
    {
        int x,y;
        solver->fire(x,y);
        ui->wdg->shoot_my(x,y);
    }

}

void SeaBattle::enemy_clicked(int x, int y)
{

    if(!curState) return;

    if(!(state == GAME || state == LOCAL))
        return;

    game = true;

    m_x = x;
    m_y = y;

    my->switch_cursor(false);

    if(state == GAME)
    {
        QString mes =  "SEND " + curPlayer + " FIRE " + QString::number(m_x) +  " " + QString::number( m_y);
        writeMessage(mes);
    }
    else
    {
        Navy::lState s = enemy->getState(m_x, m_y);
        if(s == Navy::EMPTY || s == Navy::SHOT || s == Navy::KILLED)
        {
            ui->wdg->update();
            my->switch_cursor(true);
            return;
        }

        if(s == Navy::SHIP) s = Navy::SHOT;
        else if(s == Navy::FREE) s = Navy::EMPTY;

        enemy->setState(m_x, m_y, s);
        ui->wdg->update();
        if(!enemy->isAlive())
        {
            QMessageBox::warning(this, tr("Поздравляем"), tr("Эту партию вы выиграли."));
            game = false;
            return;
        }

        if(s == Navy::SHOT || s == Navy::KILLED)
        {
            if(use_sound)
                QSound::play(":/sound/popal.wav");
            my->switch_cursor(true);
            return;
        }
        else
        {
            int x,y;
            //printf("Heello");
            solver->fire(x,y);
            ui->wdg->shoot_my(x,y);
        }
        if(!my->isAlive())
        {
            //QMessageBox::warning(this, "Fail","Computer wins!");
        }
        else
            my->switch_cursor(true);
    }
}

void SeaBattle::connectToServer()
{
    ConnectToServer * con = new ConnectToServer(this);
    if(con->exec())
    {
        const struct SeaServer * serv = con->getServ();

        if(serv->location != NULL)
        {
            ui->tab_2->setEnabled(false);

            enemy->m_location = serv->location;
            enemy->locate();
            enemy->setEnemy(true);

            my->locate();

            lblUser->setText(tr("Человек"));
            lblEnemy->setText(tr("Компьютера"));

            changeState(LOCAL);
            my->switch_cursor(true);
            solver->clear();

        }
        else
        {
            socket->disconnectFromHost();
            socket->connectToHost(serv->ip, serv->port);

            if (!socket->waitForConnected(2000))
                QMessageBox::warning(this, tr("Ошибка"), tr("Программе не удалось подключиться к игровому серверу.\nВозможно некорректно указаны настройки."));
            else
            {
                m_blockSize = 0;
                QString mes = "LOGIN " + serv->login;

                writeMessage(mes);

                lblUser->setText(serv->login);
                player = serv->login;

                lblStatus->setText(tr("Подключено к серверу: %1:%2.").arg(serv->ip).arg(serv->port));

                my->locate();
                enemy->clear();

                act_users->setEnabled(true);
            }
        }
    }
    delete con;

}

void SeaBattle::listUsers()
{
    if(socket->isOpen())
        writeMessage("LIST ");
    else
        QMessageBox::information(this, tr("Нет соединения"), tr("Отсутствует соединение с сервером.\nДля просмотра подключённых пользователей, соединитесь с доступным игровым сервером."));
}

void SeaBattle::readyRead()
{
    QDataStream in(socket);
    for(;;)
    {
        if(!m_blockSize)
        {
            if(socket->bytesAvailable() < sizeof(quint32))
                break;
            in >> m_blockSize;
        }

        if(socket->bytesAvailable()  < m_blockSize)
            break;

        //выделяем память для нашего сообщения
        char * mes = (char *) malloc(m_blockSize);
        in.readRawData(mes, m_blockSize);

        QString s = QString::fromUtf8(mes, m_blockSize);
        //обязательно освобождаем память

        free(mes);

        response(s);

        m_blockSize = 0;
    }

}

void SeaBattle::tray_activated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
        tray_show();
}

void SeaBattle::timeout()
{
    QTime now = QTime::currentTime();
    lblCurTime->setText( now.toString("hh:mm:ss"));
}

void SeaBattle::loadSettings()
{
    QSettings settings;

    QDesktopWidget wdg;
    QRect w = wdg.screenGeometry();

    int x = (w.width() - width()) / 2;
    int y = (w.height() - height()) / 2;

    w.setX(x);
    w.setY(y);
    w.setWidth(width());
    w.setHeight(height());

    QRect rect = settings.value("win-rect", w).toRect();
    setGeometry(rect);

    restoreState(settings.value("windowState").toByteArray());

}

void SeaBattle::saveSettings()
{
    QSettings settings;
    QRect rect = geometry();

    settings.setValue("win-rect", rect);
    settings.setValue("windowState", saveState());

}

void SeaBattle::tray_about()
{
    About * about = new About(this);
    about->exec();
    delete about;
}

void SeaBattle::tray_exit()
{
    tray->hide();
    qApp->exit();
}

void SeaBattle::tray_show()
{
    show();
    raise();
    activateWindow();
}

void SeaBattle::closeEvent(QCloseEvent *)
{
    if(tray->isVisible())
        hide();
}

SeaBattle::~SeaBattle()
{
    tray->hide();
    saveSettings();

    NavyPainter * navy = dynamic_cast<NavyPainter *> (my);
    if(navy)
    {
        int code = navy->getBgCode();

        QSettings settings;
        settings.setValue("bgcode", code);
    }

    delete solver;
    delete ui;

}
