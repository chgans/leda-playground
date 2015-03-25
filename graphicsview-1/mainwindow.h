#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGraphicsScene;
class QDockWidget;

class MainView;
class ObjectPreview;
class DetailView;
class OverView;
class PcbEditorWidget;
class Scene;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setGraphicsScene(Scene *scene);

private slots:

protected:
    void createActions();
    void createMenus();
    void createDockWidgets();

private:
    PcbEditorWidget *m_editor;
    QDockWidget *dw;
    ObjectPreview *opv;
    DetailView *dv;
    OverView *ov;
};

#endif // MAINWINDOW_H
