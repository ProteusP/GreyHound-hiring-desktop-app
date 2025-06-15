#include <QStackedWidget>
#include <QTest>
#include "flowlayout.h"
#include "mainpage.h"

class MainPageTest : public QObject {
    Q_OBJECT

private slots:
    void test_initial_state();
    void test_show_candidate_page();
    void test_show_employer_page();
};

void MainPageTest::test_initial_state() {
    QNetworkAccessManager manager;
    MainPage page(&manager);

    QVERIFY(page.getFlowLayout() == nullptr);  // изначальо null
}

void MainPageTest::test_show_candidate_page() {
    QNetworkAccessManager manager;
    MainPage page(&manager);

    page.setStatusOfCandidate(true);
    page.show(true);  // кандидат

    QVERIFY(
        page.getFlowLayout() == nullptr
    );  // т.к. он создается внутри create*, но не сохраняется
}

void MainPageTest::test_show_employer_page() {
    QNetworkAccessManager manager;
    MainPage page(&manager);

    page.setStatusOfCandidate(false);
    page.show(false);  // работодатель

    QVERIFY(page.getFlowLayout() == nullptr);
}

QTEST_MAIN(MainPageTest)
#include "test_mainpage.moc"
