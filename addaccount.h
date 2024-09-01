#ifndef ADDACCOUNT_H
#define ADDACCOUNT_H

#include <QDialog>
#include <QFile>
#include <QMessageBox>
#include <QDate>

namespace Ui {
class AddAccount;
}

struct Account {
    int accountNumber;
    char firstName[26];
    char lastName[26];
    char accountType[15];
    double balance;
    QDate dateOpened;
};

class AddAccount : public QDialog
{
    Q_OBJECT

public:
    explicit AddAccount(QWidget *parent = nullptr);
    ~AddAccount();

private slots:
    void on_confirmButton_clicked();

private:
    Ui::AddAccount *ui;
    void onConfirmClicked();
    bool accountNumberExists(int accountNumber);
    void saveAccountData(const Account& account);
    void clearInputFields();
};

#endif // ADDACCOUNT_H

