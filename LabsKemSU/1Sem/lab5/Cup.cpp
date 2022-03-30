#include <iostream>
#include "Cup.h"
using namespace std;
void* Cup_init(Cup B)
{
  Cup* A = new Cup;
  A->volume = B.volume;
  A->height = B.height;
  A->diameter = B.diameter;
  A->material = B.material;
  void* S = (void*) A;
  return S;
}
void Cup_del(void* S)
{
  delete (Cup*) S;
}
void Cup_out(DLList List)
{
  Cup A;
  List.MoveFirst();
  system("cls");
  if(!List.Count)
  {
    cout << "List is empty" << endl;
    return;
  }
  for(int i = 0; i < List.Count; i++)
  {
    A = *((Cup*) List.C->data);
    cout << "---------------------------------------" << endl;
    cout << "|Volume = " << A.volume << endl;
    cout << "|Height = " << A.height << endl;
    cout << "|Diameter = " << A.diameter << endl;
    cout << "|Material = " << A.material << endl;
    cout << "---------------------------------------" << endl;
    List.MoveNext();
  }
}
void Cup_cin(Cup& A)
{
  cout << "Enter your data:" << endl;
  cout << "Volume = "; cin >> A.volume;
  cout << "Height = "; cin >> A.height;
  cout << "Diameter = "; cin >> A.diameter;
  cout << "Material = "; cin >> A.material;
}
