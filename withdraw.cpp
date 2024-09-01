
#include "withdraw.h"
#include "ui_withdraw.h"
#include <QDataStream>
#include <QMessageBox>
#include <stdexcept>

Withdraw::Withdraw(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Withdraw)
{
    ui->setupUi(this);
}

Withdraw::~Withdraw()
{
    delete ui;
}

void Withdraw::on_withdrawButton_clicked()
{
    onWithdrawClicked();
}

void Withdraw::onWithdrawClicked()
{
    bool ok;
    int accountNumber = ui->accountNumberlineEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid account number!");
        return;
    }

    double amount = ui->amountlineEdit->text().toDouble(&ok);
    if (!ok || amount <= 0) {
        QMessageBox::warning(this, "Input Error", "Invalid withdrawal amount!");
        return;
    }

    if (!accountExists(accountNumber)) {
        QMessageBox::warning(this, "Account Error", "Account does not exist!");
        return;
    }

    try {
        Account account;
        QFile file("accounts.dat");
        if (!file.open(QIODevice::ReadOnly)) {
            throw std::runtime_error("Failed to open the file.");
        }

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_0);

        while (!in.atEnd()) {
            if (in.readRawData(reinterpret_cast<char*>(&account), sizeof(Account)) == sizeof(Account)) {
                if (account.accountNumber == accountNumber) {
                    break;
                }
            } else {
                throw std::runtime_error("Error reading account data.");
            }
        }

        file.close();

        if (account.balance < amount) {
            QMessageBox::warning(this, "Insufficient Funds", "Not enough balance for withdrawal!");
            return;
        }

        account.balance -= amount;
        updateAccountBalance(account);

        QMessageBox::information(this, "Success", "Withdrawal successful!");
        clearInputFields();
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

bool Withdraw::accountExists(int accountNumber)
{
    QFile file("accounts.dat");

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_0);

    while (!in.atEnd()) {
        Account account;
        if (in.readRawData(reinterpret_cast<char*>(&account), sizeof(Account)) == sizeof(Account)) {
            if (account.accountNumber == accountNumber) {
                file.close();
                return true;
            }
        } else {
            break;
        }
    }

    file.close();
    return false;
}

void Withdraw::updateAccountBalance(const Account& account)
{
    QFile file("accounts.dat");

    if (!file.open(QIODevice::ReadWrite)) {
        throw std::runtime_error("Failed to open the file.");
    }

    QDataStream io(&file);
    io.setVersion(QDataStream::Qt_5_0);

    while (!io.atEnd()) {
        qint64 pos = file.pos();
        Account currentAccount;
        if (io.readRawData(reinterpret_cast<char*>(&currentAccount), sizeof(Account)) == sizeof(Account)) {
            if (currentAccount.accountNumber == account.accountNumber) {
                file.seek(pos);
                io.writeRawData(reinterpret_cast<const char*>(&account), sizeof(Account));
                break;
            }
        } else {
            break;
        }
    }

    file.close();
}

void Withdraw::clearInputFields()
{
    ui->accountNumberlineEdit->clear();
    ui->amountlineEdit->clear();
}
