#include <QLabel>
#include <QPushButton>
#include <QSignalSpy>
#include <QtTest/QtTest>
#include "vacancycard.h"

class VacancyCardTest : public QObject {
    Q_OBJECT

private slots:

    void test_initial_state() {
        QNetworkAccessManager mgr;

        VacancyCard card(&mgr, "Title", "Company", "Salary", "Place", 1, 0, 42);

        auto titleLabel = card.findChild<QLabel *>();
        QVERIFY(titleLabel);

        auto button = card.findChild<QPushButton *>();
        QVERIFY(button);
        QCOMPARE(button->text(), "Подробнее");
    }

    void test_widget_properties() {
        QNetworkAccessManager mgr;

        VacancyCard card(
            &mgr, "Developer", "OpenAI", "от 100000₽", "Москва", 0, 1, 999
        );

        auto labels = card.findChildren<QLabel *>();
        QVERIFY(labels.size() >= 3);

        QCOMPARE(labels[0]->text(), "Developer");
        QCOMPARE(labels[1]->text(), "OpenAI");
        QCOMPARE(labels[2]->text(), "от 100000₽");
    }
};

QTEST_MAIN(VacancyCardTest)
#include "test_vacancycard.moc"
