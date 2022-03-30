#include <iostream>
#include <fstream>
#include <sstream>
#include "DLList.h"
#include "Cup.h"
using namespace std;
int main()
{
  DLList List;
  void* S;
  Cup A;
  int m = 1;
  ifstream file("lab5.txt");
  string line;
  while(getline(file, line))
  {
    istringstream line_F(line);
    line_F >> A.volume >> A.height >> A.diameter >> A.material;
    S = Cup_init(A);
    List.AddNext(S);
  }
  file.close();
  while(m)
  {
    system("cls");
    cout << "---------------------------------------" << endl;
    cout << "|Add element:                         |" << endl;
    cout << "|1 in the beginning                   |" << endl;
    cout << "|2 in the end                         |" << endl;
    cout << "|3 in the k-position                  |" << endl;
    cout << "---------------------------------------" << endl;
    cout << "|Delete element:                      |" << endl;
    cout << "|4 the first                          |" << endl;
    cout << "|5 the last                           |" << endl;
    cout << "|6 the k position                     |" << endl;
    cout << "---------------------------------------" << endl;
    cout << "|7 Sort by \"volume\"                   |" << endl;
    cout << "---------------------------------------" << endl;
    cout << "|8 Clear the list                     |" << endl;
    cout << "---------------------------------------" << endl;
    cout << "|9 Upload data from the list to a file|" << endl;
    cout << "---------------------------------------" << endl;
    cout << "|10 Show the list                     |" << endl;
    cout << "---------------------------------------" << endl;
    cout << "|0 Exit                               |" << endl;
    cout << "---------------------------------------" << endl;
    cin >> m;
    switch(m)
    {
      case 1:
      {
        system("cls");
        Cup_cin(A);
        S = Cup_init(A);
        List.AddFirst(S);
        Cup_out(List);
        system("pause");
        break;
      }
      case 2:
      {
        system("cls");
        Cup_cin(A);
        S = Cup_init(A);
        List.AddLast(S);
        Cup_out(List);
        system("pause");
        break;
      }
      case 3:
      {
        system("cls");
        int k;
        cout << "Enter the position number from " << 1 << " to " << List.Count + 1 << endl;
        cout << "k = "; cin >> k;
        k--;
        if(k >= 0 & k <= List.Count)
        {
          Cup_cin(A);
          S = Cup_init(A);
          if(k == 0)
          {
            List.AddFirst(S);
          }
          else
          {
            if(k == List.Count)
            {
              List.AddLast(S);
            }
            else
            {
              List.Move_k(k - 1);
              List.AddNext(S);
            }
          }
          Cup_out(List);
        }
        else cout << "error" << endl;
        system("pause");
        break;
      }
      case 4:
      {
        system("cls");
        if(List.DelFirst(S))
        {
          Cup_del(S);
          Cup_out(List);
        }
        else
        {
          cout << "error" << endl;
        }
        system("pause");
        break;
      }
      case 5:
      {
        system("cls");
        if(List.DelLast(S))
        {
          Cup_del(S);
          Cup_out(List);
        }
        else
        {
          cout << "error" << endl;
        }
        system("pause");
        break;
      }
      case 6:
      {
        system("cls");
        int k;
        if(!List.Count)
        {
          cout << "error" << endl;
          system("pause");
          break;
        }
        cout << "Enter the position number from " << 1 << " to " << List.Count << endl;
        cout << "k = "; cin >> k;
        k--;
        if(k >= 0 & k <= List.Count)
        {
          if(k == 0)
          {
            List.DelFirst(S);
          }
          else
          {
            if(k == (List.Count - 1))
            {
              List.DelLast(S);
            }
            else
            {
              List.Move_k(k);
              List.Del(S);
            }
          }
          Cup_out(List);
        }
        else cout << "error" << endl;
        system("pause");
        break;
      }
      case 7:
      {
        system("cls");
        void* tmp;
        for(int i = 0; i < List.Count; i++)
        {
          List.MoveLast();
          for(int j = (List.Count - 1); j >= (i + 1); j--)
          {
            if(((Cup*) List.C->data)->volume < ((Cup*) List.C->prev->data)->volume)
            {
              tmp = List.C->data;
              List.C->data = List.C->prev->data;
              List.C->prev->data = tmp;
            }
            List.MovePrev();
          }
        }
        Cup_out(List);
        system("pause");
        break;
      }
      case 8:
      {
        List.MoveFirst();
        while(List.Del(S))
        {
          Cup_del(S);
        }
        Cup_out(List);
        system("pause");
        break;
      }
      case 9:
      {
        ofstream file("lab5.txt");
        List.MoveFirst();
        for(int i = 0; i < List.Count; i++)
        {
          A = *((Cup*) List.C->data);
          file << A.volume << " "  << A.height << " "<< A.diameter << " " << A.material << endl;
          List.MoveNext();
        }
        file.close();
        cout << "Uploaded!!!" << endl;
        system("pause");
        break;
      }
      case 10:
      {
        system("cls");;
        Cup_out(List);
        system("pause");
        break;
      }
    }
  }
  List.MoveFirst();
  while(List.Del(S))
  {
    Cup_del(S);
  }
  system("pause");
  return 0;
}
