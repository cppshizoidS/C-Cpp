#include "Template.h"


Template::Template() {
    this -> single_template = new Cell[HEIGHT * WIDTH];
    this -> center.set_coord(0,0);
}

Template::~Template() {
    delete[] single_template;
}

Cell *Template::get_template() {
    return single_template;
}


Coord Template::get_center() {
    return center;
}

void Template::fill_template() {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            Coord coord(x, y);
            single_template->set_coord(coord);
            if (center.get_x() == 0 && x == WIDTH - 1) {
                single_template[y * WIDTH + x].set_state(-1);
            } else {
                if (center.get_x() == WIDTH - 1 && x == 0) {
                    single_template[y * WIDTH + x].set_state(-1);
                } else {
                    //fill kernel domino
                    if (y == 1 && (x == 1 || x == 2)) {
                    //
                        single_template[y * WIDTH + x].set_state(1);
                    } else {
                        single_template[y * WIDTH + x].set_state(0);
                    }
                }
            }
        }
    }

}

void Template::init_template(Coord &center_template) {
    this->center = center_template;
    fill_template();
}

std::ostream &operator<<(std::ostream &out, const Template &_template) {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            out << _template.single_template[y * WIDTH + x].get_state() << " ";
        }
        out << std::endl;
    }
    out << "CENTER: " << _template.center;
    return out;
}

int Template::get_center_state() {
    return single_template[center.get_y() * WIDTH + center.get_x()].get_state();
}






