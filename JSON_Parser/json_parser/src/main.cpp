#include <iostream>
#include <fstream>
#include <getopt.h>

#include "../include/jsonparser.h"

using namespace std;


auto printHelp() -> void;


int
main(int argc, char **argv)
{
    int opt;
    string src_file, key;

    while ((opt = getopt(argc, argv, "hf:k:")) != -1) {
        switch (opt) {
        case 'f':
            src_file = optarg;
            break;
        case 'k':
            key = optarg;
            break;
        case 'h':
        default:
            cout << "Usage: json_parser -f <filename> -k <key>" << endl;
            return -1;
        }
    }


    JsonParser parser(src_file);
    parser.parse();

    vector<JsonTree*> matches = parser.tree.findAllItems(key);

    for (const auto& match : matches) {
        cout << endl;

        cout <<'"' << match->extractPath() << "\":" << endl;
        match->printTree();

        cout << endl;
    }

    return 0;
}

