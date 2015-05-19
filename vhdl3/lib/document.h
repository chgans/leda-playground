#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "lib_global.h"

#include <QXmlStreamReader>

// ALL:
// - <position><x>1</x><y>2</y></position>
// - <rotation>25.36</rotation>
// - <opacity>0.98</opacity>
// - <locked>0</locked>
// - <x-mirrored>0</x-mirrored>
// - <y-mirrored>1</y-mirrored>
// - <z-value>2.54<z-value>

// Shape: Circle, Ellipse, Rect, Arc
// - <pen><width></width><color></color></pen> (TODO: style, capstyle, joinStyle)
// - <brush><style></style><color></color></brush> (TODO: gradient and texture)

// Line
// - pen
// - p1, p2
// Pin
// - <name> + visible
// - <description> + visible
// - <length>
// - TBD: color? style? (wire, bus, ...)
// - TBD: elecType and decoration/decoration or {inside|outside}[edge]Symbol
// Text (label)
// TBD: groups, textarea, picture, ...

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
