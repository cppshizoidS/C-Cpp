#pragma once
class Figure
{
public:
    virtual double calcArea() = 0;
    virtual double calcPerimeter() = 0;
};

class Circle : public Figure
{
private:
    int r;
public:
    double calcArea();
    double calcPerimeter();
    int getR();
    void setR(int r);

    Circle(int r);
    virtual ~Circle();
};

class Triangle : public Figure
{
private:
	double a;
	double b;
	double c;
    int x1, y1,
    x2, y2,
    x3, y3;
public:
	enum side{
		A = 1,
		B = 2,
		C = 3,
	};
    double calcArea();
    double calcPerimeter();
    double getSide(int number);
    void getCoor(int& x1, int& y1, int& x2, int& y2, int& x3, int& y3);
    void setCoor(int x1, int y1, int x2, int y2, int x3, int y3);

    Triangle(int x1, int y1, int x2, int y2, int x3, int y3);
    virtual ~Triangle();
};

class Ring : public Figure
{
private:
    int r;
    int R;
public:
    double calcArea();
    double calcPerimeter();
    int getr();
    int getR();
    void setr(int r);
    void setR(int R);

    Ring(int r, int R);
    virtual ~Ring();
};
