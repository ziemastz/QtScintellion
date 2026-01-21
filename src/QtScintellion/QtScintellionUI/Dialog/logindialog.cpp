#include "logindialog.h"
#include "ui_logindialog.h"

#include <QPushButton>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setupConnections();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

QString LoginDialog::login() const
{
    return ui->usernameLineEdit->text().trimmed();
}

QString LoginDialog::password() const
{
    return ui->passwordLineEdit->text();
}

void LoginDialog::setupConnections()
{
    connect(ui->loginPushButton, &QPushButton::clicked, this, &LoginDialog::loginRequested);
    connect(ui->exitPushButton, &QPushButton::clicked, this, &LoginDialog::exitRequested);
    connect(ui->registerPushButton, &QPushButton::clicked, this, [this]() {
        // Future registration handling can go here.
    });
}
