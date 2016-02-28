#include "routetracktask.h"
#include "ipathrouter.h"
#include "graphicsview.h"

#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

RouteTrackTask::RouteTrackTask(QObject *parent) : IGraphicsTask(parent)
{
    m_item = nullptr;
    m_routers << new StraightPathRouter
              << new DiagonalPathRouter
              << new DirectPathRouter;
    m_currentRouterIndex = 0;
}

RouteTrackTask::~RouteTrackTask()
{
    qDeleteAll(m_routers);
}

void RouteTrackTask::cycleRouter()
{
    IPathRouter *old = router();
    m_currentRouterIndex++;
    m_currentRouterIndex %= m_routers.count();
    router()->setDirection(old->direction());
    router()->start(QPointF(0, 0));
}

IPathRouter *RouteTrackTask::router()
{
    return m_routers.at(m_currentRouterIndex);
}

void RouteTrackTask::updatePath(const QPointF &scenePos)
{
    QPointF itemPos = m_item->mapFromScene(scenePos);
    QPainterPath path = router()->route(itemPos);
    m_item->setPath(path);
}

void RouteTrackTask::decoratePathAsPermanent(QGraphicsPathItem *item)
{
    QPen pen;
    pen.setWidthF(15);
    pen.setColor(Qt::red);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    item->setPen(pen);
}

void RouteTrackTask::decoratePathAsTemporary(QGraphicsPathItem *item)
{
    QPen pen;
    pen.setWidthF(15);
    pen.setBrush(QBrush(Qt::red, Qt::Dense4Pattern));
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    item->setPen(pen);
}

void RouteTrackTask::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (m_item != nullptr)
        {
            decoratePathAsPermanent(m_item);
            m_items.append(m_item);
        }
        m_item = scene()->addPath(QPainterPath());
        m_item->setPos(event->scenePos());
        m_item->setFlag(QGraphicsItem::ItemIsMovable);
        m_item->setFlag(QGraphicsItem::ItemIsSelectable);
        decoratePathAsTemporary(m_item);
        router()->start(QPointF(0, 0));
        updatePath(event->scenePos());
        event->accept();
    }
    else if (event->button() == Qt::RightButton)
    {
        if (m_item != nullptr)
        {
            scene()->removeItem(m_item);
            delete m_item;
            m_item = nullptr;
            if (m_items.count() > 0)
            {
                m_item = m_items.takeLast();
                decoratePathAsTemporary(m_item);
                updatePath(event->scenePos());
            }
            event->accept();
        }

    }
}

void RouteTrackTask::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_item != nullptr && m_item->path().elementCount() != 0)
    {
        updatePath(event->scenePos());
        event->accept();
    }
}

void RouteTrackTask::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
}

void RouteTrackTask::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
#if 0 // Combine all the item's path into a single QGraphicsPathItem
        QPainterPath path;
        bool firstMove = true;
        for (QGraphicsPathItem *item: m_items)
        {
            QPainterPath subPath = item->mapToScene(item->path());
            for (int idx=0; idx<subPath.elementCount(); idx++)
            {
                QPainterPath::Element element = subPath.elementAt(idx);
                switch (element.type)
                {
                case QPainterPath::LineToElement:
                    path.lineTo(element.x, element.y);
                    break;
                case QPainterPath::MoveToElement:
                    if (firstMove)
                    {
                        path.moveTo(element.x, element.y);
                        firstMove = false;
                    }
                    break;
                case QPainterPath::CurveToElement:
                default:
                    break;
                }
            }
            scene()->removeItem(item);
            delete item;
        }
        m_items.clear();
        m_item = nullptr;

        QGraphicsPathItem *item = scene()->addPath(path);
        item->setFlag(QGraphicsItem::ItemIsMovable);
        item->setFlag(QGraphicsItem::ItemIsSelectable);
        decoratePathAsPermanent(item);

        event->accept();
#else
        decoratePathAsPermanent(m_item);
        m_items.clear();
        m_item = nullptr;
        event->accept();
#endif
    }
}


void RouteTrackTask::keyPressEvent(QKeyEvent *event)
{
    QPointF scenePos = view()->mapToScene(view()->mapFromGlobal(QCursor::pos()));

    if (event->modifiers().testFlag(Qt::ShiftModifier) && event->key() == Qt::Key_Space)
    {
        cycleRouter();
        updatePath(scenePos);
    }
    else if (event->key() == Qt::Key_Space)
    {
        router()->toggleDirection();
        updatePath(scenePos);
    }
    else
    {
        event->ignore();
    }
}

void RouteTrackTask::keyReleaseEvent(QKeyEvent *event)
{
    event->ignore();
}
