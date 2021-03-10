#include <iostream>		//base math operations
using namespace std;
int main()
{
	int	a, b, _sum, _difference, _product, _quotient, x;
	cout << "Please, enter the numbers:\n";
	cin >> a >> b;
	_sum = a + b;
	_difference = a - b;
	_product = a * b;
	_quotient = a / b;
	x = a % b;
	cout << "Sum is: " << _sum << "\n" << "Difference is: " << _difference << "\n" << "Product is: " << _product << "\n" << "Quotient is: " << _quotient;
	return 0;
}