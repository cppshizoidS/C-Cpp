#include <iostream>		//base math operations
using namespace std;
int main()
{
	int	a, b, sum, difference, product, quotient, x;
	cout << "Please, enter the numbers:\n";
	cin >> a >> b;
	sum = a + b;
	difference = a - b;
	product = a * b;
	quotient = a / b;
	x = a % b;
	cout << "Sum is: " << sum << "\n" << "Difference is: " << difference << "\n" << "Product is: " << product << "\n" << "Quotient is: " << quotient;
	return 0;
}
