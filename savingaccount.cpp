
#include "savingaccount.h"
#include "ui_savingaccount.h"
#include <QDataStream>
#include <QMessageBox>
#include <cstring>
#include <stdexcept>
#include <algorithm>

SavingAccount::SavingAccount(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SavingAccount)
{
    ui->setupUi(this);
}

SavingAccount::~SavingAccount()
{
    delete ui;
}

void SavingAccount::on_confirmButton_clicked()
{
    onConfirmClicked();
}

void SavingAccount::onConfirmClicked()
{
    SavingAccountData account;

    bool ok;
    account.accountNumber = ui->accountNumberlineEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid account number!");
        return;
    }

    std::string firstNameInput = ui->firstNamelineEdit->text().toStdString();
    std::string lastNameInput = ui->lastNamelineEdit->text().toStdString();

    if (firstNameInput.length() > 25 || lastNameInput.length() > 25) {
        QMessageBox::warning(this, "Input Error", "Name input exceeds maximum allowed length (25 characters)!");
        return;
    }

    std::snprintf(account.firstName, sizeof(account.firstName), "%s", firstNameInput.c_str());
    std::snprintf(account.lastName, sizeof(account.lastName), "%s", lastNameInput.c_str());

    if (accountNumberExists(account.accountNumber)) {
        QMessageBox::warning(this, "Input Error", "Account number already exists. Please enter a valid account number.");
        return;
    }

    auto isAlphabetical = [](const std::string& str) {
        return std::all_of(str.begin(), str.end(), [](unsigned char c){ return std::isalpha(c); });
    };

    if (firstNameInput.empty() || lastNameInput.empty() ||
        !isAlphabetical(firstNameInput) || !isAlphabetical(lastNameInput)) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled correctly and names must contain only alphabetical characters!");
        return;
    }

    account.balance = ui->balancelineEdit->text().toDouble();
    account.dateOpened = ui->dateOpenedDateEdit->date();

    try {
        addInterest(account);
        saveAccountData(account);
        QMessageBox::information(this, "Success", "Savings account added successfully with interest!");
        clearInputFields();
    } catch (const InsufficientBalanceException &e) {
        QMessageBox::warning(this, "Interest Not Added", e.what());
        try {
            saveAccountData(account);
            QMessageBox::information(this, "Success", "Savings account added successfully without interest.");
            clearInputFields();
        } catch (const std::exception &e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void SavingAccount::addInterest(SavingAccountData& account)
{
    const double MINIMUM_BALANCE = 5000.0;
    const double INTEREST_RATE = 0.05; // 5% interest rate

    if (account.balance < MINIMUM_BALANCE) {
        throw InsufficientBalanceException();
    }

    account.balance += account.balance * INTEREST_RATE;
}

void SavingAccount::clearInputFields()
{
    ui->firstNamelineEdit->clear();
    ui->lastNamelineEdit->clear();
    ui->accountNumberlineEdit->clear();
    ui->balancelineEdit->clear();
    ui->dateOpenedDateEdit->setDate(QDate::currentDate());
}

void SavingAccount::saveAccountData(const SavingAccountData& account)
{
    QFile file("savings_accounts.dat");

    if (!file.open(QIODevice::Append)) {
        throw std::runtime_error("Failed to open the file.");
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_0);

    out.writeRawData(reinterpret_cast<const char*>(&account), sizeof(SavingAccountData));

    if (file.error() != QFileDevice::NoError) {
        throw std::runtime_error("Failed to write data to the file.");
    }

    file.close();
}

bool SavingAccount::accountNumberExists(int accountNumber)
{
    QFile file("savings_accounts.dat");

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_0);

    while (!in.atEnd()) {
        SavingAccountData account;
        if (in.readRawData(reinterpret_cast<char*>(&account), sizeof(SavingAccountData)) == sizeof(SavingAccountData)) {
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
