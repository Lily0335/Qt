

#include "searchaccount.h"
#include "ui_searchaccount.h"
#include <QDataStream>

SearchAccount::SearchAccount(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SearchAccount)
{
    ui->setupUi(this);
}

SearchAccount::~SearchAccount()
{
    delete ui;
}

void SearchAccount::on_searchButton_clicked()
{
    onSearchClicked();
}

void SearchAccount::onSearchClicked()
{
    bool ok;
    int accountNumber = ui->accountNumberlineEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid account number!");
        return;
    }

    Account foundAccount;
    try {
        if (searchAccountData(accountNumber, foundAccount)) {
            displayAccountInfo(foundAccount);
        } else {
            QMessageBox::information(this, "Not Found", "Account not found.");
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
    }

    clearInputField();
}

bool SearchAccount::searchAccountData(int accountNumber, Account& foundAccount)
{
    QFile inputFile("accounts.dat");

    try {
        if (!inputFile.open(QIODevice::ReadOnly)) {
            throw std::runtime_error("Error opening the accounts file.");
        }

        QDataStream in(&inputFile);
        in.setVersion(QDataStream::Qt_5_0);

        while (!in.atEnd()) {
            Account account;
            if (in.readRawData(reinterpret_cast<char*>(&account), sizeof(Account)) == sizeof(Account)) {
                if (account.accountNumber == accountNumber) {
                    foundAccount = account;
                    inputFile.close();
                    return true;
                }
            } else {
                break;
            }
        }

        inputFile.close();
        return false;
    } catch (...) {
        if (inputFile.isOpen()) {
            inputFile.close();
        }
        throw; // Re-throw the caught exception
    }
}

void SearchAccount::clearInputField()
{
    ui->accountNumberlineEdit->clear();
}

void SearchAccount::displayAccountInfo(const Account& account)
{
    QString info = QString("Account Number: %1\nfirstName: %2\nlastName: %3\nBalance: $%4")
                       .arg(account.accountNumber)
                       .arg(account.firstName)
                       .arg(account.lastName)
                       .arg(account.balance, 0, 'f', 2);
    QMessageBox::information(this, "Account Information", info);
}
