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
void GraphicsObject::addControlPoint(GraphicsControlPoint *point)
{
    m_controlPoints.append(point);
}

// TODO: QList<const GraphicsControlPoint *> GraphicsObject::controlPoints() const
QVector<GraphicsControlPoint *> GraphicsObject::controlPoints() const
{
    return m_controlPoints;
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
int GraphicsObject::controlPointNear(const QPointF &pos)
{
    for (int i = 0; i < m_controlPoints.count(); i++) {
        if (m_controlPoints[i]->shape().contains(pos)) {
            return i;
        }
    }
    return -1;
}

void GraphicsObject::moveControlPoint(int idx, const QPointF &pos)
{
    moveControlPointSilently(idx, pos);
    controlPointMoved(m_controlPoints[idx]);
}


void GraphicsObject::moveControlPointSilently(int idx, const QPointF &pos)
{
    Q_ASSERT(idx >=0 && idx < m_controlPoints.count());
    m_controlPoints.value(idx)->setPos(pos);
}

void GraphicsObject::cloneTo(GraphicsObject *dst)
{
    dst->setPos(pos());
    dst->setFlags(flags());
    dst->setSelected(isSelected());
    foreach (GraphicsControlPoint *other, controlPoints()) {
        // FIXME: dst.moveControlPoint(idx, pos);
        dst->addControlPoint(other->clone(dst));
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


