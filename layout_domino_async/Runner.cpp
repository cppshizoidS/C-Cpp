#include <sstream>
#include "Runner.h"
//#include "FileWriter.h"



Runner::Runner(std::string &file, int height, int width, double prob_0, double prob_1, int iter_count) {
    this->file_name = file;
    this->templates = new Template[AMOUNT_TEMPLATE];
    this->field = new Field(height, width);
    std::random_device rd;
    mt.seed(rd());
    this->field->init_field(mt);
    this->window = new Cell[WINDOW_SIZE * WINDOW_SIZE];
    this->iter_count = iter_count;
    this->probability = prob_0;
    this->probability_max = prob_1;

    init_templates();
}

void Runner::init_templates() {

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            Coord center(x, y);
            templates[y * WIDTH + x].init_template(center);

        }
    }
}


void Runner::fill_window(Coord center) {
    Coord begin_coord(center.get_x() - field->get_width_bound(), center.get_y() - field->get_width_bound());
    for (int y = 0; y < WINDOW_SIZE; ++y) {
        for (int x = 0; x < WINDOW_SIZE; ++x) {
            window[y * WINDOW_SIZE + x] = field->get_cell_by_xy(begin_coord.get_x() + x, begin_coord.get_y() + y);
        }
    }
}


void Runner::evolve() {

    int all_point_number = field->get_size_available_field();
    std::uniform_int_distribution<int> dist1(0, all_point_number - 1);
    //get number of points
    int max_count = 0;
    int iter_num = 0;
    for (int i = 0; i < iter_count; ++i) {

        for (int j = 0; j < all_point_number; ++j) {

            int rand_index = dist1(mt);
            //Todo: make coordinate shift inside field
            Coord center = field->get_available_field()[rand_index].get_coord();
            center.set_coord(center.get_x() + field->get_width_bound(), center.get_y() + field->get_height_bound());
            fill_window(center);

            auto hits = count_hits();
            field->change_cell_hits_by_coord(center, hits);
            change_state(center);

            int count = checkValidDomino();
            if (count > max_count) {
                max_count = count;
                iter_num = i;
            }
        }

    }

    this->field->init_field(mt);
    std::cout << iter_num << "; " << max_count << std::endl;
}

void Runner::run() {
    std::uniform_real_distribution<double> rand_prob(0, 1);
    evolve();
}

Runner::~Runner() {
    delete field;
    delete[] templates;
    delete[] window;
}

int Runner::count_hits() {
    //for horizontal
    int count_hits = 0;
    for (int i = 0; i < AMOUNT_TEMPLATE; ++i) {
        //shifted (0,0)
        auto shiftedX = 2 - templates[i].get_center().get_x();
        auto shiftedY = 2 - templates[i].get_center().get_y();
        auto isHit = compare_with_template(shiftedX, shiftedY, i, HORIZONTAL);
        if (isHit && (templates[i].get_center_state() != 1)) {
            count_hits++;
        } else {
            if (isHit && (templates[i].get_center_state() == 1)) {
                count_hits = 100;
            }
        }
    }
    //for vertical
    for (int i = 0; i < AMOUNT_TEMPLATE; ++i) {
        //shifted (0,0)
        auto shiftedX = 2 + templates[i].get_center().get_y();
        auto shiftedY = 2 - templates[i].get_center().get_x();
        auto isHit = compare_with_template(shiftedX, shiftedY, i, VERTICAL);
        if (isHit && (templates[i].get_center_state() != 1)) {
            count_hits++;
        } else {
            if (isHit && (templates[i].get_center_state() == 1)) {
                count_hits = 100;
            }
        }
    }
    return count_hits;
}

// create method
int Runner::checkValidDomino() {
    int count = 0;
    for (int i = 1; i < field->get_height() - 3; ++i) {
        for (int  j = 1; j < field->get_width() - 3; ++j) {
            bool isValidDomino = true;
            for (int y = 0; y < HEIGHT; ++y) {
                for (int x = 0; x < WIDTH; ++x) {
                    int field_cell_status = field->get_cell_by_coord(Coord (j + x, i + y)).get_state();
                    isValidDomino = (templates[1].get_template()[y * WIDTH + x].get_state() == field_cell_status);
                    if (!isValidDomino) {
                        break;
                    }
                }
                if (!isValidDomino) {
                    break;
                }

            }
            if (isValidDomino) {
                count++;
            }
        }
    }

    for (int i = 1; i < field->get_height() - 3; ++i) {
        for (int j = 1; j < field->get_width() - 3; ++j) {
            bool isValidDomino = true;
            for (int x = 0; x < WIDTH; ++x) {
                for (int y = 0; y < HEIGHT; ++y) {

                    int field_cell_status = field->get_cell_by_coord(Coord (j + y, i + x)).get_state();
                    isValidDomino = (templates[1].get_template()[y * WIDTH + x].get_state() == field_cell_status);
                    if (!isValidDomino) {
                        break;
                    }
                }
                if (!isValidDomino) {
                        break;
                    }
            }
            if (isValidDomino) {
                count++;
            }
        }
    }
    return count;
}


void Runner::change_state(Coord &point) {
    std::uniform_real_distribution<double> rand_prob(0, 1);
    std::uniform_int_distribution<int> rand_status(0, 1);
    int new_state = field->get_cell_by_coord(point).get_state();

    auto amountHits = field->get_cell_by_coord(point).get_hits();
    double prob = rand_prob(mt);
    if (amountHits == 0 && prob <= probability) {
        new_state = rand_status(mt);
    }
    prob = rand_prob(mt);
    if (amountHits == 1 && prob <= probability_max) {
        new_state = rand_status(mt);
    }

    field->change_cell_status_by_coord(point, new_state);
}

bool Runner::compare_with_template(int begin_x, int begin_y, int index_template, Tiletype tiletype) {


    //change location of template for comparing
    switch (tiletype) {
        case HORIZONTAL:
            for (int y = 0; y < HEIGHT; ++y) {
                for (int x = 0; x < WIDTH; ++x) {
                    auto curr_template = templates[index_template].get_template();
                    if (curr_template[y * WIDTH + x].get_state() != -1) {
                        if (curr_template[y * WIDTH + x].get_state() != window[(begin_y + y) * WINDOW_SIZE + (begin_x + x)].get_state()) {
                            return false;
                        }
                    }
                }
            }
            break;
        case VERTICAL:
            for (int x = 0; x < WIDTH; ++x) {
                for (int y = 0; y < HEIGHT; ++y) {
                    auto curr_template = templates[index_template].get_template();
                    if (curr_template[y * WIDTH + x].get_state() != -1) {
                        if (curr_template[y * WIDTH + x].get_state() !=
                            window[(begin_y + x) * WINDOW_SIZE + (begin_x - y)].get_state()) {
                            return false;
                        }
                    }
                }
            }
            break;
    }
    return true;
}



