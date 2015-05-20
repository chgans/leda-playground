#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "lib_global.h"

#include <QXmlStreamReader>

class LIBSHARED_EXPORT DocumentReader
{

public:
    DocumentReader();

    bool read(QIODevice *device);
    QString errorString() const;
    QObject *document() const;

private:
    QXmlStreamReader xml;
    QObject *m_document;
    void readDocument();
    QObjectList readEntities(QObject *parent);
    QObject *readEntity(QObject *parent);
    QObjectList readGenerics(QObject *parent);
    QObject *readGeneric(QObject *parent);

};

#endif // DOCUMENT_H
