#ifndef GRAPHICSRECTITEM_H
#define GRAPHICSRECTITEM_H

#include  "graphicsobject.h"

#include <QVector>
#include <QPen>
#include <QBrush>

// TODO: AbstractGraphicsShape
// TODO: add properties
class GraphicsRectItem : public GraphicsObject
{
    Q_OBJECT

public:
    GraphicsRectItem(QGraphicsItem *parent = 0);
    ~GraphicsRectItem();

    QRectF rect() const;
    void setRect(const QRectF &rect);
    QPen pen() const;
    void setPen(const QPen &pen);
    QBrush brush() const;
    void setBrush(const QBrush &brush);

private:
    void updateControlPointsSilently();

    QRectF m_rect;
    QPen m_pen;
    QBrush m_brush;
    //QVector<const GraphicsControlPoint *> m_ctlPoints;

    // GraphicsObject interface
public:
    virtual GraphicsObject *clone();
    void controlPointMoved(GraphicsControlPoint *point);

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // QGraphicsItem interface
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // GRAPHICSRECTITEM_H
