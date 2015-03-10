#ifndef GRAPHICSCONTROLPOINT_H
#define GRAPHICSCONTROLPOINT_H

#include <QRectF>
#include <QPainterPath>

class GraphicsObject;
class QStyleOptionGraphicsItem;
class QPainter;
class QWidget;

class GraphicsControlPoint
{
public:
    enum Role {
        MoveRole = 0,
        VSizeRole,
        HSizeRole,
        BDiagSizeRole,
        FDiagSizeRole,
        RotateRole,
        ShearRole,
        MarkRole
    };

    GraphicsControlPoint(Role role = MoveRole, const QPointF &pos = QPointF());
    GraphicsControlPoint(const GraphicsControlPoint &other);

    // Draw a line between this GraphicsControlPoint and other GraphicsControlPoint
    void addLink(GraphicsControlPoint *other);
    void removeLink(GraphicsControlPoint *other);
    // return a list of GraphicsControlPoint this GraphicsControlPoint has been explicitely linked to
    // (no retro link!)
    QList<GraphicsControlPoint *> links() const;

    QCursor cursor() const;

    QVariant data() const;
    void setData(const QVariant &data);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) const;
    QPointF pos() const;
    void setPos(const QPointF &pos);

private:
    QCursor roleToCursor(Role role) const;
    //GraphicsObject *m_parent;
    Role m_role;
    //QPointF m_pos;
    QRectF m_rect;
    QVariant m_data;
    QList<GraphicsControlPoint*> m_links;
    QPainterPath m_shape;
    QRectF m_boudingRect;
};

#endif // GRAPHICSCONTROLPOINT_H
