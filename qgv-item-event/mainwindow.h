#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

namespace Ui {
class MainWindow;
}

class GraphicsScene;
class GraphicsTool;
class QActionGroup;
class QToolBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void onViewContextMenuRequested(QPoint pos);
    void onViewMouseDoubleClicked(QPoint pos);

protected:
    void addTool(GraphicsTool *tool);

private:
    Ui::MainWindow *ui;
    GraphicsScene *m_scene;
    QActionGroup *m_interactiveToolActionGroup;
    QList<GraphicsTool *> m_interactiveTools;
    QToolBar *m_interactiveToolsToolBar;
};

#endif // MAINWINDOW_H
