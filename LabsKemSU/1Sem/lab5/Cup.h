#include <iostream>
#include "DLList.h"
using namespace std;
#ifndef Cup_h
#define Cup_h
struct Cup
{
  double volume;
  double height;
  double diameter;
  string material;
};
void* Cup_init(Cup);
void Cup_del(void*);
void Cup_out(DLList);
void Cup_cin(Cup&);
#endif
