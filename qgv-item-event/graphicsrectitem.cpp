#include "graphicsrectitem.h"
#include "graphicscontrolpoint.h"

#include <QBrush>
#include <QPen>
#include <QRectF>
#include <QPainter>

enum ControlPointRole {
    TopLeft = 0,
    Top,
    TopRight,
    Right,
    BottomRight,
    Bottom,
    BottomLeft,
    Left,
    NbControlPointRoles
};

// TODO: forbid objects to have write access to control points

GraphicsRectItem::GraphicsRectItem(QGraphicsItem *parent):
    GraphicsObject(parent)
{
    m_rect = QRectF(0, 0, 0, 0);
    addControlPoint(new GraphicsControlPoint(this, QPointF(0, 0)));
    addControlPoint(new GraphicsControlPoint(this, QPointF(0, 0)));
    addControlPoint(new GraphicsControlPoint(this, QPointF(0, 0)));
    addControlPoint(new GraphicsControlPoint(this, QPointF(0, 0)));
    addControlPoint(new GraphicsControlPoint(this, QPointF(0, 0)));
    addControlPoint(new GraphicsControlPoint(this, QPointF(0, 0)));
    addControlPoint(new GraphicsControlPoint(this, QPointF(0, 0)));
    addControlPoint(new GraphicsControlPoint(this, QPointF(0, 0)));
    Q_ASSERT(controlPoints().count() == NbControlPointRoles);
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

void GraphicsRectItem::updateControlPointsSilently()
{
    m_rect = m_rect.normalized();
    qreal midX = m_rect.right()-m_rect.width()/2.0;
    qreal midY = m_rect.bottom()-m_rect.height()/2.0;
    moveControlPointSilently(TopLeft, m_rect.topLeft());
    moveControlPointSilently(Top, QPointF(midX, m_rect.top()));
    moveControlPointSilently(TopRight, m_rect.topRight());
    moveControlPointSilently(Right, QPointF(m_rect.right(), midY));
    moveControlPointSilently(BottomRight, m_rect.bottomRight());
    moveControlPointSilently(Bottom, QPointF(midX, m_rect.bottom()));
    moveControlPointSilently(BottomLeft, m_rect.bottomLeft());
    moveControlPointSilently(Left, QPointF(m_rect.left(), midY));
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

void GraphicsRectItem::controlPointMoved(GraphicsControlPoint *point)
{
    ControlPointRole pos = ControlPointRole(controlPoints().indexOf(point));
    Q_ASSERT (pos >= 0 && pos < NbControlPointRoles);
    prepareGeometryChange();
    switch (pos) {
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
