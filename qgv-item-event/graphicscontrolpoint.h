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
    /*
    enum Role {
        MoveRole = 0,
        VSizeRole,
        HSizeRole,
        BDiagSizeRole,
        FDiagDizeRole,
        RotateRole,
        ShearRole,
        MarkRole
    };

    QCursor roleToCursor(Role role) const;
    GCP(Role role, const QPointF &pos = QPointF);

    // Draw a line between this GraphicsControlPoint and other GraphicsControlPoint
    void link(int otherId);
    QList<int> links() const;
*/
    GraphicsControlPoint(GraphicsObject *parent, const QPointF &pos = QPointF());

public:
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) const;
    QPointF pos() const;
    void setPos(const QPointF &pos);
    //void setScenePos(const QPointF &pos);

    GraphicsControlPoint *clone(GraphicsObject *parent);

private:
    //friend class GraphicsObject;
    GraphicsObject *m_parent;
    QRectF m_rect;
    QPointF m_pos;
};

#endif // GRAPHICSCONTROLPOINT_H
