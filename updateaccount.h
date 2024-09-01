
#ifndef UPDATEACCOUNT_H
#define UPDATEACCOUNT_H

#include <QDialog>
#include <QFile>
#include <QMessageBox>
#include "addaccount.h"  // To use the Account struct

namespace Ui {
class UpdateAccount;
}

class UpdateAccount : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateAccount(QWidget *parent = nullptr);
    ~UpdateAccount();

private slots:
    void on_searchButton_clicked();
    void on_updateButton_clicked();

private:
    Ui::UpdateAccount *ui;
    void onSearchClicked();
    void onUpdateClicked();
    bool findAccount(int accountNumber, Account& account);
    bool updateAccountData(const Account& updatedAccount);
    void displayAccountInfo(const Account& account);
    void clearInputFields();
    bool validateInput();
    bool getUpdatedAccountFromInput(Account& updatedAccount);
};

#endif // UPDATEACCOUNT_H
