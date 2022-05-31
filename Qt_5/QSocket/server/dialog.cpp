#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);

    _serv = new MyServer(this, this);

    connect(this, SIGNAL(messageFromGui(QString,QStringList)), _serv, SLOT(onMessageFromGui(QString,QStringList)));
    connect(_serv, SIGNAL(addLogToGui(QString,QColor)), this, SLOT(onAddLogToGui(QString,QColor)));

    if (_serv->doStartServer(QHostAddress::LocalHost, 1234))
    {
        ui->lwLog->insertItem(0, QTime::currentTime().toString()+" server strated at "+_serv->serverAddress().toString()+":"+QString::number(_serv->serverPort()));
        ui->lwLog->item(0)->setTextColor(Qt::green);
    }
    else
    {
        ui->lwLog->insertItem(0, QTime::currentTime().toString()+" server not strated at "+_serv->serverAddress().toString()+":"+QString::number(_serv->serverPort()));
        ui->lwLog->item(0)->setTextColor(Qt::red);
        ui->pbStartStop->setChecked(true);
    }
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::onAddUserToGui(QString name)
{
    ui->lwUsers->addItem(name);
    ui->lwLog->insertItem(0, QTime::currentTime().toString()+" "+name+" joined");
    ui->lwLog->item(0)->setTextColor(Qt::green);
}

void Dialog::onRemoveUserFromGui(QString name)
{
    for (int i = 0; i < ui->lwUsers->count(); ++i)
        if (ui->lwUsers->item(i)->text() == name)
        {
            ui->lwUsers->takeItem(i);
            ui->lwLog->insertItem(0, QTime::currentTime().toString()+" "+name+" left");
            ui->lwLog->item(0)->setTextColor(Qt::green);
            break;
        }
}

void Dialog::onMessageToGui(QString message, QString from, const QStringList &users)
{
    if (users.isEmpty())
        ui->lwLog->insertItem(0, QTime::currentTime().toString()+" message from "+from+": "+message+" to all");
    else
    {
        ui->lwLog->insertItem(0, QTime::currentTime().toString()+" message from "+from+": "+message+" to "+users.join(","));
        ui->lwLog->item(0)->setTextColor(Qt::blue);
    }
}

void Dialog::onAddLogToGui(QString string, QColor color)
{
    addToLog(string, color);
}

void Dialog::on_pbSend_clicked()
{
    if (ui->lwUsers->count() == 0)
    {
        QMessageBox::information(this, "", "No clients connected");
        return;
    }
    QStringList l;
    if (!ui->cbToAll->isChecked())
        foreach (QListWidgetItem *i, ui->lwUsers->selectedItems())
            l << i->text();
    emit messageFromGui(ui->pteMessage->document()->toPlainText(), l);
    ui->pteMessage->clear();
    if (l.isEmpty())
        addToLog("Sended public server message", Qt::black);
    else
        addToLog("Sended private server message to "+l.join(","), Qt::black);
}

void Dialog::on_cbToAll_clicked()
{
    if (ui->cbToAll->isChecked())
        ui->pbSend->setText("Send To All");
    else
        ui->pbSend->setText("Send To Selected");
}


void Dialog::on_pbStartStop_toggled(bool checked)
{
    if (checked)
    {
        addToLog(" server stopped at "+_serv->serverAddress().toString()+":"+QString::number(_serv->serverPort()), Qt::green);
        _serv->close();
        ui->pbStartStop->setText("Start server");
    }
    else
    {
        QHostAddress addr;
        if (!addr.setAddress(ui->leHost->text()))
        {
            addToLog(" invalid address "+ui->leHost->text(), Qt::red);
            return;
        }
        if (_serv->doStartServer(addr, ui->lePort->text().toInt()))
        {
            addToLog(" server strated at "+ui->leHost->text()+":"+ui->lePort->text(), Qt::green);
            ui->pbStartStop->setText("Stop server");
        }
        else
        {
            addToLog(" server not strated at "+ui->leHost->text()+":"+ui->lePort->text(), Qt::red);
            ui->pbStartStop->setChecked(true);
        }
    }
}

void Dialog::addToLog(QString text, QColor color)
{
    ui->lwLog->insertItem(0, QTime::currentTime().toString()+text);
    ui->lwLog->item(0)->setTextColor(color);
}
