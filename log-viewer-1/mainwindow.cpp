#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "messagelogviewer.h"

#include <QDockWidget>
#include <QVBoxLayout>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDockWidget *dock = new QDockWidget();
    dock->setAllowedAreas(Qt::BottomDockWidgetArea);
    dock->setWidget(new MessageLogViewer);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    setCentralWidget(new QWidget());
}

MainWindow::~MainWindow()
{
    delete ui;
}
