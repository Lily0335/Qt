
#include "depositmoney.h"
#include "ui_depositmoney.h"
#include <QDataStream>
#include <QMessageBox>
#include <QFile>
#include <stdexcept>

DepositMoney::DepositMoney(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DepositMoney)
{
    ui->setupUi(this);
}

DepositMoney::~DepositMoney()
{
    delete ui;
}

void DepositMoney::on_depositButton_clicked()
{
    bool ok;
    int accountNumber = ui->accountNumberlineEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid account number!");
        return;
    }

    double depositAmount = ui->depositAmountlineEdit->text().toDouble(&ok);
    if (!ok || depositAmount <= 0) {
        QMessageBox::warning(this, "Input Error", "Invalid deposit amount!");
        return;
    }

    try {
        double newBalance = depositToAccount(accountNumber, depositAmount);
        QMessageBox::information(this, "Success",
                                 QString("Deposit successful!\nNew balance: $%1").arg(newBalance, 0, 'f', 2));
        clearInputFields();
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

double DepositMoney::depositToAccount(int accountNumber, double depositAmount)
{
    QFile file("accounts.dat");
    if (!file.open(QIODevice::ReadWrite)) {
        throw std::runtime_error("Failed to open the file.");
    }

    QDataStream io(&file);
    io.setVersion(QDataStream::Qt_5_0);

    bool found = false;
    double newBalance = 0;

    while (!io.atEnd()) {
        qint64 currentPos = file.pos();
        Account account;
        if (io.readRawData(reinterpret_cast<char*>(&account), sizeof(Account)) == sizeof(Account)) {
            if (account.accountNumber == accountNumber) {
                account.balance += depositAmount;
                newBalance = account.balance;
                file.seek(currentPos);
                io.writeRawData(reinterpret_cast<const char*>(&account), sizeof(Account));
                found = true;
                break;
            }
        } else {
            break;
        }
    }

    file.close();

    if (!found) {
        throw std::runtime_error("Account not found.");
    }

    return newBalance;
}

void DepositMoney::clearInputFields()
{
    ui->accountNumberlineEdit->clear();
    ui->depositAmountlineEdit->clear();
}
