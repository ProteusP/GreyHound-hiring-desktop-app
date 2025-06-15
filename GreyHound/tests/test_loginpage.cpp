#include <QLineEdit>
#include <QtTest/QtTest>
#include "hashing.h"
#include "loginpage.h"

class LoginPageSmokeTest : public QObject {
    Q_OBJECT
private slots:

    void test_register_signal() {
        LoginWidget w(nullptr);
        QSignalSpy spy(&w, &LoginWidget::registerPressed);
        QMetaObject::invokeMethod(
            &w, "on_registerPB_clicked", Qt::DirectConnection
        );
        QCOMPARE(spy.count(), 1);
    }
};

QTEST_MAIN(LoginPageSmokeTest)
#include "test_loginpage.moc"
