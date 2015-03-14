#ifndef GRAPHICSLINEITEM_H
#define GRAPHICSLINEITEM_H

#include "graphicsobject.h"

#include <QPen>
#include <QLineF>

class GraphicsLineItem : public GraphicsObject
{
    Q_OBJECT

public:
    GraphicsLineItem(GraphicsObject *parent = 0);

    QLineF line() const;
    void setLine(const QLineF &line);
    QPen pen() const;
    void setPen(const QPen &pen);

    // GraphicsObject interface
public:
    GraphicsObject *clone();
    void handleMoved(const GraphicsHandle *handle);

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // QGraphicsItem interface
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    mutable QPainterPath m_shape;
    mutable QRectF m_boundingRect;
    mutable bool m_dirty;
    void markDirty();
    void updateGeometry() const;

    QPen m_pen;
    QLineF m_line;
    const GraphicsHandle *m_handle1;
    const GraphicsHandle *m_handle2;
};

#endif // GRAPHICSLINEITEM_H
