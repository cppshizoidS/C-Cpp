#include <iostream>		//financial exercize Пользователь вводит количество дней, указывает процент скидки и вводит сумму. Рассчитать прибыль, если за каждый день сумма увеличивается на 3 $  и затем применяется скидка, то есть итоговая сумма еще увеличивается на данное число процентов.
using namespace std;

int main()
{
	cout << "Write count of days and persentage:\n";
	int _count_of_days = 0;
	int _percentage = 0;
	int _sum = 0;

	cin >> _count_of_days >> _percentage;


	for (int _count_of_days = 1; _count_of_days <= 30; _count_of_days++) {
		_sum += 3;
		_sum = _percentage * _sum;
	}
	cout << _count_of_days;
	return 0;
}