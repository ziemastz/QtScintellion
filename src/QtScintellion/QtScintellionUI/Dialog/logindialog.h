#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    QString login() const;
    QString password() const;

signals:
    void loginRequested();
    void exitRequested();
    void registerRequested();

private:
    void setupConnections();

    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
