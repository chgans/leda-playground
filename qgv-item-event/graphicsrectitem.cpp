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
    m_updatingHandles(false), m_dirty(true)
{
    addHandle(TopLeft, FDiagSizeHandleRole);
    addHandle(Top, VSizeHandleRole);
    addHandle(TopRight, BDiagSizeHandleRole);
    addHandle(Right, HSizeHandleRole);
    addHandle(BottomRight, FDiagSizeHandleRole);
    addHandle(Bottom, VSizeHandleRole);
    addHandle(BottomLeft, BDiagSizeHandleRole);
    addHandle(Left, HSizeHandleRole);
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
}

void GraphicsRectItem::addHandle(GraphicsRectItem::HandleId handleId, GraphicsHandleRole role)
{
    GraphicsHandle *handle = new GraphicsHandle(this);
    handle->setRole(role);
    handle->setHandleShape(CircularHandleShape);
    handle->setPos(QPointF(0, 0));
    addObservedItem(handle);
    m_handleToId[handle] = handleId;
    m_idToHandle[handleId] = handle;
}

void GraphicsRectItem::updateHandlesSilently()
{
    m_updatingHandles = true;

    qreal midX = m_rect.right()-m_rect.width()/2.0;
    qreal midY = m_rect.bottom()-m_rect.height()/2.0;

    m_idToHandle[TopLeft]->setPos(m_rect.topLeft());
    m_idToHandle[Top]->setPos(QPointF(midX, m_rect.top()));
    m_idToHandle[TopRight]->setPos(m_rect.topRight());
    m_idToHandle[Right]->setPos(QPointF(m_rect.right(), midY));
    m_idToHandle[BottomRight]->setPos(m_rect.bottomRight());
    m_idToHandle[Bottom]->setPos(QPointF(midX, m_rect.bottom()));
    m_idToHandle[BottomLeft]->setPos(m_rect.bottomLeft());
    m_idToHandle[Left]->setPos(QPointF(m_rect.left(), midY));

    m_updatingHandles = false;
}

void GraphicsRectItem::updateGeometry() const
{

    QPainterPath path;
    path.addRect(m_rect);
    m_shape = path;

    QRectF rect = m_shape.boundingRect();
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

void GraphicsRectItem::itemNotification(IGraphicsObservableItem *item)
{
    //Q_ASSERT(m_handleToId.contains(handle));
    if (m_updatingHandles)
        return;

    GraphicsHandle *handle = dynamic_cast<GraphicsHandle*>(item);
    Q_ASSERT(handle);

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
}

// TODO: refactor
QVariant GraphicsRectItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        foreach (QGraphicsItem *h, childItems()) {
            h->setVisible(isSelected());
        }
    }
    return value;
}
