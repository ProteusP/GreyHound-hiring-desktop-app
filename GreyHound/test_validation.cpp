#include <QtTest>
#include "validation.h"

class ValidationTest : public QObject {
    Q_OBJECT
private slots:
    void test_valid_emails();
    void test_invalid_emails();
    void test_strong_passwords();
    void test_weak_passwords();
};

void ValidationTest::test_valid_emails() {
    QVERIFY(isEmailValid("user@example.com"));
    QVERIFY(isEmailValid("first.last@domain.co"));
    QVERIFY(isEmailValid("name+alias@sub.domain.org"));
}

void ValidationTest::test_invalid_emails() {
    QVERIFY(!isEmailValid("plainaddress"));
    QVERIFY(!isEmailValid("user@.com"));
    QVERIFY(!isEmailValid("user@domain"));
    QVERIFY(!isEmailValid("@nodomain.com"));
}

void ValidationTest::test_strong_passwords() {
    QVERIFY(isPasswordValid("Secur3!Pass"));
    QVERIFY(isPasswordValid("Zxcv1234$"));
}

void ValidationTest::test_weak_passwords() {
    QVERIFY(!isPasswordValid("password"));
    QVERIFY(!isPasswordValid("12345678"));
    QVERIFY(!isPasswordValid("short"));
    QVERIFY(!isPasswordValid("abcdefgH"));
}

QTEST_MAIN(ValidationTest)
#include "test_validation.moc"
