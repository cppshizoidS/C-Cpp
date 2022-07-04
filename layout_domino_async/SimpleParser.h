#ifndef LAYOUT_DOMINO_SIMPLEPARSER_H
#define LAYOUT_DOMINO_SIMPLEPARSER_H

#include <iostream>

class SimpleParser {

public:
    static std::string getCmdOption(int argc, char* argv[], const std::string &option) {
        std::string cmd;
        int size = argc;
        for (int i = 0; i <= size; i += 1) {
            std::string curr_arg = argv[i];
            if (curr_arg.find(option) == 0) {
                std::size_t position = curr_arg.find_first_of('=');
                cmd = curr_arg.substr(position + 1);
                return cmd;
            }
        }
        if (cmd.empty()) {
            throw std::invalid_argument("argument is not set");
        }
        return cmd;
    }
};

#endif //LAYOUT_DOMINO_SIMPLEPARSER_H
