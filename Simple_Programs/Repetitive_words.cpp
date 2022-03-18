#include <iostream>	//repetitive words
#include <string>
using namespace std;

int main()
{
	int number_of_words = 0;
	string previous = " ";	//"not a word";

	string current;
	while (cin >> current) {
		++number_of_words;
		if (previous == current)
			cout << "Repeat of word " << current
			<< "After " << number_of_words
			<< "words:" << '\n';
		previous = current;
	}
}
