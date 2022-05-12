#ifndef SHIPSETTLE_H
#define SHIPSETTLE_H

#include <QDialog>
#include "navy.h"

namespace Ui {
    class ShipSettle;
}

class QGraphicsScene;
class ShipSettle : public QDialog
{
    Q_OBJECT

public:
    explicit ShipSettle(QWidget *parent = 0, Navy * m_navy = NULL);
    ~ShipSettle();

private:
    Ui::ShipSettle *ui;
    Navy *navy;
    Navy temp;

    QGraphicsScene * scene;
    int w,h;

    void moveShip(Ship &);
    bool check();

private slots:

    void cancel();
    void ok();

};

#endif // SHIPSETTLE_H
