#include <QPushButton>
#include <QSignalSpy>
#include <QtTest/QtTest>
#include "candidatecard.h"

class CandidateCardTest : public QObject {
    Q_OBJECT

private slots:

    void testCreationAndDisplay() {
        QNetworkAccessManager manager;
        candidateCard card(&manager, "СПбГУ", "Математика", "Без опыта", 123);

        QList<QLabel *> labels = card.findChildren<QLabel *>();
        QVERIFY(labels.size() >= 3);  // place, faculty, experience

        QStringList texts;
        for (QLabel *label : labels) {
            texts << label->text();
        }

        QVERIFY(texts.contains("СПбГУ"));
        QVERIFY(texts.contains("Математика"));
        QVERIFY(texts.contains("Без опыта"));
    }

    void testDetailsButton() {
        QNetworkAccessManager manager;
        candidateCard card(&manager, "ИТМО", "КТ", "1-3 лет", 321);

        QPushButton *button = card.findChild<QPushButton *>();
        QVERIFY(button != nullptr);
        QCOMPARE(button->text(), QString("Подробнее"));

        // Проверка, что при клике открывается окно (мы не проверяем GUI, но
        // слот должен вызваться)
        QSignalSpy spy(button, &QPushButton::clicked);
        QTest::mouseClick(button, Qt::LeftButton);
        QCOMPARE(spy.count(), 1);
    }
};

QTEST_MAIN(CandidateCardTest)
#include "test_candidatecard.moc"
