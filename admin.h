#ifndef ADMIN_H
#define ADMIN_H

#include "addaccount.h"
#include <QDialog>
#include "removeaccount.h"
#include "updateaccount.h"
#include "searchaccount.h"
namespace Ui {
class Admin;
}

class Admin : public QDialog
{
    Q_OBJECT

public:
    explicit Admin(QWidget *parent = nullptr);
    ~Admin();

private slots:
    void on_addAccountButton_clicked();
    void on_removeAccountButton_clicked();
    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::Admin *ui;
    AddAccount *ptrAddAccount;
    RemoveAccount *ptrRemoveAccount;
    UpdateAccount *ptrUpdateAccount;
    SearchAccount *ptrSearchAccount;
};

#endif // ADMIN_H
