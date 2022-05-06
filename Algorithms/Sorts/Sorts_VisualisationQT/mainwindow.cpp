#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->centralWidget()->setContentsMargins(0,0,0,0);
    setWindowTitle("Sorting Algorithm Visualization");

    // set initial value of the ui components
    ui->labelChanges->setText("Changes\n");
    ui->labelComparisons->setText("Comparisons\n");
    ui->labelTime->setText("Time\n00ms");

    sorting = new Sorting(this);

    ui->mainpalette->addWidget(&paint); // add Paint class instance to draw elements on mainpalette, use PVR to save memory
    ui->comboBoxAlgorithm->addItems(sorting->getAlgorithms());
    ui->comboBoxShuffle->addItems(sorting->getShuffles());
    ui->comboBoxPaintType->addItems(paint.getPaintTypes());
    ui->spinBoxAnimSpeed->setValue(sorting->getDefaultAnimSpeed());
    ui->spinBoxAnimSpeed->setMaximum(sorting->getMaxAnimSpeed());
    ui->SliderSpeed->setValue(sorting->getDefaultAnimSpeed());
    ui->SliderSpeed->setMaximum(sorting->getMaxAnimSpeed());
    ui->comboBoxArraySize->addItems(sorting->getSizeList());
    ui->comboBoxArraySize->setCurrentText("32");

    sorting->setAlgorithm(ui->comboBoxAlgorithm->currentText());
    sorting->setShuffle(ui->comboBoxShuffle->currentText());
    paint.setPaintType(ui->comboBoxPaintType->currentText());
    paint.setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Expanding);
    paint.setPenWidth(32);
    paint.setSpacing(0);
    paint.update();

    completionsound = new QMediaPlayer;
    completionsound->setMedia(QUrl("qrc:/sounds/complete.mp3")); // get the binary data from Resources folder


    connect(sorting, SIGNAL(changed(int*, int, int*, int)),
            this, SLOT(onNumbersChanged(int*, int, int*, int)));

    connect(sorting, SIGNAL(done()),
            this, SLOT(onSortingFinished()));

    connect(ui->comboBoxAlgorithm, SIGNAL(currentTextChanged(QString)),
            this, SLOT(onChangeAlgorithm(QString)));

    connect(ui->comboBoxShuffle, SIGNAL(currentTextChanged(QString)),
            this, SLOT(onChangeShuffle(QString)));

    connect(ui->comboBoxPaintType, SIGNAL(currentTextChanged(QString)),
            this, SLOT(onChangePaintType(QString)));

    connect(ui->SliderSpeed, SIGNAL(valueChanged(int)),
            ui->spinBoxAnimSpeed,SLOT(setValue(int)));

    // reverse of above
    connect(ui->spinBoxAnimSpeed, SIGNAL(valueChanged(int)),
            ui->SliderSpeed, SLOT(setValue(int)));

    // trigger `onNumverOfSizeChange` when array size is chagnged by user
    connect(ui->comboBoxArraySize, SIGNAL(currentTextChanged(QString)),
            this, SLOT(onNumberOfSizeChange(QString)));

    // create array
    sorting->createArray();
}

MainWindow::~MainWindow()
{
    delete completionsound;
    delete ui;
}

// dynamically calculate `penWidth` according to the size etc...
void MainWindow::onNumberOfSizeChange(QString sizestring) {
    int newValue = sizestring.toInt();
    paint.setPenWidth(1024/newValue);
    paint.resetLineColor();
    sorting->setSize(newValue);
    sorting->shuffle();
    sorting->createArray();
    paint.update();
}


// change the number of comparions label and changes label
void MainWindow::onNumbersChanged(int* nums, int size, int* colorIndices, int sizeColorIndices) {
    ui->labelComparisons->setText("Comparisons\n" + QString::number(sorting->get_num_comparisons()));
    ui->labelChanges->setText("Changes\n" + QString::number(sorting->get_num_changed()));
    paint.setPaintData(nums, colorIndices, size, sizeColorIndices);
    paint.update();
}

void MainWindow::onSortingFinished() {
    ui->labelTime->setText("Time\n" + QString::number(timer.elapsed()) + "ms"); // update the time label
    paint.setAnimate(false); // set animate to false
    paint.setLineColor(Qt::yellow); // set line color to yellow
    paint.update(); // update the drawing
    isSorting = false;
    ui->buttonShuffle->setDisabled(false);
    ui->buttonStart->setText("Start");
    // make completion sound
    if (completionsound->state() == QMediaPlayer::PlayingState) {
        completionsound->setPosition(0);
    } else if (completionsound->state() == QMediaPlayer::StoppedState) {
        completionsound->play();
    }
}


void MainWindow::onChangeAlgorithm(QString algorithmName) {
    sorting->setAlgorithm(algorithmName);
}

void MainWindow::onChangeShuffle(QString shuffleName) {
    sorting->setShuffle(shuffleName);
}

void MainWindow::onChangePaintType(QString paintTypeName) {
    paint.setPaintType(paintTypeName);
}

void MainWindow::on_buttonStart_pressed() {
    if (!isSorting) { // if not sorting simply start sorting
        isSorting = true;
        ui->buttonStart->setText("Stop");
        ui->buttonShuffle->setDisabled(true); // disable shuffle button, users have to press stop to shuffle
        sorting->setAnimSpeed(ui->spinBoxAnimSpeed->value());
        paint.setAnimate(true);
        paint.resetLineColor();
        timer.start();
        sorting->start(); // use QThread function start()
    }
    else {
        isSorting = false;
        paint.setAnimate(false);
        ui->buttonShuffle->setDisabled(false); // enable shuffle btn
        paint.resetLineColor();
        ui->buttonStart->setText("Start");
        sorting->terminate(); // use QThread function terminate()
    }
}

void MainWindow::on_buttonShuffle_pressed() { // reset time label and line color
    paint.resetLineColor();
    ui->labelTime->setText("Time\n0ms");
    sorting->createArray();
}
