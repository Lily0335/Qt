
#ifndef CHECKINFO_H
#define CHECKINFO_H

#include <QDialog>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QFile>
#include <QDataStream>
#include "addaccount.h"
class CheckInfo : public QDialog
{
    Q_OBJECT

public:
    explicit CheckInfo(QWidget *parent = nullptr);

private slots:
    void refreshAccounts();

private:
    QVBoxLayout *layout;
    QTextEdit *accountDisplay;
    QPushButton *refreshButton;
    QPushButton *closeButton;

    void readAllAccountsFromFile();
    QString formatAccountRecord(const Account& account);

    static const int RECORD_SIZE = sizeof(Account);
};

#endif // CHECKINFO_H
