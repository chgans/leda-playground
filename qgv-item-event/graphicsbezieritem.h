#ifndef GRAPHICSBEZIERITEM_H
#define GRAPHICSBEZIERITEM_H

#include "graphicsobject.h"
#include "igraphicsitemobserver.h"
#include "graphicspathpoint.h"

#include <QPen>


class GraphicsBezierItem : public GraphicsObject, public IGraphicsItemObserver
{
public:
    GraphicsBezierItem(GraphicsObject *parent = 0);

    QPainterPath path() const;
    void setPath(const QPainterPath &path);

    QPen pen() const;
    void setPen(const QPen &pen);

    void addPoint(const QPointF &pos);
    void removePoint(int index);
    QList<GraphicsPathPoint *> points() const;
    int pointCount() const;
    GraphicsPathPoint *pointAt(int idx);

private:
    QPen m_pen;
    QLineF m_line;
    QPainterPath m_path;

    bool m_updatingHandles;

    void setBoundingRectDirty();
    void computeBoundingRect() const;
    mutable bool m_boundingRectIsDirty;
    mutable QRectF m_boundingRect;

    void setShapeDirty();
    void computeShape() const;
    mutable bool m_shapeIsDirty;
    mutable QPainterPath m_shape;

    QPainterPath copyPath(const QPainterPath &src, int first, int last);

    QVector<qreal> m_px;
    QVector<qreal> m_py;
    QVector<qreal> m_c1x;
    QVector<qreal> m_c1y;
    QVector<qreal> m_c2x;
    QVector<qreal> m_c2y;
    void updateSpline();
    void computeControlPoints(const QVector<qreal> &p, QVector<qreal> &c1, QVector<qreal> &c2);
    void updateHandles();


    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // GraphicsObject interface
public:
    virtual GraphicsObject *clone();

    // IGraphicsItemObserver interface
protected:
    virtual void itemNotification(IGraphicsObservableItem *item);

    // QGraphicsItem interface
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // GRAPHICSBEZIERITEM_H
