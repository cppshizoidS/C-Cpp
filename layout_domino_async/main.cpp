#include <iostream>


//#include "Field.h"
#include "Runner.h"
#include "SimpleParser.h"


int main(int argc, char **argv) {
    std::string filePath;
    int iter_count = 0;
    int field_width = 0;
    int field_height = 0;
    double prob_first_rule = 0;
    double prob_second_rule = 0;
    int number_runs = 0;

    try {
        number_runs = stoi(SimpleParser::getCmdOption(argc, argv, "-nruns="));
        iter_count = stoi(SimpleParser::getCmdOption(argc, argv, "-iter="));
        filePath = SimpleParser::getCmdOption(argc, argv, "-f=");
        field_width = stoi(SimpleParser::getCmdOption(argc, argv, "-w="));
        field_height = stoi(SimpleParser::getCmdOption(argc, argv, "-h="));
        prob_first_rule = stod(SimpleParser::getCmdOption(argc,argv, "-p0="));
        prob_second_rule = stod(SimpleParser::getCmdOption(argc,argv, "-pmax="));

        std::cout << field_width << filePath << field_height << prob_first_rule << prob_second_rule;
    } catch (std::invalid_argument &e) {
        std::cerr << e.what();
        return EXIT_FAILURE;
    }


    Runner runner = Runner(filePath, field_height, field_width, prob_first_rule, prob_second_rule, iter_count);
    for (int i = 0; i < number_runs; ++i) {
        runner.run();

    }

    return EXIT_SUCCESS;
}
