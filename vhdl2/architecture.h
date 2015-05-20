#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#include <QString>
#include <QList>
#include <QMap>
#include <QMetaType>
#include <QVariant>

struct Signal
{
    Signal(const QString &name = QString(),
           QMetaType::Type type = QMetaType::UnknownType):
        name(name),
        type(type)
    {}

    QString name;
    QMetaType::Type type;
};

struct Component
{
    Component(const QString &name = QString(),
              const QString &entityName = QString(),
              const QString &entityArchitectureName = QString()):
        name(name),
        entityName(entityName),
        entityArchitectureName(entityArchitectureName)
    {}

    QString name;
    QString entityName;

    // TODO: Below properties can come from a configuration
    //  => Should we enforce this?
    QString entityArchitectureName;
    QMap<QString, QString> portMap;
    // TODO: QString: allow for unit, unit prefix and parametric-ish definition
    QMap<QString, QVariant> genericMap;
};

struct Architecture
{
    Architecture(const QString &name = QString(),
                 const QString &entityName = QString()):
        name(name),
        entityName(entityName)
    {}

    QString name;
    QString entityName;
};

struct StructuralArchitecture : public Architecture
{
    StructuralArchitecture(const QString &name = QString(),
                           const QString &entityName = QString()):
        Architecture(name, entityName)
    {}

    QList<Signal *> signalList; // TODO: can have a value
    QList<Component *> componentList;
    QMap<QString, QString> portMap;
    // TBD: signal, generic, constant, ...
};

// TBD: Typ, Qucs, Spice, ...
struct BehaviouralArchitecture : public Architecture
{
    BehaviouralArchitecture(const QString &name = QString(),
                           const QString &entityName = QString()):
        Architecture(name, entityName)
    {}

    // Netlist template?
    QString statement;
};

// Drawing/graphics in schematics editors
struct SchematicsArchitecture : public Architecture
{
    SchematicsArchitecture(const QString &name = QString(),
                           const QString &entityName = QString()):
        Architecture(name, entityName)
    {}
};

// Drawing/graphics in layout editors
struct LayoutArchitecture : public Architecture
{
    LayoutArchitecture(const QString &name = QString(),
                           const QString &entityName = QString()):
        Architecture(name, entityName)
    {}
};

#endif // ARCHITECTURE_H
