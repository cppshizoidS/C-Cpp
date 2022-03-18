#include <iostream>		//base math operations
using namespace std;
int main()
{
	cout << "Please, enter the numbers:\n";
	int a, b;
	cin >> a >> b;
	int sum = a + b;
	int difference = a - b;
	int product = a * b;
	int quotient = a / b;
	int x = a % b;
	cout << "Sum is: " << sum << "\n" 
	     << "Difference is: " << difference << "\n" 
	     << "Product is: " << product << "\n" 
	     << "Quotient is: " << quotient;
	return 0;
}
