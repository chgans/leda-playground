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
    //return findChildren<DocumentObject*>(QString(), Qt::FindDirectChildrenOnly).value(row);
    return static_cast<DocumentObject*>(children().value(row));
}

int DocumentObject::childCount() const
{
    //return findChildren<DocumentObject*>(QString(), Qt::FindDirectChildrenOnly).count();
    return children().count();
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
#if 0
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
#else
    return metaObject()->property(column).read(this);
#endif
}

bool DocumentObject::setData(int column, const QVariant &value)
{
    if (column < 0 || column > columnCount())
        return false;
#if 0
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
#else
    return  metaObject()->property(column).write(this, value);
#endif
}

const char *DocumentObject::propertyName(int row) const
{
    return metaObject()->property(row).name();
}

QString DocumentObject::propertyNameString(int row) const
{
    return QString(QByteArray(propertyName(row)));
}

int DocumentObject::propertyIndex(const char *name)
{
    for (int i=0; i<metaObject()->propertyCount(); i++)
        if (!strcmp(name, metaObject()->property(i).name()))
            return i;
    Q_ASSERT(false);
}

int DocumentObject::propertyIndex(const QString &name)
{
    return propertyIndex(name.toLocal8Bit().constData());
}

DocumentObject *DocumentObject::parentObject()
{
    return static_cast<DocumentObject*>(parent());
}

int DocumentObject::row()
{
    if (!parentObject())
        return 0;
    //return parentObject()->findChildren<DocumentObject*>(QString(), Qt::FindDirectChildrenOnly).indexOf(this);
    return parentObject()->children().indexOf(this);
}
