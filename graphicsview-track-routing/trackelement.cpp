#include "trackelement.h"

TrackElement::TrackElement(QGraphicsItem *parent): QGraphicsItem(parent),
    m_width(50), m_clearance(10)
{

}


qreal TrackElement::width() const
{
    return m_width;
}

void TrackElement::setWidth(qreal width)
{
    if (qFuzzyCompare(m_width, width))
        return;

    prepareGeometryChange();
    m_width = width;
}

qreal TrackElement::clearance() const
{
    return m_clearance;
}

void TrackElement::setClearance(qreal clearance)
{
    if (qFuzzyCompare(m_clearance, clearance))
        return;

    prepareGeometryChange();
    m_clearance = clearance;
}

void TrackElement::adjust()
{

}
