#ifndef DOCUMENTOBJECT_H
#define DOCUMENTOBJECT_H

#include <QList>
#include <QVariant>

class DocumentObject
{
public:
    explicit DocumentObject(const QList<QVariant> &data,
                            DocumentObject *parent = 0);
    ~DocumentObject();

    void appendChild(DocumentObject *child);
    DocumentObject *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    DocumentObject *parent();

private:
    QList<DocumentObject *> m_children;
    QList<QVariant> m_data;
    DocumentObject *m_parent;
    QString m_typeName;

};

#endif // DOCUMENTOBJECT_H
