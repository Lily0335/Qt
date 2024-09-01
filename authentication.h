
#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include "admin.h"
 class Admin;
namespace Ui {
class Authentication;
}

class Authentication : public QDialog
{
    Q_OBJECT

public:
    explicit Authentication(QWidget *parent = nullptr);
    ~Authentication();

private slots:
    void on_loginButton_clicked();

private:
    Ui::Authentication *ui;
    Admin *ptrAdmin;

    bool validateCredentials(const QString& username, const QString& password);
    void showErrorMessage(const QString& message);
};

#endif // AUTHENTICATION_H
