#include "mainwindow.h"
#include <QApplication>
#include "splashscreen.h"
#include <QDesktopWidget>

SplashScreen *splash;

void showMessage(const QString &message)
{
    splash->showMessage(message);
    qApp->processEvents();
    for (volatile int i=0; i<0xfffffff; i++)
    {}
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/img/splash3.jpg");
    splash = new SplashScreen(pixmap);
    splash->setProductInformation("LibreEDA 2014.07");
    splash->show();

    QStringList names;
    names << "Core 1.0" << "Schematics 1.0" << "PCB Layout 1.0";
    foreach (QString name, names) {
        showMessage(QString("Loading modules: %1...").arg(name));
    }
    showMessage("Initialising GUI...");
    MainWindow w1, w2;
    QDesktopWidget *desktop = QApplication::desktop();
    w2.move(desktop->availableGeometry(1).topLeft());
    showMessage("Loading documents...");
    // ...

    // Done!
    w1.show();
    w2.show();
    splash->finish(&w1);
    return a.exec();
}
