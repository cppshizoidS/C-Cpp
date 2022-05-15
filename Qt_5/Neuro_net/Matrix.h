#pragma once
#include <iostream>
#include <vector>
#include "ActivateFunction.h"
using namespace std;

class Matrix
{
	double** matrix;
	int row, col;
public:
	void Init(int row, int col);
	void Rand();
	Matrix Multi(const Matrix& m1, const Matrix& m2);
	Matrix Transpo(const Matrix& m);
	static void Sum_Vector(double* a, const double* b, int n);
	static void Multi(const Matrix& m, const double* b, double* c);
	static void Multi_T(const Matrix& m, const double* b, double* c);
	double& operator ()(int i, int j);
	friend ostream& operator << (ostream& os, const Matrix& m);
	friend istream& operator >> (istream& is, Matrix& m);
};

