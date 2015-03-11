#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <QGraphicsObject>
#include <QPointF>
#include <QRectF>
#include <QPainterPath>
#include <QVector>

#include "graphicscontrolpoint.h"

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
    const GraphicsControlPoint *controlPointNear(const QPointF &pos) const;
    QVector<const GraphicsControlPoint *> controlPoints() const;
    void moveControlPoint(const GraphicsControlPoint *point, const QPointF &pos);

protected:
    void cloneTo(GraphicsObject *dst);

    const GraphicsControlPoint *addControlPoint(GraphicsControlPoint::Role role, const QPointF &pos);
    const GraphicsControlPoint *removeControlPoint(int index);


    //void setControlPoints(const QVector<GraphicsControlPoint*> points);
    void moveControlPointSilently(const GraphicsControlPoint *point, const QPointF &pos);
    virtual void paintControlPoints(QPainter *painter,
                                    const QStyleOptionGraphicsItem *option,
                                    QWidget *widget) const;
    //QVector<GraphicsControlPoint *> cloneControlPoints(const GraphicsObject *parent) const;
    QRectF controlPointsBoundingRect() const;
    QPainterPath controlPointsShape() const;

    virtual void controlPointMoved(const GraphicsControlPoint *point) = 0;

private:
    QVector<GraphicsControlPoint *> m_controlPoints;
    mutable bool m_controlPointsDirty;
    mutable QRectF m_controlPointsBoundingRect;
    mutable QPainterPath m_controlPointsPath;
    void updateControlPointsGeometry() const;
    void markControlPointsDirty();
};

#endif // GRAPHICSOBJECT_H
