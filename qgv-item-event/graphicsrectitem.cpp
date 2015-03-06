#include "graphicsrectitem.h"
#include "graphicscontrolpoint.h"
#include <QBrush>
#include <QPen>
#include <QRectF>

GraphicsRectItem::GraphicsRectItem(QGraphicsItem *parent):
    QGraphicsRectItem(parent)
{

}

GraphicsRectItem::~GraphicsRectItem()
{

}

QGraphicsItem *GraphicsRectItem::clone()
{
    GraphicsRectItem *item = new GraphicsRectItem();
    item->setPos(pos());
    item->setBrush(brush());
    item->setPen(pen());
    item->setRect(rect());
    item->setFlags(flags());
    item->setSelected(isSelected());
    return item;
}

void GraphicsRectItem::controlPointMoved(const GraphicsControlPoint *point)
{

}

