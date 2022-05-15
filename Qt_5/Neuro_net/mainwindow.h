#pragma once

#include <QtWidgets>
#include "ui_mainwindow.h"
#include "PaintScene.h"
#include "NetWork.h"
class mainwindow : public QMainWindow
{
    Q_OBJECT
    PaintScene* PS;
    NetWork NW;
    data_NetWork NW_config;
public:
    mainwindow(QWidget *parent = Q_NULLPTR);
    data_NetWork ReadDataNetWork(string path);
    void ReadTest(double* input, int input_n);
signals:
    void endWriteToFile();
public slots:
    void StartGuess();
    void clearLCD();
private:
    Ui::mainwindowClass ui;
};
