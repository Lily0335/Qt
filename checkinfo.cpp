
#include "checkinfo.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QIODevice>
#include <stdexcept>

CheckInfo::CheckInfo(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Account Information");
    resize(600, 400);

    layout = new QVBoxLayout(this);

    accountDisplay = new QTextEdit(this);
    accountDisplay->setReadOnly(true);
    layout->addWidget(accountDisplay);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    refreshButton = new QPushButton("Refresh", this);
    closeButton = new QPushButton("Close", this);
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addWidget(closeButton);

    layout->addLayout(buttonLayout);

    connect(refreshButton, &QPushButton::clicked, this, &CheckInfo::refreshAccounts);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    refreshAccounts(); // Load accounts initially
}

void CheckInfo::refreshAccounts()
{
    try {
        readAllAccountsFromFile();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to refresh accounts: %1").arg(e.what()));
    }
}

void CheckInfo::readAllAccountsFromFile()
{
    QString fileName = "accounts.dat";
    QFile file(fileName);

    try {
        if (!file.open(QIODevice::ReadOnly)) {
            throw std::runtime_error("Could not open file: " + fileName.toStdString());
        }

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_15);

        Account account;
        QString allAccountsText;
        int accountCount = 0;

        while (!in.atEnd()) {
            qint64 bytesRead = in.readRawData(reinterpret_cast<char*>(&account), RECORD_SIZE);
            if (bytesRead != RECORD_SIZE) {
                if (bytesRead == -1) {
                    throw std::runtime_error("Error reading from file: " + file.errorString().toStdString());
                } else {
                    throw std::runtime_error("Incomplete record read. Expected " + std::to_string(RECORD_SIZE) +
                                             " bytes, but got " + std::to_string(bytesRead));
                }
            }
            allAccountsText += formatAccountRecord(account) + "\n\n";
            accountCount++;
        }

        file.close();

        if (accountCount > 0) {
            accountDisplay->setPlainText(allAccountsText);
        } else {
            accountDisplay->setPlainText("No accounts found in the file.");
        }
    } catch (const std::exception& e) {
        if (file.isOpen()) {
            file.close();
        }
        throw; // Re-throw the exception to be caught in refreshAccounts()
    }
}

QString CheckInfo::formatAccountRecord(const Account& account)
{
    return QString("Account Number: %1\n"
                   "First Name: %2\n"
                   "Last Name: %3\n"
                   "Account Type: %4\n"
                   "Balance: $%5\n"
                   "Date Opened: %6")
        .arg(account.accountNumber)
        .arg(QString::fromUtf8(account.firstName))
        .arg(QString::fromUtf8(account.lastName))
        .arg(QString::fromUtf8(account.accountType))
        .arg(account.balance, 0, 'f', 2)
        .arg(account.dateOpened.toString("dd-MM-yyyy"));
}
