#include "documentobject.h"
#include <QVariant>

DocumentObject::DocumentObject(DocumentObject *parent) : QObject(parent)
{

}

DocumentObject::~DocumentObject()
{

}

void DocumentObject::setUid(quint64 uid)
{

}

void DocumentObject::setTypeName(const QString &name)
{

}

DocumentObjectList DocumentObject::children()
{
    DocumentObjectList list;
    foreach(QObject *obj, findChildren<DocumentObject*>(QString(), Qt::FindDirectChildrenOnly))
        list << static_cast<DocumentObject*>(obj);
    return list;
}

DocumentObject *DocumentObject::parent()
{
    return static_cast<DocumentObject*>(QObject::parent());
}

void DocumentObject::appendChild(DocumentObject *child)
{
}

int DocumentObject::row() const
{
    //if (m_parent)
    //    return m_parent->m_children.indexOf(const_cast<DocumentObject*>(this));
    return 0;
}

