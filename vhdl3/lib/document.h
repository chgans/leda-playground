#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "lib_global.h"
#include "designunit.h"

#include <QXmlStreamReader>
#include <functional>

class LIBSHARED_EXPORT DocumentReader
{

public:
    DocumentReader();

    bool read(QIODevice *device);
    QString errorString() const;
    DesignUnit *document() const;

private:
    QXmlStreamReader xml;
    DesignUnit *m_document;
    void reportUnknownTag(const QStringRef &tag);
    void readDocument();
    Entity *readEntity();
    Generic *readGeneric();
    Port *readPort();

//    template<typename T>
//    struct ItemReader {
//        typedef T* (DocumentReader::*function)(void);
//    };

//    template<typename T>
//    using ItemReaderFunc = typename ItemReader<T>::function;

    template<class T>
    QList<T *> readList(const QString &tagName,
                        T* (DocumentReader::*reader)(void))
    {
        int count = xml.attributes().value("count").toInt();
        QList<T *> items;
        items.reserve(count);
        for (int i = 0; i < count; i++) {
            if (!xml.readNextStartElement()) {
                xml.raiseError(QString("Expected %1 %2, got only %3")
                               .arg(count).arg(tagName).arg(i));
                return items;
            }
            if (xml.name() != tagName) {
                xml.raiseError(QString("Expected \"%1\" got \"%2\"")
                               .arg(tagName).arg(xml.name().toString()));
                return items;
            }
            items.append((this->*reader)());
        }
        return items;
    }
};


#endif // DOCUMENT_H
