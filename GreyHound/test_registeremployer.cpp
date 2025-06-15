#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QPushButton>
#include <QSignalSpy>
#include <QtTest/QtTest>
#include "registerpageforemployer.h"

class RegisterPageForEmployerTest : public QObject {
    Q_OBJECT

private slots:
    void test_empty_fields();
    void test_invalid_email();
    void test_password_validation_fail();
};

void RegisterPageForEmployerTest::test_empty_fields() {
    QNetworkAccessManager mgr;
    RegisterPageForEmployer w(&mgr);
    w.show();

    auto company =
        w.findChild<QLineEdit *>("registrationLineEditCompanyNameEmployer");
    auto email = w.findChild<QLineEdit *>("registrationLineEditEmailEmployer");
    auto pass =
        w.findChild<QLineEdit *>("registrationLineEditPasswordEmployer");
    auto btn = w.findChild<QPushButton *>("registrationPB_employer");

    QVERIFY(company);
    QVERIFY(email);
    QVERIFY(pass);
    QVERIFY(btn);

    company->setText("");
    email->setText("u@x.y");
    pass->setText("password123");

    QTest::mouseClick(btn, Qt::LeftButton);
    QCOMPARE(company->text().isEmpty(), true);
}

void RegisterPageForEmployerTest::test_invalid_email() {
    QNetworkAccessManager mgr;
    RegisterPageForEmployer w(&mgr);
    w.show();

    auto company =
        w.findChild<QLineEdit *>("registrationLineEditCompanyNameEmployer");
    auto email = w.findChild<QLineEdit *>("registrationLineEditEmailEmployer");
    auto pass =
        w.findChild<QLineEdit *>("registrationLineEditPasswordEmployer");
    auto btn = w.findChild<QPushButton *>("registrationPB_employer");

    company->setText("Comp");
    email->setText("invalid-email");
    pass->setText("Password123!");

    QTest::mouseClick(btn, Qt::LeftButton);
    QVERIFY(email->text().contains("@") == false);
}

void RegisterPageForEmployerTest::test_password_validation_fail() {
    QNetworkAccessManager mgr;
    RegisterPageForEmployer w(&mgr);
    w.show();

    auto company =
        w.findChild<QLineEdit *>("registrationLineEditCompanyNameEmployer");
    auto email = w.findChild<QLineEdit *>("registrationLineEditEmailEmployer");
    auto pass =
        w.findChild<QLineEdit *>("registrationLineEditPasswordEmployer");
    auto btn = w.findChild<QPushButton *>("registrationPB_employer");

    company->setText("Comp");
    email->setText("a@b.c");
    pass->setText("123");

    QTest::mouseClick(btn, Qt::LeftButton);
    QVERIFY(pass->text().length() < 8);
}

QTEST_MAIN(RegisterPageForEmployerTest)
#include "test_registeremployer.moc"
