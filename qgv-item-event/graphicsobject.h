#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <QGraphicsObject>
#include <QPointF>
#include <QRectF>
#include <QPainterPath>
#include <QVector>

#include "graphicshandle.h"

class QGraphicsItem;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

// TODO: add properties
// TODO: AbstractPath and AbstractShape (allow to morph between AbstractXYZ impl)

class GraphicsObject: public QGraphicsObject
{
    Q_OBJECT
public:
    GraphicsObject(GraphicsObject *parent = 0);
    virtual ~GraphicsObject();

    virtual GraphicsObject *clone() = 0;

    // TODO: add radius
    const GraphicsHandle *HandleNear(const QPointF &pos) const;
    QVector<const GraphicsHandle *> handles() const;
    void moveHandle(const GraphicsHandle *handle, const QPointF &pos);

protected:
    void cloneTo(GraphicsObject *dst);

    const GraphicsHandle *addHandle(GraphicsHandle::Role role, const QPointF &pos);
    const GraphicsHandle *removeHandle(int index);


    //void setHandles(const QVector<GraphicsHandle*> handles);
    void moveHandleSilently(const GraphicsHandle *handle, const QPointF &pos);
    virtual void paintHandles(QPainter *painter,
                                    const QStyleOptionGraphicsItem *option,
                                    QWidget *widget) const;
    //QVector<GraphicsHandle *> cloneHandles(const GraphicsObject *parent) const;
    QRectF handlesBoundingRect() const;
    QPainterPath handlesShape() const;

    virtual void handleMoved(const GraphicsHandle *handle) = 0;

private:
    QVector<GraphicsHandle *> m_handles;
    mutable bool m_handlesDirty;
    mutable QRectF m_handlesBoundingRect;
    mutable QPainterPath m_handlesPath;
    void updateHandlesGeometry() const;
    void markHandlesDirty();
};

#endif // GRAPHICSOBJECT_H
