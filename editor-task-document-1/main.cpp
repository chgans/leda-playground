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
    DocumentGraphicsItemGroup group;
    for (int i=0; i<10; i++)
        group.items.append(new DocumentGraphicsParameterItem);
    QJsonDocument doc;
    doc.setObject(group.toJson());
    qDebug() << doc.toJson();
}
