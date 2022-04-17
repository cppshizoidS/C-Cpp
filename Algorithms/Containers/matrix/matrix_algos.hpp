#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include <iostream>

class Matrix 
{
public:
	Matrix(unsigned int _size = 3);
	Matrix(double** _array, unsigned int _size);
	~Matrix();
	double get_determ();
	unsigned char size();
	void print(unsigned int _t = 5);
	void scan();
	void random(const long range = 1000);
	void transpose();
	void inverse();
	double* operator[](unsigned int _row);
private:
	double** matrix;
	unsigned int size_matrix;
	
	double** get_sub_matrix(double** _matrix, unsigned int size, unsigned int delim_x, unsigned int delim_y);
	double proc_determ(double** _matrix, unsigned int size);
};

#endif
