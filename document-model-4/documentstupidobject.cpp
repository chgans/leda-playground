#include "documentstupidobject.h"

DocumentStupidObject::DocumentStupidObject(QObject *parent) :
    DocumentObject(parent)
{
}

QString DocumentStupidObject::stupidProperty() const
{
    return m_stupidValue;
}

void DocumentStupidObject::setStupidProperty(const QString &value)
{
    m_stupidValue = value;
}
