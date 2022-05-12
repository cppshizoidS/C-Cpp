#include "connecttoserver.h"
#include "ui_connecttoserver.h"

#include <QSettings>

#include <randmaxlocation.h>
#include <randomlocation.h>
#include <edgelocation.h>

ConnectToServer::ConnectToServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectToServer)
{
    ui->setupUi(this);
    loadSettings();

    connect(ui->btnConnect, SIGNAL(clicked()), this, SLOT(ok()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(cancel()));

    connect(ui->grpComputer, SIGNAL(toggled(bool)), this, SLOT(computer(bool)));
    connect(ui->grpServer, SIGNAL(toggled(bool)), this, SLOT(server(bool)));

    connect(ui->LoginTxt, SIGNAL(textEdited(QString)), this, SLOT(check(QString)));
}

void ConnectToServer::check(const QString &s)
{
    QString alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString n;

    for(int i=0; i< s.length() && i < 30; ++i)
    {
        if(alpha.indexOf(s[i])!=-1)
            n+= s[i];
    }
    ui->LoginTxt->setText(n);
}

void ConnectToServer::computer(bool ok)
{
    ui->grpServer->setChecked(!ok);
    ui->grpComputer->setChecked(ok);
}

void ConnectToServer::server(bool ok)
{
    ui->grpComputer->setChecked(!ok);
    ui->grpServer->setChecked(ok);
}

void ConnectToServer::ok()
{
    QString login = ui->LoginTxt->text().trimmed();
    QString IP = ui->IPTxt->text().trimmed();
    int port = ui->PortTxt->value();

    serv.login = login;
    serv.ip = IP;
    serv.port = port;
    serv.location = NULL;

    if(ui->grpComputer->isChecked())
    {
        int ind = ui->comboBox->currentIndex();
        serv.sloc = ui->comboBox->currentText();

        serv.location = getAutoLocation(ind);

    }

    saveSettings(serv);
    accept();
}

AutoLocation * ConnectToServer::getAutoLocation(int ind)
{
    switch(ind)
    {
        case 0:
            return new RandomLocation;
        case 1:
            return new RandMaxLocation;
        case 2:
            return new EdgeLocation;
    }
    return NULL;
}

void ConnectToServer::cancel()
{
    reject();
}

const struct SeaServer * ConnectToServer::getServ()
{
    return &serv;
}

void ConnectToServer::saveSettings(const struct SeaServer & ser)
{
    QSettings settings;

    settings.setValue("Login", ser.login);
    settings.setValue("IP",ser.ip);
    settings.setValue("Port", ser.port);
    settings.setValue("Server", ser.location == NULL);
    settings.setValue("location", ui->comboBox->currentText());

}

void ConnectToServer::loadSettings()
{
    QSettings settings;

    ui->LoginTxt->setText(settings.value("Login").toString());
    ui->IPTxt->setText(settings.value("IP").toString());
    ui->PortTxt->setValue(settings.value("Port").toInt());

    bool server = settings.value("Server", true).toBool();

    ui->grpServer->setChecked(server);
    ui->grpComputer->setChecked(!server);

    int r = ui->comboBox->findText(settings.value("location",tr("Лёгкий")).toString());
    if(r != -1)
        ui->comboBox->setCurrentIndex(r);

}

ConnectToServer::~ConnectToServer()
{
    delete ui;
}
