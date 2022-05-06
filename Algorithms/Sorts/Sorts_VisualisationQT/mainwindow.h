#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QMediaPlayer>
#include <QTime>
#include <vector>
#include <string>
#include "sorting.h"
#include "paint.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Sorting* sorting;
    Paint paint;
    QTime timer;
    bool isSorting {false};
    QMediaPlayer* completionsound;

public slots:
    void onNumbersChanged(int*, int, int*, int);
    void onChangeAlgorithm(QString);
    void onChangeShuffle(QString);
    void onSortingFinished();
    void onNumberOfSizeChange(QString);
    void onChangePaintType(QString);

private slots:
    void on_buttonStart_pressed();
    void on_buttonShuffle_pressed();
};
#endif // MAINWINDOW_H
