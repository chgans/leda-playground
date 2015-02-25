#ifndef DOCUMENTOBJECT_H
#define DOCUMENTOBJECT_H

#include <QObject>
#include <QVariant>

class DocumentObject : public QObject
{
    Q_OBJECT
public:
    explicit DocumentObject(QObject *parent = 0);

    DocumentObject *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool setData(int column, const QVariant &value);
    const char *propertyName(int row) const;
    QString propertyNameString(int row) const;
    int propertyIndex(const char *name);
    int propertyIndex(const QString &name);
    DocumentObject *parentObject();
    int row();

signals:

public slots:

};

#endif // DOCUMENTOBJECT_H
