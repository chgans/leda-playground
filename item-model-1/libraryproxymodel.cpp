#include "libraryproxymodel.h"
#include <QDebug>

LibraryProxyModel::LibraryProxyModel(QObject *parent)
{

}

LibraryProxyModel::~LibraryProxyModel()
{

}

QModelIndex LibraryProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

QModelIndex LibraryProxyModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int LibraryProxyModel::rowCount(const QModelIndex &parent) const
{
    return 0;
}

int LibraryProxyModel::columnCount(const QModelIndex &parent) const
{
    return sourceModel()->columnCount();
}

QModelIndex LibraryProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    return index(sourceIndex.child(0,0).row(),
                       sourceIndex.child(0,0).column());
}

QModelIndex LibraryProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    return sourceModel()->index(proxyIndex.row(), proxyIndex.column());
}

