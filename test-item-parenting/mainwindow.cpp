#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dockwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mScene(new QGraphicsScene)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(mScene);
    //mPort = new QGraphicsItem;
    //mScene->addItem(mPort);
    mPortPin = new QGraphicsLineItem();
    mPortPin->setLine(0, 0, 100, 0);
    mPortPin->setPen(QPen(Qt::black));
    mPortPin->setFlag(QGraphicsItem::ItemIsMovable, true);
    mPortPin->setFlag(QGraphicsItem::ItemIsSelectable, true);
    mPortPin->setPos(0, 0);
    mPortName = new QGraphicsSimpleTextItem("In+", mPortPin);
    mPortName->setFlag(QGraphicsItem::ItemIsMovable, true);
    mPortName->setFlag(QGraphicsItem::ItemIsSelectable, true);
    mPortName->setPos(50, 10);
    mPortId = new QGraphicsSimpleTextItem("1", mPortPin);
    //mPortId->setFlag(QGraphicsItem::ItemIsMovable, true);
    mPortId->setFlag(QGraphicsItem::ItemIsSelectable, true);
    mPortName->setPos(-50, 0);

    mScene->addItem(mPortPin);

    addDockWidget(Qt::LeftDockWidgetArea, new DockWidget(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}
