#include "userservice.h"

#include "databasemanager.h"
#include "userrepository.h"

#include <QScopedPointer>

UserService::UserService()
    : m_userRepository(nullptr)
{
}

UserService::~UserService()
{
    delete m_userRepository;
}

bool UserService::initialize(QString *errorMessage)
{
    auto &dbManager = DatabaseManager::instance();
    if (!dbManager.open()) {
        if (errorMessage) {
            *errorMessage = QObject::tr("Nie można otworzyć bazy danych.");
        }
        return false;
    }

    auto db = dbManager.database();
    auto repo = std::make_unique<UserRepository>(db);

    if (!repo->ensureSchema()) {
        if (errorMessage) {
            *errorMessage = QObject::tr("Nie można utworzyć schematu użytkowników.");
        }
        return false;
    }
    if (!repo->ensureDefaultAdmin()) {
        if (errorMessage) {
            *errorMessage = QObject::tr("Nie można utworzyć domyślnego użytkownika admin.");
        }
        return false;
    }

    m_userRepository = repo.release();
    return true;
}

bool UserService::authenticate(const QString &login, const QString &password, QString *errorMessage)
{
    if (!m_userRepository) {
        if (errorMessage) {
            *errorMessage = QObject::tr("Serwis użytkownika nie został zainicjalizowany.");
        }
        return false;
    }
    const bool ok = m_userRepository->validateCredentials(login, password);
    if (!ok && errorMessage) {
        *errorMessage = QObject::tr("Nieprawidłowy login lub hasło.");
    }
    return ok;
}

bool UserService::registerUser(const QString &username, const QString &email, const QString &password, QString *errorMessage)
{
    if (!m_userRepository) {
        if (errorMessage) {
            *errorMessage = QObject::tr("Serwis użytkownika nie został zainicjalizowany.");
        }
        return false;
    }
    if (username.trimmed().isEmpty() || email.trimmed().isEmpty() || password.isEmpty()) {
        if (errorMessage) {
            *errorMessage = QObject::tr("Uzupełnij nazwę użytkownika, email i hasło.");
        }
        return false;
    }
    if (m_userRepository->findByUsername(username.trimmed()).has_value()) {
        if (errorMessage) {
            *errorMessage = QObject::tr("Użytkownik o podanej nazwie już istnieje.");
        }
        return false;
    }
    if (m_userRepository->findByEmail(email.trimmed()).has_value()) {
        if (errorMessage) {
            *errorMessage = QObject::tr("Adres email jest już zajęty.");
        }
        return false;
    }
    if (!m_userRepository->addUser(username, email, password)) {
        if (errorMessage) {
            *errorMessage = QObject::tr("Nie udało się utworzyć użytkownika.");
        }
        return false;
    }
    return true;
}
