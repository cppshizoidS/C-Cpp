#include <iomanip>
#include <cstring>
#include <cmath>
#include "matrix_algos.hpp"


Matrix::Matrix(unsigned int _size) : size_matrix(_size)
{
    matrix = new double*[size_matrix];
    matrix[0] = new double[size_matrix * size_matrix];
    for (unsigned int i = 0; i < size_matrix; ++i)
	{
        matrix[i] = matrix[0] + size_matrix * i;
        memset(matrix[i], 0, size_matrix);
    }
}

Matrix::Matrix(double** _array, unsigned int _size) : size_matrix(_size)
{
    matrix = new double*[size_matrix];
    matrix[0] = new double[size_matrix * size_matrix];
    for (unsigned int i = 0; i < size_matrix; ++i)
	{
        matrix[i] = matrix[0] + size_matrix * i;
        for (unsigned int j = 0; j < size_matrix; ++j)
            matrix[i][j] = _array[i][j];
    }
}

Matrix::~Matrix()
{
    delete [] matrix[0];
    delete [] matrix;
}

double** Matrix::get_sub_matrix(double** _matrix, unsigned int size, unsigned int delim_x, unsigned int delim_y)
{
    unsigned int sub_size = size - 1;
    double** sub_matrix = new double*[sub_size];
    sub_matrix[0] = new double[sub_size * sub_size];
    for (unsigned int i = 0, h = 0; i < sub_size; ++i, h++)
	{
        sub_matrix[i] = sub_matrix[0] + sub_size * i;
        for (unsigned int j = 0, k = 0; j < sub_size; ++j, ++k)
            sub_matrix[i][j] = _matrix[(h == delim_y) ? ++h : h][(j == delim_x) ? ++k : k];
    }
    return sub_matrix;
}

double Matrix::proc_determ(double** _matrix, unsigned int size)
{
    if (size <= 1)
	{
        return _matrix[0][0];
    }
    
    double determinator = 0;
    for (unsigned int i = 0; i < size; ++i)
	{
        double** sub_matrix = get_sub_matrix(_matrix, size, i, 0);
        determinator += _matrix[0][i] * proc_determ(sub_matrix, size - 1) * ((i % 2 == 0) ? 1 : -1);
        delete [] sub_matrix[0];
        delete [] sub_matrix;
    }
    
    return determinator;
}

void Matrix::print(unsigned int _t)
{    
    for (unsigned int i = 0; i < size_matrix; ++i)
	{
        for (unsigned int j = 0; j < size_matrix; ++j)
            std::cout << std::setw(_t) << matrix[i][j] << ' ';
        std::cout << std::endl;
    }
}

void Matrix::scan()
{
    for (unsigned int i = 0; i < size_matrix; ++i)
	{
        for (unsigned int j = 0; j < size_matrix; ++j)
	    {
            std::cout << "Enter the value [" << i << "][" << j << "] element: ";
            std::cin >> matrix[i][j];
        }
        std::cout << std::endl;
    }
}

void Matrix::random(const long range)
{
    for (unsigned int i = 0; i < size_matrix; ++i)
        for (unsigned int j = 0; j < size_matrix; ++j)
            matrix[i][j] = rand() % range;
}

void Matrix::transpose()
{
    for (unsigned int i = 0; i < size_matrix; ++i)
        for (unsigned int j = 0; j < size_matrix; ++j)
            if (j > i)
	        {
                double tmp = matrix[i][j];
                matrix[i][j] = matrix[j][i];
                matrix[j][i] = tmp;
            }
}

void Matrix::inverse()
{
    double determ = get_determ();
    
    if (determ == 0)
        return;
    
    double** inv_matrix = new double*[size_matrix];
    inv_matrix[0] = new double[size_matrix * size_matrix];
    for (unsigned int i = 0; i < size_matrix; ++i)
        inv_matrix[i] = inv_matrix[0] + size_matrix * i;
        
    for (unsigned int i = 0; i < size_matrix; ++i)
        for (unsigned int j = 0; j < size_matrix; ++j)
	    {
            double** sub_matrix = get_sub_matrix(matrix, size_matrix, j, i);
            inv_matrix[j][i] = proc_determ(sub_matrix, size_matrix - 1) * 
                ((((size_matrix * i + j) % 2) == 1) ? -1 : 1) / determ;
            delete [] sub_matrix[0];
            delete [] sub_matrix;
        }
    
    for (unsigned int i = 0; i < size_matrix; ++i)
        for (unsigned int j = 0; j < size_matrix; ++j)
            matrix[i][j] = inv_matrix[i][j];
    
    delete [] inv_matrix[0];
    delete [] inv_matrix;
}

double Matrix::get_determ()
{
    return proc_determ(matrix, size_matrix);
}

unsigned char Matrix::size()
{
    return size_matrix;
}

double* Matrix::operator[](unsigned int _row)
{
    if (0 <= _row || _row < size_matrix)
        return matrix[_row];
    return nullptr;
}
