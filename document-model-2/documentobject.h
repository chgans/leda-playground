#ifndef DOCUMENTOBJECT_H
#define DOCUMENTOBJECT_H

#include <QObject>
#include <QString>

class DocumentObject;
typedef QList<DocumentObject*> DocumentObjectList;

class DocumentObject : private QObject
{
    Q_OBJECT
public:
    explicit DocumentObject(DocumentObject *parent = 0);
    ~DocumentObject();

    Q_PROPERTY(quint64 uid READ uid WRITE setUid)
    Q_PROPERTY(QString typeName READ typeName WRITE setTypeName)

    quint64 uid() const { return m_uid; }
    void setUid(quint64 uid);

    QString typeName() const  { return m_typeName; }
    void setTypeName(const QString &name);

    DocumentObjectList children();
    DocumentObject *parent();

    void appendChild(DocumentObject *child);
    // TBD: remove child

    // ?
    int row() const;

    // properties R/W access and enum, both static and dynamic
signals:

public slots:

private:
    Q_DISABLE_COPY(DocumentObject)

    quint64 m_uid;
    QString m_typeName;
};

#endif // DOCUMENTOBJECT_H
