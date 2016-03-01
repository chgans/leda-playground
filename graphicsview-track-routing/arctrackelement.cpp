#include "arctrackelement.h"

#include "tracknode.h"

ArcTrackElement::ArcTrackElement(TrackNode *n1, TrackNode *n2, QGraphicsItem *parent):
    QGraphicsItem(parent), m_node1(n1), m_node2(n2)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    //m_node1->addTrack(this);
    //m_node2->addTrack(this);
    adjust();
}

qreal ArcTrackElement::width() const
{
    return m_width;
}

void ArcTrackElement::setWidth(qreal width)
{
    if (qFuzzyCompare(m_width, width))
        return;

    prepareGeometryChange();
    m_width = width;
}

qreal ArcTrackElement::clearance() const
{
    return m_clearance;
}

void ArcTrackElement::setClearance(qreal clearance)
{
    if (qFuzzyCompare(m_clearance, clearance))
        return;

    prepareGeometryChange();
    m_clearance = clearance;
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

    qreal extra = (m_width) / 2.0;

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
    stroker.setWidth(m_width);
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
