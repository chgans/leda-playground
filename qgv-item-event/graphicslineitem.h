#ifndef GRAPHICSLINEITEM_H
#define GRAPHICSLINEITEM_H

#include <QGraphicsLineItem>
#include "graphicsiteminterface.h"

class GraphicsLineItem : public QGraphicsLineItem, public GraphicsItemInterface
{
public:
    GraphicsLineItem(QGraphicsItem *parent = 0);

    QGraphicsItem *clone();

    void controlPointMoved(const GraphicsControlPoint *point);

private:
    GraphicsControlPoint *m_ctlPoint1;
    GraphicsControlPoint *m_ctlPoint2;

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // QGraphicsItem interface
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // GRAPHICSLINEITEM_H
