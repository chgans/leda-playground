#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("document-model-4");
    a.setApplicationVersion("0.0");
    a.setOrganizationName("LibreEDA");
    a.setOrganizationDomain("libre-eda.org");
    MainWindow w;
    w.show();

    return a.exec();
}
