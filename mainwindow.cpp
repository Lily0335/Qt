#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ptrCheckInfo = new CheckInfo();
    ptrAuthentication = new Authentication();
    ptrDepositMoney = new DepositMoney();
    ptrWithdraw = new Withdraw();

}

MainWindow::~MainWindow()
{

    delete ptrCheckInfo;
    delete ptrAuthentication;
    delete ptrDepositMoney;
    delete ptrWithdraw;
    delete ui;
}






void MainWindow::on_pushButton_clicked()
{
    ptrAuthentication ->show();
}


void MainWindow::on_pushButton_2_clicked()
{
    ptrCheckInfo ->show();
}


void MainWindow::on_pushButton_4_clicked()
{
    ptrDepositMoney -> show();
}


void MainWindow::on_pushButton_3_clicked()
{
    ptrWithdraw ->show();
}

