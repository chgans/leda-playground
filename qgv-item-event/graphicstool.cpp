#include "graphicstool.h"
#include "graphicsscene.h"
#include "graphicsview.h"
#include "graphicsrectitem.h"

#include <QGraphicsOpacityEffect>

GraphicsTool::GraphicsTool(QObject *parent) : QObject(parent)
{

}

GraphicsTool::~GraphicsTool()
{

}

GraphicsView *GraphicsTool::view()
{
    return m_view;
}

GraphicsScene *GraphicsTool::scene()
{
    return m_view->scene();
}

void GraphicsTool::setView(GraphicsView *view)
{
    m_view = view;
}

void GraphicsTool::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void GraphicsTool::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void GraphicsTool::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void GraphicsTool::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void GraphicsTool::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

void GraphicsTool::keyReleaseEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

void GraphicsTool::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event);
}

void GraphicsTool::cancel()
{

}

QGraphicsItem *GraphicsTool::createPhantomItem(QGraphicsItem *item)
{
    GraphicsRectItem *origItem = static_cast<GraphicsRectItem *>(item);
    GraphicsRectItem *phantomItem = origItem->clone();
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
    effect->setEnabled(true);
    phantomItem->setGraphicsEffect(effect);
    scene()->addItem(phantomItem);
    phantomItem->setSelected(false); // force deselected
    return phantomItem;
}

QList<QGraphicsItem *> GraphicsTool::createPhantomItems(QList<QGraphicsItem *> items)
{
    QList<QGraphicsItem *> phantomItems;
    foreach (QGraphicsItem *item, items) {
        phantomItems.append(createPhantomItem(item));
    }
    return phantomItems;
}
