#include "ActivateFunction.h"
void activateFunction::set() {
	actFunc = ReLU;
	//std::cout << "Set actFunc pls\n1 - sigmoid \n2 - ReLU \n3 - th(x) \n";
	//int tmp;
	//std::cin >> tmp;
	//switch (tmp)
	//{
	//case sigmoid:
	//	actFunc = sigmoid;
	//	break;
	//case ReLU:
	//	actFunc = ReLU;
	//	break;
	//case thx:
	//	actFunc = thx;
	//	break;
	//default:
	//	throw std::runtime_error("Error acrFunc");
	//	break;
	//}
}
void activateFunction::use(double* value, int n) {
	//int n = value.size();
	switch (actFunc)
	{
	case activateFunc::sigmoid:
		for (int i = 0; i < n; i++)
			value[i] = 1 / (1 + exp(-value[i]));
		break;
	case activateFunc::ReLU:
		for (int i = 0; i < n; i++) {
			if (value[i] < 0) value[i] *= 0.01;
			else if (value[i] > 1) value[i] = 1. + 0.01 * (value[i] - 1.);
		}
		//else value = value;
		break;

	case activateFunc::thx:
		for (int i = 0; i < n; i++) {
			if (value[i] < 0) value[i] = 0.01 * (exp(value[i]) - exp(-value[i])) / (exp(value[i]) + exp(-value[i]));
			else value[i] = (exp(value[i]) - exp(-value[i])) / (exp(value[i]) + exp(-value[i]));
		}
		break;
	default:
		throw std::runtime_error("Error actFunc \n");
		break;
	}
	//return value;
}
double activateFunction::use(double value) {
	switch (actFunc)
	{
	case activateFunc::sigmoid:
		value = 1 / (1 + exp(-value));
		break;
	case activateFunc::ReLU:
		if (value < 0) value *= 0.01;
		else if (value > 1) value = 1. + 0.01 * (value - 1.);
		//else value = value;
		break;

	case activateFunc::thx:
		if (value < 0) value = 0.01 * (exp(value) - exp(-value)) / (exp(value) + exp(-value));
		else value = (exp(value) - exp(-value)) / (exp(value) + exp(-value));
		break;

	default:
		throw std::runtime_error("Error actFunc \n");
		break;
	}
	return value;
}
void activateFunction::usePro(double* value, int n) {
	//int n = value.size();
	switch (actFunc)
	{
	case activateFunc::sigmoid:
		for (int i = 0; i < n; i++)
			value[i] = value[i] * (1 - value[i]);
		break;
	case activateFunc::ReLU:
		for (int i = 0; i < n; i++) {
			if (value[i] < 0)
				value[i] = 0.01;
			else if (value[i] > 1)
				value[i] = 0.01;
			else
				value[i] = 1;
		}
		break;
	case activateFunc::thx:
		for (int i = 0; i < n; i++) {
			if (value[i] < 0)
				value[i] = 0.01 * (1 - value[i] * value[i]);
			else
				value[i] = 1 - value[i] * value[i];
		}
		break;
	default:
		throw std::runtime_error("Error actFuncPro \n");
		break;
	}
	//return value;
}

double activateFunction::usePro(double value) {
	switch (actFunc)
	{
	case activateFunc::sigmoid:
		//if (fabs(value) < 1e-9 || fabs(value - 1) < 1e-9) return 0;
		return value * (1 - value);
		break;
	case activateFunc::ReLU:
		if (value < 0) return 0.01;
		else if (value > 1) return 0.01;
		else return 1;
		break;
	case activateFunc::thx:
		if (value < 0) return 0.01 * (1 - value * value);
		else return 1 - value * value;
		break;
	default:
		throw std::runtime_error("Error actFuncPro \n");
		break;
	}
}