#ifndef CHOOSEUSER_H
#define CHOOSEUSER_H

#include <QDialog>

namespace Ui {
    class ChoseUser;
}

class ChooseUser : public QDialog
{
    Q_OBJECT

public:
    ChooseUser(QWidget *parent = 0, QStringList * users = 0);
    ~ChooseUser();

    const QString getPlayer() const;

private:

    Ui::ChoseUser *ui;
    QString player;

private slots:

    void ok();
    void cancel();

};

#endif // CHOOSEUSER_H
