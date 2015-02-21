#ifndef DOCUMENTSTUPIDOBJECT_H
#define DOCUMENTSTUPIDOBJECT_H

#include "documentobject.h"

class DocumentStupidObject : public DocumentObject
{
    Q_OBJECT
public:
    explicit DocumentStupidObject(QObject *parent = 0);
    Q_PROPERTY(QString stupidProperty READ stupidProperty WRITE setStupidProperty)

    QString stupidProperty() const;
    void setStupidProperty(const QString &value);

signals:

public slots:

private:
    QString m_stupidValue;
};

#endif // DOCUMENTSTUPIDOBJECT_H
