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
    QVariant headerData(int row) const;
    DocumentObject *parentObject();
    int row();

signals:

public slots:

};

#endif // DOCUMENTOBJECT_H
