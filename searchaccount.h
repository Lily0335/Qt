

#ifndef SEARCHACCOUNT_H
#define SEARCHACCOUNT_H

#include <QDialog>
#include <QFile>
#include <QMessageBox>
#include "addaccount.h"  // To use the Account struct

namespace Ui {
class SearchAccount;
}

class SearchAccount : public QDialog
{
    Q_OBJECT

public:
    explicit SearchAccount(QWidget *parent = nullptr);
    ~SearchAccount();

private slots:
    void on_searchButton_clicked();

private:
    Ui::SearchAccount *ui;
    void onSearchClicked();
    bool searchAccountData(int accountNumber, Account& foundAccount);
    void clearInputField();
    void displayAccountInfo(const Account& account);
};

#endif // SEARCHACCOUNT_H
