#include "documentmodelexplorerwidget.h"
#include "documentmodel.h"

#include <QGridLayout>
#include <QTreeView>
#include <QHeaderView>

DocumentModelExplorerWidget::DocumentModelExplorerWidget(QWidget *parent) :
    QWidget(parent)
{
    setLayout(new QGridLayout);
    m_tree = new QTreeView();
    layout()->addWidget(m_tree);
    m_tree->header()->setStretchLastSection(true);
    m_tree->setSelectionMode(QTreeView::SingleSelection);
    m_tree->setSelectionBehavior(QTreeView::SelectRows);
    connect(m_tree->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(onCurrentChanged(QModelIndex,QModelIndex)));
}

void DocumentModelExplorerWidget::setModel(DocumentModel *model)
{
    m_model = model;
    m_tree->setModel(m_model);
    m_tree->expandAll();
    connect(m_tree->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(onCurrentChanged(QModelIndex,QModelIndex)));
}

DocumentModel *DocumentModelExplorerWidget::model()
{
    return m_model;
}

void DocumentModelExplorerWidget::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QModelIndex index = selected.indexes().first();
    emit selectionChanged(QPersistentModelIndex(index));
}

void DocumentModelExplorerWidget::onCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    emit selectionChanged(QPersistentModelIndex(current));
}
