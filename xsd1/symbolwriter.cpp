#include "symbolwriter.h"

#include "symbol.h"

#include <QFile>
#include <QXmlStreamWriter>
#include <QGraphicsItem>
#include <QPen>
#include <QBrush>

struct SymbolWriterPrivate
{
    // Polymorphics
    void writeGraphicsItem(QGraphicsItem *item);
    void writeGraphicsItemList(const char *listTag, QList<QGraphicsItem *> items);

    // Items
    void writeEllipse(QGraphicsItem *item);
    void writeLine(QGraphicsItem *item);
    void writePin(QGraphicsItem *item);
    void writeRect(QGraphicsItem *item);
    void writeGroup(QGraphicsItem *item);
    void writeArc(QGraphicsItem *item);
    void writeLabel(QGraphicsItem *item);

    // Abstract items
    void writeShape(QAbstractGraphicsShapeItem *item);
    void writeItem(QGraphicsItem *item);

    // Complex types
    void writePen(const char *tag, const QPen &pen);
    void writeBrush(const char *tag, const QBrush &brush);
    void writePos(const char *tag, const QPointF &pos);
    void writePosList(const char *listTag, const char *posTag, const QList<QPointF> &posList);

    // Simple types
    void writeColor(const char *tag, const QColor &color);
    void writePenStyle(const char *tag, Qt::PenStyle style);
    void writePenCapStyle(const char *tag, Qt::PenCapStyle style);
    void writePenJoinStyle(const char *tag, Qt::PenJoinStyle style);
    void writeBrushStyle(const char *tag, Qt::BrushStyle style);

    // Built in types
    void writeBoolean(const char *tag, bool value);
    void writeInt(const char *tag, int value);
    void writeDouble(const char *tag, qreal value);

    QXmlStreamWriter *writer;
};

SymbolWriter::SymbolWriter()
{
    p->writer = new QXmlStreamWriter();
}

bool SymbolWriter::write(const QString &filename, const Symbol *symbol)
{
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QIODevice::Truncate)) {
        m_errorString = QString("\"%1\": %2").arg(filename).arg(file.errorString());
        return false;
    }
    p->writer->setDevice(&file);
    p->writer->setAutoFormatting(true);
    p->writer->writeStartDocument("1.0");
    p->writer->writeDefaultNamespace("http://www.leda.org/xsd");
    p->writer->writeStartElement("symbol");
    p->writer->writeTextElement("name", symbol->caption);
    p->writer->writeTextElement("label", symbol->description);
    p->writeGraphicsItemList("drawing", symbol->drawings);
    p->writer->writeEndElement(); // symbol
    p->writer->writeEndDocument();
    return true;
}



void SymbolWriterPrivate::writeGraphicsItem(QGraphicsItem *item)
{
    QString type = item->data(0).toString();
    writer->writeStartElement(type);
    if (type == "ellipse") {
        writeEllipse(item);
    }
    else if (type == "line") {
        writeLine(item);
    }
    else if (type == "pin") {
        writePin(item);
    }
    else if (type == "rectangle") {
        writeRect(item);
    }
    else if (type == "group") {
        writeGroup(item);
    }
    else if (type == "arc") {
        writeArc(item);
    }
    else if (type == "label") {
        writeLabel(item);
    }
    writer->writeEndElement();
}

void SymbolWriterPrivate::writeGraphicsItemList(const char *listTag, QList<QGraphicsItem *> items)
{
    writer->writeStartElement(listTag);
    foreach (QGraphicsItem *item, items) {
       writeGraphicsItem(item);
    }
    writer->writeEndElement();
}

void SymbolWriterPrivate::writeEllipse(QGraphicsItem *item)
{
    QGraphicsEllipseItem *ellipse = static_cast<QGraphicsEllipseItem*>(item);
    writeShape(ellipse);
    writePos("center", ellipse->rect().center());
    writeDouble("x-radius", ellipse->rect().width());
    writeDouble("y-radius", ellipse->rect().height());
    writeDouble("start-angle", ellipse->startAngle());
    writeDouble("span-angle", ellipse->spanAngle());
}

void SymbolWriterPrivate::writeLine(QGraphicsItem *item)
{
    QGraphicsLineItem *line = static_cast<QGraphicsLineItem*>(item);
    writeItem(line);
    writer->writeStartElement("points");
    writePos("point", line->line().p1());
    writePos("point", line->line().p2());
    writer->writeEndElement();
}

// FIXME
void SymbolWriterPrivate::writePin(QGraphicsItem *item)
{
    QGraphicsEllipseItem *pin = static_cast<QGraphicsEllipseItem*>(item);
    writeItem(pin);
    //writer->writeTextElement("designator", "0");
}

void SymbolWriterPrivate::writeRect(QGraphicsItem *item)
{
    QGraphicsRectItem *rect = static_cast<QGraphicsRectItem*>(item);
    writeShape(rect);
    writePos("top-left", rect->rect().topLeft());
    writePos("bottom-right", rect->rect().bottomRight());
}

void SymbolWriterPrivate::writeGroup(QGraphicsItem *item)
{
    QGraphicsItemGroup *group = static_cast<QGraphicsItemGroup*>(item);
    writeItem(group);
    writeGraphicsItemList("children", group->childItems());
}

void SymbolWriterPrivate::writeArc(QGraphicsItem *item)
{
    QGraphicsEllipseItem *ellipse = static_cast<QGraphicsEllipseItem*>(item);
    writeShape(ellipse);
}

void SymbolWriterPrivate::writeLabel(QGraphicsItem *item)
{
    QGraphicsEllipseItem *ellipse = static_cast<QGraphicsEllipseItem*>(item);
    writeShape(ellipse);
}

void SymbolWriterPrivate::writeShape(QAbstractGraphicsShapeItem *item)
{
    writeItem(item);
    writePen("pen", item->pen());
    writeBrush("brush", item->brush());
}

// FIXME
void SymbolWriterPrivate::writeItem(QGraphicsItem *item)
{
    writePos("position", item->pos());
    writeDouble("z-value", item->zValue());
    writeDouble("rotation", item->rotation()); // % 360
    writeDouble("opacity", item->opacity());
    writeBoolean("locked", !item->isEnabled());
    // writeMirroring("mirrored", item.transform());
    writeBoolean("visible", item->isVisible());
}

void SymbolWriterPrivate::writePen(const char *tag, const QPen &pen)
{
    writer->writeStartElement(tag);
    writeDouble("width", pen.width());
    writeColor("color", pen.color());
    writePenStyle("style", pen.style());
    writePenCapStyle("cap-style", pen.capStyle());
    writePenJoinStyle("join-style", pen.joinStyle());
    writer->writeEndElement();
}

void SymbolWriterPrivate::writeBrush(const char *tag, const QBrush &brush)
{
    writer->writeStartElement(tag);
    writeColor("color", brush.color());
    writeBrushStyle("style", brush.style());
    writer->writeEndElement();
}

void SymbolWriterPrivate::writePos(const char *tag, const QPointF &pos)
{
    writer->writeStartElement(tag);
    writeDouble("x", pos.x());
    writeDouble("y", pos.y());
    writer->writeEndElement();
}

void SymbolWriterPrivate::writePosList(const char *listTag, const char *posTag, const QList<QPointF> &posList)
{
    writer->writeStartElement(listTag);
    foreach (const QPointF &pos, posList)
        writePos(posTag, pos);
    writer->writeEndElement();
}

void SymbolWriterPrivate::writeColor(const char *tag, const QColor &color)
{
    writer->writeTextElement(tag, color.name());
}

void SymbolWriterPrivate::writePenStyle(const char *tag, Qt::PenStyle style)
{
    QString str;
    switch (style) {
    case Qt::DashLine: str = "DashLine"; break;
    case Qt::DotLine: str = "DotLine"; break;
    case Qt::DashDotLine: str = "DashDotLine"; break;
    case Qt::DashDotDotLine: str = "DashDotDotLine"; break;
    case Qt::NoPen: str = "NoPen"; break;
    default: str = "SolidLine";
    }
    writer->writeTextElement(tag, str);
}

void SymbolWriterPrivate::writePenCapStyle(const char *tag, Qt::PenCapStyle style)
{
    QString str;
    switch (style) {
    case Qt::FlatCap: str = "Flat"; break;
    case Qt::SquareCap: str = "Square"; break;
    default: str = "Round";
    }
    writer->writeTextElement(tag, str);
}

void SymbolWriterPrivate::writePenJoinStyle(const char *tag, Qt::PenJoinStyle style)
{
    QString str;
    switch (style) {
    case Qt::MiterJoin: str = "Miter"; break;
    case Qt::BevelJoin: str = "Bevel"; break;
    default: str = "Round";
    }
    writer->writeTextElement(tag, str);
}

void SymbolWriterPrivate::writeBrushStyle(const char *tag, Qt::BrushStyle style)
{
    QString str;
    switch (style) {
    case Qt::SolidPattern: str = "Solid"; break;
    case Qt::HorPattern: str = "Horizontal"; break;
    case Qt::VerPattern: str = "Vertical"; break;
    case Qt::CrossPattern: str = "Cross"; break;
    case Qt::BDiagPattern: str = "BDiagonal"; break;
    case Qt::FDiagPattern: str = "FDiagonal"; break;
    case Qt::DiagCrossPattern: str = "CrossDiagonal"; break;
    default: str = "NoBrush";
    }
    writer->writeTextElement(tag, str);
}

void SymbolWriterPrivate::writeBoolean(const char *tag, bool value)
{
    writer->writeTextElement(tag, value ? "true" : "false");
}

void SymbolWriterPrivate::writeInt(const char *tag, int value)
{
    writer->writeTextElement(tag, QString("%1").arg(value));
}

void SymbolWriterPrivate::writeDouble(const char *tag, qreal value)
{
    writer->writeTextElement(tag, QString("%1").arg(value, 0, 'E', 6));
}
