#ifndef RANDOMLOCATION_H
#define RANDOMLOCATION_H

#include "autolocation.h"

/*

  Рандомное расположение кораблей. Поскольку пустых клеток
  значительно меньше, чем клеток для кораблей, то алгоритм
  всегда завершится.

  */

class RandomLocation : public AutoLocation
{
public:
    RandomLocation();
    void LocateShips(Navy *navy);
};

#endif // RANDOMLOCATION_H
