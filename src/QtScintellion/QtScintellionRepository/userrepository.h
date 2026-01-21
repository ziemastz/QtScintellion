#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <optional>

#include <QSqlDatabase>
#include <QString>

#include "user.h"

class UserRepository
{
public:
    explicit UserRepository(QSqlDatabase database);

    bool ensureSchema();
    bool addUser(const QString &username, const QString &email, const QString &passwordHash);
    bool ensureDefaultAdmin(const QString &username = QStringLiteral("admin"), const QString &passwordHash = QStringLiteral("admin"));
    bool validateCredentials(const QString &login, const QString &password) const;
    std::optional<User> findByLogin(const QString &login) const;
    std::optional<User> findByUsername(const QString &username) const;
    std::optional<User> findByEmail(const QString &email) const;

private:
    User mapUser(const QSqlQuery &query) const;
    bool hasAnyUser() const;
    std::optional<User> findByColumn(const QString &column, const QString &value) const;

    mutable QSqlDatabase m_db;
};

#endif // USERREPOSITORY_H
