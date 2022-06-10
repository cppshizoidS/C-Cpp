#ifndef FIGURES_H
#define FIGURES_H

namespace Figures {

    class Figure2D {
    public:
        virtual Figure2D* Clone() = 0;
        virtual double CalcArea() = 0;
        virtual double CalcPerimeter() = 0;
    };

    class Circle : public Figure2D {
        double r;
    public:
        Circle(double);
        virtual Figure2D* Clone();
        virtual double CalcArea();
        virtual double CalcPerimeter();

        double GetR();
    };

    class Rectangle : public Figure2D {
        double a, b;
    public:
        Rectangle(double, double);
        virtual Figure2D* Clone();
        virtual double CalcArea();
        virtual double CalcPerimeter();

        double GetA();
        double GetB();
    };

    class Ring : public Figure2D {
        double r, R;
    public:
        Ring(double, double);
        virtual Figure2D* Clone();
        virtual double CalcArea();
        virtual double CalcPerimeter();
        
        double Getr();
        double GetR();
    };

    // class Triangle : public Figure2D {
    //     double a, b, c;
    // public:
    //     Triangle(double, double, double);
    //     virtual Figure2D* Clone();
    //     virtual double CalcArea();
    //     virtual double CalcPerimeter();

    //     double GetA();
    //     double GetB();
    //     double GetC();
    // };

};

#endif