
#ifndef SAVINGACCOUNT_H
#define SAVINGACCOUNT_H

#include <QDialog>
#include <QFile>
#include <QMessageBox>
#include <QDate>

namespace Ui {
class SavingAccount;
}

struct SavingAccountData {
    int accountNumber;
    char firstName[26];
    char lastName[26];
    double balance;
    QDate dateOpened;
};

class SavingAccount : public QDialog
{
    Q_OBJECT

public:
    explicit SavingAccount(QWidget *parent = nullptr);
    ~SavingAccount();

private slots:
    void on_confirmButton_clicked();

private:
    Ui::SavingAccount *ui;
    void onConfirmClicked();
    bool accountNumberExists(int accountNumber);
    void saveAccountData(const SavingAccountData& account);
    void clearInputFields();
    void addInterest(SavingAccountData& account);

    class InsufficientBalanceException : public std::exception {
    public:
        const char* what() const noexcept override {
            return "Insufficient balance for interest calculation (minimum $5000 required).";
        }
    };
};

#endif // SAVINGSACCOUNT_H

