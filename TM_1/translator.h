#ifndef TM1_TRANSLATOR_H
#define TM1_TRANSLATOR_H

#include "table_var.h"
#include "table_const.h"
#include "lexeme.h"

using namespace std;

class translator {
private:
    table_const operations; // таблица опрераций - 0
    table_const keywords; // таблица ключевых слов - 1
    table_const separators; // таблица разделителей - 2
    table_const comments; // таблица символов для комментариев - 3
    table_const alphabet; // таблица букв - 4
    table_const digits; // таблица цифр - 5

    table_var identifiers; // таблица идентификаторов - 6
    table_var constants; // таблица констант - 7

    bool lex_string_analysis(string in_string); // лексическая обработка строки.
    // возвращает true, если ошибок нет
    int check_symbol(string symbol); // проверяет допустимый символ или нет.
    // возвращает -1 если не допустим или номер таблицы

    ifstream lex_source; // поток для работы с файлом кода
    ofstream os_token, os_error; // потоки для работы с файлами токенов и ошибок

public:
    translator();

    bool lex_analysis(string path_code, string path_tokens, string path_errors);
    // лексический анализ, возвращает true, если фаза успешно завершена

    void parse(string path_tokens, string path_errors);
    // синтаксический анализ

    class token {
    public:
        int table_n{}; // номер таблицы
        int chain_n{}; // первый индекс в переменной таблице или -1 в постоянной
        int index{}; // второй индекс в переменной таблице или индекс в постоянной

        token();

        token(int t_n, int ch_n, int i);

        friend ostream &operator<<(ostream &os, token &out_t); // вывод токена
        friend istream &operator>>(istream &is, token &in_t); // ввод токена
    };

};

translator::token::token() = default;

translator::token::token(int t_n, int ch_n, int i) {
    table_n = t_n;
    chain_n = ch_n;
    index = i;
}

ostream &operator<<(ostream &os, translator::token &out_t) {
    os << out_t.table_n << " " << out_t.chain_n << " " << out_t.index << endl;
    return os;
}

istream &operator>>(istream &is, translator::token &in_t) {
    is >> in_t.table_n >> in_t.chain_n >> in_t.index;
    return is;
}

translator::translator() {
    operations.read_file("1/operations.txt");
    keywords.read_file("1/keywords.txt");
    separators.read_file("1/separators.txt");
    comments.read_file("1/comment.txt");
    alphabet.read_file("1/alphabet.txt");
    digits.read_file("1/digits.txt");
}

bool translator::lex_analysis(string path_code, string path_tokens, string path_errors) {
    lex_source.open(path_code.c_str(), ios::in);
    os_token.open(path_tokens.c_str(), ios::out);
    os_error.open(path_errors.c_str(), ios::out);

    string input;
    bool is_error = false;

    while (!lex_source.eof() && !is_error) {
        lex_source >> input;
        is_error = !lex_string_analysis(input);
    };

    lex_source.close();
    os_token.close();
    os_error.close();

    return !is_error;
}

bool translator::lex_string_analysis(string in_string) {
    if (!in_string.empty()) {
        bool is_error = false;
        string fsymbol;
        fsymbol = in_string[0];
        bool is_local_error = false;
        string next_part;
        int fsymbol_type = check_symbol(fsymbol);
        switch (fsymbol_type) {
            case -1: {
                os_error << "В " << in_string << " обнаружены недопустимые символы. Символ " << fsymbol
                         << " недопустим." << endl;
                is_error = true;
                cout << "Обнаружен недопустимый символ" << endl;
            }
                break;
            case 0: {
                int position;
                int len;
                string operation = in_string.substr(0, 2);
                position = operations.get_num(operation);
                len = 2;
                if (position == -1) {
                    operation = in_string.substr(0, 1);
                    position = operations.get_num(operation);
                    len = 1;
                }
                if (position != -1) {
                    token token_operation = token(0, -1, position);
                    os_token << token_operation;
                    next_part = in_string.substr(len);
                } else {
                    is_local_error = true;
                    os_error << "В " << in_string << " обнаружены недопустимые символы. Символ "
                             << in_string.substr(position, 1)
                             << " недопустим." << endl;
                    is_error = true;
                    cout << "Обнаружен недопустимый символ" << endl;
                }
            }
                break;
            case 2: {
                int position;
                int len = 1;
                string separator = in_string.substr(0, 1);
                position = separators.get_num(separator);
                token token_separator = token(2, -1, position);
                os_token << token_separator;
                next_part = in_string.substr(len);
            }
                break;
            case 3: {
                string comment = in_string.substr(0, 1);
                string comment_body;
                if (in_string.substr(0, 2) == "/*")
                    comment = "*";
                switch (comment[0]) {
                    case '/': {
                        getline(lex_source, comment_body);
                    }
                        break;
                    case '*': {
                        bool comm_end = false;
                        while (!comm_end && !lex_source.eof()) {
                            lex_source >> comment_body;
                            int is_end = comment_body.find("*/");
                            if (is_end != -1)
                                comm_end = true;
                        }
                        if (!lex_source.eof()) {
                            int is_end = comment_body.find_first_of("*/");
                            next_part = comment_body.substr(is_end + 2);
                            is_local_error = !lex_string_analysis(next_part);
                        } else {
                            is_local_error = true;
                            os_error << "Незакрытый комментарий" << endl;
                        }
                    }
                        break;
                    default: {
                        is_local_error = true;
                        os_error << "Ошибка при использовании комментария" << endl;
                    }
                }
            }
                break;
            case 4: {
                string symbol = in_string.substr(1, 1);

                int i;
                for (i = 1; i < in_string.size() && !is_local_error; i++) {
                    is_local_error = !(alphabet.contains(symbol) || digits.contains(symbol));
                    symbol = in_string.substr(i, 1);
                }

                int start = i;
                if (is_local_error)
                    start--;
                is_local_error = false;

                next_part = in_string.substr(start);

                in_string = in_string.substr(0, start);

                if (keywords.contains(in_string)) {
                    int position = keywords.get_num(in_string);
                    token token_keyword = token(4, -1, position);
                    os_token << token_keyword;
                } else {
                    int first_index = 0, second_index = 0;
                    table_var::place position(first_index, second_index);
                    position = identifiers.add(in_string);
                    token token_identifier = token(4, position.i, position.j);
                    os_token << token_identifier;
                }
//                is_local_error = !lex_string_analysis(next_part);
            }
                break;
            case 5: {
                int i;
                string constant = in_string.substr(0, 1);
                for (i = 1; i < in_string.size() && !is_local_error; i++) {
                    is_local_error = !(digits.contains(constant) || in_string[i] == '.');
                    constant = in_string.substr(i, 1);
                }

                int start = i;
                if (is_local_error)
                    start--;
                is_local_error = false;

                next_part = in_string.substr(start);

                in_string = in_string.substr(0, start);
                int n = in_string.find_last_of('.') - in_string.find_first_of('.');
                if (n != 0)
                    is_local_error = true;

                if (!is_local_error) {
                    int first_index = 0, second_index = 0;
                    table_var::place position(first_index, second_index);
                    position = constants.add(in_string);
                    token token_constant = token(5, position.i, position.j);
                    os_token << token_constant;
                }
            }
                break;
        }
    }
}

int translator::check_symbol(string symbol) {
    if (operations.contains(symbol))
        return 0;
    if (separators.contains(symbol))
        return 2;
    if (comments.contains(symbol))
        return 3;
    if (alphabet.contains(symbol))
        return 4;
    if (digits.contains(symbol) || symbol == "-")
        return 5;
    return -1;
}

#endif //TM1_TRANSLATOR_H
