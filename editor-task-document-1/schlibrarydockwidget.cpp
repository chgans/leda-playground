#include "schlibrarydockwidget.h"
#include "ui_schlibrarydockwidget.h"

SchLibraryDockWidget::SchLibraryDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::SchLibraryDockWidget),
    m_collectionItem(0),
    m_collectionModel(new SymbolCollectionTableViewModel()),
    m_paramModel(new DocumentParameterTableViewModel)
{
    ui->setupUi(this);
    ui->collectionView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->parameterView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    connect(ui->collectionView, SIGNAL(activated(QModelIndex)),
            this, SLOT(onSymbolSelectionChanged(QModelIndex)));
}

SchLibraryDockWidget::~SchLibraryDockWidget()
{
    delete ui;
}

void SchLibraryDockWidget::setSymbolCollection(DocumentSymbolCollectionItem *collection)
{
    m_collectionItem = collection;
    m_collectionModel->setRootItem(0, m_collectionItem);
    ui->collectionView->setModel(m_collectionModel);
    if (m_collectionItem->symbols()->childItemCount() > 0)
        ui->collectionView->selectionModel()->select(m_collectionModel->index(0, 0),
                                                     QItemSelectionModel::ClearAndSelect|QItemSelectionModel::Rows );
}

void SchLibraryDockWidget::onSymbolSelectionChanged(const QModelIndex &index)
{
    const DocumentSymbolItem *sym = m_collectionItem->symbol(index.row());
    m_paramModel->setRootItem(0, sym->parameters());
    ui->parameterView->setModel(m_paramModel);
    ui->parameterView->reset();
}
