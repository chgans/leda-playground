#include "documentmodel.h"
#include "documentobject.h"

#include <QStringList>

DocumentModel::DocumentModel(DocumentObject *root, QObject *parent):
    QAbstractItemModel(parent), m_root(root)
{
}

DocumentModel::~DocumentModel()
{
}

QVariant DocumentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        DocumentObject *item = static_cast<DocumentObject*>(index.internalPointer());
        if (index.column() == 0)
            return item->typeName();
        else
            return item->uid();
    }
    else
        return QVariant();
}

bool DocumentModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        DocumentObject *item = static_cast<DocumentObject*>(index.internalPointer());
        //item->setData(value, role);
        emit dataChanged(index, index);
        return true;
   }
   return false;
}

Qt::ItemFlags DocumentModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant DocumentModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0)
            return "Type";
        else
            return "Uid";
    }

    return QVariant();
}

QModelIndex DocumentModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DocumentObject *parentItem;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<DocumentObject*>(parent.internalPointer());

    DocumentObject *childItem = parentItem->children().value(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex DocumentModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    DocumentObject *childItem = static_cast<DocumentObject*>(index.internalPointer());
    DocumentObject *parentItem = childItem->parent();

    if (parentItem == m_root)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int DocumentModel::rowCount(const QModelIndex &parent) const
{
    DocumentObject *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<DocumentObject*>(parent.internalPointer());

    return parentItem->children().size();
}

int DocumentModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

