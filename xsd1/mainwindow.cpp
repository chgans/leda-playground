#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "symbol.h"
#include "symbolreader.h"
#include "symbolwriter.h"
#include "treemodel.h"

#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QFileSystemModel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_scene(new QGraphicsScene)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);

    QFileSystemModel *m = new QFileSystemModel(ui->fileSystemView);
    m->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    m->setReadOnly(true);
    m->setRootPath(SYMBOL_DIR);
    ui->fileSystemView->setModel(m);
    ui->fileSystemView->setColumnHidden(1, true);
    ui->fileSystemView->setColumnHidden(2, true);
    ui->fileSystemView->setColumnHidden(3, true);
    ui->fileSystemView->header()->setVisible(false);

    connect(m, &QFileSystemModel::directoryLoaded,
            this, [this, m](const QString &root) {
        static bool loaded = false;
        if (loaded)
            return;
        ui->fileSystemView->setRootIndex(m->index(root));
        ui->fileSystemView->expandAll();
        loaded = true;
    });
    connect(ui->fileSystemView, &QTreeView::activated,
            this, [this, m](const QModelIndex &index) {
        QFileInfo info(m->filePath(index));
        qDebug() << info.fileName();
        if (info.isDir()) {
            //m->setRootPath(info.filePath());
        }
        else if (info.fileName().endsWith(".xml")) {
            SymbolReader *parser = new SymbolReader();
            Symbol *sym = parser->parse(info.filePath());
            if (sym == nullptr) {
                qCritical() << parser->errorString();
                return;
            }
            ui->captionLineEdit->setText(sym->caption);
            ui->descriptionLineEdit->setText(sym->description);
            m_scene->clear();
            QGraphicsItemGroup *group;
            group = m_scene->createItemGroup(sym->drawings);
            ui->graphicsView->fitInView(group, Qt::KeepAspectRatio);
            ui->graphicsView->scale(0.8, 0.8);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

