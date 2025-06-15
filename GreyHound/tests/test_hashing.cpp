#include <QtTest>
#include "hashing.h"

class HashingTest : public QObject {
    Q_OBJECT
private slots:

    void knownValues() {
        QCOMPARE(
            hashPassword("abc"),
            QStringLiteral("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb41"
                           "0ff61f20015ad")
        );
    }

    void emptyString() {
        QCOMPARE(
            hashPassword(""), QStringLiteral("e3b0c44298fc1c149afbf4c8996fb9242"
                                             "7ae41e4649b934ca495991b7852b855")
        );
    }
};

QTEST_APPLESS_MAIN(HashingTest)
#include "test_hashing.moc"
