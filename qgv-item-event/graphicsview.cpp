#include "graphicsview.h"
#include "graphicsscene.h"
#include "graphicsobject.h"
#include "graphicstool.h"

#include <QGraphicsScene>
#include <QGraphicsOpacityEffect>
#include <QGLWidget>

#include <QMouseEvent>
#include <QMessageBox>

#include <QDebug>

// Tools:
//  Select (default)
//  ZoomBox
//  Drag
//  Insert
//  Edit
// Use graphical effect for item selection and move ?
// createPhantom() and createClone() to help the model command stuff

/* TODO:
 *  - context menu:
 *   - tool options
 *   - cancel
 *   - properties
 */

GraphicsView::GraphicsView(QWidget *parent):
    QGraphicsView(parent),
    m_tool(nullptr),
    m_objectUnderMouse(nullptr),
    m_handleUnderMouse(nullptr)
{    
    setViewport(new QGLWidget);

    // The widget emits the QWidget::customContextMenuRequested() signal
    setContextMenuPolicy(Qt::CustomContextMenu);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setDragMode(QGraphicsView::NoDrag);
    setMouseTracking(true);
}

GraphicsView::~GraphicsView()
{

}

GraphicsScene *GraphicsView::scene()
{
    return static_cast<GraphicsScene *>(QGraphicsView::scene());
}

void GraphicsView::setScene(GraphicsScene *scene)
{
    QGraphicsView::setScene(scene);
}

GraphicsTool *GraphicsView::tool()
{
    return m_tool;
}

void GraphicsView::setTool(GraphicsTool *tool)
{
    if (m_tool)
        m_tool->desactivate();
    m_tool = tool;
    if (m_tool) {
        m_tool->setView(this);
        m_tool->activate();
    }
}

GraphicsObject *GraphicsView::objectAt(const QPoint &pos) const
{
    return dynamic_cast<GraphicsObject *>(QGraphicsView::itemAt(pos));
}

const GraphicsControlPoint *GraphicsView::handleAt(const QPoint &pos) const
{
    const GraphicsObject *object = objectAt(pos);
    if (object == nullptr)
        return nullptr;
    QPointF scenePos = mapToScene(pos);
    QPointF itemPos = object->mapFromScene(scenePos);
    return object->controlPointNear(itemPos);
}

const GraphicsControlPoint *GraphicsView::handleUnderMouse() const
{
    return handleAt(mousePosition());
}

GraphicsObject *GraphicsView::objectUnderMouse() const
{
    return objectAt(mousePosition());
}

QPoint GraphicsView::mousePosition() const
{
    return mapFromGlobal(QCursor::pos());
}

void GraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{

    QLinearGradient gradient(rect.topLeft(), rect.bottomLeft());
    gradient.setColorAt(0, Qt::darkBlue);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect, QBrush(gradient));

    QGraphicsView::drawBackground(painter, rect);
}

void GraphicsView::drawForeground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawForeground(painter, rect);
}

// TODO: Zoom here or tool
void GraphicsView::wheelEvent(QWheelEvent *event)
{
    if (m_tool != nullptr)
        m_tool->wheelEvent(event);
    else
        event->ignore();
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        //qDebug() << "leftMouseButtonPressed";
        emit leftMouseButtonPressed();
    }

    if (event->button() == Qt::RightButton) {
        QGraphicsView::mousePressEvent(event);
    }
    else if (event->button() == Qt::LeftButton && m_tool) {
        m_tool->mousePressEvent(event);
    }
    else {
        event->ignore();
    }
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (objectUnderMouse() && !m_objectUnderMouse) {
        //qDebug() << "hoverItemEntered";
        m_objectUnderMouse = objectUnderMouse();
        emit hoverItemEntered();
        if (event->modifiers().testFlag(Qt::ControlModifier))
            emit ctlKeyPressed();
    }
    if (!objectUnderMouse() && m_objectUnderMouse) {
        //qDebug() << "hoverItemLeft";
        emit hoverItemLeft();
        if (event->modifiers().testFlag(Qt::ControlModifier))
            emit ctlKeyPressed();
        m_objectUnderMouse = nullptr;
    }
    if (handleUnderMouse() && !m_handleUnderMouse) {
        //qDebug() << "hoverHandleEntered";
        m_handleUnderMouse = handleUnderMouse();
        if (event->modifiers().testFlag(Qt::ControlModifier))
            emit ctlKeyPressed();
        emit hoverHandleEntered();
    }
    if (!handleUnderMouse() && m_handleUnderMouse) {
        //qDebug() << "hoverHandleLeft";
        emit hoverHandleLeft();
        if (event->modifiers().testFlag(Qt::ControlModifier))
            emit ctlKeyPressed();
        m_handleUnderMouse = nullptr;
    }

    if (event->buttons().testFlag(Qt::LeftButton)) {
        //qDebug() << "mouseMoved";
        emit mouseMoved();
    }

    if (m_tool != nullptr)
        m_tool->mouseMoveEvent(event);
    else
        event->ignore();
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        //qDebug() << "leftMouseButtonReleased";
        emit leftMouseButtonReleased();
    }

    if (m_tool != nullptr)
        m_tool->mouseReleaseEvent(event);
    else
        event->ignore();
}

// TODO:
void GraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
#if 0
    if (event->button() != Qt::LeftButton)
        return;
    emit mouseDoubleClicked(event->pos());
    event->accept();
#else
    if (m_tool != nullptr)
        m_tool->mouseDoubleClickEvent(event);
    else
        event->ignore();
#endif
}

// TODO: manage esc and tab here or tool or tool manager?
void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        qDebug() << "escKeyPressed";
        emit escKeyPressed();
    }
    else if (event->key() == Qt::Key_Control) {
        qDebug() << "ctlKeyPressed";
        emit ctlKeyPressed();
    }

    if (m_tool != nullptr)
        m_tool->keyPressEvent(event);
    else
        event->ignore();
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        qDebug() << "escKeyReleased";
        emit escKeyReleased();
    }
    else if (event->key() == Qt::Key_Control) {
        qDebug() << "ctlKeyReleased";
        emit ctlKeyReleased();
    }

    if (m_tool != nullptr)
        m_tool->keyReleaseEvent(event);
    else
        event->ignore();
}
