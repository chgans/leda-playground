#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <QGraphicsObject>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>
#include <QGraphicsTextItem>
#include <QFont>
#include <QCursor>

#include <QDebug>

class IDrawingItem {
public:
    IDrawingItem();
    virtual ~IDrawingItem();

};

Q_DECLARE_INTERFACE(IDrawingItem, "leda.IGraphicsObject")

template<class DerivedFromQGraphicsObject>
class DrawingItem: public DerivedFromQGraphicsObject, public IDrawingItem
{

    Q_INTERFACES(IDrawingItem)

public:
    DrawingItem(QGraphicsItem *parent = 0):
        DerivedFromQGraphicsObject(parent)
    {
        DerivedFromQGraphicsObject::setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    }

    ~DrawingItem() {}

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
    {
        DerivedFromQGraphicsObject::mouseDoubleClickEvent(event);
    }

    virtual void itemChanged() = 0;

protected:
    virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
    {
        QVariant val = DerivedFromQGraphicsObject::itemChange(change, value);
        if (val != value)
            itemChanged();
        return val;
    }
};







#endif // GRAPHICSOBJECT_H
