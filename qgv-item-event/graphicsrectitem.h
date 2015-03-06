#ifndef GRAPHICSRECTITEM_H
#define GRAPHICSRECTITEM_H

#include <QGraphicsRectItem>
#include  "graphicsiteminterface.h"

class GraphicsRectItem : public QGraphicsRectItem, public GraphicsItemInterface
{
public:
    GraphicsRectItem(QGraphicsItem *parent = 0);
    ~GraphicsRectItem();

    virtual QGraphicsItem *clone();
    void controlPointMoved(const GraphicsControlPoint *point);
};

#endif // GRAPHICSRECTITEM_H
