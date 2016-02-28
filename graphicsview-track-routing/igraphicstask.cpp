#include "igraphicstask.h"
#include "graphicsview.h"

#include <QGraphicsScene>

IGraphicsTask::IGraphicsTask(QObject *parent): QObject(parent)
{}

IGraphicsTask::~IGraphicsTask()
{}

void IGraphicsTask::setView(GraphicsView *view)
{
    m_view = view;
}

GraphicsView *IGraphicsTask::view() const
{
    return m_view;
}

QGraphicsScene *IGraphicsTask::scene() const
{
    return m_view->scene();
}

void IGraphicsTask::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void IGraphicsTask::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void IGraphicsTask::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void IGraphicsTask::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void IGraphicsTask::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

void IGraphicsTask::keyReleaseEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}
