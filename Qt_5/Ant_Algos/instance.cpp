#include "instance.h"
using namespace std;

double norm(const int& x,const int& y){
    return sqrt(x*x + y*y);
}

// Destructor
Instance::~Instance()
{

}

void Instance::random_points(const int& n2,const int& dx,const int& dy)
{
    srand (time(NULL));

    DX = dx;
    DY = dy;

    n = n2;
    x = vector<int>(n);
    y = vector<int>(n);
    vector<vector<double> > empty_matrix = vector<vector<double> >(n,vector<double>(n));
    dist = empty_matrix;
    for(int i=0;i<n;i++){
        x[i] = dx/10 + rand()%(dx-2*dx/10);
        y[i] = dy/10 + rand()%(dy-2*dy/10);
    }
    double mx = 0.0;
    for(int i=0;i<n;i++){
        for(int j=0;j<i;j++){
            double calcul = norm(x[i]-x[j],y[i]-y[j]);
            mx = max(mx,calcul);
            dist[i][j] = calcul;
            dist[j][i] = calcul;
        }
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<i;j++){
            dist[i][j] /= mx;
            dist[j][i] /= mx;
        }
    }
    normalization = mx;
}


void Instance::write_points(QString s)
{
    if(!QDir("problems").exists())
        QDir().mkdir("problems");
    ofstream out(("problems/"+s+".txt").toStdString());
    out << n << "\n";
    for(int i = 0; i < n; i++)
       out << x[i] << " " << y[i] << "\n";
}


QString Instance::ask_file()
{
    QWidget *  fen = new QWidget();
    QString fichier = QFileDialog::getOpenFileName(fen, "Open file", QString());
    delete fen;
    return fichier;
}


void Instance::load_file(const int& dx,const int& dy)
{
    DX = dx;
    DY = dy;

    QString str = ask_file();
    if(str==NULL)return;
    ifstream in(str.toStdString());
    in >> n;
    x = vector<int>(n);
    y = vector<int>(n);
    vector<vector<double> > empty_matrix = vector<vector<double> >(n,vector<double>(n));
    dist = empty_matrix;
    for(int i=0;i<n;i++){
        in >> x[i];
        in >> y[i];
    }
    double mx = 0.0;
    for(int i=0;i<n;i++){
        for(int j=0;j<i;j++){
            double calcul = norm(x[i]-x[j],y[i]-y[j]);
            mx = max(mx,calcul);
            dist[i][j] = calcul;
            dist[j][i] = calcul;
        }
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<i;j++){
            dist[i][j] /= mx;
            dist[j][i] /= mx;
        }
    }
    normalization = mx;
}
