#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "checkinfo.h"
#include "savingaccount.h"
#include "authentication.h"
#include "admin.h"
#include "depositmoney.h"
#include "withdraw.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    Authentication *ptrAuthentication;
    CheckInfo *ptrCheckInfo;
    DepositMoney *ptrDepositMoney;
    Withdraw    *ptrWithdraw;
};
#endif // MAINWINDOW_H
