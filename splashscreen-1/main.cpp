#include "mainwindow.h"
#include <QApplication>
#include "splashscreen.h"
#include <QDesktopWidget>
#include <QBitmap>

SplashScreen *splash;

void showMessage(const QString &message)
{
    splash->showMessage(message);
    qApp->processEvents();
    for (volatile int i=0; i<0xfffffff; i++)
    {}
    for (volatile int i=0; i<0xfffffff; i++)
    {}
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/img/leda-splash-screen-500.png");
    splash = new SplashScreen(pixmap);
    splash->setProductInformation("LibreEDA 2015.04");
    splash->show();

    splash->setProgress(0);
    showMessage("Loading plugin: Leda-Core-1.2.3");
    splash->setProgress(10);
    showMessage("Loading plugin: PCB-Core-1.6.25");
    splash->setProgress(20);
    showMessage("Loading plugin: PCB-Footprint-2.0.3");
    splash->setProgress(30);
    showMessage("Loading plugin: PCB-Lib-0.0.3");
    splash->setProgress(40);
    showMessage("Loading plugin: SCH-Core-0.0.1");
    splash->setProgress(60);
    showMessage("Initialising GUI...");
    MainWindow w1, w2;
    QDesktopWidget *desktop = QApplication::desktop();
    w2.move(desktop->availableGeometry(1).topLeft());
    splash->setProgress(80);
    showMessage("Loading documents...");
    // ...

    // Done!
    w1.show();
    //w2.show();
    splash->finish(&w1);
    return a.exec();
}
