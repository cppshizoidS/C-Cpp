#include "randomlocation.h"

#include <qglobal.h>
#include <time.h>

 RandomLocation::RandomLocation() : AutoLocation()
 {
     qsrand(time(NULL));
 }

void RandomLocation::LocateShips(Navy *navy)
{
    navy->clear();
    for(int i = 4; i > 0; i--)
    {
        for(int j=0; j<= 4-i; ++j)
        {
            while(true)
            {
                int x = qrand() % 10;
                int y = qrand() % 10;

                bool ok = navy->locateShip(x,y,i, qrand() % 2);
                if(ok) break;
            }
        }
    }
}
