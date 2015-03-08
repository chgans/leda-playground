#ifndef GRAPHICSRECTITEM_H
#define GRAPHICSRECTITEM_H

#include  "graphicsobject.h"
#include "graphicscontrolpoint.h"

#include <QVector>
#include <QMap>

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
    enum CtlPointId {
        TopLeft = 0,
        Top,
        TopRight,
        Right,
        BottomRight,
        Bottom,
        BottomLeft,
        Left,
        NbControlPoint
    };
    QRectF m_rect;
    QPen m_pen;
    QBrush m_brush;
    QMap<const GraphicsControlPoint *, CtlPointId> m_ctlPointToId;
    QMap<CtlPointId, const GraphicsControlPoint *> m_idToCtlPoint;
    void addControlPoint(CtlPointId pointId, GraphicsControlPoint::Role role);
    void updateControlPointsSilently();


    // GraphicsObject interface
public:
    virtual GraphicsObject *clone();
    void controlPointMoved(const GraphicsControlPoint *point);

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
