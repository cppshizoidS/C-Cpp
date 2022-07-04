#ifndef LAYOUT_DOMINO_FIELD_H
#define LAYOUT_DOMINO_FIELD_H


#include <iostream>
#include "Cell.h"


class Field {
private:
    Cell *field;
    Cell *available_points;
    int size_available_field;
    int height;
    int width;
    const int width_bound = 2;
    const int height_bound = 2;



public:
    void init_field(std::mt19937 mt);
    Field(int rows, int columns);

    ~Field();

    Cell *get_field();

    Cell *get_available_field();

    //TODO
    int get_width_bound() const;
    int get_height_bound() const;

    int get_width();
    int get_height();
    Cell get_cell_by_coord(Coord coord);
    int get_size_available_field();
    Cell get_cell_by_xy(int x, int y);
    void change_cell_status_by_coord(Coord coord, int status);
    void change_cell_hits_by_coord(Coord coord, int hits);

    friend std::ostream& operator<< (std::ostream &out, const Field & _field);

};


#endif //LAYOUT_DOMINO_FIELD_H
