#include <QtTest>

#include "databasemanager.h"
#include "userrepository.h"
#include "userservice.h"

#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTemporaryDir>

class UsersTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        // Use dedicated temp DB file for tests before touching DatabaseManager.
        QVERIFY(tmpDir.isValid());
        QString dbPath = tmpDir.filePath("test_users.db");
        qputenv("QTSCINTELLION_DB_PATH", dbPath.toUtf8());

        // Ensure clean start.
        QFile::remove(dbPath);

        auto &dbMgr = DatabaseManager::instance();
        QVERIFY(dbMgr.open());
        QCOMPARE(dbMgr.databaseFilePath(), dbPath);

        repo = std::make_unique<UserRepository>(dbMgr.database());
        QVERIFY(repo->ensureSchema());
        clearUsers();
    }

    void cleanup()
    {
        clearUsers();
    }

    void test_defaultAdmin_created()
    {
        QVERIFY(repo->ensureDefaultAdmin());
        QVERIFY(repo->validateCredentials("admin", "admin"));
    }

    void test_add_and_find_user()
    {
        QVERIFY(repo->addUser("user1", "user1@example.com", "secret"));

        auto foundByLogin = repo->findByLogin("user1");
        QVERIFY(foundByLogin.has_value());
        QCOMPARE(foundByLogin->username, QString("user1"));

        auto foundByEmail = repo->findByLogin("user1@example.com");
        QVERIFY(foundByEmail.has_value());
        QCOMPARE(foundByEmail->email, QString("user1@example.com"));
    }

    void test_user_service_authenticate()
    {
        clearUsers();
        UserService service;
        QString err;
        QVERIFY(service.initialize(&err));
        QVERIFY(service.authenticate("admin", "admin", &err) || err.isEmpty());
        QVERIFY(service.authenticate("admin", "admin"));

        // New user path.
        QVERIFY(repo->addUser("alice", "alice@example.com", "pw"));
        QVERIFY(service.authenticate("alice", "pw"));
        QVERIFY(service.authenticate("alice@example.com", "pw"));
        QVERIFY(!service.authenticate("alice", "badpw"));
    }

private:
    void clearUsers()
    {
        QSqlQuery query(DatabaseManager::instance().database());
        query.exec(QStringLiteral("DELETE FROM users"));
    }

    QTemporaryDir tmpDir;
    std::unique_ptr<UserRepository> repo;
};

QTEST_MAIN(UsersTest)
#include "tst_users.moc"
