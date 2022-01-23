#include <iostream> //structs first practise
using namespace std;

int main() {
	struct First_Struct {
		int id;
		int age;
		double salary;
	};

	First_Struct alex;
		alex.id = 8;
		alex.age = 18;
		alex.salary = 65.5;

	First_Struct nicka;
		nicka.id = 9;
		nicka.age = 18;
		nicka.salary = 40.5;
	
	int totalAge = alex.age + nicka.age;
	if (alex.salary > nicka.salary)
		cout << "Alex makes more than Nicka\n";
	else if (alex.salary < nicka.salary)
		cout << "Alex makes less than Nicka\n";
	else
		cout << "Alex and Nicka make the same amount\n";

	//up for salary for Alex
	alex.salary += 5.5;
	//up salary for Nicka

	nicka.salary += 5.5;

	++nicka.age;
	++alex.age;

	int a = alex.age;
	int b = nicka.age;


	cout << a << b;
	return 0;
}