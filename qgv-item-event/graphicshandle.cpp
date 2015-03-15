#include "graphicsobject.h"
#include "graphicshandle.h"

#include <QStyleOptionGraphicsItem>
#include <QPainterPathStroker>
#include <QPainter>

#include <QDebug>

GraphicsHandle::GraphicsHandle(QGraphicsItem *parent):
    QGraphicsPathItem(parent),
    IGraphicsObservableItem(),
    m_handleShape(UndefinedHandleShape)
{
    setPen(QPen(QBrush(Qt::red), 0));
    setBrush(QBrush(Qt::white));

    setHandleShape(CircularHandleShape);

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIgnoresTransformations);

    setVisible(false);
}

QCursor GraphicsHandle::cursor() const
{
    return roleToCursor(m_role);
}

void GraphicsHandle::setRole(GraphicsHandleRole role)
{
    if (m_role == role)
        return;
    m_role = role;
}

GraphicsHandleRole GraphicsHandle::role() const
{
    return m_role;
}

void GraphicsHandle::setHandleShape(GraphicsHandleShape shape)
{
    if (m_handleShape == shape)
        return;
    prepareGeometryChange();
    m_handleShape = shape;

    static const int radius = 5;

    QPainterPath path;
    switch (shape) {
    case CircularHandleShape:
        path.addEllipse(QPointF(0, 0), radius, radius);
        break;
    case SquaredHandleShape:
        path.addRect(-radius, -radius, 2*radius, 2*radius);
        break;
    case DiamondedHandleShape: {
        QVector<QPointF> points;
        points << QPointF(0, -radius) << QPointF(radius, 0)
               << QPointF(0, radius) << QPointF(-radius, 0);
        path.addPolygon(QPolygonF(points));
        path.closeSubpath();
        break;
    }
    default:
        // Not reached
        break;
    }
    setPath(path);
}

GraphicsHandleShape GraphicsHandle::handleShape() const
{
    return m_handleShape;
}

QVariant GraphicsHandle::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionHasChanged) {
        notifyObservers();
    }
    return value;
}

QCursor GraphicsHandle::roleToCursor(GraphicsHandleRole role)
{
    switch (role)
    {
    case MoveHandleRole: return QCursor(Qt::PointingHandCursor);
    case VSizeHandleRole: return QCursor(Qt::SizeVerCursor);
    case HSizeHandleRole: return QCursor(Qt::SizeHorCursor);
    case BDiagSizeHandleRole: return QCursor(Qt::SizeBDiagCursor);
    case FDiagSizeHandleRole: return QCursor(Qt::SizeFDiagCursor);
    case RotateHandleRole: return QCursor(); // TBD
    case ShearHandleRole: return QCursor();  // TBD
    case MarkHandleRole: return QCursor();   // TBD
    default: return QCursor();
    }
}
