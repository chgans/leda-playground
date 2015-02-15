#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "documentitems.h"
#include "documentmodels.h"

#include "schlibrarydockwidget.h"

#include <QGraphicsScene>

DocumentSymbolItem *createSymbolItem(DocumentSymbolCollectionItem *parent, int id)
{
    DocumentSymbolItem *sym = new DocumentSymbolItem(0);
    sym->setSymbolName("sym");
    sym->setDescription(QString("a %1 symbol").arg(id));
    for (int i=0; i<5; i++) {
        sym->addParameter(QString("param%1.%2").arg(id).arg(i), QString("%1").arg(i), false);
    }
    parent->addSymbol(sym);
    return sym;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_scene(new QGraphicsScene),
    m_docModel(new DocumentTreeViewModel)
{
    m_ui->setupUi(this);
    m_scene->setSceneRect(0, 0, 297, 210);
    m_ui->view->setScene(m_scene);

    DocumentSymbolCollectionItem *rootItem = new DocumentSymbolCollectionItem(0);
    for (int i=0; i<5; i++)
        DocumentSymbolItem *item = createSymbolItem(rootItem, i);

    m_docModel->setRootItem(0, rootItem);
    m_ui->treeView->setModel(m_docModel);
    m_ui->treeView->setRootIsDecorated(true);

    SchLibraryDockWidget *sldw = new SchLibraryDockWidget();
    sldw->setSymbolCollection(rootItem);
    addDockWidget(Qt::LeftDockWidgetArea, sldw);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}
