#include "Cell.h"

Cell::Cell() {
    this -> state = -1;
    this -> hits = -1;
    this -> coord.set_coord(0,0);
}

int Cell::get_state() {
    return state;
}

Coord Cell::get_coord() {
    return coord;
}

int Cell::get_hits() {
    return hits;
}

void Cell::set_state(int new_state) {
    this -> state = new_state;
}

void Cell::set_hits(int new_hitsvalue) {
    this -> hits = new_hitsvalue;
}

void Cell::set_coord(Coord &new_coord) {
    this -> coord = new_coord;
}
