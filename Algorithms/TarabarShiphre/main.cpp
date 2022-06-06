#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector <int> tarabar(vector<int> text) {//geting vector
	vector<int> alphabet1, alphabet2, cipher; //2 line of alphabet

	int counter = 97;
	for (int i = 0; i <= 25; i++) {
		if (i < 13) {
			alphabet1.push_back(counter);// first half is normal
			counter++;
		}
		else
		{
			alphabet2.push_back(counter);//we will reverse it later
			counter++;
		}
	}
	reverse(alphabet2.begin(), alphabet2.end()); //reverse second part of the alphabet
	
	for (int i = 0; i < text.size(); i++) {
		for (int j = 0; j < alphabet1.size(); j++) {//we fall on the first part of the alphabet
			if (text[i] == alphabet1[j]) {	    // if we find the letter
				cipher.push_back(alphabet2[j]);
			}
		}
		for (int j = 0; j < alphabet2.size(); j++) {//if did not find the letter have to go to the second part
			if (text[i] == alphabet2[j]) {		// if we find the letter
				cipher.push_back(alphabet1[j]);
			}
		}
	}
	
	return cipher;
}

vector <int> de_tarabar(vector<int> text) {//geting vector
	vector<int> alphabet1, alphabet2, de_cipher;//2 line of alphabet

	int counter = 97;
	for (int i = 0; i <= 25; i++) {
		if (i < 13) {
			alphabet1.push_back(counter);// first half is normal
			counter++;
		}
		else
		{
			alphabet2.push_back(counter);//we will reverse it later
			counter++;
		}
	}
	reverse(alphabet2.begin(), alphabet2.end());//reverse second part of the alphabet

	for (int i = 0; i < text.size(); i++) {
		for (int j = 0; j < alphabet2.size(); j++) {//we fall on the first part of the alphabet
			if (text[i] == alphabet2[j]) {      // if we find the letter
				de_cipher.push_back(alphabet1[j]);
			}
		}
		for (int j = 0; j < alphabet1.size(); j++) {//if did not find the letter have to go to the second par
			if (text[i] == alphabet1[j]) {	    // if we find the letter
				de_cipher.push_back(alphabet2[j]);
			}
		}
	}
	return de_cipher;
}

void show_vector(vector<int>& a)
{
	for (vector<int>::iterator it = a.begin(); it != a.end(); ++it)
		cout << *it << " ";
}

// we can delete main function and use it as .h

int main() {
	// just checking the program
	vector<int> a{ 98, 97, 100, 98, 111, 121 };
	vector<int> show = tarabar(a);
	show_vector(show);

	cout << endl;

	vector<int> de_show = de_tarabar(show);
	show_vector(de_show);

	return 0;
}
