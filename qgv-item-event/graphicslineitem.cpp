#include "graphicslineitem.h"
#include "graphicscontrolpoint.h"
#include <QGraphicsEllipseItem>
#include <QPainter>
#include <QPen>

GraphicsLineItem::GraphicsLineItem(QGraphicsItem *parent):
    QGraphicsLineItem(parent),
    m_ctlPoint1(new GraphicsControlPoint(this)),
    m_ctlPoint2(new GraphicsControlPoint(this))
{
    addControlPoint(m_ctlPoint1);
    addControlPoint(m_ctlPoint2);
}

QGraphicsItem *GraphicsLineItem::clone()
{
    GraphicsLineItem *item = new GraphicsLineItem();
    item->setPos(pos());
    item->setPen(pen());
    //item->setLine(line());
    item->setFlags(flags());
    item->setSelected(isSelected());
    item->m_ctlPoint1->setPos(m_ctlPoint1->pos());
    item->m_ctlPoint2->setPos(m_ctlPoint2->pos());
    item->controlPointMoved(item->m_ctlPoint1);
    item->controlPointMoved(item->m_ctlPoint2);
    return item;
}

void GraphicsLineItem::controlPointMoved(const GraphicsControlPoint *point)
{
    if (point == m_ctlPoint1) {
        QLineF lineF = line();
        lineF.setP1(point->pos());
        setLine(lineF);
    }
    else {
        QLineF lineF = line();
        lineF.setP2(point->pos());
        setLine(lineF);
    }
}



QRectF GraphicsLineItem::boundingRect() const
{
    if (!isSelected())
        return QGraphicsLineItem::boundingRect();
    return QGraphicsLineItem::boundingRect() | controlPointsBoundingRect();
}

QPainterPath GraphicsLineItem::shape() const
{
    if (!isSelected())
        return QGraphicsLineItem::shape();
    QPainterPath path = controlPointsShape();
    path.addPath(QGraphicsLineItem::shape());
    return path;
}

void GraphicsLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //QGraphicsLineItem::paint(painter, option, widget);
    painter->setPen(pen());
    painter->drawLine(line());
    if (isSelected())
        paintControlPoints(painter, option, widget);
}


QVariant GraphicsLineItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    // Notify the scene that shape() and boudingRect() changed
    if (change == QGraphicsItem::ItemSelectedHasChanged)
        prepareGeometryChange();
    return value;
}
