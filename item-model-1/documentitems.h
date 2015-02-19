#ifndef DOCUMENTITEMS_H
#define DOCUMENTITEMS_H

#include <QtGlobal>
#include <QList>
#include <QVariant>

class Item {
public:
    Item *parent;
    QList<Item*> children;
};

template<class T>
class ItemList: public Item {
    QList<T> items;
};

class Property: public Item {
public:
    QVariant value;
};

typedef ItemList<Property*> PropertyList;

QFlag flag;
class Parameter: public Item {
public:
    enum Flag {
        ReadOnly,
    };
    Q_DECLARE_FLAGS(Flags, Flag)
    Property name;
    QVariant value;
    Flags flags;
    int type;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Parameter::Flags)

typedef ItemList<Parameter*> ParameterList;

class Port: public Item {
public:
    QString id;
    QString label;
    QString type;
};

typedef ItemList<Port*> PortList;

class Entity: public Item {
public:
    QString id;
    ParameterList parameters;
    PortList ports;
};

typedef ItemList<Property*> EntityList;

class EntityCollection: public Item {
public:
    QString name;
    QString description;
    QString author;
    QString publisher;
    EntityList entities;
};

#endif // DOCUMENTITEMS_H

