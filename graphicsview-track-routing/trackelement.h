#ifndef TRACKELEMENT_H
#define TRACKELEMENT_H

#include <QGraphicsItem>

class TrackElement : public QGraphicsItem
{
public:
    TrackElement(QGraphicsItem *parent = nullptr);

    qreal width() const;
    void setWidth(qreal width);

    qreal clearance() const;
    void setClearance(qreal clearance);

    virtual void adjust();

private:
    qreal m_width;
    qreal m_clearance;

};

#endif // TRACKELEMENT_H
