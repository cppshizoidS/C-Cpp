#include "NetWork.h"

void NetWork::Init(data_NetWork data) {
	actFunc.set();
	//srand(time(NULL));
	n = data.n;
	this->size = new int[n];
	for (int i = 0; i < n; i++)
		size[i] = data.size[i];
	weights_ = new Matrix[n - 1];
	bios = new double* [n - 1];
	for (int i = 0; i < n - 1; i++) {
		weights_[i].Init(size[i + 1], size[i]);
		bios[i] = new double[size[i + 1]];
		weights_[i].Rand();
		for (int j = 0; j < size[i + 1]; j++) {
			bios[i][j] = ((rand() % 50)) * 0.06 / (size[i] + 15);
		}
	}
	neurons_val = new double* [n]; neurons_err = new double* [n];
	for (int i = 0; i < n; i++) {
		neurons_val[i] = new double[size[i]]; neurons_err[i] = new double[size[i]];
	}
	neurons_bios_val = new double[n - 1]; neurons_bios_err = new double[n - 1];
}
void NetWork::SaveWeights() {
	ofstream fout;
	fout.open("Weights.txt");
	if (!fout.is_open()) {
		cout << "Error reading the file";
		system("pause");
	}
	for (int i = 0; i < n - 1; ++i)
		fout << weights_[i] << " ";

	for (int i = 0; i < n - 1; ++i) {
		for (int j = 0; j < size[i + 1]; ++j) {
			fout << bios[i][j] << " ";
		}
	}
	cout << "Weights saved \n";
	fout.close();
}
void NetWork::SetInput(double* values) {
	for (int i = 0; i < size[0]; i++) {
		neurons_val[0][i] = values[i];
	}
}
double NetWork::ForwardFeed() {
	for (int k = 1; k < n; ++k) {
		Matrix::Multi(weights_[k - 1], neurons_val[k - 1], neurons_val[k]);
		Matrix::Sum_Vector(neurons_val[k], bios[k - 1], size[k]);
		actFunc.use(neurons_val[k], size[k]);
	}
	int pred = SearchMaxIndex(neurons_val[n - 1]);
	return pred;
}
int NetWork::SearchMaxIndex(double* value) {
	double max = value[0];
	int prediction = 0;
	double tmp;
	for (int j = 1; j < size[n - 1]; j++) {
		tmp = value[j];
		if (tmp > max) {
			prediction = j;
			max = tmp;
		}
	}
	return prediction;
}

void NetWork::BackPropogation(double expect) {
	for (int i = 0; i < size[n - 1]; i++) {
		if (i != int(expect))
			neurons_err[n - 1][i] = -pow(neurons_val[n - 1][i], 2);
		else
			neurons_err[n - 1][i] = pow(1.0 - neurons_val[n - 1][i], 2);
	}
	for (int k = n - 2; k > 0; k--) {
		Matrix::Multi_T(weights_[k], neurons_err[k + 1], neurons_err[k]);
	}
	for (int i = n - 2; i >= 0; i--) {
		double err = 0;
		for (int j = 0; j < size[i + 1]; j++) {
			err += neurons_err[i + 1][j] * bios[i][j];
		}
		neurons_bios_err[i] = err;
	}
}
double NetWork::ErrorCounter() {
	double err = 0;
	for (int i = 0; i < size[n - 1]; i++) {
		err += neurons[n - 1][i].error;
	}
	return err;
}
void NetWork::WeightsUpdater(double lr) {
	for (int i = 0; i < n - 1; ++i) {
		for (int j = 0; j < size[i + 1]; ++j) {
			for (int k = 0; k < size[i]; ++k) {
				weights_[i](j, k) += neurons_err[i + 1][j] * actFunc.usePro(neurons_val[i + 1][j]) * neurons_val[i][k] * lr;
			}
		}
	}
	for (int i = 0; i < n - 1; i++) {
		for (int k = 0; k < size[i + 1]; k++) {
			bios[i][k] += neurons_err[i + 1][k] * actFunc.usePro(neurons_val[i + 1][k]) * lr;
		}
	}
}
void NetWork::ReadWeights() {
	ifstream fin;
	fin.open("Weights.txt");
	if (!fin.is_open()) {
		cout << "Error reading the file";
		system("pause");
	}
	for (int i = 0; i < n - 1; ++i) {
		fin >> weights_[i];
	}
	for (int i = 0; i < n - 1; ++i) {
		for (int j = 0; j < size[i + 1]; ++j) {
			fin >> bios[i][j];
		}
	}
	cout << "Weights readed \n";
	fin.close();
}
void NetWork::PrintConfig() {
	cout << "***********************************************************\n";
	cout << "NetWork has " << n << " layers\n";
	cout << "SIZE[]: ";
	for (int i = 0; i < n; i++) {
		cout << size[i] << " ";
	}
	cout << endl;
	cout << "***********************************************************\n";
	cout << endl;
}
void NetWork::PrintValues(int n) {
	for (int j = 0; j < size[n - 1]; j++) {
		cout << j << " " << neurons_val[n - 1][j] << endl;
	}
}