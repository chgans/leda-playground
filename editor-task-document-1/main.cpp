#include "mainwindow.h"
#include <QApplication>

#include "document.h"
#include "documentitems.h"

#include <QJsonDocument>
#include <QDebug>

void test1()
{
    QJsonDocument doc1;
    QJsonValue val1;

    DocumentGraphicsItemGroup group1;
    group1.items.append(new DocumentGraphicsParameterItem);
    group1.items.append(new DocumentGraphicsRectangleItem);
    group1.toJsonValue(val1);
    doc1.setObject(val1.toObject());
    qDebug() << doc1.toJson();

    QJsonDocument doc2 = QJsonDocument::fromJson(doc1.toJson());
    QJsonObject obj2 = doc2.object();
    DocumentItem *item = createGraphicsItem(obj2);

    QJsonDocument doc3;
    QJsonValue val3;
    item->toJsonValue(val3);
    doc3.setObject(val3.toObject());
    qDebug() << doc3.toJson();

    qDebug() << (doc3 == doc1);
}

void test2()
{
    QJsonDocument doc1;
    QJsonValue val1;

    DocumentParameterItemList list1;
    list1.append(new DocumentParameterItem);
    list1.append(new DocumentParameterItem);
    list1.toJsonValue(val1);
    doc1.setArray(val1.toArray());
    qDebug() << doc1.toJson();

    QJsonDocument doc2 = QJsonDocument::fromJson(doc1.toJson());
    QJsonValue val2 = doc2.array();
    DocumentParameterItemList list;
    list.fromJsonValue(val2);

    QJsonDocument doc3;
    QJsonValue val3;
    list.toJsonValue(val3);
    doc3.setArray(val3.toArray());
    qDebug() << doc3.toJson();

    qDebug() << (doc3 == doc1);
}


void test3()
{
    QJsonDocument doc1;
    QJsonValue val1;

    DocumentSymbolItem sym1;
    sym1.properties.append(new DocumentParameterItem);
    sym1.properties.append(new DocumentParameterItem);
    sym1.toJsonValue(val1);
    doc1.setObject(val1.toObject());
    qDebug() << doc1.toJson();

    QJsonDocument doc2 = QJsonDocument::fromJson(doc1.toJson());
    QJsonValue val2 = doc2.object();
    DocumentSymbolItem sym2;
    sym2.fromJsonValue(val2);

    QJsonDocument doc3;
    QJsonValue val3;
    sym2.toJsonValue(val3);
    doc3.setObject(val3.toObject());
    qDebug() << doc3.toJson();

    qDebug() << (doc3 == doc1);
}

void test4()
{
    DocumentSymbolItem sym1;

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //Document doc;
    //doc.setParent(&a);
    //doc.load("../editor-task-document-1/doc1.xml");
    //w.show();
    //return a.exec();
    //return 0;
    test3();
}
