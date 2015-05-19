#include <QString>
#include <QtTest>
#include <QDebug>

#include "document.h"

class XmlTest : public QObject
{
    Q_OBJECT

public:
    XmlTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
};

XmlTest::XmlTest()
{
}

void XmlTest::initTestCase()
{
}

void XmlTest::cleanupTestCase()
{
}

static const QString xml1 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                            "<leda-xdl version=\"1.0\">\n"
                            " <entities count=\"2\">\n"
                            "  <entity name=\"resistor\">\n"
                            "   <generics count=\"2\">\n"
                            "    <generic name=\"value\">\n"
                            "     <unit>ohm</unit>\n"
                            "     <required>1</required>\n"
                            "    </generic>\n"
                            "    <generic name=\"precision\">\n"
                            "     <unit>kohm</unit>\n"
                            "     <required>0</required>\n"
                            "    </generic>\n"
                            "   </generics>\n"
                            "  </entity>\n"
                            "  <entity name=\"capacitor\">\n"
                            "   <generics count=\"0\">\n"
                            "   </generics>\n"
                            "  </entity>\n"
                            " </entities>\n"
                            "</leda-xdl>";
void XmlTest::testCase1()
{
    DocumentReader reader;
    QBuffer device;
    device.setData(xml1.toUtf8());
    device.open(QIODevice::ReadOnly);
    bool res = reader.read(&device);
    qDebug() << res << reader.errorString();
    QObject *doc = reader.document();
    QVERIFY(res == true);
    QVERIFY(doc != 0);

    QObjectList entities, generics;
    QObject *entity, *generic;
    QString str;
    entities = doc->property("entities").value<QObjectList>();
    QVERIFY(entities.count() == 2);

    entity = entities.at(0);
    str = entity->property("name").value<QString>();
    QVERIFY(str == "resistor");
    generics = entity->property("generics").value<QObjectList>();
    QVERIFY(generics.count() == 2);
    generic = generics.at(0);
    QVERIFY(generic->dynamicPropertyNames().contains("name"));
    str = generic->property("name").value<QString>();
    QVERIFY(str == "value");
    QVERIFY(generic->dynamicPropertyNames().contains("unit"));
    str = generic->property("unit").value<QString>();
    QCOMPARE(str, QString("ohm"));
    QVERIFY(generic->dynamicPropertyNames().contains("required"));
    str = generic->property("required").value<QString>();
    QVERIFY(str == "1");
    generic = generics.at(1);
    str = generic->property("name").value<QString>();
    QVERIFY(str == "precision");
    str = generic->property("unit").value<QString>();
    QVERIFY(str == "kohm");
    str = generic->property("required").value<QString>();
    QVERIFY(str == "0");

    entity = entities.at(1);
    str = entity->property("name").value<QString>();
    QVERIFY(str == "capacitor");
    generics = entity->property("generics").value<QObjectList>();
    QVERIFY(generics.count() == 0);


}

QTEST_APPLESS_MAIN(XmlTest)

#include "tst_xmltest.moc"
