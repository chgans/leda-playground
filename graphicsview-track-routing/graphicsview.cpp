#include "graphicsview.h"
#include "routetracktask.h"

#include <QMouseEvent>
#include <QOpenGLWidget>

#include <QGraphicsSceneMouseEvent>

GraphicsView::GraphicsView(QWidget *parent):
    QGraphicsView(parent)
{
    setInteractive(true);
    setMouseTracking(true);
    //setViewport(new QOpenGLWidget);
    m_task = new RouteTrackTask(this);
    m_task->setView(this);
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMousePress);
    buildMouseSceneEvent(&mouseEvent, event);
    mouseEvent.setAccepted(false);

    if (m_task != nullptr)
    {
        m_task->mousePressEvent(&mouseEvent);
    }

    event->setAccepted(mouseEvent.isAccepted());
    if (!event->isAccepted())
    {
        QGraphicsView::mousePressEvent(event);
    }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMouseRelease);
    buildMouseSceneEvent(&mouseEvent, event);
    mouseEvent.setAccepted(false);

    if (m_task != nullptr)
    {
        m_task->mouseReleaseEvent(&mouseEvent);
    }

    event->setAccepted(mouseEvent.isAccepted());
    if (!event->isAccepted())
    {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void GraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMouseDoubleClick);
    buildMouseSceneEvent(&mouseEvent, event);
    mouseEvent.setAccepted(false);

    if (m_task != nullptr)
    {
        m_task->mouseDoubleClickEvent(&mouseEvent);
    }

    event->setAccepted(mouseEvent.isAccepted());
    if (!event->isAccepted())
    {
        QGraphicsView::mouseDoubleClickEvent(event);
    }
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMouseMove);
    buildMouseSceneEvent(&mouseEvent, event);
    mouseEvent.setAccepted(false);

    if (m_task != nullptr)
        m_task->mouseMoveEvent(&mouseEvent);

    event->setAccepted(mouseEvent.isAccepted());
    if (!event->isAccepted())
    {
        QGraphicsView::mouseMoveEvent(event);
    }
}

void GraphicsView::buildMouseSceneEvent(QGraphicsSceneMouseEvent *sceneEvent, QMouseEvent *event)
{
    sceneEvent->setWidget(viewport());
    //sceneEvent->setButtonDownScenePos(mousePressButton, mousePressScenePoint);
    //sceneEvent->setButtonDownScreenPos(mousePressButton, mousePressScreenPoint);
    sceneEvent->setScenePos(mapToScene(event->pos()));
    sceneEvent->setScreenPos(event->globalPos());
    //sceneEvent->setLastScenePos(lastMouseMoveScenePoint);
    //sceneEvent->setLastScreenPos(lastMouseMoveScreenPoint);
    sceneEvent->setButtons(event->buttons());
    sceneEvent->setButton(event->button());
    sceneEvent->setModifiers(event->modifiers());
    sceneEvent->setSource(event->source());
    sceneEvent->setFlags(event->flags());
}


void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    if (m_task != nullptr)
    {
        m_task->keyPressEvent(event);
    }

    if (!event->isAccepted())
    {
        if (event->key() == Qt::Key_Escape)
        {
            m_task = nullptr;
        }
        else
        {
            event->ignore();
        }
    }

    if (!event->isAccepted())
    {
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    if (m_task != nullptr)
    {
        m_task->keyReleaseEvent(event);
    }

    if (!event->isAccepted())
    {
        QGraphicsView::keyReleaseEvent(event);
    }
}
