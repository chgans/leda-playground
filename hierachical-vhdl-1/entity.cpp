#include "entity.h"
#if 0
BaseSignalSpec::~BaseSignalSpec()
{

}

QString BaseSignalSpec::name() const
{
    return m_name;
}

void BaseSignalSpec::setName(const QString &name)
{
    m_name = name;
}

PortSpec *BaseSignalSpec::portSpec()
{
    return m_port;
}

void BaseSignalSpec::setPortSpec(PortSpec *port)
{
    m_port = port;
}

int BaseSignalSpec::type() const
{
    return Base;
}

BaseSignalSpec *BaseSignalSpec::clone() const
{
    return 0;
}

BaseSignalSpec::BaseSignalSpec(const QString &name):
    m_name(name), m_port(nullptr)
{

}

BaseSignalSpec::BaseSignalSpec(const BaseSignalSpec &other):
    m_name(other.m_name), m_port(other.m_port)
{

}

bool BaseSignalSpec::equalTo(const BaseSignalSpec *other) const
{
    return m_name == other->m_name;
}

bool BaseSignalSpec::operator ==(const BaseSignalSpec &other) const
{
    return type() == other.type() && equalTo(&other);
}

bool BaseSignalSpec::operator !=(const BaseSignalSpec &other) const
{
    return !operator ==(other);
}


SingleSignalSpec::SingleSignalSpec(const QString &name):
    BaseSignalSpec(name)
{

}

SingleSignalSpec::SingleSignalSpec(const SingleSignalSpec &other):
    BaseSignalSpec(other)
{

}

SingleSignalSpec::~SingleSignalSpec()
{

}

int SingleSignalSpec::type() const
{
    return Single;
}

BaseSignalSpec *SingleSignalSpec::clone() const
{
    return new SingleSignalSpec(*this);
}

bool SingleSignalSpec::equalTo(const BaseSignalSpec *other) const
{
    const SingleSignalSpec *sig = static_cast<const SingleSignalSpec*>(other);
    return BaseSignalSpec::equalTo(sig);
}



ArraySignalSpec::ArraySignalSpec(const QString &name, int size):
    BaseSignalSpec(name), m_size(size)
{

}

ArraySignalSpec::ArraySignalSpec(const ArraySignalSpec &other):
    BaseSignalSpec(other), m_size(other.m_size)
{

}

ArraySignalSpec::~ArraySignalSpec()
{

}

int ArraySignalSpec::type() const
{
    return BaseSignalSpec::Array;
}

BaseSignalSpec *ArraySignalSpec::clone() const
{
    return new ArraySignalSpec(*this);
}

void ArraySignalSpec::setSize(int size)
{
    m_size = size;
}

int ArraySignalSpec::size() const
{
    return m_size;
}

bool ArraySignalSpec::equalTo(const BaseSignalSpec *other) const
{
    const ArraySignalSpec *sig = static_cast<const ArraySignalSpec*>(other);
    return BaseSignalSpec::equalTo(other) && m_size == sig->m_size;
}


AggregateSignalSpec::AggregateSignalSpec(const QString &name):
    BaseSignalSpec(name)
{

}

// TBD: child->setPortSpec(m_port)
AggregateSignalSpec::AggregateSignalSpec(const AggregateSignalSpec &other):
    BaseSignalSpec(other)
{
    foreach (BaseSignalSpec *otherSpec, other.m_signals) {
        BaseSignalSpec *spec = otherSpec->clone();
        m_signals.append(spec);
        m_signalMap.insert(spec->name(), spec);
    }
}

AggregateSignalSpec::~AggregateSignalSpec()
{
    qDeleteAll(m_signals);
}

int AggregateSignalSpec::type() const
{
    return BaseSignalSpec::Aggregate;
}

BaseSignalSpec *AggregateSignalSpec::clone() const
{
    return new AggregateSignalSpec(*this);
}

int AggregateSignalSpec::size() const
{
    return m_signals.size();
}

const BaseSignalSpec *AggregateSignalSpec::signal(int index) const
{
    return m_signals.value(index, nullptr);

}

const BaseSignalSpec *AggregateSignalSpec::signal(const QString &name) const
{
    return m_signalMap.value(name, nullptr);
}

bool AggregateSignalSpec::addSignal(BaseSignalSpec *spec)
{
    if (!spec || m_signalMap.contains(spec->name()))
        return false;
    m_signals.append(spec);
    m_signalMap.insert(spec->name(), spec);
    return true;
}

bool AggregateSignalSpec::removeSignal(int index)
{
    if (index < size()) {
        BaseSignalSpec *spec = m_signals.value(index);
        m_signalMap.remove(spec->name());
        m_signals.removeAt(index);
        delete spec;
        return true;
    }
    return false;
}

bool AggregateSignalSpec::removeSignal(const QString &name)
{
    if (!m_signalMap.contains(name))
        return false;
    BaseSignalSpec *spec = m_signalMap.value(name);
    m_signalMap.remove(name);
    m_signals.removeOne(spec);
    return true;
}

bool AggregateSignalSpec::equalTo(const BaseSignalSpec *other) const
{
    const AggregateSignalSpec *sig = static_cast<const AggregateSignalSpec*>(other);
    if (!BaseSignalSpec::equalTo(other) || m_signals.size() != sig->size())
        return false;
    for (int i=0; i<m_signals.size(); i++) {
        if ((*m_signals.value(i)) != (*sig->m_signals.value(i)))
            return false;
    }
    return true;
}


PortSpec::PortSpec(const QString &name, PortType type, BaseSignalSpec *spec):
    m_name(name), m_portType(type), m_signalSpec(spec), m_entity(0)
{

}

PortSpec::PortSpec(const PortSpec &other):
    m_name(other.name()), m_portType(other.m_portType),
    m_signalSpec(other.m_signalSpec->clone()), m_entity(other.m_entity)
{

}

PortSpec::~PortSpec()
{
    delete(m_signalSpec);
}

PortSpec *PortSpec::clone() const
{
    return new PortSpec(*this);
}

QString PortSpec::name() const
{
    return m_name;
}

void PortSpec::setName(const QString &name)
{
    m_name = name;
}

PortType PortSpec::portType() const
{
    return m_portType;
}

void PortSpec::setPortType(PortType type)
{
    m_portType = type;
}

const BaseSignalSpec *PortSpec::signalSpec() const
{
    return m_signalSpec;
}

void PortSpec::setSignalSpec(BaseSignalSpec *spec)
{
    m_signalSpec = spec;
}

Entity *PortSpec::entity()
{
    return m_entity;
}

void PortSpec::setEntity(Entity *entity)
{
    m_entity = entity;
}

bool PortSpec::operator ==(const PortSpec &other) const
{
    return m_name == other.m_name &&
            m_portType == other.m_portType &&
            (*m_signalSpec) == (*other.m_signalSpec);
}

bool PortSpec::operator !=(const PortSpec &other) const
{
    return !operator ==(other);
}
#endif


Entity::Entity(const QString &name):
    m_name(name), m_package(nullptr)
{

}

Entity::Entity(const Entity &other):
    m_name(other.m_name), m_generics(other.m_generics), m_constants(other.m_constants),
    m_package(other.m_package)
{
//    foreach (PortSpec *spec, other.m_ports) {
//        m_ports.append(spec->clone());
//    }
}

Entity::~Entity()
{
    //qDeleteAll(m_ports);
}

Entity *Entity::clone() const
{
    return new Entity(*this);
}

QString Entity::name() const
{
    return m_name;
}

void Entity::setName(const QString &name)
{
    m_name = name;
}

//QList<PortSpec *> Entity::ports()
//{
//    return m_ports;
//}

//void Entity::setPorts(const QList<PortSpec *> &ports)
//{
//    qDeleteAll(m_ports);
//    m_ports = ports;
//}

Package *Entity::package()
{
    return m_package;
}

void Entity::setPackage(Package *package)
{
    m_package = package;
}

bool Entity::operator ==(const Entity &other) const
{
    if (!(m_name == other.m_name && m_generics == other.m_generics &&
          m_constants == other.m_constants /*&& m_ports.size() == other.m_ports.size()*/ &&
          m_package == other.m_package))
        return false;
//    for (int i=0; i<m_ports.size(); i++) {
//        if ((*m_ports.value(i)) != (*other.m_ports.value(i)))
//            return false;
//    }
    return true;
}

bool Entity::operator !=(const Entity &other) const
{
    return !operator ==(other);
}

IPackageItem::ItemType Entity::itemType() const
{
    return IPackageItem::EntityItem;
}

int Entity::childItemCount() const
{

}

IPackageItem *Entity::childItem(int row)
{
    switch (row) {
    case 1:
        return m_constants;
    case 2:
        return m_generics;
    default:
        return nullptr;
    }
}



Package::Package()
{

}

Package::~Package()
{
    qDeleteAll(m_entities);
}

QString Package::name() const
{
    return m_name;
}

void Package::setName(const QString &name)
{
    m_name = name;
}

QVariantMap Package::generics()
{
    return m_generics;
}

void Package::setGenerics(const QVariantMap &generics)
{
    m_generics = generics;
}

QVariantMap Package::constants()
{
    return m_constants;
}

void Package::setContants(const QVariantMap &constants)
{
    m_constants = constants;
}

QList<Entity *> Package::entities()
{
    return m_entities;
}

void Package::setEntities(const QList<Entity *> &entities)
{
    qDeleteAll(m_entities);
    m_entities = entities;
}

IPackageItem::ItemType Package::itemType() const
{
    return IPackageItem::PackageItem;
}

IPackageItem *Package::childItem(int row)
{
    switch (row) {
    case 0:
        return m_name;
    case 1:
        return m_constants;
    case 2:
        return m_generics;
    case 3:
        return m_entities;
    default:
        return nullptr;
    }
}

int Package::childItemCount() const
{
    return 3;
}

int Package::indexOfChildItem(const IPackageItem *item) const
{
    if (item == static_cast<IPackageItem *>(m_constants))
        return 0;
    if (item == static_cast<IPackageItem *>(m_generics))
        return 1;
    if (item == static_cast<IPackageItem *>(m_entities))
        return 2;
}

IPackageItem *Package::parentItem()
{
    return nullptr;
}

int Package::itemRow() const
{
    if (m_parent)
        return m_parent->indexOfChildItem(static_cast<const IPackageItem *>(this));
    return 0;
}

int Package::itemColumnCount() const
{
    return 1;
}

int Package::itemRowCount() const
{
    return 4;
}

QVariant Package::data(int row, int column) const
{
    if (column != 0)
        return QVariant();
    switch (row) {
    case 0:
        return m_name;
    case 1:
        return "Constants";
    case 2:
        return "Generics";
    case 3:
        return "Entities";
    }
}

PackageModel::PackageModel(QObject *parent):
    QAbstractItemModel(parent), m_package(nullptr)
{

}

PackageModel::~PackageModel()
{

}

void PackageModel::setPackage(Package *package)
{
    beginResetModel();
    m_package = package;
    endResetModel();
}

int PackageModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    IPackageItem *item;
    if (!parent.isValid())
        item = m_package;
     else
        item = static_cast<IPackageItem*>(parent.internalPointer());

    switch(item->itemType()) {
    case IPackageItem::SignleSignalSpecItem: return 1;
    case IPackageItem::ArrayleSignalSpecItem: return 2;
    case IPackageItem::AggegateSignalSpecItem: return 2;
    case IPackageItem::PortSpecItem: return 3;
    case IPackageItem::EntityItem: return 4;
    case IPackageItem::PackageItem: return 4;
    default: return 0;
    }
}

int PackageModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant PackageModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole
            && section == 0) {
            return QVariant("packages");
    }

    return QVariant();
}

QVariant PackageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.column() > 0)
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    IPackageItem *item = static_cast<IPackageItem*>(index.internalPointer());
    switch(item->itemType()) {
    case IPackageItem::PackageItem:
        Package *p = static_cast<Package*>(item);
        switch(index.row()) {
        case 0: return p->name();
        case 1: return "constants";
        case 2: return "generics";
        case 3: return "entities";
        default: return "not supported!";
        }
    }
    return QVariant();
}

// Returns the index of the item in the model specified by the given row, column and parent index
QModelIndex PackageModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    IPackageItem *parentItem;
    if (!parent.isValid())
        parentItem = m_package;
     else
        parentItem = static_cast<IPackageItem*>(parent.internalPointer());

    IPackageItem *childItem = 0;
    switch(parentItem->itemType()) {
    case IPackageItem::PackageItem: {
        Package *package = static_cast<Package*>(parentItem);
        switch(row) {
        case 0: // name
            childItem = nullptr;
        case 1: // constants
            childItem = nullptr;
        case 2: // generics
            childItem = nullptr;
        case 3: // entities
            childItem = package->entities();
        default:
            childItem = nullptr;
        }
    }
    default:
        break;
    }

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex PackageModel::parent(const QModelIndex &index) const
{
    if (index.isValid() && index.column() != 0)
        return QModelIndex();

    IPackageItem *childItem = static_cast<IPackageItem*>(index.internalPointer());
    IPackageItem *parentItem;
    int row = 0;
    switch(childItem->itemType()) {
    case IPackageItem::EntityItem: {
        Entity *entity = static_cast<Entity*>(childItem);
        parentItem = entity->package();
        row = 3;
        }
        break;
    default:
        parentItem = 0;
    }

    if (!parentItem || parentItem == m_package)
        return createIndex(row, 0, parentItem);

    return QModelIndex();


}

