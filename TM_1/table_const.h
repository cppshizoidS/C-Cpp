#ifndef TM1_TABLE_CONST_H
#define TM1_TABLE_CONST_H

#include <fstream>
#include <string>
#include <set>

using namespace std;

class table_const
{
private:
    set<string> table;
public:
    // конструктор
    table_const() = default;

    // деструктор
    ~table_const() {
        table.clear();
    }

    // функция добавления элемента в таблицу
    void add(string elem) {
        table.insert(elem);
    }

    // заполнение таблицы из файла
    void read_file(string path) {
        ifstream fin(path);
        if (fin.is_open()) {
            string elem;
            while (!fin.eof()) {
                fin >> elem;
                add(elem);
            }
        }
        fin.close();
    }

    // функция, проверяющая есть ли элемент в таблице
    bool contains(string elem) {
        auto iterator = table.find(elem);
        if (iterator == table.end()) return false;
        return true;
    }

    // функция, возвращающая номер элемента или -1, если элемента нет
    int get_num(string elem) {
        if (!contains(elem)) return -1;
        return int(distance(table.begin(), table.find(elem)));
    }

    // функция, поиска значения элемента по номеру
    bool get_elem(int num, string &elem) {
        if (num < 0 || num >= table.size()) return false;
        auto iterator = table.begin();
        for (int i = 0; i < num; i++)
            iterator++;
        elem = *iterator;
        return true;
    }
};

#endif //TM1_TABLE_CONST_H
