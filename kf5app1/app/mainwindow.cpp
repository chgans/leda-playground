#include <functional>

#include <QDebug>

#include <KPluginInfo>
#include <KPluginTrader>
#include <KXMLGUIFactory>
#include "mainwindow.h"
#include "plugin.h"

MainWindow::MainWindow(QWidget *parent) :
    KXmlGuiWindow(parent)
{
    setCaption("Text Editor");

    setupGUI(KXmlGuiWindow::Default, ":kxmlgui/appui.rc");
}

void MainWindow::loadPlugins()
{
    qDebug() << "Loading plugins" << QCoreApplication::libraryPaths() << "...";

    KPluginInfo::List offers = KPluginTrader::self()->query("leda");

    foreach (const KPluginInfo &info, offers) {
        KPluginLoader loader(info.libraryPath());
        KPluginFactory *factory = loader.factory();
        if (!factory) {
            qWarning() << "KPluginFactory could not load the plugin:" << info.libraryPath();
            continue;
        }

        Plugin *plugin = factory->create<Plugin>(this);
        if (plugin) {
            qDebug() << "Loading plugin" << info.pluginName();
            addPlugin(plugin);
            emit pluginLoaded(plugin);
        } else {
            qDebug() << info.name() << "is not a plugin";
        }
    }
}

void MainWindow::addPlugin(Plugin *plugin)
{
    KTextEdit *editor = plugin->createEditor();
    qDebug() << editor;
    guiFactory()->addClient(plugin);
}
