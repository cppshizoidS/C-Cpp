#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <stack>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iostream>

#include "symbols.h"
#include "jsontree.h"


using namespace std;


class Rule
{
public:
    Rule(int ruleNum);

    auto getString() -> string;

private:
    int num;
};


class JsonParser
{
public:
    JsonTree tree;


    JsonParser(const string& path);

    ~JsonParser();

    auto parse() -> vector<Rule>;

    auto getTree() -> const JsonTree*;

private:
    stack<JsonTree*> nodes;
    stack<Symbol> symbol_stack;
    map<Symbol, map<Symbol, int>> table;

    ifstream src;


    auto lexer(const char sym) -> Symbol;

    auto getRuleByNum() -> void;
};

#endif // JSONPARSER_H
