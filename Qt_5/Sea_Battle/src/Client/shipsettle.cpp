#include "shipsettle.h"
#include "ui_shipsettle.h"

#include <QMessageBox>

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>

#include "shipitem.h"
#include <stdio.h>

ShipSettle::ShipSettle(QWidget *parent, Navy *n) :
    QDialog(parent),
    ui(new Ui::ShipSettle), navy(n)
{
    ui->setupUi(this);

    show();
    scene = new QGraphicsScene(this);

    scene->setSceneRect(0,0,301, 310);

    for(int i =0; i < navy->ships.count(); ++i)
        moveShip(navy->ships[i]);

    ui->scene->setScene(scene);

    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(ok()));
}

void ShipSettle::ok()
{
    if(check())
    {
        navy->clear();
        for(int i=0;i<temp.ships.size(); ++i)
        {
            navy->locateShip(
                        temp.ships[i].cells[0].x(),
                        temp.ships[i].cells[0].y(),
                        temp.ships[i].level(),
                        temp.ships[i].vert
                        );
        }
        navy->clean();
        accept();
    }
}

void ShipSettle::cancel()
{
    reject();
}

bool ShipSettle::check()
{
    temp.clear();

    //используем класс Navy для определения правильного порядка
    //расстановки кораблей.

    QList<QGraphicsItem *> items = scene->items();
    for(int i =0; i< items.count(); ++i)
    {
        ShipItem * item = static_cast<ShipItem *> (items[i]);
        Ship & ship = item->m_ship;

        int x = (item->scenePos().x() - 24) / 26.8 ;
        int y = (item->scenePos().y() -24) / 26.8 ;

        if(x < 0 || x > 10 || y <0 || y > 10)
        {
            QMessageBox::critical(this, tr("Ошибка"), tr("Проверьте расположение кораблей."));
            return false;
        }

        if(!temp.locateShip(x,y, ship.level(), item->rotate))
        {
            QMessageBox::critical(this, tr("Ошибка"), tr("Проверьте расположение кораблей."));
            return false;
        }
    }
    return true;
}

ShipSettle::~ShipSettle()
{
    delete ui;
}

void ShipSettle::moveShip(Ship & ship)
{
    ShipItem * rect = new ShipItem(scene, ship.level(), ship);

    int x = ship.cells[0].x();
    int y = ship.cells[0].y();

    int ex = ship.cells[ship.level() -1].x();
    int ey = ship.cells[ship.level() -1].y();

    if(ship.vert)
    {
        double dx = ey - y +1;
        dx = dx > 0 ? dx : 1;
        rect->setRect(0,0, 24, dx* 24,true);
        rect->setPos(x*26.8+25,y*26.8 +28);
    }
    else
    {
        double dx = ex - x+1;
        dx = dx > 0 ? dx : 1;
        rect->setRect(0,0, dx*24, 24, false);
        rect->setPos(x*26.8+25,y*26.8+30 );
    }
}
