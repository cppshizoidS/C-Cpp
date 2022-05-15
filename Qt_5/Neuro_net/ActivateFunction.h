#pragma once
#include <qmath.h>
#include <random>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
enum activateFunc { sigmoid = 1, ReLU, thx };
class activateFunction
{
	activateFunc actFunc;
public:
	void set();
	double use(double value);
	void use(double* value, int n);
	void usePro(double* value, int n);
	double usePro(double value);
};