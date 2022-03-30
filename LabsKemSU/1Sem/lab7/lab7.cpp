#include <iostream>
#include <fstream>
#include <stack>
using namespace std;
int** Read(string, int&, int&);
void Delete(int**&, int);
int* dijkstra(int**, int, int);
void dijkstra_route(int**, int, int, int, int*, stack <int>&);
int main()
{
  ofstream out;
  out.open("out.txt");
  int n, m;
  stack <int> route;
  int** A = Read("data.txt", n, m);
  if(A == NULL | n != m)
  {
    cout << "error";
    out << "error";
    return 0;
  }
  int s;
  cout << "Start vertex: "; cin >> s;
  s--;
  int* d = dijkstra(A, n, s);
  for(int i = 0; i < n; i++)
  {
    cout << s + 1 << " -> " << i + 1 << " = " << d[i] << endl;
    out << s + 1 << " -> " << i + 1 << " = " << d[i] << endl;
  }
  int end;
  cout << "Final vertex: "; cin >> end;
  end--;
  dijkstra_route(A, n, s, end, d, route);
  cout << "Route: " << endl;
  out << "Route: " << endl;
  while(!route.empty())
  {
    cout << " -> " << route.top();
    out << " -> " << route.top();
    route.pop();
  }
  out.close();
  Delete(A, n);
  delete[] d;
  return 0;
}
int** Read(string file_name, int& n, int& m)
{
  ifstream in(file_name);
  if(in.is_open())
  {
    int count = 0;
    int temp;
    while(!in.eof())
    {
      in >> temp;
      count++;
    }
    in.seekg(0, ios::beg);
    in.clear();
    int count_space = 0;
    char symbol;
    while(!in.eof())
    {
      in.get(symbol);
      if(symbol == ' ') count_space++;
      if(symbol == '\n') break;
    }
    in.seekg(0, ios::beg);
    in.clear();
    n = count / (count_space + 1);
    m = count_space + 1;
    int **x;
    x = new int*[n];
    for(int i = 0; i < n; i++) x[i] = new int[m];
    for(int i = 0; i < n; i++)
    {
      for(int j = 0; j < m; j++)
      {
        in >> x[i][j];
      }
    }
    in.close();
    return x;
  }
  in.close();
  return NULL;
}
void Delete(int**& x, int n)
{
  for (int i = 0; i < n; i++) delete[] x[i];
  delete[] x;
}
int* dijkstra(int** A, int n, int s)
{
  const int inf = INT_MAX;
  int* d = new int[n];
  bool* v = new bool[n];
  int index, u;
  for(int i = 0; i < n; i++)
  {
    d[i] = inf;
    v[i] = false;
  }
  d[s] = 0;
  for(int count = 0; count < n - 1; count++)
  {
    int min = inf;
    for(int i = 0; i < n; i++)
    if(!v[i] && d[i] <= min)
    {
      min = d[i];
      index = i;
    }
    u = index;
    v[u] = true;
    for(int i = 0; i < n; i++)
      if(!v[i] && A[u][i] && d[u] != inf && d[u] + A[u][i] < d[i]) d[i] = d[u] + A[u][i];
  }
  delete[] v;
  return d;
}
void dijkstra_route(int** A, int n, int s, int end, int* d, stack <int>& route)
{
  route.push(end + 1);
  int weight = d[end];
  while (end != s)
  {
    for (int i = 0; i < n; i++)
      if (A[i][end] != 0)
      {
        int temp = weight - A[i][end];
        if (temp == d[i])
        {
          weight = temp;
          end = i;
          route.push(i + 1);
        }
      }
  }
}
