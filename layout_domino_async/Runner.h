#ifndef LAYOUT_DOMINO_RUNNER_H
#define LAYOUT_DOMINO_RUNNER_H


#include <string>
#include <random>
#include <utility>
#include <fstream>
#include <random>
#include "Field.h"
#include "Template.h"

//#include "FileWriter.h"

class Runner {
private:
  //  FileWriter domino_info = FileWriter("../domino_info.csv");

    const int WINDOW_SIZE = 5;
    int iter_count;

    Template *templates;
    Field* field;
    Cell *window;

    std::mt19937 mt;

    double probability;
    double probability_max;
    std::string file_name;

    void init_templates();
    void evolve();
    void fill_window(Coord center);
    int count_hits();
    bool compare_with_template(int begin_x, int begin_y, int index_template, Tiletype tiletype);
    int checkValidDomino();

    void change_state(Coord &point);

public:
    const int AMOUNT_TEMPLATE = 12;

    explicit Runner(std::string &file, int height, int width, double prob_0, double prob_1, int iter_count);
    ~Runner();
    void run();
};


#endif //LAYOUT_DOMINO_RUNNER_H
