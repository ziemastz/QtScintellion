#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <QString>

class UserRepository;

class UserService
{
public:
    UserService();
    ~UserService();

    bool initialize(QString *errorMessage = nullptr);
    bool authenticate(const QString &login, const QString &password, QString *errorMessage = nullptr);

private:
    UserRepository *m_userRepository;
};

#endif // USERSERVICE_H
