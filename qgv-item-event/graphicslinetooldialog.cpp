#include "graphicslinetooldialog.h"
#include "ui_graphicslinetooldialog.h"

GraphicsLineToolDialog::GraphicsLineToolDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphicsLineToolDialog)
{
    ui->setupUi(this);
}

GraphicsLineToolDialog::~GraphicsLineToolDialog()
{
    delete ui;
}
