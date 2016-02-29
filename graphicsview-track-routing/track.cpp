#include "track.h"
#include "node.h"

#include <QPainter>
#include <QPainterPath>
#include <QPainterPathStroker>


Track::Track(Node *n1, Node *n2, QGraphicsItem *parent): QGraphicsItem(parent),
    m_p1(n1), m_p2(n2), m_width(50), m_clearance(10)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    m_p1->addTrack(this);
    m_p2->addTrack(this);
    adjust();
}

qreal Track::width() const
{
    return m_width;
}

void Track::setWidth(qreal width)
{
    if (qFuzzyCompare(m_width, width))
        return;

    prepareGeometryChange();
    m_width = width;
}

qreal Track::clearance() const
{
    return m_clearance;
}

void Track::setClearance(qreal clearance)
{
    if (qFuzzyCompare(m_clearance, clearance))
        return;

    prepareGeometryChange();
    m_clearance = clearance;
}

Node *Track::p1() const
{
    return m_p1;
}

Node *Track::p2() const
{
    return m_p2;
}

void Track::adjust()
{
    if (!m_p1 || !m_p2)
        return;

    QLineF line(mapFromItem(m_p1, 0, 0), mapFromItem(m_p2, 0, 0));
    if (m_line != line)
    {
        prepareGeometryChange();
        m_line = line;
        setZValue(qMin(m_p1->zValue(), m_p2->zValue()) - 1.0);
    }
}


QRectF Track::boundingRect() const
{
    if (!m_p1 || !m_p2)
        return QRectF();

    qreal extra = (m_width) / 2.0;

    return QRectF(m_line.p1(), QSizeF(m_line.dx(), m_line.dy()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath Track::shape() const
{
    if (!m_p1 || !m_p2)
        return QPainterPath();

    QPainterPath path;
    path.moveTo(m_line.p1());
    path.lineTo(m_line.p2());
    QPainterPathStroker stroker;
    stroker.setWidth(m_width);
    stroker.setCapStyle(Qt::RoundCap);
    stroker.setJoinStyle(Qt::RoundJoin);
    return stroker.createStroke(path);
}

void Track::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!m_p1 || !m_p2)
        return;

    painter->setPen(QPen(Qt::red, m_width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(m_line);
}


QVariant Track::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
    {
        QPointF p1 = mapToScene(m_line.p1());
        QPointF p2 = mapToScene(m_line.p2());
        m_p1->setPos(p1);
        m_p2->setPos(p2);
        break;
    }
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
