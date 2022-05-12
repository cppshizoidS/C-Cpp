#ifndef RANDMAXLOCATION_H
#define RANDMAXLOCATION_H

#include "autolocation.h"

/*

  Эвристический  подход  в  расставлении  кораблей. Идея ---
  раставить корабли, таким образом, чтобы было очень большое
  свободное пространство. Скученные корабли найдутся быстро,
  а на поиски всех остальных (если они вообще будут), у про-
  тивника может уйти много времени. Последовательно перебир-
  аем возможные рандомные комбинации, запоминаем позиции ко-
  раблей. Определяем  вариант  с наибольшим  объемом пустого
  пространства и выводим его в качестве конечного результата.

  */

class RandMaxLocation : public AutoLocation
{
public:
    RandMaxLocation();
    void LocateShips(Navy *navy);
private:
    struct coords {
        int x;
        int y;
        bool vert;
        int level;
    };
};

#endif // RANDMAXLOCATION_H
