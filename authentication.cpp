
#include "authentication.h"
#include "ui_authentication.h"

Authentication::Authentication(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Authentication)
    , ptrAdmin(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Login");
}

Authentication::~Authentication()
{
    delete ui;
    delete ptrAdmin;
}

void Authentication::on_loginButton_clicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (validateCredentials(username, password))
    {
        // Authentication successful
        accept(); // Close the dialog with Accept result

        // Create and show Admin window
        ptrAdmin = new Admin(this);
        ptrAdmin->show();
    }
    else
    {
        // Authentication failed
        showErrorMessage("Invalid username or password. Please try again.");
    }
}

bool Authentication::validateCredentials(const QString& username, const QString& password)
{
    // Simple hardcoded credentials
    const QString validUsername = "admin";
    const QString validPassword = "password123";

    // Check if the entered credentials match the valid ones
    return (username == validUsername && password == validPassword);
}

void Authentication::showErrorMessage(const QString& message)
{
    QMessageBox::warning(this, "Login Error", message);
}
