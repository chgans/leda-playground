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

QObject *DocumentReader::document() const
{
    return m_document;
}

void DocumentReader::readDocument()
{
    m_document = new QObject();
    while (xml.readNextStartElement()) {
        qDebug() << __FUNCTION__ << xml.name();
        if (xml.name() == "entities") {
            m_document->setProperty("entities",
                                    QVariant::fromValue<QObjectList>(readEntities(m_document)));
        }
        else {
            xml.skipCurrentElement();
        }
    }
}

QObjectList DocumentReader::readEntities(QObject *parent)
{
    int count = xml.attributes().value("count").toInt();
    QObjectList entities;
    entities.reserve(count);
    for (int i = 0; i < count; i++) {
        if (!xml.readNextStartElement()) {
            xml.raiseError(QString("Expected %1 entities, got only %2").arg(count).arg(i));
            return entities;
        }
        if (xml.name() != "entity") {
            xml.raiseError(QString("Expected \"entity\" got \"%1\"").arg(xml.name().toString()));
            return entities;
        }
        qDebug() << __FUNCTION__ << xml.name();
        entities.append(readEntity(parent));
    }
    return entities;
}

QObject *DocumentReader::readEntity(QObject *parent)
{
    QObject *entity = new QObject(parent);
    entity->setProperty("name", xml.attributes().value("name").toString());
    while (xml.readNextStartElement()) {
        qDebug() << __FUNCTION__ << xml.name();
        if (xml.name() == "generics") {
            entity->setProperty("generics",
                                QVariant::fromValue<QObjectList>(readGenerics(entity)));
            xml.skipCurrentElement();
        }
        else {
            xml.skipCurrentElement();
        }
    }
    return entity;
}

QObjectList DocumentReader::readGenerics(QObject *parent)
{
    int count = xml.attributes().value("count").toInt();
    QObjectList generics;
    generics.reserve(count);
    for (int i = 0; i < count; i++) {
        if (!xml.readNextStartElement()) {
            xml.raiseError(QString("Expected %1 generics, got only %2").arg(count).arg(i));
            return generics;
        }
        if (xml.name() != "generic") {
            xml.raiseError(QString("Expected \"generic\" got \"%1\"").arg(xml.name().toString()));
            return generics;
        }
        qDebug() << __FUNCTION__ << xml.name();
        generics.append(readGeneric(parent));
    }
    return generics;
}

QObject *DocumentReader::readGeneric(QObject *parent)
{
    QObject *generic = new QObject(parent);
    generic->setProperty("name", xml.attributes().value("name").toString());
    while (xml.readNextStartElement()) {
        qDebug() << __FUNCTION__ << xml.name();
        if (xml.name() == "unit") {
            generic->setProperty("unit", xml.readElementText());
        }
        else if (xml.name() == "required") {
            generic->setProperty("required", xml.readElementText());
        }
        else {
            xml.skipCurrentElement();
        }
    }
    return generic;
}
