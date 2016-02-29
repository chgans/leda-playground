#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "track.h"
#include "node.h"

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

    Node *p1 = new Node();
    p1->setPos(100, 100);
    m_scene->addItem(p1);
    Node *p2 = new Node();
    p2->setPos(200, 100);
    m_scene->addItem(p2);
    Node *p3 = new Node();
    p3->setPos(200, 200);
    m_scene->addItem(p3);
    Node *p4 = new Node();
    p4->setPos(300, 200);
    m_scene->addItem(p4);
    Track *t1 = new Track(p1, p2);
    m_scene->addItem(t1);
    Track *t2 = new Track(p2, p3);
    m_scene->addItem(t2);
    Track *t3 = new Track(p3, p4);
    m_scene->addItem(t3);
}

MainWindow::~MainWindow()
{
    delete ui;
}
