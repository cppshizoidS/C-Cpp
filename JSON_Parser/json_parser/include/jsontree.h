#ifndef JSONTREE_H
#define JSONTREE_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stack>


using namespace std;


enum JsonItemType {
    NUM,
    STR,
    OBJ,
    ARR,
    BOOL,
    _NULL,
    NONE,
};

struct JsonItem {
    string name;
    JsonItemType key_type;
    JsonItemType value_type;
};


class JsonTree
{
public:
    JsonTree();

    JsonTree(const JsonItem& key);

    virtual ~JsonTree();

    auto addItem(const JsonItem& key) -> void;

    auto getItem() -> JsonItem&;

    auto getItems() -> vector<JsonTree>&;

    auto extractPath() -> string;

    auto findAllItems(const string& key) -> vector<JsonTree*>;

    auto findItem(const string& key) -> JsonTree*;

    auto printTree(int depth = 0) -> void;

private:
    JsonItem item;
    JsonTree *parent = NULL;


    vector<JsonTree> items;
};

#endif // JSONTREE_H
