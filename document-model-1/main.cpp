#include "mainwindow.h"
#include "documentmodel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    DocumentModel model;
    w.setModel(&model);
    w.show();
    return a.exec();
}
