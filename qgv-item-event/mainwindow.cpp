#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

#include <QGraphicsDropShadowEffect>
#include <QGraphicsColorizeEffect>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_scene = new GraphicsScene(this);
    m_scene->setSceneRect(0, 0, 297, 210);
    ui->graphicsView->setScene(m_scene);
    m_item = m_scene->addRect(-25, -25, 50, 50);
    m_item->setPos(103, 52);
    m_item->setBrush(Qt::darkGreen);
    m_item->setPen(QPen(Qt::darkRed));
    m_item->setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable);
    m_item = m_scene->addRect(-25, -25, 50, 50);
    m_item->setPos(169, 78);
    m_item->setBrush(Qt::darkRed);
    m_item->setPen(QPen(Qt::darkGreen));
    m_item->setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable);

    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->start(0);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(init()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    /*
    item = new RectItem(0, 0, 100, 100);
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item->setFlag(QGraphicsItem::ItemIsMovable, true);
    item->setPen(QPen(QColor(0x80, 0x00, 0x00)));
    item->setBrush(QBrush(QColor(0xff, 0xff, 0xb0)));
    scene->addItem(item);

    item = new RectItem(0, 0, 100, 100);
    item->moveBy(50, 50);
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item->setFlag(QGraphicsItem::ItemIsMovable, true);
    item->setPen(QPen(QColor(0x80, 0x00, 0x00)));
    item->setBrush(QBrush(QColor(0xff, 0xff, 0xb0)));
    scene->addItem(item);


    QLinearGradient gradient(0, 0, 0, ui->graphicsView->height());
    gradient.setColorAt(0, QColor(0x33, 0x33, 0x66));
    gradient.setColorAt(1, Qt::white);
    ui->graphicsView->setBackgroundBrush(QBrush(gradient));

    NoteItem *nitem = new NoteItem("Hello world!\nHello world!\n");
    nitem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    nitem->setFlag(QGraphicsItem::ItemIsMovable, true);
    nitem->setTextInteractionFlags(Qt::TextEditorInteraction);
    nitem->setPos(200, 0);
    scene->addItem(nitem);
    */
}
