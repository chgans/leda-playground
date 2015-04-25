// Including Qt Classes
#include <QApplication>
#include <QTimer>
#include <QDir>

// Including KF5 Classes
#include <KAboutData>
#include <KGuiItem>
#include <KLocalizedString>
#include <KMessageBox>

// Including local classes
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    // Configure the data related to our application
    KAboutData about("app", i18n("helloworld"), "0.1",
                     i18n("A KDE version of Hello, world!"),
                     KAboutLicense::LGPL,
                     i18n("Copyright (C) 2014 Arpan P"));

    // Add application author with role and email address.
    about.addAuthor(i18n("Arpan P"), i18n("developer"), i18n("arpan@example.com"), 0);

    // Set application data
    KAboutData::setApplicationData(about);

    // We use QApplication instead of KApplication
    QApplication app(argc, argv);

    // Set up display name for our application - This will be displayed in title bar.
    QApplication::setApplicationDisplayName(i18n("Hello World"));

    MainWindow *window = new MainWindow();
    window->show();
    QTimer::singleShot(0, window, SLOT(loadPlugins()));
    return app.exec();
}
