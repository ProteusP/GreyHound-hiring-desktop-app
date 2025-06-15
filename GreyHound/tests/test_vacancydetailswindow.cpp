#include <QLabel>
#include <QNetworkAccessManager>
#include <QtTest/QtTest>
#include "vacancydetailswindow.h"

class VacancyDetailsWindowTest : public QObject {
    Q_OBJECT
private slots:

    void test_initialization() {
        QNetworkAccessManager mgr;
        vacancyDetailsWindow w(
            &mgr, "Разработчик", "Компания", "150 000₽", "Москва",
            1,  // Полный день
            1,  // Удалёнка
            42
        );

        QLabel *title = w.findChild<QLabel *>("title");
        QVERIFY(title);
        QCOMPARE(title->text(), "Разработчик");

        QLabel *schedule = w.findChild<QLabel *>("workSchedule");
        QVERIFY(schedule);
        QCOMPARE(schedule->text(), "Полный рабочий день");

        QLabel *remote = w.findChild<QLabel *>("remoteness");
        QVERIFY(remote);
        QCOMPARE(remote->text(), "Да");
    }
};

QTEST_MAIN(VacancyDetailsWindowTest)
#include "test_vacancydetailswindow.moc"
