#ifndef LINETRACKELEMENT_H
#define LINETRACKELEMENT_H

#include <QGraphicsItem>
#include <QLineF>

class TrackNode;

class LineTrackElement : public QGraphicsItem
{
public:
    LineTrackElement(TrackNode *n1, TrackNode *n2, QGraphicsItem *parent = nullptr);

    qreal width() const;
    void setWidth(qreal width);

    qreal clearance() const;
    void setClearance(qreal clearance);

    TrackNode *p1() const;
    TrackNode *p2() const;

    void adjust();

private:
    TrackNode *m_p1;
    TrackNode *m_p2;
    qreal m_width;
    qreal m_clearance;
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
