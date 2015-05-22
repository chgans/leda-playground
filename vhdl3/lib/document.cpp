#include "designunit.h"
#include "document.h"

#include <QList>
#include <QVariant>
#include <QDebug>

DocumentReader::DocumentReader()
{
}

bool DocumentReader::read(QIODevice *device)
{
    xml.setDevice(device);

    if (xml.readNextStartElement()) {
        if (xml.name() == "leda-xdl" && xml.attributes().value("version") == "1.0")
            readDocument();
        else
            xml.raiseError(QObject::tr("The file is not an LibreEDA XDL version 1.0 file."));
    }

    return !xml.error();
}

QString DocumentReader::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(xml.errorString())
            .arg(xml.lineNumber())
            .arg(xml.columnNumber());
}

DesignUnit *DocumentReader::document() const
{
    return m_document;
}

void DocumentReader::reportUnknownTag(const QStringRef &tag)
{
    xml.raiseError(QString("\"%1\": Unexpected tag").arg(tag.toString()));
}

void DocumentReader::readDocument()
{
    m_document = new DesignUnit();
    while (xml.readNextStartElement()) {
        qDebug() << __FUNCTION__ << xml.name();
        if (xml.name() == "entities") {
            m_document->entities = readList<Entity>("entity", &DocumentReader::readEntity);
        }
        else {
            reportUnknownTag(xml.name());
        }
    }
}

Entity *DocumentReader::readEntity()
{
    Entity *entity = new Entity;
    entity->name = xml.attributes().value("name").toString();
    while (xml.readNextStartElement()) {
        qDebug() << __FUNCTION__ << xml.name();
        if (xml.name() == "description") {
            entity->description = xml.readElementText();
        }
        else if (xml.name() == "generics") {
            entity->generics = readList<Generic>("generic", &DocumentReader::readGeneric);
            xml.skipCurrentElement();
        }
        else if (xml.name() == "ports") {
            entity->ports = readList<Port>("port", &DocumentReader::readPort);
            xml.skipCurrentElement();
        }
        else {
            reportUnknownTag(xml.name());
        }
    }
    return entity;
}

Generic *DocumentReader::readGeneric()
{
    Generic *generic = new Generic();
    generic->name = xml.attributes().value("name").toString();

    while (xml.readNextStartElement()) {
        qDebug() << __FUNCTION__ << xml.name();
        if (xml.name() == "unit") {
            generic->unit = xml.readElementText();
        }
        else if (xml.name() == "required") {
            generic->required = xml.readElementText() == "true" ? true : false;
        }
        else {
            reportUnknownTag(xml.name());
        }
    }
    return generic;
}

Port *DocumentReader::readPort()
{

    Port *port = new Port();
    port->name = xml.attributes().value("name").toString();

    while (xml.readNextStartElement()) {
        qDebug() << __FUNCTION__ << xml.name();
        if (xml.name() == "unit") {
        }
        else if (xml.name() == "required") {
        }
        else {
            reportUnknownTag(xml.name());
        }
    }
    return port;
}
