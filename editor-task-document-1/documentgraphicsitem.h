#ifndef DOCUMENTGRAPHICSITEM_H
#define DOCUMENTGRAPHICSITEM_H

#include <QPointF>
#include <QPen>
#include <QBrush>
#include <QJsonObject>

class DocumentItem
{
public:
    explicit DocumentItem() {}
    virtual ~DocumentItem() {}

    virtual bool fromJSon(const QJsonObject &json);
    virtual QJsonObject toJson() const;
};

class DocumentGraphicsItem: public DocumentItem
{
public:
    explicit DocumentGraphicsItem();
    virtual ~DocumentGraphicsItem();

    virtual bool fromJSon(const QJsonObject &json);
    virtual QJsonObject toJson() const;

    QPointF position;
    qreal zValue;
    qreal rotation;
    bool mirroredX;
    bool mirroredY;
    bool locked;
    bool visible;
    qreal opacity;
};

class DocumentGraphicsItemGroup: public DocumentGraphicsItem
{
public:
    explicit DocumentGraphicsItemGroup() {}
    virtual ~DocumentGraphicsItemGroup() {}

    virtual bool fromJSon(const QJsonObject &json);
    virtual QJsonObject toJson() const;

    QList<DocumentGraphicsItem*> items;
};

class DocumentGraphicsParameterItem: public DocumentGraphicsItem
{
public:
    explicit DocumentGraphicsParameterItem();
    virtual ~DocumentGraphicsParameterItem();

    virtual bool fromJSon(const QJsonObject &json);
    virtual QJsonObject toJson() const;

    QString name;
    QString value;
    bool showName;
    bool showValue;
};

/*
class DocumentGraphicsPinItem: DocumentGraphicsItem
{
public:
    explicit DocumentGraphicsPinItem();
    virtual ~DocumentGraphicsPinItem();

    virtual bool fromJSon(const QJsonObject &json);
    virtual QJsonObject toJson() const;

    QString name;
    QString number;
    bool showName;
    bool showNumber;
    // electricalType
    // decoration
    qreal length;
    // TBD: bus and agglomerate
    // TBD: pin vs port
};

class DocumentGraphicsAbstractShapeItem: DocumentGraphicsItem
{
public:
    explicit DocumentGraphicsAbstractShapeItem();
    virtual ~DocumentGraphicsAbstractShapeItem();

    virtual bool fromJSon(const QJsonObject &json);
    virtual QJsonObject toJson() const;

    QPen pen;
    QBrush brush;
};

class DocumentGraphicsRectangleItem: public DocumentGraphicsAbstractShapeItem
{
public:
    explicit DocumentGraphicsRectangleItem();
    virtual ~DocumentGraphicsRectangleItem();

    virtual bool fromJSon(const QJsonObject &json);
    virtual QJsonObject toJson() const;

    QPointF topLeftCorner;
    qreal width;
    qreal height;

};
*/
#endif // DOCUMENTGRAPHICSITEM_H
