#include <functional>

#include <QDebug>
#include <QFileInfo>

#include <KPluginInfo>
#include <KPluginTrader>
#include <KService>
#include <KTextEditor/Editor>
#include <KParts/ReadWritePart>
#include "mainwindow.h"
#include "core/plugin.h"

MainWindow::MainWindow(QWidget *parent) :
    KParts::MainWindow(parent)
{
    setCaption("Text Editor");
    setXMLFile(":kxmlgui/appui.rc");
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

        IEditorPlugin *plugin = factory->create<IEditorPlugin>(this);
        if (plugin) {
            qDebug() << "Loading plugin" << info.name() << info.pluginName();
            qDebug() << loader.metaData().value("MetaData").toObject().value("KPlugin").toObject();
            addPlugin(plugin);
            emit pluginLoaded(plugin);
        } else {
            qDebug() << baseName << "is not a plugin";
        }
    }

//    KService::Ptr service = KService::serviceByDesktopPath("katepart");
//    qDebug() << service;
//    if (service) {
//        KParts::ReadWritePart *part = service->createInstance<KParts::ReadWritePart>(nullptr);
//        if (part != nullptr)
//            createGUI(part);
//        else
//            qWarning() << "Unable to create katepart instance";
//    }
//    else
//        qWarning() << "Unable to find katepart service";

}

void MainWindow::addPlugin(IEditorPlugin *plugin)
{
    KTextEditor::Editor *editor = plugin->createEditor();
    //guiFactory()->addClient();
    //setCentralWidget(editor);
}
