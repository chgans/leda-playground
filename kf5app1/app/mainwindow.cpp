#include <functional>

#include <QDebug>
#include <QFileInfo>

#include <KPluginInfo>
#include <KPluginTrader>
#include <KXMLGUIFactory>
#include <KTextEdit>

#include "mainwindow.h"
#include "core/plugin.h"

MainWindow::MainWindow(QWidget *parent) :
    KXmlGuiWindow(parent)
{
    setCaption("Text Editor");

    setupGUI(KXmlGuiWindow::Default, ":kxmlgui/appui.rc");
}

void MainWindow::loadPlugins()
{
    qDebug() << "Loading plugins" << QCoreApplication::libraryPaths() << "...";

    KPluginInfo::List offers = KPluginTrader::self()->query("."); // TODO use "libre-eda" sub-dir

    emit aboutToLoadPlugins(offers.count());
    foreach (const KPluginInfo &info, offers) {
        QString baseName = QFileInfo(info.libraryPath()).fileName();
        KPluginLoader loader(info.libraryPath());
        if (!info.isValid()) {
            continue;
        }
        KPluginFactory *factory = loader.factory();
        if (!factory) {
            qWarning() << loader.errorString();
            continue;
        }
        qDebug() << loader.instance()->metaObject()->className();

        IEditorPlugin *plugin = factory->create<IEditorPlugin>(this);
        if (plugin) {
            qDebug() << "Loading plugin" << info.name();
            qDebug() << info.properties();
            qDebug() << loader.metaData().value("MetaData").toObject().value("KPlugin").toObject();
            addPlugin(plugin);
            emit pluginLoaded(plugin);
        } else {
            qDebug() << baseName << "is not a plugin";
        }
    }
}

void MainWindow::addPlugin(IEditorPlugin *plugin)
{
    KTextEdit *editor = plugin->createEditor();
    qDebug() << editor;
    setCentralWidget(editor);
}
