#include "documentobject.h"

DocumentObject::DocumentObject(const QList<QVariant> &data,
                               DocumentObject *parent):
    m_data(data), m_parent(parent)
{

}

DocumentObject::~DocumentObject()
{
    qDeleteAll(m_children);
}

void DocumentObject::appendChild(DocumentObject *child)
{
    m_children.append(child);
}

DocumentObject *DocumentObject::child(int num)
{
    return m_children.value(num);
}

int DocumentObject::childCount() const
{
    return m_children.count();
}

int DocumentObject::columnCount() const
{
    return m_data.count();
}

QVariant DocumentObject::data(int column) const
{
    return m_data.value(column);
}

int DocumentObject::row() const
{
    if (m_parent)
        return m_parent->m_children.indexOf(const_cast<DocumentObject*>(this));
    return 0;
}

DocumentObject *DocumentObject::parent()
{
    return m_parent;
}





