#include <iostream>
#include <vector>

using namespace std;

int Turn(int n) {
	int m = 0;
	while (n)
	{
		m = m * 10 + n % 10;
		n /= 10;
	}
	return m;
}


int main() {
	int num = 0;
	int a = 0;
	vector <int> myvec;
	
	cout << "Enter amount of numbers:";
	cin >> num;

	for (int i = 0; i < num; i++) {
		cout << endl << "Enter " << i+1 << " numb:";
		cin >> a;
		a = Turn(a);
		myvec.push_back(a);
		cout << endl;
	}
	for (int i = 0; i < num; i++) {
		cout << myvec[i] << endl;
	}
	

	return 0;
}
