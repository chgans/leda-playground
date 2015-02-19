#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "UngroupProxyModel.h"
#include "libraryproxymodel.h"

#include <QAbstractItemModel>

#include <QStandardItem>
#include <QStandardItemModel>
#include <KSelectionProxyModel>



QList<QStandardItem *> createParameter(int id)
{
    QList<QStandardItem *> param;
    param << new QStandardItem(QString("param%1").arg(id))      // Name
        << new QStandardItem(QString("value%1").arg(id)); // Value
    return param;
}

QStandardItem *createGraphItem()
{

}

QList<QStandardItem *> createPort(int id)
{
    QList<QStandardItem *> port;
    port << new QStandardItem(QString("%1").arg(id))
         << new QStandardItem(QString("label%1").arg(id))
         << new QStandardItem(id %2 ? QString("output") : QString("input"));
    return port;

}

QStandardItem *createSymbol(int id)
{
    QStandardItem *sym = new QStandardItem(QString("sym-%1").arg(id));
    sym->setColumnCount(2);
    QStandardItem *name = new QStandardItem(QString("sym-%1").arg(id));
    QStandardItem *desc = new QStandardItem(QString("Symbol #%1").arg(id));
    sym->setChild(0, 0, name);
    sym->setChild(0, 1, desc);
    QStandardItem *params = new QStandardItem(QString("Parameters"));
    for (int i=0; i<5; i++)
        params->appendRow(createParameter(i));
    sym->appendRow(params);

    QStandardItem *ports = new QStandardItem(QString("Ports"));
    for (int i=0; i<5; i++)
        ports->appendRow(createPort(i));
    sym->appendRow(ports);

    return sym;
}

QStandardItemModel *createLibrary()
{
    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(2);
    //QStandardItem *libItem = new QStandardItem(QString("Test lib"));

    //model->setItem(0, 0, libItem);
    for (int i=0; i<10; i++)
        //libItem->appendRow(createSymbol(i));
        model->appendRow(createSymbol(i));
    return model;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStandardItemModel *model = createLibrary();

    ui->treeView->setModel(model);
    ui->treeView->expandAll();

    ui->libView->setModel(model);
    ui->libView->setRootIndex(model->index(2, 2));
    ui->libView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->libView->setAlternatingRowColors(true);
    ui->libView->horizontalHeader()->setStretchLastSection(true);

    KSelectionProxyModel *selectionProxy = new KSelectionProxyModel(ui->treeView->selectionModel(), this);
    selectionProxy->setSourceModel(model);
    ui->treeView2->setModel(selectionProxy);
    ui->treeView2->expandAll();

    UngroupProxyModel *ungroupProxy = new UngroupProxyModel(this);
    ungroupProxy->setSourceModel(model);
    ui->pinView->setModel(ungroupProxy);

    LibraryProxyModel *libProxy = new LibraryProxyModel(this);
    libProxy->setSourceModel(model);
    ui->paramView->setModel(libProxy);
}

MainWindow::~MainWindow()
{
    delete ui;
}



#if 0

QStandardItemModel *createModel()
{

    QStandardItemModel *model = new QStandardItemModel(0, 3);
    int r;
    // Plain property
    for (r=0; r<2; r++ ) {
        QStandardItem *nameItem = new QStandardItem(QString("Name%0").arg(r));
        model->setItem(r, 0, nameItem);
        QStandardItem *valItem = new QStandardItem(QString("Val%0").arg(r));
        model->setItem(r, 1, valItem);
    }
    // Indexed properties
    for (; r<4; r++ ) {
        QStandardItem *nameItem = new QStandardItem(QString("Name%0").arg(r));
        QStandardItem *valItem = new QStandardItem(QString("[3 items]"));
        for (int i=0; i<3; i++ ) {
            QList<QStandardItem *> subItems;
            subItems << new QStandardItem() << new QStandardItem(QString("[%0]").arg(i))
                     << new QStandardItem(QString("Val%0.%1").arg(r).arg(i));
            nameItem->appendRow(subItems);
        }
        model->setItem(r, 0, nameItem);
        model->setItem(r, 1, valItem);
    }
    // Aggregated properties
    for (; r<6; r++ ) {
        QStandardItem *nameItem = new QStandardItem(QString("Name%0").arg(r));
        QStandardItem *valItem = new QStandardItem(QString("{4 items}"));
        for (int i=0; i<4; i++ ) {
            QList<QStandardItem *> subItems;
            subItems << new QStandardItem(QString("Name%0.%1").arg(r).arg(i))
                     << new QStandardItem(QString("Val%0.%1").arg(r).arg(i));
            nameItem->appendRow(subItems);
        }
        model->setItem(r, 0, nameItem);
        model->setItem(r, 1, valItem);
    }

    return model;
}

void createModel1()
{
    /*
    int nrows = 5;
    int ncols = 2;
    QStandardItemModel *model = new QStandardItemModel(nrows, ncols, this);
    for (int r=0; r<nrows; r++ )
        for (int c=0; c<ncols; c++) {
            QStandardItem *item = new QStandardItem(QString("Row:%0, Column:%1").arg(r).arg(c));
            if (c == 0 )
                for (int i=0; i<3; i++ )
                    item->appendRow(new QStandardItem(QString("Item %0").arg(i)));
            model->setItem(r, c, item);
        }
        */
}

#endif
