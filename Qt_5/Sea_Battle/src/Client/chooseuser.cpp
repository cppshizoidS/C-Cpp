#include "chooseuser.h"
#include "ui_chooseuser.h"

#include <QListWidgetItem>
#include <QMessageBox>

ChooseUser::ChooseUser(QWidget *parent, QStringList * users) :
    QDialog(parent),
    ui(new Ui::ChoseUser)
{
    ui->setupUi(this);

    for(int i=1; i< users->size(); ++i)
        ui->lstUsers->insertItem(i, users->at(i));

    connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(ok()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(cancel()));
}

void ChooseUser::ok()
{
    QList<QListWidgetItem *> lst = ui->lstUsers->selectedItems();
    if(lst.size() == 0)
    {
        QMessageBox::warning(this, tr("Предупреждение"), tr("Пожалуйста, выберите противника!"));
        return;
    }
    else if(lst.size() > 1)
    {
        QMessageBox::warning(this, tr("Предупреждение"), tr("В текущей версии программы вы можете играть только с одним игроком!"));
        return;
    }
    player = lst[0]->data(Qt::DisplayRole).toString();
    accept();
}

void ChooseUser::cancel()
{
    player = "";
    reject();
}

ChooseUser::~ChooseUser()
{
    delete ui;
}

const QString ChooseUser::getPlayer() const
{
    return player;
}
