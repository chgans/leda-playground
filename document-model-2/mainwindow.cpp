#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "documentmodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setModel(DocumentModel *model)
{
    ui->treeView->setModel(model);
}

