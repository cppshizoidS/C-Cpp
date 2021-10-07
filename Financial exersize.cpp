#include <iostream>
using namespace std;

int main()
{
	cout << "Write count of days and persentage:\n";
	int count_of_days = 0;
	int percentage = 0;
	int sum = 0;

	cin >> count_of_days >> percentage;


	for (int count_of_days = 1; count_of_days <= 30; count_of_days++) {
		sum += 3;
		sum = percentage * sum;
	}
	cout << count_of_days;
	return 0;
}
