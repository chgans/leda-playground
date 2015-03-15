#ifndef GRAPHICSHANDLE_H
#define GRAPHICSHANDLE_H

#include <QGraphicsPathItem>

class GraphicsObject;
class QStyleOptionGraphicsItem;
class QPainter;
class QWidget;

class GraphicsHandle: public QGraphicsPathItem
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

    enum Kind {
        CircleHandle = 0,
        SquareHandle,
        DiamondHandle
    };

    GraphicsHandle(Role role, Kind kind,
                   const QPointF &pos, GraphicsObject *parent);

    QCursor cursor() const;
    Kind kind() const;

protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    QCursor roleToCursor(Role role) const;
    GraphicsObject *m_parent;
    Role m_role;
    Kind m_kind;
};

#endif // GRAPHICSHANDLE_H
