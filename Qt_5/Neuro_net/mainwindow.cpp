#include "mainwindow.h"

mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowTitle("Draw digit:)");
    QWidget* MyWidget = new QWidget(this);
    MyWidget->setLayout(ui.gridLayout);
    setCentralWidget(MyWidget);
    resize(500, 380);

    NW_config = ReadDataNetWork("Config.txt");
    NW.Init(NW_config);
    NW.ReadWeights();
    ui.lcdNumber->display("");
}
void mainwindow::clearLCD() {
    ui.lcdNumber->display("");
}
void mainwindow::ReadTest(double* input, int input_n) {
    QFile mFile("test.txt");
    mFile.open(QIODevice::ReadOnly);
    QTextStream fin(&mFile);
    if (mFile.isOpen()) {
        for (int j = 0; j < input_n; j++) 
            fin >> input[j];
    }
    else QMessageBox::information(0, "Error", "Error reading the file");
    mFile.close();
}
void mainwindow::StartGuess() {
    double* input = new double[NW_config.size[0]];
    ReadTest(input, NW_config.size[0]);
    NW.SetInput(input);
    double digit = NW.ForwardFeed();
    ui.lcdNumber->display(QString().setNum(digit));
}
data_NetWork mainwindow::ReadDataNetWork(string path) {
    data_NetWork data{};
    ifstream fin;
    fin.open(path);
    if (!fin.is_open()) {
        cout << "Error reading the file " << path << endl;
        throw runtime_error("rror reading the file\n");
    }
    else
        cout << path << " loading...\n";
    string tmp;
    int n;
    while (!fin.eof()) {
        fin >> tmp;
        if (tmp == "NetWork") {
            fin >> n;
            data.n = n;
            data.size = new int[n];
            for (int i = 0; i < n; i++) {
                fin >> data.size[i];
            }
        }
    }
    fin.close();
    return data;
}