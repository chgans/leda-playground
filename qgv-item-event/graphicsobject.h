#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <QGraphicsObject>
#include <QPointF>
#include <QRectF>
#include <QPainterPath>
#include <QVector>

class GraphicsControlPoint;
class QGraphicsItem;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

// TODO: add properties

class GraphicsObject: public QGraphicsObject
{
    Q_OBJECT
public:
    GraphicsObject(QGraphicsItem *parent = 0);
    virtual ~GraphicsObject();

    virtual GraphicsObject *clone() = 0;

    // TODO: add radius
    int controlPointNear(const QPointF &pos);

    void moveControlPoint(int idx, const QPointF &pos);

protected:
    void cloneTo(GraphicsObject *dst);

    void addControlPoint(GraphicsControlPoint *point);
    QVector<GraphicsControlPoint *> controlPoints() const;
    //void setControlPoints(const QVector<GraphicsControlPoint*> points);
    void moveControlPointSilently(int idx, const QPointF &pos);
    virtual void paintControlPoints(QPainter *painter,
                                    const QStyleOptionGraphicsItem *option,
                                    QWidget *widget) const;
    //QVector<GraphicsControlPoint *> cloneControlPoints(const GraphicsObject *parent) const;
    QRectF controlPointsBoundingRect() const;
    QPainterPath controlPointsShape() const;

    virtual void controlPointMoved(GraphicsControlPoint *point) = 0;

private:
    QVector<GraphicsControlPoint *> m_controlPoints;
};

#endif // GRAPHICSOBJECT_H
