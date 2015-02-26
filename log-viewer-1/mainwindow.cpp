#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logmessage.h"
#include "loggingmodel.h"
#include "messagelogviewer.h"

#include <QDockWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_logModel(new LoggingModel(this)),
    m_logViewer(new MessageLogViewer)
{
    ui->setupUi(this);

    m_logViewer->setModel(m_logModel);

    QDockWidget *dock = new QDockWidget();
    dock->setAllowedAreas(Qt::BottomDockWidgetArea);
    dock->setWidget(m_logViewer);
    addDockWidget(Qt::BottomDockWidgetArea, dock);

    setCentralWidget(new QWidget());

}

MainWindow::~MainWindow()
{
    delete ui;
}

LoggingModel *MainWindow::loggingModel()
{
    return m_logModel;
}
