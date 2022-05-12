#ifndef NAVY_H
#define NAVY_H

#include <QVector>
#include <QObject>
#include "ship.h"

class AutoLocation;


class Navy
{

public:

    Navy(AutoLocation * location = 0, bool enemy = false);
    virtual ~Navy();

    enum lState {FREE, SHOT, KILLED, S_NEAR, SHIP, EMPTY, S_ERROR}; //состояние палубы корабля

    static const int N = 10; //размер поля.

    enum lState getState(int x, int y) const;
    void setState(int x, int y, enum lState);

    void clear();
    void locate();

    void setEnemy(bool);
    bool isEnemy() const;
    bool isAlive() const;
    bool isShipAlive(int x, int y);

    virtual void change(int,int);
    bool locateShip(int x0, int y0, int level, bool vert);

    void clean();


    const char * getTraining();
    virtual void switch_cursor(bool);

    AutoLocation * m_location;
    QVector<Ship> ships;

private:

    bool m_enemy;
public:

    enum lState field[N][N]; //поле
    char training[N*N+2];

};

#endif // NAVY_H
