#ifndef LAYOUT_DOMINO_TEMPLATE_H
#define LAYOUT_DOMINO_TEMPLATE_H


#include <ostream>
#include "Cell.h"
#include "Tiletype.h"
const int WIDTH = 4;
const int HEIGHT = 3;


class Template {

private:

    Cell *single_template;
    Coord center;

    void fill_template();

public:

    Template();
    ~Template();

    void init_template(Coord &center_template);


    int get_center_state();
    Cell *get_template();
    Coord get_center();

    friend std::ostream& operator<< (std::ostream &out, const Template &_template);

};

#endif //LAYOUT_DOMINO_TEMPLATE_H
