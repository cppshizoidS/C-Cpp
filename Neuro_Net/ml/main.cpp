#include <iostream>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <sstream>
#include "mathlibrery.cpp"

int main()
{
    srand(static_cast<unsigned int>(time(0)));
    Matrix<double, 4, 3> streetlights = {{1.0, 0.0, 1.0},
                                         {0.0, 1.0, 1.0},
                                         {0.0, 0.0, 1.0},
                                         {1.0, 1.0, 1.0}};

    //streetlights = {{1.0, 0.0, 1.0},
    //{0.0, 1.0, 1.0},
    //{0.0, 0.0, 1.0},
    //{1.0, 1.0, 1.0}};

    Matrix<double, 1, 4> tempMatrix = {{1.0, 1.0, 0.0, 0.0}};
    //tempMatrix = {{1.0, 1.0, 0.0, 0.0}};
    Matrix<double, 4, 1> walk_vs_stop;

    walk_vs_stop = tempMatrix.matrixTranspose();

    Matrix<double, 3, 4> weights_0_1 = {{0.1, -0.5, 0.4, 0.7},
                                        {0.2, -0.7, 0.3, 0.9},
                                        {0.1, -0.8, 0.3, 0.5}};
    Matrix<double, 4, 1> weights_1_2 = {{0.2},
                                        {-0.5},
                                        {0.3},
                                        {0.7}};

    double alpha = 0.5;
    /*Matrix<double, 3, 4> weights_0_1;
    Matrix<double, 4, 1> weights_1_2;
    weightsGenerator(weights_0_1);
    weightsGenerator(weights_1_2);
    */
    for(int i = 0; i < 600000; ++i)
    {
        double layer_2_error = 0;
        for(int j = 0; j < 4; ++j)
       {
          double reluResult[4]{};
          Matrix<double, 1, 3> layer_0;
          layer_0 = cutFunction(streetlights, j);
          Matrix<double, 1, 4> layer_1;
          layer_1 = layer_0 * weights_0_1;
          relu(layer_1);
          Matrix<double, 1, 1> layer_2;
          layer_2 = layer_1 * weights_1_2;
          Matrix<double, 1, 1> wvsCut;
          wvsCut = cutFunction(walk_vs_stop, j);
          Matrix<double, 1, 1> layer_2_delta;
          layer_2_delta = layer_2 - wvsCut;
          Matrix<double, 1, 1> matrixSqrt;
          matrixSqrt = layer_2_delta * layer_2_delta;
          layer_2_error = layer_2_error + matrixSqrt[0][0];
          Matrix<double, 1, 4> weight_1_2_Transpose;
          weight_1_2_Transpose = weights_1_2.matrixTranspose();
          Matrix<double, 1, 4> layer_1_delta;
          layer_1_delta = layer_2_delta * weight_1_2_Transpose;
          relu2deriv(layer_1, reluResult);
          for(int k = 0; k < 1; ++k)
              for(int l = 0; l < 4; ++l)
              {
                  layer_1_delta[k][l] = layer_1_delta[k][l] * reluResult[l];
              }
          Matrix<double, 4, 1> layer_1_Transpose;
          layer_1_Transpose = layer_1.matrixTranspose();
          Matrix<double, 4, 1> inst1;
          inst1 = (layer_1_Transpose * layer_2_delta) * alpha;
          weights_1_2 = weights_1_2 - inst1;

          Matrix<double, 3, 1> layer_0_Transpose;
          layer_0_Transpose = layer_0.matrixTranspose();
          Matrix<double, 3, 4> inst2;
          inst2 = (layer_0_Transpose * layer_1_delta) * alpha;
          weights_0_1 = weights_0_1 - inst2;
        }
        if(i % 10 == 9)
            //std::cout << "Error: " << layer_2_error << std::endl;
            printf("%.16e\n", layer_2_error);
    }
    return 0;
}
