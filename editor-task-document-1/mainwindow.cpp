#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsScene>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_scene(new QGraphicsScene)
{
    m_ui->setupUi(this);
    m_scene->setSceneRect(0, 0, 297, 210);
    m_ui->view->setScene(m_scene);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}
