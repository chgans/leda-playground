#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "menuitem.h"

#include <QDockWidget>
#include <QTableWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTabBar>

#include <QDebug>

// Add, edit, remove object
// Modify view (zoom, ...)
// Change selection
// Alter GUI


/* Commands
 *
 * Groups:
 *  - App
 *    - new
 *    - open
 *    - save (TBD: go to document?)
 *    - save as
 *    - save copy as
 *    - close
 *    - import
 *    - export
 *    - print (preview, pdf)
 *    - quit
 *  - Document
 *    - doc option & properties
 *    - add/edit/remove
 *    - selection handling (TBD: got to view?)
 *  - Gui
 *    - show/hide docks
 *    - show/hide toolbars
 *    - show/hide panes
 *    - preferences
 *    -
 *  - View
 *    - zoom, pan, ...
 *    - grid&snap here?
 *    -
 *
 *
 **/



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    addDocks();
    QAction *act;
    act = new QAction(this);
    act->setShortcut(QKeySequence("alt+["));
    act->setCheckable(true);
    act->setChecked(true);
    connect(act, SIGNAL(triggered(bool)),
            this, SLOT(showHideLeftDockArea(bool)));
    addAction(act);
    act = new QAction(this);
    act->setShortcut(QKeySequence("alt+]"));
    act->setCheckable(true);
    act->setChecked(true);
    connect(act, SIGNAL(triggered(bool)),
            this, SLOT(showHideRightDockArea(bool)));
    addAction(act);

    addDockWidget(Qt::RightDockWidgetArea, new QDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, new QDockWidget);

    addDockWidget(Qt::LeftDockWidgetArea, new QDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, new QDockWidget);

    MenuItem *fmenu = new MenuItem("File");
    fmenu->setChildren(MenuItemList() <<
                       new MenuItem("Open") <<
                       new MenuItem("Save") <<
                       new MenuItem("Close"));
    MenuItem *vmenu = new MenuItem("View");
    MenuItem *tmenu = new MenuItem("Toolbars", vmenu);
    MenuItem *dmenu = new MenuItem("Drawing", tmenu);
    new MenuItem("Line", dmenu);
    new MenuItem("Spline", dmenu);
    new MenuItem("Circle", dmenu);

    MenuItem *topItem = new MenuItem("");
    topItem->setChildren(MenuItemList() << fmenu << vmenu);
    setMenuBar(new QMenuBar);
    populateMenu(0, topItem);
    populateTreeView(0, topItem);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QDockWidget *createOutpuPane(const QString &name)
{
    QDockWidget *dw = new QDockWidget(name);
    dw->setAllowedAreas(Qt::BottomDockWidgetArea);
    dw->setFeatures(QDockWidget::DockWidgetVerticalTitleBar);
    dw->setWidget(new QTableWidget);
    //dw->setTitleBarWidget(0);
    return dw;
}

void MainWindow::addDocks()
{
    m_outputPanes << createOutpuPane("1. General messages")
                  << createOutpuPane("2. Rule check results")
                  << createOutpuPane("3. JavaScript console");
    QDockWidget *dw = 0;
    foreach (QDockWidget *dock, m_outputPanes) {
        addDockWidget(Qt::BottomDockWidgetArea, dock);
        if (dw)
            tabifyDockWidget(dw, dock);
        dw = dock;
        dock->setAllowedAreas(Qt::NoDockWidgetArea);
    }
}

void MainWindow::populateMenu(QMenu *topMenu, MenuItem *topItem)
{
    foreach (MenuItem *item, topItem->children()) {
        if (item->children().isEmpty()) {
            if (topMenu)
                topMenu->addAction(item->title());
            else
                menuBar()->addMenu(item->title());
        }
        else {
            QMenu *menu;
            if (topMenu)
                menu = topMenu->addMenu(item->title());
            else
                menu = menuBar()->addMenu(item->title());
            populateMenu(menu, item);
        }
    }
}

void MainWindow::populateTreeView(QTreeWidgetItem *topWidgetItem, MenuItem *topMenuItem)
{
    foreach (MenuItem *mitem, topMenuItem->children()) {
        QTreeWidgetItem *witem;
        witem = new QTreeWidgetItem(QStringList() << mitem->title(), 0);
        if (topWidgetItem) {
            topWidgetItem->addChild(witem);
        }
        else {
            ui->treeWidget->addTopLevelItem(witem);
        }
        populateTreeView(witem, mitem);
    }
}

void MainWindow::showHideLeftDockArea(bool show)
{
    setDockWidgetAreaVisible(Qt::LeftDockWidgetArea, show);
}

void MainWindow::showHideRightDockArea(bool show)
{
    setDockWidgetAreaVisible(Qt::RightDockWidgetArea, show);
}

void MainWindow::showHideOutputPane(int nb)
{
    qDebug() << Q_FUNC_INFO;
}

void MainWindow::setDockWidgetAreaVisible(Qt::DockWidgetArea area, bool visible)
{
    foreach (QDockWidget *dock, findChildren<QDockWidget*>()) {
        if (dockWidgetArea(dock) == area) {
            if (dock->toggleViewAction()->isChecked() != visible)
                dock->toggleViewAction()->trigger();
        }
    }
}
