#include <QString>
#include <QtTest>
#include "../../entity.h"
#include "../../entity.cpp"

class packageTest : public QObject
{
    Q_OBJECT

public:
    packageTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void tst_singleSignal();
    void tst_arraySignal();
    void tst_aggrSignal();
};

packageTest::packageTest()
{
}

void packageTest::initTestCase()
{
}

void packageTest::cleanupTestCase()
{
}

void packageTest::tst_singleSignal()
{
    SingleSignalSpec sig1;
    QCOMPARE(sig1.name(), QString());
    sig1.setName("sig1");
    QCOMPARE(sig1.name(), QString("sig1"));
    SingleSignalSpec sig2("sig2");
    QCOMPARE(sig2.name(), QString("sig2"));
    SingleSignalSpec sig3(sig2);
    QCOMPARE(sig3.name(), QString("sig2"));
}

void packageTest::tst_arraySignal()
{
    ArraySignalSpec arr1;
    QVERIFY(arr1.size() == 0);
    arr1.setSize(10);
    QVERIFY(arr1.size() == 10);
    ArraySignalSpec arr2(arr1);
    QVERIFY(arr2.size() == 10);
}


void packageTest::tst_aggrSignal()
{
    AggregateSignalSpec agg1;
    QVERIFY(agg1.size() == 0);
    QVERIFY(agg1.addSignal(new SingleSignalSpec()) == true);
    QVERIFY(agg1.addSignal(new SingleSignalSpec()) == false);


}

QTEST_APPLESS_MAIN(packageTest)

#include "tst_packagetest.moc"
