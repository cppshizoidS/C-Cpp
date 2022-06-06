#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector <int> atbash(vector<int> text) {//должен быть текст из ascii 97-122. это латинский алфавит 
	vector<int> alphabet,c_alphabet,cipher;

	int counter = 97;
	for (int i = 0; i <= 25; i++) {
		alphabet.push_back(counter);
		c_alphabet.push_back(counter);
		counter++;
	}

	reverse(c_alphabet.begin(), c_alphabet.end());
	
	for (int i = 0; i < text.size(); i++) {
		for (int j = 0; j < alphabet.size(); j++) {
			if (text[i] == alphabet[j]) {
				cipher.push_back(c_alphabet[j]);
			}
		}
	}
	return cipher;
}

vector <int> de_atbash(vector<int> cipher) {//there should also be a text from ascii 97-122. this is the Latin alphabet 
	vector<int> alphabet, c_alphabet, de_cipher;

	int counter = 97;
	for (int i = 0; i <= 25; i++) {
		alphabet.push_back(counter);
		c_alphabet.push_back(counter);
		counter++;
	}

	reverse(c_alphabet.begin(), c_alphabet.end());

	for (int i = 0; i < cipher.size(); i++) {		  
		for (int j = 0; j < c_alphabet.size(); j++) {	
			if (cipher[i] == c_alphabet[j]) {
				de_cipher.push_back(alphabet[j]);
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


int main() {

	//checking. everything works
	vector<int> text = { 98, 97, 100, 98, 111, 121 };
	vector<int> cipher,de_cipher;

	show_vector(text);

	cout << endl;

	cipher = atbash(text);
	show_vector(cipher);
	
	cout << endl;

	de_cipher = de_atbash(cipher);
	show_vector(de_cipher);



	return 0;
}
