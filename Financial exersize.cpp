#include <iostream>		//financial exercize Ïîëüçîâàòåëü ââîäèò êîëè÷åñòâî äíåé, óêàçûâàåò ïðîöåíò ñêèäêè è ââîäèò ñóììó. Ðàññ÷èòàòü ïðèáûëü, åñëè çà êàæäûé äåíü ñóììà óâåëè÷èâàåòñÿ íà 3 $  è çàòåì ïðèìåíÿåòñÿ ñêèäêà, òî åñòü èòîãîâàÿ ñóììà åùå óâåëè÷èâàåòñÿ íà äàííîå ÷èñëî ïðîöåíòîâ.
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
