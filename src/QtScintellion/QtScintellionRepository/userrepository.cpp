#include "userrepository.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

namespace {
constexpr auto kTableName = "users";
constexpr auto kDefaultEmailDomain = "@local";
}

UserRepository::UserRepository(QSqlDatabase database)
    : m_db(std::move(database))
{
}

bool UserRepository::ensureSchema()
{
    if (!m_db.isValid()) {
        return false;
    }
    if (!m_db.isOpen() && !m_db.open()) {
        return false;
    }

    QSqlQuery query(m_db);
    const QString ddl = QStringLiteral(
        "CREATE TABLE IF NOT EXISTS %1 ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE NOT NULL,"
        "email TEXT UNIQUE NOT NULL,"
        "password_hash TEXT NOT NULL,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ")").arg(kTableName);

    return query.exec(ddl);
}

bool UserRepository::addUser(const QString &username, const QString &email, const QString &passwordHash)
{
    if (!ensureSchema()) {
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "INSERT INTO %1 (username, email, password_hash, created_at) "
        "VALUES (:username, :email, :password_hash, CURRENT_TIMESTAMP)")
                      .arg(kTableName));
    query.bindValue(":username", username.trimmed());
    query.bindValue(":email", email.trimmed());
    query.bindValue(":password_hash", passwordHash);

    return query.exec();
}

bool UserRepository::ensureDefaultAdmin(const QString &username, const QString &passwordHash)
{
    if (!ensureSchema()) {
        return false;
    }
    if (hasAnyUser()) {
        return true;
    }

    const QString email = username + QString::fromUtf8(kDefaultEmailDomain);
    return addUser(username, email, passwordHash);
}

bool UserRepository::validateCredentials(const QString &login, const QString &password) const
{
    const auto user = findByLogin(login);
    if (!user.has_value()) {
        return false;
    }
    // NOTE: password stored as plain hash field for now; replace with real hashing.
    return user->passwordHash == password;
}

std::optional<User> UserRepository::findByLogin(const QString &login) const
{
    auto trimmed = login.trimmed();
    if (trimmed.isEmpty()) {
        return std::nullopt;
    }
    auto user = findByUsername(trimmed);
    if (user.has_value()) {
        return user;
    }
    return findByEmail(trimmed);
}

std::optional<User> UserRepository::findByUsername(const QString &username) const
{
    return findByColumn(QStringLiteral("username"), username.trimmed());
}

std::optional<User> UserRepository::findByEmail(const QString &email) const
{
    return findByColumn(QStringLiteral("email"), email.trimmed());
}

User UserRepository::mapUser(const QSqlQuery &query) const
{
    User user;
    user.id = query.value(QStringLiteral("id")).toLongLong();
    user.username = query.value(QStringLiteral("username")).toString();
    user.email = query.value(QStringLiteral("email")).toString();
    user.passwordHash = query.value(QStringLiteral("password_hash")).toString();
    user.createdAt = query.value(QStringLiteral("created_at")).toDateTime();
    return user;
}

bool UserRepository::hasAnyUser() const
{
    if (!m_db.isValid()) {
        return false;
    }
    if (!m_db.isOpen() && !m_db.open()) {
        return false;
    }

    QSqlQuery query(m_db);
    if (!query.exec(QStringLiteral("SELECT COUNT(*) FROM %1").arg(kTableName))) {
        return false;
    }
    if (query.next()) {
        return query.value(0).toLongLong() > 0;
    }
    return false;
}

std::optional<User> UserRepository::findByColumn(const QString &column, const QString &value) const
{
    if (!m_db.isValid() || value.isEmpty()) {
        return std::nullopt;
    }
    if (!m_db.isOpen() && !m_db.open()) {
        return std::nullopt;
    }

    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "SELECT id, username, email, password_hash, created_at "
        "FROM %1 WHERE %2 = :value LIMIT 1")
                      .arg(kTableName, column));
    query.bindValue(":value", value);
    if (!query.exec()) {
        return std::nullopt;
    }
    if (query.next()) {
        return mapUser(query);
    }
    return std::nullopt;
}
