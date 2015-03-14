#include "graphicsobject.h"
#include "graphicshandle.h"

#include <QStyleOptionGraphicsItem>
#include <QPainterPathStroker>
#include <QPainter>

#include <QDebug>


// TODO: dynamic size and customisable colors

GraphicsHandle::GraphicsHandle(GraphicsHandle::Role role, const QPointF &pos):
    m_role(role), m_rect(QRectF(-5.0, -5.0, 5.0, 5.0))
{
    setPos(pos);
}

/* FIXME:
 *  - links
 *  - data
 */
GraphicsHandle::GraphicsHandle(const GraphicsHandle &other):
    m_role(other.m_role), m_rect(other.m_rect)
{

}

// TBD: avoid duplicate?
void GraphicsHandle::addLink(GraphicsHandle *other)
{
    m_links.append(other);
}

void GraphicsHandle::removeLink(GraphicsHandle *other)
{
    int idx = m_links.indexOf(other);
    if (idx > 0)
        m_links.takeAt(idx);
}

QList<GraphicsHandle *> GraphicsHandle::links() const
{
    return m_links;
}

QCursor GraphicsHandle::cursor() const
{
    return roleToCursor(m_role);
}

QVariant GraphicsHandle::data() const
{
    return m_data;
}

void GraphicsHandle::setData(const QVariant &data)
{
    m_data = data;
}


QRectF GraphicsHandle::boundingRect() const
{
    return shape().boundingRect().adjusted(-1.0, -1.0, 1.0, 1.0);
}

QPainterPath GraphicsHandle::shape() const
{
    QPainterPath path;
    path.addEllipse(m_rect);
    return path;
}

void GraphicsHandle::paint(QPainter *painter,
                                 const QStyleOptionGraphicsItem *option,
                                 QWidget *widget) const
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(QPen(QBrush(Qt::darkYellow), 0));
    painter->setBrush(QBrush(Qt::darkRed));
    painter->drawEllipse(m_rect);
}

QPointF GraphicsHandle::pos() const
{
    return m_rect.center();
}

void GraphicsHandle::setPos(const QPointF &pos)
{
    m_rect.moveCenter(pos);
}

QCursor GraphicsHandle::roleToCursor(GraphicsHandle::Role role) const
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


