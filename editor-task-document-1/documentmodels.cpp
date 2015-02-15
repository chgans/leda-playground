#include "documentmodels.h"
#include "document.h"
#include "documentitems.h"

DocumentTreeViewModel::DocumentTreeViewModel(QObject *parent):
    QAbstractItemModel(parent), m_doc(0), m_root(0)
{

}

// TBD: handle reset
void DocumentTreeViewModel::setRootItem(Document *doc, DocumentItem *root)
{
    m_doc = doc;
    m_root = root;
}

int DocumentTreeViewModel::rowCount(const QModelIndex &parent) const
{
    DocumentItem *parentItem = getItem(parent);
    return parentItem->childItemCount();
}

bool DocumentTreeViewModel::insertRows(int row, int count, const QModelIndex &parent)
{
    return false;
}

bool DocumentTreeViewModel::removeRows(int row, int count, const QModelIndex &parent)
{
    return false;
}

int DocumentTreeViewModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant DocumentTreeViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch(section) {
        case 0: return QVariant("Type");
        case 1: return QVariant("Name");
        case 2: return QVariant("Value");
        }
    }
    return QVariant();
}

QVariant DocumentTreeViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    DocumentItem *item = getItem(index);
    if (index.column() == 1)
        return item->name();
    else if (index.column() == 2)
        return item->value();

    return QVariant();
}

bool DocumentTreeViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

Qt::ItemFlags DocumentTreeViewModel::flags(const QModelIndex &index) const
{
    if (index.isValid() && index.column() != 0)
        return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    return 0;
}

QModelIndex DocumentTreeViewModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    DocumentItem *parentItem = getItem(parent);
    DocumentItem *childItem = parentItem->childItem(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex DocumentTreeViewModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    DocumentItem *childItem = getItem(index);
    DocumentItem *parentItem = childItem->parentItem();

    if (parentItem == m_root)
        return QModelIndex();

    return createIndex(parentItem->childIndex(), 0, parentItem);
}

DocumentItem *DocumentTreeViewModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        DocumentItem *item = static_cast<DocumentItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_root;
}

void DocumentParameterTableViewModel::setRootItem(Document *doc, DocumentParameterItemList *root)
{
    m_doc = doc;
    m_root = root;
}

int DocumentParameterTableViewModel::rowCount(const QModelIndex &parent) const
{
    return m_root->childItemCount();
}

int DocumentParameterTableViewModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}


QVariant DocumentParameterTableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch(section) {
        case 0: return QVariant("Visible");
        case 1: return QVariant("Name");
        case 2: return QVariant("Value");
        }
    }
    return QVariant();
}

QVariant DocumentParameterTableViewModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    DocumentParameterItem *param = m_root->listItem(index.row());
    switch(index.column()) {
    case 0: return param->isVisible();
    case 1: return param->paramName();
    case 2: return param->paramValue();
    }

    return QVariant();
}



int SymbolCollectionTableViewModel::rowCount(const QModelIndex &parent) const
{
    int n = m_root->symbols()->childItemCount();
    return n;
}

int SymbolCollectionTableViewModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant SymbolCollectionTableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch(section) {
        case 0: return QVariant("Name");
        case 1: return QVariant("Description");
        }
    }
    return QVariant();
}

QVariant SymbolCollectionTableViewModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    const DocumentSymbolItem *sym = m_root->symbol(index.row());
    switch(index.column()) {
    case 0: return sym->symbolName();
    case 1: return sym->description();
    }

    return QVariant();
}
