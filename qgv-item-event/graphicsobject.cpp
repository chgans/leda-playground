#include "graphicsobject.h"
#include "graphicshandle.h"

#include <QDebug>

GraphicsObject::GraphicsObject(GraphicsObject *parent):
    QGraphicsObject(parent)
{

}

GraphicsObject::~GraphicsObject()
{
}

int GraphicsObject::handleCount() const
{
    return childItems().count();
}


void GraphicsObject::cloneTo(GraphicsObject *dst)
{
    dst->setPos(pos());
    dst->setZValue(zValue());
    dst->setFlags(flags());
    dst->setSelected(isSelected());
#if 0
    foreach (GraphicsHandle *other, m_handles) {
        // FIXME: dst.moveHandle(idx, pos);
        //dst->addHandle(other->clone(dst));
        Q_UNUSED(other);
        Q_UNUSED(dst);
    }
#endif
}

GraphicsHandle *GraphicsObject::handleAt(int idx)
{
    Q_ASSERT(idx < childItems().count());
    return dynamic_cast<GraphicsHandle*>(childItems().at(idx));
}
