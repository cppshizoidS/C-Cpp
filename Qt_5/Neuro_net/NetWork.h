#pragma once
#include <qmath.h>
#include <random>
#include <fstream>
#include <iostream>
#include <chrono>
using namespace std;
#include "ActivateFunction.h"
#include "Matrix.h"
struct neuron {
	double value;
	double error;
};
struct data_NetWork {
	int n;
	int* size;
};
class NetWork
{
	int n;
	int* size;
	activateFunction actFunc;
	neuron** neurons;
	neuron* neurons_bios;
	double*** weights;
	/////////////////////////////////////////
	Matrix* weights_;
	double** bios;
	double** neurons_val, ** neurons_err;
	double* neurons_bios_val, * neurons_bios_err;
public:
	void Init(data_NetWork data);
	void SaveWeights();
	void ReadWeights();
	void SetInput(double* values);
	double ForwardFeed();
	void BackPropogation(double expect);
	double ErrorCounter();
	void WeightsUpdater(double lr);
	void PrintConfig();
	void PrintValues(int n);
	int SearchMaxIndex(double* value);
};