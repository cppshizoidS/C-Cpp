#include <iostream>
#include <iomanip>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;
int GetN();
void NewMatr(double**& M, int n, int m);
void DelMatr(double**& M, int n, int m);
void PrintMatr(double** M, int n, int m, const char* namematr);
void PrintVect(double* x, int n, const char* namematr);
bool minor(double**& M, int n, int m, double &Det);
void reshuffle(double**& M, int n, int m, int k, double &Det);
bool Solve(double **&M, double *x, int n, int m, double &Det);
double det(double** M, int n, int m);
void test(double**& M, int n, int m);
void clone(double**& M, int n, int m, double** M1);
double check(double** M1, int n, int m, double *x);
bool reverb(double** A, int n, int m, double &Det, double**& rev);
void Mult(double** M1, double** M2, int n1, int m1, int n2, int m2);
int main()
{
  int n;
  double Det = 1;
  cout << "Enter size matrix: "; n = GetN();
  int m = n + 1;
  double **A;
  double **A1;
  double **rev;
  double *x = new double [n];
  NewMatr(A, n, m);
  test(A, n, m);
  NewMatr(A1, n, m);
  clone(A, n, m, A1);
  NewMatr(rev, n, n);
  PrintMatr(A, n, m, "A");
  if (Solve(A, x, n, m, Det))
  {
    PrintVect(x, n, "x");
    cout << "epsilon = " << check(A1, n, m, x) << endl << endl;
  }
  else cout << "no single solution" << endl;
  cout << "det = " << Det * det(A, n, m) << endl;
  if (reverb(A1, n, m, Det, rev))
  {
    PrintMatr(rev, n, n, "rev");
    Mult(A1, rev, n, m - 1, n, n);
  }
  else cout << "no reverb" << endl;
  DelMatr(A, n, m);
  DelMatr(A1, n, m);
  DelMatr(rev, n, n);
  delete [] x;
  x = NULL;
  return 0;
}
int GetN()
{
  float n;
  do
  {
    cout << "Enter (int) number: ";
    cin >> n;
    if (n <= 0 || (n - (int)n))
      cout << " ---Error, try again..." << endl;
  } while(n <= 0 || (n - (int)n));
  return n;
}
void NewMatr(double**& M, int n, int m)
{
  M = new double*[n];
  for(int i = 0; i < n; i++)
    M[i] = new double [m];
}
void DelMatr(double**& M, int n, int m)
{
  for(int i = 0; i < n; i++)
    delete [] M[i];
  delete [] M;
}
void PrintMatr(double** M, int n, int m, const char* namematr)
{
  cout << endl << " " << namematr << ":" << endl;
  for(int i = 0; i < n; i++)
    {
      for(int j = 0; j < m; j++)
        cout << setw(15) << M[i][j];
      cout << endl;
    }
    cout << endl;
}
void PrintVect(double* x, int n, const char* namematr)
{
  cout << endl << " " << namematr << ":" << endl;
  for(int i = 0; i < n; i++)
    {
      cout << setw(5) << x[i];
      cout << endl;
    }
    cout << endl;
}
void reshuffle(double**& M, int n, int m, int k, double &Det)
{
  for (int i = k + 1; i < n; i++)
  {
    if (M[i][k] != 0)
    {
      double temp;
      for (int i1 = 0; i1 < m; i1++)
      {
        temp = M[k][i1];
        M[k][i1] = M[i][i1];
        M[i][i1] = temp;
      }
      Det *= -1;
      break;
    }
  }
}
bool minor(double**& M, int n, int m, double &Det)
{
  for (int  i = 0; i < n ; i++)
  {
    if (M[i][i] == 0) reshuffle(M, n, m, i, Det);
    if (M[i][i] == 0) return false;
    for (int i1 = i + 1; i1 < n; i1++)
      for (int i2 = i + 1; i2 < m; i2++)
        M[i1][i2] = M[i][i] * M[i1][i2] - M[i][i2] * M[i1][i];
    for (int j = i + 1; j < n; j++)
    {
      M[j][i] = 0;
    }
  }
  return true;
}
bool Solve(double **&M, double *x, int n, int m, double &Det)
{
  double res = 0;
  if (!minor(M, n, m, Det)) return false;
  for(int i = n - 1; i >= 0; i--)
  {
    res = 0;
    for(int j = i + 1; j <= n - 1; j++)
      res = res - x[j] * M[i][j];
    res += M[i][n];
    x[i] = res / M[i][i];
  }
  return true;
}
double det(double** M, int n, int m)
{
  double det = 1;
  for(int i = 0; i < n; i++)
  {
    if (M[i][i] == 0) return 0;
    det *= M[i][i] / pow(M[i][i], n - 1 - i);
  }
  return det;
}
void test(double**& M, int n, int m)
{
  int k;
  cout << "Enter number test: "; cin >> k;
  switch (k)
  {
    case 1:
    {
      fstream A("data_array.txt", ios::in);
      for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
          A >> M[i][j];
      A.close();
      break;
    }
    case 2:
    {
      double a,b;
      srand(time(0));
      a = -10;
      b = 10;
      for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
          {
            M[i][j] = a + ((double) rand() / RAND_MAX) * (b - a);
          }
      break;
    }
    case 3:
    {
      for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
          if (i == j) M[i][j] = 1;
          else M[i][j] = 0;
      break;
    }
    case 4:
    {
      for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
          M[i][j] = 0;
      break;
    }
    case 5:
    {
      for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
          if (j == (m - 1)) M[i][j] = 1;
          else M[i][j] = 1. / (i + j + 1);
      break;
    }
  }
}
void clone(double**& M, int n, int m, double** M1)
{
  for(int i = 0; i < n; i++)
    for(int j = 0; j < m; j++)
      M1[i][j] = M[i][j];
}
double check(double** M1, int n, int m, double *x)
{
  double epsilon = 0;
  double s;
  for(int i = 0; i < n; i++)
  {
    s = 0;
    for(int k = 0; k < m - 1; k++)
      s += M1[i][k] * x[k];
    if (fabs(M1[i][m - 1] - s) > epsilon) epsilon = fabs(M1[i][m - 1] - s);
  }
  return epsilon;
}
bool reverb(double** M, int n, int m, double &Det, double**& rev)
{
  double *x = new double [n];
  bool metka = true;
  for(int i = 0; i < n; i++)
  {
    double** buff;
    NewMatr(buff, n, m);
    clone(M, n, m, buff);
    for(int j = 0; j < n; j++)
    {
      if (i == j) buff[j][m - 1] = 1;
      else buff[j][m - 1] = 0;
    }
    if (!Solve(buff, x, n, m, Det)) metka = false;
    for(int j = 0; j < n; j++)
      rev[j][i] = x[j];
    DelMatr(buff, n, m);
  }
  delete [] x;
  x = NULL;
  return metka;
}
void Mult(double** M1, double** M2, int n1, int m1, int n2, int m2)
{
  double** buff;
  NewMatr(buff, n1, m2);
  double S;
  if(m1 != n2)
  {
    cout << "error" << endl;
    DelMatr(buff, n1, m2);
  }
  for(int i = 0; i < n1; i++)
    for(int j = 0; j < m2; j++)
    {
      S = 0;
      for(int k = 0; k < m1; k++)
        S += M1[i][k] * M2[k][j];
      buff[i][j] = S;
    }
  PrintMatr(buff, n1, m2, "proverka");
  DelMatr(buff, n1, m2);
}
