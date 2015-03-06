#include "graphicslinetool.h"
#include "graphicslinetooldialog.h"
#include "graphicslineitem.h"
#include "graphicscontrolpoint.h"
#include "graphicsscene.h"
#include "graphicsview.h"

#include <QMouseEvent>
#include <QAction>

#include <QDebug>

GraphicsLineTool::GraphicsLineTool(QObject *parent):
    GraphicsTool(parent), m_state(0), m_line(nullptr)
{

}



void GraphicsLineTool::mousePressEvent(QMouseEvent *event)
{
    if (m_state == 0) {
        Q_ASSERT(m_line == nullptr);
        m_line = new GraphicsLineItem();
        m_line->setFlags(QGraphicsItem::ItemIsMovable |
                         QGraphicsItem::ItemIsSelectable);
        m_line->setPos(0, 0);
        setP1(event->pos());
        setP2(event->pos());
        m_line->setPen(QPen(Qt::darkMagenta));
        scene()->addItem(m_line);
        m_state = 1;
    }
    else if (m_state == 1) {
        setP2(event->pos());
        m_line = nullptr;
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
    return new QAction(QIcon(":/icons/graphics_line.svg"),
                                  "Place a line", nullptr);
}

void GraphicsLineTool::cancel()
{
    if (m_state == 0) {
        emit finished();
    }
    else if (m_state == 1) {
        scene()->removeItem(m_line);
        delete m_line;
        m_line = nullptr;
        m_state = 0;
    }
}

void GraphicsLineTool::setP1(const QPoint &point)
{
    QPointF pos = m_line->mapFromScene(view()->mapToScene(point));
//    QLineF lineF = m_line->line();
//    lineF.setP1(pos);
//    m_line->setLine(lineF);
    m_line->controlPoints().value(0)->setScenePos(pos);
}

void GraphicsLineTool::setP2(const QPoint &point)
{
    QPointF pos = m_line->mapFromScene(view()->mapToScene(point));
//    QLineF lineF = m_line->line();
//    lineF.setP2(pos);
//    m_line->setLine(lineF);
    m_line->controlPoints().value(1)->setScenePos(pos);
}
