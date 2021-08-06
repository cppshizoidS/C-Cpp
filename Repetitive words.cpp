#include <iostream>
#include <string>
using namespace std;

int main()
{
	string previous = " ";		//"not a word"

	string current;
	while (cin >> current) {
		if (previous == current)
			cout << "Repetitive word: " << current << endl;
		previous = current;
	}
}