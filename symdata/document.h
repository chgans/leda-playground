#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include "item.h"

class Document : public QObject
{
    Q_OBJECT
public:
    explicit Document(QObject *parent = 0);

    UID addItem(const Item *item);
    const Item *getItem(const UID &uid) const;
    bool deleteItem(const UID &uid);
    bool updateItem(const UID &uid, const Item *update);
};

#endif // DOCUMENT_H
