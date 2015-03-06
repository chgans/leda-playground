#include "graphicsiteminterface.h"
#include "graphicscontrolpoint.h"

GraphicsItemInterface::GraphicsItemInterface()
{

}

GraphicsItemInterface::~GraphicsItemInterface()
{

}

void GraphicsItemInterface::addControlPoint(GraphicsControlPoint *point)
{
    m_controlPoints.append(point);
}

QList<GraphicsControlPoint *> GraphicsItemInterface::controlPoints() const
{
    return m_controlPoints;
}

void GraphicsItemInterface::setControlPoints(const QList<GraphicsControlPoint *> points)
{
    qDeleteAll(m_controlPoints);
    m_controlPoints = points;
}

// Pos is in item coordinate
GraphicsControlPoint *GraphicsItemInterface::controlPointAt(const QPointF &pos)
{
    foreach (GraphicsControlPoint *point, controlPoints()) {
        if (point->shape().contains(pos)) {
            return point;
        }
    }
    return nullptr;
}

void GraphicsItemInterface::paintControlPoints(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    foreach (GraphicsControlPoint *point, m_controlPoints) {
        point->paint(painter, option, widget);
    }
}

QRectF GraphicsItemInterface::controlPointsBoundingRect() const
{
    QRectF boundingRect;
    foreach (GraphicsControlPoint *point, m_controlPoints) {
        boundingRect |= point->boundingRect();
    }
    return boundingRect;
}

QPainterPath GraphicsItemInterface::controlPointsShape() const
{
    QPainterPath path;
    foreach (GraphicsControlPoint *point, m_controlPoints) {
        path.addPath(point->shape());
    }
    return path;
}

