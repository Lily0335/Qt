
#include "admin.h"
#include "ui_admin.h"
#include "addaccount.h"

Admin::Admin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Admin)
    , ptrAddAccount(new AddAccount())
    , ptrRemoveAccount(new RemoveAccount())
    ,ptrUpdateAccount(new UpdateAccount())
    ,ptrSearchAccount(new SearchAccount())
{
    ui->setupUi(this);
}

Admin::~Admin()
{
    delete ptrAddAccount;
    delete ptrRemoveAccount;
    delete ptrSearchAccount;
    delete ptrUpdateAccount;
    delete ui;
}

void Admin::on_addAccountButton_clicked()
{
    ptrAddAccount->show();
}


void Admin::on_removeAccountButton_clicked()
{
    ptrRemoveAccount->show();
}




void Admin::on_pushButton_2_clicked()
{
    ptrUpdateAccount -> show();
}


void Admin::on_pushButton_5_clicked()
{
    ptrSearchAccount ->show();
}

