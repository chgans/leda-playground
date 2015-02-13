#ifndef DOCUMENTGRAPHICSITEM_H
#define DOCUMENTGRAPHICSITEM_H

#include <QPointF>
#include <QPen>
#include <QBrush>
#include <QJsonObject>

#include <leda.h>

class DocumentItem
{
public:
    explicit DocumentItem();
    virtual ~DocumentItem();

    virtual void fromJson(const QJsonObject &obj);
    virtual void toJson(QJsonObject &obj) const;
};

class DocumentGraphicsItem: public DocumentItem
{
public:
    explicit DocumentGraphicsItem();
    virtual ~DocumentGraphicsItem();

    virtual void fromJson(const QJsonObject &obj);
    virtual void toJson(QJsonObject &obj) const;

    QPointF position;
    qreal zValue;
    qreal rotation;
    bool mirroredX;
    bool mirroredY;
    bool locked;
    bool visible;
    qreal opacity;
};

// TODO: move to a factory thingy
DocumentGraphicsItem *createGraphicsItem(const QJsonObject &obj);

class DocumentGraphicsItemGroup: public DocumentGraphicsItem
{
public:
    explicit DocumentGraphicsItemGroup() {}
    virtual ~DocumentGraphicsItemGroup() {}

    virtual void fromJson(const QJsonObject &obj);
    virtual void toJson(QJsonObject &obj) const;

    QList<DocumentGraphicsItem*> items;
};

class DocumentGraphicsParameterItem: public DocumentGraphicsItem
{
public:
    explicit DocumentGraphicsParameterItem();
    virtual ~DocumentGraphicsParameterItem();

    virtual void fromJson(const QJsonObject &obj);
    virtual void toJson(QJsonObject &obj) const;

    QString name;
    QString value;
    bool showName;
    bool showValue;
};


class DocumentGraphicsPinItem: DocumentGraphicsItem
{
public:
    explicit DocumentGraphicsPinItem();
    virtual ~DocumentGraphicsPinItem();

    virtual void fromJson(const QJsonObject &obj);
    virtual void toJson(QJsonObject &obj) const;

    QString label;
    QString designator;
    bool showLabel;
    bool showDesignator;
    Le::ElectricalPortType electricalType;
    Le::GraphicalPortDecoration decoration;
    qreal length;
    // TBD: bus and agglomerate
    // TBD: pin vs port
};

class DocumentGraphicsAbstractShapeItem: public DocumentGraphicsItem
{
public:
    explicit DocumentGraphicsAbstractShapeItem();
    virtual ~DocumentGraphicsAbstractShapeItem();

    virtual void fromJson(const QJsonObject &obj);
    virtual void toJson(QJsonObject &obj) const;

    QPen pen;
    QBrush brush;
};

class DocumentGraphicsRectangleItem: public DocumentGraphicsAbstractShapeItem
{
public:
    explicit DocumentGraphicsRectangleItem();
    virtual ~DocumentGraphicsRectangleItem();

    virtual void fromJson(const QJsonObject &obj);
    virtual void toJson(QJsonObject &obj) const;

    QPointF topLeftCorner;
    qreal width;
    qreal height;

};

#endif // DOCUMENTGRAPHICSITEM_H
