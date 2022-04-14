#include <iostream>
#include <string>
#include "codetable.h"

using namespace std;

void doCode(byte* source, byte* dest, CodeTable & codeTable){
    for (int i = 0; source[i]; i++){
        dest[i] = codeTable[source[i]];
    }
}

int main(){
    CodeTable codeTable(32, 255);

    for(int i = 0; i < 13; i++){
        byte b = codeTable['a' + i];
        codeTable['a' + i] = codeTable['z' - i];
        codeTable['z' - i] =b;
    }


}
