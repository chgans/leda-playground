#include "graphicsobject.h"
#include "graphicshandle.h"

#include <QStyleOptionGraphicsItem>
#include <QPainterPathStroker>
#include <QPainter>

#include <QDebug>

GraphicsHandle::GraphicsHandle(GraphicsHandle::Role role,
                               Kind kind,
                               const QPointF &pos, GraphicsObject *parent):
    QGraphicsPathItem(parent),
    m_parent(parent),
    m_role(role), m_kind(kind)
{
    static const int radius = 3;

    QPainterPath path;
    switch (kind) {
    case CircleHandle:
        path.addEllipse(QPointF(0, 0), radius, radius);
        break;
    case SquareHandle:
        path.addRect(-radius, -radius, 2*radius, 2*radius);
        break;
    case DiamondHandle: {
        QVector<QPointF> points;
        points << QPointF(0, -radius) << QPointF(radius, 0)
               << QPointF(0, radius) << QPointF(-radius, 0);
        path.addPolygon(QPolygonF(points));
        break;
    }
    default:
        // Not reached
        break;
    }
    setPath(path);

    setPos(pos);
    setPen(QPen(QBrush(Qt::red), 0));
    setBrush(QBrush(Qt::white));

    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIgnoresTransformations);

    setVisible(false);
}

QCursor GraphicsHandle::cursor() const
{
    return roleToCursor(m_role);
}

GraphicsHandle::Kind GraphicsHandle::kind() const
{
    return m_kind;
}

QVariant GraphicsHandle::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionHasChanged) {
        m_parent->handleMoved(this);
    }
    return value;
}

QCursor GraphicsHandle::roleToCursor(GraphicsHandle::Role role) const
{
    switch (role)
    {
    case MoveRole: return QCursor(Qt::PointingHandCursor);
    case VSizeRole: return QCursor(Qt::SizeVerCursor);
    case HSizeRole: return QCursor(Qt::SizeHorCursor);
    case BDiagSizeRole: return QCursor(Qt::SizeBDiagCursor);
    case FDiagSizeRole: return QCursor(Qt::SizeFDiagCursor);
    case RotateRole: return QCursor(); // TBD
    case ShearRole: return QCursor();  // TBD
    case MarkRole: return QCursor();   // TBD
    default: return QCursor();
    }
}


