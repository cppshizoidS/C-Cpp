#ifndef CELL_H
#define CELL_H

class QPainter;
class Ship;

class Cell
{
public:
    Cell();
    Cell(int x, int y);

    bool isAlive() const;
    void setKilled();
    int x();
    int y();

    void setX(int x0) { m_x = x0;}
    void setY(int y0) { m_y = y0;}

    void paint(QPainter *, const Ship *, double s);

private:
    int m_x;
    int m_y;
    bool alive;
    int rot;
};

#endif // CELL_H
