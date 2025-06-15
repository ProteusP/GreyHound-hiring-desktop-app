#include <QLineEdit>
#include <QPushButton>
#include <QtTest/QtTest>
#include "registerpageforcandidate.h"

class RegisterPageForCandidateTest : public QObject {
    Q_OBJECT

private slots:

    void test_register_button_click() {
        QNetworkAccessManager mgr;
        RegisterPageForCandidate w(&mgr);
        w.show();

        auto name = w.findChild<QLineEdit *>("registrationLineNameCandidate");
        auto surname =
            w.findChild<QLineEdit *>("registrationLineSurnameCandidate");
        auto email = w.findChild<QLineEdit *>("registrationLineEmailCandidate");
        auto pass =
            w.findChild<QLineEdit *>("registrationLinePasswordCandidate");
        auto btn = w.findChild<QPushButton *>("PBregistrationCandidate");

        QVERIFY(name);
        QVERIFY(surname);
        QVERIFY(email);
        QVERIFY(pass);
        QVERIFY(btn);

        name->setText("John");
        surname->setText("Doe");
        email->setText("john@d.com");
        pass->setText("12345678");

        QTest::mouseClick(btn, Qt::LeftButton);
        QVERIFY(pass->text().length() >= 8);
    }
};

QTEST_MAIN(RegisterPageForCandidateTest)
#include "test_registercandidate.moc"
