#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "editorfactory.h"
#include "documentfactory.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    EditorFactory *m_editorFactory = EditorFactory::instance();
    DocumentFactory *m_documentFactory = DocumentFactory::instance();
}

MainWindow::~MainWindow()
{
    delete ui;
}
