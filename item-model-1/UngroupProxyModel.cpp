
#include <QtCore>
#include "UngroupProxyModel.h"

class UngroupProxyModel::Private {
public:
    UngroupProxyModel * instance;
    QHash<int, int> sourceRootsCounts; 
    bool aboutToRemoveRoots;

    int source_root2count(int row) {
        if (!instance->sourceModel())
            return 0;

        if (!sourceRootsCounts.contains(row)) {
            sourceRootsCounts[row] = instance->sourceModel()->rowCount(
                instance->sourceModel()->index(row,0)
            );
        }

        return sourceRootsCounts[row];
    }

    int row_fromSource(int root, int row) {
        for (int r =0; r< root; r++)
           row += source_root2count(r);
        return row;
    }

    QPair<int,int> row_toSource(int row) {
        int root = 0;
        for (int r =0; r < instance->sourceModel()->rowCount(); r++) {
            root = r;
            int rows_in_root = source_root2count(r);
            if (row >= rows_in_root)
                row -= rows_in_root;
            else break;
        }
        
        return qMakePair(root, row);
    }
};

UngroupProxyModel::UngroupProxyModel(QObject * parent)
:QAbstractProxyModel(parent)
{
    d = new Private;
    d->instance = this;
    d->aboutToRemoveRoots = false;
}

UngroupProxyModel::~UngroupProxyModel()
{
    delete d;
}

void UngroupProxyModel::setSourceModel(QAbstractItemModel * m)
{
    if (sourceModel()) {
        disconnect(sourceModel(), SIGNAL(rowsAboutToBeInserted(QModelIndex, int, int)), 
                   this, SLOT(source_rowsAboutToBeInserted(QModelIndex, int, int)));
        disconnect(sourceModel(), SIGNAL(rowsInserted(QModelIndex, int, int)), 
                   this, SLOT(source_rowsInserted(QModelIndex, int, int)));

        disconnect(sourceModel(), SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)), 
                   this, SLOT(source_rowsAboutToBeRemoved(QModelIndex, int, int)));
        disconnect(sourceModel(), SIGNAL(rowsRemoved(QModelIndex, int, int)), 
                   this, SLOT(source_rowsRemoved(QModelIndex, int, int)));

        disconnect(sourceModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), 
                   this, SLOT(source_dataChanged(QModelIndex,QModelIndex)));
        disconnect(sourceModel(), SIGNAL(modelReset()), 
                   this, SLOT(source_modelReset()));

        disconnect(sourceModel(), SIGNAL(layoutAboutToBeChanged()), this, SIGNAL(layoutAboutToBeChanged()));
        disconnect(sourceModel(), SIGNAL(layoutChanged()), this, SIGNAL(layoutChanged()));
    }

    QAbstractProxyModel::setSourceModel(m);

    if (sourceModel()) {
        connect(sourceModel(), SIGNAL(rowsAboutToBeInserted(QModelIndex, int, int)), 
                this, SLOT(source_rowsAboutToBeInserted(QModelIndex, int, int)));
        connect(sourceModel(), SIGNAL(rowsInserted(QModelIndex, int, int)), 
                this, SLOT(source_rowsInserted(QModelIndex, int, int)));

        connect(sourceModel(), SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)), 
                this, SLOT(source_rowsAboutToBeRemoved(QModelIndex, int, int)));
        connect(sourceModel(), SIGNAL(rowsRemoved(QModelIndex, int, int)), 
                this, SLOT(source_rowsRemoved(QModelIndex, int, int)));

        connect(sourceModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), 
                this, SLOT(source_dataChanged(QModelIndex,QModelIndex)));
        connect(sourceModel(), SIGNAL(modelReset()), 
                this, SLOT(source_modelReset()));

        connect(sourceModel(), SIGNAL(layoutAboutToBeChanged()), this, SIGNAL(layoutAboutToBeChanged()));
        connect(sourceModel(), SIGNAL(layoutChanged()), this, SIGNAL(layoutChanged()));
    }

    reset();
}

QModelIndex UngroupProxyModel::mapFromSource(const QModelIndex & source) const
{
    if (!sourceModel())
        return QModelIndex();

    if (!source.parent().isValid())
        return QModelIndex();

    return index(d->row_fromSource(source.parent().row(), source.row()), source.column());
}

QModelIndex UngroupProxyModel::mapToSource(const QModelIndex & proxy) const
{
    if (!sourceModel())
        return QModelIndex();

    QPair<int, int> pos = d->row_toSource(proxy.row());
    int root_row = pos.first;
    int row = pos.second;

    QModelIndex p = sourceModel()->index(root_row, proxy.column());
    return sourceModel()->index(row, proxy.column(), p);
}

QModelIndex	UngroupProxyModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

QModelIndex UngroupProxyModel::index(int row, int column, const QModelIndex &) const
{
    return createIndex(row, column);
}

int	UngroupProxyModel::rowCount(const QModelIndex & p) const
{
    if (p.isValid()) return 0;
    if (!sourceModel()) return 0;

    int count = 0;
    for (int root_row =0; root_row< sourceModel()->rowCount(); root_row++)
        count += d->source_root2count(root_row);

    return count;
}

int	UngroupProxyModel::columnCount(const QModelIndex & p) const
{
    if (p.isValid()) return 0;
    if (!sourceModel()) return 0;
    return sourceModel()->columnCount();
}

void UngroupProxyModel::source_rowsAboutToBeInserted(QModelIndex p, int from, int to)
{
    if (!p.isValid()) 
        return;

    int f = d->row_fromSource(p.row(), from);
    int t = f + (from-to);
    beginInsertRows(QModelIndex(), f, t);
}

void UngroupProxyModel::source_rowsInserted(QModelIndex p, int, int)
{
    d->sourceRootsCounts.clear();
    if (!p.isValid())
        return;

    endInsertRows();
}

void UngroupProxyModel::source_rowsAboutToBeRemoved(QModelIndex p, int from, int to)
{
    if (!p.isValid()) {
        //remove root items
        int f = d->row_fromSource(from,0);
        int t = d->row_fromSource(to,0)+ d->source_root2count(to);

        if (f != t) {
            beginRemoveRows(QModelIndex(), f, t-1);
            d->aboutToRemoveRoots = true;
        }
        
        return;
    }

    int f = d->row_fromSource(p.row(), from);
    int t = f + (from-to);
    beginRemoveRows(QModelIndex(), f, t);
}

void UngroupProxyModel::source_rowsRemoved(QModelIndex p, int, int)
{
    d->sourceRootsCounts.clear();

    if (!p.isValid()) {
        //remove root items
        if (d->aboutToRemoveRoots) {
            d->aboutToRemoveRoots = false;
            endRemoveRows();
        }        
        return;
    }

    endRemoveRows();
}

void UngroupProxyModel::source_dataChanged(QModelIndex tl, QModelIndex br)
{
    QModelIndex p_tl = mapFromSource(tl);
    QModelIndex p_br = mapFromSource(br);
    emit dataChanged(p_tl, p_br);
}

void UngroupProxyModel::source_modelReset()
{
    d->sourceRootsCounts.clear();
    reset();
}
