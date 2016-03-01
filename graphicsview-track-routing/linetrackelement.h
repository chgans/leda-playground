#ifndef LINETRACKELEMENT_H
#define LINETRACKELEMENT_H

#include "trackelement.h"
#include <QLineF>

class TrackNode;

class LineTrackElement : public TrackElement
{
public:
    LineTrackElement(TrackNode *n1, TrackNode *n2, QGraphicsItem *parent = nullptr);

    TrackNode *node1() const;
    TrackNode *node2() const;

    void adjust();

private:
    TrackNode *m_node1;
    TrackNode *m_node2;
    QLineF m_line;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // LINETRACKELEMENT_H
