#include "mainwindow.h"
#include "gridmanagerdialog.h"
#include "view.h"

#include <QToolBar>
#include <QAction>
#include <QGraphicsRectItem>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    mScene = new QGraphicsScene(0, 0, 1000, 1000, this);
    mView = new View(this);
    mView->setScene(mScene);
    mView->setSceneRect(mScene->sceneRect().adjusted(-50, -50, 50, 50));
    mView->ensureVisible(mScene->sceneRect());
    loadFeatures();
    mView->initialise();

    mManager = GridManager::instance();
    connect(mManager, SIGNAL(gridsChanged()),
            this, SLOT(onGridsChanged()));

    QToolBar *toolBar = addToolBar("tools");
    QAction *action = toolBar->addAction("Grids");
    connect(action, SIGNAL(triggered()),
            this, SLOT(execGridManagerDialog()));

    setCentralWidget(mView);
    showMaximized();
}

void MainWindow::execGridManagerDialog()
{
    GridManagerDialog dlg;
    dlg.show();
    dlg.exec();
}

void MainWindow::onGridsChanged()
{
    qDebug() << __FUNCTION__;
    mView->invalidateScene(mView->sceneRect(), QGraphicsScene::BackgroundLayer);
    mView->update();
}

#include "gridmanager.h"
#include "gridmanagerdialog.h"
#include "cartesiangridfactory.h"
#include "cartesiangrid.h"
#include "polargridfactory.h"
#include "polargrid.h"
#include "igrid.h"

void MainWindow::loadFeatures()
{
    GridManager *mng = GridManager::instance();

    CartesianGridFactory *fc = new CartesianGridFactory;
    mng->registerFactory(fc->id(), fc);
    PolarGridFactory *fp = new PolarGridFactory;
    mng->registerFactory(fp->id(), fp);

    QList<const IGrid *> grids;

    CartesianGrid *cgrid = new CartesianGrid;
    cgrid->setPriority(1);
    cgrid->setOrigin(QPointF(0, 0));
    cgrid->setSize(QSize(1000, 1000));
    grids.append(cgrid);

    PolarGrid *pgrid = new PolarGrid;
    pgrid->setPriority(2);
    pgrid->setOrigin(QPointF(200, 400));
    //pgrid->setSize(QSize(100, 100));
    pgrid->setAngularRange(3*90, 4*90);
    pgrid->setAngularStep(90/4.0);
    pgrid->setRadialRange(0, 100);
    pgrid->setRadialStep(10);
    pgrid->setCoarseLineColor(Qt::yellow);
    pgrid->setFineLineColor(Qt::darkYellow);
    grids.append(pgrid);

    pgrid = new PolarGrid;
    pgrid->setPriority(3);
    pgrid->setOrigin(QPointF(200, 600));
    //pgrid->setSize(QSize(100, 100));
    pgrid->setAngularRange(0*90, 1*90);
    pgrid->setAngularStep(90/4.0);
    pgrid->setRadialRange(25, 100);
    pgrid->setRadialStep(10);
    pgrid->setCoarseLineColor(Qt::green);
    pgrid->setFineLineColor(Qt::darkGreen);
    grids.append(pgrid);

    mng->setGrids(grids);
}
