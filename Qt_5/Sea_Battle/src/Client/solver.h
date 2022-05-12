#ifndef SOLVER_H
#define SOLVER_H

#include "navy.h"
#include "Net.h"
#include <QVector>

class Solver : public Navy
{
public:
    Solver(int width=10, int height=10);
    virtual ~Solver();
    virtual void fire(int &, int &);
    int getWidth();
    int getHeight();

private:
    struct shot{int x; int y;};
    NeuralNetwork::Net * net;
    int width, height;

    QVector<NeuralNetwork::real> input;
    QVector<NeuralNetwork::real> output;
//    NeuralNetwork::real *output;

};

#endif // SOLVER_H
