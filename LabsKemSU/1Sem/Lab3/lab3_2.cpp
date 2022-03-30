#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
struct product_group
{
  int quantity;
  double price;
};
struct MyQueue
{
  struct Node
  {
    product_group data;
    Node* next;
  };
  Node* First = NULL;
  int Count = 0;
  double full_cost = 0;
  double income = 0;
  bool Push(product_group);
  bool Pop(product_group&);
  void Info();
};
bool MyQueue::Push(product_group dt)
{
  if(!First)
  {
    First = new Node;
    First -> next = NULL;
    First -> data = dt;
    Count = 1;
    full_cost += dt.price * dt.quantity;
  }
  else
  {
    Node *temp;
    temp = First;
    while(temp -> next != NULL) temp = temp -> next;
    temp -> next = new Node;
    temp -> next -> data = dt;
    temp -> next -> next = NULL;
    Count++;
    full_cost += dt.price * dt.quantity;
  }
  return true;
}
bool MyQueue::Pop(product_group& dt)
{
  if(!First) return false;
  Node* temp = First -> next;
  dt = First -> data;
  delete First;
  First = temp;
  Count--;
  return true;
}
void MyQueue::Info()
{
  if(!First) cout << "The warehouse is empty" << endl;
  else
  {
    cout << endl << "Information about the warehouse: " << endl;
    cout << "\tWarehouse size = " << Count << endl;
    cout << "\tFirst quantity = " << First -> data.quantity << endl;
    cout << "\tFirst price = " << First -> data.price << endl << endl;
  }
}
bool check(MyQueue Q, product_group dt_x)
{
  product_group dt;
  MyQueue::Node *a;
  a = Q.First;
  int k = 0;
  while(a -> data.price <= dt_x.price & k != Q.Count)
  {
    dt.price = a -> data.price;
    dt.quantity = a -> data.quantity;
    if(dt_x.quantity <= dt.quantity)
    {
      dt_x.quantity = 0;
    }
    else
    {
      dt_x.quantity -= dt.quantity;
    }
    if(dt_x.quantity == 0)
    {
      return true;
    }
    else
    {
      if(k < Q.Count)
      {
        k++;
        a = a -> next;
      }
      else return false;
    }
  }
  return false;
}
int main()
{
  MyQueue Q;
  product_group dt;
  product_group dt_x;
  ifstream file("warehouse.txt");
  string line;
  while(getline(file, line))
  {
    istringstream line_F(line);
    line_F >> dt.quantity >> dt.price;
    Q.Push(dt);
  }
  file.close();
  int m = 1;
  while(m)
  {
    Q.Info();
    cout << "1. Receipt of goods" << endl;
    cout << "2. Sale of goods" << endl;
    cout << "3. Report" << endl;
    cout << "0. Exit" << endl;
    cin >> m;
    switch (m)
    {
      case 1:
      {
        system("cls");
        cout << "Enter the price and quantity of the product:" << endl;
        cout << "Quantity = "; cin >> dt.quantity;
        cout << "Price = "; cin >> dt.price;
        Q.Push(dt);
        system("cls");
        break;
      }
      case 2:
      {
        system("cls");
        cout << "Enter the price and quantity of the product:" << endl;
        cout << "Quantity = "; cin >> dt_x.quantity;
        cout << "Price = "; cin >> dt_x.price;
        dt = Q.First -> data;
        if(check(Q, dt_x))
        {
          while(dt_x.quantity)
          {
            if(dt_x.quantity <= dt.quantity)
            {
              Q.income += dt_x.quantity * (dt_x.price - dt.price);
              Q.full_cost -= dt.price * dt_x.quantity;
              dt.quantity -= dt_x.quantity;
              dt_x.quantity = 0;
              Q.First -> data.quantity = dt.quantity;
            }
            else
            {
              Q.income += dt.quantity * (dt_x.price - dt.price);
              Q.full_cost -= dt.price * dt.quantity;
              dt_x.quantity -= dt.quantity;
              dt.quantity = 0;
            }
            if(dt.quantity == 0)
            {
              Q.Pop(dt);
              dt = Q.First -> data;
            }
          }
          cout << "Purchase completed :)" << endl;
        }
        else cout << "The purchase was not made :(" << endl;
        system("pause");
        system("cls");
        break;
      }
      case 3:
      {
        system("cls");
        cout << "Number of units = " << Q.Count << endl;
        cout << "Product price = " << Q.full_cost << endl;
        cout << "Income from sale = " << Q.income << endl;
        system("pause");
        system("cls");
        break;
      }
    }
  }
}
