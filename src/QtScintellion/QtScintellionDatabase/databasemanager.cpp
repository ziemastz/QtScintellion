#include "databasemanager.h"

#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QByteArray>

namespace {
constexpr auto kConnectionName = "QtScintellionConnection";
constexpr auto kDatabaseFile = "qtscintellion.db";
}

DatabaseManager::DatabaseManager()
{
    const QString filePath = resolveDatabaseFilePath();
    m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QString::fromUtf8(kConnectionName));
    m_db.setDatabaseName(filePath);
}

DatabaseManager &DatabaseManager::instance()
{
    static DatabaseManager mgr;
    return mgr;
}

QSqlDatabase DatabaseManager::database() const
{
    return m_db;
}

QString DatabaseManager::databaseFilePath() const
{
    return m_db.databaseName();
}

QString DatabaseManager::resolveDatabaseFilePath() const
{
    const QByteArray envPath = qgetenv("QTSCINTELLION_DB_PATH");
    if (!envPath.isEmpty()) {
        return QString::fromUtf8(envPath);
    }

    const QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataDir);
    if (!dir.exists()) {
        dir.mkpath(QStringLiteral("."));
    }
    return dir.filePath(QString::fromUtf8(kDatabaseFile));
}

bool DatabaseManager::open()
{
    if (m_db.isOpen()) {
        return true;
    }
    return m_db.open();
}
