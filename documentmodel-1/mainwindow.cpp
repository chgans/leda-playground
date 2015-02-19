#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "documentitem.h"
#include "librarydocumentmodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LibraryItem *lib1 = new LibraryItem();
    lib1->setName("libtruc");
    lib1->setAuthor("Ch'Gans");
    PackageItem  *pkgItem1 = new PackageItem();
    pkgItem1->setName("pkg1");
    lib1->addPackage(pkgItem1);
    EntityItem *ent1 = new EntityItem();
    ent1->setName("OpAmp");
    pkgItem1->addEntity(ent1);
    GenericItem *gen1 = new GenericItem();
    gen1->setName("gain");
    gen1->setValueType(QVariant::Double);
    gen1->setDefaultValue(100.0);
    ent1->addGeneric(gen1);
    GenericItem *gen2 = new GenericItem();
    gen2->setName("ileak");
    gen2->setValueType(QVariant::Double);
    gen2->setDefaultValue(42.0);
    ent1->addGeneric(gen2);
    PackageItem  *pkgItem2 = new PackageItem();
    pkgItem2->setName("pkg2");
    lib1->addPackage(pkgItem2);
    LibraryModel *libraryModel = new LibraryModel(this);
    libraryModel->setLibrary(lib1);
    ui->columnView->setModel(libraryModel);
    ui->tableView->setModel(libraryModel);
    ui->tableView->setRootIndex(libraryModel->index(0, 0));
    ui->treeView->setModel(libraryModel);
    ui->listView->setModel(libraryModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}
