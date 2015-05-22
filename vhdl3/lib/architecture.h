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
    Component(const QString &name = QString()):
        name(name)
    {}

    QString name;
    QMap<QString, QString> portMap;
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

    QString body;
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
