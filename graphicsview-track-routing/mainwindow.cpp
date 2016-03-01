#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "linetrackelement.h"
#include "arctrackelement.h"
#include "tracknode.h"

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

    TrackNode *p1 = new TrackNode();
    p1->setPos(100, 100);
    m_scene->addItem(p1);
    TrackNode *p2 = new TrackNode();
    p2->setPos(200, 100);
    m_scene->addItem(p2);
    TrackNode *p3 = new TrackNode();
    p3->setPos(200, 200);
    m_scene->addItem(p3);
    TrackNode *p4 = new TrackNode();
    p4->setPos(300, 200);
    m_scene->addItem(p4);
    TrackNode *p5 = new TrackNode();
    p5->setPos(400, 100);
    m_scene->addItem(p5);
    LineTrackElement *t1 = new LineTrackElement(p1, p2);
    m_scene->addItem(t1);
    LineTrackElement *t2 = new LineTrackElement(p2, p3);
    m_scene->addItem(t2);
    LineTrackElement *t3 = new LineTrackElement(p3, p4);
    m_scene->addItem(t3);
    ArcTrackElement *a1 = new ArcTrackElement(p4, p5);
    a1->setPos(300, 100);
    m_scene->addItem(a1);
}

MainWindow::~MainWindow()
{
    delete ui;
}
