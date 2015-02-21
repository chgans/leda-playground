#include "documentobject.h"

#include <QMetaObject>
#include <QMetaProperty>

#include <QDebug>

DocumentObject::DocumentObject(QObject *parent) :
    QObject(parent)
{
}

DocumentObject *DocumentObject::child(int row)
{
    return findChildren<DocumentObject*>(QString(), Qt::FindDirectChildrenOnly).value(row);
}

int DocumentObject::childCount() const
{
    return findChildren<DocumentObject*>(QString(), Qt::FindDirectChildrenOnly).count();
}

int DocumentObject::columnCount() const
{
    const QMetaObject *metaObj = metaObject();
    while (metaObj->superClass())
        metaObj = metaObj->superClass();
    return metaObject()->propertyCount() + dynamicPropertyNames().count();
}

QVariant DocumentObject::data(int column) const
{
    if (column < 0 || column > columnCount())
        return QVariant();

    const QMetaObject *metaObj = metaObject();
    while (metaObj->superClass())
        metaObj = metaObj->superClass();

    QString name;
    if (column > metaObject()->propertyCount()) {
        column -= metaObject()->propertyCount();
        name = dynamicPropertyNames().value(column);
    }
    else
        name = metaObject()->property(column).name();
    return property(name.toLocal8Bit().constData());
}

bool DocumentObject::setData(int column, const QVariant &value)
{
    if (column < 0 || column > columnCount())
        return false;

    const QMetaObject *metaObj = metaObject();
    while (metaObj->superClass())
        metaObj = metaObj->superClass();

    QString name;
    if (column > metaObject()->propertyCount()) {
        column -= metaObject()->propertyCount();
        name = dynamicPropertyNames().value(column);
    }
    else
        name = metaObject()->property(column).name();
    qDebug() << "setData" << column << name << value;
    return setProperty(name.toLocal8Bit().constData(), value);
}

QVariant DocumentObject::headerData(int row) const
{
    return property(metaObject()->property(row).name()) ;
}

DocumentObject *DocumentObject::parentObject()
{
    return static_cast<DocumentObject*>(parent());
}

int DocumentObject::row()
{
    if (!parentObject())
        return 0;
    return parentObject()->findChildren<DocumentObject*>(QString(), Qt::FindDirectChildrenOnly).indexOf(this);
}
