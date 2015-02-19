#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGraphicsScene;
class MainView;
class ObjectPreview;
class DetailView;
class OverView;
class QDockWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setGraphicsScene(QGraphicsScene *scene);

    MainView *mv;

private slots:

protected:
    void createActions();
    void createMenus();

private:
    QDockWidget *dw;
    ObjectPreview *opv;
    DetailView *dv;
    OverView *ov;
};

#endif // MAINWINDOW_H
