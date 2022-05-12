#ifndef AUTOLOCATION_H
#define AUTOLOCATION_H

#include "navy.h"

/*

  Паттерн "Стратегия" для автоматической расстановки кораблей.

  */

class AutoLocation
{
    public:
        virtual void LocateShips(Navy *navy) = 0;
    protected:
        AutoLocation(){}
};

#endif // AUTOLOCATION_H
