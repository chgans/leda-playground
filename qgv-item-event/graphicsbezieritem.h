#ifndef GRAPHICSBEZIERITEM_H
#define GRAPHICSBEZIERITEM_H

#include "graphicsobject.h"

#include <QPen>

class GraphicsBezierItem : public GraphicsObject
{
public:
    GraphicsBezierItem(GraphicsObject *parent = 0);

    QPainterPath path() const;
    void setPath(const QPainterPath &path);
    QPen pen() const;
    void setPen(const QPen &pen);

private:
    QPen m_pen;
    QLineF m_line;
    QPainterPath m_path;
    mutable QRectF m_boundingRect;
    mutable QPainterPath m_shape;
    mutable bool m_dirty;
    void markDirty();
    void updateGeometry() const;

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // GraphicsObject interface
public:
    virtual GraphicsObject *clone();

    // GraphicsObject interface
protected:
    virtual void controlPointMoved(const GraphicsControlPoint *point);

    // QGraphicsItem interface
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // GRAPHICSBEZIERITEM_H
