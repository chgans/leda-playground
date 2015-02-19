#include "dockwidget.h"
#include "ui_dockwidget.h"

DockWidget::DockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockWidget)
{
    ui->setupUi(this);
    TreeModel *model = new TreeModel("data", this);
    model->setupModelData();
    ui->treeView->setModel(model);
}

DockWidget::~DockWidget()
{
    delete ui;
}
