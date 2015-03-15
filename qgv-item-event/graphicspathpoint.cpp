#include "graphicspathpoint.h"

#include <QDebug>

GraphicsPathPoint::GraphicsPathPoint(QGraphicsItem *parent):
    QGraphicsItem(parent),
    IGraphicsItemObserver(),
    IGraphicsObservableItem(),
    m_first(false),
    m_last(false),
    m_closing(false),
    m_handles(0),
    m_behaviour(UndefinedHandleBehaviour),
    m_nodeHandle(new GraphicsHandle()),
    m_control1Handle(new GraphicsHandle()),
    m_control2Handle(new GraphicsHandle())
{
    m_typeToHandle[NodeHandle] = &m_nodeHandle;
    m_typeToHandle[Control1Handle] = &m_control1Handle;
    m_typeToHandle[Control2Handle] = &m_control2Handle;

    m_nodeHandle->setParentItem(this);
    m_nodeHandle->setRole(MoveHandleRole);
    m_nodeHandle->setHandleShape(CircularHandleShape);
    m_nodeHandle->setVisible(false);

    m_control1Handle->setParentItem(this);
    m_control1Handle->setRole(MoveHandleRole);
    m_control1Handle->setHandleShape(DiamondedHandleShape);
    m_control1Handle->setVisible(false);

    m_control2Handle->setParentItem(this);
    m_control2Handle->setRole(MoveHandleRole);
    m_control2Handle->setHandleShape(DiamondedHandleShape);
    m_control2Handle->setVisible(false);

    setBehaviour(NormalHandleBehaviour);

    QGraphicsItem::setPos(QPointF(0, 0));
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setSelected(false);
    setVisible(false);
}

GraphicsPathPoint::~GraphicsPathPoint()
{
    if (m_nodeHandle) {
        removeObservedItem(m_nodeHandle);
        delete m_nodeHandle;
    }
    if (m_control1Handle) {
        removeObservedItem(m_control1Handle);
        delete m_control1Handle;
    }
    if (m_control2Handle) {
        removeObservedItem(m_control2Handle);
        delete m_control2Handle;
    }
    // removeItemObserver(); // ?
}

QPointF GraphicsPathPoint::nodePos() const
{
    if (m_handles.testFlag(NodeHandle)) {
        return mapToParent(m_nodeHandle->pos());
    }
    return QPointF();
}

// pos is in parent coordinate
void GraphicsPathPoint::setNodePos(const QPointF &pos)
{
    if (m_handles.testFlag(NodeHandle)) {
        //qDebug() << "setNodePos" << mapFromParent(pos);
        m_nodeHandle->setPos(mapFromParent(pos));
    }
    else
        qDebug() << "setNodePos!" << m_handles;
}

QPointF GraphicsPathPoint::control1Pos() const
{
    if (m_handles.testFlag(Control1Handle)) {
        return mapToParent(m_control1Handle->pos());
    }
    return QPointF();
}

void GraphicsPathPoint::setControl1Pos(const QPointF &pos)
{
    if (m_handles.testFlag(Control1Handle)) {
        m_control1Handle->setPos(mapFromParent(pos));
    }
    else
        qDebug() << "setCtl1Pos!";
}

QPointF GraphicsPathPoint::control2Pos() const
{
    if (m_handles.testFlag(Control2Handle)) {
        return mapToParent(m_control2Handle->pos());
    }
    return QPointF();

}

void GraphicsPathPoint::setControl2Pos(const QPointF &pos)
{
    if (m_handles.testFlag(Control2Handle)) {
        return m_control2Handle->setPos(mapFromParent(pos));
    }
    else
        qDebug() << "setCtl2Pos!";
}

QPointF GraphicsPathPoint::pos(GraphicsPathPoint::HandleType type)
{
    switch (type) {
    case NodeHandle:
        return nodePos();
    case Control1Handle:
        return control1Pos();
    case Control2Handle:
        return control2Pos();
    default:
        Q_ASSERT(false);
    }
}

void GraphicsPathPoint::setPos(GraphicsPathPoint::HandleType type, const QPointF &pos)
{
    switch (type) {
    case NodeHandle:
        setNodePos(pos);
        return;
    case Control1Handle:
        setControl1Pos(pos);
        return;
    case Control2Handle:
        setControl2Pos(pos);
        return;
    default:
        Q_ASSERT(false);
    }
}

GraphicsPathPoint::HandleTypes GraphicsPathPoint::handlesEnabled() const
{
    return m_handles;

}

void GraphicsPathPoint::enableHandle(GraphicsPathPoint::HandleType type, bool set)
{
    GraphicsHandle **handle = m_typeToHandle[type];
    Q_ASSERT(handle != nullptr);

    //qDebug() << type << m_handles << "=>" << set;

    if (set) {
        m_handles |= type;
        (*handle)->setVisible(true); // Our visibility take over
        // We do it here, because it causes contructor issues
        // If called in constructor, it would cause a call on a member function
        // while the object is not fully contructed
        addObservedItem(*handle);
    }
    else {
        m_handles &= ~type;
        (*handle)->setVisible(false);
        removeObservedItem(*handle);
    }

    qDebug() << m_handles;
}

bool GraphicsPathPoint::isFirst() const
{
    return m_first;
}

void GraphicsPathPoint::setFirst(bool first)
{
    if (m_first == first)
        return;
    qDebug() << "First" << m_first << "=>" << first;
    m_first = first;
    updateEnabledHandles();
}

bool GraphicsPathPoint::isLast() const
{
    return m_last;
}

void GraphicsPathPoint::setLast(bool last)
{
    if (m_last == last)
        return;
    qDebug() << "Last" << m_last << "=>" << last;
    m_last = last;
    updateEnabledHandles();
}

bool GraphicsPathPoint::isClosingPath() const
{
    return m_closing;
}

void GraphicsPathPoint::setClosingPath(bool closing)
{
    if (m_closing == closing)
        return;
    m_closing = closing;
    updateEnabledHandles();
}

void GraphicsPathPoint::setBehaviour(GraphicsHandleBehaviour behaviour)
{
    if (m_behaviour == behaviour)
        return;
    qDebug() << "Behaviour" << m_behaviour << "=>" << behaviour;
    m_behaviour = behaviour;
    updateEnabledHandles();
}

GraphicsHandleBehaviour GraphicsPathPoint::behaviour() const
{
    return m_behaviour;
}

bool GraphicsPathPoint::handleEnabled(GraphicsPathPoint::HandleType type)
{
    return m_handles.testFlag(type);
}

void GraphicsPathPoint::updateEnabledHandles()
{
    enableHandle(NodeHandle, true);
    enableHandle(Control1Handle, (isFirst() && isClosingPath()) || !isFirst());
    enableHandle(Control2Handle, (isLast() && isClosingPath()) || !isLast());
}

void GraphicsPathPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->setPen(QPen(QBrush(Qt::green), 0));

    if (m_handles.testFlag(Control2Handle)) {
        QLineF line(nodePos(), control2Pos());
        painter->drawLine(line);
    }

    if (m_handles.testFlag(Control1Handle)) {
        QLineF line(nodePos(), control1Pos());
        painter->drawLine(line);
    }

    painter->restore();
}

QVariant GraphicsPathPoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{

    if (change == QGraphicsItem::ItemPositionHasChanged) {
        notifyObservers();
    }
    return value;
}

void GraphicsPathPoint::itemNotification(IGraphicsObservableItem *item)
{
    GraphicsHandle *handle = dynamic_cast<GraphicsHandle*>(item);
    Q_ASSERT(handle != nullptr);
    if (handle == m_nodeHandle) {
        Q_ASSERT(m_handles.testFlag(NodeHandle));
    }
    else if (item == m_control1Handle) {
        Q_ASSERT(m_handles.testFlag(Control1Handle));
    }
    else if (item == m_control2Handle) {
        Q_ASSERT(m_handles.testFlag(Control2Handle));
    }
    else {
        Q_ASSERT(false);
    }
    notifyObservers();
}

QRectF GraphicsPathPoint::boundingRect() const
{
    return childrenBoundingRect();
}

