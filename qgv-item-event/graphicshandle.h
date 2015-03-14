#ifndef GRAPHICSHANDLE_H
#define GRAPHICSHANDLE_H

#include <QRectF>
#include <QPainterPath>

class GraphicsObject;
class QStyleOptionGraphicsItem;
class QPainter;
class QWidget;

class GraphicsHandle
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

    GraphicsHandle(Role role = MoveRole, const QPointF &pos = QPointF());
    GraphicsHandle(const GraphicsHandle &other);

    // Draw a line between this GraphicsHandle and other GraphicsHandle
    void addLink(GraphicsHandle *other);
    void removeLink(GraphicsHandle *other);
    // return a list of GraphicsHandle this GraphicsHandle has been explicitely linked to
    // (no retro link!)
    QList<GraphicsHandle *> links() const;

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
    QList<GraphicsHandle*> m_links;
    QPainterPath m_shape;
    QRectF m_boudingRect;
};

#endif // GRAPHICSHANDLE_H
