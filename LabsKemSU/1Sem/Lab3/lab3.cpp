#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
struct phone
{
  string Model;
  int RAM;
  int Memory;
  int Battery;
};
struct MyStack
{
  struct Node
  {
    phone data;
    Node *prev;
  };
  Node *Top = NULL;
  int Count = 0;
  bool Push(phone);
  bool Pop(phone&);
  void Info();
};
bool MyStack::Push(phone dt)
{
  if(!Top)
  {
    Top = new Node;
    Top -> prev = NULL;
    Count = 1;
  }
  else
  {
    Node *temp;
    temp = new Node;
    temp -> prev = Top;
    Top = temp;
    Count++;
  }
  Top -> data = dt;
  return true;
}
bool MyStack::Pop(phone& dt)
{
  if(!Top) return false;
  Node *temp = Top -> prev;
  dt = Top -> data;
  delete Top;
  Top = temp;
  Count--;
  return true;
}
void MyStack::Info()
{
  if(!Top) cout << "Stack is empty" << endl;
  else
  {
    cout << endl << "Stack info: " << endl;
    cout << "\tStack size = " << Count << endl;
    cout <<"\tTop Model = " << Top -> data.Model << endl;
    cout <<"\tTop RAM = " << Top -> data.RAM << endl;
    cout <<"\tTop Memory = " << Top -> data.Memory << endl;
    cout <<"\tTop Battery = " << Top -> data.Battery << endl << endl;
  }
}
void print(MyStack &S, MyStack &V)
{
  phone dt;
  while(S.Count)
  {
    S.Pop(dt);
    cout <<"\tModel = " << dt.Model << endl;
    cout <<"\tRAM = " << dt.RAM << endl;
    cout <<"\tMemory = " << dt.Memory << endl;
    cout <<"\tBattery = " << dt.Battery << endl << endl;
    V.Push(dt);
  }
  while(V.Count)
  {
    V.Pop(dt);
    S.Push(dt);
  }
}
int main()
{
  MyStack S;
  MyStack V;
  phone dt;
  ifstream file("phone.txt");
  string line;
  while(getline(file, line))
  {
    istringstream line_F(line);
    line_F >> dt.Model >> dt.RAM >> dt.Memory >> dt.Battery;
    S.Push(dt);
  }
  file.close();
  int m = 1;
  while(m)
  {
    S.Info();
    cout << "1. Add product to basket" << endl;
    cout << "2. Pull an item from the basket" << endl;
    cout << "3. Clear the basket" << endl;
    cout << "4. Show basket contents" << endl;
    cout << "0. Exit" << endl;
    cin >> m;
    switch(m)
    {
      case 1:
      {
        system("cls");
        cout << "Enter product specifications:" << endl;
        cout << "Model = "; cin >> dt.Model;
        cout << "RAM = "; cin >> dt.RAM;
        cout << "Memory = "; cin >> dt.Memory;
        cout << "Battery = "; cin >> dt.Battery;
        S.Push(dt);
        system("cls");
        break;
      }
      case 2:
      {
        system("cls");
        phone dt_x;
        bool metka = false;
        cout << "Enter product specifications:" << endl;
        cout << "Model = "; cin >> dt_x.Model;
        cout << "RAM = "; cin >> dt_x.RAM;
        cout << "Memory = "; cin >> dt_x.Memory;
        cout << "Battery = "; cin >> dt_x.Battery;
        while(S.Count)
        {
          S.Pop(dt);
          if(dt.Model != dt_x.Model || dt.RAM != dt_x.RAM || dt.Memory != dt_x.Memory || dt.Battery != dt_x.Battery)
          {
            V.Push(dt);
          }
          else
          {
            metka = true;
            break;
          }
        }
        if(!metka) cout << "Product not found" << endl;
        while(V.Count)
        {
          V.Pop(dt);
          S.Push(dt);
        }
        system("pause");
        system("cls");
        break;
      }
      case 3:
      {
        system("cls");
        while(S.Count)
        {
          S.Pop(dt);
        }
        break;
      }
      case 4:
      {
        system("cls");
        print(S,V);
        system("pause");
        system("cls");
        break;
      }
    }
  }
}
