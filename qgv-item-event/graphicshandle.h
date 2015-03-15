#ifndef GRAPHICSHANDLE_H
#define GRAPHICSHANDLE_H

#include <QGraphicsPathItem>
#include "igraphicsobservableitem.h"

#include <QPainter>
#include <QPointer>
#include <QCursor>

class GraphicsObject;
class QStyleOptionGraphicsItem;
class QPainter;
class QWidget;

enum GraphicsHandleRole {
    MoveHandleRole = 0,
    VSizeHandleRole,
    HSizeHandleRole,
    BDiagSizeHandleRole,
    FDiagSizeHandleRole,
    RotateHandleRole,
    ShearHandleRole,
    MarkHandleRole
};

enum GraphicsHandleShape {
    UndefinedHandleShape = 0,
    CircularHandleShape,
    SquaredHandleShape,
    DiamondedHandleShape
};

enum GraphicsHandleBehaviour {
    UndefinedHandleBehaviour = 0,
    NormalHandleBehaviour,
    CornerHandleBehaviour,
    SmoothHandleBehaviour,
    SymetricHandleBehaviour,
    AutoSmoothHandleBehaviour
};

class GraphicsHandle: public QGraphicsPathItem, public IGraphicsObservableItem
{
public:
    GraphicsHandle(QGraphicsItem *parent = 0);

    QCursor cursor() const;

    void setRole(GraphicsHandleRole role);
    GraphicsHandleRole role() const;
    void setHandleShape(GraphicsHandleShape shape);
    GraphicsHandleShape handleShape() const;

protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    static QCursor roleToCursor(GraphicsHandleRole role);
    GraphicsHandleRole m_role;
    GraphicsHandleShape m_handleShape;
};

#endif // GRAPHICSHANDLE_H
