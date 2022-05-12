#include "navy.h"
#include "autolocation.h"

#include <QtGlobal>
#include <time.h>
#include <stdio.h>
#include <QMessageBox>

Navy::Navy(AutoLocation * location, bool enemy) : m_location(location), m_enemy(enemy)
{
    clear();
}

void Navy::clear()
{
    for(int i =0; i< N; ++i)
        for(int j = 0; j < N; ++j)
            field[i][j] = FREE;
        ships.clear();
}

bool Navy::isAlive() const
{
    for(int i =0; i< N; ++i)
        for(int j=0; j< N; ++j)
            if(field[i][j] == SHIP) return true;
    return false;
}

bool Navy::isShipAlive(int x, int y)
{
    for(int i=0; i< ships.size(); ++i)
        for(int j=0; j< ships[i].cells.size(); ++j)
            if(ships[i].cells[j].x() == x && ships[i].cells[j].y() == y &&
                    !ships[i].isAlive())
                return false;
    return true;
}

void Navy::setEnemy(bool enemy)
{
    m_enemy = enemy;
}

bool Navy::isEnemy() const
{
    return m_enemy;
}

Navy::~Navy()
{
    if(m_location)
       delete m_location;
}

enum Navy::lState Navy::getState(int x, int y) const
{
    if(x < 0 || y >= 10) return Navy::S_ERROR;
    return field[x][y];
}

const char * Navy::getTraining()
{
    char *p = training;
    for(int i=0; i<N; ++i)
    for(int j=0; j<N; ++j)
    {
      lState c = field[i][j];
      if(c == SHIP) c = FREE;
      *p++ = (c + '0');
    }
    *p='\0';
    return training;
}

void Navy::setState(int x, int y, enum lState s)
{
    if(x >= 0 && x < N && y >= 0 && y < N)
    {        
        if(s == SHOT && getState(x,y) == SHIP)
        {

            setState(x-1, y-1, EMPTY);
            setState(x-1, y+1, EMPTY);
            setState(x+1, y-1, EMPTY);
            setState(x+1, y+1, EMPTY);

            for(int i =0; i< ships.size(); ++i)
            {
                for(int j=0; j< ships[i].cells.size(); ++j)
                if(x == ships[i].cells[j].x() && y == ships[i].cells[j].y())
                {
                    ships[i].cells[j].setKilled();
                    bool alive = ships[i].isAlive();
                    if(!alive)
                    {
                        for(int h=0; h< ships[i].cells.size(); ++h)
                        {
                            int _x = ships[i].cells[h].x();
                            int _y = ships[i].cells[h].y();

                            setState(_x, _y, KILLED);
                            change(_x, _y);

                            setState(_x-1, _y-1, EMPTY);
                            setState(_x+1, _y+1, EMPTY);
                            setState(_x+1, _y-1, EMPTY);
                            setState(_x-1, _y+1, EMPTY);

                        }

                        int s_x = ships[i].cells[0].x();
                        int s_y = ships[i].cells[0].y();

                        int e_x = ships[i].cells[ships[i].cells.size() - 1].x();
                        int e_y = ships[i].cells[ships[i].cells.size() - 1].y();

                        if(ships[i].level() == 1)
                        {
                            setState(s_x, s_y-1, EMPTY);
                            setState(e_x, e_y+1, EMPTY);
                            setState(s_x - 1, s_y, EMPTY);
                            setState(e_x + 1, e_y, EMPTY);
                        }

                        if(ships[i].vert)
                        {
                            setState(s_x, s_y-1, EMPTY);
                            setState(e_x, e_y+1, EMPTY);
                        }
                        else
                        {
                            setState(s_x - 1, s_y, EMPTY);
                            setState(e_x + 1, e_y, EMPTY);
                        }
                        return;
                    }
                    field[x][y] = s;
                    change(x,y);
                    return;

                }
            }
        }

        field[x][y] = s;
        change(x,y);
    }
}

void Navy::change(int, int)
{

}

bool Navy::locateShip(int x0, int y0, int level, bool vert)
{

    if(x0 < 0 || x0 >= N) return false;
    if(y0 < 0 || y0 >= N) return false;

    if(vert && (y0 + level > N))
        return false;

    if(!vert && (x0 + level > N))
        return false;

    bool ok = true;


    if(vert)
    {
        for(int y=y0; y< y0+level; ++y)
            if(field[x0][y] != FREE )
            {
                ok = false;
                break;
            }
    }
    else
    {
        for(int x=x0; x< x0+level; ++x)
            if(field[x][y0] != FREE )
            {
                ok = false;
                break;
            }
    }
    if(!ok) return false;

    Ship ship;

    ship.vert = vert;

    if(vert)
    {
        int x,y;
        for(x= x0, y = y0; y< y0+level; y++)
        {
            field[x][y] = SHIP;
            ship.addCell(x,y);

            setState(x-1, y, Navy::S_NEAR);
            setState(x+1, y, Navy::S_NEAR);
        }
        setState(x0-1, y0-1, Navy::S_NEAR);
        setState(  x0, y0-1, Navy::S_NEAR);
        setState(x0+1, y0-1, Navy::S_NEAR);
        setState(x-1, y, Navy::S_NEAR);
        setState(  x, y, Navy::S_NEAR);
        setState(x+1, y, Navy::S_NEAR);

    }
    else
    {
        int x;
        for(x= x0; x< x0 + level; x++)
        {
            field[x][y0] = SHIP;
            ship.addCell(x,y0);

            setState(x, y0-1, Navy::S_NEAR);
            setState(x, y0+1, Navy::S_NEAR);
        }
        setState(x0-1, y0-1, Navy::S_NEAR);
        setState(x0-1,   y0, Navy::S_NEAR);
        setState(x0-1, y0+1, Navy::S_NEAR);
        setState(x, y0-1, Navy::S_NEAR);
        setState(x,   y0, Navy::S_NEAR);
        setState(x, y0+1, Navy::S_NEAR);
    }

    ships.push_back(ship);
    return true;
}

void Navy::switch_cursor(bool)
{

}

void Navy::clean()
{
    for(int i = 0; i <N; ++i)
    for(int j = 0; j< N; ++j)
        if(field[i][j] == Navy::S_NEAR)
            field[i][j] = Navy::FREE;
}

void Navy::locate()
{
    clear();
    if(m_location)
        m_location->LocateShips(this);

    for(int i = 0; i <N; ++i)
    for(int j = 0; j< N; ++j)
        if(field[i][j] == Navy::S_NEAR)
            field[i][j] = Navy::FREE;
}
