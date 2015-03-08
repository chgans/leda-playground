#include "graphicsobject.h"
#include "graphicscontrolpoint.h"

GraphicsObject::GraphicsObject(QGraphicsItem *parent):
    QGraphicsObject(parent)
{

}

GraphicsObject::~GraphicsObject()
{

}

// TODO: const GraphicsControlPoint *addControlPoint(int id, const QPointF &pos);
const GraphicsControlPoint *GraphicsObject::addControlPoint(GraphicsControlPoint::Role role, const QPointF &pos)
{
    GraphicsControlPoint *point = new GraphicsControlPoint(role, pos);
    m_controlPoints.append(point);
    return point;
}

// TODO: QList<const GraphicsControlPoint *> GraphicsObject::controlPoints() const
QVector<const GraphicsControlPoint *> GraphicsObject::controlPoints() const
{
    QVector<const GraphicsControlPoint *> result;
    foreach (GraphicsControlPoint *point, m_controlPoints) {
        result << const_cast<GraphicsControlPoint *>(point);
    }
    return result;
}

// TODO: remove
/*
void GraphicsObject::setControlPoints(const QVector<GraphicsControlPoint *> points)
{
    qDeleteAll(m_controlPoints);
    m_controlPoints = points;
}
*/

// TODO: private + GraphicsTool as friend?
// Pos is in item coordinate
// TODO:
const GraphicsControlPoint *GraphicsObject::controlPointNear(const QPointF &pos)
{
    for (int i = 0; i < m_controlPoints.count(); i++) {
        if (m_controlPoints[i]->shape().contains(pos)) {
            return m_controlPoints[i];
        }
    }
    return nullptr;
}

void GraphicsObject::moveControlPoint(const GraphicsControlPoint *point, const QPointF &pos)
{
    moveControlPointSilently(point, pos);
    controlPointMoved(point);
}


void GraphicsObject::moveControlPointSilently(const GraphicsControlPoint *point, const QPointF &pos)
{
    GraphicsControlPoint *p = const_cast<GraphicsControlPoint *>(point);
    Q_ASSERT(m_controlPoints.contains(p));
    p->setPos(pos);
}

void GraphicsObject::cloneTo(GraphicsObject *dst)
{
    dst->setPos(pos());
    dst->setFlags(flags());
    dst->setSelected(isSelected());
    foreach (GraphicsControlPoint *other, m_controlPoints) {
        // FIXME: dst.moveControlPoint(idx, pos);
        //dst->addControlPoint(other->clone(dst));
    }
}

void GraphicsObject::paintControlPoints(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) const
{
    foreach (const GraphicsControlPoint *point, m_controlPoints) {
        point->paint(painter, option, widget);
    }
}

#if 0
QVector<GraphicsControlPoint *> GraphicsObject::cloneControlPoints(const GraphicsObject *parent) const
{
    QVector<GraphicsControlPoint *> result;
    foreach (GraphicsControlPoint *point, m_controlPoints) {
        result.append(point->clone(nullptr));
    }
    return result;
}
#endif

QRectF GraphicsObject::controlPointsBoundingRect() const
{
    QRectF boundingRect;
    foreach (const GraphicsControlPoint *point, m_controlPoints) {
        boundingRect |= point->boundingRect();
    }
    return boundingRect;
}

QPainterPath GraphicsObject::controlPointsShape() const
{
    QPainterPath path;
    foreach (const GraphicsControlPoint *point, m_controlPoints) {
        path.addPath(point->shape());
    }
    return path;
}


