#include "graphicslineitem.h"
#include "graphicscontrolpoint.h"

#include <QPainter>
#include <QPen>

#include <QDebug>

GraphicsLineItem::GraphicsLineItem(GraphicsObject *parent):
    GraphicsObject(parent), m_dirty(true)
{
    m_ctlPoint1 = addControlPoint(GraphicsControlPoint::MoveRole, QPointF(0, 0));
    m_ctlPoint2 = addControlPoint(GraphicsControlPoint::MoveRole, QPointF(0, 0));
}

QLineF GraphicsLineItem::line() const
{
    return m_line;
}

void GraphicsLineItem::setLine(const QLineF &line)
{
    m_line = line;
    markDirty();
}

QPen GraphicsLineItem::pen() const
{
    return m_pen;
}

void GraphicsLineItem::setPen(const QPen &pen)
{
    m_pen = pen;
    markDirty();
}

GraphicsObject *GraphicsLineItem::clone()
{
    GraphicsLineItem *item = new GraphicsLineItem();
    GraphicsObject::cloneTo(item);
    item->setPen(m_pen);
    item->setLine(line());
    return item;
}

void GraphicsLineItem::controlPointMoved(const GraphicsControlPoint *point)
{
    markDirty();
    if (point == m_ctlPoint1) {
        m_line.setP1(point->pos());
    }
    else {
        m_line.setP2(point->pos());
    }
}

QRectF GraphicsLineItem::boundingRect() const
{
    if (m_dirty)
        updateGeometry();
    return m_boundingRect;
}

QPainterPath GraphicsLineItem::shape() const
{
    if (m_dirty)
        updateGeometry();
    return m_shape;
}

void GraphicsLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(m_pen);
    painter->drawLine(line());
    if (isSelected())
        paintControlPoints(painter, option, widget);
}

// FIXME: optimisation, make sure shape() and boudingRect() don't change
// when seletion changed?
QVariant GraphicsLineItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    // Notify the scene that shape() and boudingRect() changed
    if (change == QGraphicsItem::ItemSelectedHasChanged)
        markDirty();
    return value;
}

void GraphicsLineItem::markDirty()
{
    prepareGeometryChange();
    m_dirty = true;
}

void GraphicsLineItem::updateGeometry() const
{
    QPainterPath path;
    QPainterPathStroker stroker;
    stroker.setWidth(m_pen.widthF());
    stroker.setCapStyle(m_pen.capStyle());
    stroker.setJoinStyle(m_pen.joinStyle());
    stroker.setMiterLimit(m_pen.miterLimit());

    path.moveTo(m_line.p1());
    path.lineTo(m_line.p2());

    if (isSelected()) {
        path = stroker.createStroke(path);
        m_shape = (path + controlPointsShape()).simplified();
    }
    else
        m_shape = stroker.createStroke(path);

    QRectF rect = m_shape.boundingRect();
    qreal extra = pen().widthF()/2.0;
    m_boundingRect = rect.adjusted(-extra, -extra, +extra, +extra);

    m_dirty = false;
}
