
#include "removeaccount.h"
#include "ui_removeaccount.h"
#include <QDataStream>
#include <QMessageBox>
#include <QFile>
#include <QTemporaryFile>

RemoveAccount::RemoveAccount(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RemoveAccount)
{
    ui->setupUi(this);
}

RemoveAccount::~RemoveAccount()
{
    delete ui;
}

void RemoveAccount::on_removeButton_clicked()
{
    onRemoveClicked();
}

void RemoveAccount::onRemoveClicked()
{
    bool ok;
    int accountNumber = ui->accountNumberLineEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid account number!");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Removal", "Are you sure you want to remove this account?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (removeAccountData(accountNumber)) {
            QMessageBox::information(this, "Success", "Account removed successfully!");
            clearInputField();
        } else {
            QMessageBox::critical(this, "Error", "Failed to remove account. Account may not exist.");
        }
    }
}

bool RemoveAccount::removeAccountData(int accountNumber)
{
    QFile inputFile("accounts.dat");
    QTemporaryFile tempFile;

    if (!inputFile.open(QIODevice::ReadOnly) || !tempFile.open()) {
        return false;
    }

    QDataStream in(&inputFile);
    QDataStream out(&tempFile);
    in.setVersion(QDataStream::Qt_5_0);
    out.setVersion(QDataStream::Qt_5_0);

    bool removed = false;
    while (!in.atEnd()) {
        Account account;
        if (in.readRawData(reinterpret_cast<char*>(&account), sizeof(Account)) == sizeof(Account)) {
            if (account.accountNumber != accountNumber) {
                out.writeRawData(reinterpret_cast<const char*>(&account), sizeof(Account));
            } else {
                removed = true;
            }
        } else {
            break;
        }
    }

    inputFile.close();
    tempFile.close();

    if (removed) {
        inputFile.remove();
        tempFile.copy("accounts.dat");
    }

    return removed;
}

void RemoveAccount::clearInputField()
{
    ui->accountNumberLineEdit->clear();
}
