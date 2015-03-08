#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graphicsscene.h"
#include "graphicsview.h"

#include "graphicsselecttool.h"
#include "graphicslinetool.h"
#include "graphicsrecttool.h"
#include "graphicsbeziertool.h"

#include <QTimer>
#include <QMessageBox>
#include <QActionGroup>
#include <QToolBar>

#include <QGraphicsDropShadowEffect>
#include <QGraphicsColorizeEffect>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_interactiveToolActionGroup(new QActionGroup(this)),
    m_interactiveToolsToolBar(new QToolBar(this))
{
    ui->setupUi(this);

    m_scene = new GraphicsScene(this);
    m_scene->setSceneRect(0, 0, 297, 210);

    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->scale(4, 4);

    addTool(new GraphicsSelectTool(this));
    addTool(new GraphicsLineTool(this));
    addTool(new GraphicsRectTool(this));
    addTool(new GraphicsBezierTool(this));
    addToolBar(m_interactiveToolsToolBar);

    /*
    GraphicsRectItem *item;
    item = new GraphicsRectItem();
    item->setRect(QRectF(-25, -25, 50, 50));
    item->setPos(103, 52);
    item->setBrush(Qt::darkGreen);
    item->setPen(QPen(Qt::darkRed));
    item->setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable);
    m_scene->addItem(item);
    item = new GraphicsRectItem();
    item->setRect(QRectF(-25, -25, 50, 50));
    item->setPos(169, 78);
    item->setBrush(Qt::darkRed);
    item->setPen(QPen(Qt::darkGreen));
    item->setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable);
    m_scene->addItem(item);
    */

    connect(ui->graphicsView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(onViewContextMenuRequested(QPoint)));
    connect(ui->graphicsView, SIGNAL(mouseDoubleClicked(QPoint)),
            this, SLOT(onViewMouseDoubleClicked(QPoint)));

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

void MainWindow::onViewContextMenuRequested(QPoint pos)
{
    Q_UNUSED(pos);
    QMenu menu(ui->graphicsView);
    menu.addAction(QIcon::fromTheme("document-open"), "blah");
    menu.exec(QCursor::pos());
}

void MainWindow::onViewMouseDoubleClicked(QPoint pos)
{
    Q_UNUSED(pos);
    QMessageBox::aboutQt(this);
}

void MainWindow::addTool(GraphicsTool *tool)
{
    bool firstTool = m_interactiveTools.count() == 0;
    bool firstAction = m_interactiveToolActionGroup->actions().count() == 0;
    QAction *action = tool->action();
    action->setCheckable(true);
    action->setData(QVariant::fromValue<GraphicsTool *>(tool));
    m_interactiveToolActionGroup->addAction(action);
    m_interactiveToolsToolBar->addAction(action);
    if (firstAction) {
        action->setChecked(true);
        firstAction = false;
    }
    else
        action->setChecked(false);
    connect(m_interactiveToolActionGroup, &QActionGroup::triggered,
            this, [this](QAction *action) {
        GraphicsTool *tool = action->data().value<GraphicsTool*>();
        ui->graphicsView->setTool(tool);
    });
    connect(tool, &GraphicsTool::finished,
            this, [this]() {
        // Todo: activate select tool
    });
    m_interactiveTools.append(tool);
    if (firstTool)
        ui->graphicsView->setTool(tool);
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
