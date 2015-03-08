#include "graphicsbezieritem.h"
#include "graphicsscene.h"
#include "graphicsview.h"
#include "graphicscontrolpoint.h"

#include <QPainter>

#include <QDebug>

GraphicsBezierItem::GraphicsBezierItem(GraphicsObject *parent):
    GraphicsObject(parent)
{

}

QPainterPath GraphicsBezierItem::path() const
{
    return m_path;
}

void GraphicsBezierItem::setPath(const QPainterPath &path)
{
    m_path = path;
    markDirty();
    qDebug() << __PRETTY_FUNCTION__ << path;
}

QPen GraphicsBezierItem::pen() const
{
    return m_pen;
}

void GraphicsBezierItem::setPen(const QPen &pen)
{
    m_pen = pen;
    markDirty();
}

void GraphicsBezierItem::markDirty()
{
    prepareGeometryChange();
    m_dirty = true;
}

void GraphicsBezierItem::updateGeometry() const
{
    QPainterPathStroker stroker;
    stroker.setWidth(m_pen.widthF());
    stroker.setCapStyle(m_pen.capStyle());
    stroker.setJoinStyle(m_pen.joinStyle());
    stroker.setMiterLimit(m_pen.miterLimit());
    m_shape = stroker.createStroke(m_path);
    if (isSelected())
        m_shape |= controlPointsShape();

    QRectF rect = m_shape.boundingRect();
    if (isSelected())
        rect |= controlPointsBoundingRect();
    m_boundingRect = rect.adjusted(-5, -5, +5, +5);

    m_dirty = false;
}

QRectF GraphicsBezierItem::boundingRect() const
{
    if (m_dirty)
        updateGeometry();
    return m_boundingRect;
}

QPainterPath GraphicsBezierItem::shape() const
{
    if (m_dirty)
        updateGeometry();
    return m_shape;
}

void GraphicsBezierItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen());
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path());
    if (isSelected())
        paintControlPoints(painter, option, widget);
}

GraphicsObject *GraphicsBezierItem::clone()
{
}

void GraphicsBezierItem::controlPointMoved(const GraphicsControlPoint *point)
{
}


QVariant GraphicsBezierItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    // Notify the scene that shape() and boudingRect() changed
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        markDirty();
    }
    return value;
}
