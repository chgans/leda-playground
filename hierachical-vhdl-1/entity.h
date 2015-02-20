#ifndef ENTITY_H
#define ENTITY_H

#include <QString>
#include <QVariantMap>

/*
 * VHDL:
 *  bit, bit vector,
 *  bool, bool vector,
 *  integer, int vector,
 *  natural, positive,
 *  char, string, time (duration)
 *  array of
 *  custom using range, array, record
 * Gnu radio:
 *  complex, float, int, long, byte, string
 *  vector of
 * Altium Sch:
 *  signal, bus, harness
*/

enum PortType
{
    InputPort,
    OutputPort,
    InputOutputPort,
    PassivePort
};

// TBD: add clone() and operator ==() here
class IPackageItem
{
public:
    explicit IPackageItem() {}
    virtual ~IPackageItem() {}

    enum ItemType {
        SignleSignalSpecItem = 0,
        ArrayleSignalSpecItem,
        AggegateSignalSpecItem,
        PortSpecItem,
        EntityItem,
        PackageItem,
        ItemListItem,
        PropertyMapItem
    };
    virtual ItemType itemType() const = 0;
    // get rid of that, should use data() or childItem(row, column)?
    virtual IPackageItem *childItem(int row) = 0;
    // childItemCount(int row, int column)?
    virtual int childItemCount() const = 0;
    // rowForChildItem(const IPackageItem *item)?
    // columnForChildItem(const IPackageItem *item)?
    virtual int indexOfChildItem(const IPackageItem *item) const = 0;
    // OK
    virtual IPackageItem *parentItem() = 0;
    // itemRow() and intemColumn()
    virtual int itemRow() const = 0;
    // OK
    virtual int itemColumnCount() const = 0;
    // OK
    virtual int itemRowCount() const = 0;
    // OK
    virtual QVariant data(int row, int column) const = 0;
};

class IPackageItemList: public IPackageItem
{
public:
    virtual ItemType itemListType() const = 0;
};

template <typename I, IPackageItem::ItemType T>
class PackageItemList: public IPackageItemList
{
public:
    ItemType itemType() const {
        return IPackageItem::ItemListItem;
    }

    ItemType itemListType() const {
        return T;
    }


    IPackageItem *childItem(int row) {
        if (row > 0 && row < m_children.size())
            return m_children.value(row);
    }

    int childItemCount() const {
        return m_children.count();
    }

    int itemColumnCount() const {
        return 1;
    }

    int itemRowCount() const {
        return 0;
    }

    int indexOfChildItem(const IPackageItem *item) const {
        return m_children.indexOf(item);
    }

    IPackageItem *parentItem() {
        return m_parent;
    }

    int itemRow() const {
        if (m_parent)
            return m_parent->indexOfChildItem(const_cast<IPackageItem *>(this));
        return 0;
    }

    virtual bool addItem(I *item) {
        m_children.append(item);
        return true;
    }

    virtual bool removeItem(const I *item) {
        if (m_children.contains(item)) {
            m_children.removeOne(item);
            return true;
        }
        return false;
    }

    QVariant data(int row, int column) const
    {
        if (row == 0 && column == 0)
            return QString("[%1 items]").arg(childItemCount());
        return QVariant();
    }

private:
    IPackageItem *m_parent;
    QList<I *> m_children;
};


class PropertyMap: public IPackageItem
{
public:

    ItemType itemType() const
    {
        return IPackageItem::PropertyMapItem;
    }

    IPackageItem *childItem(int row)
    {
        Q_UNUSED(row);
        return nullptr;
    }

    int childItemCount() const
    {
        return 0;
    }

    int indexOfChildItem(const IPackageItem *item) const
    {
        Q_UNUSED(item);
        return 0;
    }

    IPackageItem *parentItem()
    {
        return m_parent;
    }

    int itemRow() const
    {
        if (m_parent)
            return m_parent->indexOfChildItem(static_cast<const IPackageItem *>(this));
        return 0;
    }

    int itemColumnCount() const
    {
        return 3;
    }

    int itemRowCount() const
    {
        return m_properties.size();
    }

    QVariant data(int row, int column) const
    {
        switch (column) {
        case 0: // name
            return m_names.value(const_cast<QVariant *>(&m_properties.at(row)));
        case 1: // value
            return m_properties.at(row);
        case 2: // type
            return m_properties.at(row).type();
        default:
            return QVariant();
        }
    }

private:
    IPackageItem *m_parent;
    QList<QVariant> m_properties;
    QMap<QVariant*, QString> m_names;
};

#if 0
class PortSpec;
class BaseSignalSpec: public IPackageItem
{
public:
    virtual ~BaseSignalSpec();

    enum SignalSpecType {
        Base = 0,
        Single,
        Array,
        Aggregate
    };
    virtual int type() const;
    virtual BaseSignalSpec *clone() const;
    QString name() const;
    void setName(const QString &name);
    PortSpec *portSpec();
    void setPortSpec(PortSpec *port);
    bool operator ==(const BaseSignalSpec &other) const;
    bool operator !=(const BaseSignalSpec &other) const;

protected:
    explicit BaseSignalSpec(const QString &name);
    BaseSignalSpec(const BaseSignalSpec &other);
    virtual bool equalTo(const BaseSignalSpec *other) const;

private:
    QString m_name;
    PortSpec *m_port;
};

class SingleSignalSpec: public BaseSignalSpec
{
public:
    explicit SingleSignalSpec(const QString &name = QString());
    SingleSignalSpec(const SingleSignalSpec &other);
    ~SingleSignalSpec();

    int type() const;
    BaseSignalSpec *clone() const;

    virtual ItemType itemType() const {
        return IPackageItem::SignleSignalSpecItem;
    }

protected:
    bool equalTo(const BaseSignalSpec *other) const;
};


class ArraySignalSpec: public BaseSignalSpec
{
public:
    explicit ArraySignalSpec(const QString &name = QString(),
                             int size = 0);
    ArraySignalSpec(const ArraySignalSpec &other);
    ~ArraySignalSpec();

    int type() const;
    BaseSignalSpec *clone() const;
    void setSize(int size);
    int size() const;

    virtual ItemType itemType() const {
        return IPackageItem::ArrayleSignalSpecItem;
    }

protected:
    bool equalTo(const BaseSignalSpec *other) const;

private:
    int m_size;
};

class AggregateSignalSpec: public BaseSignalSpec
{
public:
    explicit AggregateSignalSpec(const QString &name = QString());
    AggregateSignalSpec(const AggregateSignalSpec &other);
    ~AggregateSignalSpec();

    int type() const;
    BaseSignalSpec *clone() const;
    int size() const;
    const BaseSignalSpec *signal(int index) const;
    const BaseSignalSpec *signal(const QString &name) const;
    bool addSignal(BaseSignalSpec *spec);
    bool removeSignal(int index);
    bool removeSignal(const QString &name);

    virtual ItemType itemType() const {
        return IPackageItem::AggegateSignalSpecItem;
    }

protected:
    virtual bool equalTo(const BaseSignalSpec *other) const;

private:
    QList<BaseSignalSpec*> m_signals;
    QMap<QString, BaseSignalSpec*> m_signalMap;
};

class Entity;
class PortSpec: public IPackageItem
{
public:
    explicit PortSpec(const QString &name = QString(),
                      PortType type = PassivePort,
                      BaseSignalSpec *spec = 0);
    PortSpec(const PortSpec &other);
    ~PortSpec();

    PortSpec *clone() const;
    QString name() const;
    void setName(const QString &name);
    PortType portType() const;
    void setPortType(PortType type);
    const BaseSignalSpec *signalSpec() const;
    void setSignalSpec(BaseSignalSpec *spec);
    Entity *entity();
    void setEntity(Entity *entity);

    bool operator ==(const PortSpec &other) const;
    bool operator !=(const PortSpec &other) const;

    virtual ItemType itemType() const {
        return IPackageItem::PortSpecItem;
    }

private:
    QString m_name;
    PortType m_portType;
    BaseSignalSpec *m_signalSpec;
    Entity *m_entity;
};
typedef PackageItemList<PortSpec, IPackageItem::PortSpecItem> PortSpecList;
#endif

class Package;
class Entity: public IPackageItem
{
public:
    explicit Entity(const QString &name);
    Entity(const Entity &other);
    ~Entity();

    Entity *clone() const;

    QString name() const;
    void setName(const QString &name);
    //QList<PortSpec *> ports();
    //void setPorts(const QList<PortSpec*> &ports);
    Package *package();
    void setPackage(Package *package);
    bool operator ==(const Entity &other) const;
    bool operator !=(const Entity &other) const;

    // From IPackageItem
    virtual ItemType itemType() const;
    virtual IPackageItem *childItem(int row) = 0;
    virtual int childItemCount() const = 0;
    virtual int indexOfChildItem(const IPackageItem *item) const = 0;
    virtual IPackageItem *parentItem() = 0;
    virtual int itemRow() const = 0;
    virtual int itemColumnCount() const = 0;
    virtual int itemRowCount() const = 0;
    virtual QVariant data(int row, int column) const = 0;

private:
    QString m_name;
    PropertyMap m_generics;
    PropertyMap m_constants;
    //PortSpecList m_ports;
    Package *m_package;
};
typedef PackageItemList<Entity, IPackageItem::EntityItem> EntityList;


// TODO: dependencies/imports
class Package: public IPackageItem
{
public:
    explicit Package();
    ~Package();

    QString name() const;
    void setName(const QString &name);
    QVariantMap generics();
    void setGenerics(const QVariantMap &generics);
    QVariantMap constants();
    void setContants(const QVariantMap &constants);
    QList<Entity *> entities();
    void setEntities(const QList<Entity *> &entities);

    // From IPackageItem
    virtual ItemType itemType() const;
    virtual IPackageItem *childItem(int row);
    virtual int childItemCount() const;
    virtual int indexOfChildItem(const IPackageItem *item) const;
    virtual IPackageItem *parentItem();
    virtual int itemRow() const;
    virtual int itemColumnCount() const;
    virtual int itemRowCount() const;
    virtual QVariant data(int row, int column) const;

private:
    QString m_name;
    PropertyMap m_generics;
    PropertyMap m_constants;
    EntityList m_entities;
};

#include <QAbstractItemModel>

class PackageModel: public QAbstractItemModel
{
public:
    PackageModel(QObject *parent);
    ~PackageModel();

    void setPackage(Package *package);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    Package *m_package;
};

#endif // ENTITY_H
