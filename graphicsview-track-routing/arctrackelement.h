#ifndef ARCTRACKELEMENT_H
#define ARCTRACKELEMENT_H

#include <QGraphicsItem>

class TrackNode;

class ArcTrackElement : public QGraphicsItem
{
public:
    ArcTrackElement(TrackNode *n1, TrackNode *n2, QGraphicsItem *parent = nullptr);

    qreal width() const;
    void setWidth(qreal width);

    qreal clearance() const;
    void setClearance(qreal clearance);

    TrackNode *n1() const;
    TrackNode *n2() const;

    void adjust();

private:
    TrackNode *m_node1;
    TrackNode *m_node2;
    qreal m_width;
    qreal m_clearance;
    qreal m_radius;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // ARCTRACKELEMENT_H
