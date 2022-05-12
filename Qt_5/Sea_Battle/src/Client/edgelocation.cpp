#include "edgelocation.h"
#include <qglobal.h>
#include <time.h>
#include <QVector>

EdgeLocation::EdgeLocation() : AutoLocation()
{
    qsrand(time(NULL));
}

void EdgeLocation::LocateShips(Navy *navy)
{
    const int cycles = 500;
    int mv = 0;
    int best = 0;

    QVector<struct coords> data(cycles);

    for(int cycle =0; cycle < cycles; ++cycle)
    {
        for(int i = 4; i > 1; i--)
        {
            for(int j=0; j<= 4-i; ++j)
            {
                while(true)
                {
                    int x = qrand() % 10;
                    int y = qrand() % 10;
                    bool vert = qrand() % 2;

                    bool ok = navy->locateShip(x,y,i, vert);
                    if(ok)
                    {
                        struct coords temp;
                        temp.x = x;
                        temp.y = y;
                        temp.vert = vert;
                        temp.level = i;

                        data.push_back(temp);
                        break;
                    }
                }
            }


        }
        int k =0;
        for(int i =0; i < navy->N; ++i)
            for(int j =0; j< navy->N; ++j)
                if(navy->getState(i,j) == Navy::FREE)
                    k++;

        if(k > mv)
        {
            best = data.size();
            if(best > 6)
                best-=6;

            mv = k;
        }
        navy->clear();
    }

    for(int i = best; i< best+6; ++i)
    {
        int x = data[i].x;
        int y = data[i].y;
        int level = data[i].level;
        bool vert = data[i].vert;

        navy->locateShip(x,y,level, vert);
    }

//--------------------------------------(то, что выше, из randmaxlocation, а то, что ниже, из randomlocation)

    for(int i = 1; i > 0; i--)
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


