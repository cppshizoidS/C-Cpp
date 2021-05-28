#include <iostream>		//lenth of string
#include <string>
using namespace std;

int main()
{
	string txt, a, b;
	cout << "Enter two srtings\n";
	cin >> a >> b;
	txt = a + b;
	cout << txt	<< "\n";
	cout << "The length of the string is: " << txt.length();
	return 0;
}
