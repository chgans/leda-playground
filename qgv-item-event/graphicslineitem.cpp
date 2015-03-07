#include "graphicslineitem.h"
#include "graphicscontrolpoint.h"

#include <QPainter>
#include <QPen>

#include <QDebug>

GraphicsLineItem::GraphicsLineItem(QGraphicsItem *parent):
    GraphicsObject(parent),
    m_ctlPoint1(new GraphicsControlPoint(this)),
    m_ctlPoint2(new GraphicsControlPoint(this))
{
    addControlPoint(m_ctlPoint1);
    addControlPoint(m_ctlPoint2);
    //setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

QLineF GraphicsLineItem::line() const
{
    return m_line;
}

void GraphicsLineItem::setLine(const QLineF &line)
{
    prepareGeometryChange();
    m_line = line;
    qDebug() << m_line;
}

QPen GraphicsLineItem::pen() const
{
    return m_pen;
}

void GraphicsLineItem::setPen(const QPen &pen)
{
    prepareGeometryChange();
    m_pen = pen;
}

GraphicsObject *GraphicsLineItem::clone()
{
    GraphicsLineItem *item = new GraphicsLineItem();
    GraphicsObject::cloneTo(item);
    item->setPen(m_pen);
    item->setLine(line());
    return item;
}

void GraphicsLineItem::controlPointMoved(GraphicsControlPoint *point)
{
    if (point == m_ctlPoint1) {
        prepareGeometryChange();
        m_line.setP1(point->pos());
    }
    else {
        prepareGeometryChange();
        m_line.setP2(point->pos());
    }
}

QRectF GraphicsLineItem::boundingRect() const
{
    return shape().boundingRect();
}

QPainterPath GraphicsLineItem::shape() const
{
    QPainterPath path;
    QPainterPathStroker stroker;
    stroker.setWidth(m_pen.widthF());
    stroker.setCapStyle(m_pen.capStyle());
    stroker.setJoinStyle(m_pen.joinStyle());
    stroker.setMiterLimit(m_pen.miterLimit());
    path.moveTo(m_line.p1());
    path.lineTo(m_line.p2());
    path = stroker.createStroke(path);
    if (!isSelected())
        path |= controlPointsShape();
    return path;
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
        prepareGeometryChange();
    return value;
}
