#ifndef IFIGURES_H
#define IFIGURES_H

#include "DrawFunctions.h"
#include "Figures.h"

struct Coords {
    double x;
    double y;
    Coords(double sX = 0, double sY = 0) : x{sX}, y{sY} {}
};

class IFigures {
protected:
    Coords coords;
public:
    virtual ~IFigures() = default;
    void setCoords(double, double);
    void setCoords(Coords);
    virtual void Draw(const Cairo::RefPtr<Cairo::Context>& pContext) = 0;
};

class IRectangle : public IFigures {
    Figures::Rectangle r;
public:
    IRectangle(Figures::Rectangle a): r(a.GetA(), a.GetB()) {
        r = a;
    }

    void Draw(const Cairo::RefPtr<Cairo::Context>& pContext) override {
        DrawRectangle(pContext, coords.x, coords.y, r.GetA(), r.GetB());
    }
};

class ICircle : public IFigures {
    Figures::Circle c;
public:    
    ICircle(Figures::Circle a): c{a.GetR()}{
        c = a;
    }

    void Draw(const Cairo::RefPtr<Cairo::Context>& pContext) override {
        DrawCircle(pContext, coords.x, coords.y, c.GetR());
    }
};

class IRing : public IFigures {
    Figures::Ring r;
public:    
    IRing(Figures::Ring a): r(a.Getr(), a.GetR()){
        r = a;
    }

    void Draw(const Cairo::RefPtr<Cairo::Context>& pContext) override {
        DrawRing(pContext, coords.x, coords.y, r.Getr(), r.GetR());
    }
};

// class ITriangle : public IFigures {
//     Figures::Triangle t;
// public:    
//     ITriangle(Figures::Triangle a): mtX1{ptX1}, mtY1{ptY1}, mtX2{ptX2}, mtY2{ptY2}{
//         t = a;
//     }

//     void Draw(const Cairo::RefPtr<Cairo::Context>& pContext) override {
//         DrawTriangle(pContext, mtX1, mtY1, mtX2, mtY2);
//     }
// };

#endif