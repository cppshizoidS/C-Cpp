#include "solver.h"
#include <QtGlobal>

#include <stdio.h>

#include <fstream>
#include <QMessageBox>
#include <QCoreApplication>

#include "Net.h"

Solver::Solver(int width, int height)
{
    this->width = width;
    this->height = height;

    input = QVector<NeuralNetwork::real>(width * height);
    output = QVector<NeuralNetwork::real>(width * height);

    std::ifstream stream;

    QString path = QCoreApplication::applicationDirPath() + "/solver.nnw";

    //QMessageBox::warning(0, "gg", path);

    stream.open(path.toLatin1(), std::ifstream::binary);

    if(stream.is_open())
    {
        net = new NeuralNetwork::Net(stream);
//        input = (double *) calloc(100, sizeof(double));
//        output = (double *) calloc(30, sizeof(double));
//        output = (NeuralNetwork::real *)malloc(width * height);
    }
    else
    {
        net = NULL;
    }
}

int Solver::getWidth()
{
    return width;
}

int Solver::getHeight()
{
    return height;
}

Solver::~Solver()
{
    if(net)
    {
//        delete input;
//        delete output;
//        free(output);
        delete net;
    }
}

void Solver::fire(int &x, int &y)
{
    printf("fire");
    int x0, y0;
    int k = 0;

    for(int i =0; i <width; ++i)
    {
        for(int j =0; j< height; ++j)
        {
            printf("%d ", field[j][i]);
            input[k++] = field[i][j];
        }
        printf("\n");
    }

    printf("====================================\n");

    if(net != NULL)
    {
    //Что скажеть нейросеть?
        net->run(input.data(), output.data());

    int n_x = output[0] * 10 + 1;
    int n_y = output[1] * 10 + 1;

    printf("Net %d %d\n", n_x, n_y);
    lState ns = getState(n_x-1, n_y-1);

    //первое что делаем - ищем подбитые корабли
    for(int i =0; i <10; ++i)
    {
        for(int j =0; j< 10; ++j)
            if(getState(i,j) == Navy::SHOT)
            {
                //оцениваем куда бить...

                struct shot shots[4];
                int n = 0; //количество возможных ударов

                for(int k = i-1; k>= 0; k--)
                {
                    lState s = getState(k, j);
                    if(s == Navy::S_ERROR || s == Navy::EMPTY)
                        break;
                    if(s == Navy::FREE)
                    {
                        shots[n].x = k +1;
                        shots[n].y = j + 1;
                        n++;
                        break;
                    }
                }

                for(int k = i+ 1; k < 10; k++)
                {
                    lState s = getState(k, j);
                    if(s == Navy::S_ERROR || s == Navy::EMPTY)
                        break;
                    if(s == Navy::FREE)
                    {
                        shots[n].x = k +1;
                        shots[n].y = j + 1;
                        n++;
                        break;
                    }
                }

                for(int k = j + 1; k < 10; k++)
                {
                    lState s = getState(i, k);
                    if(s == Navy::S_ERROR || s == Navy::EMPTY)
                        break;
                    if(s == Navy::FREE)
                    {
                        shots[n].x = i + 1;
                        shots[n].y = k + 1;
                        n++;
                        break;
                    }
                }

                for(int k = j - 1; k >=0 ; k--)
                {
                    lState s = getState(i, k);
                    if(s == Navy::S_ERROR || s == Navy::EMPTY)
                        break;
                    if(s == Navy::FREE)
                    {
                        shots[n].x = i + 1;
                        shots[n].y = k + 1;
                        n++;
                        break;
                    }
                }

                printf("possible shots: %d\n", n);

                if(ns == FREE)
                {
                    //если координата удара по нейросети совпадает с какой-либо
                    //координатой нашего выбора, то бьём по ней
                    //иначе выбираем рандомную цель

                    for(int i=0; i< n; ++i)
                    {
                        if(shots[i].x == n_x &&
                                shots[i].y == n_y)
                        {
                            x = n_x;
                            y = n_y;
                            return;
                        }
                    }
                }

                if(n > 0)
                {
                    //выбираем удар
                    int r = rand() % n;

                    x = shots[r].x;
                    y = shots[r].y;
                    return;
                }
            }

    }

    if(ns == Navy::FREE)
    {
        x = n_x;
        y = n_y;
        return;
    }
}
    while(true)
    {
        x0 = qrand() % 10;
        y0 = qrand() % 10;
        lState w = getState(x0, y0);
        if(w == Navy::FREE)
        {
            x = x0+1;
            y = y0+1;
            return;
        }
    }
}
