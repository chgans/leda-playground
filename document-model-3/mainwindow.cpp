#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "documentitem.h"
#include "librarydocumentmodel.h"
#include "packageitemform.h"

GenericItem *makeGeneric(int idx)
{
    GenericItem *item = new GenericItem();
    if (idx % 2) {
        item->setName(QString("gain"));
        item->setValueType(QVariant::Double);
        item->setDefaultValue(100.0);
        item->setReadOnly(false);
    }
    else {
        item->setName(QString("ref"));
        item->setValueType(QVariant::String);
        item->setDefaultValue("blah");
        item->setReadOnly(true);
    }
    return item;
}

EntityItem *makeEntity(int idx)
{
    EntityItem *item = new EntityItem();
    if (idx % 2) {
        item->setName(QString("Operational Amplifier"));
    }
    else {
        item->setName(QString("Resistor"));
    }
    item->addGeneric(makeGeneric(0));
    item->addGeneric(makeGeneric(1));
    return item;
}

PackageItem *makePackage(int idx)
{
    PackageItem  *item = new PackageItem();
    item->setName(QString("leda.package.passive").arg(idx));
    item->setDisplay("Standard passive entities");
    item->setAuthor("Christian Gagneraud");
    item->setLicense("LGPL v2.1");
    item->addEntity(makeEntity(0));
    item->addEntity(makeEntity(1));
    return item;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),m_model(nullptr),m_form(nullptr)
{
    ui->setupUi(this);
    m_model = new DocumentModel(this);
    m_model->setDocumentItem(makePackage(0));
    ui->columnView->setModel(m_model);
    connect(ui->columnView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(onItemClicked(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onItemClicked(const QModelIndex &item)
{
    if (m_form)
        delete m_form;
    if (ui->centralWidget->layout()->count() > 1)
        ui->centralWidget->layout()->takeAt(1);
    m_form = new PackageItemForm();
    m_form->setModel(m_model);
    m_form->setRootModelIndex(item);
    ui->centralWidget->layout()->addWidget(m_form);

}
