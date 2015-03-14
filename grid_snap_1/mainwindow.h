#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "view.h"
#include "gridmanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:

protected slots:
    void execGridManagerDialog();
    void onGridsChanged();

private:
    void loadFeatures();

    QGraphicsScene *mScene;
    View *mView;
    GridManager *mManager;
};

#endif // MAINWINDOW_H
