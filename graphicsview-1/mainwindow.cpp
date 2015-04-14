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
    m_editor->activateEditor(this);
}

void MainWindow::createActions()
{
}

void MainWindow::createMenus()
{
}

void MainWindow::createDockWidgets()
{
}

