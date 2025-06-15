#include <QLabel>
#include <QPushButton>
#include <QTest>
#include "candidatedetailswindow.h"

class CandidateDetailsWindowTest : public QObject {
    Q_OBJECT

private slots:
    void test_initialization();
    void test_exit_button();
};

void CandidateDetailsWindowTest::test_initialization() {
    QNetworkAccessManager manager;
    candidateDetailsWindow window(&manager, 1);

    QLabel *nameLabel = window.findChild<QLabel *>("name");
    QLabel *surnameLabel = window.findChild<QLabel *>("surname");
    QLabel *emailLabel = window.findChild<QLabel *>("email");

    QVERIFY(nameLabel != nullptr);
    QVERIFY(surnameLabel != nullptr);
    QVERIFY(emailLabel != nullptr);
}

void CandidateDetailsWindowTest::test_exit_button() {
    QNetworkAccessManager manager;
    candidateDetailsWindow window(&manager, 1);

    QPushButton *exitBtn = window.findChild<QPushButton *>("exitButton");
    QVERIFY(exitBtn != nullptr);

    window.show();
    QVERIFY(QTest::qWaitForWindowExposed(&window));

    QTest::mouseClick(exitBtn, Qt::LeftButton);
    QVERIFY(window.isHidden());  // окно должно закрыться
}

QTEST_MAIN(CandidateDetailsWindowTest)
#include "test_candidatedetailswindow.moc"
