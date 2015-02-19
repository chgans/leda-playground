#include "documentmodel.h"
#include "documentobject.h"

#include <QStringList>

DocumentModel::DocumentModel(QObject *parent):
    QAbstractItemModel(parent)
{
    QList<QVariant> data;
    data << "Title" << "Summary";
    m_root = new DocumentObject(data);
    int i, j;
    for (i=0; i<5; i++) {
        data.clear();
        data << QString("Item %1").arg(i) << "blah...";
        DocumentObject *item = new DocumentObject(data, m_root);
        m_root->appendChild(item);
        for (j=0; j<3; j++) {
            data.clear();
            data << QString("Item %1.%2").arg(i).arg(j) << "blah...";
            DocumentObject *leaf = new DocumentObject(data, item);
            item->appendChild(leaf);
        }
    }
}

DocumentModel::~DocumentModel()
{
    delete m_root;
}

QVariant DocumentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        DocumentObject *item = static_cast<DocumentObject*>(index.internalPointer());
        return item->data(index.column());
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
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_root->data(section);

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

    DocumentObject *childItem = parentItem->child(row);
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

    return parentItem->childCount();
}

int DocumentModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<DocumentObject*>(parent.internalPointer())->columnCount();
    else
        return m_root->columnCount();
}

