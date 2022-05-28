#ifndef TM1_TABLE_VAR_H
#define TM1_TABLE_VAR_H

#include "lexeme.h"
#include <string>
#include <set>
#include <vector>

using namespace std;

class table_var {
private:
    vector<vector<lexeme>> table;
public:
    explicit table_var(int _size = 53) {
        table.resize(_size);
    }

    ~table_var() {
        table.clear();
    }

    // структура позиции элемента в таблице
    struct place {
        int i, j;
        place(int _i, int _j) : i(_i), j(_j) {}
    };

    // функция, возвращающая хэш элемента
    int get_hash(string name) {
        int hash = 0;
        for (char i: name) {
            hash += i;
        }
        return hash % table.size();
    }

    // функция, проверяющая есть ли лексема в таблице
    bool contains(string name) {
        int hash = get_hash(name);
        for (auto & i : table[hash]) {
            if (i.name == name) {
                return true;
            }
        }
        return false;
    }

    // функция, возвращающая позицию лексемы в таблице
    place find_place(string name) {
        int hash = get_hash(name);
        if (!contains(name)) {
            return {-1, -1};
        }
        for (int i = 0; i < table[hash].size(); i++) {
            if (table[hash][i].name == name) {
                return {hash, i};
            }
        }
    }

    // добавление лексемы в таблицу
    place add(string name) {
        if (contains(name)) return find_place(name);
        int hash = get_hash(name);
        lexeme _lexeme(name, UNDEFINED, false);
        table[hash].push_back(_lexeme);
        return find_place(name);
    }

    // задание типа по позиции лексемы
    void set_type(place _place, TYPE _type) {
        table[_place.i][_place.j].type = _type;
    }

    // задание init по позиции лексемы
    void set_init(place _place, bool _init) {
        table[_place.i][_place.j].init = _init;
    }

    // получение лексемы по позиции
    lexeme get_lexeme(place _place) {
        return table[_place.i][_place.j];
    }

};

#endif //TM1_TABLE_VAR_H
