#ifndef GRAPHICSITEMINTERFACE_H
#define GRAPHICSITEMINTERFACE_H

#include <QPointF>
#include <QRectF>
#include <QPainterPath>
#include <QList>

class GraphicsControlPoint;
class QGraphicsItem;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

class GraphicsItemInterface
{
public:
    GraphicsItemInterface();
    virtual ~GraphicsItemInterface();

    virtual QGraphicsItem *clone() = 0;

    void addControlPoint(GraphicsControlPoint *point);
    QList<GraphicsControlPoint *> controlPoints() const;
    void setControlPoints(const QList<GraphicsControlPoint*> points);
    GraphicsControlPoint *controlPointAt(const QPointF &pos);

    virtual void paintControlPoints(QPainter *painter,
                                    const QStyleOptionGraphicsItem *option,
                                    QWidget *widget);
    QRectF controlPointsBoundingRect() const;
    QPainterPath controlPointsShape() const;

    virtual void controlPointMoved(const GraphicsControlPoint *point) = 0;

private:
    QList<GraphicsControlPoint *> m_controlPoints;
};

#endif // GRAPHICSITEMINTERFACE_H
