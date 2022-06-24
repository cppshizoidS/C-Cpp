#include "../include/jsonparser.h"

typedef JsonParser JP;


JP::JsonParser(const string& path)
{
    src.open(path);

    // [expected symbol][catched symbol] = rule number
    table[NTS_OBJ][TS_OBJ_START]   = 1; // { PAIR }
    table[NTS_ARR][TS_ARR_START]   = 2; // [ VALUE ]
    table[NTS_VALUE][TS_OBJ_START] = 3; // OBJ
    table[NTS_VALUE][TS_ARR_START] = 4; // ARR
    table[NTS_VALUE][TS_MARK]      = 5; // " LTR "
    table[NTS_KEY][TS_MARK]        = 6; // " LTR "
    table[TS_OBJ_END][TS_NEXT]     = 7; // , PAIR
    table[TS_ARR_END][TS_NEXT]     = 8; // , VALUE
    table[NTS_PAIR][TS_MARK]       = 9; // KEY : VALUE
    table[NTS_VALUE][TS_DIGIT]     = 10;
    table[NTS_VALUE][TS_T]         = 11; // TRUE
    table[NTS_VALUE][TS_F]         = 12; // FALSE
    table[NTS_VALUE][TS_N]         = 13; // NULL

    tree.getItem().value_type = OBJ;
    nodes.push(&tree);
}

JP::~JsonParser()
{
    src.close();
}


Symbol
JP::lexer(const char sym)
{
    if (sym < ' ')
        return MOD_KEY;

    if ((sym >= '0') && (sym <= '9'))
        return TS_DIGIT;

    switch (sym)
    {
    case '{': return TS_OBJ_START;
    case '}': return TS_OBJ_END;
    case '[': return TS_ARR_START;
    case ']': return TS_ARR_END;
    case '"': return TS_MARK;
    case ':': return TS_CHAIN;
    case ',': return TS_NEXT;
    case '\\': return TS_BSLASH;
    case 't': return TS_T;
    case 'r': return TS_R;
    case 'u': return TS_U;
    case 'e': return TS_E;
    case 'f': return TS_F;
    case 'a': return TS_A;
    case 'l': return TS_L;
    case 's': return TS_S;
    case 'n': return TS_N;
    case ' ':
    case '\n':
    case '\t':
        return ESCAPE;
    default: return TS_UNKOWN;
    }
}


vector<Rule>
JP::parse()
{
    vector<Rule> rules;

    bool escape_mark = false;
    string line;
    int ltr;

    symbol_stack.push(TS_EOS);
    symbol_stack.push(NTS_OBJ);


    string key;
    JsonItemType type = NONE;


    while (!src.eof()) {
        ltr = 0;
        getline(src, line);

        while (!symbol_stack.empty() && ltr < line.size()) {
            Symbol sym = lexer(line[ltr]);

            // если в данный момент ожидается строка
            if (symbol_stack.top() == NTS_STR) {
                switch (sym) {
                case TS_MARK:
                    if (!escape_mark) {
                        nodes.top()->addItem({
                            .name = key,
                            .key_type = STR,
                            .value_type = NONE
                        });
                        key.clear();
                        symbol_stack.pop();
                        escape_mark = false;
                    }
                    break;
                case TS_BSLASH:
                    escape_mark = true;
                    break;
                default:
                    if (escape_mark)
                        escape_mark = false;
                    key += line[ltr];
                    break;
                }

                ltr++;
                continue;
            } else if (sym == ESCAPE) {
                ltr++;
                continue;
            }


            // если в данный момент ожидается число
            if (symbol_stack.top() == NTS_NUM) {
                switch (sym) {
                case TS_DIGIT:
                    key += line[ltr];
                    ltr++;
                    continue;
                case TS_NEXT:
                case TS_OBJ_END:
                case TS_ARR_END:
                    symbol_stack.pop();
                    nodes.top()->addItem({
                        .name = key,
                        .key_type = NUM,
                        .value_type = NONE
                    });
                    key.clear();
                    break;
                default:
                    cout << "unexcepted token: " << line[ltr] << endl;
                    return rules;
                }
            }

            if (symbol_stack.top() != NTS_NUM) {
                switch (sym) {
                case TS_OBJ_END:
                case TS_ARR_END:
                    nodes.pop();
                    break;
                default: break;
                }
            }


            if (sym == TS_UNKOWN) {
                cout << "unexpected token: " << line[ltr]
                     << "(" << (int)line[ltr] << ")" << endl;
                return rules;
            }


            if (sym == symbol_stack.top()) {
                ltr++;
                symbol_stack.pop();
            } else {
                int ruleNum = table[symbol_stack.top()][sym];

                if (ruleNum > 0)
                    rules.push_back(Rule(ruleNum));

                switch (ruleNum) {
                case 1: // [OBJ][OBJ START]
                    symbol_stack.pop();
                    symbol_stack.push(TS_OBJ_END);   // }
                    symbol_stack.push(NTS_PAIR);     // PAIR
                    symbol_stack.push(TS_OBJ_START); // {
                    break;
                case 2: // [ARR][ARR START]
                    symbol_stack.pop();
                    symbol_stack.push(TS_ARR_END);   // ]
                    symbol_stack.push(NTS_VALUE);    // VALUE
                    symbol_stack.push(TS_ARR_START); // [
                    break;
                case 3: // [VALUE][OBJ START]
                    symbol_stack.pop();
                    symbol_stack.push(NTS_OBJ); // OBJ

                    switch (nodes.top()->getItem().value_type) {
                    case OBJ:
                        nodes.push(&nodes.top()->getItems().back());
                        nodes.top()->getItem().value_type = OBJ;
                        break;
                    case ARR:
                        nodes.top()->addItem({
                            .name = "",
                            .key_type = ARR,
                            .value_type = OBJ
                        });
                        nodes.push(&nodes.top()->getItems().back());
                        break;
                    default: break;
                    }
                    break;
                case 4: // [VALUE][ARR START]
                    symbol_stack.pop();
                    symbol_stack.push(NTS_ARR); // ARR

                    switch (nodes.top()->getItem().value_type) {
                    case OBJ:
                        nodes.push(&nodes.top()->getItems().back());
                        nodes.top()->getItem().value_type = ARR;
                        break;
                    case ARR:
                        nodes.top()->addItem({
                            .name = "",
                            .key_type = ARR,
                            .value_type = ARR
                        });
                        nodes.push(&nodes.top()->getItems().back());
                        break;
                    default: break;
                    }
                    break;
                case 5: // [VALUE][MARK]
                    switch (nodes.top()->getItem().value_type) {
                    case OBJ:
                        nodes.push(&nodes.top()->getItems().back());
                        nodes.top()->getItem().value_type = STR;
                        break;
                    case ARR:
                        nodes.top()->addItem({
                            .name = "",
                            .key_type = ARR,
                            .value_type = STR
                        });
                        nodes.push(&nodes.top()->getItems().back());
                        break;
                    default: break;
                    }
                case 6: // [KEY][MARK]
                    symbol_stack.pop();
                    symbol_stack.push(NTS_STR); // STR
                    ltr++; // skip first "
                    break;
                case 7: // [OBJ END][NEXT]
                    symbol_stack.push(NTS_PAIR); // PAIR
                    symbol_stack.push(TS_NEXT);  // ,
                    nodes.pop();
                    break;
                case 8: // [ARR END][NEXT]
                    symbol_stack.push(NTS_VALUE); // VALUE
                    symbol_stack.push(TS_NEXT);   // ,
                    nodes.pop();
                    break;
                case 9: // [PAIR][MARK]
                    symbol_stack.pop();
                    symbol_stack.push(NTS_VALUE); // VALUE
                    symbol_stack.push(TS_CHAIN);  // :
                    symbol_stack.push(NTS_KEY);   // KEY
                    break;
                case 10: // [VALUE][DIGIT]
                    symbol_stack.pop();
                    symbol_stack.push(NTS_NUM); // NUM

                    switch (nodes.top()->getItem().value_type) {
                    case OBJ:
                        nodes.push(&nodes.top()->getItems().back());
                        nodes.top()->getItem().value_type = NUM;
                        break;
                    case ARR:
                        nodes.top()->addItem({
                            .name = "",
                            .key_type = ARR,
                            .value_type = NUM
                        });
                        nodes.push(&nodes.top()->getItems().back());
                        break;
                    default: break;
                    }
                    break;
                case 11: // [VALUE][TRUE]
                    symbol_stack.pop();
                    symbol_stack.push(TS_E);
                    symbol_stack.push(TS_U);
                    symbol_stack.push(TS_R);
                    symbol_stack.push(TS_T);

                    switch (nodes.top()->getItem().value_type) {
                    case OBJ:
                        nodes.push(&nodes.top()->getItems().back());
                        nodes.top()->getItem().value_type = BOOL;
                        break;
                    case ARR:
                        nodes.top()->addItem({
                            .name = "",
                            .key_type = ARR,
                            .value_type = BOOL
                        });
                        nodes.push(&nodes.top()->getItems().back());
                        break;
                    default: break;
                    }

                    nodes.top()->addItem({
                        .name = "true",
                        .key_type = BOOL,
                        .value_type = NONE
                    });
                    break;
                case 12: // [VALUE][FALSE]
                    symbol_stack.pop();
                    symbol_stack.push(TS_E);
                    symbol_stack.push(TS_S);
                    symbol_stack.push(TS_L);
                    symbol_stack.push(TS_A);
                    symbol_stack.push(TS_F);

                    switch (nodes.top()->getItem().value_type) {
                    case OBJ:
                        nodes.push(&nodes.top()->getItems().back());
                        nodes.top()->getItem().value_type = BOOL;
                        break;
                    case ARR:
                        nodes.top()->addItem({
                            .name = "",
                            .key_type = ARR,
                            .value_type = BOOL
                        });
                        nodes.push(&nodes.top()->getItems().back());
                        break;
                    default: break;
                    }

                    nodes.top()->addItem({
                        .name = "false",
                        .key_type = BOOL,
                        .value_type = NONE
                    });
                    break;
                case 13: // [VALUE][NULL]
                    symbol_stack.pop();
                    symbol_stack.push(TS_L);
                    symbol_stack.push(TS_L);
                    symbol_stack.push(TS_U);
                    symbol_stack.push(TS_N);

                    switch (nodes.top()->getItem().value_type) {
                    case OBJ:
                        nodes.push(&nodes.top()->getItems().back());
                        nodes.top()->getItem().value_type = _NULL;
                        break;
                    case ARR:
                        nodes.top()->addItem({
                            .name = "",
                            .key_type = ARR,
                            .value_type = _NULL
                        });
                        nodes.push(&nodes.top()->getItems().back());
                        break;
                    default: break;
                    }

                    nodes.top()->addItem({
                        .name = "null",
                        .key_type = _NULL,
                        .value_type = NONE
                    });
                    break;
                default:
                    if (sym != MOD_KEY) {
                        cout << "_" << (int)line[ltr] << "_";
                        return rules;
                    }
                }
            }
        }
    }

    return rules;
}


Rule::Rule(int ruleNum)
{
    num = ruleNum;
}


string
Rule::getString()
{
    switch (num) {
    case 1: return "OBJ -> { PAIR }";
    case 2: return "ARR -> [ VALUE ]";
    case 3: return "VALUE -> OBJ";
    case 4: return "VALUE -> ARR";
    case 5: return "VALUE -> STR";
    case 6: return "KEY -> STR";
    case 7: return "OBJ_END -> , PAIR";
    case 8: return "ARR_END -> , VALUE";
    case 9: return "PAIR -> KEY : VALUE";
    case 10: return "VALUE -> NUM";
    case 11: return "VALUE -> TRUE";
    case 12: return "VALUE -> FALSE";
    case 13: return "VALUE -> NULL";
    default: return "UNKOWN PRODUCTION";
    }
}
