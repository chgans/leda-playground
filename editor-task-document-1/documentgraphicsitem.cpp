#include "documentgraphicsitem.h"
#include <QJsonArray>

// TODO: move to a factory thingy
DocumentGraphicsItem *createGraphicsItem(const QJsonObject &obj)
{
    QString typeName = obj.value("type").toString();
    DocumentGraphicsItem *item = 0;
    if (typeName == "graphicsItemGroup")
        item = new DocumentGraphicsItemGroup;
    else if (typeName == "graphicsParameterItem")
        item = new DocumentGraphicsParameterItem;
    else if (typeName == "graphicsRectangleItem")
        item = new DocumentGraphicsRectangleItem;
    if (item)
        item->fromJson(obj);
    return item;
}


DocumentItem::DocumentItem()
{

}

DocumentItem::~DocumentItem()
{

}

void DocumentItem::fromJson(const QJsonObject &obj)
{
}

void DocumentItem::toJson(QJsonObject &obj) const
{
}


DocumentGraphicsItem::DocumentGraphicsItem():
    position(QPointF(0, 0)), zValue(0), rotation(0),
    mirroredX(false), mirroredY(false), locked(false),
    visible(true), opacity(1.0)
{

}

DocumentGraphicsItem::~DocumentGraphicsItem()
{

}

void DocumentGraphicsItem::fromJson(const QJsonObject &obj)
{
    DocumentItem::fromJson(obj);
    position.setX(obj.value("x").toDouble(0.0));
    position.setY(obj.value("y").toDouble(0.0));
    zValue = obj.value("z").toDouble(0.0);
    rotation = obj.value("rotation").toDouble(0.0);
    mirroredX = obj.value("mirroredX").toBool(false);
    mirroredY = obj.value("mirroredY").toBool(false);
    locked = obj.value("locked").toBool(false);
    visible = obj.value("visible").toBool(true);
    opacity = obj.value("opacity").toDouble(1.0);
}

void DocumentGraphicsItem::toJson(QJsonObject &obj) const
{
    DocumentItem::toJson(obj);
    obj.insert("type", "graphicsItem");
    obj.insert("x", position.x());
    obj.insert("y", position.y());
    obj.insert("z", zValue);
    obj.insert("rotation", rotation);
    obj.insert("mirroredX", mirroredX);
    obj.insert("mirroredY", mirroredY);
    obj.insert("locked", locked);
    obj.insert("visible", visible);
    obj.insert("opacity", opacity);
}

void DocumentGraphicsItemGroup::fromJson(const QJsonObject &obj)
{
    qDeleteAll(items);
    items.clear();
    QJsonArray list = obj.value("items").toArray();
    foreach (QJsonValue valItem, list) {
        QJsonObject objItem = valItem.toObject();
        DocumentGraphicsItem *item;
        item = createGraphicsItem(objItem);
        items.append(item);
    }
}

void DocumentGraphicsItemGroup::toJson(QJsonObject &obj) const
{
    DocumentGraphicsItem::toJson(obj);
    obj.insert("type", "graphicsItemGroup");
    QJsonArray list;
    foreach (DocumentGraphicsItem *item, items) {
        QJsonObject objItem;
        item->toJson(objItem);
        list.append(objItem);
    }
    obj.insert("items", list);
}

DocumentGraphicsParameterItem::DocumentGraphicsParameterItem():
    showName(false), showValue(true)
{

}

DocumentGraphicsParameterItem::~DocumentGraphicsParameterItem()
{

}

void DocumentGraphicsParameterItem::fromJson(const QJsonObject &obj)
{
    DocumentGraphicsItem::fromJson(obj);
    name = obj.value("name").toString();
    value = obj.value("value").toString();
    showName = obj.value("showName").toBool(false);
    showValue = obj.value("showValue").toBool(true);
}

void DocumentGraphicsParameterItem::toJson(QJsonObject &obj) const
{
    DocumentGraphicsItem::toJson(obj);
    obj.insert("type", "graphicsParameterItem");
    obj.insert("name", name);
    obj.insert("value", value);
    obj.insert("showName", showName);
    obj.insert("showValue", showValue);
}


DocumentGraphicsAbstractShapeItem::DocumentGraphicsAbstractShapeItem()
{

}

DocumentGraphicsAbstractShapeItem::~DocumentGraphicsAbstractShapeItem()
{

}

void DocumentGraphicsAbstractShapeItem::fromJson(const QJsonObject &obj)
{
    DocumentGraphicsItem::fromJson(obj);
    QJsonObject penObj = obj.value("pen").toObject();
    QJsonObject brushObj = obj.value("brush").toObject();
    pen.setColor(QColor(penObj.value("color").toString("#FF000000")));
    pen.setWidth(penObj.value("width").toInt(0)); // cosmetic
    pen.setStyle((Qt::PenStyle)penObj.value("style").toInt(Qt::SolidLine));
    pen.setCapStyle((Qt::PenCapStyle)penObj.value("cap").toInt(Qt::SquareCap));
    pen.setJoinStyle((Qt::PenJoinStyle)penObj.value("join").toInt(Qt::BevelJoin));
    brush.setColor(brushObj.value("color").toString("#FF000000"));
    brush.setStyle((Qt::BrushStyle)brushObj.value("style").toInt(Qt::SolidPattern));
}

void DocumentGraphicsAbstractShapeItem::toJson(QJsonObject &obj) const
{
    DocumentGraphicsItem::toJson(obj);
    obj.insert("type", "graphicsAbstractShapeItem");
    QJsonObject penObj;
    penObj.insert("color", pen.color().name(QColor::HexArgb));
    penObj.insert("width", pen.width());
    penObj.insert("style", pen.style());
    penObj.insert("cap", pen.capStyle());
    penObj.insert("join", pen.joinStyle());
    QJsonObject brushObj;
    brushObj.insert("color", brush.color().name(QColor::HexArgb));
    brushObj.insert("style", brush.style()); // No support for gradient and texture
    obj.insert("pen", penObj);
    obj.insert("brush", brushObj);
}


DocumentGraphicsRectangleItem::DocumentGraphicsRectangleItem():
    topLeftCorner(0, 0), width(0), height(0)
{

}

DocumentGraphicsRectangleItem::~DocumentGraphicsRectangleItem()
{

}

void DocumentGraphicsRectangleItem::fromJson(const QJsonObject &obj)
{
    DocumentGraphicsAbstractShapeItem::fromJson(obj);
    topLeftCorner.setX(obj.value("x1").toDouble(0.0));
    topLeftCorner.setY(obj.value("y1").toDouble(0.0));
    width = obj.value("width").toDouble(0.0);
    height = obj.value("height").toDouble(0.0);
}

void DocumentGraphicsRectangleItem::toJson(QJsonObject &obj) const
{
    DocumentGraphicsAbstractShapeItem::toJson(obj);
    obj.insert("type", "graphicsRectangleItem");
    obj.insert("x1", topLeftCorner.x());
    obj.insert("y1", topLeftCorner.y());
    obj.insert("width", width);
    obj.insert("height", height);
}


DocumentGraphicsPinItem::DocumentGraphicsPinItem():
    showLabel(true), showDesignator(true), length(5)
{

}

DocumentGraphicsPinItem::~DocumentGraphicsPinItem()
{

}

void DocumentGraphicsPinItem::fromJson(const QJsonObject &obj)
{
    DocumentGraphicsItem::fromJson(obj);
    label = obj.value("label").toString();
    designator = obj.value("designator").toString();
    showDesignator = obj.value("showDesignator").toBool(true);
    showLabel = obj.value("showLabel").toBool(true);
    electricalType = (Le::ElectricalPortType)obj.value("electricalType").toInt(Le::UnspecifiedPortType);
    decoration = (Le::GraphicalPortDecoration)obj.value("decoration").toInt(Le::NoPortDecoration);
    length = obj.value("length").toDouble(5);
}

void DocumentGraphicsPinItem::toJson(QJsonObject &obj) const
{
    DocumentGraphicsItem::toJson(obj);
    obj.insert("label", label);
    obj.insert("designator", designator);
    obj.insert("showLabel", showLabel);
    obj.insert("showDesignator", showDesignator);
    obj.insert("length", length);
    obj.insert("decoration", decoration);
    obj.insert("electricalType", electricalType);
}
