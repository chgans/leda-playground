#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>

class Track;

class Node : public QGraphicsItem
{
public:
    Node(QGraphicsItem *parent = nullptr);

    QList<Track *> tracks() const;
    void addTrack(Track *track);

private:
    QList<Track *> m_tracks;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // NODE_H
