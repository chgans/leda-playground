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

void GraphicsBezierItem::addPoint(const QPointF &pos)
{
    if (m_path.elementCount() == 0) {
        const GraphicsControlPoint *node = addControlPoint(GraphicsControlPoint::MoveRole,
                                                           pos);
        m_nodeHandles.append(node);
        m_path.moveTo(node->pos());
        m_nodeHandles.append(node);
        m_path.moveTo(pos);
    }
    else {
        qDebug() << m_path.currentPosition() << pos;
        QPointF c1Pos = m_path.currentPosition() + QPointF (10, 10);
        QPointF c2Pos = pos + QPointF (10, 10);
        const GraphicsControlPoint *c1 = addControlPoint(GraphicsControlPoint::MoveRole,
                                                         c1Pos);
        const GraphicsControlPoint *c2 = addControlPoint(GraphicsControlPoint::MoveRole,
                                                         c2Pos);
        const GraphicsControlPoint *node = addControlPoint(GraphicsControlPoint::MoveRole,
                                                           pos);

        m_startHandles.append(c1);
        m_endHandles.append(c2);
        m_nodeHandles.append(node);
        m_path.cubicTo(c1->pos(), c2->pos(), node->pos());
    }
    qDebug() << m_path;
    markDirty();
}

void GraphicsBezierItem::removePoint(int index)
{
    QPainterPath path;
    markDirty();

}

QList<QPointF> GraphicsBezierItem::points() const
{
    QList<QPointF> result;
    QPainterPath::Element elt;
    int count = m_path.elementCount();
    int i = 0;
    while (i < count) {
        elt = m_path.elementAt(i);
        result.append(QPointF(elt.x, elt.y));
        if (i>1)
            i += 3;
        else
            i++;
    }
    return result;
}

int GraphicsBezierItem::pointCount() const
{
    if (m_path.elementCount() < 2)
        return m_path.elementCount();
    int count = (m_path.elementCount() - 1) / 3;
    qDebug() << "Count" << count;
    return count;
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
    if (isSelected()) {
        QPainterPath path;
        path = stroker.createStroke(m_path);
        m_shape = (path + controlPointsShape()).simplified();
    }
    else
        m_shape = stroker.createStroke(m_path);

    QRectF rect = m_shape.boundingRect();
    qreal extra = pen().widthF()/2.0;
    m_boundingRect = rect.adjusted(-extra, -extra, +extra, +extra);

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
    painter->drawPath(m_path);
    if (isSelected())
        paintControlPoints(painter, option, widget);
}

GraphicsObject *GraphicsBezierItem::clone()
{
    GraphicsBezierItem *item = new GraphicsBezierItem();
    GraphicsObject::cloneTo(item);
    item->setPen(pen());
    item->m_path = m_path;
    return item;
}

void GraphicsBezierItem::controlPointMoved(const GraphicsControlPoint *point)
{
#if 1
    // FIXME
    for (int i = 0; i < m_nodeHandles.count(); i++) {
        if (point == m_nodeHandles.value(i)) {
            m_path.setElementPositionAt(3*i, point->pos().x(), point->pos().y());
            markDirty();
            return;
        }
    }
    for (int i = 0; i < m_startHandles.count(); i++) {
        if (point == m_startHandles.value(i)) {
            m_path.setElementPositionAt(3*i+1, point->pos().x(), point->pos().y());
            markDirty();
            return;
        }
    }
    for (int i = 0; i < m_endHandles.count(); i++) {
        if (point == m_endHandles.value(i)) {
            m_path.setElementPositionAt(3*i+2, point->pos().x(), point->pos().y());
            markDirty();
            return;
        }
    }
#endif
}


QVariant GraphicsBezierItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    // Notify the scene that shape() and boudingRect() changed
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        markDirty();
    }
    return value;
}
