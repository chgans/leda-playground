#include "graphicsiteminterface.h"
#include "graphicscontrolpoint.h"
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QPainterPathStroker>
#include <QPainter>

GraphicsControlPoint::GraphicsControlPoint(QGraphicsItem *parent):
    m_parent(parent)
{
    m_rect = QRectF(-2, -2, 2, 2);
    m_pos = QPointF(0, 0);
}



QRectF GraphicsControlPoint::boundingRect() const
{
    return m_rect.adjusted(-0.5, -0.5, +0.5, +0.5);
}

QPainterPath GraphicsControlPoint::shape() const
{
    QPainterPath path;
    QPainterPathStroker stroker;
    stroker.setWidth(0);
    path.addEllipse(m_rect);
    return path;
}

void GraphicsControlPoint::paint(QPainter *painter,
                                 const QStyleOptionGraphicsItem *option,
                                 QWidget *widget)
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
    dynamic_cast<GraphicsItemInterface *>(m_parent)->controlPointMoved(this);
}

void GraphicsControlPoint::setScenePos(const QPointF &pos)
{
    setPos(m_parent->mapFromScene(pos));
}

