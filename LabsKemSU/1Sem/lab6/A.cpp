#include <iostream>
#include "A.h"
using namespace std;
Apple* Init(Apple A)
{
  Apple* B = new Apple;
  *B = A;
  return B;
}
void Del(Apple* A)
{
  delete A;
}
void A_Print(Apple* A)
{
  cout << "____________________" << endl;
  cout << "Color: " << A->color << endl;
  cout << "Weight: " << A->weight << endl;
  cout << "Country: " << A->country << endl;
  cout << "Price: " << A->price << endl;
  cout << "____________________" << endl;
}
