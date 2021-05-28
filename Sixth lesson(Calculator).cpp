#include <iostream>
using namespace std;

int main()
{
	cout << "Write the expression:\n";
	cout << "Add x to the end of expression\n";
	int lval = 0;
	int rval;
	cin >> lval;

	if (!cin)
	{
		cout<< "No first operand ";
	}

	for (char op; cin >> op;) {
		if (op != 'x')
		{
			cin >> rval;
		}

		if (!cin) 
		{
			cout << "No second operand";
		}

		switch (op)
		{
			case '+':
				lval += rval;
				break;

			case '-':
				lval -= rval;
				break;

			case '*':
				lval *= rval;
				break;

			case '/':
				lval /= rval;

			default:
				cout << "Result: " << lval << '\n';
				return 0;
		}
	}
}