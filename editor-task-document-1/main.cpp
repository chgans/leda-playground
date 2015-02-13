#include "mainwindow.h"
#include <QApplication>

#include "document.h"
#include "documentgraphicsitem.h"

#include <QJsonDocument>
#include <QDebug>

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
    DocumentGraphicsItemGroup group1;
    group1.items.append(new DocumentGraphicsParameterItem);
    group1.items.append(new DocumentGraphicsRectangleItem);
    QJsonDocument doc1;
    QJsonObject obj1;
    group1.toJson(obj1);
    doc1.setObject(obj1);
    qDebug() << doc1.toJson();

    QJsonDocument doc2 = QJsonDocument::fromJson(doc1.toJson());
    QJsonObject obj2 = doc2.object();
    DocumentItem *item = createGraphicsItem(obj2);

    QJsonDocument doc3;
    QJsonObject obj3;
    item->toJson(obj3);
    doc3.setObject(obj3);
    qDebug() << doc3.toJson();

    qDebug() << (doc3 == doc1);

}
