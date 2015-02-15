#include "documentitems.h"
#include <QJsonArray>


#if 0
// TODO: move to a factory thingy
DocumentGraphicsItem *createGraphicsItem(const QJsonValue &val)
{
    QString typeName = val.toObject().value("type").toString();
    DocumentGraphicsItem *item = 0;
    if (typeName == "graphicsItemGroup")
        item = new DocumentGraphicsItemGroup;
    else if (typeName == "graphicsParameterItem")
        item = new DocumentGraphicsParameterItem;
    else if (typeName == "graphicsRectangleItem")
        item = new DocumentGraphicsRectangleItem;
    if (item)
        item->fromJsonValue(val);
    return item;
}


DocumentItem::DocumentItem()
{

}

DocumentItem::~DocumentItem()
{

}

void DocumentItem::fromJsonValue(const QJsonValue &val)
{
}

void DocumentItem::toJsonValue(QJsonValue &val) const
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

void DocumentGraphicsItem::fromJsonValue(const QJsonValue &val)
{
    DocumentItem::fromJsonValue(val);
    QJsonObject obj = val.toObject();
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

void DocumentGraphicsItem::toJsonValue(QJsonValue &val) const
{
    DocumentItem::toJsonValue(val);
    QJsonObject obj = val.toObject();
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
    val = QJsonValue(obj);
}

void DocumentGraphicsItemGroup::fromJsonValue(const QJsonValue &val)
{
    qDeleteAll(items); // ?
    items.clear();
    QJsonObject obj = val.toObject();
    items.fromJsonValue(obj.value("items"));
}

void DocumentGraphicsItemGroup::toJsonValue(QJsonValue &val) const
{
    DocumentGraphicsItem::toJsonValue(val);
    QJsonObject obj = val.toObject();
    obj.insert("type", "graphicsItemGroup");
    QJsonValue itemVals;
    items.toJsonValue(itemVals);
    obj.insert("items", itemVals);
    val = QJsonValue(obj);
}

DocumentGraphicsParameterItem::DocumentGraphicsParameterItem():
    showName(false), showValue(true)
{

}

DocumentGraphicsParameterItem::~DocumentGraphicsParameterItem()
{

}

void DocumentGraphicsParameterItem::fromJsonValue(const QJsonValue &val)
{
    DocumentGraphicsItem::fromJsonValue(val);
    QJsonObject obj = val.toObject();
    name = obj.value("name").toString();
    value = obj.value("value").toString();
    showName = obj.value("showName").toBool(false);
    showValue = obj.value("showValue").toBool(true);
}

void DocumentGraphicsParameterItem::toJsonValue(QJsonValue &val) const
{
    DocumentGraphicsItem::toJsonValue(val);
    QJsonObject obj = val.toObject();
    obj.insert("type", "graphicsParameterItem");
    obj.insert("name", name);
    obj.insert("value", value);
    obj.insert("showName", showName);
    obj.insert("showValue", showValue);
    val = QJsonValue(obj);
}


DocumentGraphicsAbstractShapeItem::DocumentGraphicsAbstractShapeItem()
{

}

DocumentGraphicsAbstractShapeItem::~DocumentGraphicsAbstractShapeItem()
{

}

void DocumentGraphicsAbstractShapeItem::fromJsonValue(const QJsonValue &val)
{
    DocumentGraphicsItem::fromJsonValue(val);
    QJsonObject obj = val.toObject();
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

void DocumentGraphicsAbstractShapeItem::toJsonValue(QJsonValue &val) const
{
    DocumentGraphicsItem::toJsonValue(val);
    QJsonObject obj = val.toObject();
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
    val = QJsonValue(obj);
}


DocumentGraphicsRectangleItem::DocumentGraphicsRectangleItem():
    topLeftCorner(0, 0), width(0), height(0)
{

}

DocumentGraphicsRectangleItem::~DocumentGraphicsRectangleItem()
{

}

void DocumentGraphicsRectangleItem::fromJsonValue(const QJsonValue &val)
{
    DocumentGraphicsAbstractShapeItem::fromJsonValue(val);
    QJsonObject obj = val.toObject();
    topLeftCorner.setX(obj.value("x1").toDouble(0.0));
    topLeftCorner.setY(obj.value("y1").toDouble(0.0));
    width = obj.value("width").toDouble(0.0);
    height = obj.value("height").toDouble(0.0);
}

void DocumentGraphicsRectangleItem::toJsonValue(QJsonValue &val) const
{
    DocumentGraphicsAbstractShapeItem::toJsonValue(val);
    QJsonObject obj = val.toObject();
    obj.insert("type", "graphicsRectangleItem");
    obj.insert("x1", topLeftCorner.x());
    obj.insert("y1", topLeftCorner.y());
    obj.insert("width", width);
    obj.insert("height", height);
    val = QJsonValue(obj);
}


DocumentGraphicsPinItem::DocumentGraphicsPinItem():
    showLabel(true), showDesignator(true), length(5)
{

}

DocumentGraphicsPinItem::~DocumentGraphicsPinItem()
{

}

void DocumentGraphicsPinItem::fromJsonValue(const QJsonValue &val)
{
    DocumentGraphicsItem::fromJsonValue(val);
    QJsonObject obj = val.toObject();
    label = obj.value("label").toString();
    designator = obj.value("designator").toString();
    showDesignator = obj.value("showDesignator").toBool(true);
    showLabel = obj.value("showLabel").toBool(true);
    electricalType = (Le::ElectricalPortType)obj.value("electricalType").toInt(Le::UnspecifiedPortType);
    decoration = (Le::GraphicalPortDecoration)obj.value("decoration").toInt(Le::NoPortDecoration);
    length = obj.value("length").toDouble(5);
}

void DocumentGraphicsPinItem::toJsonValue(QJsonValue &val) const
{
    DocumentGraphicsItem::toJsonValue(val);
    QJsonObject obj = val.toObject();
    obj.insert("label", label);
    obj.insert("designator", designator);
    obj.insert("showLabel", showLabel);
    obj.insert("showDesignator", showDesignator);
    obj.insert("length", length);
    obj.insert("decoration", decoration);
    obj.insert("electricalType", electricalType);
    val = QJsonValue(obj);
}




DocumentParameterItem::DocumentParameterItem():
    visible(true)
{

}

DocumentParameterItem::~DocumentParameterItem()
{

}

void DocumentParameterItem::fromJsonValue(const QJsonValue &val)
{
    DocumentItem::fromJsonValue(val);
    QJsonObject obj = val.toObject();
    visible = obj.value("visible").toBool(true);
    name = obj.value("name").toString();
    value = obj.value("value").toString();
}

void DocumentParameterItem::toJsonValue(QJsonValue &val) const
{
    DocumentItem::toJsonValue(val);
    QJsonObject obj = val.toObject();
    obj.insert("visible", visible);
    obj.insert("name", name);
    obj.insert("value", value);
    val = QJsonValue(obj);
}


DocumentSymbolItem::DocumentSymbolItem()
{

}

DocumentSymbolItem::~DocumentSymbolItem()
{

}

void DocumentSymbolItem::fromJsonValue(const QJsonValue &val)
{
    DocumentItem::fromJsonValue(val);
    QJsonObject obj = val.toObject();
    parameters.fromJsonValue(obj.value("parameters"));
    ports.fromJsonValue(obj.value("ports"));
    drawingItems.fromJsonValue(obj.value("drawingItems"));
}

void DocumentSymbolItem::toJsonValue(QJsonValue &val) const
{
    DocumentItem::toJsonValue(val);
    QJsonObject obj = val.toObject();
    QJsonValue valList;
    parameters.toJsonValue(valList);
    obj.insert("properties", valList);
    ports.toJsonValue(valList);
    obj.insert("ports", valList);
    drawingItems.toJsonValue(valList);
    obj.insert("drawingItems", valList);
    val = QJsonValue(obj);
}


DocumentPortItem::DocumentPortItem():
    electricalType(Le::UnspecifiedPortType)
{

}

DocumentPortItem::~DocumentPortItem()
{

}

void DocumentPortItem::fromJsonValue(const QJsonValue &val)
{
    DocumentItem::fromJsonValue(val);
    QJsonObject obj = val.toObject();
    label = obj.value("label").toString();
    designator = obj.value("designator").toString();
    electricalType = (Le::ElectricalPortType)obj.value("electricalType").toInt(Le::UnspecifiedPortType);
}

void DocumentPortItem::toJsonValue(QJsonValue &val) const
{
    DocumentItem::toJsonValue(val);
    QJsonObject obj = val.toObject();
    obj.insert("label", label);
    obj.insert("designator", designator);
    obj.insert("electricalType", electricalType);
    val = QJsonValue(obj);
}


DocumentParameterItem::DocumentParameterItem()
{

}

#endif

DocumentItem::DocumentItem(DocumentItem *parent, const QString &name, const QVariant &value):
    m_parent(parent), m_name(name), m_value(value)
{
    if (m_parent)
        m_parent->m_children.append(this);
}

DocumentItem::~DocumentItem()
{

}


DocumentParameterItem::DocumentParameterItem(DocumentItem *parent):
    DocumentItem(parent, "parameter")
{
    m_visible = new DocumentItem(this, "visible", true);
    m_name = new DocumentItem(this, "name", "");
    m_value = new DocumentItem(this, "value");
}

DocumentParameterItem::~DocumentParameterItem()
{

}

DocumentSymbolItem::DocumentSymbolItem(DocumentItem *parent):
    DocumentItem(parent, "symbol")
{
    m_symbolName = new DocumentItem(this, "name", "");
    m_description = new DocumentItem(this, "description", "");
    m_designator = new DocumentItem(this, "designator", "");
    m_parameters = new DocumentParameterItemList(this, "parameters");
}

DocumentSymbolItem::~DocumentSymbolItem()
{

}



DocumentSymbolCollectionItem::DocumentSymbolCollectionItem(DocumentItem *parent):
    DocumentItem(parent, "symbolCollection")
{

    m_creator = new DocumentItem(this, "creator", "");
    m_license = new DocumentItem(this, "license", "");
    m_symbols = new DocumentSymbolItemList(this, "symbols");
}

DocumentSymbolCollectionItem::~DocumentSymbolCollectionItem()
{

}
