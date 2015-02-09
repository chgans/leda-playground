#include "document.h"

#include <QVariant>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>

#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QTimeZone>
#include <QPointF>

Document::Document(QObject *parent) : QObject(parent)
{

}

Document::~Document()
{

}

void Document::load(const QString &filename)
{
    m_filename = filename;
    QFile file(filename);
    file.open(QFile::ReadOnly);
    QXmlStreamReader xml(&file);
    if (xml.readNextStartElement()) {
        if (xml.name() != "ledoc" && xml.attributes().value("version") == "1.0") {
            printError(&xml, "Not a LibreEDA version 0.0 document");
            return;
        }
        m_root = parseObject(&xml, 0);
        m_root->setParent(this);
    }
    if (xml.hasError()) {
        printError(&xml, xml.errorString());
        return;
    }
}

void Document::save()
{
}

bool Document::isModified() const
{
    return false;
}

QString Document::filename() const
{
    return m_filename;
}

QObject *Document::parseObject(QXmlStreamReader *xml, QObject *parent)
{
    xml->readNextStartElement();
    QString token = xml->name().toString();
    if (token != "object") {
        printError(xml, "Expected object element");
        return 0;
    }
    if (!xml->attributes().hasAttribute("type") ||
            !xml->attributes().hasAttribute("name")) {
        printError(xml, "Missing type or name attribute");
        return 0;
    }
    QString type = xml->attributes().value("type").toString();
    QString name = xml->attributes().value("name").toString();
    QObject *object = createObject(type);
    object->setParent(parent);
    object->setObjectName(name);

    xml->readNextStartElement();
    token = xml->name().toString();
    if (token == "properties") {
        if (!xml->attributes().hasAttribute("count")) {
            printError(xml, "Missing count attribute");
            return 0;
        }
        int n = xml->attributes().value("count").toInt();
        for (int i=0; i<n; i++) {
            xml->readNextStartElement();
            if (xml->name() != "property") {
                printError(xml, "Expected property element");
                return 0;
            }
            if (!xml->attributes().hasAttribute("type") ||
                    !xml->attributes().hasAttribute("name")) {
                printError(xml, "Missing type or name attribute");
                return 0;
            }
            QString type = xml->attributes().value("type").toString();
            QByteArray name = xml->attributes().value("name").toString().toLocal8Bit();
            QString value = xml->readElementText();
            QVariant var = createVariant(type, value);
            object->setProperty(name.constData(), var);
        }
        xml->skipCurrentElement(); // </properties>
    }

    xml->readNextStartElement();
    token = xml->name().toString();
    if (token == "objects") {
        int n = xml->attributes().value("count").toInt();
        for (int i=0; i<n; i++) {
            QObject *child = parseObject(xml, object);
        }
        xml->skipCurrentElement(); // </objects>
        token = xml->name().toString();
    }

    return object;
}

void Document::printError(QXmlStreamReader *xml, const QString &message)
{
    qDebug() << QString("Error in %1, line %2, column %3, element %4: %5")
                .arg(m_filename).arg(xml->lineNumber()).arg(xml->columnNumber()).arg(xml->name().toString()).arg(message);

}

QObject *Document::createObject(const QString &type)
{
    if (type == "line")
        return new QObject;
    return new QObject;
}

QVariant Document::createVariant(const QString &type, const QString &value)
{
    if (type == "string")
        return QVariant::fromValue<QString>(value);
    else if (type == "datetime") {
        qint64 msec = value.toLongLong();
        QDateTime dt;
        dt.setTimeZone(QTimeZone(0));
        dt.setMSecsSinceEpoch(msec);
        return QVariant::fromValue<QDateTime>(dt);
    }
    else if (type == "pointf") {
        QStringList tokens = value.split(",");
        QPointF p(tokens[0].toDouble(), tokens[1].toDouble());
        return QVariant::fromValue<QPointF>(p);
    }
    else
        return QVariant();
}

