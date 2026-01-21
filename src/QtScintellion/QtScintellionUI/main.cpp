#include "Dialog/logindialog.h"
#include "mainwindow.h"
#include "databasemanager.h"
#include "userrepository.h"

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

    auto &dbManager = DatabaseManager::instance();
    if (!dbManager.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Błąd bazy danych"), QObject::tr("Nie można otworzyć bazy danych."));
        return -1;
    }

    UserRepository userRepo(dbManager.database());
    userRepo.ensureSchema();
    userRepo.ensureDefaultAdmin();

    bool authenticated = false;
    while (!authenticated) {
        LoginDialog loginDialog;

        QObject::connect(&loginDialog, &LoginDialog::exitRequested, &loginDialog, &LoginDialog::reject);
        QObject::connect(&loginDialog, &LoginDialog::loginRequested, [&]() {
            if (userRepo.validateCredentials(loginDialog.login(), loginDialog.password())) {
                authenticated = true;
                loginDialog.accept();
            } else {
                QMessageBox::warning(&loginDialog, QObject::tr("Błąd logowania"), QObject::tr("Nieprawidłowy login lub hasło."));
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
