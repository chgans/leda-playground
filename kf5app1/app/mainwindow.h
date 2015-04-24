#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>

class Plugin;

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:
    void pluginLoaded(Plugin *plugin);

public slots:
    void loadPlugins();

protected:
    void addPlugin(Plugin *plugin);
};

#endif // MAINWINDOW_H
