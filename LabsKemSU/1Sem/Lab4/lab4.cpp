#include <iostream>
#include <string>
#include <fstream>
using namespace std;
struct laptop
{
  int memory;
  int RAM;
  string model;
  string color;
};
struct MyQueue
{
  struct Node
  {
    char* data;
    int nnn;
    Node* next;
  };
  Node* First = NULL;
  int Count = 0;
  bool Push(char*, int n);
  bool Pop(char*&, int& n);
};
bool MyQueue::Push(char* data, int n)
{

  if(!First)
  {
    First = new Node;
    First -> next = NULL;
    First -> data = new char[n];
    for(int i = 0; i < n; i++) (First -> data)[i] = data[i];
    Count = 1;
    First -> nnn = n;
  }
  else
  {
    Node *temp;
    temp = First;
    while(temp -> next != NULL) temp = temp -> next;
    temp -> next = new Node;
    temp -> next -> data = new char[n];
    for(int i = 0; i < n; i++) (temp -> next -> data)[i] = data[i];
    temp -> next -> nnn = n;
    temp -> next -> next = NULL;
    Count++;
  }
  return true;
}
bool MyQueue::Pop(char*& data, int& n)
{
  if(!First) return false;
  Node* temp = First -> next;
  n = First -> nnn;
  data = new char[n];
  for(int i = 0; i < First -> nnn; i++) data[i] = (First -> data)[i];
  delete[] (First -> data);
  delete First;
  First = temp;
  Count--;
  return true;
}
void GetSeria(char*& data, int &n, laptop A)
{
  size_t s1 = A.model.size();
  size_t s2 = A.color.size();
  int n1 = sizeof(int);
  int n2 = sizeof(int);
  int n3_size = sizeof(size_t);
  int n3 = s1;
  int n4_size = sizeof(size_t);
  int n4 = s2;
  n = n1 + n2 + n3_size + n3 + n4_size + n4;
  data = new char [n];
  char* d1 = reinterpret_cast<char*> (&A.memory);
  char* d2 = reinterpret_cast<char*> (&A.RAM);
  char* d3_size = reinterpret_cast<char*> (&s1);
  char* d3 = const_cast<char*> (A.model.c_str());
  char* d4_size = reinterpret_cast<char*> (&s2);
  char* d4 = const_cast<char*> (A.color.c_str());
  for(int i = 0; i < n1; i++) data[i] = d1[i];
  for(int i = 0; i < n2; i++) data[i + n1] = d2[i];
  for(int i = 0; i < n3_size; i++) data[i + n1 + n2] = d3_size[i];
  for(int i = 0; i < n3; i++) data[i + n1 + n2 + n3_size] = d3[i];
  for(int i = 0; i < n4_size; i++) data[i + n1 + n2 + n3_size + n3] = d4_size[i];
  for(int i = 0; i < n4; i++) data[i + n1 + n2 + n3_size + n3 + n4_size] = d4[i];
}
void GetDeSeria(char* data, int n, laptop& A)
{
  int n1, n2, n3_size, n3, n4_size, n4;
  n1 = sizeof(int);
  n2 = sizeof(int);
  n3_size = sizeof(size_t);
  size_t p1 = *reinterpret_cast<size_t*> (data + n1 + n2);
  n3 = p1;
  string s1(data + n1 + n2 + n3_size, p1);
  n4_size = sizeof(size_t);
  size_t p2 = *reinterpret_cast<size_t*> (data + n1 + n2 + n3_size + n3);
  n4 = p2;
  string s2(data + n1 + n2 + n3_size + n3 + n4_size, p2);
  A.memory = *reinterpret_cast<int*> (data);
  A.RAM = *reinterpret_cast<int*> (data + n1);
  A.model = s1;
  A.color = s2;
}
bool InputBinaryFile(MyQueue &Q)
{
  fstream f_in("out.dat", ios::in | ios::binary);
  if(!f_in)
  {
    return false;
  }
  laptop A;
  int n = 0;
  while(!f_in.eof())
  {
    if(f_in.read((char*) &n, sizeof(int)))
    {
      char* data = new char[n];
      f_in.read(data, n);
      GetDeSeria(data, n, A);
      delete[] data;
      GetSeria(data, n, A);
      Q.Push(data, n);
    }
  }
  f_in.close();
  return true;
}
int main()
{
  MyQueue Q;
  char* data;
  int n = 0;
  int m = 1;
  laptop A;
  InputBinaryFile(Q);
  while(m)
  {
    cout << "1. Add an item to the queue" << endl;
    cout << "2. Pull out an item" << endl;
    cout << "3. Clear the queue" << endl;
    cout << "0. Exit" << endl;
    cin >> m;
    switch(m)
    {
      case 1:
      {
        system("cls");
        cout << "Enter product specifications:" << endl;
        cout << "Memory = "; cin >> A.memory;
        cout << "RAM = "; cin >> A.RAM;
        cout << "Model = "; cin >> A.model;
        cout << "Color = "; cin >> A.color;
        GetSeria(data, n, A);
        Q.Push(data, n);
        system("cls");
        break;
      }
      case 2:
      {
        system("cls");
        laptop A_x;
        bool metka = false;
        cout << "Enter product specifications:" << endl;
        cout << "Memory = "; cin >> A_x.memory;
        cout << "RAM = "; cin >> A_x.RAM;
        cout << "Model = "; cin >> A_x.model;
        cout << "Color = "; cin >> A_x.color;
        int Y = Q.Count;
        for(int i = 0; i < Y; i++)
        {
          Q.Pop(data, n);
          GetDeSeria(data, n, A);
          delete[] data;
          if(A.memory != A_x.memory || A.RAM != A_x.RAM || A.model != A_x.model || A.color != A_x.color)
          {
            GetSeria(data, n, A);
            Q.Push(data, n);
            delete[] data;
          }
          else
          {
            metka = true;
            break;
          }
        }
        if(!metka) cout << "Product not found" << endl;
        system("pause");
        system("cls");
        break;
      }
      case 3:
      {
        while(Q.Count)
        {
          Q.Pop(data, n);
          GetDeSeria(data, n, A);
          delete[] data;
          cout << "Memory = " << A.memory << endl;
          cout << "RAM = " << A.RAM << endl;
          cout << "Model = " << A.model << endl;
          cout << "Color = " << A.color << endl << endl;
        }
        system("pause");
        system("cls");
        break;
      }
    }
  }
while(Q.Count)
{
  Q.Pop(data, n);
  delete[] data;
}
}
