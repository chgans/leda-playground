#include "graphicsbeziertool.h"
#include "graphicsbezieritem.h"
#include "graphicscontrolpoint.h"
#include "graphicsview.h"
#include "graphicsscene.h"

#include <QMouseEvent>

#include <QAction>
#include <QIcon>

GraphicsBezierTool::GraphicsBezierTool(QObject *parent):
    GraphicsTool(parent), m_item(nullptr), m_elementNb(0)
{

}

void GraphicsBezierTool::mousePressEvent(QMouseEvent *event)
{
    QPointF scenePos = view()->mapToScene(event->pos());
    if (m_item == nullptr) {
        m_item = new GraphicsBezierItem();
        m_item->setPen(QPen(QBrush(Qt::darkCyan), 2));
        scene()->addItem(m_item);
        m_item->setPos(view()->mapToScene(event->pos()));
        QPointF itemPos = m_item->mapFromScene(scenePos);
        QPainterPath path(QPointF(0, 0));
        path.cubicTo(itemPos + QPointF(-0.1, -0.1),
                     itemPos + QPointF(0.1, 0.1),
                     itemPos);
        m_item->setPath(path);
    }
    else {
        QPointF itemPos = m_item->mapFromScene(scenePos);
        QPainterPath path = m_item->path();
        path.cubicTo(itemPos + QPointF(-0.1, -0.1),
                     itemPos + QPointF(0.1, 0.1),
                     itemPos);
        m_item->setPath(path);
    }
}

void GraphicsBezierTool::mouseMoveEvent(QMouseEvent *event)
{
    if  (m_item != nullptr) {
        QPointF scenePos = view()->mapToScene(event->pos());
        QPointF itemPos = m_item->mapFromScene(scenePos);
        QPainterPath path = m_item->path();
        int n = path.elementCount();
        path.setElementPositionAt(n - 1, itemPos.x(), itemPos.y());
        path.setElementPositionAt(n - 2, itemPos.x(), itemPos.y());
        m_item->setPath(path);
    }
    event->accept();
}

void GraphicsBezierTool::mouseReleaseEvent(QMouseEvent *event)
{
}

void GraphicsBezierTool::keyPressEvent(QKeyEvent *event)
{
}

void GraphicsBezierTool::keyReleaseEvent(QKeyEvent *event)
{
}

QDialog *GraphicsBezierTool::optionDialog()
{
    return nullptr;
}

QString GraphicsBezierTool::toolGroup() const
{
    return "interactive-tools";
}

QAction *GraphicsBezierTool::action() const
{
    return new QAction(QIcon(":/icons/graphicsbeziertool.svg"),
                                  "Place a bezier curve", nullptr);
}

void GraphicsBezierTool::activate()
{
}

void GraphicsBezierTool::desactivate()
{
}
