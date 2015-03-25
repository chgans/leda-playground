#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "scene.h"
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
    m_editor = new PcbEditorWidget();
    setCentralWidget(m_editor);

    createActions();
    createMenus();
    createDockWidgets();

}

MainWindow::~MainWindow()
{
}


void MainWindow::setGraphicsScene(Scene *scene)
{
    m_editor->setScene(scene);
    dv->setObservedView(m_editor->graphicsView());
    ov->setObservedView(m_editor->graphicsView());
}

void MainWindow::createActions()
{
}

void MainWindow::createMenus()
{
}

void MainWindow::createDockWidgets()
{
    QSplitter *s = new QSplitter(Qt::Vertical);
    QVBoxLayout *l;
    QWidget *lw;

#if 1
    opv = new ObjectPreview;
    l = new QVBoxLayout;
    l->addWidget(opv);
    lw = new QWidget();
    lw->setLayout(l);
    s->addWidget(lw);
#endif
    dv = new DetailView;
    l = new QVBoxLayout;
    l->addWidget(dv);
    lw = new QWidget();
    lw->setLayout(l);
    s->addWidget(lw);

    ov = new OverView;
    l = new QVBoxLayout;
    l->addWidget(ov);
    lw = new QWidget();
    lw->setLayout(l);
    s->addWidget(lw);

    dw = new QDockWidget("Overview");
    dw->setWidget(s);
    addDockWidget(Qt::LeftDockWidgetArea, dw);
}

