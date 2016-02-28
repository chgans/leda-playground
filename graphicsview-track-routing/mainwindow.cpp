#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_scene(new QGraphicsScene(this)),
    m_view(new GraphicsView)
{
    ui->setupUi(this);
    m_scene->setSceneRect(0, 0, 297, 210);
    m_view->setScene(m_scene);
    setCentralWidget(m_view);
    statusBar()->showMessage("SHIFT+SPACEBAR to cycle through routing strategies, "
                             "SPACEBAR to switch b/w routing direction");
}

MainWindow::~MainWindow()
{
    delete ui;
}
