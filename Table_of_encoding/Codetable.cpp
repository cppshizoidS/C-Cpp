#include <stdexcept>
#include <string>
#include "codetable.h"

using namespace std;

CodeTable::CodeTable(byte low, byte high, byte* iniTable){
    if ((hBound = high) < (lBound = low)) {
        throw out_of_range("Code Table constructor: lower bound is higher that upper one");
    }

}
