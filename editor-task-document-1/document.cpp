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

