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
    Q_ASSERT(m_path.elementCount() == controlPoints().count());

    if (m_path.elementCount() == 0) {
        const GraphicsControlPoint *node = addControlPoint(GraphicsControlPoint::MoveRole,
                                                           pos);
        m_nodeHandles.append(node);
        m_path.moveTo(node->pos());
    }
    else if (m_path.currentPosition() != pos) {
        QPointF c1Pos = m_path.currentPosition();// + QPointF (1, 1);
        QPointF c2Pos = pos;// + QPointF (1, 1);
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
    else {
        qWarning() << QString("Cannot add a cubic bezier to pos [%1, %2] which is equal to current pos")
                      .arg(pos.x()).arg(pos.y());
    }
    qDebug() << m_path << controlPoints().count();
    markDirty();
}

void GraphicsBezierItem::removePoint(int index)
{
    int nbElt = m_path.elementCount();
    int nbPoints = pointCount();
    int eltIndex = index*3;

    Q_ASSERT(nbElt > 0);
    Q_ASSERT(index < nbPoints);
    Q_ASSERT(m_path.elementCount() == controlPoints().count());

    QPainterPath path;
    if (index == 0) {
        if (m_path.elementCount() > 1) {
            path.moveTo(m_path.elementAt(3).x, m_path.elementAt(3).y);
        }
        m_path = path;
        removeControlPoint(0);
    }
    else if (index == nbPoints - 1) {
        m_path = copyPath(m_path, 0, index - 1);
        removeControlPoint(3*index);
        removeControlPoint(3*index - 1);
        removeControlPoint(3*index - 2);
    }
    qDebug() << m_path;
    markDirty();
}

QPainterPath GraphicsBezierItem::copyPath(const QPainterPath &src, int first, int last)
{
    QPainterPath dst;
    int i = first;
    while (i <= (last*3)) {
        if (i == 0) {
            QPainterPath::Element elt = src.elementAt(i);
            dst.moveTo(elt.x, elt.y);
            i++;
        }
        else {
            QPainterPath::Element c1 = src.elementAt(i);
            QPainterPath::Element c2 = src.elementAt(i+1);
            QPainterPath::Element pt = src.elementAt(i+2);
            dst.cubicTo(c1.x, c1.y, c2.x, c2.y, pt.x, pt.y);
            i+= 3;
        }
    }
    qDebug() << dst;
    return dst;
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
        if (i > 0)
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
    int count = 1 + (m_path.elementCount() - 1) / 3;
    //qDebug() << "Count" << count;
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

// TODO:
// - when moving a node, move ctl points as well
// - ctl points can be symetric, move one -> move the other
// - use moveControlPoint(), beware of infinite recursion
void GraphicsBezierItem::controlPointMoved(const GraphicsControlPoint *point)
{
    Q_ASSERT(m_path.elementCount() == controlPoints().count());

#if 1
    // FIXME
    for (int i = 0; i < m_nodeHandles.count(); i++) {
        if (point == m_nodeHandles.value(i)) {
            m_path.setElementPositionAt(3*i, point->pos().x(), point->pos().y());
            qDebug() << point << 3*i << point->pos();
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
