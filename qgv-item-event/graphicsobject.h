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
    int handleCount() const;
    GraphicsHandle *handleAt(int idx);

protected:
    void cloneTo(GraphicsObject *dst);
    //virtual void handleMoved(const GraphicsHandle *handle) = 0;
    //friend class GraphicsHandle;
};

#endif // GRAPHICSOBJECT_H
