
#include "updateaccount.h"
#include "ui_updateaccount.h"
#include <QDataStream>
#include <QMessageBox>
#include <QFile>
#include <QTemporaryFile>
#include <algorithm>
#include <stdexcept>
#include <QDebug>

UpdateAccount::UpdateAccount(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UpdateAccount)
{
    ui->setupUi(this);
    ui->updateButton->setEnabled(false);
}

UpdateAccount::~UpdateAccount()
{
    delete ui;
}

void UpdateAccount::on_searchButton_clicked()
{
    onSearchClicked();
}

void UpdateAccount::on_updateButton_clicked()
{
    onUpdateClicked();
}

void UpdateAccount::onSearchClicked()
{
    try {
        qDebug() << "Search button clicked";
        bool ok;
        int accountNumber = ui->accountNumberlineEdit->text().toInt(&ok);
        if (!ok) {
            throw std::runtime_error("Invalid account number!");
        }

        Account account;
        if (findAccount(accountNumber, account)) {
            displayAccountInfo(account);
            ui->updateButton->setEnabled(true);
            qDebug() << "Account found and displayed";
        } else {
            throw std::runtime_error("Account not found.");
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Error searching for account:" << e.what();
        QMessageBox::warning(this, "Error", e.what());
        clearInputFields();
        ui->updateButton->setEnabled(false);
    }
}

void UpdateAccount::onUpdateClicked()
{
    try {
        qDebug() << "Update button clicked";

        if (!validateInput()) {
            throw std::runtime_error("Invalid input. Please check all fields.");
        }

        Account updatedAccount;
        if (!getUpdatedAccountFromInput(updatedAccount)) {
            throw std::runtime_error("Failed to get updated account information.");
        }

        qDebug() << "Attempting to update account:" << updatedAccount.accountNumber;

        if (updateAccountData(updatedAccount)) {
            QMessageBox::information(this, "Success", "Account updated successfully!");
            clearInputFields();
            ui->updateButton->setEnabled(false);
            qDebug() << "Account updated successfully";
        } else {
            throw std::runtime_error("Failed to update account.");
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Error updating account:" << e.what();
        QMessageBox::critical(this, "Error", e.what());
    }
}

bool UpdateAccount::findAccount(int accountNumber, Account& account)
{
    try {
        QFile file("accounts.dat");

        if (!file.open(QIODevice::ReadOnly)) {
            throw std::runtime_error("Failed to open the file.");
        }

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_0);

        while (!in.atEnd()) {
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
    catch (const std::exception& e) {
        qDebug() << "Error finding account:" << e.what();
        QMessageBox::critical(this, "Error", e.what());
        return false;
    }
}

bool UpdateAccount::updateAccountData(const Account& updatedAccount)
{
    try {
        QFile inputFile("accounts.dat");
        QTemporaryFile tempFile;

        if (!inputFile.open(QIODevice::ReadOnly) || !tempFile.open()) {
            throw std::runtime_error("Failed to open files for updating.");
        }

        QDataStream in(&inputFile);
        QDataStream out(&tempFile);
        in.setVersion(QDataStream::Qt_5_0);
        out.setVersion(QDataStream::Qt_5_0);

        bool updated = false;
        while (!in.atEnd()) {
            Account account;
            if (in.readRawData(reinterpret_cast<char*>(&account), sizeof(Account)) == sizeof(Account)) {
                if (account.accountNumber == updatedAccount.accountNumber) {
                    out.writeRawData(reinterpret_cast<const char*>(&updatedAccount), sizeof(Account));
                    updated = true;
                } else {
                    out.writeRawData(reinterpret_cast<const char*>(&account), sizeof(Account));
                }
            } else {
                break;
            }
        }

        inputFile.close();
        tempFile.close();

        if (updated) {
            if (!inputFile.remove()) {
                throw std::runtime_error("Failed to remove the original file.");
            }
            if (!tempFile.copy("accounts.dat")) {
                throw std::runtime_error("Failed to copy the updated data to the file.");
            }
        }

        return updated;
    }
    catch (const std::exception& e) {
        qDebug() << "Error updating account data:" << e.what();
        QMessageBox::critical(this, "Error", e.what());
        return false;
    }
}

void UpdateAccount::displayAccountInfo(const Account& account)
{
    ui->firstNamelineEdit->setText(QString::fromUtf8(account.firstName));
    ui->lastNamelineEdit->setText(QString::fromUtf8(account.lastName));
    ui->accountTypelineEdit->setText(QString::number(account.accountNumber) + " " + QString::fromUtf8(account.accountType));
    ui->balancelineEdit->setText(QString::number(account.balance));
    ui->dateOpenedDateEdit->setDate(account.dateOpened);
}

void UpdateAccount::clearInputFields()
{
    ui->firstNamelineEdit->clear();
    ui->lastNamelineEdit->clear();
    ui->accountTypelineEdit->clear();
    ui->balancelineEdit->clear();
    ui->dateOpenedDateEdit->setDate(QDate::currentDate());
}

bool UpdateAccount::validateInput()
{
    std::string firstNameInput = ui->firstNamelineEdit->text().toStdString();
    std::string lastNameInput = ui->lastNamelineEdit->text().toStdString();
    QString accountInfo = ui->accountTypelineEdit->text();
    QStringList parts = accountInfo.split(" ", Qt::SkipEmptyParts);

    if (parts.size() < 2) {
        throw std::runtime_error("Please enter both account number and type!");
    }

    std::string accountTypeInput = parts.mid(1).join(" ").toStdString();

    if (firstNameInput.length() > 25 || lastNameInput.length() > 25 || accountTypeInput.length() > 14) {
        throw std::runtime_error("Input exceeds maximum allowed length!");
    }

    auto isAlphabetical = [](const std::string& str) {
        return std::all_of(str.begin(), str.end(), [](unsigned char c){ return std::isalpha(c); });
    };

    if (firstNameInput.empty() || lastNameInput.empty() || accountTypeInput.empty() ||
        !isAlphabetical(firstNameInput) || !isAlphabetical(lastNameInput)) {
        throw std::runtime_error("All fields must be filled correctly and names must contain only alphabetical characters!");
    }

    return true;
}

bool UpdateAccount::getUpdatedAccountFromInput(Account& updatedAccount)
{
    try {
        updatedAccount.accountNumber = ui->accountNumberlineEdit->text().toInt();

        QString accountInfo = ui->accountTypelineEdit->text();
        QStringList parts = accountInfo.split(" ", Qt::SkipEmptyParts);
        std::string accountTypeInput = parts.mid(1).join(" ").toStdString();

        std::string firstNameInput = ui->firstNamelineEdit->text().toStdString();
        std::string lastNameInput = ui->lastNamelineEdit->text().toStdString();

        strncpy(updatedAccount.firstName, firstNameInput.c_str(), sizeof(updatedAccount.firstName) - 1);
        updatedAccount.firstName[sizeof(updatedAccount.firstName) - 1] = '\0'; // Ensure null termination

        strncpy(updatedAccount.lastName, lastNameInput.c_str(), sizeof(updatedAccount.lastName) - 1);
        updatedAccount.lastName[sizeof(updatedAccount.lastName) - 1] = '\0'; // Ensure null termination

        strncpy(updatedAccount.accountType, accountTypeInput.c_str(), sizeof(updatedAccount.accountType) - 1);
        updatedAccount.accountType[sizeof(updatedAccount.accountType) - 1] = '\0'; // Ensure null termination
        bool ok;
        updatedAccount.balance = ui->balancelineEdit->text().toDouble(&ok);
        if (!ok) {
            throw std::runtime_error("Invalid balance input!");
        }

        updatedAccount.dateOpened = ui->dateOpenedDateEdit->date();

        return true;
    }
    catch (const std::exception& e) {
        qDebug() << "Error getting updated account from input:" << e.what();
        QMessageBox::critical(this, "Error", e.what());
        return false;
    }
}
