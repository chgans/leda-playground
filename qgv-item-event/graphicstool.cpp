#include "graphicstool.h"
#include "graphicsscene.h"
#include "graphicsview.h"
#include "graphicsobject.h"
#include <QGraphicsItem>

#include <QGraphicsOpacityEffect>

#include <QMouseEvent>
#include <QKeyEvent>
#include <QDialog>

GraphicsTool::GraphicsTool(QObject *parent) :
    QObject(parent),
    m_view(nullptr)
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
    switch (event->key()) {
    case Qt::Key_Escape:
        cancel();
        break;
    case Qt::Key_Tab:
        QDialog *dialog = optionDialog();
        if (dialog)
            dialog->exec();
        break;
    }
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

// TODO: the original should be dimmed out instead
GraphicsObject *GraphicsTool::createPhantomItem(GraphicsObject *item)
{
    GraphicsObject *phantomItem = item->clone();
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
    effect->setEnabled(true);
    phantomItem->setGraphicsEffect(effect);
    scene()->addItem(phantomItem);
    phantomItem->setSelected(false); // force deselected
    return phantomItem;
}

QList<GraphicsObject *> GraphicsTool::createPhantomItems(const QList<GraphicsObject *> &items)
{
    QList<GraphicsObject *> phantomItems;
    foreach (GraphicsObject *item, items) {
        phantomItems.append(createPhantomItem(item));
    }
    return phantomItems;
}
