#ifndef GRAPHICSLINEITEM_H
#define GRAPHICSLINEITEM_H

#include "graphicsobject.h"

#include <QPen>
#include <QLineF>

class GraphicsLineItem : public GraphicsObject
{
    Q_OBJECT

public:
    GraphicsLineItem(QGraphicsItem *parent = 0);

    QLineF line() const;
    void setLine(const QLineF &line);
    QPen pen() const;
    void setPen(const QPen &pen);

    // GraphicsObject interface
public:
    GraphicsObject *clone();
    void controlPointMoved(GraphicsControlPoint *point);

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // QGraphicsItem interface
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    QPen m_pen;
    QLineF m_line;
    GraphicsControlPoint *m_ctlPoint1;
    GraphicsControlPoint *m_ctlPoint2;
};

#endif // GRAPHICSLINEITEM_H
