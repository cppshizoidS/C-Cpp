#ifndef LAYOUT_DOMINO_CELL_H
#define LAYOUT_DOMINO_CELL_H


#include "Coord.h"

class Cell {
private:
        int state;
        int hits;
        Coord coord;

public:
        Cell();

        int get_state();
        int get_hits();
        Coord get_coord();

        void set_state(int new_state);
        void set_hits(int new_hitsvalue);
        void set_coord(Coord &new_coord);
};


#endif //LAYOUT_DOMINO_CELL_H
