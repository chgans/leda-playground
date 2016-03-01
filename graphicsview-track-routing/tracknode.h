#ifndef TRACKNODE_H
#define TRACKNODE_H

#include <QGraphicsItem>

class LineTrackElement;

class TrackNode : public QGraphicsItem
{
public:
    TrackNode(QGraphicsItem *parent = nullptr);

    QList<LineTrackElement *> tracks() const;
    void addTrack(LineTrackElement *track);

private:
    QList<LineTrackElement *> m_tracks;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // TRACKNODE_H
