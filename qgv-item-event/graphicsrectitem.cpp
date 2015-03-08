#include "graphicsrectitem.h"
#include "graphicscontrolpoint.h"

#include <QBrush>
#include <QPen>
#include <QRectF>
#include <QPainter>

// TODO: forbid objects to have write access to control points

GraphicsRectItem::GraphicsRectItem(QGraphicsItem *parent):
    GraphicsObject(parent), m_rect(QRectF(0, 0, 0, 0))
{
    addControlPoint(TopLeft, GraphicsControlPoint::FDiagSizeRole);
    addControlPoint(Top, GraphicsControlPoint::VSizeRole);
    addControlPoint(TopRight, GraphicsControlPoint::BDiagSizeRole);
    addControlPoint(Right, GraphicsControlPoint::HSizeRole);
    addControlPoint(BottomRight, GraphicsControlPoint::FDiagSizeRole);
    addControlPoint(Bottom, GraphicsControlPoint::VSizeRole);
    addControlPoint(BottomLeft, GraphicsControlPoint::BDiagSizeRole);
    addControlPoint(Left, GraphicsControlPoint::HSizeRole);
}

GraphicsRectItem::~GraphicsRectItem()
{

}

QRectF GraphicsRectItem::rect() const
{
    return m_rect;
}

void GraphicsRectItem::setRect(const QRectF &rect)
{
    prepareGeometryChange();
    m_rect = rect;
}

QPen GraphicsRectItem::pen() const
{
    return m_pen;
}

void GraphicsRectItem::setPen(const QPen &pen)
{
    prepareGeometryChange();
    m_pen = pen;
}

QBrush GraphicsRectItem::brush() const
{
    return m_brush;
}

void GraphicsRectItem::setBrush(const QBrush &brush)
{
    prepareGeometryChange(); // needed?
    m_brush = brush;
}

void GraphicsRectItem::addControlPoint(GraphicsRectItem::CtlPointId pointId, GraphicsControlPoint::Role role)
{
    const GraphicsControlPoint *point = GraphicsObject::addControlPoint(role,
                                                                    QPointF(0, 0));
    m_ctlPointToId[point] = pointId;
    m_idToCtlPoint[pointId] = point;

}

void GraphicsRectItem::updateControlPointsSilently()
{
    m_rect = m_rect.normalized();
    qreal midX = m_rect.right()-m_rect.width()/2.0;
    qreal midY = m_rect.bottom()-m_rect.height()/2.0;

    moveControlPointSilently(m_idToCtlPoint[TopLeft], m_rect.topLeft());
    moveControlPointSilently(m_idToCtlPoint[Top], QPointF(midX, m_rect.top()));
    moveControlPointSilently(m_idToCtlPoint[TopRight], m_rect.topRight());
    moveControlPointSilently(m_idToCtlPoint[Right], QPointF(m_rect.right(), midY));
    moveControlPointSilently(m_idToCtlPoint[BottomRight], m_rect.bottomRight());
    moveControlPointSilently(m_idToCtlPoint[Bottom], QPointF(midX, m_rect.bottom()));
    moveControlPointSilently(m_idToCtlPoint[BottomLeft], m_rect.bottomLeft());
    moveControlPointSilently(m_idToCtlPoint[Left], QPointF(m_rect.left(), midY));
}

GraphicsObject *GraphicsRectItem::clone()
{
    GraphicsRectItem *item = new GraphicsRectItem();
    GraphicsObject::cloneTo(item);
    item->setBrush(brush());
    item->setPen(pen());
    item->setRect(rect());
    return item;
}

void GraphicsRectItem::controlPointMoved(const GraphicsControlPoint *point)
{
    Q_ASSERT(m_ctlPointToId.contains(point));
    CtlPointId id = m_ctlPointToId[point];
    prepareGeometryChange();
    switch (id) {
    case TopLeft:
        m_rect.setTopLeft(point->pos());
        break;
    case Top:
        m_rect.setTop(point->pos().y());
        break;
    case TopRight:
        m_rect.setTopRight(point->pos());
        break;
    case Right:
        m_rect.setRight(point->pos().x());
        break;
    case BottomRight:
        m_rect.setBottomRight(point->pos());
        break;
    case Bottom:
        m_rect.setBottom(point->pos().y());
        break;
    case BottomLeft:
        m_rect.setBottomLeft(point->pos());
        break;
    case Left:
        m_rect.setLeft(point->pos().x());
        break;
    default:
        break;
    }

    updateControlPointsSilently();
}

QRectF GraphicsRectItem::boundingRect() const
{
    QRectF rect = shape().boundingRect();
    if (isSelected())
        rect |= controlPointsBoundingRect();
    return rect.adjusted(-5, -5, +5, +5);

}

QPainterPath GraphicsRectItem::shape() const
{
    QPainterPath path;
    path.addRect(m_rect);
    if (isSelected())
       path |= controlPointsShape();
    return path;

}

void GraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawRect(m_rect);
    if (isSelected())
        paintControlPoints(painter, option, widget);
}

QVariant GraphicsRectItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    // Notify the scene that shape() and boudingRect() changed
    if (change == QGraphicsItem::ItemSelectedHasChanged)
        prepareGeometryChange();
    return value;
}
