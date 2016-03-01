#include "tracknode.h"
#include "linetrackelement.h"

#include <QPainter>
#include <QPainterPath>

TrackNode::TrackNode(QGraphicsItem *parent): QGraphicsItem(parent)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIgnoresTransformations);
    setFlag(ItemSendsGeometryChanges);
}

QList<LineTrackElement *> TrackNode::tracks() const
{
    return m_tracks;
}

void TrackNode::addTrack(LineTrackElement *track)
{
    m_tracks.append(track);
}


QRectF TrackNode::boundingRect() const
{
    return QRectF(-10, -10, 20, 20);
}

QPainterPath TrackNode::shape() const
{
    QPainterPath path;
    path.addEllipse(QPointF(0, 0), 10, 10);
    return path;
}

void TrackNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen());
    painter->setBrush(QBrush(Qt::lightGray));
    painter->drawEllipse(QPointF(0, 0), 10, 10);
}

QVariant TrackNode::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        for (LineTrackElement *track: m_tracks)
            track->adjust();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
