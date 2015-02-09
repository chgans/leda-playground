#include "documentgraphicsitem.h"
#include <QJsonArray>


DocumentGraphicsItem::DocumentGraphicsItem():
    position(QPointF(0, 0)), zValue(0), rotation(0),
    mirroredX(false), mirroredY(false), locked(false),
    visible(true), opacity(1.0)
{

}

DocumentGraphicsItem::~DocumentGraphicsItem()
{

}

bool DocumentGraphicsItem::fromJSon(const QJsonObject &json)
{
    return false;
}

QJsonObject DocumentGraphicsItem::toJson() const
{
    QJsonObject obj = DocumentItem::toJson();
    obj.insert("x", position.x());
    obj.insert("y", position.y());
    obj.insert("z", zValue);
    obj.insert("rotation", rotation);
    obj.insert("mirroredX", mirroredX);
    obj.insert("mirroredY", mirroredY);
    obj.insert("locked", locked);
    obj.insert("visible", visible);
    obj.insert("opacity", opacity);
    return obj;
}



bool DocumentGraphicsItemGroup::fromJSon(const QJsonObject &json)
{
    return false;
}

QJsonObject DocumentGraphicsItemGroup::toJson() const
{
    QJsonObject obj = DocumentGraphicsItem::toJson();
    QJsonArray list;
    foreach (DocumentGraphicsItem *item, items)
        list.append(item->toJson());
    obj.insert("items", list);
    return obj;
}


bool DocumentItem::fromJSon(const QJsonObject &json)
{
    return false;
}

QJsonObject DocumentItem::toJson() const
{
    return QJsonObject();
}


DocumentGraphicsParameterItem::DocumentGraphicsParameterItem():
    showName(false), showValue(true)
{

}

DocumentGraphicsParameterItem::~DocumentGraphicsParameterItem()
{

}

bool DocumentGraphicsParameterItem::fromJSon(const QJsonObject &json)
{
    return false;
}

QJsonObject DocumentGraphicsParameterItem::toJson() const
{
    QJsonObject obj = DocumentGraphicsItem::toJson();
    obj.insert("name", name);
    obj.insert("value", value);
    obj.insert("showName", showName);
    obj.insert("showValue", showValue);
    return obj;
}
