#include "graphicsobject.h"
#include "graphicshandle.h"

#include <QDebug>

GraphicsObject::GraphicsObject(GraphicsObject *parent):
    QGraphicsObject(parent),
    m_handlesDirty(true)
{

}

GraphicsObject::~GraphicsObject()
{
    qDeleteAll(m_handles);
}

// TODO: const GraphicsHandle *addHandle(int id, const QPointF &pos);
const GraphicsHandle *GraphicsObject::addHandle(GraphicsHandle::Role role, const QPointF &pos)
{
    //qDebug() << "Handles" << Handles().count() << "=>" << Handles().count() + 1;
    GraphicsHandle *handle = new GraphicsHandle(role, pos);
    m_handles.append(handle);
    markHandlesDirty();
    return handle;
}

const GraphicsHandle *GraphicsObject::removeHandle(int index)
{
    Q_ASSERT(index < m_handles.count());
    const GraphicsHandle *handle = m_handles.value(index);
    m_handles.removeAt(index);
    delete handle;
    return handle;
}

// TODO: QList<const GraphicsHandle *> GraphicsObject::Handles() const
QVector<const GraphicsHandle *> GraphicsObject::handles() const
{
    QVector<const GraphicsHandle *> result;
    foreach (GraphicsHandle *handle, m_handles) {
        result << const_cast<GraphicsHandle *>(handle);
    }
    return result;
}

// TODO: remove
/*
void GraphicsObject::setHandles(const QVector<GraphicsHandle *> handles)
{
    qDeleteAll(m_Handles);
    m_Handles = handles;
}
*/

// TODO: private + GraphicsTool as friend?
// Pos is in item coordinate
// TODO:
const GraphicsHandle *GraphicsObject::HandleNear(const QPointF &pos) const
{
    for (int i = 0; i < m_handles.count(); i++) {
        if (m_handles[i]->shape().contains(pos)) {
            return m_handles[i];
        }
    }
    return nullptr;
}

void GraphicsObject::moveHandle(const GraphicsHandle *handle, const QPointF &pos)
{
    moveHandleSilently(handle, pos);
    handleMoved(handle);
}


void GraphicsObject::moveHandleSilently(const GraphicsHandle *handle, const QPointF &pos)
{
    markHandlesDirty();
    GraphicsHandle *p = const_cast<GraphicsHandle *>(handle);
    Q_ASSERT(m_handles.contains(p));
    p->setPos(pos);
}

void GraphicsObject::cloneTo(GraphicsObject *dst)
{
    dst->setPos(pos());
    dst->setZValue(zValue());
    dst->setFlags(flags());
    dst->setSelected(isSelected());
    foreach (GraphicsHandle *other, m_handles) {
        // FIXME: dst.moveHandle(idx, pos);
        //dst->addHandle(other->clone(dst));
        Q_UNUSED(other);
        Q_UNUSED(dst);
    }
}

void GraphicsObject::paintHandles(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) const
{
    foreach (const GraphicsHandle *handle, m_handles) {
        handle->paint(painter, option, widget);
    }
}

#if 0
QVector<GraphicsHandle *> GraphicsObject::cloneHandles(const GraphicsObject *parent) const
{
    QVector<GraphicsHandle *> result;
    foreach (GraphicsHandle *handle, m_handles) {
        result.append(handle->clone(nullptr));
    }
    return result;
}
#endif

QRectF GraphicsObject::handlesBoundingRect() const
{
    if (m_handlesDirty)
        updateHandlesGeometry();
    return m_handlesBoundingRect;
}

QPainterPath GraphicsObject::handlesShape() const
{
    if (m_handlesDirty)
        updateHandlesGeometry();
    return m_handlesPath;
}

void GraphicsObject::updateHandlesGeometry() const
{
    m_handlesBoundingRect = QRectF();
    foreach (const GraphicsHandle *handle, m_handles) {
        m_handlesBoundingRect |= handle->boundingRect();
    }
    m_handlesPath = QPainterPath();
    foreach (const GraphicsHandle *handle, m_handles) {
        m_handlesPath |= handle->shape();
    }
    m_handlesDirty = false;
}

void GraphicsObject::markHandlesDirty()
{
    prepareGeometryChange();
    m_handlesDirty = true;
}


