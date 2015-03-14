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

    void addPoint(const QPointF &pos);
    void removePoint(int index);
    QList<QPointF> points() const;
    int pointCount() const;

private:
    QPen m_pen;
    QLineF m_line;
    QPainterPath m_path;

    void setBoundingRectDirty();
    void computeBoundingRect() const;
    mutable bool m_boundingRectIsDirty;
    mutable QRectF m_boundingRect;

    void setShapeDirty();
    void computeShape() const;
    mutable bool m_shapeIsDirty;
    mutable QPainterPath m_shape;

    QMap<const GraphicsHandle *, int> m_handleToElementIndex;

    QPainterPath copyPath(const QPainterPath &src, int first, int last);
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
    virtual void handleMoved(const GraphicsHandle *point);

    // QGraphicsItem interface
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // GRAPHICSBEZIERITEM_H
