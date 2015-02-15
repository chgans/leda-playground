#ifndef DOCUMENTGRAPHICSITEM_H
#define DOCUMENTGRAPHICSITEM_H

#include <QPointF>
#include <QPen>
#include <QBrush>

#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QVariant>
#include <leda.h>

// SymbolCollectionTableViewModel
// SymbolPinTableViewModel
// SymbolModelTableViewModel
// SymbolParameterTableViewModel
// SymbolGraphicsViewModel

class DocumentItem
{
public:
    explicit DocumentItem(DocumentItem *parent,
                          const QString &name = QString(),
                          const QVariant &value = QVariant());
    virtual ~DocumentItem();

    QString name() const {
        return m_name;
    }
    void setName(const QString &name) {
        m_name = name;
    }

    QVariant value() const {
        return m_value;
    }
    void setValue(const QVariant &value) {
        m_value = value;
    }

    int childItemCount() const {
        return m_children.size();
    }
    DocumentItem *parentItem() {
        return m_parent;
    }

    DocumentItem *childItem(int index) {
        return m_children.value(index);
    }

    int childIndex() const {
        return m_parent->m_children.indexOf(const_cast<DocumentItem*>(this));
    }

protected:
    DocumentItem *m_parent;
    QList<DocumentItem *> m_children;
    QString m_name;
    QVariant m_value;
};

template <class T>
class DocumentItemList: public DocumentItem, public QList<T*>
{
public:
    explicit DocumentItemList(DocumentItem *parent, const QString &name = QString()):
    DocumentItem(parent, name)
    {}

    virtual ~DocumentItemList() {}

private:
#if 0
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
#endif
};

class DocumentParameterItem: public DocumentItem
{
public:
    explicit DocumentParameterItem(DocumentItem *parent = 0);
    virtual ~DocumentParameterItem();

     bool isVisible() const {
         return m_visible->value().toBool();
     }
     void setVisibled(bool visible) {
         m_visible->setValue(visible);
     }
     const QString paramName() const {
         return m_name->value().toString();
     }
     void setParamName(const QString &name) {
         m_name->setValue(name);
     }
     QVariant paramValue() const {
         return m_value->value();
     }
     void setParamValue(const QVariant &value) {
         m_value->setValue(value);
     }

private:
    DocumentItem *m_visible;
    DocumentItem *m_name;
    DocumentItem *m_value;
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

#endif

// Like VHDL's entity
class DocumentSymbolItem: public DocumentItem
{
public:
    explicit DocumentSymbolItem(DocumentItem *parent);
    virtual ~DocumentSymbolItem();

    QString symbolName() const {
        return m_symbolName->value().toString();
    }
    void setSymbolName(const QString &name) {
        m_symbolName->setValue(name);
    }
    QString description() const {
        return m_description->value().toString();
    }
    void setDescription(const QString &name) {
        m_description->setValue(name);
    }
    QString designator() const {
        return m_designator->value().toString();
    }
    void setDesignator(const QString &name) {
        m_designator->setValue(name);
    }
    
private:
    DocumentItem *m_symbolName;
    DocumentItem *m_description;
    DocumentItem *m_designator;

    DocumentParameterItemList *m_parameters;
    //DocumentPortItemList ports;
    //QList<DocumentModelItem *> models; // TBD: port mapping
    //DocumentGraphicsItemList drawingItems;
};
typedef DocumentItemList<DocumentSymbolItem> DocumentSymbolItemList;

#if 0
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
#endif

#endif // DOCUMENTGRAPHICSITEM_H
