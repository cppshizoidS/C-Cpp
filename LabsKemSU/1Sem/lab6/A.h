#include <iostream>
using namespace std;
#ifndef A_h
#define A_h
struct Apple
{
  string color;
  float weight;
  string country;
  float price;
};
Apple* Init(Apple);
void Del(Apple*);
void A_Print(Apple*);
#endif
