#ifndef TRACKNODE_H
#define TRACKNODE_H

#include <QGraphicsItem>

class TrackElement;

class TrackNode : public QGraphicsItem
{
public:
    TrackNode(QGraphicsItem *parent = nullptr);

    QList<TrackElement *> trackElements() const;
    void addTrackElement(TrackElement *track);

private:
    QList<TrackElement *> m_trackElements;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // TRACKNODE_H
