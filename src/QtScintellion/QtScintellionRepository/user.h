#ifndef USER_H
#define USER_H

#include <QDateTime>
#include <QString>

struct User
{
    qint64 id = 0;
    QString username;
    QString email;
    QString passwordHash;
    QDateTime createdAt;

    bool isValid() const { return id > 0 && !username.isEmpty(); }
};

#endif // USER_H
