#ifndef ARCTRACKELEMENT_H
#define ARCTRACKELEMENT_H

#include "trackelement.h"

class TrackNode;

class ArcTrackElement : public TrackElement
{
public:
    ArcTrackElement(TrackNode *n1, TrackNode *n2, QGraphicsItem *parent = nullptr);

    TrackNode *n1() const;
    TrackNode *n2() const;

    void adjust();

private:
    TrackNode *m_node1;
    TrackNode *m_node2;
    qreal m_radius;
    QRectF m_rect;
    qreal m_startAngle;
    qreal m_sweepLength;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // ARCTRACKELEMENT_H
