#ifndef COLONIE_H_INCLUDED
#define COLONIE_H_INCLUDED

#include "instance.h"
#include "cview.h"
using namespace std;



const double Beta = 5.0;
const double Alpha = 2.0;
const int Q = 10;
const int MAX_IT = 250;
const double Rho = 0.95;
const double E = 10.;
const int NMut = 50;
const int MUT_DEPTH = 6;
const double BIG_DOUBLE = 1e15;
const double INITIAL_PHEROMONES = 250.0;



class Colony_view;

struct Colony_parameters
{
    int m = -1;
    double alpha = Alpha;
    double beta = Beta;
    double q = Q;
    double rho = Rho;
    double max_it = MAX_IT;
    double e = E;
    int number_of_mutations = NMut;
    int mutation_depth = MUT_DEPTH;
    int number_of_neighbors = -1;
};

struct Colony_state
{
    vector<vector<double> > pheromone;
    vector<vector<double> > add_pheromone;
    vector<int> best_path;
    vector<int> best_path_elite;
    double min_length = BIG_DOUBLE;
    double min_length_elite = BIG_DOUBLE;
    double average_length = 0.;
    double max_p = 0.;
    int step = 0;
};


class Colony : public Instance
{

private:
    Colony_state etat;
    Colony_parameters param;
    vector<int> random_walk(const int& f);
    void add_pheromones_from_walk(const vector<int>& path,const double& length,const double& q);
    double compute_length(const vector<int>& path);
    void update(vector<int>& path,const vector<int>& path2,double& length,const double& length2);
    void shift(const int &kk, vector<int>& path, double& length);
    void shift2(const int &kk, const int &sh, vector<int>& path, double& length);
    void reverse(const int& kk,vector<int>& path, double& length);
    void ant_try(const int& f);
    void update_pheromones();
    void plot_points(int width);
    void plot_best_result(int width);
    void plot_pheromones(int width);
    QGraphicsScene * scene;
    bool display_pheromones = true;
    bool display_best_walk = true;
    bool pause = false;
    bool already_initialized = false;
    void set_neighbors_graph(const int& k);

public:
    Colony();
    Colony(const Colony& c);
    ~Colony();
    void colonie_steps(const int& k);
    void plot(const double &f);
    void set_colony_view(Colony_view & cview);
    void ask_parameters();
    bool is_finished();
    void write_result(const QString& s);
    void set_options(const bool& b1,const bool& b2);
    void set_pause();
    void stop();
};



bool is_finished(vector<Colony> colonies);




#endif // COLONIE_H_INCLUDED
