#include "IFigures.h"

void IFigures::setCoords(double x, double y) {
    this->coords.x = x;
    this->coords.y = y;
}

void IFigures::setCoords(Coords coord) {
    this->coords.x = coord.x;
    this->coords.y = coord.y;
}
