#include "interface.h"
using namespace std;



int main(int argc, char *argv[])
{
    QApplication * a = new QApplication(argc, argv);

    int K,n,DX,DY;
    bool choice = ask_main_parameters(K,n,DX,DY);
    Colony c;

    c.random_points(n,DX,DY);

    if(choice){
        c.load_file(DX,DY);
    }
    else{
        QDate date = QDate::currentDate();
        QTime time = QTime::currentTime();

        c.write_points("Instance_"+date.toString("dd-MM-yyyy")+"_"+time.toString("hh-mm-ss"));
    }

    c.ask_parameters();


    vector<Colony> colonies;
    for(int i=0;i<K;i++){
        colonies.push_back(Colony(c));
    }

    DX = min(DX,QApplication::desktop()->screenGeometry().width()/K);
    DY = min(DY,QApplication::desktop()->screenGeometry().height()-200);
    QWidget * window = new QWidget();
    window->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);

    QPushButton * run = new QPushButton(window);
    QPushButton * close = new QPushButton(window);
    QPushButton * pause = new QPushButton(window);
    QPushButton * stop = new QPushButton(window);
    run->setFixedSize(K*DX/4,25);
    run->move(0,DY);
    run->setText("Run");
    run->setStyleSheet("QPushButton { font-size: 10pt; font-weight: bold; color: green ;}");
    pause->setFixedSize(K*DX/4,25);
    pause->move(K*DX/4,DY);
    pause->setText("Pause");
    pause->setStyleSheet("QPushButton { font-size: 10pt; font-weight: bold; color: blue ;}");
    stop->setFixedSize(K*DX/4,25);
    stop->move(2*K*DX/4,DY);
    stop->setText("Stop");
    stop->setStyleSheet("QPushButton { font-size: 10pt; font-weight: bold; color: #bb0000 ;}");
    close->setFixedSize(K*DX/4,25);
    close->move(3*K*DX/4,DY);
    close->setText("Close");
    close->setStyleSheet("QPushButton { font-size: 10pt; font-weight: bold; color: black ;}");
    window->setWindowTitle("Improved Ant Colony System Algorithm");


    SetThings * connexions = new SetThings[K];
    SetThings * global = new SetThings();
    global->window = window;
    Colony_view * cviews = new Colony_view[K];
    vector<Colony_view> cviews;

    qDebug() << "There\n";

    for(int i=0;i<K;i++)
    {
        cviews.push_back(Colony_view(i,window,DX,DY));
        connexions[i].c = &colonies[i];
        cviews[i].connect(&connexions[i]);
        QObject::connect(pause, SIGNAL(clicked()),&connexions[i], SLOT(pause()));
        QObject::connect(stop, SIGNAL(clicked()),&connexions[i], SLOT(stop()));
        QObject::connect(close, SIGNAL(clicked()),&connexions[i], SLOT(stop()));
    }

    for(int i=0;i<K;i++)
    {
        colonies[i].set_colony_view(cviews[i]);
        colonies[i].plot(1.0);
    }


    window->show();
    window->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            window->size(),
            a->desktop()->availableGeometry()
        )
    );
    window->setFixedSize(window->size());

    QObject::connect(close, SIGNAL(clicked()), a, SLOT(quit()));
    QObject::connect(close, SIGNAL(clicked()),global, SLOT(quit_window()));

    QEventLoop loop;
    QObject::connect(run, SIGNAL(clicked()), &loop, SLOT(quit()));
    QObject::connect(stop, SIGNAL(clicked()), &loop, SLOT(quit()));
    QObject::connect(close, SIGNAL(clicked()), &loop, SLOT(quit()));

    loop.exec();
    run->setDisabled(true);
    run->setStyleSheet("QPushButton { font-size: 10pt; font-weight: bold; color: grey;}");


    while(!is_finished(colonies))
    {
        for(int i=0;i<K;i++){
            colonies[i].colonie_steps(1);
            colonies[i].set_colony_view(cviews[i]);
            colonies[i].plot(1.0);
        }
        a->processEvents();
    }

    for(int i=0;i<int(colonies.size());i++){

        QDate date = QDate::currentDate();
        QTime time = QTime::currentTime();

        colonies[i].write_result("Solution" + QString::number(i+1) + "_" + date.toString("dd-MM-yyyy") + "_"+time.toString("hh-mm-ss"));
        colonies[i].set_options(false,true);
        colonies[i].set_colony_view(cviews[i]);
        colonies[i].plot(1.0);
        a->processEvents();
    }



    stop->setEnabled(false);
    stop->setStyleSheet("QPushButton { font-size: 10pt; font-weight: bold; color: grey;}");

    if(!window->isVisible())a->quit();

    a->exec();
    delete window;
    a->quit();

    return 0;
}
