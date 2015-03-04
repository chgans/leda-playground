#ifndef GRAPHICSRECTITEM_H
#define GRAPHICSRECTITEM_H

#include <QGraphicsRectItem>

class GraphicsRectItem : public QGraphicsRectItem
{
public:
    GraphicsRectItem(QGraphicsItem *parent = 0);
    ~GraphicsRectItem();

    GraphicsRectItem *clone();
};

#endif // GRAPHICSRECTITEM_H
