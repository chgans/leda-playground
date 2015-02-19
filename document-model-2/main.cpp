#include "mainwindow.h"
#include <QApplication>
#include <documentmodel.h>
#include <documentobject.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    DocumentModel *model;
    DocumentObject *doc;

    doc = new DocumentObject();
    doc->setTypeName("leda.document.symbol");
    doc->setUid(123456);

    int i, j;
    for (i=0; i<5; i++) {
        DocumentObject *item = new DocumentObject(doc);
        doc->appendChild(item);
        for (j=0; j<3; j++) {
            DocumentObject *leaf = new DocumentObject(item);
            item->appendChild(leaf);
        }
    }
    model = new DocumentModel(doc, &w);
    w.setModel(model);
    w.show();
    return a.exec();
}
