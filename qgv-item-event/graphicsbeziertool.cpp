#include "graphicsbeziertool.h"
#include "graphicsbezieritem.h"
#include "graphicscontrolpoint.h"
#include "graphicsview.h"
#include "graphicsscene.h"

#include <QMouseEvent>

#include <QAction>
#include <QIcon>

#include <QDebug>

GraphicsBezierTool::GraphicsBezierTool(QObject *parent):
    GraphicsTool(parent),
    m_state(NotStarted),
    m_item(nullptr),
    m_insertPointOnMouseMove(false)
{

}

QPointF GraphicsBezierTool::mapToScene(const QPoint &pos)
{
    if (view() == nullptr)
        return QPointF();
    return view()->mapToScene(pos);
}

QPointF GraphicsBezierTool::mapToItem(const QPoint &pos)
{
    if (m_item == nullptr)
        return QPointF();
    return m_item->mapFromScene(mapToScene(pos));
}

QPointF GraphicsBezierTool::mapFromScene(const QPointF &pos)
{
    if (view() == nullptr)
        return QPointF();
    return view()->mapFromScene(pos);
}

QPointF GraphicsBezierTool::mapFromItem(const QPointF &pos)
{
    if (m_item == nullptr)
        return QPointF();
    return m_item->mapToScene(mapFromScene(pos));
}

void GraphicsBezierTool::setState(GraphicsBezierTool::State state)
{
    qDebug() << m_state << "=>" << state;
    m_state = state;
}

void GraphicsBezierTool::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }

    qDebug() << "MPE";
    switch (m_state) {
    case NotStarted:
        scene()->clearSelection();
        m_nodePos = event->pos();
        setState(FirstPoint);
        break;
    case MidPoints:
        m_nodePos = event->pos();
        break;
    default:
        qWarning() << "Inconsistent state in MPE!";
        break;
    }

    event->accept();
}

void GraphicsBezierTool::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::LeftButton)) {
        return; // Do nothing if dragging
    }

    switch (m_state) {
    case NotStarted:
        // Nothing to do
        break;
    case FirstPoint:
        m_nodePos = event->pos();
        if (m_insertPointOnMouseMove) { // TODO: check for minimum view distance, 10 pixels?
            m_item->addPoint(mapToItem(m_nodePos));
            m_insertPointOnMouseMove = false;
            setState(MidPoints);
        }
        break;
    case MidPoints: {
        m_nodePos = event->pos();
        if (m_insertPointOnMouseMove) { // Same as above
            m_item->addPoint(mapToItem(m_nodePos));
            m_insertPointOnMouseMove = false;
        }
        int idx = m_item->controlPoints().count() - 1;
        const GraphicsControlPoint *p = m_item->controlPoints().value(idx);
        m_item->moveControlPoint(p, mapToItem(m_nodePos));
        break;
    }
    default:
        qWarning() << "Inconsistent state in MVE!";
        break;
    }

    event->accept();
}

void GraphicsBezierTool::mouseReleaseEvent(QMouseEvent *event)
{
    // FIXME: track which button was pressed!
    //if (!event->buttons().testFlag(Qt::LeftButton)) {
    //    return;
    //}

    qDebug() << "MRE";
    switch (m_state) {
    case NotStarted:
        break;
    case FirstPoint:
        m_item = new GraphicsBezierItem();
        m_item->setPos(mapToScene(event->pos()));
        m_item->setPen(QPen(QBrush(Qt::darkCyan), 2));
        m_item->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
        m_item->setPos(mapToScene(m_nodePos));
        m_item->addPoint(mapToItem(m_nodePos));
        m_item->setSelected(true);
        scene()->addItem(m_item);
        m_insertPointOnMouseMove = true;
        break;
    case MidPoints:
        m_insertPointOnMouseMove = true;
        break;
    case LastPoint:
        //m_item->removePoint(m_item->pointCount()-1);
        m_item->setSelected(true);
        m_item = nullptr;
        setState(NotStarted);
        break;
    default:
        qWarning() << "Inconsistent state in MRE!";
        break;
    }

    event->accept();
}


void GraphicsBezierTool::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (!event->buttons().testFlag(Qt::LeftButton)) {
        return;
    }

    qDebug() << "MDCE";
    switch (m_state) {
    case FirstPoint:
        scene()->removeItem(m_item);
        delete m_item;
        m_item = nullptr;
        setState(NotStarted);
        break;
    case MidPoints:
        // TODO: check the edge case when user just double click:
        // => only 2 points with almost same pos
        setState(LastPoint);
        break;
    default:
        qWarning() << "Inconsistent state MDCE!";
        break;
    }

    event->accept();
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

