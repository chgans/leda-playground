#include "graphicslinetool.h"
#include "graphicslinetooldialog.h"
#include "graphicslineitem.h"
#include "graphicshandle.h"
#include "graphicsscene.h"
#include "graphicsview.h"

#include <QMouseEvent>
#include <QAction>

#include <QDebug>

GraphicsLineTool::GraphicsLineTool(QObject *parent):
    GraphicsTool(parent), m_state(0), m_item(nullptr)
{

}



void GraphicsLineTool::mousePressEvent(QMouseEvent *event)
{
    if (m_state == 0) {
        Q_ASSERT(m_item == nullptr);
        m_item = new GraphicsLineItem();
        m_item->setFlags(QGraphicsItem::ItemIsMovable |
                         QGraphicsItem::ItemIsSelectable);
        QPointF scenePos = view()->mapToScene(event->pos());
        m_item->setPos(scenePos);
        setP1(event->pos());
        setP2(event->pos());
        m_item->setPen(QPen(Qt::darkMagenta));
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

void GraphicsLineTool::mouseMoveEvent(QMouseEvent *event)
{
    if (m_state == 1) {
        setP2(event->pos());
    }
    event->accept();
}

void GraphicsLineTool::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (m_state == 1) {
        // TODO: use command stack
    }
}

QDialog *GraphicsLineTool::optionDialog()
{
    return new GraphicsLineToolDialog();
}

QString GraphicsLineTool::toolGroup() const
{
    return "interactive-tools";
}

QAction *GraphicsLineTool::action() const
{
    return new QAction(QIcon(":/icons/graphicslinetool.svg"),
                                  "Place a line", nullptr);
}

void GraphicsLineTool::cancel()
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

void GraphicsLineTool::setP1(const QPoint &handlePos)
{
    QPointF pos = m_item->mapFromScene(view()->mapToScene(handlePos));
    m_item->moveHandle(m_item->handles()[0], pos);
}

void GraphicsLineTool::setP2(const QPoint &handlePos)
{
    QPointF pos = m_item->mapFromScene(view()->mapToScene(handlePos));
    m_item->moveHandle(m_item->handles()[1], pos);
}


void GraphicsLineTool::activate()
{
}

void GraphicsLineTool::desactivate()
{
}
