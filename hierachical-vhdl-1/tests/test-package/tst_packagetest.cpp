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
    void tst_portSpec();
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
    QVERIFY(sig1.type() == BaseSignalSpec::Single);
    QCOMPARE(sig1.name(), QString());
    sig1.setName("sig1");
    QCOMPARE(sig1.name(), QString("sig1"));
    SingleSignalSpec sig2("sig2");
    QCOMPARE(sig2.name(), QString("sig2"));
    SingleSignalSpec sig3(sig2);
    QCOMPARE(sig3.name(), QString("sig2"));
    QVERIFY(sig2 == sig3);
    QVERIFY(sig1 != sig2);
}

void packageTest::tst_arraySignal()
{
    ArraySignalSpec arr1("arr1");
    QVERIFY(arr1.type() == BaseSignalSpec::Array);
    QVERIFY(arr1.size() == 0);
    arr1.setSize(10);
    QVERIFY(arr1.size() == 10);
    ArraySignalSpec arr2(arr1);
    QVERIFY(arr2.size() == 10);
    QVERIFY(arr1 == arr2);
    ArraySignalSpec arr3("arr1", 5);
    QVERIFY(arr1 != arr3);
    BaseSignalSpec *b1 = &arr1;
    BaseSignalSpec *b2 = &arr2;
    BaseSignalSpec *b3 = &arr3;
    QVERIFY(*b1 == *b2);
    QVERIFY(*b1 != *b3);
}


void packageTest::tst_aggrSignal()
{
    AggregateSignalSpec agg1;
    QVERIFY(agg1.type() == BaseSignalSpec::Aggregate);
    QVERIFY(agg1.size() == 0);
    QVERIFY(agg1.addSignal(new SingleSignalSpec()) == true);
    QVERIFY(agg1.addSignal(new SingleSignalSpec()) == false);
    QVERIFY(agg1.size() == 1);
    SingleSignalSpec *sig1 = new SingleSignalSpec("sig1");
    ArraySignalSpec *sig2 = new ArraySignalSpec("sig2", 10);
    QVERIFY(agg1.addSignal(sig1) == true);
    QVERIFY(agg1.addSignal(sig2) == true);
    QVERIFY(agg1.size() == 3);
    QVERIFY(agg1.signal("sig1") == sig1);
    QVERIFY(agg1.signal("sig2") == sig2);
    QVERIFY(agg1.signal(1) == sig1);
    QVERIFY(agg1.signal(2) == sig2);

    AggregateSignalSpec agg2(agg1);
    QVERIFY(agg1 == agg2);
    agg2.addSignal(new SingleSignalSpec("sig3"));
    QVERIFY(agg1 != agg2);

}

// TBD: port name vs signal name
void packageTest::tst_portSpec()
{
    PortSpec port1("1", InputPort, new SingleSignalSpec("1"));
    PortSpec port2("data", InputPort, new ArraySignalSpec("data", 32));
    PortSpec port3(port1);
    QVERIFY(port1.name() == "1");
    QVERIFY(port1.portType() == InputPort);
    QVERIFY(port1.signalSpec()->name() == "1");
    QVERIFY(port1 == port3);
    QVERIFY(port1.signalSpec() != port3.signalSpec());
    QVERIFY(port1 != port2);
    BaseSignalSpec *sig1 = new AggregateSignalSpec("ctrl");
    port3.setName("3");
    port3.setPortType(InputOutputPort);
    port3.setSignalSpec(sig1);
    QVERIFY(port3.name() == "3");
    QVERIFY(port3.portType() == InputOutputPort);
    QVERIFY(port3.signalSpec() == sig1);
}

QTEST_APPLESS_MAIN(packageTest)

#include "tst_packagetest.moc"
