#include "about.h"
#include "ui_about.h"

#include <QMessageBox>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnQt, SIGNAL(clicked()), this, SLOT(show_qt()));
}

void About::show_qt()
{
    QMessageBox::aboutQt(this);
}

About::~About()
{
    delete ui;
}
