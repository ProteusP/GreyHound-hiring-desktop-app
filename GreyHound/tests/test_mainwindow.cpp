#include <QTest>
#include "mainwindow.h"

class MainWindowTest : public QObject {
    Q_OBJECT

private slots:

    void test_switchToRegisterStatusPage() {
        MainWindow w;
        w.show();
        QVERIFY(QTest::qWaitForWindowExposed(&w));

        w.onRegisterStatusPage();

        QVERIFY(
            w.centralWidget()->findChild<QStackedWidget *>()->currentWidget() !=
            nullptr
        );
    }

    void test_profileCandidateLoaded() {
        MainWindow w;
        w.show();

        w.onMainPage(true);

        auto current =
            w.centralWidget()->findChild<QStackedWidget *>()->currentWidget();
        QVERIFY(current != nullptr);
        QVERIFY(current->objectName().contains("MainPage"));
    }

    void test_profileEmployerLoaded() {
        MainWindow w;
        w.show();

        w.onMainPage(false);

        auto current =
            w.centralWidget()->findChild<QStackedWidget *>()->currentWidget();
        QVERIFY(current != nullptr);
        QVERIFY(current->objectName().contains("MainPage"));
    }
};

QTEST_MAIN(MainWindowTest)
#include "test_mainwindow.moc"
