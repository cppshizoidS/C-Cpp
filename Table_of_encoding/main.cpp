#include <iostream>
#include <string>
#include <cstring>
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

    byte *src = (byte*)"Hello world!";
    byte *dst = new byte[strlen((char*)src)+1];
    memset(dst, 0, strlen((char*)src) + 1);
    doCode(src, dst,codeTable);
    cout<<"Source string: <" << src <<">\n"
        <<"Destination string: <" << dst <<">\n";
    return 0;
}
