#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainview.h"
#include "objectpreview.h"
#include "detailview.h"
#include "overview.h"

#include "pcbeditorwidget.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDockWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QKeyEvent>
#include <QTabWidget>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    //menuBar()->addMenu("test");

    QTabWidget *tw = new QTabWidget(this);
    setCentralWidget(tw);

    PcbEditorWidget *w;
    w = new PcbEditorWidget;
    tw->addTab(w, "Editor 1");
    w = new PcbEditorWidget;
    tw->addTab(w, "Editor 2");
    mv = w->graphicsView();

    createActions();
    createMenus();

#if 1
    QSplitter *s = new QSplitter(Qt::Vertical);
    QVBoxLayout *l;
    QWidget *lw;

    opv = new ObjectPreview;
    l = new QVBoxLayout;
    l->addWidget(opv);
    lw = new QWidget();
    lw->setLayout(l);
    s->addWidget(lw);

    dv = new DetailView;
    dv->setObservedView(mv);
    l = new QVBoxLayout;
    l->addWidget(dv);
    lw = new QWidget();
    lw->setLayout(l);
    s->addWidget(lw);

    ov = new OverView;
    ov->setObservedView(mv);
    l = new QVBoxLayout;
    l->addWidget(ov);
    lw = new QWidget();
    lw->setLayout(l);
    s->addWidget(lw);


    dw = new QDockWidget("Overview");
    dw->setWidget(s);
    addDockWidget(Qt::LeftDockWidgetArea, dw);
#endif
}

MainWindow::~MainWindow()
{
}


void MainWindow::setGraphicsScene(QGraphicsScene *scene)
{
    mv->setScene(scene);
    mv->scale(0.75, 0.75);
#if 1
    opv->setScene(scene);
    dv->setScene(scene);
    ov->setScene(scene);
#endif
}

void MainWindow::createActions()
{
}

void MainWindow::createMenus()
{
}

