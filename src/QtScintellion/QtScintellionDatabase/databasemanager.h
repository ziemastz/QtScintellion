#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>

class DatabaseManager
{
public:
    static DatabaseManager &instance();

    QSqlDatabase database() const;
    QString databaseFilePath() const;

    bool open();

private:
    DatabaseManager();
    QString resolveDatabaseFilePath() const;

    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
