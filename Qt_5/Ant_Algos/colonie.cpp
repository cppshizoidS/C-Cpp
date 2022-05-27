#include "colonie.h"
using namespace std;


Colony::Colony(){

}

Colony::Colony(const Colony& c)
{
    n = c.n;
    x = c.x;
    y = c.y;
    dist = c.dist;
    adj = c.adj;
    sub_graph = c.sub_graph;
    normalization = c.normalization;
    DX = c.DX;
    DY = c.DY;

    param = c.param;

    scene = new QGraphicsScene();
    already_initialized = true;

    if(!c.already_initialized){
        vector<vector<double> > empty_matrix = vector<vector<double> >(n,vector<double>(n,0));
        etat.pheromone = vector<vector<double> >(n,vector<double>(n,INITIAL_PHEROMONES));;
        etat.add_pheromone = empty_matrix;
        set_neighbors_graph(param.number_of_neighbors);
    } else {
        etat = c.etat;
    }
};



// Destructor
Colony::~Colony()
{
    if(already_initialized){
        delete scene;
    }
}


void Colony::set_neighbors_graph(const int& k)
{
    param.number_of_neighbors = k;
    sub_graph = vector<vector<bool> >(n,vector<bool>(n,0));
    for(int i=0;i<n;i++){
        vector<pair<double,int> > tri;
        for(int j=0;j<n;j++)
        {
            tri.push_back(make_pair(dist[i][j],j));
        }
        sort(tri.begin(),tri.end());
        for(int k=0;k<min(n,param.number_of_neighbors);k++)
        {
            sub_graph[i][tri[k].second] = true;
        }
    }
}


void Colony::colonie_steps(const int& k)
{
    if(pause)return;

    for(int it=0;(etat.step<param.max_it)&&(it<k);it++)
    {
        etat.average_length = 0;
        etat.min_length_elite = 1e12;
        int start = rand()%n;
        for(int f=0;f<param.m;f++)
        {
            int ff = (4*f+start)%n;
            ant_try(ff);
        }
        etat.average_length/=param.m;
        add_pheromones_from_walk(etat.best_path_elite,etat.min_length_elite,param.e*param.q);
        add_pheromones_from_walk(etat.best_path,etat.min_length,1.0*param.e*param.q);
        update_pheromones();
        etat.step++;
    }
}

void Colony::ant_try(const int& f)
{
    vector<int> path = random_walk(f);

    if(int(path.size())<=n)return;

    double length = compute_length(path);
    vector<int> path2 = path;
    double length2 = length;

    update(path,path2,length,length2);

    for(int tt=0;tt<param.number_of_mutations;tt++)
    {
        shift(2,path,length);
        int MAX_LEN = min(n,param.mutation_depth);
        for(int len=3;len<MAX_LEN;len++){
            shift(len,path,length);
            if(len>=4) shift2(len,2,path,length);
            if(len>=5) shift2(len,3,path,length);
            reverse(len,path,length);
        }
        shift(MAX_LEN + rand()%(n-MAX_LEN),path,length);
        reverse(MAX_LEN + rand()%(n-MAX_LEN),path,length);
        int len = MAX_LEN + rand()%(n-MAX_LEN-2);
        shift2(len,2+rand()%(len-3),path,length);
    }


    etat.average_length += length;
    add_pheromones_from_walk(path,length,param.q);
}

vector<int> Colony::random_walk(const int& f)
{
    vector<int> path;

    random_device rd; /// should be an attribute of the colony
    mt19937 gen(rd());


    vector<bool> used(n,0);


    used[f] = true;
    int pos = f;
    path.push_back(pos);

    vector<double> distrib(n,0.0);

    for(int i=1;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            distrib[j] = (!used[j] && (i >= n - END_OF_WALK || sub_graph[pos][j]) ? EPSILON_WEIGHT + pow(etat.pheromone[pos][j],param.alpha)*1/pow(dist[pos][j],param.beta) : 0.);
        }

        discrete_distribution<int> d(distrib.begin(),distrib.end());
        pos = d(gen);

        if(used[pos])return path;

        used[pos]=true;
        path.push_back(pos);
    }

    path.push_back(f);

    return path;
}

void Colony::add_pheromones_from_walk(const vector<int>& path,const double& length, const double &q)
{
    for(int i=0;i+1<int(path.size());i++)
    {
        etat.add_pheromone[path[i]][path[i+1]]+=q/length;
        etat.add_pheromone[path[i+1]][path[i]]+=q/length;
    }
}


double Colony::compute_length(const vector<int>& path)
{
    double length = 0.;
    for(int i=0;i+1<int(path.size());i++)
    {
        length+=dist[path[i]][path[i+1]];
    }
    return length;
}


void Colony::update(vector<int>& path,const vector<int>& path2,double& length,const double& length2)
{
    if(length2<length)
    {
        length = length2;
        path = path2;
    }

    if(length<etat.min_length)
    {
        etat.min_length = length;
        etat.best_path = path;
    }

    if(length<etat.min_length_elite)
    {
        etat.min_length_elite = length;
        etat.best_path_elite = path;
    }
}


void Colony::shift(const int& kk,vector<int>& path,double& length)
{
    int num = kk-1;
    for(int i=0;i<n;i++)
    {
        vector<int> path2 = path;
        int aux = path2[i];
        for(int ii=i;ii<i+num;ii++)
        {
            path2[ii%n]=path2[(ii+1)%n];
        }
        path2[(i+num)%n]=aux;
        path2[n]=path2[0];
        double length2=compute_length(path2);
        update(path,path2,length,length2);
    }
}

void Colony::shift2(const int& kk,const int& sh,vector<int>& path,double& length)
{
    for(int i=0;i<n;i++)
    {
        vector<int> path2 = path;
        vector<int> path3 = path2;
        for(int ii=i;ii<i+kk;ii++)
        {
            int rel = ii-i;
            int rel2 = (rel+sh)%kk;
            path2[(i+rel)%n]=path3[(i+rel2)%n];
        }
        path2[n]=path2[0];
        double length2=compute_length(path2);
        update(path,path2,length,length2);
    }
}


void Colony::reverse(const int& kk,vector<int>& path,double& length)
{
    for(int i=0;i<n;i++)
    {
        vector<int> path2 = path;
        vector<int> path3 = path2;
        for(int ii=i;ii<i+kk;ii++)
        {
            path2[ii%n]=path3[(i+kk-1-(ii-i))%n];
        }
        path2[n]=path2[0];
        double length2=compute_length(path2);
        update(path,path2,length,length2);
    }
}


void Colony::update_pheromones()
{
    etat.max_p = 0;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            etat.pheromone[i][j] = param.rho*etat.pheromone[i][j] + etat.add_pheromone[i][j];
            etat.max_p = max(etat.max_p, etat.pheromone[i][j]);
            etat.add_pheromone[i][j] = 0.;
        }
    }
};


bool Colony::is_finished()
{
    return !(etat.step < param.max_it);
}


bool is_finished(vector<Colony> colonies){
    for(int i=0;i<int(colonies.size());i++)
    {
        if(!(colonies[i].is_finished()))
            return false;
    }
    return true;
}



void Colony::write_result(const QString &s)
{
    if(!QDir("results").exists())
        QDir().mkdir("results");
    vector<int> path = etat.best_path;
    ofstream out(("results/"+s+".txt").toStdString());
    out << etat.min_length << "\n";
    for(int i = 0; i < int(path.size()); i++)
       out << path[i] + 1 << " ";
}


void Colony::set_options(const bool& b1,const bool& b2)
{
    display_pheromones = b1;
    display_best_walk = b2;
}


void Colony::set_pause()
{
    pause = !pause;
}


void Colony::stop()
{
    param.max_it = etat.step;
}


void Colony::plot_points(int width)
{
    QPen pen2(Qt::blue);
    int radius = 2*width;
    for(int i = 0; i < int(x.size()); i++)
        scene->addEllipse((x[i]-radius), (y[i]-radius), 2*radius, 2*radius,pen2,QBrush(Qt::blue,Qt::SolidPattern));
}


void Colony::plot_best_result(int width)
{
    if(etat.step==0)width=0;

    QVector <QPointF> points;
    vector<int> path = etat.best_path;

    for(int i = 0; i< int(path.size()); i++)
       points.append(QPointF(x[path[i]], y[path[i]]));

    QPen pen(Qt::red);
    pen.setWidth(width);
    for(int i = 0; i+1< points.size(); i++)
        scene->addLine(points[i].x(), points[i].y(), points[i+1].x(), points[i+1].y(),pen);
}


void Colony::plot_pheromones(int width)
{
    if(etat.step==0)width=0;

    QPen pen(Qt::green);
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<i;j++)
        {
            int ww = min(double(width),max(0.,width*pow(etat.pheromone[i][j]/(EPSILON_GENERAL + etat.max_p),2)));
            pen.setWidth(ww);
            if(ww>0)
                scene->addLine(x[i], y[i], x[j], y[j], pen);
        }
    }
}


void Colony::plot(const double&f)
{
    scene->clear();
    plot_points(1.5);

    double w = scene->sceneRect().width();
    double h = scene->sceneRect().height();

    double pw = f*8.0*max(1.0*w/DX,1.0*h/DY);

    scene->clear();
    if(display_pheromones)plot_pheromones(pw);
    if(display_best_walk)plot_best_result(pw/6.0);
    plot_points(pw/5.0);
}


void Colony::set_colony_view(Colony_view & cview)
{
    cview.load(scene,param.max_it,etat.step,etat.min_length*normalization);
}


void Colony::ask_parameters()
{
    Colony_parameters c = param;
    QWidget * window0 = new QWidget();
    window0->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
    window0->setWindowTitle("Algorithm Parameters");
    window0->setFixedWidth(300);
    window0->setStyleSheet("QPushButton { font-size: 10pt; background-color: lightgrey }");
    QSpinBox * mbox = new QSpinBox;
    mbox->setRange(1,1000);
    if(c.m<0)c.m=n/4;
    mbox->setValue(c.m);
    QDoubleSpinBox * alphabox = new QDoubleSpinBox;
    alphabox->setRange(0.,40.);
    alphabox->setValue(c.alpha);
    QDoubleSpinBox * betabox = new QDoubleSpinBox;
    betabox->setRange(0.,20.);
    betabox->setValue(c.beta);
    QDoubleSpinBox * qbox = new QDoubleSpinBox;
    qbox->setRange(1.,10000000.);
    qbox->setValue(c.q);
    QDoubleSpinBox * rhobox = new QDoubleSpinBox;
    rhobox->setRange(0.,1.);
    rhobox->setValue(c.rho);
    QSpinBox * itbox = new QSpinBox;
    itbox->setRange(1,100000);
    itbox->setValue(c.max_it);
    QSpinBox * ebox = new QSpinBox;
    ebox->setRange(0,1000);
    ebox->setValue(c.e);
    QSpinBox * number_of_mutationsbox = new QSpinBox;
    number_of_mutationsbox->setRange(0,10000);
    number_of_mutationsbox->setValue(c.number_of_mutations);
    QSpinBox * mutation_depthbox = new QSpinBox;
    mutation_depthbox->setRange(4,n-4);
    mutation_depthbox->setValue(c.mutation_depth);
    QSpinBox * number_of_neighborsbox = new QSpinBox;
    if(c.number_of_neighbors<0)c.number_of_neighbors=NBV_d;
    number_of_neighborsbox->setRange(5,1000);
    number_of_neighborsbox->setValue(c.number_of_neighbors);
    QCheckBox * gpbox = new QCheckBox;
    QCheckBox * gbbox = new QCheckBox;

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Number of ants : ", mbox);
    formLayout->addRow("Alpha : ", alphabox);
    formLayout->addRow("Beta : ", betabox);
    formLayout->addRow("Q : ", qbox);
    formLayout->addRow("Conservation rho : ", rhobox);
    formLayout->addRow("Number of iterations : ", itbox);
    formLayout->addRow("Elite : ", ebox);
    formLayout->addRow("Local search iterations : ", number_of_mutationsbox);
    formLayout->addRow("Mutation depth : ", mutation_depthbox);
    formLayout->addRow("Number max of neighboors : ", number_of_neighborsbox);
    formLayout->addRow("Show pheromones ", gpbox);
    gpbox->setChecked(display_pheromones);
    formLayout->addRow("Show best result so far ", gbbox);
    gbbox->setChecked(display_best_walk);

    QVBoxLayout *layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->addLayout(formLayout);

    QPushButton *boutonOk = new QPushButton("OK");
    layoutPrincipal->addWidget(boutonOk);

    window0->setLayout(layoutPrincipal);
    window0->show();

    QEventLoop loop;
    QObject::connect(boutonOk, SIGNAL(clicked()), &loop, SLOT(quit()));
    loop.exec();

    param.m = mbox->value();
    param.alpha = alphabox->value();
    param.beta = betabox->value();
    param.q = qbox->value();
    param.rho = rhobox->value();
    param.max_it = itbox->value();
    param.e = ebox->value();
    param.number_of_mutations = number_of_mutationsbox->value();
    param.mutation_depth = mutation_depthbox->value();
    display_pheromones = gpbox->checkState();
    display_best_walk = gbbox->checkState();
    set_neighbors_graph(number_of_neighborsbox->value());

    delete mbox;
    delete alphabox;
    delete betabox;
    delete qbox;
    delete rhobox;
    delete itbox;
    delete ebox;
    delete number_of_mutationsbox;
    delete mutation_depthbox;
    delete gpbox;
    delete gbbox;
    delete number_of_neighborsbox;
    delete formLayout;
    delete boutonOk;
    delete layoutPrincipal;
    delete window0;
}
