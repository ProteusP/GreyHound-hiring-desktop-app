#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QtTest/QtTest>
#include "profilepageforcandidate.h"

class ProfilePageForCandidateTest : public QObject {
    Q_OBJECT

private slots:

    void test_setCandidateData() {
        QNetworkAccessManager manager;
        ProfilePageForCandidate page(&manager);

        QString name = "Глеб";
        QString surname = "Легостаев";
        QString email = "lol@example.com";
        QString phone = "79248757766";
        QString place = "Алдан";
        qint16 searchStatus = 2;
        QString faculty = "шифт";
        QString university = "ПМИ";
        qint16 experience = 3;

        page.setCandidateData(
            name, email, surname, phone, place, searchStatus, faculty,
            university, experience
        );

        auto nameEdit = page.findChild<QLabel *>("nameEdit");
        auto surnameEdit = page.findChild<QLabel *>("surnameEdit");
        auto emailEdit = page.findChild<QLabel *>("emailEdit");
        auto phoneEdit = page.findChild<QLineEdit *>("phoneEdit");
        auto placeEdit = page.findChild<QLineEdit *>("placeEdit");
        auto facultyEdit = page.findChild<QLineEdit *>("facultyEdit");
        auto universityEdit = page.findChild<QLineEdit *>("universityEdit");
        auto statusCombo = page.findChild<QComboBox *>("statusCombo");
        auto experinceCombo = page.findChild<QComboBox *>("experinceCombo");

        QVERIFY(nameEdit);
        QVERIFY(surnameEdit);
        QVERIFY(emailEdit);
        QVERIFY(phoneEdit);
        QVERIFY(placeEdit);
        QVERIFY(facultyEdit);
        QVERIFY(universityEdit);
        QVERIFY(statusCombo);
        QVERIFY(experinceCombo);

        QCOMPARE(nameEdit->text(), name);
        QCOMPARE(surnameEdit->text(), surname);
        QCOMPARE(emailEdit->text(), email);
        QCOMPARE(phoneEdit->text(), phone);
        QCOMPARE(placeEdit->text(), place);
        QCOMPARE(facultyEdit->text(), faculty);
        QCOMPARE(universityEdit->text(), university);
        QCOMPARE(statusCombo->currentIndex(), searchStatus - 1);
        QCOMPARE(experinceCombo->currentIndex(), experience - 1);
    }
};

QTEST_MAIN(ProfilePageForCandidateTest)
#include "test_profilepageforcandidate.moc"
