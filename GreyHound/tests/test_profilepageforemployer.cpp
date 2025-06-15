#include <QLineEdit>
#include <QTextEdit>
#include <QtTest/QtTest>
#include "profilepageforemployer.h"

class ProfilePageForEmployerTest : public QObject {
    Q_OBJECT

private slots:

    void test_setEmployerData() {
        QNetworkAccessManager manager;
        ProfilePageForEmployer page(&manager);

        QString companyName = "TestCorp";
        QString email = "test@example.com";
        QString about = "About company";

        page.setEmployerData(companyName, email, about);

        auto nameEdit = page.findChild<QLineEdit *>("companyNameEdit");
        auto emailEdit = page.findChild<QLabel *>("emailEdit");
        auto aboutEdit = page.findChild<QTextEdit *>("aboutEdit");

        QVERIFY(nameEdit != nullptr);
        QVERIFY(aboutEdit != nullptr);

        QCOMPARE(nameEdit->text(), companyName);
        QCOMPARE(emailEdit->text(), email);
        QCOMPARE(aboutEdit->toPlainText(), about);
    }
};

QTEST_MAIN(ProfilePageForEmployerTest)
#include "test_profilepageforemployer.moc"
