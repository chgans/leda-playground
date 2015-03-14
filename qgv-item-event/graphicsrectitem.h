#ifndef GRAPHICSRECTITEM_H
#define GRAPHICSRECTITEM_H

#include  "graphicsobject.h"
#include "graphicshandle.h"

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
    GraphicsRectItem(GraphicsObject *parent = 0);
    ~GraphicsRectItem();

    QRectF rect() const;
    void setRect(const QRectF &rect);
    QPen pen() const;
    void setPen(const QPen &pen);
    QBrush brush() const;
    void setBrush(const QBrush &brush);

private:
    enum HandleId {
        TopLeft = 0,
        Top,
        TopRight,
        Right,
        BottomRight,
        Bottom,
        BottomLeft,
        Left,
        NbHandles
    };
    QRectF m_rect;
    QPen m_pen;
    QBrush m_brush;
    QMap<const GraphicsHandle *, HandleId> m_handleToId;
    QMap<HandleId, const GraphicsHandle *> m_idToHandle;
    void addHandle(HandleId handleId, GraphicsHandle::Role role);
    void updateHandlesSilently();
    mutable QRectF m_boundingRect;
    mutable QPainterPath m_shape;
    mutable bool m_dirty;
    void updateGeometry() const;
    void markDirty();

    // GraphicsObject interface
public:
    virtual GraphicsObject *clone();
    void handleMoved(const GraphicsHandle *handle);

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
