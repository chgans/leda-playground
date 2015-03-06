#ifndef GRAPHICSCONTROLPOINT_H
#define GRAPHICSCONTROLPOINT_H

#include <QRectF>
#include <QPainterPath>

class QGraphicsItem;
class QStyleOptionGraphicsItem;
class QPainter;
class QWidget;

class GraphicsControlPoint
{
public:
    GraphicsControlPoint(QGraphicsItem *parent);

public:
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPointF pos() const;
    void setPos(const QPointF &pos);
    void setScenePos(const QPointF &pos);

private:
    QGraphicsItem *m_parent;
    QRectF m_rect;
    QPointF m_pos;
};

#endif // GRAPHICSCONTROLPOINT_H
