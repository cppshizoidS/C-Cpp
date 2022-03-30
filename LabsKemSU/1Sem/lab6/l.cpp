#include <iostream>
#include <fstream>
#include <sstream>
#include "A.h"
#include "T.h"
using namespace std;
int main()
{
  Apple A;
  Tree* T = NULL;
  int m = 1;
  while(m)
  {
    ifstream file("l.txt");
    string line;
    system("cls");
    cout << "___________________________________________________" << endl;
    cout << "1. Sort by color" << endl;
    cout << "2. Sort by price" << endl;
    cout << "3. Show apples at a price below the specified price" << endl;
    cout << "4. Show apples whose color starts with your letter" << endl;
    cout << "0. Exit" << endl;
    cout << "___________________________________________________" << endl;
    cin >> m;
    switch(m)
    {
      case 1:
      {
        while(getline(file, line))
        {
          istringstream line_F(line);
          line_F >> A.color >> A.weight >> A.country >> A.price;
          Add_color(T, Init(A));
        }
        system("cls");
        Print(T);
        system("pause");
        break;
      }
      case 2:
      {
        while(getline(file, line))
        {
          istringstream line_F(line);
          line_F >> A.color >> A.weight >> A.country >> A.price;
          Add_price(T, Init(A));
        }
        system("cls");
        Print(T);
        system("pause");
        break;
      }
      case 3:
      {
        float price;
        cout << "Price = "; cin >> price;
        while(getline(file, line))
        {
          istringstream line_F(line);
          line_F >> A.color >> A.weight >> A.country >> A.price;
          Add_price(T, Init(A));
        }
        system("cls");
        Print_price(T, price);
        system("pause");
        break;
      }
      case 4:
      {
        char a;
        cout << "Letter = "; cin >> a;
        while(getline(file, line))
        {
          istringstream line_F(line);
          line_F >> A.color >> A.weight >> A.country >> A.price;
          Add_color(T, Init(A));
        }
        system("cls");
        Print_color(T, a);
        system("pause");
        break;
      }
    }
    Delete(T);
    file.close();
  }
  return 0;
}
