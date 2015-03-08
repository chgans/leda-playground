#include "graphicsobject.h"
#include "graphicscontrolpoint.h"

#include <QStyleOptionGraphicsItem>
#include <QPainterPathStroker>
#include <QPainter>

#include <QDebug>


// TODO: dynamic size and customisable colors

GraphicsControlPoint::GraphicsControlPoint(GraphicsControlPoint::Role role, const QPointF &pos):
    m_role(role), m_pos(pos), m_rect(QRectF(-2, -2, 2, 2))
{

}

/* FIXME:
 *  - links
 *  - data
 */
GraphicsControlPoint::GraphicsControlPoint(const GraphicsControlPoint &other):
    m_role(other.m_role), m_pos(other.m_pos), m_rect(other.m_rect)
{

}

// TBD: avoid duplicate?
void GraphicsControlPoint::addLink(GraphicsControlPoint *other)
{
    m_links.append(other);
}

void GraphicsControlPoint::removeLink(GraphicsControlPoint *other)
{
    int idx = m_links.indexOf(other);
    if (idx > 0)
        m_links.takeAt(idx);
}

QList<GraphicsControlPoint *> GraphicsControlPoint::links() const
{
    return m_links;
}

QCursor GraphicsControlPoint::cursor() const
{
    return roleToCursor(m_role);
}

QVariant GraphicsControlPoint::data() const
{
    return m_data;
}

void GraphicsControlPoint::setData(const QVariant &data)
{
    m_data = data;
}


QRectF GraphicsControlPoint::boundingRect() const
{
    return shape().boundingRect();
}

QPainterPath GraphicsControlPoint::shape() const
{
    QPainterPath path;
    path.addEllipse(m_rect); // FIXME: add half pen size
    return path;
}

void GraphicsControlPoint::paint(QPainter *painter,
                                 const QStyleOptionGraphicsItem *option,
                                 QWidget *widget) const
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(QPen(QBrush(Qt::white), 0));
    painter->setBrush(QBrush(Qt::red));
    painter->drawEllipse(m_rect);
}

QPointF GraphicsControlPoint::pos() const
{
    return m_rect.center();
}

void GraphicsControlPoint::setPos(const QPointF &pos)
{
    m_rect.moveCenter(pos);
    qDebug() << this << pos;
}

QCursor GraphicsControlPoint::roleToCursor(GraphicsControlPoint::Role role) const
{
    switch (role)
    {
    case MoveRole: return QCursor(Qt::PointingHandCursor);
    case VSizeRole: return QCursor(Qt::SizeVerCursor);
    case HSizeRole: return QCursor(Qt::SizeHorCursor);
    case BDiagSizeRole: return QCursor(Qt::SizeBDiagCursor);
    case FDiagSizeRole: return QCursor(Qt::SizeFDiagCursor);
    case RotateRole: return QCursor(); // TBD
    case ShearRole: return QCursor();  // TBD
    case MarkRole: return QCursor();   // TBD
    default: return QCursor();
    }
}

#if 0
void GraphicsControlPoint::setScenePos(const QPointF &pos)
{
    setPos(m_parent->mapFromScene(pos));
}

GraphicsControlPoint *GraphicsControlPoint::clone(GraphicsObject *parent)
{
    GraphicsControlPoint *other = new GraphicsControlPoint(parent);
    other->m_pos = m_pos;
    other->m_rect = m_rect;
}
#endif

