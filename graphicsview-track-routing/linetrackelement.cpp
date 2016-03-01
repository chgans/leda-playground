#include "linetrackelement.h"
#include "tracknode.h"

#include <QPainter>
#include <QPainterPath>
#include <QPainterPathStroker>


LineTrackElement::LineTrackElement(TrackNode *n1, TrackNode *n2, QGraphicsItem *parent): TrackElement(parent),
    m_node1(n1), m_node2(n2)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    m_node1->addTrackElement(this);
    m_node2->addTrackElement(this);
    adjust();
}

TrackNode *LineTrackElement::node1() const
{
    return m_node1;
}

TrackNode *LineTrackElement::node2() const
{
    return m_node2;
}

void LineTrackElement::adjust()
{
    if (!m_node1 || !m_node2)
        return;

    QLineF line(mapFromItem(m_node1, 0, 0), mapFromItem(m_node2, 0, 0));
    if (m_line != line)
    {
        prepareGeometryChange();
        m_line = line;
        setZValue(qMin(m_node1->zValue(), m_node2->zValue()) - 1.0);
    }
}

QRectF LineTrackElement::boundingRect() const
{
    if (!m_node1 || !m_node2)
        return QRectF();

    qreal extra = (width()) / 2.0;

    return QRectF(m_line.p1(), QSizeF(m_line.dx(), m_line.dy()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath LineTrackElement::shape() const
{
    if (!m_node1 || !m_node2)
        return QPainterPath();

    QPainterPath path;
    path.moveTo(m_line.p1());
    path.lineTo(m_line.p2());
    QPainterPathStroker stroker;
    stroker.setWidth(width());
    stroker.setCapStyle(Qt::RoundCap);
    stroker.setJoinStyle(Qt::RoundJoin);
    return stroker.createStroke(path);
}

void LineTrackElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!m_node1 || !m_node2)
        return;

    painter->setPen(QPen(Qt::red, width(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(m_line);
}


QVariant LineTrackElement::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
    {
        QPointF p1 = mapToScene(m_line.p1());
        QPointF p2 = mapToScene(m_line.p2());
        m_node1->setPos(p1);
        m_node2->setPos(p2);
        break;
    }
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
