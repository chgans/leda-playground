#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KParts/MainWindow>

class IEditorPlugin;

// TODO:
// States:
class MainWindow : public KParts::MainWindow
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
