#include <QString>
#include <QtTest>
#include <QDebug>

#include "document.h"
#include "designunit.h"

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
                            "   <description>desc</description>\n"
                            "   <generics count=\"2\">\n"
                            "    <generic name=\"value\">\n"
                            "     <unit>ohm</unit>\n"
                            "     <required>true</required>\n"
                            "    </generic>\n"
                            "    <generic name=\"precision\">\n"
                            "     <unit>kohm</unit>\n"
                            "     <required>false</required>\n"
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
    DesignUnit *doc = reader.document();
    QVERIFY(res == true);
    QVERIFY(doc != 0);

    EntityList entities;
    Entity *entity;
    GenericList generics;
    Generic *generic;

    entities = doc->entities;
    QVERIFY(entities.count() == 2);

    entity = entities.at(0);
    QVERIFY(entity->name == "resistor");
    QVERIFY(entity->description == "desc");
    generics = entity->generics;
    QVERIFY(generics.count() == 2);
    generic = generics.at(0);
    QVERIFY(generic->name == "value");
    QCOMPARE(generic->unit, QString("ohm"));
    QVERIFY(generic->required == true);
    generic = generics.at(1);
    QVERIFY(generic->name == "precision");
    QVERIFY(generic->unit == "kohm");
    QVERIFY(generic->required == false);

    entity = entities.at(1);
    QVERIFY(entity->name == "capacitor");
    generics = entity->generics;
    QVERIFY(generics.count() == 0);
}

QTEST_APPLESS_MAIN(XmlTest)

#include "tst_xmltest.moc"
