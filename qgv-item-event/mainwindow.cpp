#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graphicsscene.h"
#include "graphicsview.h"

#include "graphicsselecttool.h"
#include "graphicslinetool.h"
#include "graphicsrecttool.h"
#include "graphicsbeziertool.h"

#include <QMessageBox>
#include <QActionGroup>
#include <QToolBar>

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

    connect(ui->graphicsView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(onViewContextMenuRequested(QPoint)));
    connect(ui->graphicsView, SIGNAL(mouseDoubleClicked(QPoint)),
            this, SLOT(onViewMouseDoubleClicked(QPoint)));
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
