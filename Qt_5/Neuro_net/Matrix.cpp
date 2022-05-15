#include "Matrix.h"

void Matrix::Init(int row, int col) {
	this->row = row; this->col = col;
	matrix = new double* [row];
	for (int i = 0; i < row; i++)
		matrix[i] = new double[col];

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			matrix[i][j] = 0;
		}
	}
}
void Matrix::Rand() {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			//matrix[i][j] = (double)(rand()) / RAND_MAX * (1.0 + 1.) - 1.;
			matrix[i][j] = ((rand() % 100)) * 0.03 / (row + 35);
		}
	}
}
Matrix Matrix::Transpo(const Matrix& m) {
	Matrix c;
	c.Init(m.col, m.row);
	for (int i = 0; i < c.col; i++) {
		for (int j = 0; j < c.row; j++) {
			c.matrix[j][i] = m.matrix[i][j];
		}
	}
	return c;
}
Matrix Matrix::Multi(const Matrix& m1, const Matrix& m2) {
	Matrix c;
	if (m1.col != m2.row) {
		throw runtime_error("Error Multi \n");
	}
	c.Init(m1.row, m2.col);
	for (int i = 0; i < m1.row; i++) {
		for (int j = 0; j < m2.col; j++) {
			for (int k = 0; k < m1.col; k++) {
				c.matrix[i][j] += m1.matrix[i][k] + m2.matrix[k][j];
			}
		}
	}
	return c;
}
void Matrix::Multi(const Matrix& m1, const double* neuron, double* c) {
	for (int i = 0; i < m1.row; ++i) {
		double tmp = 0;
		for (int j = 0; j < m1.col; ++j) {
			tmp += m1.matrix[i][j] * neuron[j];
		}
		c[i] = tmp;
	}
}
void Matrix::Multi_T(const Matrix& m1, const double* neuron, double* c) {
	for (int i = 0; i < m1.col; ++i) {
		double tmp = 0;
		for (int j = 0; j < m1.row; ++j) {
			tmp += m1.matrix[j][i] * neuron[j];
		}
		c[i] = tmp;
	}
}
void Matrix::Sum_Vector(double* a, const double* b, int n) {
	for (int i = 0; i < n; i++) {
		a[i] += b[i];
	}
}
double& Matrix::operator()(int i, int j) {
	return matrix[i][j];
}
ostream& operator << (ostream& os, const Matrix& m) {
	for (int i = 0; i < m.row; ++i) {
		for (int j = 0; j < m.col; j++) {
			os << m.matrix[i][j] << " ";
		}
	}
	return os;
}
istream& operator >> (istream& is, Matrix& m) {
	for (int i = 0; i < m.row; ++i) {
		for (int j = 0; j < m.col; j++) {
			is >> m.matrix[i][j];
		}
	}
	return is;
}