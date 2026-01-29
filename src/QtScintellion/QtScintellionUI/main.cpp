#include "Dialog/logindialog.h"
#include "Dialog/registerdialog.h"
#include "mainwindow.h"
#include "userservice.h"

#include <QApplication>
#include <QLocale>
#include <QMessageBox>
#include <QTranslator>

namespace {
bool showLoginFlow(UserService &userService)
{
    while (true) {
        LoginDialog loginDialog;

        QObject::connect(&loginDialog, &LoginDialog::exitRequested, &loginDialog, &LoginDialog::reject);
        QObject::connect(&loginDialog, &LoginDialog::registerRequested, [&]() {
            RegisterDialog regDialog(&loginDialog);
            if (regDialog.exec() != QDialog::Accepted) {
                return;
            }
            if (regDialog.password() != regDialog.passwordConfirmation()) {
                QMessageBox::warning(&loginDialog, QObject::tr("Błąd rejestracji"), QObject::tr("Hasła nie są identyczne."));
                return;
            }
            QString regError;
            if (!userService.registerUser(regDialog.username(), regDialog.email(), regDialog.password(), &regError)) {
                if (regError.isEmpty()) {
                    regError = QObject::tr("Nie udało się zarejestrować użytkownika.");
                }
                QMessageBox::warning(&loginDialog, QObject::tr("Błąd rejestracji"), regError);
            } else {
                QMessageBox::information(&loginDialog, QObject::tr("Rejestracja zakończona"), QObject::tr("Konto zostało utworzone. Możesz się zalogować."));
            }
        });
        QObject::connect(&loginDialog, &LoginDialog::loginRequested, [&]() {
            QString authError;
            if (userService.authenticate(loginDialog.login(), loginDialog.password(), &authError)) {
                loginDialog.accept();
            } else {
                QMessageBox::warning(&loginDialog, QObject::tr("Blad logowania"), authError);
            }
        });

        if (loginDialog.exec() == QDialog::Accepted) {
            return true;
        }
        return false;
    }
}
} // namespace

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "QtScintellionUI_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    UserService userService;
    QString initError;
    if (!userService.initialize(&initError)) {
        QMessageBox::critical(nullptr, QObject::tr("Blad bazy danych"), initError);
        return -1;
    }

    if (!showLoginFlow(userService)) {
        return 0;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
