#include "objectpreview.h"

#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>

#include <QDebug>

ObjectPreview::ObjectPreview(QWidget *parent) :
    QGraphicsView(parent)
{
    //setAttribute(Qt::WA_TransparentForMouseEvents, true);
    setInteractive(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setOptimizationFlag(QGraphicsView::IndirectPainting, true);
    m_object = 0;
}

void ObjectPreview::setObjectToPreview(QGraphicsItem *item)
{
    //setSceneRect(item->mapRectToScene(item->boundingRect()).adjusted(-25, -25, 25, 25));
    QRectF rect = item->mapRectToScene(item->boundingRect()).adjusted(-25, -25, 25, 25);
    setSceneRect(scene()->sceneRect());
    fitInView(rect, Qt::KeepAspectRatio);
    centerOn(rect.center());
    m_object = item;
}

// Instead use standard view, which has higlight feature: grey out other items by
// painting a mask in drawForeground
void ObjectPreview::drawItems(QPainter *painter,
                              int numItems,
                              QGraphicsItem *items[],
                              const QStyleOptionGraphicsItem options[])
{
    for (int i = 0; i < numItems; ++i) {
        if (items[i] != m_object)
            continue;
        QGraphicsView::drawItems(painter, 1,
                                 &items[i], &options[i]);
        return;
    }
}

void ObjectPreview::resizeEvent(QResizeEvent *event)
{
    if (m_object) {
        setObjectToPreview(m_object);
    }
}
