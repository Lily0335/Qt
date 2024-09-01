
#ifndef WITHDRAW_H
#define WITHDRAW_H

#include <QDialog>
#include "addaccount.h"

namespace Ui {
class Withdraw;
}

class Withdraw : public QDialog
{
    Q_OBJECT

public:
    explicit Withdraw(QWidget *parent = nullptr);
    ~Withdraw();

private slots:
    void on_withdrawButton_clicked();

private:
    Ui::Withdraw *ui;
    void onWithdrawClicked();
    bool accountExists(int accountNumber);
    void updateAccountBalance(const Account& account);
    void clearInputFields();
};

#endif // WITHDRAW_H
