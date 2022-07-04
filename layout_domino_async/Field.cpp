
#include <random>
#include <ctime>
#include <iomanip>
#include "Field.h"

Field::Field(int rows, int columns) {
    field = new Cell[(rows + 2 * height_bound) * (columns + 2 * width_bound)];
    //Todo: is it diamond or not
    //diamond
//    if (width % 2 == 0) {
//        size_available_field = ((rows + 1) * (columns + 1)) / 2;
//        std::cout << size_available_field << std::endl;
//    } else {
//        size_available_field = std::ceil(rows * columns / 2);
//        std::cout << size_available_field << std::endl;
//    }

//square
    std::cout << std::endl;
    size_available_field = rows * columns;

    available_points = new Cell[size_available_field];
    width = columns + 2 * width_bound;
    height = rows + 2 * height_bound;

}

Field::~Field() {
    delete[] field;
}


Cell *Field::get_field() {
    return field;
}

Cell *Field::get_available_field() {
    return available_points;
}

int Field::get_size_available_field() {
    return size_available_field;
}

void Field::init_field(std::mt19937 mt) {
    std::uniform_int_distribution<int> dist(0, 1);
    int i = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            //array creates with size + bounds
            //Todo: is it diamond or not
            //diamond
            if (x >= width_bound && x < width - width_bound && y >= height_bound && y < height - height_bound) {
//                float center_x = (float) (width - 1) / 2;
//                float center_y = (float) (height - 1) / 2;
//
//                float distance = std::abs(center_x - (float) x) + std::abs(center_y - (float) y);
//                if (distance <= std::ceil((float) (width - 1 - 2 * width_bound) / 2)) {
//                    field[y * width + x].set_state(dist(mt));
//                    //field[y * width + x].set_state(0);
//                    //add romb state
//                    Coord coord(x - width_bound, y - height_bound);
//                    field[y * width + x].set_coord(coord);
//                    available_points[i] = field[y * width + x];
//                    i++;
//                } else {
//                    field[y * width + x].set_state(0);
//                    Coord coord(-1, -1);
//                    field[y * width + x].set_coord(coord);
//                }


                //square
                field[y * width + x].set_state(dist(mt));
                field[y * width + x].set_state(0);
//                add romb state
                Coord coord(x - width_bound, y - height_bound);
                field[y * width + x].set_coord(coord);
                available_points[i] = field[y * width + x];
                i++;
            } else {
                field[y * width + x].set_state(0);
                Coord coord(-1, -1);
                field[y * width + x].set_coord(coord);
            }
            field[y * width + x].set_hits(0);
        }
    }
}

int Field::get_width() {
    return width;
}

int Field::get_height() {
    return height;
}

std::ostream &operator<<(std::ostream &out, const Field &_field) {
    //write state of field
    out << "Status :" << std::endl;
    for (int y = -1; y < _field.height; ++y) {
        for (int x = -1; x < _field.width; ++x) {
            if (y == -1 && x >= 0) {
                out << std::setw(3) << x;
                continue;
            }
            if (x == -1 && y >= 0) {
                out << std::setw(3) << y;
                continue;
            } else if (x < _field.width_bound || y < _field.height_bound
                       || x >= _field.width - _field.width_bound || y >= _field.height - _field.height_bound ||
                       _field.field[y * _field.width + x].get_coord().get_x() == -1) {
                out << std::setw(3) << "#";
            } else {
                out << std::setw(3) << _field.field[y * _field.width + x].get_state();
            }
        }
        out << std::endl;
    }

    return out;
}

int Field::get_width_bound() const {
    return width_bound;
}

int Field::get_height_bound() const {
    return height_bound;
}


void Field::change_cell_status_by_coord(Coord coord, int status) {
    field[coord.get_y() * width + coord.get_x()].set_state(status);
}


void Field::change_cell_hits_by_coord(Coord coord, int hits) {
    field[coord.get_y() * width + coord.get_x()].set_hits(hits);
}

Cell Field::get_cell_by_coord(Coord coord) {
    return field[coord.get_y() * width + coord.get_x()];
}

Cell Field::get_cell_by_xy(int x, int y) {
    int shifted_y = y;
    int shifted_x = x;
    return field[shifted_y * width + shifted_x];
}
