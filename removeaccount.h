
#ifndef REMOVEACCOUNT_H
#define REMOVEACCOUNT_H

#include <QDialog>
#include <QFile>
#include <QMessageBox>
#include "addaccount.h"  // To use the Account struct

namespace Ui {
class RemoveAccount;
}

class RemoveAccount : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveAccount(QWidget *parent = nullptr);
    ~RemoveAccount();

private slots:
    void on_removeButton_clicked();

private:
    Ui::RemoveAccount *ui;
    void onRemoveClicked();
    bool removeAccountData(int accountNumber);
    void clearInputField();
};

#endif // REMOVEACCOUNT_H
