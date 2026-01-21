#include "Dialog/logindialog.h"
#include "mainwindow.h"
#include "userservice.h"

#include <QApplication>
#include <QLocale>
#include <QMessageBox>
#include <QTranslator>

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

    bool authenticated = false;
    while (!authenticated) {
        LoginDialog loginDialog;

        QObject::connect(&loginDialog, &LoginDialog::exitRequested, &loginDialog, &LoginDialog::reject);
        QObject::connect(&loginDialog, &LoginDialog::loginRequested, [&]() {
            QString authError;
            if (userService.authenticate(loginDialog.login(), loginDialog.password(), &authError)) {
                authenticated = true;
                loginDialog.accept();
            } else {
                QMessageBox::warning(&loginDialog, QObject::tr("Blad logowania"), authError);
            }
        });

        if (loginDialog.exec() != QDialog::Accepted) {
            return 0;
        }
    }

    MainWindow w;
    w.show();
    return a.exec();
}
