
#ifndef LAYOUT_DOMINO_COORD_H
#define LAYOUT_DOMINO_COORD_H


#include <ostream>

struct Coord {
private:
    int x;
    int y;
public :
    int get_x() const;
    int get_y() const;
    void set_coord(int x_coord, int y_coord);

    Coord();
    Coord(int x_coord, int y_coord);

    friend std::ostream& operator<< (std::ostream &out, const Coord &point);
};


#endif //LAYOUT_DOMINO_COORD_H
