#include "node.h"
#include "track.h"

#include <QPainter>
#include <QPainterPath>

Node::Node(QGraphicsItem *parent): QGraphicsItem(parent)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIgnoresTransformations);
    setFlag(ItemSendsGeometryChanges);
}

QList<Track *> Node::tracks() const
{
    return m_tracks;
}

void Node::addTrack(Track *track)
{
    m_tracks.append(track);
}


QRectF Node::boundingRect() const
{
    return QRectF(-10, -10, 20, 20);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(QPointF(0, 0), 10, 10);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen());
    painter->setBrush(QBrush(Qt::lightGray));
    painter->drawEllipse(QPointF(0, 0), 10, 10);
}

QVariant Node::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        for (Track *track: m_tracks)
            track->adjust();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
