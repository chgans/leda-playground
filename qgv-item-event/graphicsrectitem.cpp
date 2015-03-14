#include "graphicsrectitem.h"
#include "graphicshandle.h"

#include <QBrush>
#include <QPen>
#include <QRectF>
#include <QPainter>

#include <QDebug>

// TODO: forbid objects to have write access to handles

GraphicsRectItem::GraphicsRectItem(GraphicsObject *parent):
    GraphicsObject(parent), m_rect(QRectF(0, 0, 0, 0)),
    m_dirty(true)
{
    addHandle(TopLeft, GraphicsHandle::FDiagSizeRole);
    addHandle(Top, GraphicsHandle::VSizeRole);
    addHandle(TopRight, GraphicsHandle::BDiagSizeRole);
    addHandle(Right, GraphicsHandle::HSizeRole);
    addHandle(BottomRight, GraphicsHandle::FDiagSizeRole);
    addHandle(Bottom, GraphicsHandle::VSizeRole);
    addHandle(BottomLeft, GraphicsHandle::BDiagSizeRole);
    addHandle(Left, GraphicsHandle::HSizeRole);
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
    m_rect = rect;
    markDirty();
}

QPen GraphicsRectItem::pen() const
{
    return m_pen;
}

void GraphicsRectItem::setPen(const QPen &pen)
{
    m_pen = pen;
    markDirty();
}

QBrush GraphicsRectItem::brush() const
{
    return m_brush;
}

void GraphicsRectItem::setBrush(const QBrush &brush)
{
    m_brush = brush;
    markDirty(); // needed?
}

void GraphicsRectItem::addHandle(GraphicsRectItem::HandleId handleId, GraphicsHandle::Role role)
{
    const GraphicsHandle *handle = GraphicsObject::addHandle(role,
                                                                    QPointF(0, 0));
    m_handleToId[handle] = handleId;
    m_idToHandle[handleId] = handle;
}

void GraphicsRectItem::updateHandlesSilently()
{
    qreal midX = m_rect.right()-m_rect.width()/2.0;
    qreal midY = m_rect.bottom()-m_rect.height()/2.0;

    moveHandleSilently(m_idToHandle[TopLeft], m_rect.topLeft());
    moveHandleSilently(m_idToHandle[Top], QPointF(midX, m_rect.top()));
    moveHandleSilently(m_idToHandle[TopRight], m_rect.topRight());
    moveHandleSilently(m_idToHandle[Right], QPointF(m_rect.right(), midY));
    moveHandleSilently(m_idToHandle[BottomRight], m_rect.bottomRight());
    moveHandleSilently(m_idToHandle[Bottom], QPointF(midX, m_rect.bottom()));
    moveHandleSilently(m_idToHandle[BottomLeft], m_rect.bottomLeft());
    moveHandleSilently(m_idToHandle[Left], QPointF(m_rect.left(), midY));
}

void GraphicsRectItem::updateGeometry() const
{

    QPainterPath path;
    path.addRect(m_rect);
    if (isSelected()) {
        m_shape = (path + handlesShape()).simplified();
    }
    else
        m_shape = path;

    QRectF rect = m_shape.boundingRect();
    if (isSelected())
        rect |= handlesBoundingRect();

    qreal extra = pen().widthF()/2.0;
    m_boundingRect = rect.adjusted(-extra, -extra, +extra, +extra);

    m_dirty = false;
}

void GraphicsRectItem::markDirty()
{
    prepareGeometryChange();
    m_dirty = true;
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

void GraphicsRectItem::handleMoved(const GraphicsHandle *handle)
{
    Q_ASSERT(m_handleToId.contains(handle));
    HandleId id = m_handleToId[handle];
    switch (id) {
    case TopLeft:
        m_rect.setTopLeft(handle->pos());
        break;
    case Top:
        m_rect.setTop(handle->pos().y());
        break;
    case TopRight:
        m_rect.setTopRight(handle->pos());
        break;
    case Right:
        m_rect.setRight(handle->pos().x());
        break;
    case BottomRight:
        m_rect.setBottomRight(handle->pos());
        break;
    case Bottom:
        m_rect.setBottom(handle->pos().y());
        break;
    case BottomLeft:
        m_rect.setBottomLeft(handle->pos());
        break;
    case Left:
        m_rect.setLeft(handle->pos().x());
        break;
    default:
        break;
    }
    updateHandlesSilently();
    markDirty();
}

QRectF GraphicsRectItem::boundingRect() const
{
    if (m_dirty)
        updateGeometry();
    return m_boundingRect;
}

QPainterPath GraphicsRectItem::shape() const
{
    if (m_dirty)
        updateGeometry();
    return m_shape;
}

void GraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawRect(m_rect);
    if (isSelected())
        paintHandles(painter, option, widget);
}

QVariant GraphicsRectItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    // Notify the scene that shape() and boudingRect() changed
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        markDirty();
    }
    return value;
}
