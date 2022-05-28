#include <iostream>
#include "table_const.h"
#include "table_var.h"
#include "lexeme.h"
#include "translator.h"

int main() {
    translator translator;
    translator.lex_analysis("code.txt", "tokens.txt", "errors.txt");
    return 0;
}
