#include "documentmodel.h"
#include "documentobject.h"
#include "documentstupidobject.h"

#include <QMetaObject>
#include <QMetaProperty>
#include <QDebug>

DocumentModel::DocumentModel(QObject *parent) :
    QAbstractItemModel(parent), m_root(new DocumentObject)
{
    m_root->setObjectName("Root");
    DocumentStupidObject *obj;
    obj = new DocumentStupidObject(m_root);
    obj->setObjectName("Obj1");
    obj->setStupidProperty("s1");
#if 1
    obj = new DocumentStupidObject(obj);
    obj->setObjectName("Obj2");
    obj->setStupidProperty("s2");
    obj = new DocumentStupidObject(obj);
    obj->setObjectName("Obj3");
    obj->setStupidProperty("s3");
    obj = new DocumentStupidObject(obj->parent());
    obj->setObjectName("Obj4");
    obj->setStupidProperty("s4");
    obj = new DocumentStupidObject(obj->parent()->parent());
    obj->setObjectName("Obj5");
    obj->setStupidProperty("s5");
    obj = new DocumentStupidObject(m_root);
    obj->setObjectName("Obj6");
    obj->setStupidProperty("s6");
#endif
}

void DocumentModel::clear()
{
    if (rowCount() > 0) {
        beginResetModel();
        removeRows(0, rowCount());
        endResetModel();
    }
}

//void DocumentModel::setRootObject(DocumentObject *object)
//{
//    m_root = object;
//}

DocumentObject *DocumentModel::object(const QModelIndex &index) const
{
    if (index.isValid())
        return static_cast<DocumentObject*>(index.internalPointer());
    qDebug() << "DocumentModel::object: index invalid" << index;
    return nullptr;
}

QModelIndex DocumentModel::addProperty(const QModelIndex &index, const QString &name, const QVariant &value)
{
    DocumentObject *obj = object(index);
    int column = obj->columnCount();
    beginInsertColumns(index, column, column);
    //obj->setData(name.toLocal8Bit().constData(), value);
    endInsertColumns();
    return createIndex(index.row(), column, obj);
}

void DocumentModel::removeProperty(const QModelIndex &index)
{
    beginRemoveColumns(index.parent(), index.column(), index.column());
    DocumentObject *obj = object(index);
    QString name(obj->dynamicPropertyNames().at(index.column()-obj->metaObject()->propertyCount()));
    //obj->setData(name.toLocal8Bit().constData(), QVariant());
    endRemoveColumns();
}

void DocumentModel::changeProperty(const QModelIndex &index, const QVariant &value)
{
    setData(index, value, Qt::DisplayRole);
}

QModelIndex DocumentModel::addObject(const QModelIndex &parent, DocumentObject *obj)
{
    int row = object(parent)->childCount();
    beginInsertRows(parent, row, row);
    obj->setParent(object(parent));
    endInsertRows();
    return createIndex(row, 0, obj);
}

void DocumentModel::removeObject(const QModelIndex &index)
{
    beginRemoveRows(index.parent(), index.row(), index.row());
    object(index)->setParent(nullptr);
    endRemoveRows();
}

QVariant DocumentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    DocumentObject *obj = object(index);
    return obj->data(index.column());
}

bool DocumentModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if (role != Qt::DisplayRole)
        return false;

    DocumentObject *obj = object(index);
    obj->setData(index.column(), value);
    emit dataChanged(index, index);

    return true;
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
        return m_root->propertyName(section);

    return QVariant();
}

QModelIndex DocumentModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DocumentObject *parentObject;

    if (!parent.isValid())
        parentObject = m_root;
    else
        parentObject = object(parent);

    DocumentObject *childObject = parentObject->child(row);

    if (childObject)
        return createIndex(row, column, childObject);
    else
        return QModelIndex();
}

QModelIndex DocumentModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    DocumentObject *parentObject = object(index)->parentObject();

    if (parentObject == m_root)
        return QModelIndex();

    return createIndex(parentObject->row(), 0, parentObject);
}

int DocumentModel::rowCount(const QModelIndex &parent) const
{
    DocumentObject *parentObject;

    if (parent.column() > 0)
            return 0;

    if (!parent.isValid())
        parentObject = m_root;
    else
        parentObject = object(parent);

    return parentObject->childCount();
}

int DocumentModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return object(parent)->columnCount();
    else
        return m_root->columnCount();
}
