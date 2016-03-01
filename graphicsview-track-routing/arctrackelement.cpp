#include "arctrackelement.h"
#include "tracknode.h"

#include <QPainter>
#include <QPainterPath>
#include <QPainterPathStroker>
#include <QtMath>

ArcTrackElement::ArcTrackElement(TrackNode *n1, TrackNode *n2, QGraphicsItem *parent):
    TrackElement(parent), m_node1(n1), m_node2(n2)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    m_node1->addTrackElement(this);
    m_node2->addTrackElement(this);

    m_radius = 100;
    m_startAngle = -90;
    m_sweepLength = 90;
    m_rect = QRectF(-m_radius, -m_radius, 2*m_radius, 2*m_radius);
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

    prepareGeometryChange();
    setZValue(qMin(m_node1->zValue(), m_node2->zValue()) - 1.0);
}

QRectF ArcTrackElement::boundingRect() const
{
    if (!m_node1 || !m_node2)
        return QRectF();

    qreal extra = (width()) / 2.0;

    return m_rect.adjusted(-extra, -extra, extra, extra);
}

QPainterPath ArcTrackElement::shape() const
{
    if (!m_node1 || !m_node2)
        return QPainterPath();

    QPainterPath path;
    path.arcTo(m_rect, m_startAngle, m_sweepLength);
    QPainterPathStroker stroker;
    stroker.setWidth(width());
    stroker.setCapStyle(Qt::RoundCap);
    stroker.setJoinStyle(Qt::RoundJoin);
    return stroker.createStroke(path);
}

void ArcTrackElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!m_node1 || !m_node2)
        return;

    painter->setPen(QPen(Qt::red, width(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawArc(m_rect, qFloor(m_startAngle*16), qFloor(m_sweepLength*16));
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
