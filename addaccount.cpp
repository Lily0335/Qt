#include "addaccount.h"
#include "ui_addaccount.h"
#include <QDataStream>
#include <QMessageBox>
#include <cstring>
#include <stdexcept>
#include <algorithm>


const double SAVINGS_INTEREST_RATE = 0.05; // 5% interest

AddAccount::AddAccount(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddAccount)
{
    ui->setupUi(this);
}

AddAccount::~AddAccount()
{
    delete ui;
}

void AddAccount::on_confirmButton_clicked()
{
    onConfirmClicked();
}

void AddAccount::onConfirmClicked()
{
    Account account;

    // Parse account number and type from accountTypelineEdit
    QString accountInfo = ui->accountTypelineEdit->text();
    QStringList parts = accountInfo.split(" ", Qt::SkipEmptyParts);
    if (parts.size() < 2) {
        QMessageBox::warning(this, "Input Error", "Please enter both account number and type!");
        return;
    }

    bool ok;
    account.accountNumber = parts[0].toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Input Error", "Invalid account number!");
        return;
    }

    std::string accountTypeInput = parts.mid(1).join(" ").toStdString();

    std::string firstNameInput = ui->firstNamelineEdit->text().toStdString();
    std::string lastNameInput = ui->lastNamelineEdit->text().toStdString();

    if (firstNameInput.length() > 25 || lastNameInput.length() > 25 || accountTypeInput.length() > 14) {
        QMessageBox::warning(this, "Input Error", "Input exceeds maximum allowed length!");
        return;
    }

    strncpy(account.firstName, firstNameInput.c_str(), sizeof(account.firstName) - 1);
    account.firstName[sizeof(account.firstName) - 1] = '\0';

    strncpy(account.lastName, lastNameInput.c_str(), sizeof(account.lastName) - 1);
    account.lastName[sizeof(account.lastName) - 1] = '\0';

    strncpy(account.accountType, accountTypeInput.c_str(), sizeof(account.accountType) - 1);
    account.accountType[sizeof(account.accountType) - 1] = '\0';

    if (accountNumberExists(account.accountNumber)) {
        QMessageBox::warning(this, "Input Error", "Account number already exists. Please enter a valid account number.");
        return;
    }

    auto isAlphabetical = [](const std::string& str) {
        return std::all_of(str.begin(), str.end(), [](unsigned char c){ return std::isalpha(c); });
    };

    if (firstNameInput.empty() || lastNameInput.empty() || accountTypeInput.empty() ||
        !isAlphabetical(firstNameInput) || !isAlphabetical(lastNameInput)) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled correctly and names must contain only alphabetical characters!");
        return;
    }

    account.balance = ui->balancelineEdit->text().toDouble();
    account.dateOpened = ui->dateOpenedDateEdit->date();

    // Check if the account is a savings account with balance > 5000
    if (QString(account.accountType).toLower() == "saving" && account.balance > 5000) {
        double interest = account.balance * SAVINGS_INTEREST_RATE;
        account.balance += interest;
        QMessageBox::information(this, "Interest Added",
                                 QString("Interest of $%1 has been added to the account.").arg(interest, 0, 'f', 2));
    }

    try {
        saveAccountData(account);
        QMessageBox::information(this, "Success", "Account added successfully!");
        clearInputFields();
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void AddAccount::clearInputFields()
{
    ui->firstNamelineEdit->clear();
    ui->lastNamelineEdit->clear();
    ui->accountTypelineEdit->clear();
    ui->balancelineEdit->clear();
    ui->dateOpenedDateEdit->setDate(QDate::currentDate());
}

void AddAccount::saveAccountData(const Account& account)
{
    QFile file("accounts.dat");

    if (!file.open(QIODevice::Append)) {
        throw std::runtime_error("Failed to open the file.");
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_0);

    out.writeRawData(reinterpret_cast<const char*>(&account), sizeof(Account));

    if (file.error() != QFileDevice::NoError) {
        throw std::runtime_error("Failed to write data to the file.");
    }

    file.close();
}

bool AddAccount::accountNumberExists(int accountNumber)
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
