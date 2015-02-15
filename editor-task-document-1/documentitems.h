#ifndef DOCUMENTGRAPHICSITEM_H
#define DOCUMENTGRAPHICSITEM_H

#include <QPointF>
#include <QPen>
#include <QBrush>

#include <QJsonObject>
#include <QJsonArray>
#include <QList>

#include <leda.h>

// SymbolCollectionTableViewModel
// SymbolPinTableViewModel
// SymbolModelTableViewModel
// SymbolParameterTableViewModel
// SymbolGraphicsViewModel

class DocumentItem
{
public:
    explicit DocumentItem();
    virtual ~DocumentItem();

    virtual void fromJsonValue(const QJsonValue &val);
    virtual void toJsonValue(QJsonValue &val) const;

    // propertyCount() rows w/ 3 columns: type, name, value
    int propertyCount() const;
    QVariant property(int index) const;
    void setProperty(int index, const QVariant &val);

    int childItemCount() const;
    DocumentItem *parentItem();
    DocumentItem *childItem(int index);
    int childIndex() const;

protected:
};

template <class T>
class DocumentItemList: public DocumentItem, public QList<T*>
{
public:
    explicit DocumentItemList() {}
    virtual ~DocumentItemList() {}

    virtual void fromJsonValue(const QJsonValue &val) {
        DocumentItem::fromJsonValue(val);
        QJsonArray valList = val.toArray();
        foreach (QJsonValue val, valList) {
            T *item = static_cast<T*>(createItem(val));
            this->append(item);
        }
    }

    virtual void toJsonValue(QJsonValue &val) const {
        DocumentItem::toJsonValue(val);
        QJsonArray valList;
        foreach (T *item, *this) {
            QJsonValue itemVal;
            item->toJsonValue(itemVal);
            valList.append(itemVal);
        }
        val = QJsonValue(valList);
    }

    virtual DocumentItem *createItem(const QJsonValue &val) const
    {
        T *item = new T();
        item->fromJsonValue(val);
        return item;
    }
};

class DocumentParameterItem: public DocumentItem
{
public:
    explicit DocumentParameterItem();
    virtual ~DocumentParameterItem();

    virtual void fromJsonValue(const QJsonValue &val);
    virtual void toJsonValue(QJsonValue &val) const;

    bool visible;
    QString name;
    QString value;
};

typedef DocumentItemList<DocumentParameterItem> DocumentParameterItemList;

#if 0
// Like VHDL's architecture
class DocumentModelItem: public DocumentItem
{
public:
    explicit DocumentModelItem();
    virtual ~DocumentModelItem();

    virtual void fromJsonValue(const QJsonValue &val);
    virtual void toJsonValue(QJsonValue &val) const;

};
#endif

// Like VHDL's ports
class DocumentPortItem: public DocumentItem
{
public:
    explicit DocumentPortItem();
    virtual ~DocumentPortItem();

    virtual void fromJsonValue(const QJsonValue &val);
    virtual void toJsonValue(QJsonValue &val) const;

    QString label;
    QString designator;
    Le::ElectricalPortType electricalType;
};
typedef DocumentItemList<DocumentPortItem> DocumentPortItemList;




class DocumentGraphicsItem: public DocumentItem
{
public:
    explicit DocumentGraphicsItem();
    virtual ~DocumentGraphicsItem();

    virtual void fromJsonValue(const QJsonValue &val);
    virtual void toJsonValue(QJsonValue &val) const;

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
DocumentGraphicsItem *createGraphicsItem(const QJsonValue &val);

class DocumentGraphicsItemList: public DocumentItemList<DocumentGraphicsItem>
{
public:
    DocumentItem *createItem(const QJsonValue &val) const
    {
        return createGraphicsItem(val);
    }
};


// Like VHDL's entity
class DocumentSymbolItem: public DocumentItem
{
public:
    explicit DocumentSymbolItem();
    virtual ~DocumentSymbolItem();

    virtual void fromJsonValue(const QJsonValue &val);
    virtual void toJsonValue(QJsonValue &val) const;

    QString name;
    QString description;
    QString designator;

    DocumentParameterItemList parameters;
    DocumentPortItemList ports;
    //QList<DocumentModelItem *> models; // TBD: port mapping
    DocumentGraphicsItemList drawingItems;
};
typedef DocumentItemList<DocumentSymbolItem> DocumentSymbolItemList;

class DocumentSymbolCollectionItem: public DocumentItem
{
public:
    explicit DocumentSymbolCollectionItem();
    virtual ~DocumentSymbolCollectionItem();

    virtual void fromJsonValue(const QJsonValue &val);
    virtual void toJsonValue(QJsonValue &val) const;

    QString creator;
    QString license;
    DocumentParameterItemList parameters;
    DocumentSymbolItemList symbols;
};

class DocumentGraphicsItemGroup: public DocumentGraphicsItem
{
public:
    explicit DocumentGraphicsItemGroup() {}
    virtual ~DocumentGraphicsItemGroup() {}

    virtual void fromJsonValue(const QJsonValue &val);
    virtual void toJsonValue(QJsonValue &val) const;

    DocumentGraphicsItemList items;
};

class DocumentGraphicsParameterItem: public DocumentGraphicsItem
{
public:
    explicit DocumentGraphicsParameterItem();
    virtual ~DocumentGraphicsParameterItem();

    virtual void fromJsonValue(const QJsonValue &val);
    virtual void toJsonValue(QJsonValue &val) const;

    QString name;
    QString value;
    bool showName;
    bool showValue;
};

// TODO: only graphical data, needs a ref to DocumentPortItem
class DocumentGraphicsPinItem: DocumentGraphicsItem
{
public:
    explicit DocumentGraphicsPinItem();
    virtual ~DocumentGraphicsPinItem();

    virtual void fromJsonValue(const QJsonValue &val);
    virtual void toJsonValue(QJsonValue &val) const;

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

    virtual void fromJsonValue(const QJsonValue &val);
    virtual void toJsonValue(QJsonValue &val) const;

    QPen pen;
    QBrush brush;
};

class DocumentGraphicsRectangleItem: public DocumentGraphicsAbstractShapeItem
{
public:
    explicit DocumentGraphicsRectangleItem();
    virtual ~DocumentGraphicsRectangleItem();

    virtual void fromJsonValue(const QJsonValue &val);
    virtual void toJsonValue(QJsonValue &val) const;

    QPointF topLeftCorner;
    qreal width;
    qreal height;

};

#endif // DOCUMENTGRAPHICSITEM_H
