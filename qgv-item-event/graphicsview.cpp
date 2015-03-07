#include "graphicsview.h"
#include "graphicsscene.h"
#include "graphicsobject.h"
#include "graphicstool.h"
//#include "graphicsrectitem.h"

#include <QGraphicsScene>
#include <QGraphicsOpacityEffect>
#include <QRubberBand>

#include <QMouseEvent>
#include <QMessageBox>

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
    m_tool(nullptr)
{
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
    m_tool = tool;
    if (m_tool)
        m_tool->setView(this);
}

GraphicsObject *GraphicsView::objectAt(const QPoint &pos) const
{
    return dynamic_cast<GraphicsObject *>(QGraphicsView::itemAt(pos));
}

void GraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
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
    if (m_tool != nullptr)
        m_tool->mouseMoveEvent(event);
    else
        event->ignore();
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
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
    if (m_tool != nullptr)
        m_tool->keyPressEvent(event);
    else
        event->ignore();
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    if (m_tool != nullptr)
        m_tool->keyReleaseEvent(event);
    else
        event->ignore();
}
