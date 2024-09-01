// #ifndef DEPOSITMONEY_H
// #define DEPOSITMONEY_H

// #include <QDialog>

// namespace Ui {
// class DepositMoney;
// }

// class DepositMoney : public QDialog
// {
//     Q_OBJECT

// public:
//     explicit DepositMoney(QWidget *parent = nullptr);
//     ~DepositMoney();

// private:
//     Ui::DepositMoney *ui;
// };

// #endif // DEPOSITMONEY_H
// depositmoney.h

#ifndef DEPOSITMONEY_H
#define DEPOSITMONEY_H

#include <QDialog>
#include "addaccount.h" // Include the AddAccount header to access the Account struct

namespace Ui {
class DepositMoney;
}

class DepositMoney : public QDialog
{
    Q_OBJECT

public:
    explicit DepositMoney(QWidget *parent = nullptr);
    ~DepositMoney();

private slots:
    void on_depositButton_clicked();

private:
    Ui::DepositMoney *ui;
    double depositToAccount(int accountNumber, double depositAmount);
    void clearInputFields();
};

#endif // DEPOSITMONEY_H
