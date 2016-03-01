#include "arctrackelement.h"

#include "tracknode.h"

ArcTrackElement::ArcTrackElement(TrackNode *n1, TrackNode *n2, QGraphicsItem *parent):
    TrackElement(parent), m_node1(n1), m_node2(n2)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    m_node1->addTrackElement(this);
    m_node2->addTrackElement(this);
    adjust();
}

TrackNode *ArcTrackElement::n1() const
{
    return m_node1;
}

TrackNode *ArcTrackElement::n2() const
{
    return m_node2;
}

void ArcTrackElement::adjust()
{
    if (!m_node1 || !m_node2)
        return;
}

QRectF ArcTrackElement::boundingRect() const
{
    if (!m_node1 || !m_node2)
        return QRectF();

    qreal extra = (width()) / 2.0;

    return QRectF();
}

QPainterPath ArcTrackElement::shape() const
{
    if (!m_node1 || !m_node2)
        return QPainterPath();

    QPainterPath path;
    //path.moveTo(m_line.p1());
    //path.lineTo(m_line.p2());
    QPainterPathStroker stroker;
    stroker.setWidth(width());
    stroker.setCapStyle(Qt::RoundCap);
    stroker.setJoinStyle(Qt::RoundJoin);
    return stroker.createStroke(path);
}

void ArcTrackElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

QVariant ArcTrackElement::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
    {
        break;
    }
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
