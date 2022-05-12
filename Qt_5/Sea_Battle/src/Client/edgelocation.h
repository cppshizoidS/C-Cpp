#ifndef EDGELOCATION_H
#define EDGELOCATION_H

#include "autolocation.h"

/*Тактика расстановки кораблей: все корабли кроме однопалубных расставляются по краям поля.
  Таким образом обеспечивается максимальное время поиска однопалубных кораблей.
  Достичь этого пробую слиянием уже существующих randmaxlocation и randomlocation. */

class EdgeLocation : public AutoLocation
{
public:
    EdgeLocation();
    void LocateShips(Navy *navy);
private:
    struct coords
    {
        int x;
        int y;
        bool vert;
        int level;
    };
};

#endif // EDGELOCATION_H
