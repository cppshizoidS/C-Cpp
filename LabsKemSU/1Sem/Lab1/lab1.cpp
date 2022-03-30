#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
void fillarr(double* arr, int n);
void printarr(double* arr, int n);
double* createarr(int n);
void reassign(double*& arr, int &n, int k, double number);
void del(double*& arr, int &n, int k);
void zd7(double*& arr, int &n, double number, double x);
void zd8(double*& arr, int &n, double x);
int check_n();
void number_menu(double &number);
using namespace std;
int main()
{
  int n = check_n();
  if (!n)
  {
    cout << "the file data is incorrect" << endl;
    return 0;
  }
  double* arr = createarr(n);
  fillarr(arr, n);
  double number;
  int m = 1;
  while (m)
  {
    system("cls");
    cout << "Array: ";
    printarr(arr, n);
    cout << "Enter the task number (1-8)(0 - exit): "; cin >> m;
    switch (m)
    {
      case 1:
      {
        number_menu(number);
        reassign(arr, n, n + 1, number);
        break;
      }
      case 2:
      {
        number_menu(number);
        reassign(arr, n, 1, number);
        break;
      }
      case 3:
      {
        unsigned int k;
        cout << "Enter the position of the element: "; cin >> k;
        number_menu(number);
        reassign(arr, n, k, number);
        break;
      }
      case 4:
      {
        del(arr, n, n);
        break;
      }
      case 5:
      {
        del(arr, n, 1);
        break;
      }
      case 6:
      {
        unsigned int k;
        cout << "Enter the position of the element: "; cin >> k;
        del(arr, n, k);
        break;
      }
      case 7:
      {
        double x;
        cout << "Enter the element after which you want to add the element: "; cin >> x;
        number_menu(number);
        zd7(arr, n, number, x);
        system("pause");
        break;
      }
      case 8:
      {
        double x;
        cout << "Enter the element you want to delete: "; cin >> x;
        zd8(arr, n, x);
        system("pause");
        break;
      }
    }
  }
  delete[] arr;
}
void number_menu(double &number)
{
  int m = 1;
  double a = -100;
  double b = 100;
  cout << "What number will we add?" << endl;
  cout << "1 -- random number" << endl;
  cout << "2 -- the number you entered" << endl;
  cin >> m;
  switch (m)
  {
    case 1:
    {
      srand(time(0));
      number = a + (b - a) * (double) rand() /  RAND_MAX;
      m = 0;
      break;
    }
    case 2:
    {
      cout << "Enter your number: ";
      cin >> number;
      m = 0;
      break;
    }
    default:
    {
      cout << "Error, try again..." << endl;
      break;
    }
  }
}
void fillarr(double* arr, int n)
{
  fstream A("data_array.txt", ios::in);
  for (int i = 0; i < n; i++)
  {
    A >> arr[i];
  }
  A.close();
}
void printarr(double* arr, int n)
{
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
}
double* createarr(int n)
{
    double* arr = new double[n];
    return arr;
}
void reassign(double*& arr, int &n, int k, double number)
{
    k--;
    if (k >= 0 && k <= n)
    {
        double* buff = new double[n + 1];
        for (int i = 0; i < k; i++)
            buff[i] = arr[i];
        buff[k] = number;
        n++;
        for (int i = k + 1; i < n; i++)
            buff[i] = arr[i - 1];
        delete[] arr;
        arr = buff;
    }
    else cout << "error";
}
void del(double*& arr, int &n, int k)
{
    k--;
    if (k >= 0 && k <= n)
    {
        double* buff = new double[n - 1];
        for (int i = 0; i < k; i++)
            buff[i] = arr[i];
        n--;
        for (int i = k; i < n; i++)
            buff[i] = arr[i + 1];
        delete[] arr;
        arr = buff;
    }
    else cout << "error";
}
void zd7(double*& arr, int &n, double number, double x)
{
    int k = 0;
    for (int i = 0; i < n; i++)
    {
      if (arr[i] == x)
      {
        k = i + 1;
        break;
      }
    }
    if (k > 0)
    {
      double* buff = new double[n + 1];
      for (int i = 0; i < k; i++)
      {
        buff[i] = arr[i];
      }
      n++;
      buff[k] = number;
      for (int i = k + 1; i < n; i++)
      {
        buff[i] = arr[i - 1];
      }
      delete[] arr;
      arr = buff;
    }
    else cout << "element not found" << endl;
}
void zd8(double*& arr, int &n, double x)
{
  int k = 0;
  for (int i = 0; i < n; i++)
  {
    if (arr[i] == x)
    {
      k++;
    }
  }
  if (k > 0)
  {
    k = 0;
    double* buff = new double[n - k];
    for (int i = 0; i < n; i++)
    {
      if (arr[i] == x)
      {
        k++;
      }
      else buff[i - k] = arr[i];
    }
    n -= k;
    delete[] arr;
    arr = buff;
  }
  else cout << "element not found" << endl;
}
int check_n()
{
  double a;
  int k = 0;
  fstream A("data_array.txt", ios::in);
  while (A >> a)
  {
    k++;
  }
  A.close();
  return k;
}
