#include "graphicsrecttool.h"
#include "graphicsrectitem.h"
#include "graphicslinetooldialog.h"
#include "graphicscontrolpoint.h"
#include "graphicsscene.h"
#include "graphicsview.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QAction>

GraphicsRectTool::GraphicsRectTool(QObject *parent):
    GraphicsTool(parent), m_state(0), m_item(nullptr)
{

}

void GraphicsRectTool::mousePressEvent(QMouseEvent *event)
{

    if (m_state == 0) {
        Q_ASSERT(m_item == nullptr);
        m_item = new GraphicsRectItem();
        m_item->setFlags(QGraphicsItem::ItemIsMovable |
                         QGraphicsItem::ItemIsSelectable);
        QPointF scenePos = view()->mapToScene(event->pos());
        m_item->setPos(scenePos);
        setP1(event->pos());
        setP2(event->pos());
        m_item->setPen(QPen(QBrush(Qt::darkBlue), 0));
        m_item->setBrush(QBrush(Qt::darkYellow));
        scene()->addItem(m_item);
        m_state = 1;
    }
    else if (m_state == 1) {
        setP2(event->pos());
        m_item = nullptr;
        m_state = 0;
    }
    event->accept();
}

void GraphicsRectTool::mouseMoveEvent(QMouseEvent *event)
{
    if (m_state == 1) {
        setP2(event->pos());
    }
    event->accept();
}

void GraphicsRectTool::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (m_state == 1) {
        // TODO: use command stack
    }
}

QDialog *GraphicsRectTool::optionDialog()
{
    return new GraphicsLineToolDialog();
}

QString GraphicsRectTool::toolGroup() const
{
    return "interactive-tools";
}

QAction *GraphicsRectTool::action() const
{
    return new QAction(QIcon(":/icons/graphicsrecttool.svg"),
                                  "Place a rectangle", nullptr);
}

void GraphicsRectTool::cancel()
{
    if (m_state == 0) {
        emit finished();
    }
    else if (m_state == 1) {
        scene()->removeItem(m_item);
        delete m_item;
        m_item = nullptr;
        m_state = 0;
    }
}

void GraphicsRectTool::setP1(const QPoint &point)
{
    QPointF pos = m_item->mapFromScene(view()->mapToScene(point));
    m_item->moveControlPoint(m_item->controlPoints()[0], pos);
}

void GraphicsRectTool::setP2(const QPoint &point)
{
    QPointF pos = m_item->mapFromScene(view()->mapToScene(point));
    m_item->moveControlPoint(m_item->controlPoints()[4], pos);
}


void GraphicsRectTool::activate()
{
}

void GraphicsRectTool::desactivate()
{
}
