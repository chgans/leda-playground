#ifndef TRACK_H
#define TRACK_H

#include <QGraphicsItem>
#include <QLineF>

class Node;

class Track : public QGraphicsItem
{
public:
    Track(Node *n1, Node *n2, QGraphicsItem *parent = nullptr);

    qreal width() const;
    void setWidth(qreal width);

    qreal clearance() const;
    void setClearance(qreal clearance);

    Node *p1() const;
    Node *p2() const;

    void adjust();

private:
    Node *m_p1;
    Node *m_p2;
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

#endif // TRACK_H
