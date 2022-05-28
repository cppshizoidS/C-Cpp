#ifndef TM1_LEXEME_H
#define TM1_LEXEME_H

#include <string>

using namespace std;

enum TYPE {UNDEFINED, INT};

struct lexeme {
    string name;
    TYPE type;
    // false - не инициализирован
    // true - инициализирован
    bool init;

    lexeme(string _name, TYPE _type, bool _init) : name(_name), type(_type), init(_init) {}
};

#endif //TM1_LEXEME_H
