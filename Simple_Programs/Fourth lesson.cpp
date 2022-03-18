#include <iostream>		//constants
using namespace std;
int main()
{
	int const x = 10;
	int	y, z;
	cout << "Please, enter y and z:\n";
	cin >> y >> z;
	int S = x + y + z;
	cout << "Sum is = " << S;
	return 0;
}
