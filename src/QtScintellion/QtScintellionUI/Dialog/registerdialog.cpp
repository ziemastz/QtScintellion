#include "registerdialog.h"
#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

QString RegisterDialog::username() const
{
    return ui->usernameLineEdit->text().trimmed();
}

QString RegisterDialog::email() const
{
    return ui->emailLineEdit->text().trimmed();
}

QString RegisterDialog::password() const
{
    return ui->passwordLineEdit->text();
}

QString RegisterDialog::passwordConfirmation() const
{
    return ui->confirmPasswordLineEdit->text();
}
