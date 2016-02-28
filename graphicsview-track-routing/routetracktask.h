#ifndef ROUTETRACKTASK_H
#define ROUTETRACKTASK_H

#include "igraphicstask.h"
#include <QObject>

#include <QPainterPath>
class IPathRouter;
class QGraphicsPathItem;

class RouteTrackTask : public IGraphicsTask
{
    Q_OBJECT
public:
    explicit RouteTrackTask(QObject *parent = 0);
    ~RouteTrackTask();

signals:

public slots:

private:
    QList<IPathRouter *> m_routers;
    int m_currentRouterIndex;
    void cycleRouter();
    IPathRouter *router();

    QGraphicsPathItem *m_item;
    void updatePath(const QPointF &scenePos);

    QList<QGraphicsPathItem *> m_items;

private:
    void decoratePathAsPermanent(QGraphicsPathItem *item);
    void decoratePathAsTemporary(QGraphicsPathItem *item);

    // IGraphicsTask interface
public:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // ROUTETRACKTASK_H
