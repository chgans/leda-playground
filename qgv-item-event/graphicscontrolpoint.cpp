#include "graphicsobject.h"
#include "graphicscontrolpoint.h"

#include <QStyleOptionGraphicsItem>
#include <QPainterPathStroker>
#include <QPainter>

#include <QDebug>

GraphicsControlPoint::GraphicsControlPoint(GraphicsObject *parent, const QPointF &pos):
    m_parent(parent), m_pos(pos)
{
    m_rect = QRectF(-2, -2, 2, 2);
}



QRectF GraphicsControlPoint::boundingRect() const
{
    return shape().boundingRect();
}

QPainterPath GraphicsControlPoint::shape() const
{
    QPainterPath path;
    QPainterPathStroker stroker;
    stroker.setWidth(0);
    path.addEllipse(m_rect);
    //return stroker.createStroke(path);
    return path;
}

void GraphicsControlPoint::paint(QPainter *painter,
                                 const QStyleOptionGraphicsItem *option,
                                 QWidget *widget) const
{
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
    //m_parent->controlPointMoved(this);
    qDebug() << this << pos;
}

#if 0
void GraphicsControlPoint::setScenePos(const QPointF &pos)
{
    setPos(m_parent->mapFromScene(pos));
}
#endif

GraphicsControlPoint *GraphicsControlPoint::clone(GraphicsObject *parent)
{
    GraphicsControlPoint *other = new GraphicsControlPoint(parent);
    other->m_pos = m_pos;
    other->m_rect = m_rect;
}

