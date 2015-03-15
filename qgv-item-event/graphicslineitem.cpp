#include "graphicslineitem.h"
#include "graphicshandle.h"

#include <QPainter>
#include <QPen>

#include <QDebug>

GraphicsLineItem::GraphicsLineItem(GraphicsObject *parent):
    GraphicsObject(parent), m_dirty(true)
{
    // TODO: same way of doing as bezier item
    // Add handles in mouserelease/move
    m_handle1 = new GraphicsHandle(this);
    m_handle1->setRole(MoveHandleRole);
    m_handle1->setHandleShape(CircularHandleShape);
    m_handle1->setPos(QPointF(0, 0));
    addObservedItem(m_handle1);

    m_handle2 = new GraphicsHandle(this);
    m_handle2->setRole(MoveHandleRole);
    m_handle2->setHandleShape(CircularHandleShape);
    m_handle2->setPos(QPointF(0, 0));
    addObservedItem(m_handle2);
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

void GraphicsLineItem::itemNotification(IGraphicsObservableItem *item)
{
    GraphicsHandle *handle = dynamic_cast<GraphicsHandle*>(item);
    Q_ASSERT(handle);

    markDirty();
    if (handle == m_handle1) {
        m_line.setP1(handle->pos());
    }
    else {
        m_line.setP2(handle->pos());
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
}

QVariant GraphicsLineItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        foreach (QGraphicsItem *h, childItems()) {
            h->setVisible(isSelected());
        }
    }
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

    m_shape = stroker.createStroke(path);

    QRectF rect = m_shape.boundingRect();
    qreal extra = pen().widthF()/2.0;
    m_boundingRect = rect.adjusted(-extra, -extra, +extra, +extra);

    m_dirty = false;
}
