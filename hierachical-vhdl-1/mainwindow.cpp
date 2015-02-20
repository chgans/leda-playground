#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "entity.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Package *package = new Package();
    QVariantMap params;
    package->setName("stdpkg");
    for (int i=0; i<5; i++)
        params.insert(QString("c%1").arg(i), QString("val%1").arg(i));
    package->setContants(params);
    params.clear();
    for (int i=0; i<5; i++)
        params.insert(QString("g%1").arg(i), QString("val%1").arg(i));
    package->setGenerics(params);
    PackageModel *model = new PackageModel(this);
    model->setPackage(package);
    ui->treeView->setModel(model);
    ui->treeView->setRootIsDecorated(true);
    ui->treeView->expandAll();
}

MainWindow::~MainWindow()
{
    delete ui;
}
