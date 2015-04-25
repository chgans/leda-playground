#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>

class IEditorPlugin;

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:
    void aboutToLoadPlugins(int count);
    void pluginLoaded(IEditorPlugin *plugin);

public slots:
    void loadPlugins();

protected:
    void addPlugin(IEditorPlugin *plugin);
};

#endif // MAINWINDOW_H
