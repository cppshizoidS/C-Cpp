#include <iostream>
#include "A.h"
using namespace std;
#ifndef T_h
#define T_h
struct Tree
{
  Apple* data;
  Tree* left;
  Tree* right;
};
bool sort_str(string, string);
void Add_price(Tree*&, Apple*);
void Add_color(Tree*&, Apple*);
void Print(Tree*);
void Delete(Tree*&);
void Print_price(Tree*, float);
void Print_color(Tree*, char);
#endif
